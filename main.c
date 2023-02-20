#include<curses.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<ctype.h>

#define UP 1
#define RIGHT 2
#define DOWN 3
#define LEFT 4

#define SNAKE_HEAD 'H'
#define BLOCK_CHAR '#'
#define SNAKE_MAX_SIZE 120
#define MENU_START 15
#define MENU_END 17

int main(void);

void convert_char_to_lower(int *input){
	*input += 32;
}

void print_name(int x_coordinate, int y_coordinate){
	move(x_coordinate, y_coordinate);
	addstr(" _____       _   _");
	move(++x_coordinate, y_coordinate);
	addstr("|  _  |_____| |_| |_ ___ ___ _ _");
	move(++x_coordinate, y_coordinate);
	addstr("|     |   |_   _|   | . |   | | |");
	move(++x_coordinate, y_coordinate);
	addstr("|__|__|_|_| |_| |_|_|___|_|_|_  |");
	move(++x_coordinate, y_coordinate);
	addstr("                            |___|");
	return;
}

/* Parameters tell where to draw the snake */
void print_snake(int x_coordinate, int y_coordinate){
	/* Art reference:
	 * https://www.asciiart.eu/animals/reptiles/snakes */
	move(x_coordinate, y_coordinate);
	addstr("             ____");
	move(++x_coordinate, y_coordinate);
	addstr("            / . .\\");
	move(++x_coordinate, y_coordinate);
	addstr("            \\  ---<");
	move(++x_coordinate, y_coordinate);
	addstr("             \\  /");
	move(++x_coordinate, y_coordinate);
	addstr("   __________/ /");
	move(++x_coordinate, y_coordinate);
	addstr("-=:___________/");
	return;
}

void build_block(unsigned short SIZE_H, unsigned short SIZE_V){

	for (int x_coordinate = 0; x_coordinate <= SIZE_V; ++x_coordinate){
		for (int y_coordinate = 0; y_coordinate <= SIZE_H; ++y_coordinate){
		if (x_coordinate == 0 || x_coordinate == SIZE_V){
				move(x_coordinate, y_coordinate);
				printw("%c", BLOCK_CHAR);
			}
			else if (y_coordinate == 0 || y_coordinate == SIZE_H){
				move(x_coordinate, y_coordinate);
				printw("%c", BLOCK_CHAR);
			}
		}
	}
	return;
}

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

typedef struct snake_body{
	int x_coord;
	int y_coord;
	int past_x;
	int past_y;
} snake_builder;

void init_snake(snake_builder *snake[]){
	/* Mid-screen default position */
	int snake_x = 13;
	int snake_y = 25;

	/* Allocate memory for all snake pointers */
	for (int i = 0; i < SNAKE_MAX_SIZE; ++i){
		snake[i] = malloc(sizeof(snake_builder));
	}

	/* Fill snake struct coords 
	 * i choose -1 to represent a 'non-used' coord in the snake body
	 * using this information i can see when snake body ends */
	for (int i = 1; i < SNAKE_MAX_SIZE; ++i){
		snake[i]->x_coord = -1;
		snake[i]->y_coord = -1;
		snake[i]->past_x = -1;
		snake[i]->past_y = -1;
	}

	/* Set snake head position */
	snake[0]->x_coord = snake_x;
	snake[0]->y_coord = snake_y;
	snake[0]->past_x = snake[0]->x_coord;
	snake[0]->past_y = snake[0]->y_coord;

	return;
}

void get_snake_dir(int *input, int *direction){

	if (isupper(*input))
		convert_char_to_lower(input);
	
	/* Handle invalid movements */
	if (*direction == UP && *input == DOWN || *direction == DOWN && *input == UP)
		return;
	if (*direction == LEFT && *input == RIGHT || *direction == RIGHT && *input == LEFT)
		return;

	switch(*input){
		case 'w':
			*direction = UP;
			/* Debugging purpose */
			move(28, 0);
			addstr("console: dir set to UP   ");
			break;
		case 's':
			*direction = DOWN;
			/* Debugging purpose */
			move(28, 0);
			addstr("console: dir set to DOWN ");
			break;
		case 'a':
			*direction = LEFT;
			/* Debugging purpose */
			move(28, 0);
			addstr("console: dir set to LEFT ");
			break;
		case 'd':
			*direction = RIGHT;
			/* Debugging purpose */
			move(28, 0);
			addstr("console: dir set to RIGHT");
			break;
	}

	return;
}

void move_snake(snake_builder *snake[], int *dir){
	/* Handle snake movement
	 *
	 * The snake movement follow these steps:
	 * - Erases last snake position
	 * - Move cursor to new snake coord
	 * - Print snake head
	 * - Assign past snake head coord
	 * - Updates snake new coord based in user input
	 * - TODO Update snake body
	 * - Move cursor out screen 
	 *
	 * That is a big block of code, but i really don't
	 * know what to do about it, srry :(
	 */
	if (*dir == UP){
		move(snake[0]->past_x, snake[0]->past_y);
		addstr(" ");

		move(snake[0]->x_coord, snake[0]->y_coord);
		printw("%c", SNAKE_HEAD);

		snake[0]->past_x = snake[0]->x_coord;
		snake[0]->past_y = snake[0]->y_coord;

		/* TODO: Update entire snake body
		 *
		 * Want to use a for loop to iterate beetween
		 * all struct arrays and update actual coords
		 * by the previous element past coords
		 *
		 * update_snake_body(&snake[0]); */

		--snake[0]->x_coord;

		move(26, 0);
	} else if (*dir == DOWN){
		move(snake[0]->past_x, snake[0]->past_y);
		addstr(" ");

		move(snake[0]->x_coord, snake[0]->y_coord);
		printw("%c", SNAKE_HEAD);

		snake[0]->past_x = snake[0]->x_coord;
		snake[0]->past_y = snake[0]->y_coord;

		++snake[0]->x_coord;

		move(26, 0);
	} else if (*dir == RIGHT){
		move(snake[0]->past_x, snake[0]->past_y);
		addstr(" ");	

		move(snake[0]->x_coord, snake[0]->y_coord);
		printw("%c", SNAKE_HEAD);

		snake[0]->past_x = snake[0]->x_coord;
		snake[0]->past_y = snake[0]->y_coord;

		++snake[0]->y_coord;

		move(26, 0);
	} else if (*dir == LEFT){
		move(snake[0]->past_x, snake[0]->past_y);
		addstr(" ");

		move(snake[0]->x_coord, snake[0]->y_coord);
		printw("%c", SNAKE_HEAD);

		snake[0]->past_x = snake[0]->x_coord;
		snake[0]->past_y = snake[0]->y_coord;

		--snake[0]->y_coord;

		move(26, 0);
	}
	return;
}

int start_game(void){
		/* Draw frame */
	clear();
	build_block(50, 25);

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
	int *dir = malloc(sizeof(int));
	*dir = LEFT;

	do{
		/* Wait user input for 1 second */
		timeout(500);
		int key = getch();
		timeout(-1);

		/* Update snake direction */
		if (key != ERR)
			get_snake_dir(&key, dir);
		move_snake(&snake[0], dir);
	} while(1);

	/* Free memory */
	for (int i = 0; i < SNAKE_MAX_SIZE; ++i){
		free(snake[i]);
	}
	free(dir);

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
