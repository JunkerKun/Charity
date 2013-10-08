#ifndef __INPUT
#define __INPUT
#include <SFML\Window.hpp>
#include <map>

class Input {
public:
	Input(sf::Event *evnt);
	sf::Vector2i GetMousePosition(bool absolute=false);
	sf::Vector2i GetLastMousePosition(bool absolute=false);
	bool GetMousePressed(int button);
	bool GetMouseIsPressed(int button);
	bool GetKeyPressed(int key);
	bool GetKeyIsPressed(int key);
	char GetLetterPressed();
	bool GetAnyKeyPressed(bool letter=false);
	bool GetAnyKeyIsPressed(bool letter=false);
	void Update();
private:
	std::map<char, sf::Keyboard::Key> letters;
	bool keyStates[2][255];
	bool mouseStates[2][10];
	sf::Vector2i mousePosition;
	sf::Vector2i mousePositionLast;
	sf::Event* event;
};

#endif