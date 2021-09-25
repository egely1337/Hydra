#include "Hydra.hpp"	




class App : public Window {
public:
	Mesh* mesh;

	void Update() {
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			mesh->position.x += 10;
	} 

	void Start() {
		SetFPS(120);
		mesh = ObjectManager()->Instantiate(new Mesh());
	}
};

int main() {
	Window* window = new App();
	window->Init("Hydra", 800, 600);
}