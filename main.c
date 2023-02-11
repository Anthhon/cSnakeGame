#include<stdio.h>
#include<curses.h>

void printBlock(void){
	const short unsigned BOX_SIZE_H = 80;
	const short unsigned BOX_SIZE_V = 30;
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

int main(void){
	initscr();
	printBlock();
	endwin();
	return 0;
}
