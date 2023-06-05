#pragma once
//sfml includes
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
//my includes
#include "PieceEnum.h"

class Piece{
public :
	Piece();
	Piece(E_Piece piece);
	~Piece();
	
	void draw(sf::RenderWindow* windowptr);
	void update();

public :
	E_Piece m_state = empty;

};

