#define X 10
#define Y 20

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

typedef struct pt {
	int x;
	int y;
} point;

int board[Y][X];

point block[4];

// Set the point position absolute to provided coordinates.
int set_position(point*p,int x, int y){
	p->x = x;
	p->y = y;
	return 0;
}

// Set the point position relative to provided coordinates.
int set_position_relative(point*p,int x, int y){
	p->x += x;
	p->y += y;
	return 0;
}

int decide_block_type(int n,point*p[]){
	switch(n){
		
		// Standard Square
		case 0:
			set_position(p[0],0,0);
			set_position(p[1],0,1);
			set_position(p[2],1,0);
			set_position(p[3],1,1);
		
		// 'L' Shape
		case 1:
			set_position(p[0],0,0);
			set_position(p[1],0,1);
			set_position(p[2],0,2);
			set_position(p[3],1,2);
		
		// 4x1 Line
		case 2:
			set_position(p[0],0,0);
			set_position(p[1],0,1);
			set_position(p[2],0,2);
			set_position(p[3],0,3);
		
		// 'Z' Shape
		case 3:
			set_position(p[0],0,0);
			set_position(p[1],0,1);
			set_position(p[2],1,1);
			set_position(p[3],1,2);
		
		// 'T' Shape
		case 4:
			set_position(p[0],0,0);
			set_position(p[1],1,0);
			set_position(p[2],2,0);
			set_position(p[3],1,1);
	}
	
	// Align the blocks to the centre of the board
	set_position_relative(p[0],0,5);
	set_position_relative(p[1],0,5);
	set_position_relative(p[2],0,5);
	set_position_relative(p[3],0,5);
	
	return 0;
}

//Checks for a collision between the given coordinates and the board position.
int check_for_collision(int x, int y){
	if (board[y][x]==1){
		return 1;
	}
	return 0;
}

// Called when the left button is pressed on the D-Pad
int interrupt_left(){
	int i;
	for (i=0;i<3;i++){
		set_position_relative(&block[i],1,0);
	}
	return 0;
}

// Called when the right button is pressed on the D-Pad
int interrupt_right(){
	int i;
	for (i=0;i<3;i++){
		set_position_relative(&block[i],-1,0);
	}
	return 0;
}

int interrupt_restart(){
	int i,j;
	for(i=0;i<Y;i++){
		for(j=0;j<X;j++){
			board[i][j]=0;
		}
	}
	return 0;
}

// Called when a row is filled to clear the row and move all the blocks down one space.
int cascade_and_delete(int y){
	int i,j;
	for(j=0;j<X;j++){
		board[y][j]=0;
	}

	for(i=y;i<Y;i++){
		for(j=0;j<X;j++){
			board[i][j]=board[i-1][j];
		}
	}
	
	for (j=0;j<X;j++){
		board[Y-1][j]=0;
	}
	return 0;
}

// Called every step to update the game state.
int update(){
	
	int i,j;
	
	int contact=0;
	
	for(i=0;i<3;i++){
		if(check_for_collision(block[i].x, block[i].y) || (block[i].y==Y)){
			contact=1;
		}
	}
	
	if(contact){
		// Add player blocks to board
		for(i=0;i<3;i++){
			board[block[i].y][block[i].x]=1;
		}
		
		// Check for complete row
		for(i=0;i<Y;i++){
			int complete=1;
			for(j=0;j<X;j++){
				if(board[i][j]==0) complete=0;
			}
			if(complete) cascade_and_delete(i);
		}
	}
	
	for(i=0;i<3;i++){
		set_position_relative(&block[i],0,1);
	}
	
	LCD_Blank();
	
	for(i=0;i<Y;i++){
		for(j=0;j<X;j++){
			if (board[i][j]==1){
				LCD_Pixel(i,j,1);
			}
		}
	}
	return 0;
}
