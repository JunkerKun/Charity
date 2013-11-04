#include "ResourcesManager.h"
#include "Engine.h"

extern Engine* engine;

ResourcesManager::ResourcesManager() {
	texturesList= new std::map<std::string,sf::Texture*>;
	soundsList= new std::map<std::string,Sound*>;
	texturesListGlobal= new std::map<std::string,sf::Texture*>;
	soundsListGlobal= new std::map<std::string,Sound*>;
	texturesSettings= new std::map<std::string,std::vector<int>*>;
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

	for (std::map<std::string,Sound*>::iterator it=soundsList->begin(); it!=soundsList->end(); it++) {
		delete it->second;
	};
	soundsList->clear();
	delete soundsList;

	for (std::map<std::string,sf::Texture*>::iterator it=texturesListGlobal->begin(); it!=texturesListGlobal->end(); it++) {
		delete it->second;
	};
	texturesListGlobal->clear();
	delete texturesListGlobal;

	for (std::map<std::string,Sound*>::iterator it=soundsListGlobal->begin(); it!=soundsListGlobal->end(); it++) {
		delete it->second;
	};
	soundsListGlobal->clear();
	delete soundsListGlobal;

	for (std::map<std::string,std::vector<int>*>::iterator it=texturesSettings->begin(); it!=texturesSettings->end(); it++) {
		delete it->second;
	};
	texturesSettings->clear();
	delete texturesSettings;

	delete fontText;
};

bool ResourcesManager::AddTexture(std::string name, std::string path, std::vector<int>* settings, bool global) {
	sf::Texture* tex=new sf::Texture;
	tex->setSmooth(false);
	tex->loadFromFile(path);
	texturesSettings->insert(std::pair<std::string,std::vector<int>*>(name,settings));
	if (global) {
		texturesListGlobal->insert(std::pair<std::string,sf::Texture*>(name,tex));
		return true;
	};
	texturesList->insert(std::pair<std::string,sf::Texture*>(name,tex));
	return true;
};

bool ResourcesManager::AddSound(std::string name, std::string path, bool global) {
	Sound* snd=new Sound(path);
	if (global) {
		soundsListGlobal->insert(std::pair<std::string,Sound*>(name,snd));
		return true;
	};
	soundsList->insert(std::pair<std::string,Sound*>(name,snd));
	return true;
};

bool ResourcesManager::DeleteTexture(std::string name) {
	std::map<std::string,sf::Texture*>::iterator it=texturesList->find(name);
	if (it==texturesList->end()) return false;
	delete it->second;
	texturesList->erase(it);
	return true;
};

bool ResourcesManager::DeleteSound(std::string name) {
	std::map<std::string,Sound*>::iterator it=soundsList->find(name);
	if (it==soundsList->end()) return false;
	delete it->second;
	soundsList->erase(it);
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

void ResourcesManager::ColorizeTexture(std::string name, int mode, sf::Color color) {
	sf::Texture* tex=GetTexture(name);
	sf::Image img=tex->copyToImage();
	for (int i=0;i<img.getSize().x;i++) {
		for (int j=0;j<img.getSize().y;j++) {
			sf::Color pixel=img.getPixel(i,j);
			sf::Color saturation;
			if (mode==0) {
			saturation.r=(pixel.r*color.r)/255;
			saturation.g=(pixel.g*color.g)/255;
			saturation.b=(pixel.b*color.b)/255;
			saturation.a=pixel.a;
			};
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
	if (it==texturesList->end()) {
		std::map<std::string,sf::Texture*>::iterator it=texturesListGlobal->find(name);
		if (it!=texturesListGlobal->end()) {
		return it->second;
		};
		return NULL;
	};
	return it->second;
};

std::vector<int>* ResourcesManager::GetTextureSettings(std::string name) {
	std::map<std::string,std::vector<int>*>::iterator it=texturesSettings->find(name);
	if (it==texturesSettings->end()) {
		return NULL;
	};
	return it->second;
};

Sound* ResourcesManager::GetSound(std::string name) {
	std::map<std::string,Sound*>::iterator it=soundsList->find(name);
	if (it==soundsList->end()) {
		std::map<std::string,Sound*>::iterator it=soundsListGlobal->find(name);
		if (it!=soundsListGlobal->end()) {
			return it->second;
		};
		return NULL;
	};
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
	for(std::map<std::string,sf::Texture*>::iterator it=texturesListGlobal->begin();it!=texturesListGlobal->end();it++) {
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
		std::map<std::string,std::vector<int>*>::iterator it2=texturesSettings->find(it->first);
		if (it2!=texturesSettings->end()) {
			delete it2->second;
			texturesSettings->erase(it2);
		};
		delete it->second;
	};
	texturesList->clear();
};

void ResourcesManager::ClearSounds() {
	for (std::map<std::string,Sound*>::iterator it=soundsList->begin(); it!=soundsList->end(); it++) {
		delete it->second;
	};
	soundsList->clear();
};

void ResourcesManager::ClearAll() {
	ClearTextures();
	ClearSounds();
};