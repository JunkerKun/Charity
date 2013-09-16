#ifndef __TEXTBOX
#define __TEXTBOX

#include <SFML/Graphics.hpp>
#include "Object.h"

class TextBox : public Object {
public:
	TextBox(int x=0, int y=0, sf::Texture* tex=NULL);
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
	sf::Texture* textImage;
	std::vector<std::wstring> strings; 
	int index;
	float timer;
	bool locked, nvlMode;
};

#endif