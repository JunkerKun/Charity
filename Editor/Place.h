#ifndef __PLACE
#define __PLACE

#include "Object.h"

class Place:public Object {
public:
	Place();
	bool Update();
	bool Draw(sf::RenderTarget& RT);
	~Place();
	int place;
private:
	sf::Text text;
};

#endif