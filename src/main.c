#include<curses.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<ctype.h>

#include"./builder.h"
#include"./snake.h"

#define TRUE 1
#define FALSE 0

#define MENU_START 15
#define MENU_END 17

int main(void);

void score_update(){
	/* Initialize at -1 so game starts with 0 */
	static int score = -1;
	++score;

	/* Initialize color */
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	attron(COLOR_PAIR(3));

	/* Clean actual score */
	move(4, 54);
	addstr("             ");
	move(4, 54);
	printw("  Score: %i", score);

	attroff(COLOR_PAIR(3));

	return;
}

int select_option(){
	/* Menu navigation */
	int x_coordinate = 15,
	    y_coordinate = 10;

	move(x_coordinate, y_coordinate); /* Move cursor to first option */
	noecho(); /* Avoid overwriting in screen */
	
	while(true){
		int key = getch();

		switch(key){
			case 'w':
				move(--x_coordinate, y_coordinate);
				if (x_coordinate < MENU_START)
					x_coordinate = MENU_END;
				break;
			case 's':
				move(++x_coordinate, y_coordinate);
				if (x_coordinate > MENU_END)
					x_coordinate = MENU_START;
				break;
			case '\n': 
				return x_coordinate;
				break;
		}
		move(x_coordinate, y_coordinate);
	}
}

int menu_exit(void){
	int x_coordinate = 5,
	    y_coordinate = 10;

	/* Print menu */
	clear();
	build_block(52, 10);
	move(x_coordinate, y_coordinate);

	/* Initialize color */
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	attron(COLOR_PAIR(3));

	/* Ask question */
	addstr("You really want to quit? (Y/N)");
	move(++x_coordinate, y_coordinate);

	/* Turn-off color */
	attroff(COLOR_PAIR(3));

	while(1){
		int key = getch();
		if (key == 'Y' || key == 'y')
			return 1;
		else if (key == 'N' || key == 'n')
			return 2;
	}
}

void menu_credits(void){
	/* Print menu */
	clear();
	build_block(54, 20);

	/* Print my name */
	int x_coordinate = 6,
	    y_coordinate = 10;

	move(x_coordinate, y_coordinate);
	addstr("Game made by:");
	move(++x_coordinate, y_coordinate);
	print_name(x_coordinate, y_coordinate);

	/* Print contributors list */
	x_coordinate = 13;
	move(x_coordinate, y_coordinate);
	addstr("Contributors:");
	move(++x_coordinate, y_coordinate);
	addstr("No one yet :(");
	move(++x_coordinate, y_coordinate);
	getch();

	/* Return to menu */
	main();
	return;
}

void menu_lose(int screen_size_h, int screen_size_v){
	/* Clean game screen */
	build_block(screen_size_h, screen_size_v);
	clear_in_range(screen_size_h, screen_size_v);

	print_you_lose(8, 5);

	/* Initialize color */
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	attron(COLOR_PAIR(3));

	move(16, 8);	
	addstr("Press ENTER to return to main menu");
	move(17, 9);	
	addstr("Or another key to exit the game");

	attroff(COLOR_PAIR(3));

	if (getch() == '\n') main();

	return;
}

int game_start(void){
	/* Build scenario frame */
	int block_size_h = 50,
	    block_size_v = 25;

	clear();
	build_block(block_size_h, block_size_v);

	/* Build game GUI frame */
	build_block_in(17, 12, 53, 0);
	score_update();
	move(2, 57);
	addstr("GAME STATS");
	move(3, 56);
	addstr("------------");

	/* Initialize color */
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	attron(COLOR_PAIR(3));

	/* Waits for player */
	move(8, 12);
	addstr("Press something to start!");

	/* Turn-off color */
	attron(COLOR_PAIR(3));
	getch();

	move(8, 12);
	addstr("                         "); /* Erase text */
	move(26, 0);

	snake_builder *snake[SNAKE_MAX_SIZE];
	init_snake(&snake[0]);

	/* Initialize snake */
	int dir = LEFT;
	int *dir_ptr = &dir;
	int snake_alive = TRUE;
	
	build_apple(block_size_v, block_size_h);
	move(26, 0); /* Avoid mouse in-screen delay */

	do{
		/* Wait user input */
		timeout(200);
		int key = getch();
		timeout(-1);

		/* Update snake direction */
		if (key != ERR)
			get_snake_dir(&key, dir_ptr);

		/* Check if next position collides with something */
		check_collision(snake[0]->x_coord, snake[0]->y_coord, &snake[0], &snake_alive);
		update_snake(&snake[0], dir_ptr);
	}
	while(snake_alive);

	/* Free snake body memory*/
	for (int i = 0; i < SNAKE_MAX_SIZE; ++i)
		free(snake[i]);

	menu_lose(block_size_h, block_size_v);

	return 0;
}
	
int menu_main(void){
	int x_coordinate = 11,
	    y_coordinate = 10;

	/* Draw frame */
	clear();
	build_block(80, 28);
	/* Parameters tell where to draw snake */
	print_snake(5, 10);
	/* Draw the menu header */
	/* Make function to draw the game name in place of snake
	 * drawTitle(30, 10); */
	move(++x_coordinate, y_coordinate);
	addstr("Welcome to cSnakeGame!!");
	move(++x_coordinate, y_coordinate);
	addstr("-----------------------");
	move(++x_coordinate, y_coordinate);
	addstr("Choose an option:");
	move(++x_coordinate, y_coordinate);

	/* Initialize color */
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	attron(COLOR_PAIR(3));

	addstr("1) Start game");
	move(++x_coordinate, y_coordinate);
	addstr("2) Credits");
	move(++x_coordinate, y_coordinate);
	addstr("3) Exit");

	/* Turn color off */
	attroff(COLOR_PAIR(3));

	move(++x_coordinate, y_coordinate);
	move(++x_coordinate, y_coordinate);
	addstr("TIP: use W and S arrow to navigate and ENTER to select!");

	return select_option();
}

int main(void){
	/* Initialize ncurses library */
	initscr();
	start_color();

	init_color(COLOR_WHITE, 1000, 1000, 1000);
	assume_default_colors(COLOR_WHITE, COLOR_BLACK);

	switch(menu_main()){
		case 15:
			/* Start game */
			game_start();
			break;
		case 16:
			/* Show credits */
			menu_credits();
			break;
		case 17:
			/* Exit message */
			if (menu_exit() == 2)
				main();
			break;
	}

	/* Exit game */
	endwin();
	return 0;
}
