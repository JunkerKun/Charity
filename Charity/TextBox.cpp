#include "TextBox.h"
#include "Engine.h"

extern Engine* engine;

TextBox::TextBox(int xx, int yy):Object() {
	SetPosition(xx,yy);
	sf::Texture* tex=engine->resourcesManager->GetTexture("UITextBox");
	sprite.setTexture(*tex,true);
	bBox.left=0;
	bBox.top=0;
	bBox.width=tex->getSize().x;
	bBox.height=tex->getSize().y;
	text.setColor(engine->colorText);
	text.setCharacterSize(engine->sizeText);
	text.setFont(*engine->resourcesManager->GetFont(1));
	index=0;
	engine->objectsManager->GetPlayer()->canMove=false;
	locked=true;
};

std::vector<std::wstring> TextBox::GetStrings() {
	return strings;
};

int TextBox::GetIndex() {
	return index;
};

bool TextBox::Update() {
	int xx=engine->camera->xView+floor(x);
	int yy=engine->camera->yView+floor(y);
	sprite.setPosition(xx,yy);
	text.setPosition(xx+10,yy+10);
	if (!locked) {
		if (text.getString().getSize()!=strings.at(index).size()) {
			timer+=120*engine->GetDelta();
			if (timer>1) {
				SetText(strings.at(index).substr(0,text.getString().getSize()+1));
				timer=0;
			};
		};
		if (engine->input->GetKeyPressed(sf::Keyboard::Z)) {
			if (index<strings.size()-1) {
				if (text.getString().getSize()>=strings.at(index).size()) {
					index+=1;
					SetText(strings.at(index).substr(0,strings.at(index).find_first_of(L"]")+2));
				} else {
					SetText(strings.at(index));
				};
			} else {
				if (text.getString().getSize()>=strings.at(index).size()) {
					delete this;
				} else {
					SetText(strings.at(index));
				};
			};
		};
	};
	return true;
};

void TextBox::Unlock() {
	locked=false;
};

bool TextBox::Draw(sf::RenderTarget &RT) {
	if (!visible) return false;
	RT.draw(sprite);
	RT.draw(text);
	return true;
};

void TextBox::SetText(std::wstring str) {
	text.setString(str);
};

void TextBox::AddText(std::wstring string) {
	strings.push_back(string);
};

sf::Text TextBox::GetTextBox() {
	return text;
};

TextBox::~TextBox() {
	engine->textBox=NULL;
	engine->objectsManager->GetPlayer()->canMove=true;
};