#pragma once
#include "Entity.h"
#include "Components.h"
#include "../OpenGLRenderer.h"
#include "Renderer.h"
#include <string>
#include <map>

class Scene
{
public:
	Scene(std::string name) : name(name) {};
	Scene() : name("UntitledScene") {};
	void AddEntity(std::shared_ptr<Entity> entity);
	void AddChildToEntity(std::shared_ptr<Entity> child, std::shared_ptr<Entity> entity);
	void RemoveChildFromEntity(std::shared_ptr<Entity> child, std::shared_ptr<Entity> entity);
	std::shared_ptr<Entity> GetEntity(std::string uuid);
	void Update(float dt);
	std::map<std::string, std::shared_ptr<Entity>> entities;
	std::string name;
private:
};