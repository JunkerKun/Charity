#include "Tile.h"
#include "Engine.h"

extern Engine* engine; 

Tile::Tile(std::string tn, int xx, int yy, int col2, int row2) {
	x=xx;
	y=yy;
	col=col2;
	row=row2;
	texName=tn;

	sprite.setTexture(*engine->resourcesManager->GetTexture(tn), false);
	sprite.setTextureRect(sf::IntRect(engine->gridSize*col2,engine->gridSize*row2,engine->gridSize,engine->gridSize));
	//sprite.setTextureRect(sf::IntRect(64*col2,64*row2,64,64));
	sprite.setPosition(x,y);

	depth=0;
	layer=0;
};

bool Tile::Draw(sf::RenderTarget &rt) {
	rt.draw(sprite);
	return true;
};

Tile::~Tile() {

};