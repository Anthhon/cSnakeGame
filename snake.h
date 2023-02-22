#include<stdio.h>

#define SNAKE_MAX_SIZE 120

#define UP 1
#define RIGHT 2
#define DOWN 3
#define LEFT 4

#define SNAKE_BODY 'o'
#define SNAKE_HEAD 'H'

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
