#include "Queue.h"

Queue::Queue(Scripting &scr) {
	scripting=scr;
	index=0;
	done=false;
};

void Queue::Add(std::wstring func, std::wstring time) {
	if (queue.size()==0) {
		timer.endTime=stof(time);
	};
	queue.push_back(func);
	queue.push_back(time);
};

void Queue::Update(float delta) {
	if (timer.endTime!=-1) {
		if (timer.time<timer.endTime) {
			timer.time+=delta;
		};
		if (timer.time>=timer.endTime) {
			scripting.ExecuteString(queue.at(index));
			if (done) {done=false; return;}
			if (index+2<queue.size()) {
				index+=2;
				timer.endTime=stof(queue.at(index+1));
				timer.time=0;
			} else {
				index=0;
				queue.clear();
				timer.time=0;
				timer.endTime=-1;
			};
		};
	};
	done=false;
};

void Queue::Clear() {
	queue.clear();
	timer.time=0;
	timer.endTime=-1;
	index=0;
	//queue.push_back(L"none");
	//queue.push_back(L"-1");
};