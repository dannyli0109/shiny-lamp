#include "SceneHierarchy.h"

void SceneHierarchy::Update(float deltaTime, std::shared_ptr<Scene> scene)
{
	ImGui::Begin("Scene Hierarchy");

	if (scene)
	{
		for (auto [uuid, entity] : scene->entities)
		{
			TransformComponent* transform = entity->GetComponent<TransformComponent>();
			if (transform->parent.expired())
			{
				DrawNode(entity, scene);
			}
		}
	}
	ImGui::End();
}

void SceneHierarchy::DrawNode(std::shared_ptr<Entity> entity, std::shared_ptr<Scene> scene)
{
	TransformComponent* transform = entity->GetComponent<TransformComponent>();
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
	flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
	if (entity == selected)
	{
		flags |= ImGuiTreeNodeFlags_Selected;
	}

	if (transform->children.size() == 0)
	{
		flags |= ImGuiTreeNodeFlags_Leaf;
	}
	
	bool tree = ImGui::TreeNodeEx(std::string(entity->uuid).c_str(), flags, entity->GetComponent<TagComponent>()->name.c_str());
	if (ImGui::IsItemClicked())
	{
		selected = entity;
	}

	if (tree)
	{
		for (int i = 0; i < transform->children.size(); i++)
		{
			DrawNode(transform->children[i], scene);
		}
		ImGui::TreePop();
	}
}