#ifndef __RESOURCESMANAGER
#define __RESOURCESMANAGER

#include <SFML\Graphics.hpp>
#include <map>

class ResourcesManager {
public:
	ResourcesManager();
	~ResourcesManager();
	bool AddTexture(std::string name, std::string path);
	sf::Texture* GetTexture(std::string name);
	sf::Texture* GetTexture(int index);
	std::string GetTextureName(int index);
	int GetTexturesNumber();
private:
	std::map<std::string,sf::Texture*>* texturesList;
};

#endif