#include "Block.h"
#include "Engine.h"

extern Engine* engine;

Block::Block():Object() {
	SetBBox(-16,-16,32,32);
	objectIndex=0;
	visible=engine->debug;
	solid=true;
};

Block::~Block() {
};

bool Block::Update() {
	return true;
};

bool Block::Draw(sf::RenderTarget& RT) {
	if (visible) {
	sf::RectangleShape RS;
	RS.setPosition(x+bBox.left,y+bBox.top);
	RS.setSize(sf::Vector2f(bBox.width,bBox.height));
	RS.setFillColor(sf::Color(200,0,0,120));
	RT.draw(RS);
	};
	return true;
};