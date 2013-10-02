#include "Usable.h"
#include "Engine.h"

extern Engine* engine;

Usable::Usable():Object() {
	objectIndex=2;
	SetBBox(-28,-28,56,56);
	function="";
};

bool Usable::Update() {
	return true;
};

bool Usable::Draw(sf::RenderTarget &RT) {
	if (engine->debug) {
	sf::RectangleShape RS;
	RS.setPosition(x+bBox.left,y+bBox.top);
	RS.setSize(sf::Vector2f(bBox.width,bBox.height));
	RS.setFillColor(sf::Color(0,128,255,120));
	RT.draw(RS);
	};
	return true;
};

Usable::~Usable() {

};