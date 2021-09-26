#pragma once
#include "Hydra.hpp"


class Player : public ECS {
public:
	void Update() {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			position->x += 1;
		}
	}

	void Start() {
		Log::GetLogger().Info("Spawn");
	}
};