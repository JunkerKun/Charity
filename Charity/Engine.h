#ifndef __ENGINE
#define __ENGINE

#include <vector>
#include <iostream>
#include <map>
#include "SFML\Graphics.hpp"
#include "SFML\System.hpp"
#include "Object.h"
#include "Image.h"
#include "Player.h"
#include "Block.h"
#include "Scripting.h"
#include "Collisions.h"
#include "Functions.h"
#include "Input.h"
#include "ObjectsManager.h"
#include "TilesManager.h"
#include "ResourcesManager.h"
#include "Camera.h"
#include "TextBox.h"
#include "Usable.h"
#include "Decoration.h"
#include "Trigger.h"
#include "Npc.h"
#include "ChoiceBox.h"
#include "Overlay.h"
#include "Queue.h"

struct RenderImg {
	sf::RenderTexture texture;
	sf::Sprite sprite;
	RenderImg(int w, int h) {
		texture.create(w,h);
		sprite.setTexture(texture.getTexture());
	};
};

class Engine {
public:
	Engine();
	~Engine();
	void Begin();
	void End();
	bool Tick();
	float GetDelta();
	bool Update();
	bool Draw();
	void SetDrawNoise(bool mode=true);
	sf::Event GetInputEvent();
	std::map<std::wstring,int>* commandFunctions;
	std::vector<std::wstring>* commands;
	std::map<std::wstring,float>* variables;
	std::map<std::wstring,float>* variablesGlobal;
	unsigned int commandNumber;
	unsigned int commandLastText;
	bool commandPause;
	int commandReturn;
	Scripting scripting;
	Queue* queue;
	std::string scriptName;
	sf::Vector2i windowSize;
	TextBox* textBox;
	ChoiceBox* choiceBox;
	Camera* camera;
	Input* input;
	ObjectsManager* objectsManager;
	TilesManager* tilesManager;
	ResourcesManager* resourcesManager;
	bool GetIsFocused();
	bool LoadMap(std::string name);
	std::string pathMusic;
	char* pathMap;
	std::string mapName;
	std::string playerSpriteName;
	int placeIndex;
	std::ifstream IniOpen(std::string filename);
	void IniClose(std::ifstream &ini);
	float IniFindValue(std::ifstream &ini, std::string group, std::string key, float defaultValue);
	void LoadSettings();
	void Fade(int mode,float speed);
	void SetLamp(float mode,float speed);
	void SetFadeColor(sf::Color color);

	sf::Text* textGame;

	//Settings
	bool debug, setVsync, setBloom, setFullscreen, setOutline, gameEnd;
	int setTextSpeed, setFramerateLimit, volumeSounds, volumeBGM;
	int gridSize;

	float playerHP, playerSP, playerMP;
	bool drawHP, drawSP, drawMP;

private:
	sf::RenderWindow renderWindow;
	sf::Clock clock;
	sf::Event inputEvent;
	sf::Text* debugText;
	sf::Color fadeColor;
	float fadeAlpha, fadeSpeed, lampMode, lampAlpha, lampSpeed;
	int fadeMode;

	RenderImg* renderImg;
	
	bool isFocused, drawNoise;
	float delta, timeScale;
};
#endif