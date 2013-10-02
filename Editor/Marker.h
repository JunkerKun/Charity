#ifndef __MARKER
#define __MARKER

#include "Object.h"

class Marker:public Object {
public:
	Marker(int mode=0);
	bool Update();
	bool Draw(sf::RenderTarget& RT);
	void SetTexture(sf::Texture* tex);
	void SetCell(int col, int row);
	~Marker();
	sf::Texture* tex;
private:
	int mode;
	sf::Sprite sprite;
	sf::Text text;
	bool draw;
};

#endif