#include "Input.h"
#include "Engine.h"

extern Engine* engine; 

Input::Input(sf::Event *evnt) {
	event=evnt;
	for(int i=0;i<100;i++) {
		keyStates[0][i]=false;
		keyStates[1][i]=false;
	};
	for(int i=0;i<10;i++) {
		mouseStates[0][i]=false;
		mouseStates[1][i]=false;
	};
	mousePosition.x=0;
	mousePosition.y=0;
	letters.insert(std::pair<char,sf::Keyboard::Key>('a',(sf::Keyboard::A)));
	letters.insert(std::pair<char,sf::Keyboard::Key>('b',(sf::Keyboard::B)));
	letters.insert(std::pair<char,sf::Keyboard::Key>('c',(sf::Keyboard::C)));
	letters.insert(std::pair<char,sf::Keyboard::Key>('d',(sf::Keyboard::D)));
	letters.insert(std::pair<char,sf::Keyboard::Key>('e',(sf::Keyboard::E)));
	letters.insert(std::pair<char,sf::Keyboard::Key>('f',(sf::Keyboard::F)));
	letters.insert(std::pair<char,sf::Keyboard::Key>('g',(sf::Keyboard::G)));
	letters.insert(std::pair<char,sf::Keyboard::Key>('h',(sf::Keyboard::H)));
	letters.insert(std::pair<char,sf::Keyboard::Key>('i',(sf::Keyboard::I)));
	letters.insert(std::pair<char,sf::Keyboard::Key>('j',(sf::Keyboard::J)));
	letters.insert(std::pair<char,sf::Keyboard::Key>('k',(sf::Keyboard::K)));
	letters.insert(std::pair<char,sf::Keyboard::Key>('l',(sf::Keyboard::L)));
	letters.insert(std::pair<char,sf::Keyboard::Key>('m',(sf::Keyboard::M)));
	letters.insert(std::pair<char,sf::Keyboard::Key>('n',(sf::Keyboard::N)));
	letters.insert(std::pair<char,sf::Keyboard::Key>('o',(sf::Keyboard::O)));
	letters.insert(std::pair<char,sf::Keyboard::Key>('p',(sf::Keyboard::P)));
	letters.insert(std::pair<char,sf::Keyboard::Key>('q',(sf::Keyboard::Q)));
	letters.insert(std::pair<char,sf::Keyboard::Key>('r',(sf::Keyboard::R)));
	letters.insert(std::pair<char,sf::Keyboard::Key>('s',(sf::Keyboard::S)));
	letters.insert(std::pair<char,sf::Keyboard::Key>('t',(sf::Keyboard::T)));
	letters.insert(std::pair<char,sf::Keyboard::Key>('u',(sf::Keyboard::U)));
	letters.insert(std::pair<char,sf::Keyboard::Key>('v',(sf::Keyboard::V)));
	letters.insert(std::pair<char,sf::Keyboard::Key>('w',(sf::Keyboard::W)));
	letters.insert(std::pair<char,sf::Keyboard::Key>('x',(sf::Keyboard::X)));
	letters.insert(std::pair<char,sf::Keyboard::Key>('y',(sf::Keyboard::Y)));
	letters.insert(std::pair<char,sf::Keyboard::Key>('z',(sf::Keyboard::Z)));
	letters.insert(std::pair<char,sf::Keyboard::Key>('0',(sf::Keyboard::Num0)));
	letters.insert(std::pair<char,sf::Keyboard::Key>('1',(sf::Keyboard::Num1)));
	letters.insert(std::pair<char,sf::Keyboard::Key>('2',(sf::Keyboard::Num2)));
	letters.insert(std::pair<char,sf::Keyboard::Key>('3',(sf::Keyboard::Num3)));
	letters.insert(std::pair<char,sf::Keyboard::Key>('4',(sf::Keyboard::Num4)));
	letters.insert(std::pair<char,sf::Keyboard::Key>('5',(sf::Keyboard::Num5)));
	letters.insert(std::pair<char,sf::Keyboard::Key>('6',(sf::Keyboard::Num6)));
	letters.insert(std::pair<char,sf::Keyboard::Key>('7',(sf::Keyboard::Num7)));
	letters.insert(std::pair<char,sf::Keyboard::Key>('8',(sf::Keyboard::Num8)));
	letters.insert(std::pair<char,sf::Keyboard::Key>('9',(sf::Keyboard::Num9)));
};

sf::Vector2i Input::GetMousePosition(bool absolute) {
	if (absolute) {
		sf::Vector2i vec;
		vec.x=engine->xView+mousePosition.x;
		vec.y=engine->yView+mousePosition.y;
		return vec;
	};
		return mousePosition;
};

bool Input::GetMousePressed(int button) {
	return mouseStates[1][button];
};

bool Input::GetMouseIsPressed(int button) {
	return mouseStates[0][button];
};

bool Input::GetKeyPressed(int key) {
	return keyStates[1][key];
};

bool Input::GetKeyIsPressed(int key) {
	return keyStates[0][key];
};

bool Input::GetAnyKeyPressed(bool letter) {
	for(int i=0;i<100;i++) {
		if (keyStates[1][i]) {
			if (letter) {
				if (i<=35) return true;
			} else {
				return true;
			};
		};
	};
	return false;
};

bool Input::GetAnyKeyIsPressed(bool letter) {
	for(int i=0;i<100;i++) {
		if (keyStates[0][i]) {
			if (letter) {
				if (i<=35) return true;
			} else {
				return true;
			};
		};
	};
	return false;
};

char Input::GetLetterPressed() {
	for (std::map<char, sf::Keyboard::Key>::iterator i=letters.begin();i!=letters.end();i++) {
		if (GetKeyPressed(i->second)) {
			if (GetKeyIsPressed(sf::Keyboard::LShift)) {
				return (i->first-32);
			};
			return (i->first);
		};
	};
	return 0;
};

void Input::Update() {
	for(int i=0;i<100;i++) {
		keyStates[1][i]=false;
	};
	for(int i=0;i<10;i++) {
		mouseStates[1][i]=false;
	};

	if (!engine->GetIsFocused()) 
		return;

	for (int i=0; i<100; i++) {
		if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(i))) {
			if (!keyStates[0][i]) {
				keyStates[1][i]=true;
			};
			keyStates[0][i]=true;
		} else {
			keyStates[0][i]=false;
		};
	};

	if (event->type==sf::Event::MouseMoved) {
		mousePosition.x=event->mouseMove.x;
		mousePosition.y=event->mouseMove.y;
	};

	for (int i=0; i<3; i++) {
		if (sf::Mouse::isButtonPressed(static_cast<sf::Mouse::Button>(i))) {
			if (!mouseStates[0][i]) {
				mouseStates[1][i]=true;
			};
			mouseStates[0][i]=true;
		} else {
			mouseStates[0][i]=false;
		};
	};

	/*if (event->type==sf::Event::MouseButtonPressed) {
		if (!mouseStates[0][event->mouseButton.button]) {
			mouseStates[1][event->mouseButton.button]=true;
		}
		mouseStates[0][event->mouseButton.button]=true;
	};
	if (event->type==sf::Event::MouseButtonReleased) {
		mouseStates[0][event->mouseButton.button]=false;
	};*/
};