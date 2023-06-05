#pragma once
//this is bad, cus i need the textures ín the pieces,
//but i have the texture handler in the game... pieces dont have access to the game

//sfml includes
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
//my includes
#include "Enums.h"

class Piece{
public :
	Piece(int width, int height);
	Piece(E_Piece piece);
	~Piece();
	
	void draw(sf::RenderWindow* windowptr);
	void setTexture(sf::Texture* texture, int x, int y);
	void setstate(E_Piece state_in);
	E_Piece getstate();

private :
	int spritewidht;
	int spriteheight;
	
	E_Piece m_state = empty;
	sf::Sprite* sprite = new sf::Sprite;

};