#ifndef __DECORATION
#define __DECORATION

#include "Image.h"

class Decoration: public Image {
public:
	Decoration(sf::Texture* tex);
	bool Update();
	bool Draw(sf::RenderTarget& RT);
	~Decoration();
	std::string spriteName;
};

#endif