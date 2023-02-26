/* This header file contais some 'third-parties' functions
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

void build_apple(int max_size_x, int max_size_y){
	/* Seeds the random generator */
	srand(time(0)); 
	int min_block_size = 2,
	    random_coord_x = rand() % max_size_x,
	    random_coord_y = rand() % max_size_y;

	/* Avoid apple being placed in walls */
	if (random_coord_x == 0) ++random_coord_x;
	if (random_coord_y == 0) ++random_coord_y;
	
	move(random_coord_x, random_coord_y);
	printw("%c", APPLE_CHAR);

	return;
}

void build_block(unsigned short SIZE_H, unsigned short SIZE_V){
	/* I little bit ugly but i don't know how to improve, feel free to refatorate :) */
	for (int x_coordinate = 0; x_coordinate <= SIZE_V; ++x_coordinate)
		for (int y_coordinate = 0; y_coordinate <= SIZE_H; ++y_coordinate){
			if (x_coordinate == 0 || x_coordinate == SIZE_V || y_coordinate == 0 || y_coordinate == SIZE_H){
				move(x_coordinate, y_coordinate);
				printw("%c", BLOCK_CHAR);
			}
		}
	return;
}
