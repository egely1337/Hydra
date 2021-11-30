#include "Hydra.hpp"	
#include "Player.hpp"



class App : public Window {
public:
	Mesh* mesh;
	void Update() {

	}


	void Start() {
		mesh = scene.GetRenderObjects()->Instantiate(new Mesh("Hyper"));
		mesh->AddComponent<Player>();
	}
};

int main() {
	Window* window = new App();
	window->Init("Hydra Editor [Untitled]", 1920, 1080);
}