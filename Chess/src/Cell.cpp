//sfml includes
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
//my includes
#include "headers/Cell.h"

Cell::Cell() {
}


Cell::~Cell() {
}

void Cell::draw(sf::RenderWindow* windowptr) {
	sf::RectangleShape rect(sf::Vector2f(m_width, m_height));
	rect.setPosition(sf::Vector2f(m_x, m_y));
	rect.setFillColor(m_color);
	windowptr->draw(rect);
	piece.draw(windowptr);
}