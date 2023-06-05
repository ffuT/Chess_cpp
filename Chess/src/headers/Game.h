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
#include "headers/Cell.h"
#include "Texturehandler.h"

class Game {

public:
	Game(const int width_in, const int height_in, const char title_in[]);
	~Game();
	void start();
private:
	void KeyPress();
	void KeyRelease();
	void update();
	void render();
	void inipiece(int x, int y, E_Piece state);

private:
	const int m_width;
	const int m_height;
	const int m_cellAmount = 8;

	const float m_cellwidth = m_width / m_cellAmount;	//dynamic cellsize x
	const float m_cellheight = m_height / m_cellAmount;	//dynamic cellsize y
	const float MAXFPS = 30.0;							//targetframerate		
	const float targetFramerate = 1.0f / MAXFPS;		//target frametime

	int m_mouse_y = 0;
	int m_mouse_x = 0;
	int m_mouse_width = m_width / m_cellAmount;
	int m_mouse_height = m_height / m_cellAmount;
	int fps = 0;
	bool change = true;

	Cell board[8][8];
	TextureHandler txh;

	sf::RenderWindow window;
	sf::RenderWindow* windowptr = nullptr;
	sf::Event ev;
	sf::Font* m_font_arial = new sf::Font;
};

