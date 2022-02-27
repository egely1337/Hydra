#pragma once
#include <Windows.h>
#include <iostream>

#define MODULE_DIRECTORY L"./Hydra.dll"
#define MODULE_ERROR 0
#define MODULE_PASS	 1

class EngineManager {
public:
	bool Load();
	bool Run();
private:
	HMODULE hydraModule;
	int(*engineStart)();
	std::wstring moduleDirectory = MODULE_DIRECTORY;
};