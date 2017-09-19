#include "registers.h"
#include "tetris.c"

unsigned char LCDmat[1056];

extern void LCD_Init(void);
extern void LCD_Cmd_Out(unsigned int);
extern void LCD_Dat_Out(unsigned int);
extern void LCD_Char_Out(unsigned int);
extern void LCD_Contrast(unsigned int, unsigned int);
extern void LCD_Pixel(unsigned int, unsigned int, unsigned int);
extern void LCD_Refresh(void);
extern void LCD_Blank(void);
extern void Setup_GPIOA_UART(void);
extern void DelayMs_R0(unsigned int);

extern void GPIO_Setup_A(void);
extern void GPIO_Setup_E(void);

void dpad_setup(void);
void lcd_setup(void);

int main(){
	dpad_setup();
	GPIO_Setup_A();
	GPIO_Setup_E();
	LCD_Blank();
	LCD_Init();
	LCD_Blank();
	LCD_Refresh();
	LCD_Blank();

	while(1)
	{
		WaitForInterrupt();
	}
}

void dpad_setup(){
	// Setup GPIO B
	
	// Ports to use with GPIO B: A5,A6,A7
	int PB = 224;
	
	SYSCTL_RCGCGPIO |= 0x2;
	while((SYSCTL_PRGPIO & 0x2)!=0x2){};
	
	GPIOB_DIR |=PB;
	
	GPIOB_AFSEL |= PB;
		
	GPIOB_DR8R |= PB;
		
	GPIOB_DEN |= PB;
		
	__asm("CPSID I");
	
	GPIOB_IM &= ~(PB);
	GPIOB_IS &= ~(PB);
	GPIOB_IEV &= ~(PB);
	GPIOB_IEV &= ~(PB);
	GPIOB_IM |= PB;
	
	NVIC_EN0 |= (1);
		
	__asm("CPSIE I");
}

void ISR_PORTB(){
	LCD_Dat_Out(0xFFFF);
}

void lcd_setup(){
	
	// Setup GPIO A
	
	// Ports to use with GPIO A: A5,A6,A7
	
	int PA = 224;
	
	SYSCTL_RCGCGPIO |= 0x1;
	while((SYSCTL_PRGPIO & 0x1)!=0x1){};
	
	GPIOA_DIR |=PA;
	
	GPIOA_AFSEL |= PA;
		
	GPIOA_DR8R |= PA;
		
	GPIOA_DEN |= PA;
		
	// Setup GPIO E
		
	// Ports to use with GPIO E: E3, E4, E5
		
	int PE = 56;
		
	SYSCTL_RCGCGPIO |= 16;
	while((SYSCTL_PRGPIO & 16) != 16) {};

	GPIOE_DIR |= PE;
		
	GPIOE_AFSEL &= ~PE;
		
	GPIOE_DR8R |= PE;
		
	GPIOE_DEN |= PE;
		
}
