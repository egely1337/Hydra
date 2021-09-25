#include "Hydra.hpp"	




class App : public Window {
public:
	Mesh* mesh;

	void Update() {
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			mesh->position.x += 10;
	} 

	void Start() {
		SetFPS(500);
		mesh = ObjectManager()->Instantiate(new Mesh("Hyper"));
		Mesh* mesh2 = ObjectManager()->Instantiate(new Mesh("adas"));
		Mesh* mesh3 = ObjectManager()->Instantiate(new Mesh("Hyasdasdasper"));
		mesh->SetPosition(Vector2((float)1920 / 2,(float)1080 / 2));
	}
};

int main() {
	Window* window = new App();
	window->Init("Hydra", 1920, 1080);
}