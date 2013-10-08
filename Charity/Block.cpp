#include "Block.h"
#include "Engine.h"

extern Engine* engine;

Block::Block():Object() {
	SetBBox(-engine->gridSize/2,-engine->gridSize/2,engine->gridSize,engine->gridSize);
	//SetBBox(-32,-32,64,64);
	objectIndex=0;
	visible=false;
	solid=true;
};

Block::~Block() {
};

bool Block::Update() {
	return true;
};

bool Block::Draw(sf::RenderTarget& RT) {
	if (engine->debug) {
		sf::RectangleShape RS;
		RS.setPosition(x+bBox.left,y+bBox.top);
		RS.setSize(sf::Vector2f(bBox.width,bBox.height));
		RS.setFillColor(sf::Color(200,0,0,120));
		RT.draw(RS);
	};
	return true;
};