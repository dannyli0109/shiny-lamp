#include "SceneHierarchy.h"

void SceneHierarchy::Update(float deltaTime, std::shared_ptr<Scene> scene)
{
	ImGui::Begin("Scene Hierarchy");

	if (scene)
	{
		for (auto entity : scene->entities)
		{
			if (entity)
			{
				TransformComponent* transform = entity->GetComponent<TransformComponent>();
				if (transform->parent.expired())
				{
					DrawNode(entity, scene);
				}
			}
		}
		DrawAddEntity(scene);
	}
	ImGui::End();
}

void SceneHierarchy::DrawNode(std::shared_ptr<Entity> entity, std::shared_ptr<Scene> scene)
{
	TransformComponent* transform = entity->GetComponent<TransformComponent>();
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
	flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
	flags |= ImGuiTreeNodeFlags_DefaultOpen;
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
	if (ImGui::BeginPopupContextItem()) // <-- use last item id as popup id
	{
		if (ImGui::Button(ImGui::GetUniqueName("Add Child", entity->uuid).c_str()))
		{
			scene->CreateChild(entity);
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Button(ImGui::GetUniqueName("Delete", entity->uuid).c_str()))
		{
			scene->RemoveEntity(entity);
			ImGui::CloseCurrentPopup();
		}
	    ImGui::EndPopup();
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

void SceneHierarchy::DrawAddEntity(std::shared_ptr<Scene> scene)
{
	ImGui::Spacing(5);
	if (ImGui::Button("Add Entity", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f)))
	{
		scene->CreateEntity();
	}
}
