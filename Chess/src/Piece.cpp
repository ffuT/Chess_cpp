//sfml includes
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
//my includes
#include "headers/Piece.h"

Piece::Piece() {

}

Piece::Piece(E_Piece piece) {
	m_state = piece;
}

Piece::~Piece() {

}

void Piece::update() {
	//yes
}

void Piece::draw(sf::RenderWindow* windowptr) {
	switch (m_state) {
	case empty:
		return;
	case pawn:
		//update sprite to pawn
		break;
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
	default:
		break;
	}
	//windowptr->draw(sprite);
}