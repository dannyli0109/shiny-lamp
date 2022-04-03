#pragma once
#include "Components.h"
#include <string>
#include <vector>
#include <memory>
#include "../Model.h"
#include "UUID.h"
#include "SceneSerialiser.h"
#include "OpenGLRenderer.h"

class Scene;

class Entity
{
public:
	Entity();
	Entity(std::string name);
	Entity(std::string id, std::shared_ptr<TagComponent> tagComponent, std::shared_ptr<TransformComponent> transformComponent);
	void AddComponent(std::shared_ptr<Component> component);
	//void AddChild(std::shared_ptr<Entity> child);
	//void RemoveChild(std::shared_ptr<Entity> child);
	void RemoveComponent(std::shared_ptr<Component> component);
	std::vector<std::shared_ptr<Component>> GetComponents();
	template<typename T>
	T* GetComponent()
	{
		for (auto component : components)
		{
			if (dynamic_cast<T*>(component.get()) != nullptr)
			{
				return dynamic_cast<T*>(component.get());
			}
		}
		return nullptr;
	}
	//Entity* parent;
	Util::UUID uuid;
private:
	std::vector<std::shared_ptr<Component>> components;
};

class ModelConverter
{
public:
	std::shared_ptr<Entity> Convert(std::shared_ptr<Model> model, std::shared_ptr<Scene> scene);
};