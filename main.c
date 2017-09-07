#include "registers.h"
#include "tetris.c"

void dpad_setup(void);
void lcd_setup(void);

int main(){
	dpad_setup();
	lcd_setup();
	while(1){ }
}

void dpad_setup(){
	
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
		
	// Ports to use with GPIO E: E4, E5
		
	int PE = 48;
		
	SYSCTL_RCGCGPIO |= 0xF;
	while((SYSCTL_PRGPIO & 0xF) != 0xF) {};
		
	GPIOE_DIR |= PE;
		
	GPIOE_AFSEL &= PE;
		
	GPIOE_DR8R |= PE;
		
	GPIOE_DEN |= PE;
		
}
