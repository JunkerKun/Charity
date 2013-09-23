#ifndef __IMAGE
#define __IMAGE
#include "Object.h"
#include <map>
#include "SFML\Graphics.hpp"

class Image: public Object {
public:
	Image(char* path);
	Image(sf::Texture* tex);
	bool Load(char* path);
	bool Load(sf::Texture* tex);
	bool Update();
	bool Draw(sf::RenderTarget& RT);
	bool SetFrame(int frame);
	bool SetOrigin(int left, int top);
	bool AddSequence(float first, float last, float speed);
	bool SetSequence(int sequence, bool force=false);
	int imageWidth, imageHeight;
	void SetTexture(sf::Texture* tex);
	~Image();
private:
	sf::Texture texture;
	sf::Sprite sprite;
	float imageFrame, imageSpeed;
	int firstFrame, lastFrame, seq, seqOld, temp;
	std::map<int,std::vector<float>> sequences;
};

#endif