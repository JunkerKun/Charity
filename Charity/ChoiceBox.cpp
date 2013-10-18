#include "ChoiceBox.h"
#include "Engine.h"

extern Engine* engine;

ChoiceBox::ChoiceBox():Object() {
	texBG=engine->resourcesManager->GetTexture("UIChoiceBox");
	sprBG.setTexture(*texBG,1);
	texLine=engine->resourcesManager->GetTexture("UIChoiceLine");
	sprLine.setTexture(*texLine,1);

	x=floor(engine->windowSize.x/2.f-texBG->getSize().x/2.f);
	y=floor(engine->windowSize.y/2.f-texBG->getSize().y/2.f);

	text=engine->textGame;

	index=0;
	drawSprite=true;
	drawLine=true;

	sndAccept=engine->resourcesManager->GetSound("sndSysLineAccept");
	sndBack=engine->resourcesManager->GetSound("sndSysLineBack");
	sndMove=engine->resourcesManager->GetSound("sndSysLineMove");
};

ChoiceBox::~ChoiceBox() {
	for(int i=0;i<choices.size();i++) {
		delete choices.at(i);
	};
	if (engine->objectsManager->GetPlayer()!=NULL) engine->objectsManager->GetPlayer()->canMove=true;
	if (engine->choiceBox==this) engine->choiceBox=NULL;
};

void ChoiceBox::SetBackTexture(sf::Texture* tex) {
	sprBG.setTexture(*tex,1);
};

void ChoiceBox::SetLineTexture(sf::Texture* tex) {
	sprLine.setTexture(*tex,1);
};

bool ChoiceBox::Update() {
	if (engine->objectsManager->GetPlayer()!=NULL) engine->objectsManager->GetPlayer()->canMove=false;
	if (engine->input->GetKeyPressed(sf::Keyboard::Up)) {
		if (index>1) {
			sndMove->sound->play();
			index--;
		};
	} else
		if (engine->input->GetKeyPressed(sf::Keyboard::Down)) {
			if (index<choices.size()-1) {
				sndMove->sound->play();
				index++;
			};
		} else
			if (engine->input->GetKeyPressed(sf::Keyboard::Z)) {
				if (choices.at(index)->function!=L"") {
					sndAccept->sound->play();
					engine->scripting.ExecuteFunction(choices.at(index)->function);
					delete this;
				};
			};
		return true;
};

bool ChoiceBox::Draw(sf::RenderTarget &RT) {
	if (drawSprite) {
		sprBG.setPosition(x,y);
		RT.draw(sprBG);
	};
	int textX=0, textY=0, size=choices.size(), height=static_cast<int>(texBG->getSize().y)/size;
	if (drawLine) {
		sprLine.setPosition(x,y+height/2-texLine->getSize().y/2+height*index);
		RT.draw(sprLine);
	};

	for (int i=0;i<size;i++) {
		text->setString(choices.at(i)->title);
		textX=floor(engine->windowSize.x/2-text->getLocalBounds().width/2);
		textY=y+height/2-(text->getLocalBounds().top+text->getLocalBounds().height)/2+height*i;

		if (engine->setOutline) {
			text->setColor(sf::Color::Black);
			text->setPosition(textX-1,textY);
			RT.draw(*text);
			text->setPosition(textX+1,textY);
			RT.draw(*text);
			text->setPosition(textX,textY-1);
			RT.draw(*text);
			text->setPosition(textX,textY+1);
			RT.draw(*text);
		};
		text->setColor(sf::Color::White);
		text->setPosition(textX,textY);
		RT.draw(*text);
	};

	return true;
};

void ChoiceBox::AddChoice(std::wstring title, std::wstring function) {
	choices.push_back(new Choice(title,function));
};