#include "TextBox.h"
#include "Engine.h"

extern Engine* engine;

TextBox::TextBox(int xx, int yy, sf::Texture* texture):Object() {
	nvlMode=false;
	textCenter=false;
	drawSprite=true;
	xAdv=20;
	yAdv=418;
	xNvl=0;
	yNvl=0;
	//toDelete=false;
	textImage=texture;
	objectIndex=10001;
	SetPosition(xx,yy);
	if (xx==-1 && yy==-1) {
		SetPosition(xAdv,yAdv);
	};
	if (textImage==NULL) {
		sf::Texture* tex=engine->resourcesManager->GetTexture("UITextBox");
		sprite.setTexture(*tex,true);
		bBox.left=0;
		bBox.top=0;
		bBox.width=tex->getSize().x;
		bBox.height=tex->getSize().y;
		//xAdv=20;
		//yAdv=engine->windowSize.y-tex->getSize().y-20;
	} else {
		nvlMode=true;
		sprite.setTexture(*textImage,true);
		bBox.left=0;
		bBox.top=0;
		bBox.width=textImage->getSize().x;
		bBox.height=textImage->getSize().y;
	};

	/*Font* fnt=engine->resourcesManager->GetFont(1);
	text->setColor(fnt->color);
	text->setCharacterSize(fnt->size);
	text->setFont(fnt->font);*/
	text=engine->textGame;
	index=0;
	locked=true;
	timer=0;
};

std::vector<std::wstring> TextBox::GetStrings() {
	return strings;
};

int TextBox::GetIndex() {
	return index;
};

bool TextBox::Update() {
	engine->objectsManager->GetPlayer()->canMove=false;
	if (engine->choiceBox==NULL) {
		if (!locked) {
			if (text->getString().getSize()!=strings.at(index).size()) {
				timer+=engine->setTextSpeed*engine->GetDelta();
				if (timer>1) {
					SetText(strings.at(index).substr(0,text->getString().getSize()+1));
					timer=0;
				};
			};
			if (engine->input->GetKeyPressed(sf::Keyboard::Z)) {
				if (index<strings.size()-1) {
					if (text->getString().getSize()>=strings.at(index).size()) {
						index+=1;
						SetText(strings.at(index).substr(0,strings.at(index).find_first_of(L"]")+2));
					} else {
						SetText(strings.at(index));
					};
				} else {
					if (text->getString().getSize()>=strings.at(index).size()) {
						//toDelete=true;
						if (callback!=L"") {
							std::wstring temp=callback;
							engine->scripting.ExecuteFunction(callback);
							if (callback==temp) callback=L"";
							if (index<strings.size()-1) {
								index+=1;
								text->setString(L"");
							} else {
								delete this;
								return true;
							};
						} else {
							delete this;
							return true;
						};
					} else {
						SetText(strings.at(index));
					};
				};
			};
		};
	};
	return true;
};

void TextBox::SetCallback(std::wstring cb) {
	callback=cb;
};

void TextBox::SetNvl(bool enabled, sf::Texture* tex) {
	if (enabled) {
		textImage=tex;
		sprite.setTexture(*textImage,true);
		engine->textBox->SetPosition(0,0);
	} else {
		sf::Texture* tex=engine->resourcesManager->GetTexture("UITextBox");
		sprite.setTexture(*tex,true);
		engine->textBox->SetPosition(xAdv,yAdv);
	};
	nvlMode=enabled;
};

void TextBox::Unlock() {
	locked=false;
};

void TextBox::Complete() {
	SetText(strings.at(index));
};

bool TextBox::Draw(sf::RenderTarget &RT) {
	if (!visible || locked) return false;
	int xx=floor(x);//engine->camera->xView+floor(x);
	int yy=floor(y);//engine->camera->yView+floor(y);
	sprite.setPosition(xx,yy);
	if (drawSprite) RT.draw(sprite);
	if (nvlMode) {
		sf::RectangleShape RS;
		RS.setPosition(0,0);//engine->camera->xView,engine->camera->yView);
		RS.setSize(sf::Vector2f(engine->windowSize.x,engine->windowSize.y));
		RS.setFillColor(sf::Color(0,0,0,200));
		RT.draw(RS);
	};
	int textToX, textToY;
	if (!textCenter) {
		textToX=xx+10;
		textToY=yy+10;
		//text->setPosition(xx+10,yy+10);
	}
	else {
		textToX=floor(engine->windowSize.x/2-(text->getLocalBounds().width-text->getLocalBounds().left)/2);
		textToY=floor(engine->windowSize.y/2-(text->getLocalBounds().height+text->getLocalBounds().top)/2);
		//text->setPosition(floor(engine->windowSize.x/2-(text->getLocalBounds().width-text->getLocalBounds().left)/2),
		//floor(engine->windowSize.y/2-(text->getLocalBounds().height+text->getLocalBounds().top)/2));
	};
	if (engine->setOutline) {
		text->setColor(sf::Color::Black);
		text->setPosition(textToX-1,textToY);
		RT.draw(*text);
		text->setPosition(textToX+1,textToY);
		RT.draw(*text);
		text->setPosition(textToX,textToY-1);
		RT.draw(*text);
		text->setPosition(textToX,textToY+1);
		RT.draw(*text);
	};
	text->setColor(sf::Color::White);
	text->setPosition(textToX,textToY);
	RT.draw(*text);
	return true;
};

void TextBox::SetText(std::wstring str) {
	text->setString(str);
};

void TextBox::AddText(std::wstring string) {
	strings.push_back(string);
};

void TextBox::ExtendText(std::wstring string) {
	strings.at(strings.size()-1)+=string;
};

sf::Text TextBox::GetTextBox() {
	return *text;
};

TextBox::~TextBox() {
	engine->textBox=NULL;
	engine->objectsManager->GetPlayer()->canMove=true;
};