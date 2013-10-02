#include "Image.h"
#include "Engine.h"
#include <iostream>

extern Engine* engine;

Image::Image(char* path):Object() {
	Load(path);
};

Image::Image(sf::Texture &tex):Object() {
	texture=tex;
	imageWidth=texture.getSize().y;
	imageHeight=texture.getSize().y;
	imageSpeed=0.25;
	imageFrame=0;
	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(0,0,imageWidth,imageHeight));
	firstFrame=0;
	lastFrame=3;
	seq=0;
	seqOld=-1;
	x=100;
	y=100;
	SetBBox(0,0,32,32);
	SetOrigin(imageWidth/2,imageHeight-8);
};

bool Image::Load(char* path) {
	texture.loadFromFile(path);
	imageWidth=texture.getSize().y;
	imageHeight=texture.getSize().y;
	imageSpeed=0.25;
	imageFrame=0;
	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(0,0,imageWidth,imageHeight));
	firstFrame=0;
	lastFrame=3;
	seq=0;
	seqOld=-1;
	x=100;
	y=100;
	SetBBox(0,0,32,32);
	SetOrigin(imageWidth/2,imageHeight-8);
	return true;
};

bool Image::SetOrigin(int left, int top) {
	sprite.setOrigin(left, top);
	return true;
};

bool Image::Update() {
	sprite.setPosition(x,y);
	imageFrame+=(1/imageSpeed)*engine->GetDelta();
	if (imageFrame>lastFrame+1) {
		imageFrame=firstFrame;
	};
	if (seqOld!=seq) {
		imageFrame=firstFrame;
		seqOld=seq;
	};
	SetFrame((int)imageFrame);
	return true;
};

bool Image::Draw(sf::RenderTarget& RT) {
	if (!visible) return false;
	RT.draw(sprite);
	sf::RectangleShape RS;
	RS.setPosition(x+bBox.left,y+bBox.top);
	RS.setSize(sf::Vector2f(bBox.width,bBox.height));
	RS.setFillColor(sf::Color(255,255,255,120));
	RT.draw(RS);
	return true;
};

bool Image::SetFrame(int frame) {
	sprite.setTextureRect(sf::IntRect(imageWidth*frame,0,imageWidth,imageHeight));
	return true;
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

Image::~Image() {

};