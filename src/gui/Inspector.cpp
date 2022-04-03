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
	//DrawMaterial(entity);
	ImGui::Separator();
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
			Material::MaterialAttribute& attribute = material->parameters[i];
				
			switch (material->parameters[i].type)
			{
			case Material::MaterialAttributeType::Texture:
			{
				if (ImGui::TreeNodeEx(attribute.name.c_str()))
				{
					OpenGLRenderer& renderer = OpenGLRenderer::GetSingleton();
					std::vector<char*> textureNames = GetResourceNames(renderer.textures);
					//std::string
					int index = renderer.textureMap[attribute.textureValue.lock()->uuid];
					ImGui::Combo(attribute.name.c_str(), &index, textureNames.data(), textureNames.size());
					attribute.textureValue = renderer.textures[index];

					float width = ImGui::GetContentRegionAvail().x;
					float aspect = attribute.textureValue.lock()->size.x / attribute.textureValue.lock()->size.y;
					ImGui::Image((void*)(attribute.textureValue.lock()->id), ImVec2(width, width / aspect));
					ImGui::TreePop();
				}
				break;
			}
			case Material::MaterialAttributeType::Float:
				break;
			case Material::MaterialAttributeType::Int:
				break;
			case Material::MaterialAttributeType::Vector2:
				break;
			case Material::MaterialAttributeType::Vector3:
				ImGui::DragFloat3(attribute.name.c_str(), &attribute.vector3Value.x);
				break;
			case Material::MaterialAttributeType::Vector4:
				break;
			case Material::MaterialAttributeType::Mat4:
				break;
			case Material::MaterialAttributeType::Color:
				ImGui::ColorEdit3(attribute.name.c_str(), &attribute.vector3Value.x);
				break;
			case Material::MaterialAttributeType::Bool:
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

void Inspector::DrawMeshRenderer(std::shared_ptr<Entity> entity)
{
	MeshRendererComponent* meshRendererComponent = entity->GetComponent<MeshRendererComponent>();
	if (meshRendererComponent)
	{
		OpenGLRenderer& renderer = OpenGLRenderer::GetSingleton();
		for (int i = 0; i < meshRendererComponent->geometryIndices.size(); i++)
		{
			if (ImGui::TreeNodeEx("Mesh"))
			{
				std::shared_ptr<MaterialInstance> material = renderer.materialInstances[meshRendererComponent->materialIndices[i]];
				DrawMaterial(material);
				ImGui::TreePop();
			}
		}
	}
}

