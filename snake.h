#include<stdio.h>

#define SNAKE_MAX_SIZE 120

#define UP 1
#define RIGHT 2
#define DOWN 3
#define LEFT 4

#define SNAKE_BODY 'o'
#define SNAKE_HEAD 'H'

#define BLOCK_CHAR '#'
#define APPLE_CHAR '*'

typedef struct snake_body{
	int x_coord,
	    y_coord,
	    past_x,
	    past_y;
} snake_builder;

void init_snake(snake_builder *snake[]){
	/* Mid-screen default position */
	int snake_x = 13,
	    snake_y = 25;

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

	/* Add first snake body part */
	snake[1]->x_coord = snake_x;
	snake[1]->y_coord = +snake_y;
	snake[1]->past_x = snake[1]->x_coord;
	snake[1]->past_y = snake[1]->y_coord;

	return;
}

void increase_snake_size(snake_builder*snake[]){
	int body_index = 1;
	int prev_body_index = 0;

	while (body_index < SNAKE_MAX_SIZE){
		/* Add a new segment when finds the snake body end */
		if (snake[body_index]->x_coord == -1){
			snake[body_index]->x_coord = snake[prev_body_index]->past_x;
			snake[body_index]->y_coord = snake[prev_body_index]->past_y;
			break;
		}
		++body_index;
		++prev_body_index;
	}

	return;
}

void get_snake_dir(int *input, int *direction){
	/* Avoid CAPSLOCK pressed errors*/
	if (isupper(*input))
		convert_char_to_lower(input);
	
	/* Handle invalid movements
	if (*direction == UP && *input == DOWN || *direction == DOWN && *input == UP)
		return;
	if (*direction == LEFT && *input == RIGHT || *direction == RIGHT && *input == LEFT)
		return;
	 */

	if (*input == 'w') *direction = UP;
	if (*input == 's') *direction = DOWN;
	if (*input == 'a') *direction = LEFT;
	if (*input == 'd') *direction = RIGHT;

	return;
}

void check_collision(int x_coord, int y_coord, snake_builder *snake[], int *snake_alive){
	/* Extract the character in the given position */
	char next_position_char = mvinch(x_coord, y_coord) & A_CHARTEXT;
	
	switch(next_position_char)
	{
	case BLOCK_CHAR:
		*snake_alive = 0;
		return;
		break;
	case APPLE_CHAR:
		increase_snake_size(&snake[0]);
		build_apple(25, 50);
		return;
		break;
	default:
		return;
		break;
	}
}

void move_snake_body(snake_builder *snake[], int *dir){
	int body_index = 1;
	int prev_body_index = 0;

	/* Search the last used body segment of snake */
	while (body_index < SNAKE_MAX_SIZE && snake[body_index]->x_coord != -1){
		/* Erase the last body segment */
		move(snake[body_index]->past_x, snake[body_index]->past_y);
		addstr(" ");

		move(snake[body_index]->x_coord, snake[body_index]->y_coord);
		printw("%c", SNAKE_BODY);

		/* Update the coordinates of the body segment */
		snake[body_index]->past_x = snake[body_index]->x_coord;
		snake[body_index]->past_y = snake[body_index]->y_coord;
		snake[body_index]->x_coord = snake[prev_body_index]->past_x;
		snake[body_index]->y_coord = snake[prev_body_index]->past_y;

		++body_index;
		++prev_body_index;
	}

	return;
}

void move_snake_head(snake_builder *snake[], int *dir){
	/* Move snake body based in user input */
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
	/* Move cursor out the screen */
	move(26, 0);
	return;
}
