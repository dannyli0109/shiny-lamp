#include "Inspector.h"

static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
{
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];

	ImGui::PushID(label.c_str());

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

	float lineHeight = GImGui->Font->FontSize * io.FontGlobalScale + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("X", buttonSize))
		values.x = resetValue;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("Y", buttonSize))
		values.y = resetValue;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("Z", buttonSize))
		values.z = resetValue;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();

	ImGui::PopStyleVar();

	ImGui::Columns(1);

	ImGui::PopID();
}


void Inspector::Update(float deltaTime, std::shared_ptr<Entity> entity)
{
	ImGui::Begin("Inspector");
	DrawTag(entity);
	ImGui::Separator();
	DrawId(entity);
	ImGui::Separator();
	DrawTransform(entity);
	ImGui::Separator();
	DrawMeshRenderer(entity);
	ImGui::Separator();
	DrawAddComponent(entity);
	ImGui::End();
}

void Inspector::DrawId(std::shared_ptr<Entity> entity)
{
	ImGui::Text(std::string(entity->uuid).c_str());
}

void Inspector::DrawTag(std::shared_ptr<Entity> entity)
{
	TagComponent* tagComponent = entity->GetComponent<TagComponent>();
	if (tagComponent)
	{
		char* str = tagComponent->name.data();
		ImGui::Text("Tag");
		ImGui::SameLine();
		ImGui::PushItemWidth(-1);
		ImGui::InputText("##label", str, 128);
		ImGui::PopItemWidth();
		tagComponent->name = str;
	}
}

void Inspector::DrawTransform(std::shared_ptr<Entity> entity)
{
	TransformComponent* transformComponent = entity->GetComponent<TransformComponent>();

	if (transformComponent)
	{
		if (ImGui::TreeNodeEx("Transform Component"))
		{
			float width = ImGui::GetContentRegionAvail().x / 3.0f;
			DrawVec3Control("Translation", transformComponent->translation, 0.0f, width);
			DrawVec3Control("Rotation", transformComponent->rotation, 0.0f, width);
			DrawVec3Control("Scale", transformComponent->scale, 1.0f, width);
			ImGui::TreePop();
		}
	}
}

void Inspector::DrawMaterial(std::shared_ptr<MaterialInstance> material)
{
	if (ImGui::TreeNodeEx("Material"))
	{
		if (!material->modifiable) ImGui::BeginDisabled();
		for (int i = 0; i < material->parameters.size(); i++)
		{
			MaterialAttribute& attribute = material->parameters[i];
				
			switch (material->parameters[i].type)
			{
			case MaterialAttributeType::Texture:
			{
				if (ImGui::TreeNodeEx(attribute.name.c_str()))
				{
					//OpenGLRenderer& renderer = OpenGLRenderer::GetSingleton();
					ResourceManager& resources = ResourceManager::GetSingleton();

					std::vector<char*> textureNames = GetResourceNames(resources.textures);
					//std::string
					//int index = renderer.textureMap[attribute.textureValue.lock()->uuid];
					int index = resources.GetTextureIndex(attribute.textureValue.lock()->uuid);

					ImGui::Combo(attribute.name.c_str(), &index, textureNames.data(), textureNames.size());
					attribute.textureValue = resources.textures[index];

					float width = ImGui::GetContentRegionAvail().x;
					float aspect = attribute.textureValue.lock()->size.x / attribute.textureValue.lock()->size.y;
					ImGui::Image((void*)(attribute.textureValue.lock()->id), ImVec2(width, width / aspect));
					ImGui::TreePop();
				}
				break;
			}
			case MaterialAttributeType::Float:
				break;
			case MaterialAttributeType::Int:
				break;
			case MaterialAttributeType::Vector2:
				break;
			case MaterialAttributeType::Vector3:
				ImGui::DragFloat3(attribute.name.c_str(), &attribute.vector3Value.x);
				break;
			case MaterialAttributeType::Vector4:
				break;
			case MaterialAttributeType::Mat4:
				break;
			case MaterialAttributeType::Color:
				ImGui::ColorEdit3(attribute.name.c_str(), &attribute.vector3Value.x);
				break;
			case MaterialAttributeType::Bool:
			{
				bool b = attribute.intValue;
				ImGui::Checkbox(attribute.name.c_str(), &b);
				attribute.intValue = b;
				break;
			}
			default:
				break;
			}
					
		}
		if (!material->modifiable) ImGui::EndDisabled();
		ImGui::TreePop();
	}
}

void Inspector::DrawMesh(std::shared_ptr<Mesh> mesh)
{
	if (ImGui::TreeNodeEx("Mesh"))
	{
		//OpenGLRenderer& renderer = OpenGLRenderer::GetSingleton();
		ResourceManager& resources = ResourceManager::GetSingleton();

		std::vector<char*> meshNames = GetResourceNames(resources.meshes);
		int index = resources.GetMeshIndex(mesh->uuid);
		ImGui::Combo(mesh->name.c_str(), &index, meshNames.data(), meshNames.size());
		mesh = resources.meshes[index];
		ImGui::TreePop();
	}
}

void Inspector::DrawMeshRenderer(std::shared_ptr<Entity> entity)
{
	MeshRendererComponent* meshRendererComponent = entity->GetComponent<MeshRendererComponent>();
	if (meshRendererComponent)
	{
		if (ImGui::TreeNodeEx("MeshRenderer Component"))
		{
			//OpenGLRenderer& renderer = OpenGLRenderer::GetSingleton();
			ResourceManager& resources = ResourceManager::GetSingleton();
			for (int i = 0; i < meshRendererComponent->meshes.size(); i++)
			{
				std::string str = ImGui::GetUniqueName("Mesh", std::to_string(i));
				if (ImGui::TreeNodeEx(str.c_str()))
				{
					std::shared_ptr<Mesh> mesh = meshRendererComponent->meshes[i].lock();
					DrawMesh(mesh);

					std::shared_ptr<MaterialInstance> material = meshRendererComponent->materials[i].lock();
					DrawMaterial(material);

					ImGui::TreePop();
				}
			}

			std::string str = ImGui::GetUniqueName("Add Mesh", entity->uuid);
			if (ImGui::Button(str.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0.0f)))
			{
				ImGui::OpenPopup(str.c_str());
			}

			if (ImGui::BeginPopup(str.c_str())) 
			{
				for (int i = 0; i < resources.meshes.size(); i++)
				{
					if (ImGui::Selectable(ImGui::GetUniqueName(resources.meshes[i]->name.c_str(), resources.meshes[i]->uuid).c_str()))
					{
						meshRendererComponent->meshes.push_back(resources.meshes[i]);

						meshRendererComponent->materials.push_back(resources.materialInstances[0]);
						ImGui::CloseCurrentPopup();
					}
				}
				ImGui::EndPopup();
			}
			ImGui::TreePop();
		}
	}
}

void Inspector::DrawAddComponent(std::shared_ptr<Entity> entity)
{
	std::string str = ImGui::GetUniqueName("Add Component", entity->uuid);
	if (ImGui::Button(str.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0.0f)))
	{
		ImGui::OpenPopup(str.c_str());
	}

	if (ImGui::BeginPopup(str.c_str())) // <-- use last item id as popup id
	{
		DrawAddMeshRendererComponent(entity);
		ImGui::EndPopup();
	}
}

void Inspector::DrawAddMeshRendererComponent(std::shared_ptr<Entity> entity)
{
	MeshRendererComponent* meshRendererComponent = entity->GetComponent<MeshRendererComponent>();
	if (!meshRendererComponent)
	{
		if (ImGui::Selectable(ImGui::GetUniqueName("Mesh Renderer", entity->uuid).c_str()))
		{
			std::shared_ptr<MeshRendererComponent> component = std::make_shared<MeshRendererComponent>();
			entity->AddComponent(component);
			ImGui::CloseCurrentPopup();
		}
	}
}

