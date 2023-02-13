#include<curses.h>
#include<stdio.h>
#include<unistd.h>

#define UP 1
#define RIGHT 2
#define DOWN 3
#define LEFT 4

int main(void);

void printName(int x_vec, int y_vec){
	move(x_vec, y_vec);
	addstr(" _____       _   _");
	move(++x_vec, y_vec);
	addstr("|  _  |_____| |_| |_ ___ ___ _ _");
	move(++x_vec, y_vec);
	addstr("|     |   |_   _|   | . |   | | |");
	move(++x_vec, y_vec);
	addstr("|__|__|_|_| |_| |_|_|___|_|_|_  |");
	move(++x_vec, y_vec);
	addstr("                            |___|");
	return;
}

// Parameters tell where to draw the snake
void printSnake(int x_vec, int y_vec){
	move(x_vec, y_vec);
	addstr("             ____");
	move(++x_vec, y_vec);
	addstr("            / . .\\");
	move(++x_vec, y_vec);
	addstr("            \\  ---<");
	move(++x_vec, y_vec);
	addstr("             \\  /");
	move(++x_vec, y_vec);
	addstr("   __________/ /");
	move(++x_vec, y_vec);
	addstr("-=:___________/");
	return;
}

void buildBlock(unsigned short SIZE_H, unsigned short SIZE_V){
	const short unsigned BOX_SIZE_H = SIZE_H;
	const short unsigned BOX_SIZE_V = SIZE_V;
	char BOX_CHAR = '#';

	for (int x_vec = 0; x_vec <= BOX_SIZE_V; ++x_vec){
		for (int y_vec = 0; y_vec <= BOX_SIZE_H; ++y_vec){
		if (x_vec == 0 || x_vec == BOX_SIZE_V){
				move(x_vec, y_vec);
				printw("%c", BOX_CHAR);
			}
			else if (y_vec == 0 || y_vec == BOX_SIZE_H){
				move(x_vec, y_vec);
				printw("%c", BOX_CHAR);
			}
		}
	}
	return;
}

int exitMenu(void){
	int x_vec = 5,
	    y_vec = 10;

	// Print menu
	clear();
	buildBlock(52, 10);
	move(x_vec, y_vec);

	// Ask question
	addstr("You really want to quit? (Y/N)");
	move(++x_vec, y_vec);
	int key = getch();

	if (key == 'Y' || key == 'y')
		return 1;
	else if (key == 'N' || key == 'n')
		return 2;
	else exitMenu();
}

void creditsMenu(void){
	clear();
	buildBlock(54, 20);

	// Print my name
	int x_vec = 6, y_vec = 10;
	move(x_vec, y_vec);
	addstr("Game made by:");
	move(++x_vec, y_vec);
	move(x_vec, y_vec);
	printName(x_vec, y_vec);

	// Print contributors list
	x_vec = 13;
	move(x_vec, y_vec);
	addstr("Contributors:");
	move(++x_vec, y_vec);
	addstr("No one yet :(");
	move(++x_vec, y_vec);
	getch();

	// Return to menu
	main();
	return;
}

void getSnakeDir(int input, int *direction){
	switch(input){
		case 'w':
			*direction = UP;
			break;
		case 's':
			*direction = DOWN;
			break;
		case 'a':
			*direction = LEFT;
			break;
		case 'd':
			*direction = RIGHT;
			break;
	}
	return;
}

int selectOption(){
	// Menu navigation 
	int x_vec = 15;
	int y_vec = 10;
	short int menu_start = 15;
	short int menu_end = 17;
	move(x_vec, y_vec); // Move cursor to first option
	noecho(); // Avoid overwritting in screen
	
	while(true){
		int key = getch();
		switch(key){
			case 'w':
				move(--x_vec, y_vec);
				if (x_vec < menu_start){
					x_vec = menu_end;
					move(x_vec, y_vec);
				}
				break;
			case 's':
				move(++x_vec, y_vec);
				if (x_vec > menu_end){
					x_vec = menu_start;
					move(x_vec, y_vec);
				}
				break;
			case '\n':
				return x_vec;
				break;
		}
	}
}

int startGame(void){
	int snake_x = 13;
	int snake_y = 25;

	// Draw frame
	clear();
	buildBlock(50, 25);

	// Waits for user
	move(8, 12);
	addstr("Press something to start!");
	move(snake_x, snake_y);
	getch();

	// Build snake
	struct snakeBuilder{
		int x_coord;
		int y_coord;
		int past_x;
		int past_y;
	};

	struct snakeBuilder snake[120];
	snake[0].x_coord = snake_x;
	snake[0].y_coord = snake_y;
	snake[0].past_x = snake[0].x_coord;
	snake[0].past_y = snake[0].y_coord;

	// Initialize game
	int *dir;
	*dir = UP;

	do{
		// Wait user inpit for 1 second
		timeout(500);
		int key = getch();
		timeout(-1);

		// Print snake
		if (key != ERR)
			getSnakeDir(key, dir);
		move(snake[0].x_coord, snake[0].y_coord);
		addstr("#");

		// Move snake
		++snake[0].x_coord;

	} while(1);

	return 0;
}
	
int mainMenu(void){
	int x_vec = 11,
	    y_vec = 10;

	// Draw frame
	clear();
	buildBlock(80, 28);
	// Parameters tell where to draw snake
	printSnake(5, 10);
	// Draw the menu header
	// drawTitle(30, 10);
	move(++x_vec, y_vec);
	addstr("Welcome to cSnakeGame!!");
	move(++x_vec, y_vec);
	addstr("-----------------------");
	move(++x_vec, y_vec);
	addstr("Choose an option:");
	move(++x_vec, y_vec);
	addstr("1) Start game");
	move(++x_vec, y_vec);
	addstr("2) Credits");
	move(++x_vec, y_vec);
	addstr("3) Exit");
	move(++x_vec, y_vec);
	move(++x_vec, y_vec);
	addstr("TIP: use W and S arrow to navigate and ENTER to select!");

	return selectOption();
}

int main(void){
	// Start main menu
	initscr();
	switch(mainMenu()){
		case 15:
			// Start game
			startGame();
			break;
		case 16:
			// Show credits
			creditsMenu();
			break;
		case 17:
			// Exit message
			if (exitMenu() == 2)
				main();
			break;
	}

	// Exit game
	endwin();
	return 0;
}
