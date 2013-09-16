#include "ResourcesManager.h"

ResourcesManager::ResourcesManager() {
	texturesList= new std::map<std::string,sf::Texture*>;
	fontText= new sf::Font;
};

ResourcesManager::~ResourcesManager() {
	for (std::map<std::string,sf::Texture*>::iterator it=texturesList->begin(); it!=texturesList->end(); it++) {
		delete it->second;
	};
	texturesList->clear();
	delete texturesList;
	delete fontText;
};

bool ResourcesManager::AddTexture(std::string name, std::string path) {
	sf::Texture* tex=new sf::Texture;
	tex->setSmooth(false);
	tex->loadFromFile(path);
	texturesList->insert(std::pair<std::string,sf::Texture*>(name,tex));
	return true;
};

sf::Texture* ResourcesManager::GetTexture(std::string name) {
	return texturesList->find(name)->second;
};

sf::Texture* ResourcesManager::GetTexture(int index) {
	std::map<std::string,sf::Texture*>::iterator it=texturesList->begin();
	for(int i=0;i<index;i++) {
		it++;
	};
	return it->second;
};

std::string ResourcesManager::GetTextureName(int index) {
	std::map<std::string,sf::Texture*>::iterator it=texturesList->begin();
	for(int i=0;i<index;i++) {
		it++;
	};
	return it->first;
};

int ResourcesManager::GetTexturesNumber() {
	return texturesList->size();
};

bool ResourcesManager::AddFont(int type, std::string path) {
	switch (type) {
	case 1:
		if (!fontText->loadFromFile(path)) return false;
		break;
	};
	return true;
};

sf::Font* ResourcesManager::GetFont(int type) {
	switch (type) {
	case 1:
		return fontText;
	default:
		return fontText;
	};
};