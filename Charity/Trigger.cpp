#include "Trigger.h"
#include "Engine.h"

extern Engine* engine;

Trigger::Trigger():Object() {
	objectIndex=4;
	SetBBox(-engine->gridSize/2,-engine->gridSize/2,engine->gridSize,engine->gridSize);
	//SetBBox(-32,-32,64,64);
	function="";
};

bool Trigger::Update() {
	return true;
};

bool Trigger::Draw(sf::RenderTarget &RT) {
	if (engine->debug) {
		sf::RectangleShape RS;
		RS.setPosition(x+bBox.left,y+bBox.top);
		RS.setSize(sf::Vector2f(bBox.width,bBox.height));
		RS.setFillColor(sf::Color(0,0,200,120));
		RT.draw(RS);
	};
	return true;
};

Trigger::~Trigger() {

};