#pragma once

#include "Graphics.h"
#include "GUI.h"
#include "Shader.h"
#include "Util.h"
#include "OpenGLRenderer.h"
#include "InputManager.h"
#include <string>
#include <memory>
#include "components/Entity.h"
#include "components/Scene.h"
#include "components/SceneSerialiser.h"
#include "gui/SceneHierarchy.h"
#include "gui/Inspector.h"

class Program
{
public:
	Program() = default;
	bool Init(int width, int height, std::string title);
	void Update();
	void End();
private:
	bool InitWindow(int width, int height, std::string title);
	bool InitGUI();
	void BeginUpdate();
	void UpdateGUI();
	void Draw();
	void EndUpdate();
	void SetGUITheme();
private:
	float w, h;
	float dpiScaleX, dpiScaleY;
	GLFWwindow* window;
	float dt;
	double previousTime;
	//std::shared_ptr<Entity> entity;
	std::shared_ptr<Scene> scene;
	std::shared_ptr<SceneHierarchy> sceneHierarchy;
	std::shared_ptr<Inspector> inspector;

};