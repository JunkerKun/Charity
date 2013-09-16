#ifndef __OBJECTSMANAGER
#define __OBJECTSMANAGER

#include <vector>
#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "Object.h"

struct Chunk {
	std::vector<Object*>* list;
	Chunk() {
		list = new std::vector<Object*>;
	};
	~Chunk() {
		delete list;
	};
};

class ObjectsManager {
public:
	ObjectsManager();
	~ObjectsManager();
	void Update();
	void Draw(sf::RenderTarget &rt);
	void Clear(int sizeX, int sizeY);
	Object* AddObject(int x, int y, int index, std::string function="none");
	void DeleteObject(Object* obj);
	void Resize(int sizeX, int sizeY);
	bool LoadMap(std::string path);
	void DeleteAt(int x, int y);
	Object* GetObjectAt(int x, int y);
	Object* FindObject(Object* obj);
	Object* GetPlayer();
	void SetPlayer(Object* obj);
	sf::Vector2i chunkSize;
	sf::Vector2i chunksNumber;
	std::vector<std::vector<Chunk*>*>* chunks;
private:
	Object* player;
};

#endif