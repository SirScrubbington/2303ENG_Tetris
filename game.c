#define X 32
#define Y 16
#define MAX_ENEMIES 12
#define MAX_BULLETS 32

// Structure Definitions
typedef struct e {int x; int y;int exists;} enemy;
typedef struct p {int x;int s;} plr;
typedef struct b {int x;int y;int exists;} bullet;

// Global Variables
int score;
int lives;
int direction=0;
int running=1;

// Global Data Structures
int display[Y][X];

plr player = {X/2,0};
enemy enmlst[MAX_ENEMIES];
bullet bltlst[MAX_BULLETS];
bullet nmyblt[MAX_BULLETS];

// spawn_bullets(): void
// if possible, spawns new bullets for each enemy
void spawn_bullets(){
	int i,j;
	for(i=0;i<MAX_ENEMIES;i++){
		if(enmlst[i].exists==1){
			for(j=0;j<MAX_BULLETS;j++){
				if(nmyblt[j].exists==0){
					nmyblt[j].exists=1;
					nmyblt[j].x = enmlst[i].x;
					nmyblt[j].y = enmlst[i].y-1;
					break;
				}
			}
		}
	}
}

// update_bullets(): void
// updates the y coordinates of each active bullet,
// and cleans up bullets outside of the screen
void update_bullets(){
	int i;
	for(i=0;i<MAX_BULLETS;i++){
		
		if(bltlst[i].exists==1){
			bltlst[i].y++;
			if(bltlst[i].y > Y){
				bltlst[i].exists=0;
			}
		}
		
		if(nmyblt[i].exists==1){
			nmyblt[i].y--;
			if(nmyblt[i].y < 0){
				nmyblt[i].exists=0;
			}
		}
	}
}

// move_enemies(): void
// update the y coordinates of each active enemy object.
void move_enemies(){
	int i;
	for(i=0;i<MAX_ENEMIES;i++){
		if(enmlst[i].exists==1){
			enmlst[i].y--;
			if(enmlst[i].y < 0){
				enmlst[i].exists=0;
			}
		}
	}
}

// start_game(): void
// assign the score and life global variables, 
// initialise the existence of each enemy and bullet object to zero
void start_game(){
	
	score=0;	
	lives=3;
	
	int i;
	
	for(i=0;i<MAX_ENEMIES;i++){
		enmlst[i].exists=0;
	}
	
	for(i=0;i<MAX_BULLETS;i++){
		bltlst[i].exists=0;
		nmyblt[i].exists=0;
	}
}

// spawn_enemy(position): void
// if available, initialise an enemy 
// object at x coordinate 'position'
void spawn_enemy(int position){
	int i;
	for(i=0;i<MAX_ENEMIES;i++){
		if (enmlst[i].exists==0){
			enmlst[i].exists=1;
			enmlst[i].y = Y-1;
			enmlst[i].x = position;
			break;
		}
	}
}

// move_left(): void
// Moves the player object right three spaces.
// Three spaces is chosen to align with the 
// 'move right' interrupt, which is more sensitive.
// Triggered by the interrupt on pin B5.
void move_left(){
	if(player.x > 1){
		player.x-=3;
	}
}

// move_right(): void
// Moves the player object right left three spaces.
// Triggered by the interrupt on pin B7.
void move_right(){
	if(player.x < X-1){
		player.x++;
	}
}

// shoot(): void
// if available, creates a player bullet 
// object at the player's position. 
// Triggered by the interrupt on pin B6.
void shoot(){
	int i;
	for(i=0;i<MAX_BULLETS;i++){
		if(bltlst[i].exists==0){
			bltlst[i].exists=1;
			bltlst[i].x = player.x;
			bltlst[i].y = 2;
			break;
		}
	}
}

// gameover(): void
// if called, the game will end after the next refesh loop.
void gameover(){
	running=0;
}

// 0 = void, 1 = player, 2 = enemy, 3 = bullet
// Refreshes all of the game objects which 
// will be displayed to the screen, and performs
// a number of important logical evalations related
// to collisions.
void game_refresh(){
	
	int i,j;
	
	// Wipe board blank
	for(i=0;i<Y;i++){
		for(j=0;j<X;j++){
			display[i][j]=0;
		}
	}
	
	// For all enemy objects:
	for(i=0;i<MAX_ENEMIES;i++){
		if(enmlst[i].exists == 1){
			// if collision with player is found, lose a life
			// and call gameover if all lives are lost
			if((enmlst[i].y == 1) && (enmlst[i].x == player.x)){
				lives--;
				if(lives < 1){ 
					gameover();
				}
			}
			// add enemy position to screen display
			display[enmlst[i].y][enmlst[i].x]=2;
		}
	}
	
	// For all bullet objects:
	for(i=0;i<MAX_BULLETS;i++){
		// Enemy Bullets:
		if(nmyblt[i].exists==1){
			// If colliding with player, lose a life and 
			// and call gameover if all lives are lost.
			if((nmyblt[i].y == 1) && (nmyblt[i].x == player.x)){
				lives--;
				if(lives < 1){ 
					gameover();
				}
			}
			// add enemy bullet position to screen display
			display[nmyblt[i].y][nmyblt[i].x]=3;
		}
		// Player Bullets:
		if(bltlst[i].exists==1){
			// If colliding with enemy, destroy the bullet and the enemy
			for(j=0;j<MAX_ENEMIES;j++){
				if((bltlst[i].y == enmlst[j].y) && (bltlst[i].x == enmlst[j].x)){
					enmlst[j].exists=0;
					bltlst[i].exists=0;
					score++;
				}
			}
			
			// add player bullet to screen display
			display[bltlst[i].y][bltlst[i].x]=4;
		}
	}
	
	// Add player position to screen display
	display[0][player.x]=1;
}
