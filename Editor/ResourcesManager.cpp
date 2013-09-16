#include "ResourcesManager.h"

ResourcesManager::ResourcesManager() {
	texturesList= new std::map<std::string,sf::Texture*>;
};

ResourcesManager::~ResourcesManager() {
	for (std::map<std::string,sf::Texture*>::iterator it=texturesList->begin(); it!=texturesList->end(); it++) {
		delete it->second;
	};
	texturesList->clear();
	delete texturesList;
};

bool ResourcesManager::AddTexture(std::string name, std::string path) {
	sf::Texture* tex=new sf::Texture;
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