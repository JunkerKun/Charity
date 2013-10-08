#ifndef __RESOURCESMANAGER
#define __RESOURCESMANAGER

#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
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

struct Sound {
	sf::SoundBuffer* buffer;
	sf::Sound* sound;
	Sound(std::string path) {
		buffer = new sf::SoundBuffer();
		buffer->loadFromFile(path);
		sound = new sf::Sound();
		sound->setBuffer(*buffer);
	};
	~Sound() {
		delete sound;
		delete buffer;
	};
};

class ResourcesManager {
public:
	ResourcesManager();
	~ResourcesManager();
	bool AddTexture(std::string name, std::string path, std::vector<int>* settings, bool global=false);
	bool AddSound(std::string name, std::string path, bool global=false);
	void DesaturateTexture(std::string name);
	void ColorizeTexture(std::string name, int mode, sf::Color color);
	sf::Texture* GetTexture(std::string name);
	sf::Texture* GetTexture(int index);
	std::vector<int>* GetTextureSettings(std::string name);
	Sound* GetSound(std::string name);
	bool DeleteTexture(std::string name);
	bool DeleteSound(std::string name);
	std::string GetTextureName(int index);
	std::string GetTextureName(sf::Texture* tex);
	int GetTexturesNumber();
	bool AddFont(int type, std::string path, int size=16, int r=255, int g=255, int b=255);
	Font* GetFont(int type); 
	void ClearTextures();
	void ClearSounds();
	void ClearAll();

	sf::Music bgMusic;
private:
	std::map<std::string,sf::Texture*>* texturesListGlobal;
	std::map<std::string,Sound*>* soundsListGlobal;
	std::map<std::string,sf::Texture*>* texturesList;
	std::map<std::string,Sound*>* soundsList;
	std::map<std::string,std::vector<int>*>* texturesSettings;
	Font* fontText;
};

#endif