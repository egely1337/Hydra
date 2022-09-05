#include "Hydra.hpp"	
#include "Player.hpp"



class App : public Window {
public:
	void Update() {

	}


	void Start() {
		SetFPS(90*100);
	}
};

extern "C" {
	__declspec(dllexport) int _startengine() {
		Window* window = new App();
		int exit_code = window->Init("Hydra Editor [Untitled]", 1920, 1080);
		return exit_code;
	}
}