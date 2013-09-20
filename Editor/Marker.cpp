#include "Marker.h"
#include "Engine.h"

extern Engine* engine;

Marker::Marker(int mode):Object() {
	SetBBox(-16,-16,32,32);
	objectIndex=0;
	this->mode=mode;
	draw=false;
	text.setColor(sf::Color::Black);
	text.setCharacterSize(14);
	text.setFont(engine->font);
};

Marker::~Marker() {
};

void Marker::SetTexture(sf::Texture* tex) {
	sprite.setTexture(*tex, false);
	sprite.setTextureRect(sf::IntRect(0,0,32,32));
	draw=true;
};

void Marker::SetCell(int col, int row) {
	sprite.setTextureRect(sf::IntRect(col*32,row*32,32,32));
};

bool Marker::Update() {
	if (grabbed) {
		if (engine->GetInputEvent().type==sf::Event::MouseMoved) {
			if (!engine->input->GetKeyIsPressed(sf::Keyboard::LShift)) {
				x=std::floor(static_cast<float>(engine->input->GetMousePosition(true).x)/32)*32+16;
				y=std::floor(static_cast<float>(engine->input->GetMousePosition(true).y)/32)*32+16;
				sprite.setPosition(x-16,y-16);
			} else {
				x=std::floor(static_cast<float>(engine->input->GetMousePosition(true).x)/16)*16+16;
				y=std::floor(static_cast<float>(engine->input->GetMousePosition(true).y)/16)*16+16;
				sprite.setPosition(x-16,y-16);
			};
		};
	};
	return true;
};

bool Marker::Draw(sf::RenderTarget& RT) {
	if (draw) {
		RT.draw(sprite);
		return true;
	};

	sf::RectangleShape RS;
	RS.setPosition(x+bBox.left,y+bBox.top);
	RS.setSize(sf::Vector2f(bBox.width,bBox.height));
	if (mode==0) {
		RS.setFillColor(sf::Color(0,255,0,60));
	} else if (mode==1) {
		RS.setFillColor(sf::Color(0,0,255,60));
	};
	RT.draw(RS);
	switch(engine->editorObjectIndex) {
	case 0:
		text.setString("Blk");
		break;
	case 1:
		text.setString("Plr");
		break;
	case 2:
		text.setString("Use");
		break;
	case 3:
		text.setString("Dec");
		break;
	case 4:
		text.setString("Trg");
		break;
	case 5:
		text.setString("Plc");
		break;
	};
	text.setPosition(x-8,y-10);
	RT.draw(text);
	return true;
};