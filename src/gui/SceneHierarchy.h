#pragma once
#include "components/Scene.h"
#include "GUI.h"

class SceneHierarchy
{
public:
	SceneHierarchy() = default;
	void Update(float deltaTime, std::shared_ptr<Scene> scene);
	void DrawNode(std::shared_ptr<Entity> entity, std::shared_ptr<Scene> scene);
	void DrawAddEntity(std::shared_ptr<Scene> scene);
	std::shared_ptr<Entity> selected = nullptr;
};