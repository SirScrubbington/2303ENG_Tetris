#include "registers.h"
#include "general.c"
#include "LCD_Use.c"
#include "game.c"

void dpad_setup(void);
void lcd_setup(void);
void uart_setup(void);

void write_char(char);
char read_char(void);
void write_line(int,char*);
void newl(void);
void print_board(void);
char * itoa(int,int);

int main(){
		
	int i;
	
	// Run Setups on Interrupts, LCD and UART
	uart_setup();
	dpad_setup();
	lcd_setup();

	newl();

	// Demonstrate basic LCD test
	LCD_Blank();
	LCD_Init();
	LCD_Blank();
	LCD_Dat_Out(0xFF);
	DelayMs(100);
	LCD_Dat_Out(0x0);
	LCD_Blank();
	
	// Play Game on Terminal
	start_game();
	
	// Game Variables
	int count = 4;
	int waves = 0;
	int apos=16;
	int bpos=15;
	
	// Until Game is lost
	while(running)
	{
		// Every 4 Frames, spawn new enemies
		if (count % 4 == 0){
			waves++;
			spawn_enemy(apos);
			spawn_enemy(bpos);
			
			apos-=2;
			bpos+=2;
			
			if(apos < 0) {
				apos = 16;
			}
			
			if(bpos > 31) {
				bpos = 15;
			}
		}
		
		// Every 2 Frames, update enemy positions
		if(count % 2 == 0){
			move_enemies();
		}
		
		// Every 6 frames, spawn bullets
		if(count % 6 == 0){
			spawn_bullets();
		}
		
		// Update bullet positions every frame
		update_bullets();

		DelayMs(20);
		game_refresh();
		print_board();
		
		count++;
	}
	
	// Once game is over, print score / wave messages and spin
	char msgmvr[10] = "Game Over!";
	
	for(i=0;i<10;i++){
		write_char(msgmvr[i]);
	}
		
	newl();
	
	char msgscr[7] = "Score: ";
	for(i=0;i<7;i++){
		write_char(msgscr[i]);
	}
	
	for(i=0;i<score;i++){
		write_char('|');
	}
	
	newl();
	
	char msgwav[7] = "Waves: ";
	for(i=0;i<7;i++){
		write_char(msgwav[i]);
	}
	
	for(i=0;i<waves;i++){
		write_char('|');
	}
	
	newl();
	
	while(1){};
}

// print_board(): void
// prints the current game board state to the screen
void print_board(){

	int i,j;
	
	// resets the line pointer to the top of the terminal
	write_char('\033');
	write_char('\143');
	
	newl();
	// print board state to terminal
	// 1 : Player
	// 2 : Enemy
	// 3 : Enemy Bullet
	// 4 : Player Bullet
	// Else: Blank Space
	for(i=Y-1;i>=0;i--){
		for(j=X-1;j>=0;j--){
			//write_char(' ');
			if(display[i][j]==1){
				write_char('&');
			}
			else if(display[i][j]==2){
				write_char('V');
			}
			else if(display[i][j]==3){
				write_char('*');
			}
			else if(display[i][j]==4){
				write_char('@');
			}
			else{
				write_char(' ');
			}
		}
		newl();
	}
	
	// Bottom-out the display with underscores
	for(i=0;i<X;i++){
		write_char('_');
	}
		
	newl();
	
	// print the number of remaining lives
	for(i=0;i<lives;i++){
		write_char('<');
		write_char('3');
		write_char(' ');
	}
	
	newl();
	
}

// newl(): void
// Writes a new line to the terminal
void newl(void){
	write_char('\n');
	write_char('\r');
}

// write_char(c): void
// Write a single character to the terminal
void write_char(char c){
	while((UART0_FR & 32)==32){}
	UART0_DR |= c;
}

// read_char(): char
// Read a single character from the terminal
char read_char(){
	while((UART0_FR & 16)==16){}
	return UART0_DR;
}

// uart_setup():
// Configures UART0 setup on Port A Pins 0,1
void uart_setup(){
	// Port A Pins 0,1 (UART0)
	
	unsigned int uart_in = 0x1; // pin 0
	unsigned int uart_out = 0x2; // pin 1
	
	SYSCTL_RCGUART |= 0x1;
	while((SYSCTL_PRUART & 0x1) != 0x1){}
	
	SYSCTL_RCGCGPIO |= 0x1;
	while((SYSCTL_PRGPIO & 0x1)!=0x1){};
		
	UART0_CTL &= ~0x1;
		
	UART0_IBRD = 208;
		
	UART0_FBRD = 21;
		
	// Line Control

  UART0_LCRH |= 0x40;


	UART0_CTL |= 0x20;
	UART0_CTL |= 0x300;
	UART0_CTL |= 0x1;
	
	GPIOA_DIR &= ~uart_in;
	GPIOA_DIR |= uart_out;
		
	GPIOA_AFSEL |= (uart_in + uart_out);
	
	GPIOA_PCTL &= ~0xFF0000;
	GPIOA_PCTL |= 0x110000;	
	
	GPIOA_DEN |= (uart_in + uart_out);
	
}

// pwm_setup(): void
// Incomplete PWM setup which has previously
// Bricked the Tiva Microprocessor, please avoid using.
void pwm_setup(){
	// Port C Pins 0,1 (Clock 4 A)
	int PC = 0x3;
	
	SYSCTL_RCGCGPIO |= 0x4;
	while((SYSCTL_PRGPIO &= 0x4)!=0x4){};
	
	GPIOC_DIR |= PC;
	
	GPIOC_DEN |= PC;
		
	GPIOC_AFSEL |= PC;
		
	GPIOC_PCTL |= PC;
		
	SYSCTL_RCGCTIMER |= 0x16;	
	while((SYSCTL_PRTIMER & 0x16)!=0x16){};
	
	TIMER4_CTL &= 0x1;
		
	TIMER4_CTL &= 64;
		
	TIMER4_CFG = 0x4;
		
	TIMER4_TAMR |= 0x8;
		
	TIMER4_TAMR &= ~0x4;

	TIMER4_TAMR |= 0x2;
		
	TIMER4_TAMR &= 0x1;
		
	TIMER4_TAILR = 0xFA00;
		
	TIMER4_TAMATCH = 0x44800;
		
	TIMER4_CTL |= 0x1;
		
}

// dpad_setup(): void
// Configures GPIO with Interrupts on GPIO Port B pins A5-A7.
void dpad_setup(){
	// Setup GPIO B
	
	// Ports to use with GPIO B: A5,A6,A7
	int PB = 224;
	
	SYSCTL_RCGCGPIO |= 0x2;
	while((SYSCTL_PRGPIO & 0x2)!=0x2){};
	
	GPIOB_DIR &=~PB;
	
	GPIOB_AFSEL &= ~PB;
		
	GPIOB_DEN |= PB;
		
	__asm("CPSID I");
	
	GPIOB_IM &= ~(PB);
	GPIOB_IS &= ~(PB);
	GPIOB_IEV &= ~(PB);
	GPIOB_IEV &= ~(PB);
	GPIOB_IM |= PB;
	
	NVIC_EN0 |= (1<<1);
		
	__asm("CPSIE I");
}

// ISR_PORTB(): void
// Called by the ISR to handle interrupts on Port B.
void ISR_PORTB(){
	if((GPIOB_RIS & 32)==32){
		//interrupt_right();
		//write_char('R');
		move_right();
		GPIOB_ICR |= 32;
	}
	if((GPIOB_RIS & 64)==64){
		//interrupt_left();
		//write_char('L');
		move_left();
		GPIOB_ICR |= 64;
	}
	if((GPIOB_RIS & 128)==128){
		//interrupt_restart();
		//write_char('\n');
		//write_char('\r');
		shoot();
		GPIOB_ICR |= 128;
	}
}

// lcd_setup(): void
// Configures GPIO Port A to interact with an LCD screen on pins A5-A7,
// And configures Port E to interact with the LCD screen on pins E3-E5.
void lcd_setup(){
	
	// Setup GPIO A
	// Ports to use with GPIO A: A5,A6,A7
	
	int PA = 224;
	
	SYSCTL_RCGCGPIO |= 0x1;
	while((SYSCTL_PRGPIO & 0x1)!=0x1){};
	
	GPIOA_DIR |=PA;
	
	GPIOA_AFSEL &= ~PA;
		
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
