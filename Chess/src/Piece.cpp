//sfml includes
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
//my includes
#include "headers/Piece.h"

Piece::Piece(int width, int height) {
	spritewidht = width;
	spriteheight = height;
}

Piece::Piece(E_Piece piece) {
	m_state = piece;
}

Piece::~Piece() {
	delete sprite;
}

E_Piece Piece::getstate() {
	return m_state;
}

void Piece::setstate(E_Piece state_in) {
	m_state = state_in;
}

void Piece::setTexture(sf::Texture* texture, int x, int y) {
	sprite->setTexture(*texture);
	sprite->setPosition(sf::Vector2f(x, y));
}

void Piece::draw(sf::RenderWindow* windowptr) {
	windowptr->draw(*sprite);
}