#define X 10
#define Y 20

typedef struct pt {
	int x;
	int y;
} point;

int board[Y][X];

point block[4];

// Set the point position absolute to provided coordinates.
int set_position(int x,int y,point*p){
	block.x = x;
	block.y = y;
	return 0;
}

// Set the point position relative to provided coordinates.
int set_position_relative(int x, int y, point*p){
	block.x += x;
	block.y += y;
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
		
		default:
			return -1;
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
		set_position_relative(1,0,block[i]);
	}
	return 0;
}

// Called when the right button is pressed on the D-Pad
int interrupt_right(){
	int i;
	for (i=0;i<3;i++){
		set_position_relative(-1,0,block[i]);
	}
	return 0;
}

// Called when a row is filled to clear the row and move all the blocks down one space.
int cascade_and_delete(int y){
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
}

// Called every step to update the game state.
int update(){
	
	int contact=0;
	
	for(i=0;i<3;i++){
		if(check_for_collision(block[i].x, block[i].y) or (block[i].y==Y)){
			contact=1;
		}
	}
	
	if(contact){
		// Add player blocks to board
		for(i=0;i<3;i++){
			board[block[i].y[block[i].x]=1;
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
		set_position_relative(0,1,block[i]);
	}
}
