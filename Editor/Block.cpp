#include "Block.h"
#include "Engine.h"

extern Engine* engine;

Block::Block():Object() {
	SetBBox(-16,-16,32,32);
	objectIndex=0;
};

Block::~Block() {
};

bool Block::Update() {
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

bool Block::Draw(sf::RenderTarget& RT) {
	sf::RectangleShape RS;
	RS.setPosition(x+bBox.left,y+bBox.top);
	RS.setSize(sf::Vector2f(bBox.width,bBox.height));
	RS.setFillColor(sf::Color(200,0,0,120));
	RT.draw(RS);
	return true;
};