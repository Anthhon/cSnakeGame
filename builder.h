/* This header file contains some 'third-parties' functions
 * who deal with secondary functions not directly related with
 * the game experience */

#include<stdio.h>
#include<time.h>
#include<stdlib.h>

#define BLOCK_CHAR '#'
#define APPLE_CHAR '*'

void convert_char_to_lower(int *input){
	*input += 32;
}

void clear_in_range(int x_range, int y_range){
	/* Avoid erasing walls */
	--y_range;
	--x_range;

	/* Clear each point between given range */
	for (int y_coord = 1; y_coord <= y_range; ++y_coord)
		for (int x_coord = 1; x_coord <= x_range; ++x_coord)
			mvaddch(y_coord, x_coord, ' ');

	return;
}

void print_name(int x_coordinate, int y_coordinate){
	/* Initialize color */
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	attron(COLOR_PAIR(3));

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

	/* Turn-off color */
	attroff(COLOR_PAIR(3));

	return;
}

void print_you_lose(int x_coordinate, int y_coordinate){
	move(x_coordinate, y_coordinate);
	addstr(" __     __           _");
	move(++x_coordinate, y_coordinate);
	addstr(" \\ \\   / /          | |");
	move(++x_coordinate, y_coordinate);
	addstr("  \\ \\_/ /__  _   _  | | ___  ___  ___");
	move(++x_coordinate, y_coordinate);
	addstr("   \\   / _ \\| | | | | |/ _ \\/ __|/ _ \\");
	move(++x_coordinate, y_coordinate);
	addstr("    | | (_) | |_| | | | (_) \\__ \\  __/");
	move(++x_coordinate, y_coordinate);
	addstr("    |_|\\___/ \\__,_| |_|\\___/|___/\\___|");

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

void build_apple(int max_size_x, int max_size_y){

	/* Initialize color */
	init_pair(2, COLOR_MAGENTA, COLOR_BLACK);

	/* Make sound when player eats an apple */
	printf("\a\n");

	/* Generate a random numbers between
	 * block map size and build an apple */
	srand(time(0)); 
	int min_block_size = 2,
	    random_coord_x = rand() % max_size_x,
	    random_coord_y = rand() % max_size_y;

	/* Avoid apple being placed in walls */
	if (random_coord_x == 0) ++random_coord_x;
	if (random_coord_y == 0) ++random_coord_y;
	
	move(random_coord_x, random_coord_y);

	attron(COLOR_PAIR(2));
	printw("%c", APPLE_CHAR);
	attroff(COLOR_PAIR(2));

	return;
}

/* SIZE represent the block size to be built
 * COORD represent where the block should be built */
void build_block_in(int SIZE_H, int SIZE_V,
		 int y_coord, int x_coord){

	int x_end = SIZE_V + x_coord;
	int y_end = SIZE_H + y_coord;

	for (int x = x_coord; x <= x_end; ++x)
		for (int y = y_coord; y <= y_end; ++y)
			if (x == x_coord || x == x_end || y == y_coord || y == y_end){
				move(x, y);
				printw("%c", BLOCK_CHAR);
			}

	return;
}

void build_block(int SIZE_H, int SIZE_V){
	/* Provide default coord values if not provided */
	build_block_in(SIZE_H, SIZE_V, 0, 0);
	return;
}



