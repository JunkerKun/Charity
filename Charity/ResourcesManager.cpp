#include "ResourcesManager.h"
#include "Engine.h"

extern Engine* engine;

ResourcesManager::ResourcesManager() {
	texturesList= new std::map<std::string,sf::Texture*>;
	fontText= new Font(14,255,255,255);
	sf::Image img;
	img.create(engine->windowSize.x/2,engine->windowSize.y/2);
	for (int i=0;i<engine->windowSize.x/2;i++) {
		for (int j=0;j<engine->windowSize.y/2;j++) {
			int chance=rand()%50;
			if (chance==1) {
				img.setPixel(i,j,sf::Color(128,128,128,255));
			};
		};
	};
	img.createMaskFromColor(sf::Color::Black);
	sf::Texture* noiseTexture=new sf::Texture;
	noiseTexture->loadFromImage(img);
	noiseTexture->setRepeated(true);
	texturesList->insert(std::pair<std::string,sf::Texture*>("sprNoise",noiseTexture));
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

bool ResourcesManager::DeleteTexture(std::string name) {
	std::map<std::string,sf::Texture*>::iterator it=texturesList->find(name);
	if (it==texturesList->end()) return false;
	delete it->second;
	texturesList->erase(it);
	return true;
};

void ResourcesManager::DesaturateTexture(std::string name) {
	sf::Texture* tex=GetTexture(name);
	sf::Image img=tex->copyToImage();
	for (int i=0;i<img.getSize().x;i++) {
		for (int j=0;j<img.getSize().y;j++) {
			sf::Color pixel=img.getPixel(i,j);
			sf::Color saturation;
			saturation.r=pixel.r*0.3+pixel.g*0.59+pixel.b*0.11;
			saturation.g=pixel.r*0.3+pixel.g*0.59+pixel.b*0.11;
			saturation.b=pixel.r*0.3+pixel.g*0.59+pixel.b*0.11;
			saturation.a=pixel.a;
			img.setPixel(i,j,saturation);
		};
	};
	DeleteTexture(name);
	sf::Texture* tex2=new sf::Texture();
	tex2->loadFromImage(img);
	texturesList->insert(std::pair<std::string,sf::Texture*>(name,tex2));
};

sf::Texture* ResourcesManager::GetTexture(std::string name) {
	std::map<std::string,sf::Texture*>::iterator it=texturesList->find(name);
	if (it==texturesList->end()) return NULL;
	return it->second;
};

sf::Texture* ResourcesManager::GetTexture(int index) {
	if (index>texturesList->size()) return NULL;
	std::map<std::string,sf::Texture*>::iterator it=texturesList->begin();
	for(int i=0;i<index;i++) {
		it++;
	};
	return it->second;
};

std::string ResourcesManager::GetTextureName(int index) {
	if (index>texturesList->size()) return "";
	std::map<std::string,sf::Texture*>::iterator it=texturesList->begin();
	for(int i=0;i<index;i++) {
		it++;
	};
	return it->first;
};

std::string ResourcesManager::GetTextureName(sf::Texture* tex) {
	for(std::map<std::string,sf::Texture*>::iterator it=texturesList->begin();it!=texturesList->end();it++) {
		if (it->second==tex) return it->first;
	};
	return "";
};

int ResourcesManager::GetTexturesNumber() {
	return texturesList->size();
};

bool ResourcesManager::AddFont(int type, std::string path, int size, int r, int g, int b) {
	switch (type) {
	case 1:
		if (!fontText->font.loadFromFile(path)) return false;
		fontText->color=sf::Color(r,g,b,255);
		fontText->size=size;
		break;
	};
	return true;
};

Font* ResourcesManager::GetFont(int type) {
	switch (type) {
	case 1:
		return fontText;
	default:
		return fontText;
	};
};

void ResourcesManager::ClearTextures() {
	for (std::map<std::string,sf::Texture*>::iterator it=texturesList->begin(); it!=texturesList->end(); it++) {
		delete it->second;
	};
	texturesList->clear();
};

void ResourcesManager::ClearAll() {
	ClearTextures();
};