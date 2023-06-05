//shits fucked idk cant name it Game.cpp cuz "class not registered" idk phind.com said dll error

//i've tried to optimize gameloop using waitevent and a bool called change
//its pretty bad but it works

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
#include "headers/Cell.h"
#include "headers/Texturehandler.h"

Game::Game(const int width_in, const int height_in, const char title_in[]) :
	m_width(width_in), m_height(height_in),
	window(sf::VideoMode(width_in, height_in), title_in, sf::Style::Default), ev() {
	windowptr = &window;
	
	m_font_arial->loadFromFile("Resources/Extra/arial.ttf");

	//initialize board (2d cell array) here
	for (int x = 0; x <= 7; x++) {
		for (int y = 0; y <= 7; y++) {
			board[x][y].m_width = m_cellwidth;			
			board[x][y].m_height = m_cellheight;		
			board[x][y].m_x = (int) x * m_cellwidth;	
			board[x][y].m_y = (int) y * m_cellheight;
			if ((x + y) % 2 != 0) {
				board[x][y].m_color = sf::Color(40,25,25);
			} else {
				board[x][y].m_color = sf::Color(230,225,200);
			}
			board[x][y].piece.setSpriteScale(m_cellwidth / 64, m_cellheight / 64);
		}
	}

	//initialize pieces
	//black pieces
	for (int i = 0; i <= 7; i++) { inipiece(i, 1, bpawn); }
	inipiece(0, 0, brook);
	inipiece(7, 0, brook);
	inipiece(1, 0, bknight);
	inipiece(6, 0, bknight);
	inipiece(2, 0, bbishop);
	inipiece(5, 0, bbishop);
	inipiece(3, 0, bqueen);
	inipiece(4, 0, bking);
	//white
	for (int i = 0; i <= 7; i++) {inipiece(i, 6, pawn);}
	inipiece(0, 7, rook);
	inipiece(7, 7, rook);
	inipiece(1, 7, knight);
	inipiece(6, 7, knight);
	inipiece(2, 7, bishop);
	inipiece(5, 7, bishop);
	inipiece(3, 7, queen);
	inipiece(4, 7, king);
}

Game::~Game() {

}

void Game::inipiece(int x, int y, E_Piece state) {
	board[x][y].piece.setstate(state);
	board[x][y].piece.setTexture(txh.GetTexture(board[x][y].piece.getstate()), board[x][y].m_x, board[x][y].m_y);
}

void Game::start() {
	//gameloop
	auto onesec = std::chrono::steady_clock::now() + std::chrono::seconds(1);
	auto lastTime = std::chrono::steady_clock::now();
	float accumulatedTime = 0.0f;
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
				//perhaps this should be in a function
				m_mouse_x = sf::Mouse::getPosition(window).x / m_mouse_width;
				m_mouse_y = sf::Mouse::getPosition(window).y / m_mouse_height;
				std::cout << "mousecell: " << m_mouse_x << ", " << m_mouse_y << ". piece: "
						  << board[m_mouse_x][m_mouse_y].piece.getstate() << "\n";
				//TODO make click do shit
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
				break;
			}
			update();
		}
		//limit render if constant window events
		while (accumulatedTime >= targetFramerate) {
			render();
			accumulatedTime -= targetFramerate;
			if (onesec <= now) {
				onesec += std::chrono::seconds(1);
				if (fps != 0)
					std::cout << "fps: " << fps << "\n";
				fps = 0;
			}
		}
		lastTime = now;
		accumulatedTime += deltaTime;
	}
}

void Game::KeyPress() {
	auto keycode = ev.key.code;
	if (keycode == sf::Keyboard::Escape) {	//close on escape
		window.close();
	}
}

void Game::KeyRelease() {}

void Game::update() {
	change = true;
}

void Game::render() {
	if (!change) //uses less graphics idk maybe good maybe bad
		return;
	fps++;

	window.clear(sf::Color(33, 33, 33));
	//draw here

	//draw the background
	for (int x = 0; x <= 7; x++) {
		for (int y = 0; y <= 7; y++) {
			board[x][y].draw(windowptr);
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