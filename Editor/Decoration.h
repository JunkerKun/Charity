#ifndef __DECORATION
#define __DECORATION

#include "Block.h"

class Decoration: public Block {
public:
	Decoration();
	bool Update();
	bool Draw(sf::RenderTarget& RT);
	~Decoration();
	std::string spriteName;
private:
	sf::Text text;
};

#endif