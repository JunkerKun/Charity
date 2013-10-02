#ifndef __CHOICEBOX
#define __CHOICEBOX

#include "Object.h"
#include <SFML/Graphics.hpp>

struct Choice {
	std::wstring title;
	std::wstring function;
	Choice(std::wstring titl, std::wstring func) {
		title=titl;
		function=func;
	};
};

class ChoiceBox:public Object {
public:
	int index;
	ChoiceBox();
	~ChoiceBox();
	bool Update();
	bool Draw(sf::RenderTarget& RT);
	void AddChoice(std::wstring title, std::wstring function);
private:
	sf::Text* text;
	sf::Texture* texBG;
	sf::Texture* texLine;
	sf::Sprite sprBG;
	sf::Sprite sprLine;
	std::vector<Choice*> choices;
};

#endif