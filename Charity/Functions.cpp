#include "Functions.h"
#include "Engine.h"

extern Engine* engine;

float Distance(int x1, int y1, int x2, int y2) {
	return sqrt(pow(static_cast<double>(x2-x1),2)+pow(static_cast<double>(y2-y1),2));
};

float Increment(float start, float end, float speed) {
	float val = start;
	if (val!=end) {
		if (val>end) {
			val-=speed*engine->GetDelta();
			if (val<end) {val=end;};
		} else if (val<end) {
			val+=speed*engine->GetDelta();
			if (val>end) {val=end;};
		};
	};
	return val;
};

sf::Vector2f Interpolate2D(sf::Vector2f vec1, sf::Vector2f vec2, float scalar) {
	sf::Vector2f result;
	result.x = vec1.x*(1-scalar*engine->GetDelta())+vec2.x*scalar*engine->GetDelta();
	result.y = vec1.y*(1-scalar*engine->GetDelta())+vec2.y*scalar*engine->GetDelta();
	return result;
};

float Interpolate2D(float begin, float end, float scalar) {
	float result;
	result = begin*(1-scalar*engine->GetDelta())+end*scalar*engine->GetDelta();
	return result;
};