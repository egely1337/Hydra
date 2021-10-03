#include "Hydra.hpp"	
#include "Player.hpp"



class App : public Window {
public:
	Mesh* mesh;
	void Update() {
		
	} 

	void Start() {
		SetFPS(90);
		Play(true);
		mesh = scene.GetRenderObjects()->Instantiate(new Mesh("Hyper","asdas"));
		mesh->SetPosition(Vector2((float)1920 / 2,(float)1080 / 2));
		mesh->AddComponent(new Player());
	}
};

int main() {
	Window* window = new App();
	window->Init("Hydra", 1920, 1080);
}