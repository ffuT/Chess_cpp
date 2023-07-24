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

void Game::start() {
	//gameloop
	auto onesec = std::chrono::steady_clock::now() + std::chrono::seconds(1);
	auto lastTime = std::chrono::steady_clock::now();
	float accumulatedTime = 0.0f;

	render(); //render before the loop cuz of waitevent

	while (window.isOpen()) {		//window loop
		auto now = std::chrono::steady_clock::now();
		auto deltaTime = std::chrono::duration<float>(now - lastTime).count();

		//window events
		if (window.waitEvent(ev)) {
			switch (ev.type) {
			case sf::Event::Closed:
				window.close();
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
	if (!cellSelected) {

		selectedX = m_mouse_x;
		selectedY = m_mouse_y;
		calcLegalMoves();

		if(board[selectedX][selectedY] != 0){  //cant select empty cells
			std::cout << static_cast<char>(selectedX + 65) << abs(selectedY - 8) << " selected\n";
			cellSelected = true;
		}

	} else {
			if(legalMoves[m_mouse_x][m_mouse_y]) {
				std::cout << static_cast<char>(selectedX + 65) << abs(selectedY - 8) <<
					" moved to: " << static_cast<char>(m_mouse_x + 65) <<
					abs(m_mouse_y - 8) << "\n";

				board[m_mouse_x][m_mouse_y] = board[selectedX][selectedY];
				board[selectedX][selectedY] = E_Piece::empty;

				cellSelected = false;
			}
			else if (m_mouse_x == selectedX && m_mouse_y == selectedY) {
				cellSelected = false;
			}

			for (auto& rows : legalMoves) //clear legalmoves
			{
				for (auto& elem : rows)
				{
					elem = false;
				}
			}
	}
}

void Game::calcLegalMoves() {
	for (auto& rows : legalMoves) // Iterating over rows
	{
		for (auto& elem : rows)
		{
			elem = false;
		}
	}

	//i need to switch cause pawn are diffrent
	switch (board[selectedX][selectedY]) {
	case empty:
		return;
	case bpawn:
		legalMoves[selectedX][selectedY + 1] = true;
		return;
	case pawn:
		legalMoves[selectedX][selectedY - 1] = true;
		return;
	}
	switch (abs(board[selectedX][selectedY])){
	case bishop:
		break;
	case knight:
		break;
	case rook:
		break;
	case queen:
		break;
	case king:
		break;
	}
}

void Game::KeyPress() {
	auto keycode = ev.key.code;
	if (keycode == sf::Keyboard::Escape) {	//close on escape
		window.close();
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
					backGroundRect.setFillColor(sf::Color(177, 220, 140));
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
	change = false;
}