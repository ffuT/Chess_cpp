#include "headers/Game.h"
#include <iostream>

// i was bored and wanted to actually make a program
// i decided to make chess
// the program is without using a guide or tutorial, its all homemade except for SFML :P
// the program uses SFML via dlls (dynamic)
// currently very unfinished

int main() {

	Game game(800, 800, "Chess");
	game.start();

	return 0;
}