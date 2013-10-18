#include "Engine.h"
#include "Overlay.h"

Overlay::Overlay(sf::Texture *tex):Image(tex) {
};

Overlay::~Overlay() {
};

bool Overlay::Update() {
	Image::Update();
	return true;
};

bool Overlay::Draw(sf::RenderTarget &rt) {
	Image::Draw(rt);
return true;
};