#include "Decoration.h"
#include "Engine.h"

extern Engine* engine;

Decoration::Decoration(sf::Texture* tex):Image(tex) {
	objectIndex=0;
	SetBBox(-engine->gridSize/2+engine->gridSize/32*2,-engine->gridSize/2+engine->gridSize/32*4,
		engine->gridSize-engine->gridSize/32*4,engine->gridSize-engine->gridSize/32*8);
	//SetBBox(-28,-25,56,48);
	solid=true;
	spriteName="none";
	SetOrigin(imageWidth/2,imageHeight-10*2);
};

Decoration::~Decoration() {
	
};

bool Decoration::Update() {
	Image::Update();
	return true;
};

bool Decoration::Draw(sf::RenderTarget& RT) {
	Image::Draw(RT);
	return true;
};