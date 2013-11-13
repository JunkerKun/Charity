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
	scaleFactor=3;
	chunkSize=sf::Vector2i(engine->windowSize.x/3,engine->windowSize.y/scaleFactor);
	chunks=NULL;
	Clear(1,1);
	engine->camera->SetBorders(0,0,engine->windowSize.x,engine->windowSize.y);
	playerXStart=-1;
	playerYStart=-1;
	playerDirStart=-1;
	loaded=false;
};

void ObjectsManager::Resize(int sizeX, int sizeY) {
	int sX=sizeX;
	if (sX<3) sX=3;
	int sY=sizeY;
	if (sY<3) sY=3;
	Clear(sX,sY);
};

Object* ObjectsManager::GetObjectByName(std::string name) {
	for(int i=0;i<chunksNumber.x;i++) {
		for(int j=0;j<chunksNumber.y;j++) {
			for (int k=0;k<chunks->at(i)->at(j)->list->size();k++) {
				Object* obj = chunks->at(i)->at(j)->list->at(k);
				if (obj->objectName==name) {
					return obj;
				};
			};
		};
	};
	return NULL;
};

Object* ObjectsManager::GetObjectAt(int x, int y, int index) {
	Object* collision=CollisionCheckPoint(x,y,index);
	if (collision!=NULL) return collision;
	return NULL;
};

void ObjectsManager::DeleteAt(int x, int y) {
	Object* collision=GetObjectAt(x,y,-1);
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
	int chunkXEnd = std::min(chunksNumber.x,(chunkX+3)*scaleFactor);
	int chunkYEnd = std::min(chunksNumber.y,(chunkY+3)*scaleFactor);
	for(int i=chunkXStart;i<chunkXEnd;i++) {
		for(int j=chunkYStart;j<chunkYEnd;j++) {
			for(int k=0;k<chunks->at(i)->at(j)->list->size();k++) {
				chunks->at(i)->at(j)->list->at(k)->Update();
				if (loaded) {
					//loaded=false;
					return;
				};
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
	drawQueue.clear();
	int chunkX = floor(engine->camera->xView/chunkSize.x);
	int chunkY = floor(engine->camera->yView/chunkSize.y);
	int chunkXStart = std::max(0,chunkX-1);
	int chunkYStart = std::max(0,chunkY-1);
	int chunkXEnd = std::min(chunksNumber.x,(chunkX+3)*scaleFactor);
	int chunkYEnd = std::min(chunksNumber.y,(chunkY+3)*scaleFactor);
	for(int i=chunkXStart;i<chunkXEnd;i++) {
		for(int j=chunkYStart;j<chunkYEnd;j++) {
			//std::sort(chunks->at(i)->at(j)->list->begin(),chunks->at(i)->at(j)->list->end(),SortObjectsPredicate);
			for(int k=0;k<chunks->at(i)->at(j)->list->size();k++) {
				//chunks->at(i)->at(j)->list->at(k)->Draw(rt);
				drawQueue.push_back(chunks->at(i)->at(j)->list->at(k));
			};
		};
	};
	if (loaded) {
		loaded=false;
		return;
	};
	std::sort(drawQueue.begin(),drawQueue.end(),SortObjectsPredicate);
	for(int i=0;i<drawQueue.size();i++) {
		drawQueue.at(i)->Draw(rt);
	};
};

Object* ObjectsManager::AddObject(int x, int y, int index, std::string function) {
	if (chunks==NULL) return NULL;
	bool done=true;
	Object* temp;
	switch(index) {
	case 0:
		temp=new Block();
		break;
	case 1: {
		temp=new Player(engine->resourcesManager->GetTexture(engine->playerSpriteName));
		Player* plr = static_cast<Player*>(temp);
		/*std::vector<int>* sets=engine->resourcesManager->GetTextureSettings(engine->playerSpriteName);
		if (sets->at(0)!=-1) plr->imageWidth=sets->at(0);
		if (sets->at(1)!=-1) plr->imageHeight=sets->at(1);*/
		//plr->SetOrigin(plr->imageWidth/2,plr->imageHeight-static_cast<int>(plr->imageHeight/7.63));
		//plr->SetOrigin(plr->imageWidth/2,plr->imageHeight-6*engine->gridSize/32);
		break;
			};
	case 2: {
		temp=new Usable();
		Usable* use = static_cast<Usable*>(temp);
		use->function=function;
		break;
			};
	case 3: {
		temp=new Decoration(engine->resourcesManager->GetTexture(function));
		Decoration* dec = static_cast<Decoration*>(temp);
		/*std::vector<int>* sets=engine->resourcesManager->GetTextureSettings(function);
		if (sets->at(0)!=-1) dec->imageWidth=sets->at(0);
		if (sets->at(1)!=-1) dec->imageHeight=sets->at(1);*/
		//dec->SetOrigin(dec->imageWidth/2,dec->imageHeight-6*engine->gridSize/32);
		dec->spriteName=function;
		dec->objectName=function;
		break;
			};
	case 4: {
		temp=new Trigger();
		Trigger* trg = static_cast<Trigger*>(temp);
		trg->function=function;
		break;
			};
	case 5: {
		if (stoi(function)==engine->placeIndex) {
			temp=new Player(engine->resourcesManager->GetTexture(engine->playerSpriteName));
			Player* plr = static_cast<Player*>(temp);
			/*std::vector<int>* sets=engine->resourcesManager->GetTextureSettings(engine->playerSpriteName);
			if (sets->at(0)!=-1) plr->imageWidth=sets->at(0);
			if (sets->at(1)!=-1) plr->imageHeight=sets->at(1);*/
			//plr->SetOrigin(plr->imageWidth/2,plr->imageHeight-static_cast<int>(plr->imageHeight/7.63));
			//plr->SetOrigin(plr->imageWidth/2,plr->imageHeight-6*engine->gridSize/32);
			//engine->placeIndex=0;
			break;
		};
		done=false;
		break;
			};
	};
	if (done) {
		temp->SetPosition(x, y);
		temp->chunk.x=floor(temp->x/chunkSize.x);
		temp->chunk.y=floor(temp->y/chunkSize.y);
		temp->depth=temp->y;
		chunks->at(temp->chunk.x)->at(temp->chunk.y)->list->push_back(static_cast<Object*>(temp));
		return temp;
	};
	return NULL;
};

Object* ObjectsManager::AddNpc(int x, int y, std::string name, sf::Texture* tex) {
	if (chunks==NULL) return NULL;
	Npc* temp;
	temp=new Npc(tex);
	temp->SetPosition(x, y);
	temp->chunk.x=floor(temp->x/chunkSize.x);
	temp->chunk.y=floor(temp->y/chunkSize.y);
	temp->depth=temp->y;
	temp->index=npcList.size();
	chunks->at(temp->chunk.x)->at(temp->chunk.y)->list->push_back(static_cast<Object*>(temp));
	npcList.insert(npcList.end(),std::pair<std::string,Object*>(name,temp));
	return temp;
};

void ObjectsManager::DeleteNpc(std::string name) {
	std::map<std::string,Object*>::iterator it=npcList.find(name);
	Object* obj;
	if (it!=npcList.end()) {
		obj=it->second;
		DeleteObject(obj);
	}
};

void ObjectsManager::AddMover(Object* obj, int x, int y, float sp) {
	if (obj!=NULL) movers.push_back(new Mover(obj,x,y,sp));
};

Overlay* ObjectsManager::AddOverlay(sf::Texture *tex) {
	Overlay* ovr=new Overlay(tex);
	overlays.push_back(ovr);
	return ovr;
};


void ObjectsManager::DeleteMover(int index) {
	movers[index]->object->isMoving=false;
	delete movers[index];
	movers.erase(movers.begin()+index);
};

Player* ObjectsManager::GetPlayer() {
	return player;
};

Npc* ObjectsManager::GetNpc(std::string name) {
	std::map<std::string,Object*>::iterator it=npcList.find(name);
	if (it==npcList.end()) return NULL;
	else return static_cast<Npc*>(it->second);
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
	player=NULL;
	for(int i=0;i<overlays.size();i++) {
		delete overlays.at(i);
	};
	overlays.clear();
	int size=npcList.size();
	for(int i=0;i<size;i++) {
		npcList.erase(npcList.begin());
	};
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
	if (sizeX>-1 && sizeY>-1) {
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
	engine->camera->SetBorders(0,0,chunkSize.x*chunksNumber.x,chunkSize.y*chunksNumber.y);
	loaded=true;
	return true;
};

ObjectsManager::~ObjectsManager() {
	Clear(-1,-1);
};