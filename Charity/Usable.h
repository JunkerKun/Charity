#ifndef __USABLE
#define __USABLE

#include <string>
#include "Block.h"
#include <SFML/Graphics.hpp>

class Usable: public Object {
public:
	Usable();
	~Usable();
	bool Update();
	bool Draw(sf::RenderTarget &RT);
	std::string function;
};

#endif