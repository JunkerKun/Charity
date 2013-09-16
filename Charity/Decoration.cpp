#include "Decoration.h"
#include "Engine.h"

extern Engine* engine;

Decoration::Decoration(sf::Texture* tex):Image(tex) {
	objectIndex=0;
	SetBBox(-14,-12,28,24);
	solid=true;
	spriteName="none";
	SetOrigin(imageWidth/2,imageHeight-10);
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