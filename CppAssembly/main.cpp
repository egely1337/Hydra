#include "../Hydra/Hydra.hpp"
class Radiant : public ECS {
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

class Gui : GuiRenderer {
	void Update() {
		ImGui::Begin("Hello World!");
		ImGui::End();
	}
};


class Spammer : Monobehaviour {
public:
	void Start() {
		for (int i = 0; i < 3000; i++) {
			SPAWN(new Mesh("Cu"));
		}
	}
};


class PlayerManager : Monobehaviour {
public:
	void Update() {
		if(IsKeyPressed(KeyCode::D)) {
			position->x += 0.1f * speed;
		}

		if (IsKeyPressed(KeyCode::A)) {
			position->x -= 0.1f * speed;
		}

		if (IsKeyPressed(KeyCode::W)) {
			position->y -= 0.1f * speed;
		}

		if (IsKeyPressed(KeyCode::S)) {
			position->y += 0.1f * speed;
		}

		if (IsKeyPressed(KeyCode::Escape)) {
			This->DeleteComponent<PlayerManager>();
		}
	}

	void Start() {
		//Start func.
	}

	float speed = 5;
};





extern "C" __declspec(dllexport) std::vector<ECS*> GetClasses() {
	std::vector<ECS*> v;
	v.push_back(new PlayerManager());
	v.push_back(new Radiant());
	v.push_back(new Spammer());
	return v;
}
