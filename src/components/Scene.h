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
	void CreateChild(std::shared_ptr<Entity> entity);
	void CreateEntity();

	void AddEntity(std::shared_ptr<Entity> entity);
	void AddChildToEntity(std::shared_ptr<Entity> child, std::shared_ptr<Entity> entity);
	std::vector<std::shared_ptr<Entity>>& GetChildrenForEntity(std::shared_ptr<Entity> entity);
	std::shared_ptr<Entity> GetParentForEntity(std::shared_ptr<Entity> entity);
	void RemoveChildFromEntity(std::shared_ptr<Entity> child, std::shared_ptr<Entity> entity);
	void RemoveEntity(std::shared_ptr<Entity> entity);
	std::shared_ptr<Entity> GetEntity(std::string uuid);
	void Update(float dt);
	void CleanUp();
	std::vector<std::shared_ptr<Entity>> entities;
	std::vector<std::shared_ptr<Entity>> expired;
	std::vector<std::shared_ptr<Entity>> created;

	std::string name;
	void RemoveEntityInternal(std::shared_ptr<Entity> entity);
	void AddEntityInternal(std::shared_ptr<Entity> entity);
private:
};