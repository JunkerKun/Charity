#ifndef __PLAYER
#define __PLAYER

#include "Image.h"

class Player: public Image {
public:
	Player(sf::Texture* tex);
	bool Update();
	bool Draw(sf::RenderTarget &RT);
	void SetDirection(int dir);
	int GetDirection();
	~Player();
	Object* collisionTrigger;
private:
	float speed;
	float direction;
	int hp;
	sf::Sprite sprShadow;
	sf::Sprite sprExclamation;
	bool drawExclamation;
};

#endif