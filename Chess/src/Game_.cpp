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

Game::Game(const int width_in, const int height_in, const char title_in[]) :
	m_width(width_in), m_height(height_in),
	window(sf::VideoMode(width_in, height_in), title_in, sf::Style::Default), ev() {
	windowptr = &window;
	
	m_font_arial.loadFromFile("Resources/Extra/arial.ttf");

	//initialize board here:
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
		}
	}
	//maybe do more shit heres

}

Game::~Game() {

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
				std::cout << "mousecell: " << m_mouse_x << ", " << m_mouse_y << "\n";
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
					std::cout << fps << "\n";
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
		sf::Text text(i, m_font_arial, 24);
		text.setPosition(sf::Vector2f((i - 65) * m_cellwidth + m_cellwidth - 32, m_height - 32));
		window.draw(text);
	}

	//draw numbers based on ascii values 49 = '1'
	for (char i = 48; i <= 48 + 8; i++) {
		sf::Text text(i, m_font_arial, 24);
		text.setPosition(sf::Vector2f(4, m_cellheight * 8 - (i-48) * m_cellheight));
		window.draw(text);
	}

	window.display();
	change = false;

}