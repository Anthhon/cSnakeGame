#include<stdio.h>
#include<curses.h>

void drawSnake(int x_vec, int y_vec){
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

void printBlock(unsigned short SIZE_H, unsigned short SIZE_V){
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

int printMenu(void){
	int x_vec = 11,
	    y_vec = 10;

	// Draw frame
	printBlock(80, 28);
	// Parameters tell where to draw snake
	drawSnake(5, 10);
	// Draw the menu header
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

	// Menu navigation 
	x_vec = 15;
	y_vec = 10;
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

int main(void){
	// Start main menu
	initscr();
	switch(printMenu()){
		case 15:
			// Start game
			clear();
			addstr("Game started!");
			getch();
			break;
		case 16:
			// Show credits
			clear();
			addstr("Game made by Anthhon");
			getch();
			break;
		case 17:
			// Exit message
			clear();
			addstr("Do You really want to exit the game? (Y/N)");
			getch();
			break;
	}

	// Exit game
	endwin();
	return 0;
}
