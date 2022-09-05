#define _CRT_SECURE_NO_WARNINGS
#include <random>
#include <SFML/Graphics.hpp>
#include <discord_register.h>
#include <discord_rpc.h>
#include <yaml-cpp/yaml.h>
#include <iostream>
#include <bitset>
#include <future>
#include <vector>
#include <chrono>
#include <algorithm>
#include <sstream>
#define SFML_STATIC
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui-SFML.h>
#include <imfilebrowser.h>
#include <Windows.h>
#include <stdexcept>
#include <fstream>
#include "UUID.h"
#include <Psapi.h>
//Define starts
#define INFO(a) Logger->Info(a)
#define WARNING(a) Logger->Warning(a)
#define ERROR(a) Logger->Error(a)
#define SPAWN(a) SceneManager->Instantiate(a)
#define Monobehaviour public ECS
#define IsKeyPressed sf::Keyboard::isKeyPressed
#define KeyCode sf::Keyboard
//Define ends
typedef unsigned int uint_t;
using namespace std::chrono;
