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
#include "Collisions.h"
#include "Functions.h"
#include "Input.h"
#include "Marker.h"
#include "ObjectsManager.h"
#include "TilesManager.h"
#include "ResourcesManager.h"
#include "Usable.h"
#include "Decoration.h"
#include "Trigger.h"
#include "Place.h"

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
	sf::Event GetInputEvent();
	sf::Vector2i windowSize;
	float xView, yView;
	ObjectsManager* objectsManager;
	TilesManager* tilesManager;
	ResourcesManager* resourcesManager;
	Object* grab;
	Input* input;
	bool SaveMap(std::string name);
	bool LoadMap(std::string name);
	bool GetIsFocused();
	Tile* tile;
	sf::Texture tex;
	sf::Font font;
	int editorObjectIndex, editorUsablesNumber, editorPlaceIndex;
	int editorGridSize;
	bool load, save;

	char* pathPlayerSprite;
private:
	sf::RenderWindow renderWindow;
	sf::View windowView, miniView;
	sf::Clock clock;
	sf::Event inputEvent;
	float delta;
	void MouseCheck();
	bool isFocused, drawChunks, drawGrid;
	bool create, resize, enter, tiling, addinfo;
	int addinfotype;
	std::string path;
	Marker* marker;
	Marker* tileMarker;
	int editorTilesIndex, editorTilesLayer, col, row;
	std::string editorTileName, editorFuncString;
	sf::Text text;
	Object* editorCollision;

	void EditorSpace();
	void EditorF2();
	void EditorF5();
	void EditorF9();
	void EditorT();
	void EditorNum1();
	void EditorNum2();
	void EditorNum3();
	void EditorNum4();
	void EditorNum5();
	void EditorNum6();
	void EditorNum7();
	void EditorNum8();
	void EditorNum9();
	void EditorNum0();
};
#endif