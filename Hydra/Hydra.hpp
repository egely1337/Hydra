#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <chrono>
#include <sstream>
#include <thread>
#define SFML_STATIC
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui-SFML.h>


typedef unsigned int uint_t;
using namespace std::chrono;

class Vector2 {
public:
	Vector2(float _x = 0, float _y = 0) { x = _x, y = _y; }
	float x;
	float y;
};


class Log {
public:
	std::vector<std::string> logData;

	static Log& GetLogger() {
		static Log l; 
		return l;
	}

	bool Info(std::string text) {
		std::stringstream buffer;
		buffer << "INFO: " << text;
		logData.push_back(buffer.str());
		std::cout << buffer.str() << std::endl;
		return true;
	}
	bool Warning(std::string text) {
		std::stringstream buffer;
		buffer << "WARNING: " << text;
		logData.push_back(buffer.str());
		std::cout << buffer.str() << std::endl;
		return true;
	}
	bool Error(std::string text) {
		std::stringstream buffer;
		buffer << "ERROR: " << text;
		logData.push_back(buffer.str());
		std::cout << buffer.str() << std::endl;
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

	Mesh(const char* gameObject) {
		name = gameObject;
	}

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

	const char* GetObjectName() {
		return name;
	}

protected:
	sf::CircleShape* shape;
	sf::RenderWindow* window;
	std::vector<ECS> components;
	const char* name;


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
			_fps = framePerSecond;
			framePerSecond = 0;
			refresh = high_resolution_clock::now();
		}

		return false;
	}

	int GetFps() {
		return _fps;
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
	int _fps;
	long fps = 10;
	float lastCPUTime = 0;
};

class Window {
public:
	Window() {};
	~Window() {}
	bool Init(const char* title, uint_t width, uint_t height) { window = new sf::RenderWindow(sf::VideoMode(width, height), title); Application(); return true; }
	bool Application(){
		Log::GetLogger().Info("Engine trying to get ready.");
		time.SetWindow(window);
		ImGui::SFML::Init(*window);
		objects.SetWindow(window);
		Log::GetLogger().Info("Ready!");
		Start();
		while (window->isOpen()) {
			while (time.Update()) {
				Update();
				Event();
				window->clear();
				window->pushGLStates();
				Render();
				ImGuiRenderer();
				window->popGLStates();
				ImGui::SFML::Render();
				window->display();
			}
		}
		ImGui::SFML::Shutdown();
		return true;
	}

	bool ImGuiRenderer() {
		ImGui::SFML::Update(*window, deltaClock.restart());
		SetupStyleImGui();
		
	
		ImGui::Begin("Hierarchy", (bool*)0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
		ImGui::Text("Scene");
		if (ImGui::CollapsingHeader("GameObjects")) {
			for (int i = 0; i < objects.GetRenderObjects().size(); i++) {
				if (ImGui::Selectable(objects.GetRenderObjects()[i]->GetObjectName())) {
					clickedData = i;
				}
			}
		}
		ImGui::SetWindowPos({ 0, 0 });
		ImGui::SetWindowPos({ 0, 0 });
		ImGui::SetWindowSize({ 300,(float)window->getSize().y});
		ImGui::End();



		ImGui::Begin("Inspector", (bool*)0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
		if (ImGui::CollapsingHeader("Object Data")) {
			float* x = &objects.GetRenderObjects()[clickedData]->position.x;
			float* y = &objects.GetRenderObjects()[clickedData]->position.y;
			ImGui::Text(objects.GetRenderObjects()[clickedData]->GetObjectName());
			ImGui::SliderFloat("X", x ,0.f, window->getSize().x);
			ImGui::SliderFloat("Y", y ,0.f, window->getSize().y);
		}
		ImGui::SetWindowPos({ (float)window->getSize().x -300, 0 });
		ImGui::SetWindowSize({ 300,(float)window->getSize().y });
		ImGui::End();

		ImGui::Begin("Console",(bool*)false,ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
		for (size_t i = 0; i < Log::GetLogger().logData.size(); i++) {
			std::string s = Log::GetLogger().logData[i];
			ImGui::Text(s.c_str());
		}
		ImGui::SetWindowPos({ (float)window->getPosition().x + (ImGui::GetWindowSize().x - 1025) , (float)window->getSize().y - (ImGui::GetWindowSize().y)});
		ImGui::SetWindowSize({ 1326 , 164});
		ImGui::End();

		return true;
	}

	bool Event() {
		if (window->pollEvent(event)) {
			ImGui::SFML::ProcessEvent(event);
			if (event.type == sf::Event::Closed) {
				window->close();
			}
		}
		return true;
	}

	bool Render() 
	{
		for (auto& b : objects.GetRenderObjects()) {
			b->Render();
		}
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

	int GetFPS() {
		return time.GetFps();
	}

	void SetupStyleImGui() {
		//Credit: https://github.com/Derydoca

		ImGuiStyle* style = &ImGui::GetStyle();
		ImVec4* colors = style->Colors;
		colors[ImGuiCol_Text] = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.500f, 0.500f, 0.500f, 1.000f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.180f, 0.180f, 0.180f, 1.000f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.280f, 0.280f, 0.280f, 0.000f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);
		colors[ImGuiCol_Border] = ImVec4(0.266f, 0.266f, 0.266f, 1.000f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.000f, 0.000f, 0.000f, 0.000f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.200f, 0.200f, 0.200f, 1.000f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.280f, 0.280f, 0.280f, 1.000f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.277f, 0.277f, 0.277f, 1.000f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.300f, 0.300f, 0.300f, 1.000f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
		colors[ImGuiCol_CheckMark] = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
		colors[ImGuiCol_Button] = ImVec4(1.000f, 1.000f, 1.000f, 0.000f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
		colors[ImGuiCol_ButtonActive] = ImVec4(1.000f, 1.000f, 1.000f, 0.391f);
		colors[ImGuiCol_Header] = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
		colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(1.000f, 1.000f, 1.000f, 0.250f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.000f, 1.000f, 1.000f, 0.670f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
		colors[ImGuiCol_Tab] = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.352f, 0.352f, 0.352f, 1.000f);
		colors[ImGuiCol_TabActive] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.586f, 0.586f, 0.586f, 1.000f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
		colors[ImGuiCol_NavHighlight] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);

		style->ChildRounding = 4.0f;
		style->FrameBorderSize = 1.0f;
		style->FrameRounding = 2.0f;
		style->GrabMinSize = 7.0f;
		style->PopupRounding = 2.0f;
		style->ScrollbarRounding = 12.0f;
		style->ScrollbarSize = 13.0f;
		style->TabBorderSize = 1.0f;
		style->TabRounding = 0.0f;
		style->WindowRounding = 4.0f;

	}


protected:
	sf::RenderWindow* window = NULL;
	sf::Event event;
	RenderObjects objects;
	Time time;
	sf::Clock deltaClock;
	int clickedData = 0;
};