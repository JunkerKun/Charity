#ifndef __USABLE
#define __USABLE

#include <string>
#include "Object.h"
#include <SFML/Graphics.hpp>

class Usable: public Object {
public:
	Usable();
	~Usable();
	bool Update();
	bool Draw(sf::RenderTarget &RT);
	std::string function;
	int functionIndex;
private:
	sf::Text text;
};

#endif