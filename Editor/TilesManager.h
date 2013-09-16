#ifndef __TILESMANAGER
#define __TILESMANAGER

#include "Tile.h"
#include <vector>
#include <string>
#include <fstream>

struct TileChunk {
	std::vector<Tile*>* list;
	TileChunk() {
		list = new std::vector<Tile*>;
	};
	~TileChunk() {
		delete list;
	};
};

class TilesManager {
public:
	TilesManager();
	~TilesManager();
	void Draw(sf::RenderTarget &rt);
	Tile* AddTile(std::string texName,int x, int y, int col, int row);
	void DeleteTile(Tile* obj);
	void DeleteAt(int x, int y);
	void Clear(int sizeX, int sizeY);
	void Resize(int sizeX, int sizeY);
	std::vector<std::vector<TileChunk*>*>* GetChunks();
	Tile* GetTileAt(int x, int y);
	bool SaveTiles(std::ofstream& file);
	bool LoadTiles(std::ifstream& file);
private:
	sf::Vector2i chunkSize;
	sf::Vector2i chunksNumber;
	std::vector<std::vector<TileChunk*>*>* chunks;
};

#endif