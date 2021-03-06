#include "TilesManager.h"
#include "Engine.h"

extern Engine* engine;
bool SortTilesPredicate(const Tile* o1, const Tile* o2) {
	return (o1->layer<o2->layer);
};

TilesManager::TilesManager() {
	chunkSize=engine->objectsManager->chunkSize;
	chunks=NULL;
	scaleFactor=engine->objectsManager->scaleFactor;
	Clear(3,3);
};

void TilesManager::Resize(int sizeX, int sizeY) {
	int sX=sizeX;
	if (sX<3) sX=3;
	int sY=sizeY;
	if (sY<3) sY=3;
	Clear(sX,sY);
};

void TilesManager::Draw(sf::RenderTarget &rt) {
	if (chunks==NULL) return;
	int chunkX = floor(engine->xView/chunkSize.x);
	int chunkY = floor(engine->yView/chunkSize.y);
	int chunkXStart = std::max(0,chunkX-1);
	int chunkYStart = std::max(0,chunkY-1);
	int chunkXEnd = std::min(chunksNumber.x,(chunkX+2)*scaleFactor);
	int chunkYEnd = std::min(chunksNumber.y,(chunkY+2)*scaleFactor);
	for(int i=chunkXStart;i<chunkXEnd;i++) {
		for(int j=chunkYStart;j<chunkYEnd;j++) {
			int size=chunks->at(i)->at(j)->list->size();
			std::sort(chunks->at(i)->at(j)->list->begin(),chunks->at(i)->at(j)->list->end(),SortTilesPredicate);
			for(int k=0;k<size;k++) {
				chunks->at(i)->at(j)->list->at(k)->Draw(rt);
			};
		};
	};
};

Tile* TilesManager::AddTile(std::string texName,int x, int y, int col, int row) {
	if (chunks==NULL) return NULL;
	Tile* temp;
	temp=new Tile(texName, x, y, col, row);
	temp->chunk.x=floor(static_cast<float>(temp->x)/chunkSize.x);
	temp->chunk.y=floor(static_cast<float>(temp->y)/chunkSize.y);
	temp->depth=temp->y;
	chunks->at(temp->chunk.x)->at(temp->chunk.y)->list->push_back(temp);
	return temp;
};

void TilesManager::DeleteTile(Tile* obj) {
	for (int i=0;i<chunks->at(obj->chunk.x)->at(obj->chunk.y)->list->size();i++) {
		if (chunks->at(obj->chunk.x)->at(obj->chunk.y)->list->at(i)==obj) {
			chunks->at(obj->chunk.x)->at(obj->chunk.y)->list->erase(
				chunks->at(obj->chunk.x)->at(obj->chunk.y)->list->begin()+i);
			delete obj;
			break;
		};
	};
};

bool TilesManager::SaveTiles(std::ofstream& save) {
	if (!save) {
		std::cout<<"\nFailed to save tiles\n";
		return false;
	};
	for (int i=0;i<chunks->size();i++) {
		for (int j=0;j<chunks->at(i)->size();j++) {
			for (int k=0;k<chunks->at(i)->at(j)->list->size();k++) {
				Tile* obj = chunks->at(i)->at(j)->list->at(k);
				save<<obj->texName<<" ";
				save<<ToString(obj->x)<<" ";
				save<<ToString(obj->y)<<" ";
				save<<ToString(obj->col)<<" ";
				save<<ToString(obj->row)<<" ";
				save<<ToString(obj->layer)<<" ";
				save<<"\n";
			};
		};
	};
	save<<"-404\n";
	return true;
};

void TilesManager::Clear(int sizeX, int sizeY) {
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
		chunks = new std::vector<std::vector<TileChunk*>*>;
		for(int i=0;i<chunksNumber.x;i++) {
			chunks->push_back(new std::vector<TileChunk*>);
			for(int j=0;j<chunksNumber.y;j++) {
				chunks->at(chunks->size()-1)->push_back(new TileChunk);
			};
		};
	};
};

Tile* TilesManager::GetTileAt(int x, int y) {
	int chunkX = floor(engine->xView/engine->tilesManager->chunkSize.x);
	int chunkY = floor(engine->yView/engine->tilesManager->chunkSize.y);
	int chunkXStart = std::max(0,chunkX-1);
	int chunkYStart = std::max(0,chunkY-1);
	int chunkXEnd = std::min(engine->tilesManager->chunksNumber.x,(chunkX+2)*scaleFactor);
	int chunkYEnd = std::min(engine->tilesManager->chunksNumber.y,(chunkY+2)*scaleFactor);
	for(int i=chunkXStart;i<chunkXEnd;i++) {
		for(int j=chunkYStart;j<chunkYEnd;j++) {
			int size=engine->tilesManager->GetChunks()->at(i)->at(j)->list->size();
			for(int k=0;k<size;k++) {
				bool collision=false;
				Tile* obj = engine->tilesManager->GetChunks()->at(i)->at(j)->list->at(k);
				if (x>obj->x) {
					if (y>obj->y) {
						if (x<obj->x+64) {
							if (y<obj->y+64) {
								collision=true;
							};
						};
					};
				};
				if (collision) {
					return obj;
				};
			};
		};
	};
	return NULL;
};

void TilesManager::DeleteAt(int x, int y) {
	Tile* collision=GetTileAt(x,y);
	if (collision!=NULL) {
		DeleteTile(collision);
	};
};

bool TilesManager::LoadTiles(std::ifstream& load) {
	if (!load) {
		std::cout<<"\nFailed to load tiles\n";
		return false;
	};

	Clear(engine->objectsManager->chunksNumber.x, engine->objectsManager->chunksNumber.y);

	while(load) {
		int x,y,col,row,layer;
		std::string texName;
		std::string script;
		load>>texName;
		if (texName=="-404") break;
		load>>x;
		load>>y;
		load>>col;
		load>>row;
		load>>layer;
		Tile* tile = AddTile(texName,x,y,col,row);
		tile->layer=layer;
	};
	return true;
};

TilesManager::~TilesManager() {
	Clear(-1,-1);
};

std::vector<std::vector<TileChunk*>*>* TilesManager::GetChunks() {
	return chunks;
};