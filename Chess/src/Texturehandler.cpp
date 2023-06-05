#include "headers/Texturehandler.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

//return a pointer to the specified texture
sf::Texture* TextureHandler::GetTexture(E_Piece texture) {
	switch (texture){
	case bking:
		return kingB;
	case bqueen:
		return queenB;
	case brook:
		return rookB;
	case bknight:
		return knightB;
	case bbishop:
		return bishopB;
	case bpawn:
		return pawnB;
	case empty:
		return pawnW;
	case pawn:
		return pawnW;
	case bishop:
		return bishopW;
	case knight:
		return knightW;
	case rook:
		return rookW;
	case queen:
		return queenW;
	case king:
		return kingW;
	}

	return nullptr;	//should never happen
}

//load from file
void TextureHandler::LoadTexture(sf::Texture* inTexture, const char filename[]) {
	if (!inTexture->loadFromFile(PATH + filename))
		std::exit(1);
}

//load textures
TextureHandler::TextureHandler() {
	LoadTexture(pawnW, "pawnW.png");
	LoadTexture(pawnB, "pawnB.png");
	LoadTexture(bishopW, "bishopW.png");
	LoadTexture(bishopB, "bishopB.png");
	LoadTexture(knightW, "knightW.png");
	LoadTexture(knightB, "knightB.png");
	LoadTexture(rookW, "rookW.png");
	LoadTexture(rookB, "rookB.png");
	LoadTexture(queenW, "queenW.png");
	LoadTexture(queenB, "queenB.png");
	LoadTexture(kingW, "kingW.png");
	LoadTexture(kingB, "kingB.png");
}

//delete alle the textures
TextureHandler::~TextureHandler() {
	delete pawnW;
	delete pawnB;
	delete bishopW;
	delete bishopB;
	delete knightW;
	delete knightB;
	delete rookW;
	delete rookB;
	delete queenW;
	delete queenB;
	delete kingW;
	delete kingB;
}