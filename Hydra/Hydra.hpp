#pragma once
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <chrono>
#include <sstream>
#include <thread>
#define SFML_STATIC


typedef unsigned int uint_t;
using namespace std::chrono;

class Vector2 {
public:
	float x;
	float y;
};


class Log {
public:
	static bool Info(const char* text) {
		std::stringstream buffer;
		buffer << text;
		std::cout << "INFO: " << buffer.str() << std::endl;
		return true;
	}
};

class ECS {
public:
	virtual void Start(){};
	virtual void Update(){};
};


class Mesh {
public:
	Vector2 position;


	void Initialize() {
		shape = new sf::CircleShape(50);
	}

	bool Update() {
		shape->setPosition(sf::Vector2f(position.x, position.y));
		return true;
	}

	bool Render() {
		window->draw(*shape);
		Update();
		return true;
	}

	bool SetWindow(sf::RenderWindow* m_RenderWindow) {
		window = m_RenderWindow;
		return true;
	}

	bool SetPosition(Vector2 _vec) {
		position = _vec;
		return true;
	}

protected:
	sf::CircleShape* shape;
	sf::RenderWindow* window;
	std::vector<ECS> components;


};


class RenderObjects {
public:

	bool SetWindow(sf::RenderWindow* v_RenderWindow) { e_RenderWindow = v_RenderWindow; return true;}

	Mesh* Instantiate(Mesh* _mesh){
		_mesh->Initialize();
		_mesh->SetWindow(e_RenderWindow);
		e_RenderObjects.push_back(_mesh);
		return _mesh;
	}

	std::vector<Mesh*> GetRenderObjects(){
		return e_RenderObjects;
	}
protected:
	sf::RenderWindow* e_RenderWindow;
	std::vector<Mesh*> e_RenderObjects;
};

class Time {
public:

	bool SetWindow(sf::RenderWindow* window) {
		m_Window = window;
		return true;
	}
	bool Update() {
		if (duration_cast<milliseconds>(high_resolution_clock::now() - now) >= milliseconds(1000 / fps)) {
			framePerSecond++;
			lastCPUTime = duration_cast<milliseconds>(high_resolution_clock::now() - now).count();
			now = high_resolution_clock::now();
			return true;
		}

		if (duration_cast<seconds>(high_resolution_clock::now() - refresh) >= seconds{ 1 }) {
			std::stringstream ss;
			ss << "FPS: " << framePerSecond << " " << "CPU Time: " << lastCPUTime << std::endl;
			printf(ss.str().c_str());
			framePerSecond = 0;
			refresh = high_resolution_clock::now();
		}

		return false;
	}

	bool SetFPS(long _framePerSecond) {
		fps = _framePerSecond;
		return true;
	}
protected:
	time_point<high_resolution_clock> now = high_resolution_clock::now();
	time_point<high_resolution_clock> refresh = high_resolution_clock::now();
	sf::RenderWindow* m_Window;
	int framePerSecond = 0;
	long fps = 10;
	float lastCPUTime = 0;
};

class Window {
public:
	Window() {};
	~Window() {}
	bool Init(const char* title, uint_t width, uint_t height) { window = new sf::RenderWindow(sf::VideoMode(width, height), title); Application(); return true; }
	bool Application(){
		Log::Info("Application has started.");
		time.SetWindow(window);
		objects.SetWindow(window);
		Start();
		while (window->isOpen()) {
			while (time.Update()) {
				Update();
				Event();
				window->clear();
				Render();
				window->display();
			}
		}
		return true;
	}
	bool Event() {
		if (window->pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window->close();
			}
		}
		return true;
	}

	bool Render() {
		for (auto& b : objects.GetRenderObjects()) {
			b->Render();
		}
		ImGui::SFML::Init(*window);
		return true;
	}

	virtual void Start() {};
	virtual void Update() {};
	
	bool ApplicationStart() {
		return true;
	}

	bool MainThread() {
		Update();
		return true;
	}

	RenderObjects* ObjectManager() {
		return &objects;
	}

	void SetFPS(double fps) {
		time.SetFPS(fps);
	}
protected:
	sf::RenderWindow* window = NULL;
	sf::Event event;
	RenderObjects objects;
	Time time;
};