#ifndef __TEXTBOX
#define __TEXTBOX

#include <SFML/Graphics.hpp>
#include "Object.h"
#include <SFML/Graphics.hpp>

class TextBox : public Object {
public:
	TextBox(int x=0, int y=0);
	~TextBox();
	bool Update();
	bool Draw(sf::RenderTarget &RT);
	void SetText(std::wstring str);
	void AddText(std::wstring text);
	std::vector<std::wstring> GetStrings();
	int GetIndex();
	void Unlock();
	sf::Text GetTextBox();
private:
	sf::Sprite sprite;
	sf::Text text;
	std::vector<std::wstring> strings; 
	int index;
	float timer;
	bool locked;
};

#endif