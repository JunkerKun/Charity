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

std::string ToString(float number) {
	std::stringstream str;
	str<<number;
	return str.str();
};

std::wstring ToWString(float number) {
	std::wstringstream str;
	str<<number;
	return str.str();
};

std::string wStringToString(std::wstring str) {
	char* buffer = new char[str.length()+1];
	sprintf(buffer,"%ls",str.c_str());
	std::string string;
	string = buffer;
	delete buffer;
	return string;
};

std::wstring StringToWString(std::string str) {
	std::wstring string;
	string.assign(str.begin(),str.end());
	return string;
};