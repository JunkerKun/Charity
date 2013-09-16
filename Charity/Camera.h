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
};

#endif