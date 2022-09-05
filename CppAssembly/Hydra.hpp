#include <iostream>
#include <vector>
#include <chrono>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <bitset>	
#include "../Hydra/UUID.h"
typedef unsigned int uint_t;
using namespace std::chrono;
#define INFO(a) Logger->Info(a)
#define WARNING(a) Logger->Warning(a)
#define ERROR(a) Logger->Error(a)

class Time {
public:
	float fpsRates[5];

	bool SetWindow(sf::RenderWindow* window) {
		m_Window = window;
		for (int i = 0; i < 5; i++) {
			fpsRates[i] = 0;
		}
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
			_fps = framePerSecond;
			framePerSecond = 0;
			fpsVector.push_back(_fps);
			if (fpsVector.size() == 5) {
				for (int i = 0; i < 5; i++) {
					fpsRates[i] = fpsVector[i];
				}
				fpsVector.clear();
			}
			refresh = high_resolution_clock::now();
		}
		Sleep(1000 / fps);


		return false;
	}



	int GetFps() {
		return _fps;
	}

	bool SetFPS(long _framePerSecond) {
		fps = _framePerSecond;
		return true;
	}

	float deltaTime() {
		return lastCPUTime;
	}
protected:
	time_point<high_resolution_clock> now = high_resolution_clock::now();
	time_point<high_resolution_clock> refresh = high_resolution_clock::now();
	sf::RenderWindow* m_Window;
	int framePerSecond = 0;
	int _fps;
	long fps = 10;
	float lastCPUTime = 0;
	std::vector<int> fpsVector;

};
class Utils {
public:
	static std::string StringToBinary(std::string data) {
		std::string binary = "";
		for (auto& b : data) {
			binary += std::bitset<8>(b).to_string();
		}
		return binary;
	}

	static bool startswith(std::string data, std::string arg) {
		std::stringstream ss;
		for (auto& b : data) {
			if (b == 0x20) {
				std::cout << "Spotted!";
				break;
			}
			ss << b;
		}
		if (ss.str() == arg) {
			return true;
		}
		return false;
	}

	static std::vector<std::string> getwords(std::string data) {
		std::stringstream ss;
		std::vector<std::string> array;
		int i = 0;
		for (auto& b : data) {
			ss << b;
			i++;
			switch (b)
			{
			case 0x20:
				array.push_back(ss.str());
				ss = std::stringstream();
				break;
			}
			if (i > data.size() - 1) {
				array.push_back(ss.str());
				ss = std::stringstream();
				break;
			}
		}
		return array;
	}
};
class Vector2 {
public:
	Vector2 operator+=(Vector2 temp) { this->x += temp.x; this->y += temp.y; return *this; }
	Vector2(float _x = 1, float _y = 1) { x = _x, y = _y; }
	float x;
	float y;
};
class Keyboard {
public:
	static Keyboard GetKeyboard() {
		static Keyboard k;
		return k;
	}
	//TODO
};
class Log {
public:
	std::vector<std::string> logData;

	static Log& GetLogger() {
		static Log l;
		return l;
	}

	bool Clear() {
		logData.clear();
		return true;
	}

	bool Info(std::string text) {
		std::stringstream buffer;
		buffer << "INFO " << logData.size() << ": " << text;
		logData.push_back(buffer.str());
		std::cout << buffer.str() << std::endl;
		return true;
	}
	bool Warning(std::string text) {
		std::stringstream buffer;
		buffer << "WARNING " << logData.size() << ": " << text;
		logData.push_back(buffer.str());
		std::cout << buffer.str() << std::endl;
		return true;
	}
	bool Error(std::string text) {
		std::stringstream buffer;
		buffer << "ERROR " << logData.size() << ": " << text;
		logData.push_back(buffer.str());
		std::cout << buffer.str() << std::endl;
		return true;
	}

};
class Mesh {
public:
	Vector2 position;
	Vector2 scale;
	Vector2 textureRect;

	Mesh(std::string gameObject, std::string textureLoc = "default_assets/square.png") {
		LoadTexture(textureLoc);
		name = gameObject;
		textureRect.x = 32;
		textureRect.y = 32;
	}

	void Initialize() {
		UUID = uuid::generate_uuid_v4();
		shape = new sf::Sprite(o_Texture);
	}

	std::string GetTextureDirectory() {
		return textureDirectory;
	}

	void SetScale(float _x, float _y) {
		scale.x = _x;
		scale.y = _y;
	}
	template <typename T>
	T& AddComponent() {
		T* component = new T();
		component->position = &position;
		component->scale = &scale;
		component->textureRect = &textureRect;
		component->Time = time;
		component->Logger = &Log::GetLogger();
		component->Start();
		std::unique_ptr<ECS> ptr({ component });
		components.push_back(std::move(ptr));
		return *component;
	}

	void AddComp(ECS* component) {
		component->position = &position;
		component->scale = &scale;
		component->textureRect = &textureRect;
		component->Time = time;
		component->Logger = &Log::GetLogger();
		component->Start();
		std::unique_ptr<ECS> ptr({ component });
		components.push_back(std::move(ptr));
	}

	void HelloWorld() {
		std::cout << "Hello World from Instantiate!";
	}


	sf::Sprite* getSprite() {
		return shape;
	}

	bool LoadTexture(std::string _dir) {
		if (!o_Texture.loadFromFile(_dir)) {
			Log::GetLogger().Error("Can't get a texture.");
			Log::GetLogger().Info("Trying to get default texture.");
			if (o_Texture.loadFromFile("default_assets/square.png")) {
				Log::GetLogger().Info("Succesfully get default texture.");
				textureDirectory = "default_assets/square.png";
				return true;
			}
			return false;
		}
		Log::GetLogger().Info("Succesfully get a texture.");
		textureDirectory = _dir;
		return true;
	}


	bool Update() {
		for (auto& b : components) {
			b->Update();
		}
		shape->setPosition(sf::Vector2f(position.x, position.y));
		shape->setScale(sf::Vector2f(scale.x, scale.y));
		shape->setTextureRect(sf::IntRect(64, 64, textureRect.x, textureRect.y));
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

	std::string GetObjectName() {
		return name;
	}

	bool SetName(std::string _name) {
		name = _name;
		return true;
	}

	std::string GetUUID() {
		return UUID;
	}
	void SetUUID(std::string _uuid) {
		UUID = _uuid;
	}

	Time* time;
protected:
	sf::Sprite* shape;
	std::string name;
	sf::RenderWindow* window;
	std::vector<std::unique_ptr<ECS>> components;
	sf::Texture o_Texture;
	std::string textureDirectory;
	std::string UUID;
};
class ECS {
public:
	//TODO
	virtual void Start() {};
	virtual void Update() {};

	Vector2* position;
	Vector2* scale;
	Vector2* textureRect;
	Time* Time;
	Log* Logger;
	void (*Instantiate)();
};