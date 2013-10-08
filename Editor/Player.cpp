#include "Player.h"
#include "Engine.h"

extern Engine* engine;

Player::Player(char* path):Image(path) {
	objectIndex=1;
	direction=0;
	SetBBox(-gridSize/2+gridSize/32*2,-gridSize/2+gridSize/32*7,gridSize-gridSize/32*4,gridSize-gridSize/32*12);
	//SetBBox(-28,-18,56,36);
	AddSequence(0,0,0.25);
};

Player::Player(sf::Texture &tex):Image(tex) {
	objectIndex=1;
	direction=0;
	SetBBox(-gridSize/2-gridSize/32*2,-gridSize/2-gridSize/32*7,gridSize-gridSize/32*4,gridSize-gridSize/32*12);
	//SetBBox(-28,-24,56,48);
	AddSequence(0,0,0.25);
};

bool Player::Update() {
if (grabbed) {
		if (engine->GetInputEvent().type==sf::Event::MouseMoved) {
			if (!engine->input->GetKeyIsPressed(sf::Keyboard::LShift)) {
				x=std::floor(static_cast<float>(engine->input->GetMousePosition(true).x)/gridSize)*gridSize+(gridSize/2);
				y=std::floor(static_cast<float>(engine->input->GetMousePosition(true).y)/gridSize)*gridSize+(gridSize/2);
			} else {
				x=std::floor(static_cast<float>(engine->input->GetMousePosition(true).x)/(gridSize/2))*(gridSize/2)+(gridSize/2);
				y=std::floor(static_cast<float>(engine->input->GetMousePosition(true).y)/(gridSize/2))*(gridSize/2)+(gridSize/2);
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