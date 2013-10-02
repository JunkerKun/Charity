#include "Camera.h"
#include "Engine.h"
#include "Functions.h"

extern Engine* engine;

Camera::Camera() {
	borders=sf::IntRect(0,0,engine->windowSize.x,engine->windowSize.y);
	xView = 0;
	yView = 0;
	target=NULL;
	view.setSize(engine->windowSize.x, engine->windowSize.y);
	Update();
};

void Camera::SetBorders(int left, int top, int right, int bottom) {
	borders=sf::IntRect(left,top,right,bottom);
};

bool Camera::Update() {
	if (target!=NULL) {
		viewTo=Interpolate2D(viewTo, sf::Vector2f(target->x-(engine->windowSize.x/2),
			target->y-(engine->windowSize.y/2)),3);
		/*if (viewTo.x<borders.left) viewTo.x=borders.left;
	else
		if (viewTo.x+engine->windowSize.x>borders.width) viewTo.x=borders.width-engine->windowSize.x;
	if (viewTo.y<borders.top) viewTo.y=borders.top;
	else
		if (viewTo.y+engine->windowSize.y>borders.height) viewTo.y=borders.height-engine->windowSize.y;*/
		xView=floor(viewTo.x);
		yView=floor(viewTo.y);
	};
	if (xView<borders.left) xView=borders.left;
	else
		if (xView+engine->windowSize.x>borders.width) xView=borders.width-engine->windowSize.x;
	if (yView<borders.top) yView=borders.top;
	else
		if (yView+engine->windowSize.y>borders.height) yView=borders.height-engine->windowSize.y;

	view.setCenter(xView+engine->windowSize.x/2,yView+engine->windowSize.y/2);
	return true;
};

Camera::~Camera() {
};