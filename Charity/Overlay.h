#ifndef __OVERLAY
#define __OVERLAY

#include <SFML/Graphics.hpp>
#include "Image.h"

class Overlay: public Image {
public:
	Overlay(sf::Texture* tex);
	~Overlay();
	bool Update();
	bool Draw(sf::RenderTarget& rt);
private:
	//int *variable;
};

#endif;