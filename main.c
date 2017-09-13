#include "registers.h"
#include "tetris.c"
#include "LCD_Use.c"

void dpad_setup(void);
void lcd_setup(void);

int main(){
	dpad_setup();
	lcd_setup();
	LCD_Init();
	while(1){
		//DelayMs(1000);
		//update();
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
	
	NVIC_EN0 |= (1<<17);
		
	__asm("CPSIE I");
}

void ISR_PORTB(){
	 __asm("MOV R11 ,#0xFFFF");
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
