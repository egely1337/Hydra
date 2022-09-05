#include <iostream>
#include <Windows.h>
#include "EngineManager.h"



int main()
{
	EngineManager engineManager;
	engineManager.Load();
	engineManager.Run();

	exit(EXIT_SUCCESS);
}