#ifndef __PLAYER
#define __PLAYER

#include "Image.h"

class Player: public Image {
public:
	Player(sf::Texture* tex);
	bool Update();
	bool Draw(sf::RenderTarget &RT);
	~Player();
private:
	float speed;
	float direction;
	sf::Sprite sprShadow;
};

#endif