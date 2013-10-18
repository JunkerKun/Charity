#ifndef __PLAYER
#define __PLAYER

#include "Image.h"
#include <SFML/Audio.hpp>
#include "Overlay.h"

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
	float maxHP, maxSP, maxMP;
	float* playerHP,* playerSP,* playerMP;
	Overlay* overlayHP,* overlaySP,* overlayMP;
	bool drawHP, drawSP, drawMP;
	sf::Sprite sprShadow;
	sf::Sprite sprExclamation;
	bool drawExclamation, canPlaySound;
	sf::Sound* sndStep[4];
};

#endif