#include "Collisions.h"
#include "Engine.h"

extern Engine* engine;

bool CollisionRadius(float rad, float x, float y, Object* obj) {
	return (Distance(x,y,obj->x,obj->y)<rad);
};

bool CollisionPoint(float x, float y, Object* obj) {
	if (x>obj->x+obj->GetBBox().left) {
		if (y>obj->y+obj->GetBBox().top) {
			if (x<obj->x+obj->GetBBox().left+obj->GetBBox().width) {
				if (y<obj->y+obj->GetBBox().top+obj->GetBBox().height) {
					return true;//obj->solid;
				};
			};
		};
	};
	return false;
};

bool CollisionIntersect(Object* o1, Object* o2) {
	sf::Rect<sf::Int16> Rect1(o1->GetBBox());
	sf::Rect<sf::Int16> Rect2(o2->GetBBox());
	Rect1.left+=o1->x;
	Rect1.top+=o1->y;
	Rect2.left+=o2->x;
	Rect2.top+=o2->y;
	/*bool inter=Rect1.intersects(Rect2);
	if (inter) return o2->solid;
	return false;*/
	return Rect1.intersects(Rect2);
};

Object* CollisionCheckRadius(float rad, float x, float y, unsigned int index) {
	int chunkX = floor(x/engine->objectsManager->chunkSize.x);
	int chunkY = floor(y/engine->objectsManager->chunkSize.y);
	int chunkXStart = std::max(0,chunkX-1);
	int chunkYStart = std::max(0,chunkY-1);
	int chunkXEnd = std::min(engine->objectsManager->chunksNumber.x,chunkX+2);
	int chunkYEnd = std::min(engine->objectsManager->chunksNumber.y,chunkY+2);
	for(int i=chunkXStart;i<chunkXEnd;i++) {
		for(int j=chunkYStart;j<chunkYEnd;j++) {
			int size=engine->objectsManager->chunks->at(i)->at(j)->list->size();
			for(int k=0;k<size;k++) {
				Object* obj = engine->objectsManager->chunks->at(i)->at(j)->list->at(k);
				if (obj->objectIndex==index || index==-1) {
					if (obj->solid || obj->objectIndex==2 || obj->objectIndex==4) {
					bool collision = CollisionRadius(rad,x,y,obj);
					if (collision) return obj;
					};
				};
			};
		};
	};
	return NULL;
};

Object* CollisionCheckPoint(float x, float y, unsigned int index) {
	int chunkX = floor(x/engine->objectsManager->chunkSize.x);
	int chunkY = floor(y/engine->objectsManager->chunkSize.y);
	int chunkXStart = chunkX;//std::max(0,chunkX-1);
	int chunkYStart = chunkY;//std::max(0,chunkY-1);
	int chunkXEnd = std::min(engine->objectsManager->chunksNumber.x,chunkX+1);
	int chunkYEnd = std::min(engine->objectsManager->chunksNumber.y,chunkY+1);
	for(int i=chunkXStart;i<chunkXEnd;i++) {
		for(int j=chunkYStart;j<chunkYEnd;j++) {
			int size=engine->objectsManager->chunks->at(i)->at(j)->list->size();
			for(int k=0;k<size;k++) {
				Object* obj = engine->objectsManager->chunks->at(i)->at(j)->list->at(k);
				if (obj->objectIndex==index || index==-1) {
					if (obj->solid || obj->objectIndex==2 || obj->objectIndex==4) {
					bool collision = CollisionPoint(x,y,obj);
					if (collision) return obj;
					};
				};
			};
		};
	};
	return NULL;
};

Object* CollisionCheckIntersect(Object* o1, unsigned int index) {
	int chunkX = o1->chunk.x;//floor(o1->x/engine->objectsManager->chunkSize.x);
	int chunkY = o1->chunk.y;//floor(o1->y/engine->objectsManager->chunkSize.y);
	int chunkXStart = std::max(0,chunkX-1);
	int chunkYStart = std::max(0,chunkY-1);
	int chunkXEnd = std::min(engine->objectsManager->chunksNumber.x,chunkX+2);
	int chunkYEnd = std::min(engine->objectsManager->chunksNumber.y,chunkY+2);
	for(int i=chunkXStart;i<chunkXEnd;i++) {
		for(int j=chunkYStart;j<chunkYEnd;j++) {
			int size=engine->objectsManager->chunks->at(i)->at(j)->list->size();
			for(int k=0;k<size;k++) {
				Object* obj = engine->objectsManager->chunks->at(i)->at(j)->list->at(k);
				if (obj->objectIndex==index || index==-1) {
					if (obj->solid || obj->objectIndex==2 || obj->objectIndex==4) {
					bool collision = CollisionIntersect(o1,obj);
					if (collision) return obj;
					};
				};
			};
		};
	};
	return NULL;
};