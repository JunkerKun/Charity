#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <locale>
#include <codecvt>

class Scripting {
public:
	Scripting();
	std::wstring WrapString(std::wstring str, int linewidth);
	std::string ToString(float number);
	std::wstring ToWString(float number);
	std::string wStringToString(std::wstring str);
	std::wstring StringToWString(std::string str);
	std::wstring Calculate(std::wstring str);
	void SplitString(std::wstring str, std::vector<std::wstring> &stringsarray);
	bool ExecuteCommand(std::vector<std::wstring> &parameters);
	bool ExecuteString(std::wstring string);
	bool ExecuteFile(std::string filename);
	bool ExecuteFunction(std::wstring function);
	bool LoadFile(std::string filename);
	void Execute();
};