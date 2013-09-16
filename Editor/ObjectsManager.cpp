#include "ObjectsManager.h"
#include "Engine.h"
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
	if (collision) return collision;
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
	int chunkX = floor(engine->xView/chunkSize.x);
	int chunkY = floor(engine->yView/chunkSize.y);
	int chunkXStart = std::max(0,chunkX-1);
	int chunkYStart = std::max(0,chunkY-1);
	int chunkXEnd = std::min(chunksNumber.x,chunkXStart+3);
	int chunkYEnd = std::min(chunksNumber.y,chunkYStart+3);
	for(int i=chunkXStart;i<chunkXEnd;i++) {
		for(int j=chunkYStart;j<chunkYEnd;j++) {
			int size=chunks->at(i)->at(j)->list->size();
			for(int k=0;k<size;k++) {
				chunks->at(i)->at(j)->list->at(k)->Update();
			};
		};
	};
};

void ObjectsManager::Draw(sf::RenderTarget &rt) {
	if (chunks==NULL) return;
	int chunkX = floor(engine->xView/chunkSize.x);
	int chunkY = floor(engine->yView/chunkSize.y);
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
		temp=new Player(engine->pathPlayerSprite);
		break;
	case 2: {
		temp=new Usable();
		Usable* use=static_cast<Usable*>(temp);
		if (function=="none") {
			use->function="Function";
			use->function+=ToString(engine->editorUsablesNumber);
			use->functionIndex=engine->editorUsablesNumber;
			engine->editorUsablesNumber+=1;
		} else {
			use->function=function;
		};
		break;
			};
	case 3: {
		temp=new Decoration();
		Decoration* dec = static_cast<Decoration*>(temp);
		dec->spriteName=function;
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

void ObjectsManager::DeleteObject(Object* obj) {
	for (int i=0;i<chunks->at(obj->chunk.x)->at(obj->chunk.y)->list->size();i++) {
		if (chunks->at(obj->chunk.x)->at(obj->chunk.y)->list->at(i)==obj) {
			chunks->at(obj->chunk.x)->at(obj->chunk.y)->list->erase(
				chunks->at(obj->chunk.x)->at(obj->chunk.y)->list->begin()+i);
			delete obj;
			break;
		};
	};
};

bool ObjectsManager::SaveMap(std::string name) {
	std::ofstream save(name);
	if (!save) {
		std::cout<<"Failed to save the map\n";
		return false;
	};
	save<<chunksNumber.x<<" ";
	save<<chunksNumber.y<<"\n";
	for (int i=0;i<chunks->size();i++) {
		for (int j=0;j<chunks->at(i)->size();j++) {
			for (int k=0;k<chunks->at(i)->at(j)->list->size();k++) {
				Object* obj = chunks->at(i)->at(j)->list->at(k);
				save<<ToString(obj->objectIndex)<<" ";
				save<<ToString(obj->x)<<" ";
				save<<ToString(obj->y)<<" ";
				switch(obj->objectIndex) {
				case 2: {
					Usable* use=static_cast<Usable*>(obj);
					save<<use->function<<" ";
					break;
						};
				case 3: {
					Decoration* dec=static_cast<Decoration*>(obj);
					save<<dec->spriteName<<" ";
					break;
						};
				default:
					save<<"none"<<" ";
					break;
				};
				save<<"\n";
			};
		};
	};
	save<<"-404\n";
	engine->tilesManager->SaveTiles(save);
	save.close();
	return true;
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

		Object* obj = AddObject(x,y,index,script);
	};
	engine->tilesManager->LoadTiles(load);
	load.close();
	return true;
};

ObjectsManager::~ObjectsManager() {
	Clear(-1,-1);
};