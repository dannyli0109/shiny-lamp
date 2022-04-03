#pragma once
#include "Matrices.h"
#include "Geometry.h"
#include "Shader.h"
#include <memory>
#include <string>
class Entity;
//class Material;
class MaterialInstance;
class Component
{
public:
	Entity* entity;
	virtual void Placeholder() {};
	//virtual void Update(float dt) = 0;
};

class TransformComponent : public Component
{
public:
	TransformComponent(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale) : translation(translation), rotation(rotation), scale(scale) {};
	glm::vec3 translation;
	glm::vec3 rotation;
	glm::vec3 scale;
	std::weak_ptr<Entity> parent;
	std::vector<std::shared_ptr<Entity>> children;
	//glm::mat4 transform;

	glm::mat4 GetTransform();
	glm::mat4 GetLocalTransform();
	glm::mat4 GetParentTransform();
};


class MeshRendererComponent : public Component
{
public:
	std::vector<int> geometryIndices;
	std::vector<int> materialIndices;
};

class TagComponent : public Component
{
public:
	std::string name;
	TagComponent() {
		name = "Entity";
	}
	TagComponent(std::string name) : name(name) {};
};

