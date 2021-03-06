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
	bool Load(sf::Texture* tex, std::vector<int>* sets);
	bool Update();
	bool Draw(sf::RenderTarget& RT);
	bool SetFrame(int frame);
	bool SetOrigin(int left, int top);
	bool AddSequence(float first, float last, float speed);
	bool SetSequence(int sequence, bool force=false);
	void ClearSequences();
	bool LoadSequences(std::string path);
	bool LoadSettings(std::string path);
	int imageWidth, imageHeight;
	void SetTexture(sf::Texture* tex);
	sf::Texture& GetTexture();
	sf::Sprite& GetSprite();
	~Image();
protected:
	sf::Texture texture;
	sf::Sprite sprite;
	float imageFrame, imageSpeed;
	int firstFrame, lastFrame, seq, seqOld, temp;
	std::map<int,std::vector<float>> sequences;
};

#endif