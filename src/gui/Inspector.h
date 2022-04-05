#pragma once
#include "components/Entity.h"
#include "components/Components.h";
//#include "OpenGLRenderer.h";
#include "GUI.h"
#include "imgui_internal.h"
#include "Resource.h"
#include "ResourceManager.h"

class Inspector
{
public:
	Inspector() = default;

	void Update(float deltaTime, std::shared_ptr<Entity> entity);
	void DrawId(std::shared_ptr<Entity> entity);
	void DrawTag(std::shared_ptr<Entity> entity);
	void DrawTransform(std::shared_ptr<Entity> entity);
	void DrawMaterial(std::shared_ptr<MaterialInstance> material);
	void DrawMesh(std::shared_ptr<Mesh> mesh);
	void DrawMeshRenderer(std::shared_ptr<Entity> entity);

	void DrawAddComponent(std::shared_ptr<Entity> entity);
	void DrawAddMeshRendererComponent(std::shared_ptr<Entity> entity);
	template <typename T>
	std::vector<char*> GetResourceNames(std::vector<std::shared_ptr<T>>& resources)
	{
		std::vector<char*> output(resources.size());
		for (int i = 0; i < resources.size(); i++)
		{
			output[i] = resources[i]->name.data();
		}
		return output;
	}
};