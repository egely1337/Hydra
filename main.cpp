#include "../Hydra/Hydra.hpp"

class Cube;
class Player;
class GameManager;

class Player : public ECS {
public:

	float angle = 0;
	std::vector<const char*> heap;
	void Update() {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			position->x += 1;
		}

		angle += 0.1f + Time->deltaTime();
		position->x = cos(angle / 180 * 3.14) * 300 + 933;
		position->y = sin(angle / 180 * 3.14) * 300 + 443;
	}

	void Start() {
		Log::GetLogger().Info("Hello World!");
	}
};

class GameManager : public ECS {
public:
	void Update() {
	}
	void Start() {
		This->AddComponent(new Player());
	}
private:

};


class Cube : public ECS {
public:
	void Update() {
		
	}
	void Start() {
		This->AddComponent(new GameManager());
	}
private:

};


class EmptyClass : public ECS {
public:
	void Update() {

	}
	void Start() {
		This->AddComponent(new GameManager());
	}
private:

};



extern "C" __declspec(dllexport) std::vector<ECS*> GetClasses() {
	std::vector<ECS*> v;
	v.push_back(new Player());
	v.push_back(new Cube());
	v.push_back(new GameManager());
	v.push_back(new EmptyClass());
	return v;
}
