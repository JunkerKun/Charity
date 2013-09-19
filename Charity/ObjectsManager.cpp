#include "ObjectsManager.h"
#include "Engine.h"
#include "Object.h"
#include "Image.h"
#include "Player.h"
#include "Block.h"
#include <fstream>
#include <string>

extern Engine* engine;

bool SortObjectsPredicate(const Object* o1, const Object* o2) {
	return (o1->depth<o2->depth);
};

ObjectsManager::ObjectsManager() {
	chunkSize=engine->windowSize;
	chunks=NULL;
	Clear(1,1);
};

void ObjectsManager::Resize(int sizeX, int sizeY) {
	Clear(sizeX,sizeY);
};

Object* ObjectsManager::GetObjectAt(int x, int y) {
	Object* collision=CollisionCheckPoint(x,y,-1);
	if (collision!=NULL) return collision;
	return NULL;
};

void ObjectsManager::DeleteAt(int x, int y) {
	Object* collision=GetObjectAt(x,y);
	if (collision!=NULL) {
		DeleteObject(collision);
	};
};

void ObjectsManager::Update() {
	if (chunks==NULL) return;
	int chunkX = floor(engine->camera->xView/chunkSize.x);
	int chunkY = floor(engine->camera->yView/chunkSize.y);
	int chunkXStart = std::max(0,chunkX-1);
	int chunkYStart = std::max(0,chunkY-1);
	int chunkXEnd = std::min(chunksNumber.x,chunkXStart+3);
	int chunkYEnd = std::min(chunksNumber.y,chunkYStart+3);
	for(int i=chunkXStart;i<chunkXEnd;i++) {
		for(int j=chunkYStart;j<chunkYEnd;j++) {
			//int size=;
			for(int k=0;k<chunks->at(i)->at(j)->list->size();k++) {
				chunks->at(i)->at(j)->list->at(k)->Update();
			};
		};
	};
	if (movers.size()!=0) {
		int size=movers.size();
		for(int i=0;i<size;i++) {
			if (movers[i]->toDelete) {
				DeleteMover(i);
				size--;
			} else {
				movers[i]->Update();
			};
		};
	};
};

void ObjectsManager::Draw(sf::RenderTarget &rt) {
	if (chunks==NULL) return;
	int chunkX = floor(engine->camera->xView/chunkSize.x);
	int chunkY = floor(engine->camera->yView/chunkSize.y);
	int chunkXStart = std::max(0,chunkX-1);
	int chunkYStart = std::max(0,chunkY-1);
	int chunkXEnd = std::min(chunksNumber.x,chunkXStart+3);
	int chunkYEnd = std::min(chunksNumber.y,chunkYStart+3);
	for(int i=chunkXStart;i<chunkXEnd;i++) {
		for(int j=chunkYStart;j<chunkYEnd;j++) {
			int size=chunks->at(i)->at(j)->list->size();
			std::sort(chunks->at(i)->at(j)->list->begin(),chunks->at(i)->at(j)->list->end(),SortObjectsPredicate);
			for(int k=0;k<size;k++) {
				chunks->at(i)->at(j)->list->at(k)->Draw(rt);
			};
		};
	};
};

Object* ObjectsManager::AddObject(int x, int y, int index, std::string function) {
	if (chunks==NULL) return NULL;
	Object* temp;
	switch(index) {
	case 0:
		temp=new Block();
		break;
	case 1:
		temp=new Player(engine->resourcesManager->GetTexture(engine->playerSpriteName));
		break;
	case 2: {
		temp=new Usable();
		Usable* use = static_cast<Usable*>(temp);
		use->function=function;
		break;
			};
	case 3: {
		temp=new Decoration(engine->resourcesManager->GetTexture(function));
		Decoration* dec = static_cast<Decoration*>(temp);
		dec->spriteName=function;
		break;
			};
	case 4: {
		temp=new Trigger();
		Trigger* trg = static_cast<Trigger*>(temp);
		trg->function=function;
		break;
			};
	};
	temp->SetPosition(x, y);
	temp->chunk.x=floor(temp->x/chunkSize.x);
	temp->chunk.y=floor(temp->y/chunkSize.y);
	temp->depth=temp->y;
	chunks->at(temp->chunk.x)->at(temp->chunk.y)->list->push_back(static_cast<Object*>(temp));
	return temp;
	
};

void ObjectsManager::AddMover(Object* obj, int x, int y, float sp) {
	movers.push_back(new Mover(obj,x,y,sp));
};

void ObjectsManager::DeleteMover(int index) {
	delete movers[index];
	movers.erase(movers.begin()+index);
};

Object* ObjectsManager::FindObject(Object* obj) {
	for (int i=0;i<chunks->at(obj->chunk.x)->at(obj->chunk.y)->list->size();i++) {
		if (chunks->at(obj->chunk.x)->at(obj->chunk.y)->list->at(i)==obj) {
			chunks->at(obj->chunk.x)->at(obj->chunk.y)->list->erase(
				chunks->at(obj->chunk.x)->at(obj->chunk.y)->list->begin()+i);
			return obj;
		};
	};
	return NULL;
};

Player* ObjectsManager::GetPlayer() {
	return player;
};

void ObjectsManager::SetPlayer(Object* obj) {
	player=static_cast<Player*>(obj);
};

void ObjectsManager::DeleteObject(Object* obj) {
	for (int i=0;i<chunks->at(obj->chunk.x)->at(obj->chunk.y)->list->size();i++) {
		if (chunks->at(obj->chunk.x)->at(obj->chunk.y)->list->at(i)==obj) {
			chunks->at(obj->chunk.x)->at(obj->chunk.y)->list->erase(chunks->at(obj->chunk.x)->at(obj->chunk.y)->list->begin()+i);
			delete obj;
			return;
		};
	};
};

void ObjectsManager::Clear(int sizeX, int sizeY) {
	if (chunks!=NULL) {
		for(int i=0;i<chunksNumber.x;i++) {
			for(int j=0;j<chunksNumber.y;j++) {
				for (int k=0;k<chunks->at(i)->at(j)->list->size();k++) {
					delete chunks->at(i)->at(j)->list->at(k);
				};
				delete chunks->at(i)->at(j);
			};
			delete chunks->at(i);
		};
		delete chunks;
		chunks=NULL;
	};
	if (sizeX!=-1 && sizeY!=-1) {
		chunksNumber.x=sizeX;
		chunksNumber.y=sizeY;
		chunks = new std::vector<std::vector<Chunk*>*>;
		for(int i=0;i<chunksNumber.x;i++) {
			chunks->push_back(new std::vector<Chunk*>);
			for(int j=0;j<chunksNumber.y;j++) {
				chunks->at(chunks->size()-1)->push_back(new Chunk);
			};
		};
	};
};

bool ObjectsManager::LoadMap(std::string name) {
	std::ifstream load(name);
	if (!load) {
		std::cout<<"Failed to load the map\n";
		return false;
	};
	int sizeX,sizeY;
	load>>sizeX;
	load>>sizeY;

	Clear(sizeX, sizeY);

	while(load) {
		int index,x,y;
		std::string script;
		load>>index;
		if (index==-404) break;
		load>>x;
		load>>y;
		load>>script;

		AddObject(x,y,index,script);
	};
	engine->tilesManager->LoadTiles(load);
	load.close();
	return true;
};

ObjectsManager::~ObjectsManager() {
	Clear(-1,-1);
};