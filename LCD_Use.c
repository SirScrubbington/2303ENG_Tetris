#include "registers.h"

#define CSbit 0x80
#define RSTbit 0x40
#define A0bit 0x20
#define CLKbit 0x20
#define DATbit 0x10 
#define CSport (*((unsigned long *)0x400043FC))
#define RSTport (*((unsigned long *)0x400043FC))
#define A0port (*((unsigned long *)0x400043FC))
#define CLKport (*((unsigned long *)0x400243FC))
#define DATport (*((unsigned long *)0x400243FC))
	
#define GPIOA_DATA	(*((unsigned long *)0x400043FC))

unsigned char LCDmat[1056];

void DelayMs(unsigned int);
void LCD_Cmd_Out(unsigned int);
void LCD_Char_Out(unsigned int);
void LCD_Contrast(unsigned int,unsigned int);
void LCD_Refresh(void);
void LCD_Pixel(unsigned int, unsigned int, unsigned int);
void LCD_Blank(void);

void LCD_Init(){
	
	CSport &=  ~CSbit;
	RSTport &= ~RSTbit;

	DelayMs(50);
	
	RSTport |= RSTbit;
	
	DelayMs(50);	
	
	LCD_Cmd_Out(0xA2);
	LCD_Cmd_Out(0xA0);
	LCD_Cmd_Out(0xC0);	
	LCD_Cmd_Out(0xC8);
	LCD_Cmd_Out(0x20 | 0x3);
	LCD_Cmd_Out(0x28 | 0x4);
	
	DelayMs(50);
	
	LCD_Cmd_Out(0x28 | 0x7);
	
	DelayMs(50);
	
	LCD_Cmd_Out(0x26);
	LCD_Cmd_Out(0x81);
	LCD_Cmd_Out(31);
	LCD_Cmd_Out(0x40);
	LCD_Cmd_Out(0xAF);
	
	CSport |= CSbit;
	
	LCD_Contrast(5,50);	
	LCD_Cmd_Out(0xF0);
	
}

void LCD_Cmd_Out(unsigned int n){
	n &= 0xFF;
	A0port &= ~(A0bit);
	LCD_Char_Out(n);
	
}

void LCD_Dat_Out(unsigned int n){
	n &= ~0xFF;
	A0port |= A0bit;
	LCD_Char_Out(n);	
}

void LCD_Char_Out(unsigned int n){
	CSport &= ~(CSbit);
	
	int i;
	
	for(i=0; i<8;i++){
		unsigned int temp = n & 0x80;
		
		if (temp == 0){
			DATport &= ~(DATbit);
		}
		else {
			DATport |= DATbit;
		}
		
		CLKport |= CLKbit;
		
		CLKport &= ~(CLKbit);
		
		n = n << 1;
	}	
	
	CSport |= CSbit;
	
}

void LCD_Contrast(unsigned int res_ratio, unsigned int contrast){
	if (res_ratio > 7) res_ratio = 7;
	if (contrast > 63) contrast = 63;
	
	res_ratio |= 0x20;
	LCD_Cmd_Out(res_ratio);
	res_ratio = 0x81;
	LCD_Cmd_Out(res_ratio);
	res_ratio = contrast;
	LCD_Cmd_Out(res_ratio);
}

void LCD_Pixel(unsigned int x,unsigned int y,unsigned int colour){
	
	// char LCDmat[1024];
	
	x -=1;
	
	if (y > 32){
		y += 32;
	}
	
	y-=1;
	
	int main = y / 8;
	int rem = main * 8 - y;
	
	int MATPOS = main * 128 + x;
	
	int n = 1;
	
	n = n << rem;
	
	if (colour == 1){
		LCDmat[MATPOS] &= ~n;
	}
	else {
		LCDmat[MATPOS] |= n;
	}
}

void LCD_Refresh(){
	// y = R6, x = R5
	int x,y;
	for (y=0; y<8 ;y++){
		
		int  n = x | 0xB0;
		LCD_Cmd_Out(n);
		n = 0;
		LCD_Cmd_Out(n);
		n = 0x10;
		LCD_Cmd_Out(n);
		
		for (x=0; x<128 ; x++){
			int n2 = y*128 + x;
			LCD_Dat_Out(LCDmat[n2]);
		}
	}
}

void LCD_Blank(){
	int i;
	for(i=0;i<1024;i++){
		LCDmat[i]=0;
	}	
}


void DelayMs(unsigned int numLoops)
{
unsigned int lp;
unsigned int i;

for(lp=0; lp<numLoops; lp++)
	for (i=0; i<=0xFFFF; i++) ;
}
