#ifndef __OBJECTSMANAGER
#define __OBJECTSMANAGER

#include <vector>
#include <map>
#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "Object.h"
#include "Functions.h"
#include "Player.h"
#include "Npc.h"

struct Chunk {
	std::vector<Object*>* list;
	Chunk() {
		list = new std::vector<Object*>;
	};
	~Chunk() {
		delete list;
	};
};

struct Mover {
	Object* object;
	float xspeed, yspeed;
	sf::Vector2i coords;
	bool toDelete;
	Mover(Object* obj, int x, int y, float sp) {
		object=obj;
		xspeed=abs(x-obj->x)/sp;
		yspeed=abs(y-obj->y)/sp;
		coords.x=x;
		coords.y=y;
		toDelete=false;
	};
	void Update() {
		object->x=Increment(object->x,coords.x,xspeed);
		object->y=Increment(object->y,coords.y,yspeed);
		object->isControlled=true;
		object->isMoving=true;
		if (object->x==coords.x && object->y==coords.y) {
			toDelete=true;
			object->isControlled=false;
		};
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
	Object* AddNpc(int x, int y, std::string name, sf::Texture* tex);
	void DeleteObject(Object* obj);
	void DeleteNpc(std::string name);
	void AddMover(Object* obj, int x, int y, float sp);
	void DeleteMover(int index);
	void Resize(int sizeX, int sizeY);
	bool LoadMap(std::string path);
	void DeleteAt(int x, int y);
	Object* GetObjectByName(std::string name);
	Object* GetObjectAt(int x, int y, int index);
	Player* GetPlayer();
	Npc* GetNpc(std::string name);
	void SetPlayer(Object* obj);
	sf::Vector2i chunkSize;
	sf::Vector2i chunksNumber;
	std::vector<std::vector<Chunk*>*>* chunks;
	std::vector<Mover*> movers;
	int scaleFactor;
private:
	Player* player;
	std::map<std::string,Object*> npcList;
};

#endif