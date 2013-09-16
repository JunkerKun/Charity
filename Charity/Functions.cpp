#include "Functions.h"
#include "Engine.h"

extern Engine* engine;

float Increment(float start, float end, float& speed) {
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