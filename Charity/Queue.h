#ifndef __QUEUE
#define __QUEUE

#include "Scripting.h"

struct Timer {
	float time, endTime;
	Timer() {
		time=0;
		endTime=-1;
	};
};

class Queue {
public:
	std::vector<std::wstring> queue;
	bool done;
	Scripting scripting;
	int index;
	Timer timer;
	Queue(Scripting &scr);
	void Add(std::wstring func, std::wstring time);
	void Update(float delta);
	void Clear();
};

#endif