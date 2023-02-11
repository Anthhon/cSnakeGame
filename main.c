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

void printBlock(void){
	const short unsigned BOX_SIZE_H = 50;
	const short unsigned BOX_SIZE_V = 15;
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

void printMenu(void){
	int x_vec = 11,
	    y_vec = 10;

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

	// Move cursor to final position
	move(++x_vec, y_vec);
}

int main(void){
	initscr();
	// printBlock();
	printMenu();
	getch();
	endwin();
	return 0;
}
