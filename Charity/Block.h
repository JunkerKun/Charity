#ifndef __BLOCK
#define __BLOCK

#include "Object.h"

class Block:public Object {
public:
	Block();
	bool Update();
	bool Draw(sf::RenderTarget& RT);
	~Block();
};

#endif