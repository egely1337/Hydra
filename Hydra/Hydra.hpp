#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <SFML/Graphics.hpp>
#include <iostream>
#include <future>
#include <vector>
#include <chrono>
#include <sstream>
#define SFML_STATIC
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui-SFML.h>
#include <imfilebrowser.h>
#include <Windows.h>
#include <stdexcept>
#include <fstream>
typedef unsigned int uint_t;
using namespace std::chrono;



class Vector2 {
public:
	Vector2(float _x = 1, float _y = 1) { x = _x, y = _y; }
	float x;
	float y;
};

class Keyboard {
public:
	static Keyboard GetKeyboard(){
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

class ECS {
public:
	//TODO
	virtual void Start(){};
	virtual void Update(){};

	Vector2* position;
	Vector2* scale;
	Vector2* textureRect;
};


class Mesh {
public:
	Vector2 position;
	Vector2 scale;
	Vector2 textureRect;

	Mesh(std::string gameObject,std::string textureLoc = "default_assets/square.png") {
		LoadTexture(textureLoc);
		name = gameObject;
		textureRect.x = 32;
		textureRect.y = 32;
	}

	void Initialize() {
		shape = new sf::Sprite(o_Texture);
	}

	std::string GetTextureDirectory() {
		return textureDirectory;
	}

	void SetScale(float _x, float _y) {
		scale.x = _x;
		scale.y = _y;
	}
	void AddComponent(ECS* component) {
		components.push_back(component);
		component->Start();
		component->position = &position;
		component->scale = &scale;
		component->textureRect = &textureRect;
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
			}
			return false;
		}
		Log::GetLogger().Info("Succesfully get a texture.");
		textureDirectory = _dir;
		return true;
	}


	bool Update() {

		for (auto* b : components) {
			b->Update();
		}
		shape->setPosition(sf::Vector2f(position.x, position.y));
		shape->setScale(sf::Vector2f(scale.x, scale.y));
		shape->setTextureRect(sf::IntRect(32, 32, textureRect.x, textureRect.y));
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

protected:
	sf::Sprite* shape;
	std::string name;
	sf::RenderWindow* window;
	std::vector<ECS*> components;
	sf::Texture o_Texture;
	std::string textureDirectory;

	


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

	void Clear() {
		e_RenderObjects.clear();
	}

	void ClearById(int _id) {
		e_RenderObjects.erase(e_RenderObjects.begin() + _id);
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

	float getCpuTime() {
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
};


class Scene {
public:
	bool SaveScene() { 
		return true;
	};


	bool SaveScene(std::string path) {
		_set_abort_behavior(0, _WRITE_ABORT_MSG);
		SaveFile(path);
		std::stringstream ss;
		ss << "Successfully saved file in [" << path << "]";
		Log::GetLogger().Info(ss.str());
		return true;
	}

	bool ResetScene() {
		s_RenderObjects.Clear();
		return true;
	}

	bool LoadScene(std::string path) {
		_set_abort_behavior(0, _WRITE_ABORT_MSG);
		s_RenderObjects.Clear();
		_saveFile.clear();
		LoadFile(path);

		for (size_t i = 0; i < _saveFile.size(); i++) {

			if (_saveFile[i] == "[GAMEOBJECT]") {
				struct Entity {
					std::string name;
					float posx = 0;
					float posy = 0;
					float scalex = 0;
					float scaley = 0;
					std::string textureLocation;
				};
				Entity ent;
				try{
					ent.name = _saveFile[i + 1];
					ent.posx = std::stof(_saveFile[i + 2]);
					ent.posy = std::stof(_saveFile[i + 3]);
					ent.scalex = std::stof(_saveFile[i + 4]);
					ent.scaley = std::stof(_saveFile[i + 5]);
					ent.textureLocation = _saveFile[i + 6];
				}
				catch (const std::invalid_argument& b) {
					std::stringstream ss;
					ss << "Cannot load scene (" << b.what() << ") " << "[" << __FILE__ << ":" << __LINE__ << "]";
					Log::GetLogger().Error(ss.str());
					return false;
				}
				Mesh* m = s_RenderObjects.Instantiate(new Mesh(ent.name));
				m->position.x = ent.posx;
				m->position.y = ent.posy;
				m->scale.x = ent.scalex;
				m->scale.y = ent.scaley;
				m->LoadTexture(ent.textureLocation);


			}
		}
		return true;
	};


	RenderObjects* GetRenderObjects() {
		return &s_RenderObjects;
	}
protected:
	bool LoadFile(std::string path) {
		std::ifstream ss;
		ss.open(path);
		std::string data;
		if (ss.is_open()) {
			while (std::getline(ss, data)) {
				_saveFile.push_back(data);
			}
		}
		return true;
	}

	bool SaveFile(std::string path) {
		std::ofstream ss;
		ss.open(path);
		for(auto& b: s_RenderObjects.GetRenderObjects()){
			ss << "[GAMEOBJECT]\n";
			ss << b->GetObjectName() << std::endl;
			ss << b->position.x << std::endl;
			ss << b->position.y << std::endl;
			ss << b->scale.x << std::endl;
			ss << b->scale.y << std::endl;
			ss << b->GetTextureDirectory() << std::endl;
		}
		return true;
	}
	RenderObjects s_RenderObjects;
	std::vector<std::string> _saveFile;
};

class Window {
public:
	Window() {}
	~Window() {}
	bool Init(const char* title, uint_t width, uint_t height) { window = new sf::RenderWindow(sf::VideoMode(width, height), title); Application(); return true; }
	bool Application(){
		Log::GetLogger().Info("Engine trying to get ready.");
		time.SetWindow(window);
		ImGui::SFML::Init(*window);
		scene.GetRenderObjects()->SetWindow(window);
		Log::GetLogger().Info("Ready!");
		SetFPS(60);
		std:async(std::launch::async, &Window::Start, this);
		while (window->isOpen()) {
			while (time.Update()) {
				Update();
				ApplicationUpdate();
				window->clear();
				window->pushGLStates();
				Render();
				if (playable)
					ImGuiRenderer();
				window->popGLStates();
				if (playable)
					ImGui::SFML::Render();
				window->display();
				Event();
			}
		}
		ImGui::SFML::Shutdown();
		return true;
	}


	bool ImGuiRenderer() {

		ImGui::SFML::Update(*window, deltaClock.restart());
		SetupStyleImGui();

		if (sceneBrowser.HasSelected()) {
			scene.LoadScene(sceneBrowser.GetSelected().generic_string());
			sceneBrowser.ClearSelected();
		}

		if (saveSceneBrowser.HasSelected()) {
			scene.SaveScene(saveSceneBrowser.GetSelected().generic_string());
			saveSceneBrowser.ClearSelected();
		}
		sceneBrowser.SetTypeFilters({ ".hscene" });
		sceneBrowser.SetTitle("Select a scene.");
		sceneBrowser.Display();
		saveSceneBrowser.SetTypeFilters({ ".hscene" });
		saveSceneBrowser.SetTitle("Select a scene");
		saveSceneBrowser.Display();
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("New")) {
					scene.ResetScene();
				}
				if (ImGui::MenuItem("Save")) {
					saveSceneBrowser.Open();
				}
				if (ImGui::MenuItem("Load")) {
					sceneBrowser.Open();			
				}
				if (ImGui::MenuItem("Play")) {
					Play(false);
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Settings")) {
				if (ImGui::MenuItem("Engine Settings")) {
					settings = true;
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Toolbar")) {
				if (ImGui::MenuItem("Clear Console")) {
					Log::GetLogger().Clear();
					Log::GetLogger().Info("Logs are cleared.");
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

	
		ImGui::Begin("Hierarchy", (bool*)0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
		ImGui::Text("Scene");
		if (ImGui::CollapsingHeader("GameObjects")) {
			if (!scene.GetRenderObjects()->GetRenderObjects().empty()) {
				for (int i = 0; i < scene.GetRenderObjects()->GetRenderObjects().size(); i++) {
					if (ImGui::Selectable(scene.GetRenderObjects()->GetRenderObjects()[i]->GetObjectName().c_str())) {
						clickedData = i;
					}
				}
			}
		}
		if (ImGui::Button("Add Object")) {
			isCreateOpen = true;
		}
		ImGui::SetWindowPos({ 0, 20 });
		ImGui::SetWindowPos({ 0, 20 });
		ImGui::SetWindowSize({ 300,(float)window->getSize().y});
		ImGui::End();


		if (isCreateOpen) {
			ImGui::Begin("Create Object",(bool*)0,ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
			static char b[128];
			ImGui::InputText("GameObject name",b,IM_ARRAYSIZE(b));
			if (ImGui::Button("Create")) {
				std::stringstream ss;
				ss << b;
				if (ss.str() != "") {
					for (auto& a : scene.GetRenderObjects()->GetRenderObjects()) {
						if (a->GetObjectName() == ss.str()) {
							ss = std::stringstream();
							ss << b <<" (ID: " << rand() << ")";
							break;
						}
					}
					scene.GetRenderObjects()->Instantiate(new Mesh(ss.str()));
					isCreateOpen = false;
				}
				else {
					Log::GetLogger().Error("You can't instantiate this object cause object name is empty.");
				}
			}
			if (ImGui::Button("Cancel")) {
				isCreateOpen = false;
			}
			ImGui::End();
		}

		ImGui::Begin("engine_data", (bool*)0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoTitleBar);
		std::stringstream ss;
		ss << "FPS: " << GetFPS() << "\n" << "CPU Time : " << getCpuTime() << "ms";
		ImGui::SetWindowSize({ 200,200 });
		ImGui::SetWindowPos({ 300,20 });
		ImGui::Text(ss.str().c_str());
		ImGui::End();

		ImGui::Begin("Inspector", (bool*)0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
		if (scene.GetRenderObjects()->GetRenderObjects().empty() == false) {
			if (ImGui::CollapsingHeader("Object Data")) {
				float* x = &scene.GetRenderObjects()->GetRenderObjects()[clickedData]->position.x;
				float* y = &scene.GetRenderObjects()->GetRenderObjects()[clickedData]->position.y;
				float* s_X = &scene.GetRenderObjects()->GetRenderObjects()[clickedData]->scale.x;
				float* s_Y = &scene.GetRenderObjects()->GetRenderObjects()[clickedData]->scale.y;
				float* t_X = &scene.GetRenderObjects()->GetRenderObjects()[clickedData]->textureRect.x;
				float* t_Y = &scene.GetRenderObjects()->GetRenderObjects()[clickedData]->textureRect.y;
				ImGui::Text(scene.GetRenderObjects()->GetRenderObjects()[clickedData]->GetObjectName().c_str());
				ImGui::Text("Position");
				ImGui::SliderFloat("Position X", x, 0.f, window->getSize().x);
				ImGui::SliderFloat("Position Y", y, 0.f, window->getSize().y);
				ImGui::Text("Scale");
				ImGui::SliderFloat("Scale X", s_X, 0.f, 100);
				ImGui::SliderFloat("Scale Y", s_Y, 0.f, 100);
				ImGui::Text("Texture");
				ImGui::SliderFloat("Texture X", t_X, 0.f, 1920);
				ImGui::SliderFloat("Texture Y", t_Y, 0.f, 1080);
				ImGui::Button("Add Component");
				if (ImGui::Button("Delete Object")) {
					try {
						scene.GetRenderObjects()->ClearById(clickedData);
						clickedData = 0;
						Log::GetLogger().Info("Successfully removed object.");
					}
					catch (std::out_of_range& b) {
						std::cerr << "Out of range: " << b.what() << "\n";
					}
				}
				textureFileBrowser.SetTypeFilters({ ".png",".jpg" });
				textureFileBrowser.SetTitle("Select a Texture");
				textureFileBrowser.Display();

				if (textureFileBrowser.HasSelected()) {
					scene.GetRenderObjects()->GetRenderObjects()[clickedData]->LoadTexture(textureFileBrowser.GetSelected().generic_string());
					textureFileBrowser.ClearSelected();
				}
				if (ImGui::Button("Change Texture")) {
					textureFileBrowser.Open();
				}
			}
			ImGui::SetWindowPos({ (float)window->getSize().x - 300, 20 });
			ImGui::SetWindowSize({ 300,(float)window->getSize().y });
		}
		ImGui::End();
		ImGui::Begin("Console", (bool*)false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
		for (size_t i = 0; i < Log::GetLogger().logData.size(); i++) {
			std::string s = Log::GetLogger().logData[i];
			ImGui::Text(s.c_str());
		}
		ImGui::SetWindowPos({ (float)window->getPosition().x + (ImGui::GetWindowSize().x - 1025) , (float)window->getSize().y - (ImGui::GetWindowSize().y) });
		ImGui::SetWindowSize({ 1326 , 164 });
		ImGui::End();

		return true;
	}


	bool Event() {
		if (window->pollEvent(event)) {
			ImGui::SFML::ProcessEvent(event);
			if (event.type == sf::Event::Closed) {
				const int b = MessageBox(NULL, L"Do you want quit from Hydra?", L"Are you sure?", MB_ICONQUESTION | MB_YESNO);
				switch (b) {
				case IDYES:
					std::ofstream stream;
					std::stringstream fileName;
					std::time_t t = std::time(0);
					std::tm* now = std::localtime(&t);
					fileName << "./logs/" << now->tm_mday << "-" << now->tm_mon << "-" << now->tm_year << ".log";
					stream.open(fileName.str().c_str(), std::ios::out);

					for (auto b : Log::GetLogger().logData) {
						stream << b << "\n";
						std::cout << b;
					}
					Log::GetLogger().Info("Processing logs.");
					stream.close();
					window->close();
					exit(0);
				}
			}
		}
		return true;
	}

	void ApplicationUpdate() {

	}

	bool Render() 
	{
		for (auto& b : scene.GetRenderObjects()->GetRenderObjects()) {
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


	void SetFPS(double fps = 60) {
		time.SetFPS(fps);
	}

	int GetFPS() {
		return time.GetFps();
	}

	float getCpuTime() {
		return time.getCpuTime();
	}

	bool Play(bool _p = true) {
		playable = _p;
		return true;
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
	Scene scene;
	Time time;
	sf::Clock deltaClock;

	int clickedData = 0;	

	bool settings;
	bool playable = true;
	bool isCreateOpen = false;

	ImGui::FileBrowser textureFileBrowser;
	ImGui::FileBrowser sceneBrowser;
	ImGui::FileBrowser saveSceneBrowser;
};