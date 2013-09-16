#include "Player.h"
#include "Engine.h"

extern Engine* engine;

Player::Player(char* path):Image(path) {
	objectIndex=1;
	direction=0;
	SetBBox(-14,-12,28,24);
	AddSequence(0,0,0.25);
};

Player::Player(sf::Texture &tex):Image(tex) {
	objectIndex=1;
	direction=0;
	SetBBox(-14,-12,28,24);
	AddSequence(0,0,0.25);
};

bool Player::Update() {
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

	MoveToChunk();

	depth=y+16;
	SetSequence(0,1);
	Image::Update();
	
	return true;
};

Player::~Player() {

};