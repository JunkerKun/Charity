#ifndef __PLAYER
#define __PLAYER

#include "Image.h"
#include <SFML/Audio.hpp>

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
	int direction;
	int hp, maxHp;
	sf::Sprite sprShadow;
	sf::Sprite sprExclamation;
	bool drawExclamation, canPlaySound;
	sf::Sound* sndStep[4];
};

#endif