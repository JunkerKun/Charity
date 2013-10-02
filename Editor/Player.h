#ifndef __PLAYER
#define __PLAYER

#include "Image.h"

class Player: public Image {
public:
	Player(char* path);
	Player(sf::Texture &tex);
	bool Update();
	~Player();
private:
	float speed;
	float direction;
};

#endif