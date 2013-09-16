#include "Usable.h"
#include "Engine.h"

extern Engine* engine;

Usable::Usable():Object() {
	SetBBox(-14,-14,28,28);
	objectIndex=2;
	functionIndex=0;
	function="none";
	text.setColor(sf::Color::Black);
	text.setCharacterSize(14);
	text.setFont(engine->font);
};

bool Usable::Update() {
	if (grabbed) {
	if (engine->GetInputEvent().type==sf::Event::MouseMoved) {
		if (!engine->input->GetKeyIsPressed(sf::Keyboard::LShift)) {
		x=std::floor(static_cast<float>(engine->input->GetMousePosition(true).x)/32)*32+16;
		y=std::floor(static_cast<float>(engine->input->GetMousePosition(true).y)/32)*32+16;
		} else {
		x=std::floor(static_cast<float>(engine->input->GetMousePosition(true).x)/16)*16+16;
		y=std::floor(static_cast<float>(engine->input->GetMousePosition(true).y)/16)*16+16;
		};
	};
	};
	return true;
};

bool Usable::Draw(sf::RenderTarget &RT) {
	sf::RectangleShape RS;
	RS.setPosition(x+bBox.left,y+bBox.top);
	RS.setSize(sf::Vector2f(bBox.width,bBox.height));
	RS.setFillColor(sf::Color(0,128,255,120));
	RT.draw(RS);
	if (function=="none") {
	text.setString(ToString(functionIndex));
	} else {
		text.setString(function);
	};
	text.setColor(sf::Color::White);
	text.setPosition(floor(x-text.getLocalBounds().width/2),floor(y-10));
	RT.draw(text);
	return true;
};

Usable::~Usable() {

};