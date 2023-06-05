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

class TextureHandler {
public:
	TextureHandler();
	~TextureHandler();

	sf::Texture* GetTexture(E_Piece texture);
private:
	const std::string PATH = "Resources/Textures/";
	void LoadTexture(sf::Texture*, const char[]);
	
	//list all textures:
	sf::Texture* pawnW = new sf::Texture;
	sf::Texture* pawnB = new sf::Texture;
	sf::Texture* bishopW = new sf::Texture;
	sf::Texture* bishopB = new sf::Texture;
	sf::Texture* knightW = new sf::Texture;
	sf::Texture* knightB = new sf::Texture;
	sf::Texture* rookW = new sf::Texture;
	sf::Texture* rookB = new sf::Texture;
	sf::Texture* queenW = new sf::Texture;
	sf::Texture* queenB = new sf::Texture;
	sf::Texture* kingW = new sf::Texture;
	sf::Texture* kingB = new sf::Texture;
};

