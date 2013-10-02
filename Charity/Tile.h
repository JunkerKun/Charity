#ifndef __TILE
#define __TILE

#include <SFML\Graphics.hpp>

class Tile {
public:
	Tile(std::string texName, int x, int y, int col, int row);
	~Tile();
	int x,y, col, row, depth, layer;
	std::string texName;
	sf::Vector2i chunk;
	bool Draw(sf::RenderTarget& rt);
public:
	sf::Sprite sprite;
};

#endif