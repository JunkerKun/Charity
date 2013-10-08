#ifndef __FUNCTIONS
#define __FUNCTIONS

#include <SFML/System.hpp>

float Distance(int x1, int y1, int x2, int y2);
float Increment(float start, float end, float speed);
sf::Vector2f Interpolate2D(sf::Vector2f vec1, sf::Vector2f vec2, float scalar);
float Interpolate2D(float begin, float end, float scalar);
#endif