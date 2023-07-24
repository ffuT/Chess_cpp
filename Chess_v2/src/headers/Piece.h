#pragma once
//sfml includes
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
//my includes
#include "Enums.h"

class Piece{
public :
	Piece();
	~Piece();
	
	void draw(sf::RenderWindow* windowptr);
	void setTexture(sf::Texture* texture, int x, int y);
	void setSpriteScale(float scalex, float scaley);

private :
	sf::Sprite sprite;
};