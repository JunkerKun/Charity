#include "Decoration.h"
#include "Engine.h"

extern Engine* engine;

Decoration::Decoration():Block() {
	objectIndex=3;
	spriteName="none";
	SetBBox(-14,-12,28,24);
	text.setColor(sf::Color::Black);
	text.setCharacterSize(14);
	text.setFont(engine->font);
};

Decoration::~Decoration() {
	
};

bool Decoration::Update() {
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

bool Decoration::Draw(sf::RenderTarget& RT) {
	sf::RectangleShape RS;
	RS.setPosition(x+bBox.left,y+bBox.top);
	RS.setSize(sf::Vector2f(bBox.width,bBox.height));
	RS.setFillColor(sf::Color(200,200,0,120));
	RT.draw(RS);
	text.setString(spriteName);
	text.setColor(sf::Color::White);
	text.setPosition(floor(x-text.getLocalBounds().width/2),floor(y-10));
	RT.draw(text);
	return true;
};