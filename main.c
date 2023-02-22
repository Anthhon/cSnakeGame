#include<curses.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<ctype.h>

#include"./builder.h"
#include"./snake.h"

#define MENU_START 15
#define MENU_END 17

int main(void);

int select_option(){
	/* Menu navigation */
	int x_coordinate = 15;
	int y_coordinate = 10;
	move(x_coordinate, y_coordinate); /* Move cursor to first option */
	noecho(); /* Avoid overwritting in screen */
	
	while(true){
		int key = getch();
		switch(key){
			case 'w':
				move(--x_coordinate, y_coordinate);
				if (x_coordinate < MENU_START){
					x_coordinate = MENU_END;
					move(x_coordinate, y_coordinate);
				}
				break;
			case 's':
				move(++x_coordinate, y_coordinate);
				if (x_coordinate > MENU_END){
					x_coordinate = MENU_START;
					move(x_coordinate, y_coordinate);
				}
				break;
			/* '\n' refeers to ENTER keybutton */
			case '\n': 
				return x_coordinate;
				break;
		}
	}
}

int exit_menu(void){
	int x_coordinate = 5,
	    y_coordinate = 10;

	/* Print menu */
	clear();
	build_block(52, 10);
	move(x_coordinate, y_coordinate);

	/* Ask question */
	addstr("You really want to quit? (Y/N)");
	move(++x_coordinate, y_coordinate);
	int key = getch();

	if (key == 'Y' || key == 'y')
		return 1;
	else if (key == 'N' || key == 'n')
		return 2;
	else exit_menu();
}

void credits_menu(void){
	clear();
	build_block(54, 20);

	/* Print my name */
	int x_coordinate = 6, y_coordinate = 10;
	move(x_coordinate, y_coordinate);
	addstr("Game made by:");
	move(++x_coordinate, y_coordinate);
	move(x_coordinate, y_coordinate);
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

int start_game(void){
	/* Build scenario frame */
	int block_size_h = 50,
	    block_size_v = 25;

	clear();
	build_block(block_size_h, block_size_v);

	/* Waits for user */
	move(8, 12);
	addstr("Press something to start!");
	getch();
	move(8, 12);
	addstr("                         "); /* Erase text */
	move(26, 0);

	snake_builder *snake[SNAKE_MAX_SIZE];
	init_snake(&snake[0]);

	/* Initialize game */
	int dir = LEFT;
	int *dir_ptr = &dir;

	/* This function should work without coordinates, it should generate
	 * a random numbers beetween block map size and build an apple */
	build_apple(block_size_v, block_size_h);
	move(26, 0); /* Avoid mouse in-screen delay */

	do{
		/* Wait user input for 1 second */
		timeout(420);
		int key = getch();
		timeout(-1);

		
		/* Update snake direction */
		if (key != ERR)
			get_snake_dir(&key, dir_ptr);
		update_snake(&snake[0], dir_ptr);
		/* TESTING 
		 * Increase snake size when 'K' is pressed
		if (key == 'j')
			increase_snake_size(&snake[0]); */

	} while(1);

	/* Free memory */
	for (int i = 0; i < SNAKE_MAX_SIZE; ++i){
		free(snake[i]);
	}
	free(dir_ptr);

	return 0;
}
	
int main_menu(void){
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
	addstr("1) Start game");
	move(++x_coordinate, y_coordinate);
	addstr("2) Credits");
	move(++x_coordinate, y_coordinate);
	addstr("3) Exit");
	move(++x_coordinate, y_coordinate);
	move(++x_coordinate, y_coordinate);
	addstr("TIP: use W and S arrow to navigate and ENTER to select!");

	return select_option();
}

int main(void){
	/* Start main menu */
	initscr();
	switch(main_menu()){
		case 15:
			/* Start game */
			start_game();
			break;
		case 16:
			/* Show credits */
			credits_menu();
			break;
		case 17:
			/* Exit message */
			if (exit_menu() == 2)
				main();
			break;
	}

	/* Exit game */
	endwin();
	return 0;
}
