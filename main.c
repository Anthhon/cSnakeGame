#include<curses.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<ctype.h>

#define UP 1
#define RIGHT 2
#define DOWN 3
#define LEFT 4

#define SNAKE_BODY 'o'
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

void build_apple(int block_size_h, int block_size_v){
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

	/* Add 1 snake body part */
	snake[1]->x_coord = snake_x;
	snake[1]->y_coord = +snake_y;
	snake[1]->past_x = snake[1]->x_coord;
	snake[1]->past_y = snake[1]->y_coord;

	return;
}

void increase_snake_size(snake_builder*snake[]){
	for (int i = 1, past = 0; i < SNAKE_MAX_SIZE; ++i, ++past){
		if (snake[i]->x_coord == -1){
			snake[i]->x_coord = snake[past]->past_x;
			snake[i]->y_coord = snake[past]->past_y;
			break;
		}
	}

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

void move_snake_body(snake_builder *snake[], int *dir){
	for (int i = 1, last = 0; i < SNAKE_MAX_SIZE; ++i, ++last){
		/* Detect snake body end */
		if (snake[i]->x_coord == -1) break;

		move(snake[i]->past_x, snake[i]->past_y);
		addstr(" ");

		move(snake[i]->x_coord, snake[i]->y_coord);
		printw("%c", SNAKE_BODY);

		snake[i]->past_x = snake[i]->x_coord;
		snake[i]->past_y = snake[i]->y_coord;

		snake[i]->x_coord = snake[last]->past_x;
		snake[i]->y_coord = snake[last]->past_y;
	}
	move(26, 0);

	return;
}

void move_snake_head(snake_builder *snake[], int *dir){
	/* Handle snake head movement
	 *
	 * The snake movement follow these steps:
	 * - Erases last snake position
	 * - Move cursor to new snake coord
	 * - Print snake body
	 * - Assign past snake body coord
	 * - Updates snake new coord based in user input
	 * - Move cursor out screen */

	move(snake[0]->past_x, snake[0]->past_y);
	addstr(" ");

	move(snake[0]->x_coord, snake[0]->y_coord);
	printw("%c", SNAKE_HEAD);

	snake[0]->past_x = snake[0]->x_coord;
	snake[0]->past_y = snake[0]->y_coord;

	switch(*dir)
	{
	case UP:
		--snake[0]->x_coord;
		break;
	case DOWN:
		++snake[0]->x_coord;
		break;
	case LEFT:
		--snake[0]->y_coord;
		break;
	case RIGHT:
		++snake[0]->y_coord;
		break;
	}
	return;
}

void update_snake(snake_builder *snake[], int *dir){
		move_snake_head(&snake[0], dir);
		move_snake_body(&snake[0], dir);
		return;
}

int start_game(void){
	/* Build scenario frame */
	int block_size_h = 50,
	    block_size_v = 25;
	clear();
	build_block(block_size_h, block_size_v);
	/* This function should work without coordinates, it should generate
	 * a random numbers beetween block map size and build an apple
	 * build_apple(block_size_v, block_size_h);

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
