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
	void ExtendText(std::wstring text);
	std::vector<std::wstring> GetStrings();
	int GetIndex();
	void Complete();
	sf::Text GetTextBox();
	void SetCallback(std::wstring callback);
	void SetNvl(bool enabled, sf::Texture* tex=NULL);
	int xAdv, yAdv, xNvl, yNvl;
	bool textCenter, drawSprite;
	//bool toDelete;
private:
	std::wstring callback;
	sf::Sprite sprite;
	sf::Text* text;
	sf::Texture* textImage;
	std::vector<std::wstring> strings; 
	int index;
	float timer;
	bool nvlMode;
};

#endif