#pragma once
#include "Include.h"



class Mesh;
class RenderObjects;
class Window;
class GuiRenderObject;
class Button;


class Discord {
public:
	Discord() {
		memset(&presence, 0, sizeof(presence));
		memset(&handler, 0, sizeof(handler));
		Discord_Initialize("971457040968990770", &handler, 1, 0);
	}

	bool Init(const char *state, const char *details) {
		presence.startTimestamp = time(NULL);
		presence.state = state;
		presence.details = details;
		presence.largeImageKey = "hydra";
		presence.smallImageKey = "hydra";
		Discord_UpdatePresence(&presence);
		return true;
	}

	void ChangeStatus(const char* state, const char* details) {
		presence.startTimestamp = time(NULL);
		presence.state = state;
		presence.details = details;
		Discord_UpdatePresence(&presence);
	}
private:
	DiscordRichPresence presence;
	DiscordEventHandlers handler;
};


class Language{
public:
	bool LanguageInit() {
		for (auto b : std::filesystem::directory_iterator()) {
			std::cout <<b << std::endl;
		}
		return true;
	}
private:
	std::map<std::string, std::string> languageData;
};

class GuiRenderer {
public:
	void Start(sf::RenderWindow* window) { ImGui::SFML::Init(*window); }
	virtual void Update() {};
};

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
		framePerSecond++;
		if (duration_cast<milliseconds>(high_resolution_clock::now() - now) >= milliseconds(1000 / fps)) {
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


		return false;
	}
	inline int GetFps() {return _fps;}
	inline bool SetFPS(long _framePerSecond) {fps = _framePerSecond;return true;}
	inline float deltaTime() {return lastCPUTime;}
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
			binary += std::bitset<8> (b).to_string();
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


	static void removeClass(std::string* string) {
		*string = std::string(string->begin() + 6, string->end());
	}
};
class Vector2 {
public:
	inline Vector2 operator+=(Vector2 temp) {this->x += temp.x;this->y += temp.y;return *this;}
	inline Vector2(float _x = 1, float _y = 1) { x = _x, y = _y; }
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
	virtual void Start() {};
	virtual void Update() {};

	Vector2* position;
	Vector2* scale;
	Vector2* textureRect;
	Time* Time;
	Log* Logger;
	Mesh* This;
	RenderObjects* SceneManager;
	GuiRenderObject* GuiManager;
};
class Mesh {
public:
	Vector2 position;
	Vector2 scale;
	Vector2 textureRect;
	RenderObjects* scene;
	Mesh(std::string gameObject, std::string textureLoc = "default_assets/square.png") {
		LoadTexture(textureLoc);
		name = gameObject;
		textureRect.x = 32;
		textureRect.y = 32;
	}
	void Initialize() {UUID = uuid::generate_uuid_v4();shape = new sf::Sprite(o_Texture);}
	std::string GetTextureDirectory() {return textureDirectory;}
	void SetScale(float _x, float _y) {scale.x = _x;scale.y = _y;}
	void DeleteComponent(int _id) {components.erase(components.begin() + _id);}
	void AddComponent(ECS* component) {
		bool error = false;
		for (auto b : components) {
			if (typeid(*b).name() == typeid(*component).name()) {
				error = true;
				Log::GetLogger().Info("You can't add same classes.");
				break;
			}
		}
		if (error == true) return;
		try {
			component->position = &position;
			component->scale = &scale;
			component->textureRect = &textureRect;
			component->Time = time;
			component->Logger = &Log::GetLogger();
			component->This = this;
			component->SceneManager = scene;
			component->GuiManager = GuiManager;
			component->Start();
			components.push_back(component);
		}
		catch (std::out_of_range& b) {
			std::cerr << "Out of range: " << b.what() << "\n";
		}
	}


	//If you gets some error, delete.
	template <typename T>
	T& HasComponent() {
		T* t = new T();
		for (auto b : components) {
			if (typeid(*b).name() == typeid(*t).name()) {
				free(t);
				return true;
			}
		}
		
	}

	template <typename T>
	T& DeleteComponent() {
		T* t = new T();
		for (int i = 0; i < components.size(); i = i+1) {
			if (typeid(*components[i]).name() == typeid(*t).name()) {
				components.erase(components.begin() + i);
				delete t;
				return *t;
			}
		}
		return *t;
	}

	//Work in progress.

	sf::Sprite* getSprite() {return shape;}
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
		//If u have a problem. just delete this.
		if (canPlayable != nullptr) {
			if (*canPlayable == true) {
				for (auto& b : components) {
					b->Update();
				}
			}
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

	bool SetName(std::string _name) {name = _name;return true;}
	std::string GetUUID() {return UUID;}
	void SetUUID(std::string _uuid) {UUID = _uuid;}
	Time* time;
	std::vector<ECS*> *GetComponents() {
		return &components;
	}
	ECS* GetComponent(int _val) {
		return components[_val];
	}
	bool* canPlayable;
	bool SetGuiRenderObj(GuiRenderObject* g_RendererObj) { GuiManager = g_RendererObj; return true; }
protected:
	sf::Sprite* shape;
	std::string name;
	sf::RenderWindow* window;
	std::vector<ECS*> components;
	sf::Texture o_Texture;
	std::string textureDirectory;
	std::string UUID;
	GuiRenderObject* GuiManager;
	bool CheckComponent(ECS* _ecs) {
		for (auto b : components) {
			if (b == _ecs) {
				return true;
			}
		}
		return false;
	}
};
class RenderObjects {
public:

	bool SetWindow(sf::RenderWindow* v_RenderWindow) { e_RenderWindow = v_RenderWindow; return true; }
	Mesh* Instantiate(Mesh* _mesh) {
		for (auto& a : GetRenderObjects()) {
			std::stringstream ss;
			ss << _mesh->GetObjectName();
			if (a->GetObjectName() == ss.str()) {
				ss = std::stringstream();
				ss << _mesh->GetObjectName() << " (ID: " << rand() << ")";
				_mesh->SetName(ss.str());
				break;
			}
		}
		_mesh->Initialize();
		_mesh->SetWindow(e_RenderWindow);
		_mesh->time = windowTime;
		_mesh->scene = this;
		_mesh->canPlayable = canPlayable;
		_mesh->SetGuiRenderObj(g_RendererObj);
		e_RenderObjects.push_back(_mesh);
		return _mesh;
	}

	std::vector<Mesh*> &GetRenderObjects() {return e_RenderObjects;}
	inline void Clear() {e_RenderObjects.clear();}
	inline void ClearById(int _id) {e_RenderObjects.erase(e_RenderObjects.begin() + _id);}


	Time* windowTime;
	bool* canPlayable;
	GuiRenderObject* g_RendererObj;
protected:
	sf::RenderWindow* e_RenderWindow;
	std::vector<Mesh*> e_RenderObjects;
};

class Script {
public:
	Script(ECS* _ecs, std::string name) {ecs = _ecs; componentName = name;}
	ECS* ecs;
	std::string componentName;
};


class BoxCollider2D : public ECS {
public:
	void Update() {
		for (int i = 0; i < SceneManager->GetRenderObjects().size(); i++) {
			for (int b = i; b < SceneManager->GetRenderObjects().size(); b = b + 1) {
				//You need to work again.
				if (SceneManager->GetRenderObjects()[i] != SceneManager->GetRenderObjects()[b]) {
					if (SceneManager->GetRenderObjects()[b]->position.x > SceneManager->GetRenderObjects()[i]->position.x + SceneManager->GetRenderObjects()[i]->scale.x
						&& SceneManager->GetRenderObjects()[b]->position.x + SceneManager->GetRenderObjects()[b]->scale.x < SceneManager->GetRenderObjects()[i]->position.x) {
						INFO("Collision detected really");
					}
				}
			}
		}
	}
};

class Button : public ECS {
public:
	void Update() {
		ImVec2 mousePos = ImGui::GetMousePos();
		if (mousePos.x > this->position->x + this->scale->x && 
			this->position->x + this->scale->x < mousePos.x &&
			mousePos.y > this->position->y + this->scale->y && 
			this->position->y + this->scale->y < mousePos.y
			){

			//Just testing bro.

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				ButtonEvent();
			}
		}
		else {
		}
	}

	void ButtonEvent() {
		This->LoadTexture("assets/15Turkey-03-articleLarge.png");
	}
private:

};


class Assembly {
public:
	bool LoadModules() {
		moduleData = LoadLibrary(L"assemblies/CppAssembly.dll");
		if (moduleData == NULL) return false;
		else {
			std::vector<ECS*> tempVector;

			std::vector<ECS*>(*ecs)() = (std::vector<ECS*>(*)())GetProcAddress(moduleData, "GetClasses");
			if (ecs == 0) return false;
			tempVector = ecs();
			for (auto* b : tempVector) {
				std::stringstream ss;
				std::string className = typeid(*b).name();
				Utils::removeClass(&className);
				ss << className;
				ss << "[0x" << b << "]";
				classes.push_back(new Script(b, ss.str()));
			}


			Button* b_Component = new Button();
			std::stringstream ss;
			std::string className = typeid(*b_Component).name();
			Utils::removeClass(&className);
			ss << className;
			ss << "[0x" << b_Component << "]";
			classes.push_back(new Script(b_Component,ss.str()));


			BoxCollider2D* b_Component2 = new BoxCollider2D();
			std::stringstream ss2;
			std::string className2 = typeid(*b_Component2).name();
			Utils::removeClass(&className2);
			ss2 << className2;
			ss2 << "[0x" << b_Component2 << "]";
			classes.push_back(new Script(b_Component2, ss2.str()));
		}
	}

	void ReloadAssemblies() {
		FreeLibrary(moduleData);
		classes.clear();
		Sleep(2000);
		LoadModules();
	}
	std::vector<Script*>& getAssemblies() {
		return classes;
	}
private:
	HMODULE moduleData;
	std::vector<Script*> classes;
};
class SystemInfo {

public:
	SystemInfo() {
		GetSystemInfo(&sysInfo);
	}

	DWORD GetCPUCount() {
		return sysInfo.dwNumberOfProcessors;
	}
private:
	SYSTEM_INFO sysInfo;
};



class Scene {
public:
	bool SaveScene() { 
		return true;
	};

	bool GenerateGUID() {
		sceneGUID = uuid::generate_uuid_v4();
		return true;
	}


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
		GenerateGUID();
		return true;
	}

	std::string GetSceneName() {
		return sceneName;
	}

	bool LoadScene(std::string path) {
		std::ifstream stream(path);
		lastPath = path;
		std::stringstream buffer;
		buffer << stream.rdbuf();
		YAML::Node sceneData = YAML::Load(buffer.str());
		

		if(!sceneData["Scene"])
			return false;
		sceneName = sceneData["Scene"].as<std::string>();
		sceneGUID = sceneData["Scene GUID"].as<std::string>();
		
		GetRenderObjects()->Clear();

		auto entities = sceneData["Entities"];
		if (entities) {
			for (auto entity : entities) {
				struct EntityData {
					std::string UUID;
					std::string name;
					std::string texturePath;
					float positionX;
					float positionY;
					float scaleX;
					float scaleY;
				};
				EntityData ent;
				ent.UUID = entity["GUID"].as<std::string>();
				ent.name = entity["Object Name"].as<std::string>();
				ent.texturePath = entity["Object Texture"].as<std::string>();
				ent.positionX = entity["Position X"].as<float>();
				ent.positionY = entity["Position Y"].as<float>();
				ent.scaleX = entity["Scale X"].as<float>();
				ent.scaleY = entity["Scale Y"].as<float>();

				Mesh* object = GetRenderObjects()->Instantiate(new Mesh(ent.name));
				object->SetUUID(ent.UUID);
				object->position.x = ent.positionX;
				object->position.y = ent.positionY;
				object->scale.x = ent.scaleX;
				object->scale.y = ent.scaleY;
				object->LoadTexture(ent.texturePath);

				auto components = entity["Components"];

				if (components) {
					std::vector<Script*> scripts = assembly->getAssemblies();
					for (auto component : components) {
						for (auto script : scripts) {
							if (component.as<std::string>() == typeid(*script->ecs).name()) {
								object->AddComponent(script->ecs);
							}
						}
					}
				}


			}

		}

		Log::GetLogger().Info("Scene loaded successfully.");
		return true;
	};


	RenderObjects* GetRenderObjects() {
		return &s_RenderObjects;
	}

	void SetAssembly(Assembly* _assembly) {
		assembly = _assembly;
	}
	std::string lastPath;
protected:

	bool SaveFile(std::string path) {
		std::ofstream ss;
		ss.open(path);
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << sceneName;
		out << YAML::Key << "Scene GUID" << YAML::Value << sceneGUID;
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		for (auto& b : s_RenderObjects.GetRenderObjects()) {
			out << YAML::BeginMap;
			out << YAML::Key << "GUID" << YAML::Value << b->GetUUID();
			out << YAML::Key << "Object Name" << YAML::Value << b->GetObjectName();
			out << YAML::Key << "Object Texture" << YAML::Value << b->GetTextureDirectory();
			out << YAML::Key << "Position X" << YAML::Value << b->position.x;
			out << YAML::Key << "Position Y" << YAML::Value << b->position.y;
			out << YAML::Key << "Scale X" << YAML::Value << b->scale.x;
			out << YAML::Key << "Scale Y" << YAML::Value << b->scale.y;

			out << YAML::Key << "Components" << YAML::BeginSeq;
			for (auto a : *b->GetComponents()) {
				out << YAML::Key << typeid(*a).name();
			}
			out << YAML::EndSeq;
			out << YAML::EndMap;
		}
		out << YAML::EndSeq;
		out << YAML::EndMap;
		ss << out.c_str();
		return true;
	}
	RenderObjects s_RenderObjects;
	std::string sceneName = "Untitled";
	std::string sceneGUID = "NULL";
	Assembly* assembly;
};

class GuiRenderObject{
public:
	void AddGui(GuiRenderer* _renderer) {
		g_Renderers.push_back(_renderer);
		_renderer->Start(window);
	}
	void RemoveGui(int _t) {g_Renderers.erase(g_Renderers.begin() + _t);}
	std::vector<GuiRenderer*>* getRenderers() { return &g_Renderers; }
	void SetRenderWindow(sf::RenderWindow* _window) { window = _window; }
private:
	std::vector<GuiRenderer*> g_Renderers;
	sf::RenderWindow* window;
};

class EditorWindow : GuiRenderer {
public:
	void Update() {
		if (sceneBrowser.HasSelected()) {
			std::thread(&Scene::LoadScene,scene, sceneBrowser.GetSelected().generic_string()).join();
			std::stringstream ss;
			ss << "Hydra Editor [" << sceneBrowser.GetSelected().generic_string() << "]";
			window->setTitle(ss.str());
			sceneBrowser.ClearSelected();
		}

		if (saveSceneBrowser.HasSelected()) {
			
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
					scene->ResetScene();
				}
				if (ImGui::MenuItem("Save Scene")) {
					saveSceneBrowser.Open();
				}
				if (ImGui::MenuItem("Load")) {
					sceneBrowser.Open();
				}
				if (ImGui::MenuItem("Play")) {
					playable = true;
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
				if (ImGui::MenuItem("Reload Scripts")) {
					assembly->ReloadAssemblies();
				}
				if (ImGui::MenuItem("Take Screenshot")) {
					std::stringstream ss;
					ss << "./screenshots/SCREENSHOT_" << std::time(NULL) << ".bmp";
					BYTE b = BMP::HDCToFile(ss.str().c_str(), GetDC(window->getSystemHandle()), {0,0,(LONG)window->getSize().x,(LONG)window->getSize().y});
					std::stringstream ss2;
					if (b) {
						ss2 << "Screenshot successfully saved in " << ss.str();
						Log::GetLogger().Info(ss2.str());
					}
					else {
						Log::GetLogger().Info("Taking screenshot has failed and we don't know why.");
					}
				}
				if (ImGui::MenuItem("Crash!")) {
					exit(EXIT_FAILURE);
				}
				if (ImGui::MenuItem("Reload Scene")) {
					if (scene->lastPath != "") {
						scene->LoadScene(scene->lastPath);
					}
					else {
						Log::GetLogger().Error("Engine cannot load the path because the path is null.");
					}
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}


		ImGui::Begin("Hierarchy", (bool*)0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
		ImGui::Text("Scene");
		if (ImGui::CollapsingHeader("GameObjects")) {
			if (!scene->GetRenderObjects()->GetRenderObjects().empty()) {
				for (int i = 0; i < scene->GetRenderObjects()->GetRenderObjects().size(); i++) {
					if (ImGui::Selectable(scene->GetRenderObjects()->GetRenderObjects()[i]->GetObjectName().c_str())) {
						clickedData = i;
						std::stringstream ss;
						ss << "Making something with : " << std::hex << "0x" << &scene->GetRenderObjects()->GetRenderObjects()[clickedData];
						discord->ChangeStatus("Exploring Engine", ss.str().c_str());
					}
				}
			}
		}
		if (ImGui::Button("Add Object")) {
			isCreateOpen = true;
		}
		ImGui::SetWindowPos({ 0, 20 });
		ImGui::SetWindowPos({ 0, 20 });
		ImGui::SetWindowSize({ 300,(float)window->getSize().y });
		ImGui::End();


		if (isCreateOpen) {
			ImGui::Begin("Create Object", (bool*)0, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
			static char b[128];
			ImGui::InputText("GameObject name", b, IM_ARRAYSIZE(b));
			if (ImGui::Button("Create")) {
				std::stringstream ss;
				ss << b;
				if (ss.str() != "") {
					scene->GetRenderObjects()->Instantiate(new Mesh(ss.str()));
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

		MEMORYSTATUSEX memex;
		memex.dwLength = sizeof(memex);
		GlobalMemoryStatusEx(&memex);

		PROCESS_MEMORY_COUNTERS_EX ex;
		GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&ex, sizeof(ex));

		ImGui::Begin("engine_data", (bool*)0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoTitleBar);
		std::stringstream ss;
		ss << "FPS: " << time->GetFps() << "\n" << "CPU Time : " << time->deltaTime() << "ms" << "\n" <<
			"Memory: " << memex.ullTotalPhys / 1024 / 1024 << "MB"
			<< "\nUsed Memory: " << ex.PrivateUsage / 1024 / 1024 << "MB"
			<< "\nProcessors: " << sysInfo.GetCPUCount();
		ImGui::SetWindowSize({ 230,200 });
		ImGui::SetWindowPos({ 300,20 });
		ImGui::Text(ss.str().c_str());
		ImGui::PlotLines("FPS Graphic", time->fpsRates, 5);
		ImGui::End();


		ImGui::Begin("Inspector", (bool*)0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
		if (scene->GetRenderObjects()->GetRenderObjects().empty() == false) {
			if (ImGui::CollapsingHeader("Object Data")) {
				float* x = &scene->GetRenderObjects()->GetRenderObjects()[clickedData]->position.x;
				float* y = &scene->GetRenderObjects()->GetRenderObjects()[clickedData]->position.y;
				float* s_X = &scene->GetRenderObjects()->GetRenderObjects()[clickedData]->scale.x;
				float* s_Y = &scene->GetRenderObjects()->GetRenderObjects()[clickedData]->scale.y;
				float* t_X = &scene->GetRenderObjects()->GetRenderObjects()[clickedData]->textureRect.x;
				float* t_Y = &scene->GetRenderObjects()->GetRenderObjects()[clickedData]->textureRect.y;
				ImGui::Text(scene->GetRenderObjects()->GetRenderObjects()[clickedData]->GetObjectName().c_str());
				if (ImGui::CollapsingHeader("Position")) {
					ImGui::SliderFloat("Position X", x, 0.f, window->getSize().x);
					ImGui::SliderFloat("Position Y", y, 0.f, window->getSize().y);
				}
				if (ImGui::CollapsingHeader("Scale")) {
					ImGui::SliderFloat("Scale X", s_X, 0.f, 100);
					ImGui::SliderFloat("Scale Y", s_Y, 0.f, 100);
				}
				if (ImGui::CollapsingHeader("Texture")) {
					ImGui::SliderFloat("Texture X", t_X, 0.f, 1920);
					ImGui::SliderFloat("Texture Y", t_Y, 0.f, 1080);
					if (ImGui::Button("Change Texture")) {
						textureFileBrowser.Open();
					}
				}

				//If gives an error, just remove this for your safety.
				if (ImGui::CollapsingHeader("Components"))
				{

					for (int i = 0; i < scene->GetRenderObjects()->GetRenderObjects()[clickedData]->GetComponents()->size(); i++) {
						ECS* data = scene->GetRenderObjects()->GetRenderObjects()[clickedData]->GetComponent(i);
						if (ImGui::TreeNode(typeid(*data).name())) {
							std::stringstream ss;
							ss << "Delete Class " << "[" << "0x" << data << "]";
							if (ImGui::Button(ss.str().c_str())) {
								scene->GetRenderObjects()->GetRenderObjects()[clickedData]->DeleteComponent(i);
							}
							ImGui::TreePop();
						}
					}
				}
				if (ImGui::CollapsingHeader("Add Component")) {
					for (int i = 0; i < assembly->getAssemblies().size(); i++) {
						std::stringstream ss;
						ss << assembly->getAssemblies()[i]->componentName.c_str();
						if (ImGui::Button(ss.str().c_str())) {
							try {
								scene->GetRenderObjects()->GetRenderObjects()[clickedData]->AddComponent(assembly->getAssemblies()[i]->ecs);
							}
							catch (std::exception& b) {
								std::cerr << "Exception: " << b.what() << "\n";
							}
						}
					}
				}

				ImGui::Text("--------------");
				//Info ends.

				if (ImGui::Button("Delete Object")) {
					try {
						scene->GetRenderObjects()->ClearById(clickedData);
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
					scene->GetRenderObjects()->GetRenderObjects()[clickedData]->LoadTexture(textureFileBrowser.GetSelected().generic_string());
					textureFileBrowser.ClearSelected();
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

	}

	bool settings;
	bool playable = false;
	bool isCreateOpen = false;
	bool isSceneCreatorOpen = false; //TODO

	ImGui::FileBrowser textureFileBrowser;
	ImGui::FileBrowser sceneBrowser;
	ImGui::FileBrowser saveSceneBrowser;
	Discord* discord;
	void SetEditorWindow(Assembly* a, Scene* s, Time* t, sf::RenderWindow* w) {
		assembly = a;
		scene = s;
		time = t;
		window = w;
	}
private:
	Assembly* assembly;
	Scene* scene;
	Time* time;
	sf::RenderWindow* window;
	SystemInfo sysInfo;
	int clickedData;

	std::thread loadSceneThread;
};

class Window {
public:
	Window() {}
	~Window() {}
	bool Init(const char* title, uint_t width, uint_t height) { window = new sf::RenderWindow(sf::VideoMode(width, height), title); return Application(); }
	bool Application(){
		g_RendererObj->SetRenderWindow(window);
		Log::GetLogger().Info("Engine trying to get ready.");
		time.SetWindow(window);
		ImGui::SFML::Init(*window);
		scene.GetRenderObjects()->SetWindow(window);
		Log::GetLogger().Info("Ready!");
		scene.GetRenderObjects()->windowTime = &time;
		scene.GetRenderObjects()->g_RendererObj = g_RendererObj;
		std:async(std::launch::async, &Window::Start, this);
		assembly.LoadModules();
		scene.SetAssembly(&assembly);
		scene.GetRenderObjects()->canPlayable = &e_Window->playable;
		e_Window->SetEditorWindow(&assembly, &scene, &time, window);
		e_Window->discord = &discordRPC;
		SetupStyleImGui();
		discordRPC.Init("Exploring Engine", "Hello, World!");
		while (window->isOpen()) {
			do {
				Window::Update();
				Window::ApplicationUpdate();
				window->clear();
				window->pushGLStates();
				//Start
				Window::Render();
				Window::ImGuiRenderer();
				window->popGLStates();
				ImGui::SFML::Render();
				window->display();
				Event();
			} while (time.Update());
		}
		ImGui::SFML::Shutdown();
		return false;
	}




	bool ImGuiRenderer() {
		ImGui::SFML::Update(*window, deltaClock.restart());
		if(!e_Window->playable)e_Window->Update();
		for (auto b : *g_RendererObj->getRenderers()) {
			b->Update();
		}
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
					fileName << "./logs/" << now->tm_mday << "-" << now->tm_mon << "-" << now->tm_year << std::time(0) << ".log";
					stream.open(fileName.str().c_str(), std::ios::out);

					for (auto b : Log::GetLogger().logData) {
						stream << b << "\n";
					}
					Log::GetLogger().Info("Processing logs.");
					stream.close();
					window->close();
				}
			}
		}
		return true;
	}
	void Render() 
	{
		for (auto& b : scene.GetRenderObjects()->GetRenderObjects()) {
			b->Render();
		}
	}
	virtual void Start() {};
	virtual void Update() {};
	void ApplicationUpdate() {}
	bool ApplicationStart() {return true;}
	bool MainThread() {Update();return true;}
	void SetFPS(double fps = 60) {time.SetFPS(fps);}
	float getCpuTime() {return time.deltaTime();}
	bool Play(bool _p = true) {e_Window->playable = _p;return true;}
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
	Assembly assembly;
	sf::Clock deltaClock;
	Discord discordRPC;
	EditorWindow* e_Window = new EditorWindow();
	GuiRenderObject* g_RendererObj = new GuiRenderObject();
	SystemInfo sysInfo;


	std::vector<std::thread> threads;
};