# cSnakeGame
cSnakeGame is made in C programming language **using the ncurses library** which
consists of a snake that moves around the screen and has to eat food while avoiding collision
with the walls or with its own body. The snake will grow in length every time it eats food and the game will end when the snake collides with the walls or with its own body.

# Usage

### Installing ncurses library

    sudo apt-get install libncurses5-dev libncursesw5-dev

### Building
Using cmake (Recommended)

    cd build & cmake .. & make

Using GCC

    gcc -o cSnakeGame cSnakeGame -lncurses

### Running

    ./cSnakeGame

# Requirements

- Linux OS (Unfortunely code STILL doesn't works in Windows)
- GCC compiler
- Cmake
- NCurses library

# Roadmap

Here is some features that i want to add to this project by priority order

## Basic-features

OK - Game menu

OK - Menu navigation

OK - Credits screen

OK - Exit button/screen

## Ingame-features

OK - Implement the snake movement using WASD

OK - Increase size functionality

OK - Apples to spawn in random places in-frame

OK - Increase snake length for each apple fed (GUI)

OK - You lose screen appears when lost

INCOMING - Score for each apple fed (GUI)

- Timer (GUI)

- Phrases to appear in GUI for some scores the user reach

- Difficulty option (Game gets faster)

## Others

- Extra menu option with a snake running to catch a apple
- Score menu option with last tries and some contributors best scores (Please be honest about your best haha)
- Credits page with my name + possible contributors name with some fancy ascii art
- Ask user if he really wants to exit the game

# Contributing

Any contributions are welcome ;)

