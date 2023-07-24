#include "headers/Game.h"
// i was bored and wanted to actually make a program
// i decided to make chess
// the program is without using a guide or tutorial, its all homemade except for SFML :P
// the program uses SFML via dlls (dynamic)
// currently very unfinished

//idk man
//its too abstract/too deep
//no need for a cell class
//also the texture handler could probobly be better. 
//perhaps with a single large texture map or better written idk :P

int main() {

	Game game(800, 800, "Chess");
	game.start();

	return 0;
}