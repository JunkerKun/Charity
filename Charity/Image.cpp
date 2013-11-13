#include "Image.h"
#include "Engine.h"
#include <iostream>

extern Engine* engine;

Image::Image(char* path):Object() {
	Load(path);
};

Image::Image(sf::Texture* tex):Object() {
	std::string name = engine->resourcesManager->GetTextureName(tex);
	std::vector<int>* vec=engine->resourcesManager->GetTextureSettings(name);
	if (tex!=NULL) Load(tex, vec);
};

bool Image::Load(sf::Texture* tex, std::vector<int>* sets) {
	texture=*tex;
	imageWidth=texture.getSize().y;
	imageHeight=texture.getSize().y;
	if (sets->at(0)!=-1) imageWidth=sets->at(0);
	if (sets->at(1)!=-1) imageHeight=sets->at(1);
	imageSpeed=0;
	imageFrame=0;
	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(0,0,imageWidth,imageHeight));
	firstFrame=0;
	lastFrame=static_cast<int>(texture.getSize().x/imageHeight);
	seq=0;
	seqOld=-1;
	x=0;
	y=0;
	SetBBox(0,0,32,32);
	SetOrigin(0,0);
	return true;
};

bool Image::Load(char* path) {
	texture.loadFromFile(path);
	imageWidth=texture.getSize().y;
	imageHeight=texture.getSize().y;
	imageSpeed=0;
	imageFrame=0;
	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(0,0,imageWidth,imageHeight));
	firstFrame=0;
	lastFrame=0;
	seq=0;
	seqOld=-1;
	x=0;
	y=0;
	SetBBox(0,0,32,32);
	SetOrigin(0,0);
	return true;
};

bool Image::SetOrigin(int left, int top) {
	sprite.setOrigin(left, top);
	return true;
};

bool Image::Update() {
	sprite.setPosition(floor(x),floor(y));
	if (!locked) {
	imageFrame+=(1/imageSpeed)*engine->GetDelta();
	if (imageFrame>lastFrame+1) {
		imageFrame=firstFrame;
	};
	if (seqOld!=seq) {
		imageFrame=firstFrame;
		seqOld=seq;
	};
	SetFrame((int)imageFrame);
	depth=y+16;
	};
	return true;
};

bool Image::Draw(sf::RenderTarget& RT) {
	if (!visible) return false;
	RT.draw(sprite);
	if (engine->debug) {
	sf::RectangleShape RS;
	RS.setPosition(x+bBox.left,y+bBox.top);
	RS.setSize(sf::Vector2f(bBox.width,bBox.height));
	RS.setFillColor(sf::Color(255,255,255,120));
	RT.draw(RS);
	};
	return true;
};

void Image::SetTexture(sf::Texture* tex) {
	sprite.setTexture(*tex);
};

sf::Texture& Image::GetTexture() {
	return texture;
};

sf::Sprite& Image::GetSprite() {
	return sprite;
};

bool Image::SetFrame(int frame) {
	sprite.setTextureRect(sf::IntRect(imageWidth*frame,0,imageWidth,imageHeight));
	return true;
};

void Image::ClearSequences() {
	sequences.clear();
	seq=0;
	seqOld=0;
	imageFrame=0;
	firstFrame=0;
	lastFrame=0;
	imageSpeed=0.25;
};

bool Image::AddSequence(float first, float last, float speed) {
	std::vector<float> temp;
	temp.push_back(floor(first));
	temp.push_back(floor(last));
	temp.push_back(speed);
	sequences.insert(sequences.end(),std::pair<int,std::vector<float>>(sequences.size(),temp));
	return true;
};

bool Image::SetSequence(int sequence, bool force) {
	if (seq!=sequence || force) {
		std::vector<float> temp=sequences.at(sequence);
		firstFrame=temp.at(0);
		lastFrame=temp.at(1);
		imageSpeed=temp.at(2);
		seq=sequence;
		return true;
	};
	return false;
};

bool Image::LoadSequences(std::string path) {
	ClearSequences();
	std::wifstream file(path);
	if (!file) return false;
	while (!file.eof()) {
		int first, last, speed;
		file>>first;
		file>>last;
		file>>speed;
		AddSequence(first,last,speed);
	};
	file.close();
	return true;
};

bool Image::LoadSettings(std::string path) {
	std::wifstream file(path);
	if (!file) return false;
		int width, height;
		file>>width;
		file>>height;
		imageWidth=width;
		imageHeight=height;
	file.close();
	return true;
};

Image::~Image() {

};