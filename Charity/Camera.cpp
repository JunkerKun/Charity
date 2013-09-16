#include "Camera.h"
#include "Engine.h"

extern Engine* engine;

Camera::Camera() {
	xView = 0;
	yView = 0;
	target=NULL;
	view.setSize(engine->windowSize.x, engine->windowSize.y);

	Update();
};

bool Camera::Update() {
	if (target!=NULL) {
		if (xView>0 && xView+engine->windowSize.x<engine->objectsManager->chunksNumber.x*engine->objectsManager->chunkSize.x) 
			xView=floor(target->x-(engine->windowSize.x/2));

		if (yView>0 && yView+engine->windowSize.y<engine->objectsManager->chunksNumber.y*engine->objectsManager->chunkSize.y) 
		yView=floor(target->y-(engine->windowSize.y/2));
	};
	view.setCenter(xView+engine->windowSize.x/2,yView+engine->windowSize.y/2);
	return true;
};

Camera::~Camera() {
};