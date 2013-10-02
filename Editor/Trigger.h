#ifndef __TRIGGER
#define __TRIGGER

#include "Object.h"
#include <SFML\Graphics.hpp>

class Trigger : public Object {
public:
	Trigger();
	~Trigger();
	bool Update();
	bool Draw(sf::RenderTarget &RT);
	std::string function;
private:
	sf::Text text;
};

#endif