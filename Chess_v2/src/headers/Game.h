#pragma once
//sfml includes
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
//cpp includess
#include <math.h>
#include <chrono>
//my includes
#include "Texturehandler.h"
#include "Piece.h"

class Game {

public:
	Game(const int width_in, const int height_in, const char title_in[]);
	
	~Game();
	void start();
	void stop();
private:
	void KeyPress();
	void KeyRelease();
	void render();
	void inipiece(int x, int y, E_Piece state);
	void mouseClicked();
	void calcLegalMoves();
	void clearLegalMoves();

private:
	//dimensions
	const int m_width;
	const int m_height;
	const int m_cellAmount = 8;
	const float m_cellwidth = m_width / m_cellAmount;	//dynamic cellsize x
	const float m_cellheight = m_height / m_cellAmount;	//dynamic cellsize y
	
	//gameloop
	bool running;
	const float MAXFPS = 24.0;	//targetframerate		
	const float targetFramerate = 1.0f / MAXFPS;	//target frametime
	bool change = true;
	
	//mouse pos
	int m_mouse_y;
	int m_mouse_x;
	int m_mouse_width = m_width / m_cellAmount;
	int m_mouse_height = m_height / m_cellAmount;
	
	//game logic
	bool whiteTurn = true;
	bool cellSelected = false;
	int selectedX=7, selectedY=7;
	int board[8][8];
	bool legalMoves[8][8];
	
	//rendering
	sf::Font* m_font_arial = new sf::Font;
	sf::RectangleShape backGroundRect = sf::RectangleShape(sf::Vector2f(m_cellwidth, m_cellheight));
	TextureHandler txh;
	Piece piece;
	
	//window
	sf::RenderWindow window;
	sf::RenderWindow* windowptr;
	sf::Event ev;
};