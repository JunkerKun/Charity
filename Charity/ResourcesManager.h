#ifndef __RESOURCESMANAGER
#define __RESOURCESMANAGER

#include <SFML\Graphics.hpp>
#include <map>

struct Font {
	sf::Font font;
	int size;
	sf::Color color;
	Font(int siz, int r, int g, int b) {
		size=16;
		color=sf::Color(r,g,b,255);
	};
};

class ResourcesManager {
public:
	ResourcesManager();
	~ResourcesManager();
	bool AddTexture(std::string name, std::string path);
	void DesaturateTexture(std::string name);
	sf::Texture* GetTexture(std::string name);
	sf::Texture* GetTexture(int index);
	bool DeleteTexture(std::string name);
	std::string GetTextureName(int index);
	int GetTexturesNumber();
	bool AddFont(int type, std::string path, int size=16, int r=255, int g=255, int b=255);
	Font* GetFont(int type); 
private:
	std::map<std::string,sf::Texture*>* texturesList;
	Font* fontText;
};

#endif