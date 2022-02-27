#include "EngineManager.h"


bool EngineManager::Load() {
	hydraModule = LoadLibrary(moduleDirectory.c_str());
	std::cout << "[LOADER] Trying to start engine...\n";
	if (hydraModule == MODULE_ERROR) {
		MessageBox(GetConsoleWindow(), L"Module couldnt found.", L"FATAL ERROR!", MB_OK | MB_ICONERROR);
		exit(EXIT_FAILURE);
		return false;
	}
	engineStart = (int(*)())GetProcAddress(hydraModule, "_startengine");
	if (engineStart == 0) {
		MessageBox(GetConsoleWindow(), L"We detected a problem. Reinstall Hydra.", L"FATAL ERROR!", MB_OK | MB_ICONERROR);
		exit(EXIT_FAILURE);
		return false;
	}
	return true;
}

bool EngineManager::Run() {
	std::cout << "ENGINE_LAUNCHER: Engine location: " << std::hex << "0x" << &engineStart << std::endl;
	int status = engineStart();
	FreeLibrary(hydraModule);
	if (status == 1) {
		MessageBox(GetConsoleWindow(), L"Hydra crashed!", L"FATAL ERROR!", MB_OK | MB_ICONERROR);
	}
	return true;
}