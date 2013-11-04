#include "Npc.h"
#include "Engine.h"

extern Engine* engine;

Npc::Npc(sf::Texture* tex):Image(tex) {
	locked=true;
	direction=0;
	objectIndex=6;
	index=0;

	SetOrigin(imageWidth/2,imageHeight-11);

	SetBBox(-engine->gridSize/2+engine->gridSize/32*4,-engine->gridSize/2+engine->gridSize/32*8,
		engine->gridSize-engine->gridSize/32*8,engine->gridSize-engine->gridSize/32*15);

	AddSequence(0,0,0.25);
	AddSequence(5,5,0.25);
	AddSequence(10,10,0.25);
	AddSequence(15,15,0.25);

	AddSequence(1,4,0.2);
	AddSequence(6,9,0.2);
	AddSequence(11,14,0.2);
	AddSequence(16,19,0.2);

	SetSequence(0,1);

	sprShadow.setTexture(*engine->resourcesManager->GetTexture("sprShadow"),true);
	sprShadow.setOrigin(engine->resourcesManager->GetTexture("sprShadow")->getSize().x/2,
		engine->resourcesManager->GetTexture("sprShadow")->getSize().y/2-1*2);
};

Npc::~Npc() {
};

bool Npc::Update() {
	xPrev=x;
	yPrev=y;
	SetSequence(floor(direction)+4*(isMoving));
	Image::Update();
	sprShadow.setPosition(floor(x),floor(y));
	locked=false;
	MoveToChunk();
	return true;
};

bool Npc::Draw(sf::RenderTarget &RT) {
	if (!visible || locked) return true;
	RT.draw(sprShadow);
	Image::Draw(RT);
	return true;
};

void Npc::SetDirection(int dir) {
	direction=dir;
};

int Npc::GetDirection() {
	return direction;
};