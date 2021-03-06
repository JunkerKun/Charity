#ifndef __COLLISIONS
#define __COLLISIONS

#include "Object.h"

bool CollisionRadius(float rad, float x, float y, Object* obj);
bool CollisionPoint(float x, float y,Object* obj);
bool CollisionIntersect(Object* o1, Object* o2);
Object* CollisionCheckRadius(float rad, float x, float y, unsigned int index);
Object* CollisionCheckPoint(float x, float y, unsigned int index);
Object* CollisionCheckIntersect(Object* o1, unsigned int index);
#endif