#include "Hydra.hpp"	
#include "Player.hpp"



class App : public Window {
public:
	Mesh* mesh;
	Mesh* mesh2;
	void Update() {
		//if (mesh->position.x < mesh2->position.x + mesh2->scale.x
		//	&& mesh->position.x + mesh->scale.x > mesh2->position.x) {
		//	Log::GetLogger().Info("Collision!");
		//}
		//if (mesh2->position.x < mesh->position.x + mesh->scale.x
		//	&& mesh2->position.x + mesh2->scale.x > mesh->position.x) {
		//	Log::GetLogger().Info("Collision!");
		//} TODO
	} 


	void Start() {

	}
};

int main() {
	Window* window = new App();
	window->Init("Hydra", 1920, 1080);
}