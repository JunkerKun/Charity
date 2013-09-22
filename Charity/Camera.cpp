#include "Camera.h"
#include "Engine.h"
#include "Functions.h"

extern Engine* engine;

Camera::Camera() {
	xView = 0;
	yView = 0;
	target=NULL;
	view.setSize(engine->windowSize.x*engine->screenScale, engine->windowSize.y*engine->screenScale);
	Update();
};

bool Camera::Update() {
	if (target!=NULL) {
		viewTo=Interpolate2D(viewTo, sf::Vector2f(target->x-(engine->windowSize.x/2*engine->screenScale),
			target->y-(engine->windowSize.y/2*engine->screenScale)),3);
		xView=floor(viewTo.x);
		yView=floor(viewTo.y);
	};
	view.setCenter(xView+engine->windowSize.x/2*engine->screenScale,yView+engine->windowSize.y/2*engine->screenScale);
	return true;
};

Camera::~Camera() {
};