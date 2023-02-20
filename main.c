#include<curses.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

#define UP 1
#define RIGHT 2
#define DOWN 3
#define LEFT 4

int main(void);

void print_name(int x_vec, int y_vec){
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

/* Parameters tell where to draw the snake */
void print_snake(int x_vec, int y_vec){
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

void build_block(unsigned short SIZE_H, unsigned short SIZE_V){
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

int select_option(){
	/* Menu navigation */
	int x_vec = 15;
	int y_vec = 10;
	short int menu_start = 15;
	short int menu_end = 17;
	move(x_vec, y_vec); /* Move cursor to first option */
	noecho(); /* Avoid overwritting in screen */
	
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
			/* '\n' refeers to ENTER keybutton */
			case '\n': 
				return x_vec;
				break;
		}
	}
}

int exit_menu(void){
	int x_vec = 5,
	    y_vec = 10;

	/* Print menu */
	clear();
	build_block(52, 10);
	move(x_vec, y_vec);

	/* Ask question */
	addstr("You really want to quit? (Y/N)");
	move(++x_vec, y_vec);
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
	int x_vec = 6, y_vec = 10;
	move(x_vec, y_vec);
	addstr("Game made by:");
	move(++x_vec, y_vec);
	move(x_vec, y_vec);
	print_name(x_vec, y_vec);

	/* Print contributors list */
	x_vec = 13;
	move(x_vec, y_vec);
	addstr("Contributors:");
	move(++x_vec, y_vec);
	addstr("No one yet :(");
	move(++x_vec, y_vec);
	getch();

	/* Return to menu */
	main();
	return;
}

void convert_char_to_lower(int *key){
	*key += 32;
}

int char_is_upper(int *key){
	if (64 < *key > 91)
		return true;
	else return false;
}

void get_snake_dir(int input, int *direction){

	int *key = malloc(sizeof(int));
	*key = input;

	if (char_is_upper(key))
		convert_char_to_lower(key);
	
	/* Handle invalid movements */
	if (*direction == UP && input == DOWN || *direction == DOWN && input == UP)
		return;
	if (*direction == LEFT && input == RIGHT || *direction == RIGHT && input == LEFT)
		return;

	switch(*key){
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

	/* Free memory */
	free(key);

	return;
}

int start_game(void){
	int snake_x = 13;
	int snake_y = 25;

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

	/* Build snake */
	typedef struct snakeBody{
		int x_coord;
		int y_coord;
		int past_x;
		int past_y;
	} snakeBuilder;
	snakeBuilder *snake[120];
	/* Allocate memory for all snake pointers */
	for (int i = 0; i < 120; ++i){
		snake[i] = malloc(sizeof(snakeBuilder));
	}

	/* Fill snake struct coords 
	 * i choose -1 to represent a 'non-used' coord in the snake body
	 * using this information i can see when snake body ends*/
	for (int i = 1; i < 120; ++i){
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
			get_snake_dir(key, dir);

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
		 */
		if (*dir == UP){
			move(snake[0]->past_x, snake[0]->past_y);
			addstr(" ");

			move(snake[0]->x_coord, snake[0]->y_coord);
			addstr("#");

			snake[0]->past_x = snake[0]->x_coord;
			snake[0]->past_y = snake[0]->y_coord;

			--snake[0]->x_coord;

			move(26, 0);
		} else if (*dir == DOWN){
			move(snake[0]->past_x, snake[0]->past_y);
			addstr(" ");

			move(snake[0]->x_coord, snake[0]->y_coord);
			addstr("#");

			snake[0]->past_x = snake[0]->x_coord;
			snake[0]->past_y = snake[0]->y_coord;

			++snake[0]->x_coord;

			move(26, 0);
		} else if (*dir == RIGHT){
			move(snake[0]->past_x, snake[0]->past_y);
			addstr(" ");	

			move(snake[0]->x_coord, snake[0]->y_coord);
			addstr("#");

			snake[0]->past_x = snake[0]->x_coord;
			snake[0]->past_y = snake[0]->y_coord;

			++snake[0]->y_coord;

			move(26, 0);
		} else if (*dir == LEFT){
			move(snake[0]->past_x, snake[0]->past_y);
			addstr(" ");

			move(snake[0]->x_coord, snake[0]->y_coord);
			addstr("#");

			snake[0]->past_x = snake[0]->x_coord;
			snake[0]->past_y = snake[0]->y_coord;

			--snake[0]->y_coord;

			move(26, 0);
		}
	} while(1);

	/* Free memory */
	for (int i = 0; i < 120; ++i){
		free(snake[i]);
	}
	free(dir);

	return 0;
}
	
int main_menu(void){
	int x_vec = 11,
	    y_vec = 10;

	/* Draw frame */
	clear();
	build_block(80, 28);
	/* Parameters tell where to draw snake */
	print_snake(5, 10);
	/* Draw the menu header */
	/* drawTitle(30, 10); */
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
