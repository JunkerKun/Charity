#include "Block.h"
#include "Engine.h"

extern Engine* engine;

Place::Place():Object() {
	SetBBox(-16,-16,32,32);
	objectIndex=5;
	place=0;
	text.setColor(sf::Color::Black);
	text.setCharacterSize(14);
	text.setFont(engine->font);
};

Place::~Place() {
};

bool Place::Update() {
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

bool Place::Draw(sf::RenderTarget& RT) {
	sf::RectangleShape RS;
	RS.setPosition(x+bBox.left,y+bBox.top);
	RS.setSize(sf::Vector2f(bBox.width,bBox.height));
	RS.setFillColor(sf::Color(100,255,100,120));
	RT.draw(RS);
	text.setString(ToString(place));
	text.setColor(sf::Color::White);
	text.setPosition(floor(x-text.getLocalBounds().width/2),floor(y-10));
	RT.draw(text);
	return true;
};