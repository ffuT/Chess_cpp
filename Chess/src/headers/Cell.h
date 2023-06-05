#pragma once
//sfml includes
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
//my includes
#include "Piece.h"

class Cell{
public:
	
	Cell();
	~Cell();

	void draw(sf::RenderWindow* windowptr);
	void update();

	int m_x = 0;
	int m_y = 0;
	int m_width = 0;
	int m_height = 0;

	Piece piece = Piece(m_width, m_height);
	sf::Color m_color = sf::Color(0, 0, 0);

};