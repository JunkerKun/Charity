#ifndef __OBJECT
#define __OBJECT
#include "SFML\Graphics.hpp"

class Object {
public:
	Object();
	virtual ~Object()=0;
	unsigned int objectIndex; //0-Block; 1-Player; 3-Usable;
	sf::Vector2i chunk;
	bool visible;
	int depth;
	virtual bool Update()=0;
	virtual bool Draw(sf::RenderTarget& RT)=0;
	sf::Rect<sf::Int16> GetBBox();
	bool SetBBox(int left, int top, int width, int height);
	float x,y, xPrev, yPrev;
	void SetPosition(float xx, float yy);
	void MoveToChunk();
	void Grab(bool grab=true);
	bool grabbed;
protected:
	sf::Rect<sf::Int16> bBox;
};

#endif