#ifndef __NPC
#define __NPC

#include "Image.h"
#include <SFML/Graphics.hpp>

class Npc: public Image {
public:
	Npc(sf::Texture* tex);
	~Npc();
	bool Update();
	bool Draw(sf::RenderTarget& RT);
	int index;
	float direction;
private:
	sf::Sprite sprShadow;
};

#endif