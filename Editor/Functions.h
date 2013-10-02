#ifndef __FUNCTIONS
#define __FUNCTIONS
#include <string>
#include <sstream>

float Increment(float start, float end, float& speed);
std::string ToString(float number);
	std::wstring ToWString(float number);
	std::string wStringToString(std::wstring str);
	std::wstring StringToWString(std::string str);
#endif