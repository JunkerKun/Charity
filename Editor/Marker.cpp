#include "Marker.h"
#include "Engine.h"

extern Engine* engine;

Marker::Marker(int mode):Object() {
	SetBBox(-gridSize/2,-gridSize/2,gridSize,gridSize);
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
	this->tex=tex;
	sprite.setTexture(*tex, false);
	sprite.setTextureRect(sf::IntRect(0,0,gridSize,gridSize));
	draw=true;
};

void Marker::SetCell(int col, int row) {
	sprite.setTextureRect(sf::IntRect(col*gridSize,row*gridSize,gridSize,gridSize));
};

bool Marker::Update() {
	if (grabbed) {
		if (engine->GetInputEvent().type==sf::Event::MouseMoved) {
			if (!engine->input->GetKeyIsPressed(sf::Keyboard::LShift)) {
				x=std::floor(static_cast<float>(engine->input->GetMousePosition(true).x)/gridSize)*gridSize+(gridSize/2);
				y=std::floor(static_cast<float>(engine->input->GetMousePosition(true).y)/gridSize)*gridSize+(gridSize/2);
				sprite.setPosition(x-gridSize/2,y-gridSize/2);
			} else {
				x=std::floor(static_cast<float>(engine->input->GetMousePosition(true).x)/(gridSize/2))*(gridSize/2)+(gridSize/2);
				y=std::floor(static_cast<float>(engine->input->GetMousePosition(true).y)/(gridSize/2))*(gridSize/2)+(gridSize/2);
				sprite.setPosition(x-gridSize/2,y-gridSize/2);
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
	RS.setPosition(x,y);
	RS.setSize(sf::Vector2f(2,2));
	RS.setFillColor(sf::Color(255,255,255,255));
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
	text.setPosition(x-8*gridSize/32,y-10*gridSize/32);
	RT.draw(text);
	return true;
};