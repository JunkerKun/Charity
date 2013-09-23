#ifndef __CAMERA
#define __CAMERA

#include <SFML/Graphics.hpp>
#include "Object.h"

class Camera {
public:
	Camera();
	~Camera();
	sf::View view;
	float xView, yView;
	bool Update();
	Object* target;
	sf::Vector2f viewTo;
	sf::IntRect borders;
	void SetBorders(int left, int top, int right, int bottom);
};

#endif