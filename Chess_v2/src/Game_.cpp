//shits fucked idk cant name it Game.cpp cuz "class not registered" idk phind.com said dll error

//i've tried to optimize gameloop using waitevent and a bool called change
//dont know if its good, but it works

//sfml includes
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
//other includes
#include <math.h>
#include <iostream>
//my includes
#include "headers/Game.h"
#include "headers/Texturehandler.h"
#include "headers/Piece.h"

Game::Game(const int width_in, const int height_in, const char title_in[]) :
	m_width(width_in), m_height(height_in),
	window(sf::VideoMode(width_in, height_in), title_in, sf::Style::Default), ev() {
	windowptr = &window;
	
	m_font_arial->loadFromFile("Resources/Extra/arial.ttf");

	//initialize pieces. yes its bad
	//black pieces
	for (int i = 0; i <= 7; i++) { 
		inipiece(i, 1, bpawn); }
	inipiece(0, 0, brook);
	inipiece(7, 0, brook);
	inipiece(1, 0, bknight);
	inipiece(6, 0, bknight);
	inipiece(2, 0, bbishop);
	inipiece(5, 0, bbishop);
	inipiece(3, 0, bqueen);
	inipiece(4, 0, bking);
	//white
	for (int i = 0; i <= 7; i++) {
		inipiece(i, 6, pawn);}
	inipiece(0, 7, rook);
	inipiece(7, 7, rook);
	inipiece(1, 7, knight);
	inipiece(6, 7, knight);
	inipiece(2, 7, bishop);
	inipiece(5, 7, bishop);
	inipiece(3, 7, queen);
	inipiece(4, 7, king);

	for (bool move : legalMoves) {
		move = false;
	}

}

Game::~Game() {

}

void Game::inipiece(int x, int y, E_Piece state) {
	board[x][y] = state;
}

void Game::stop() {
	running = false;
	window.close();
}

void Game::start() {
	//gameloop
	running = true;
	auto onesec = std::chrono::steady_clock::now() + std::chrono::seconds(1);
	auto lastTime = std::chrono::steady_clock::now();
	float accumulatedTime = 0.0f;

	render(); //render before the loop cuz of waitevent

	while (running) {		//window loop
		auto now = std::chrono::steady_clock::now();
		auto deltaTime = std::chrono::duration<float>(now - lastTime).count();

		//window events
		if (window.waitEvent(ev)) {
			switch (ev.type) {
			case sf::Event::Closed:
				stop();
				break;
			case sf::Event::MouseButtonPressed:
				mouseClicked();
				break;
			case sf::Event::KeyPressed:
				KeyPress();
				break;
			case sf::Event::KeyReleased:
				KeyRelease();
				break;
			case sf::Event::Resized:
				m_mouse_height = ev.size.height / m_cellAmount;
				m_mouse_width = ev.size.width / m_cellAmount;
				render();
				break;
			}
			change = true;
		}
		//limit render if constant window events
		while (accumulatedTime >= targetFramerate) {
			render();
			change = false;
			accumulatedTime -= targetFramerate;
		}
		lastTime = now;
		accumulatedTime += deltaTime;
	}
}

void Game::mouseClicked() {
	if (ev.mouseButton.button == sf::Mouse::Right) {
		//mouseRightClick();
		return;
	}

	m_mouse_x = sf::Mouse::getPosition(window).x / m_mouse_width;
	m_mouse_y = sf::Mouse::getPosition(window).y / m_mouse_height;

	//select cell
	if (!cellSelected && ((whiteTurn && (board[m_mouse_x][m_mouse_y] > 0)) || (!whiteTurn && !(board[m_mouse_x][m_mouse_y] > 0)))) {
		
		selectedX = m_mouse_x;
		selectedY = m_mouse_y;

		calcLegalMoves();
		if(board[selectedX][selectedY] != 0){  //cant select empty cells
			if(whiteTurn && board[selectedX][selectedY] > 0)
			cellSelected = true;	
			if(!whiteTurn && board[selectedX][selectedY] < 0)
			cellSelected = true;
		}
	}	else {
		if(legalMoves[m_mouse_x][m_mouse_y]) {	//if legal move move
			std::cout << static_cast<char>(selectedX + 65) << abs(selectedY - 8) <<
				" moved to: " << static_cast<char>(m_mouse_x + 65) <<
				abs(m_mouse_y - 8) << "\n";

			board[m_mouse_x][m_mouse_y] = board[selectedX][selectedY];
			board[selectedX][selectedY] = E_Piece::empty;
			
			if (whiteTurn) { // swtich turns after move
				whiteTurn = false;
			} else{
				whiteTurn = true;
			}

			cellSelected = false;
			clearLegalMoves();
		}	//click on already selected de-select 
		else if (m_mouse_x == selectedX && m_mouse_y == selectedY) {
			cellSelected = false;
			clearLegalMoves();
		} //click on other team piece swap selected 
		else if (board[m_mouse_x][m_mouse_y] * board[selectedX][selectedY] > 0) {
			cellSelected = false;
			mouseClicked();	// select the other piece
			return;
		}
	}
	render();	// render to always display moves
}

void Game::clearLegalMoves() {
	change = true;
	// clear legal moves array (all = false)
	for (auto& rows : legalMoves){
		for (auto& elem : rows){
			elem = false;
		}
	}
}

void Game::KeyPress() {
	auto keycode = ev.key.code;
	if (keycode == sf::Keyboard::Escape) {	//close on escape
		stop();
	}
	if (keycode == sf::Keyboard::R){
		std::cout << "restart not made yet :P\n";
	}
		//maybe add more
}

void Game::KeyRelease() {
//idk why i made this function :P
}

void Game::render() {
	if (!change) //uses less graphics idk maybe good maybe bad
		return;

	window.clear(sf::Color(33, 33, 33));
	//draw here

	//draw the background
	for (int x = 0; x <= 7; x++) {
		for (int y = 0; y <= 7; y++) {
			
			backGroundRect.setPosition(x * m_cellwidth, y * m_cellheight);
			
			if ((x - y) % 2 == 0) {
				backGroundRect.setFillColor(sf::Color(200, 195, 155));
				if(legalMoves[x][y])
					backGroundRect.setFillColor(sf::Color(172, 200, 110));
			}
			else {
				backGroundRect.setFillColor(sf::Color(100, 44, 5));
				if (legalMoves[x][y])
					backGroundRect.setFillColor(sf::Color(114, 140, 20));
			}
			
			window.draw(backGroundRect);
			
			//draw pieces
			if (board[x][y] != 0) {

				piece.setSpriteScale(m_cellwidth / 64, m_cellheight / 64);
				piece.setTexture(txh.GetTexture(static_cast<E_Piece>(board[x][y])), x * m_cellwidth, y * m_cellheight);

				piece.draw(windowptr);
			}
		}
	}

	//draw letters based on ascii values 65 = 'A'
	for (char i = 65; i <= 65 + 8; i++) {
		sf::Text text;
		text.setCharacterSize(24);
		text.setFont(*m_font_arial);
		text.setString(i);
		text.setFillColor(sf::Color::Black);
		text.setPosition(sf::Vector2f((i - 65) * m_cellwidth + m_cellwidth - 32, m_height - 32));
		window.draw(text);
	}

	//draw numbers based on ascii values 49 = '1'
	for (char i = 48; i <= 48 + 8; i++) {
		sf::Text text;
		text.setCharacterSize(24);
		text.setFont(*m_font_arial);
		text.setString(i);
		text.setFillColor(sf::Color::Black);
		text.setPosition(sf::Vector2f(4, m_cellheight * 8 - (i-48) * m_cellheight));
		window.draw(text);
	}

	window.display();
}

//this function sucks... but it works so fuck you
//this has to change idk i cant be bothered fuck idk man.. 
//i think alot of the code needs a full restructure
void Game::calcLegalMoves() {	//like its really bad
	clearLegalMoves();
	switch (board[selectedX][selectedY]) {
	case empty:	//if its empty fuck off pwease :3
		return;
	case bpawn:
		if (selectedY == 1) {	//first time moving
			if (board[selectedX][selectedY + 1] == 0) {	//if not blocked 1 forward
				legalMoves[selectedX][selectedY + 1] = true;
				if (board[selectedX][selectedY + 2] == 0)	//if not blocked 2 forward
					legalMoves[selectedX][selectedY + 2] = true;
			}
		}
		else {	//not first time moving
			if (board[selectedX][selectedY + 1] == 0)	//if infront not blocked
				legalMoves[selectedX][selectedY + 1] = true;
		}
		if (board[selectedX - 1][selectedY + 1] > 0)	//if diagonal left is enemy
			legalMoves[selectedX - 1][selectedY + 1] = true;
		if (board[selectedX + 1][selectedY + 1] > 0)	//if diagonal right is enemy
			legalMoves[selectedX + 1][selectedY + 1] = true;
		break;
	case pawn:	//same as black pawn just other way
		if (selectedY == 6) {	//first time moving
			if (board[selectedX][selectedY - 1] == 0) {
				legalMoves[selectedX][selectedY - 1] = true;
				if (board[selectedX][selectedY - 2] == 0)
					legalMoves[selectedX][selectedY - 2] = true;
			}
		}
		else {
			if (board[selectedX][selectedY - 1] == 0)
				legalMoves[selectedX][selectedY - 1] = true;
		}
		if (board[selectedX - 1][selectedY - 1] < 0)
			legalMoves[selectedX - 1][selectedY - 1] = true;
		if (board[selectedX + 1][selectedY - 1] < 0)
			legalMoves[selectedX + 1][selectedY - 1] = true;
		break;
	case bishop:
		for (int i = 1; i < 8; i++) { // diagonal up right
			if (selectedX + i > 7 || selectedY - i < 0)
				break;
			if (board[selectedX + i][selectedY - i] == 0) {	// if empty
				legalMoves[selectedX + i][selectedY - i] = true;
			}
			if (board[selectedX + i][selectedY - i] != 0) {	// if taken
				if (board[selectedX + i][selectedY - i] < 0)	// if enemy 
					legalMoves[selectedX + i][selectedY - i] = true;
				break;
			}
		}
		for (int i = 1; i < 8; i++) { // diagonal up left
			if (selectedX - i < 0 || selectedY - i < 0)
				break;
			if (board[selectedX - i][selectedY - i] == 0) {	// if empty
				legalMoves[selectedX - i][selectedY - i] = true;
			}
			if (board[selectedX - i][selectedY - i] != 0) {	// if taken
				if (board[selectedX - i][selectedY - i] < 0)	// if enemy 
					legalMoves[selectedX - i][selectedY - i] = true;
				break;
			}
		}
		for (int i = 1; i < 8; i++) { // diagonal down right
			if (selectedX + i > 7 || selectedY + i > 7)
				break;
			if (board[selectedX + i][selectedY + i] == 0) {	// if empty
				legalMoves[selectedX + i][selectedY + i] = true;
			}
			if (board[selectedX + i][selectedY + i] != 0) {	// if taken
				if (board[selectedX + i][selectedY + i] < 0)	// if enemy 
					legalMoves[selectedX + i][selectedY + i] = true;
				break;
			}
		}
		for (int i = 1; i < 8; i++) { // diagonal down left
			if (selectedX - i < 0 || selectedY + i > 7)
				break;
			if (board[selectedX - i][selectedY + i] == 0) {	// if empty
				legalMoves[selectedX - i][selectedY + i] = true;
			}
			if (board[selectedX - i][selectedY + i] != 0) {	// if taken
				if (board[selectedX - i][selectedY + i] < 0)	// if enemy 
					legalMoves[selectedX - i][selectedY + i] = true;
				break;
			}
		}
		break;
	case bbishop:
		for (int i = 1; i < 8; i++) { // diagonal up right
			if (selectedX + i > 7 || selectedY - i < 0)
				break;
			if (board[selectedX + i][selectedY - i] == 0) {	// if empty
				legalMoves[selectedX + i][selectedY - i] = true;
			}
			if (board[selectedX + i][selectedY - i] != 0) {	// if taken
				if (board[selectedX + i][selectedY - i] > 0)	// if enemy 
					legalMoves[selectedX + i][selectedY - i] = true;
				break;
			}
		}
		for (int i = 1; i < 8; i++) { // diagonal up left
			if (selectedX - i < 0 || selectedY - i < 0)
				break;
			if (board[selectedX - i][selectedY - i] == 0) {	// if empty
				legalMoves[selectedX - i][selectedY - i] = true;
			}
			if (board[selectedX - i][selectedY - i] != 0) {	// if taken
				if (board[selectedX - i][selectedY - i] > 0)	// if enemy 
					legalMoves[selectedX - i][selectedY - i] = true;
				break;
			}
		}
		for (int i = 1; i < 8; i++) { // diagonal down right
			if (selectedX + i > 7 || selectedY + i > 7)
				break;
			if (board[selectedX + i][selectedY + i] == 0) {	// if empty
				legalMoves[selectedX + i][selectedY + i] = true;
			}
			if (board[selectedX + i][selectedY + i] != 0) {	// if taken
				if (board[selectedX + i][selectedY + i] > 0)	// if enemy 
					legalMoves[selectedX + i][selectedY + i] = true;
				break;
			}
		}
		for (int i = 1; i < 8; i++) { // diagonal down left
			if (selectedX - i < 0 || selectedY + i > 7)
				break;
			if (board[selectedX - i][selectedY + i] == 0) {	// if empty
				legalMoves[selectedX - i][selectedY + i] = true;
			}
			if (board[selectedX - i][selectedY + i] != 0) {	// if taken
				if (board[selectedX - i][selectedY + i] > 0)	// if enemy 
					legalMoves[selectedX - i][selectedY + i] = true;
				break;
			}
		}
		break;
	case knight:
		if (selectedX - 1 >= 0) {	// x - 1 in x calc y-2 & +2
			if (board[selectedX - 1][selectedY + 2] <= 0 && selectedY + 2 <= 7)
				legalMoves[selectedX - 1][selectedY + 2] = true;
			if (board[selectedX - 1][selectedY - 2] <= 0 && selectedY - 2 >= 0)
				legalMoves[selectedX - 1][selectedY - 2] = true;
		}
		if(selectedX - 2 >= 0){	// x - 2 in x calc y-2 & +2
			if (board[selectedX - 2][selectedY + 1] <= 0 && selectedY + 1 <= 7)
				legalMoves[selectedX - 2][selectedY + 1] = true;
			if (board[selectedX - 2][selectedY - 1] <= 0 && selectedY - 1 >= 0)
				legalMoves[selectedX - 2][selectedY - 1] = true;
		}
		if (selectedX + 1 <= 7) { // x + 1 in x calc y-2 & +2
			if (board[selectedX + 1][selectedY + 2] <= 0 && selectedY + 2 <= 7)
				legalMoves[selectedX + 1][selectedY + 2] = true;
			if (board[selectedX + 1][selectedY - 2] <= 0 && selectedY - 2 >= 0)
				legalMoves[selectedX + 1][selectedY - 2] = true;
		}
		if (selectedX + 2 <= 7) { // x + 2 in x calc y-2 & +2
			if (board[selectedX + 2][selectedY + 1] <= 0 && selectedY + 1 <= 7)
				legalMoves[selectedX + 2][selectedY + 1] = true;
			if (board[selectedX + 2][selectedY - 1] <= 0 && selectedY - 1 >= 0)
				legalMoves[selectedX + 2][selectedY - 1] = true;
		}
		break;
	case bknight:
		if (selectedX - 1 >= 0) {	// x - 1 in x calc y-2 & +2
			if (board[selectedX - 1][selectedY + 2] >= 0 && selectedY + 2 <= 7)
				legalMoves[selectedX - 1][selectedY + 2] = true;
			if (board[selectedX - 1][selectedY - 2] >= 0 && selectedY - 2 >= 0)
				legalMoves[selectedX - 1][selectedY - 2] = true;
		}
		if (selectedX - 2 >= 0) {	// x - 2 in x calc y-2 & +2
			if (board[selectedX - 2][selectedY + 1] >= 0 && selectedY + 1 <= 7)
				legalMoves[selectedX - 2][selectedY + 1] = true;
			if (board[selectedX - 2][selectedY - 1] >= 0 && selectedY - 1 >= 0)
				legalMoves[selectedX - 2][selectedY - 1] = true;
		}
		if (selectedX + 1 <= 7) { // x + 1 in x calc y-2 & +2
			if (board[selectedX + 1][selectedY + 2] >= 0 && selectedY + 2 <= 7)
				legalMoves[selectedX + 1][selectedY + 2] = true;
			if (board[selectedX + 1][selectedY - 2] >= 0 && selectedY - 2 >= 0)
				legalMoves[selectedX + 1][selectedY - 2] = true;
		}
		if (selectedX + 2 <= 7) { // x + 2 in x calc y-2 & +2
			if (board[selectedX + 2][selectedY + 1] >= 0 && selectedY + 1 <= 7)
				legalMoves[selectedX + 2][selectedY + 1] = true;
			if (board[selectedX + 2][selectedY - 1] >= 0 && selectedY - 1 >= 0)
				legalMoves[selectedX + 2][selectedY - 1] = true;
		}
		break;
	case rook:
		for (int i = 1; i < 8; i++) {	//up
			if (selectedY - i < 0)	// stop on 0
				break;
			if (board[selectedX][selectedY - i] == 0) {	// if empty legal
				legalMoves[selectedX][selectedY - i] = true;
			}
			if (board[selectedX][selectedY - i] != 0) {	// if taken
				if (board[selectedX][selectedY - i] < 0)	// if enemy legal
					legalMoves[selectedX][selectedY - i] = true;
				break;
			}
		}
		for (int i = 1; i < 8; i++) {	//down
			if (selectedY + i > 7)	//stop on 8
				break;
			if (board[selectedX][selectedY + i] == 0) {	//if empty
				legalMoves[selectedX][selectedY + i] = true;
			}
			if (board[selectedX][selectedY + i] != 0) {	//if taken
				if (board[selectedX][selectedY + i] < 0) //if enemy legal
					legalMoves[selectedX][selectedY + i] = true;
				break;
			}
		}
		for (int i = 1; i < 8; i++) { //left
			if (selectedX - i < 0)	//stop on 8
				break;
			if (board[selectedX - i][selectedY] == 0) {	//if empty
				legalMoves[selectedX - i][selectedY] = true;
			}
			if (board[selectedX - i][selectedY] != 0) {	//if taken
				if (board[selectedX - i][selectedY] < 0) //if enemy legal
					legalMoves[selectedX - i][selectedY] = true;
				break;
			}
		}
		for (int i = 1; i < 8; i++) { //right
			if (selectedX + i > 7)	//stop on 8
				break;
			if (board[selectedX + i][selectedY] == 0) {	//if empty
				legalMoves[selectedX + i][selectedY] = true;
			}
			if (board[selectedX + i][selectedY] != 0) {	//if taken
				if (board[selectedX + i][selectedY] < 0) //if enemy legal
					legalMoves[selectedX + i][selectedY] = true;
				break;
			}
		}
		break;
	case brook:
		for (int i = 1; i < 8; i++) {
			if (selectedY - i < 0)	// stop on 0
				break;
			if (board[selectedX][selectedY - i] == 0) {	// if empty legal
				legalMoves[selectedX][selectedY - i] = true;
			}
			if (board[selectedX][selectedY - i] != 0) {	// if taken
				if (board[selectedX][selectedY - i] > 0)	// if enemy legal
					legalMoves[selectedX][selectedY - i] = true;
				break;
			}
		}
		for (int i = 1; i < 8; i++) {
			if (selectedY + i > 7)	//stop on 8
				break;
			if (board[selectedX][selectedY + i] == 0) {	//if empty
				legalMoves[selectedX][selectedY + i] = true;
			}
			if (board[selectedX][selectedY + i] != 0) {	//if taken
				if (board[selectedX][selectedY + i] > 0) //if enemy legal
					legalMoves[selectedX][selectedY + i] = true;
				break;
			}
		}
		for (int i = 1; i < 8; i++) {
			if (selectedX - i < 0)	//stop on 8
				break;
			if (board[selectedX - i][selectedY] == 0) {	//if empty
				legalMoves[selectedX - i][selectedY] = true;
			}
			if (board[selectedX - i][selectedY] != 0) {	//if taken
				if (board[selectedX - i][selectedY] > 0) //if enemy legal
					legalMoves[selectedX - i][selectedY] = true;
				break;
			}
		}
		for (int i = 1; i < 8; i++) {
			if (selectedX + i > 7)	//stop on 8
				break;
			if (board[selectedX + i][selectedY] == 0) {	//if empty
				legalMoves[selectedX + i][selectedY] = true;
			}
			if (board[selectedX + i][selectedY] != 0) {	//if taken
				if (board[selectedX + i][selectedY] > 0) //if enemy legal
					legalMoves[selectedX + i][selectedY] = true;
				break;
			}
		}
		break;
	case queen:
		for (int i = 1; i < 8; i++) { // diagonal up right
			if (selectedX + i > 7 || selectedY - i < 0)
				break;
			if (board[selectedX + i][selectedY - i] == 0) {	// if empty
				legalMoves[selectedX + i][selectedY - i] = true;
			}
			if (board[selectedX + i][selectedY - i] != 0) {	// if taken
				if (board[selectedX + i][selectedY - i] < 0)	// if enemy 
					legalMoves[selectedX + i][selectedY - i] = true;
				break;
			}
		}
		for (int i = 1; i < 8; i++) { // diagonal up left
			if (selectedX - i < 0 || selectedY - i < 0)
				break;
			if (board[selectedX - i][selectedY - i] == 0) {	// if empty
				legalMoves[selectedX - i][selectedY - i] = true;
			}
			if (board[selectedX - i][selectedY - i] != 0) {	// if taken
				if (board[selectedX - i][selectedY - i] < 0)	// if enemy 
					legalMoves[selectedX - i][selectedY - i] = true;
				break;
			}
		}
		for (int i = 1; i < 8; i++) { // diagonal down right
			if (selectedX + i > 7 || selectedY + i > 7)
				break;
			if (board[selectedX + i][selectedY + i] == 0) {	// if empty
				legalMoves[selectedX + i][selectedY + i] = true;
			}
			if (board[selectedX + i][selectedY + i] != 0) {	// if taken
				if (board[selectedX + i][selectedY + i] < 0)	// if enemy 
					legalMoves[selectedX + i][selectedY + i] = true;
				break;
			}
		}
		for (int i = 1; i < 8; i++) { // diagonal down left
			if (selectedX - i < 0 || selectedY + i > 7)
				break;
			if (board[selectedX - i][selectedY + i] == 0) {	// if empty
				legalMoves[selectedX - i][selectedY + i] = true;
			}
			if (board[selectedX - i][selectedY + i] != 0) {	// if taken
				if (board[selectedX - i][selectedY + i] < 0)	// if enemy 
					legalMoves[selectedX - i][selectedY + i] = true;
				break;
			}
		}
		for (int i = 1; i < 8; i++) {
			if (selectedY - i < 0)	// stop on 0
				break;
			if (board[selectedX][selectedY - i] == 0) {	// if empty legal
				legalMoves[selectedX][selectedY - i] = true;
			}
			if (board[selectedX][selectedY - i] != 0) {	// if taken
				if (board[selectedX][selectedY - i] < 0)	// if enemy legal
					legalMoves[selectedX][selectedY - i] = true;
				break;
			}
		}
		for (int i = 1; i < 8; i++) {
			if (selectedY + i > 7)	//stop on 8
				break;
			if (board[selectedX][selectedY + i] == 0) {	//if empty
				legalMoves[selectedX][selectedY + i] = true;
			}
			if (board[selectedX][selectedY + i] != 0) {	//if taken
				if (board[selectedX][selectedY + i] < 0) //if enemy legal
					legalMoves[selectedX][selectedY + i] = true;
				break;
			}
		}
		for (int i = 1; i < 8; i++) {
			if (selectedX - i < 0)	//stop on 8
				break;
			if (board[selectedX - i][selectedY] == 0) {	//if empty
				legalMoves[selectedX - i][selectedY] = true;
			}
			if (board[selectedX - i][selectedY] != 0) {	//if taken
				if (board[selectedX - i][selectedY] < 0) //if enemy legal
					legalMoves[selectedX - i][selectedY] = true;
				break;
			}
		}
		for (int i = 1; i < 8; i++) {
			if (selectedX + i > 7)	//stop on 8
				break;
			if (board[selectedX + i][selectedY] == 0) {	//if empty
				legalMoves[selectedX + i][selectedY] = true;
			}
			if (board[selectedX + i][selectedY] != 0) {	//if taken
				if (board[selectedX + i][selectedY] < 0) //if enemy legal
					legalMoves[selectedX + i][selectedY] = true;
				break;
			}
		}
		break;
		//this is so dumb, ctrl + c -> ctrl v
	case bqueen:
		for (int i = 1; i < 8; i++) { // diagonal up right
			if (selectedX + i > 7 || selectedY - i < 0)
				break;
			if (board[selectedX + i][selectedY - i] == 0) {	// if empty
				legalMoves[selectedX + i][selectedY - i] = true;
			}
			if (board[selectedX + i][selectedY - i] != 0) {	// if taken
				if (board[selectedX + i][selectedY - i] > 0)	// if enemy 
					legalMoves[selectedX + i][selectedY - i] = true;
				break;
			}
		}
		for (int i = 1; i < 8; i++) { // diagonal up left
			if (selectedX - i < 0 || selectedY - i < 0)
				break;
			if (board[selectedX - i][selectedY - i] == 0) {	// if empty
				legalMoves[selectedX - i][selectedY - i] = true;
			}
			if (board[selectedX - i][selectedY - i] != 0) {	// if taken
				if (board[selectedX - i][selectedY - i] > 0)	// if enemy 
					legalMoves[selectedX - i][selectedY - i] = true;
				break;
			}
		}
		for (int i = 1; i < 8; i++) { // diagonal down right
			if (selectedX + i > 7 || selectedY + i > 7)
				break;
			if (board[selectedX + i][selectedY + i] == 0) {	// if empty
				legalMoves[selectedX + i][selectedY + i] = true;
			}
			if (board[selectedX + i][selectedY + i] != 0) {	// if taken
				if (board[selectedX + i][selectedY + i] > 0)	// if enemy 
					legalMoves[selectedX + i][selectedY + i] = true;
				break;
			}
		}
		for (int i = 1; i < 8; i++) { // diagonal down left
			if (selectedX - i < 0 || selectedY + i > 7)
				break;
			if (board[selectedX - i][selectedY + i] == 0) {	// if empty
				legalMoves[selectedX - i][selectedY + i] = true;
			}
			if (board[selectedX - i][selectedY + i] != 0) {	// if taken
				if (board[selectedX - i][selectedY + i] > 0)	// if enemy 
					legalMoves[selectedX - i][selectedY + i] = true;
				break;
			}
		}
		for (int i = 1; i < 8; i++) {
			if (selectedY - i < 0)	// stop on 0
				break;
			if (board[selectedX][selectedY - i] == 0) {	// if empty legal
				legalMoves[selectedX][selectedY - i] = true;
			}
			if (board[selectedX][selectedY - i] != 0) {	// if taken
				if (board[selectedX][selectedY - i] > 0)	// if enemy legal
					legalMoves[selectedX][selectedY - i] = true;
				break;
			}
		}
		for (int i = 1; i < 8; i++) {
			if (selectedY + i > 7)	//stop on 8
				break;
			if (board[selectedX][selectedY + i] == 0) {	//if empty
				legalMoves[selectedX][selectedY + i] = true;
			}
			if (board[selectedX][selectedY + i] != 0) {	//if taken
				if (board[selectedX][selectedY + i] > 0) //if enemy legal
					legalMoves[selectedX][selectedY + i] = true;
				break;
			}
		}
		for (int i = 1; i < 8; i++) {
			if (selectedX - i < 0)	//stop on 8
				break;
			if (board[selectedX - i][selectedY] == 0) {	//if empty
				legalMoves[selectedX - i][selectedY] = true;
			}
			if (board[selectedX - i][selectedY] != 0) {	//if taken
				if (board[selectedX - i][selectedY] > 0) //if enemy legal
					legalMoves[selectedX - i][selectedY] = true;
				break;
			}
		}
		for (int i = 1; i < 8; i++) {
			if (selectedX + i > 7)	//stop on 8
				break;
			if (board[selectedX + i][selectedY] == 0) {	//if empty
				legalMoves[selectedX + i][selectedY] = true;
			}
			if (board[selectedX + i][selectedY] != 0) {	//if taken
				if (board[selectedX + i][selectedY] > 0) //if enemy legal
					legalMoves[selectedX + i][selectedY] = true;
				break;
			}
		}
		break;
	case king:
		if(selectedX + 1 <= 7){	//x+1 right 3 moves
			if(board[selectedX + 1][selectedY] <= 0)
				legalMoves[selectedX + 1][selectedY] = true;	//rigt
			if(selectedY + 1 <= 7 && board[selectedX + 1][selectedY+1] <= 0)
				legalMoves[selectedX+1][selectedY+1] = true;	//right down
			if(selectedY - 1 >= 0 && board[selectedX + 1][selectedY - 1] <= 0)
				legalMoves[selectedX+1][selectedY-1] = true;	//right up
		}
		if(selectedX -1 >= 0){ //x-1 left 3 moves
			if (board[selectedX - 1][selectedY] <= 0)
				legalMoves[selectedX - 1][selectedY] = true;	//left
			if (selectedY - 1 >= 0 && board[selectedX - 1][selectedY - 1] <= 0)
				legalMoves[selectedX-1][selectedY-1] = true;	//left up
			if (selectedY + 1 <= 7 && board[selectedX - 1][selectedY + 1] <= 0)
				legalMoves[selectedX-1][selectedY+1] = true;	//left down
		}
		// x+0
		if (selectedY + 1 <= 7 && board[selectedX][selectedY + 1] <= 0)
			legalMoves[selectedX][selectedY+1] = true;	//up
		if(selectedY - 1 >= 0 && board[selectedX][selectedY - 1] <= 0)
			legalMoves[selectedX][selectedY-1] = true;	//down
		break;
	case bking:
		if (selectedX + 1 <= 7) {	//x+1 right 3 moves
			if (board[selectedX + 1][selectedY] >= 0)
				legalMoves[selectedX + 1][selectedY] = true;	//rigt
			if (selectedY + 1 <= 7 && board[selectedX + 1][selectedY + 1] >= 0)
				legalMoves[selectedX + 1][selectedY + 1] = true;	//right down
			if (selectedY - 1 >= 0 && board[selectedX + 1][selectedY - 1] >= 0)
				legalMoves[selectedX + 1][selectedY - 1] = true;	//right up
		}
		if (selectedX - 1 >= 0) { //x-1 left 3 moves
			if (board[selectedX - 1][selectedY] >= 0)
				legalMoves[selectedX - 1][selectedY] = true;	//left
			if (selectedY - 1 >= 0 && board[selectedX - 1][selectedY - 1] >= 0)
				legalMoves[selectedX - 1][selectedY - 1] = true;	//left up
			if (selectedY + 1 <= 7 && board[selectedX - 1][selectedY + 1] >= 0)
				legalMoves[selectedX - 1][selectedY + 1] = true;	//left down
		}
		// x+0
		if (selectedY + 1 <= 7 && board[selectedX][selectedY + 1] >= 0)
			legalMoves[selectedX][selectedY + 1] = true;	//up
		if (selectedY - 1 >= 0 && board[selectedX][selectedY - 1] >= 0)
			legalMoves[selectedX][selectedY - 1] = true;	//down
		break;
	}
}
//535 lines of dog shit code... idk man