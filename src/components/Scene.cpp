#include "Scene.h"

void Scene::AddEntity(std::shared_ptr<Entity> entity)
{
	entities.emplace(entity->uuid, entity);
}

void Scene::AddChildToEntity(std::shared_ptr<Entity> child, std::shared_ptr<Entity> entity)
{
	TransformComponent* transformComponent = entity->GetComponent<TransformComponent>();
	TransformComponent* childTransformComponent = child->GetComponent<TransformComponent>();

	childTransformComponent->parent = entity;
	transformComponent->children.push_back(child);
}

void Scene::RemoveChildFromEntity(std::shared_ptr<Entity> child, std::shared_ptr<Entity> entity)
{
	TransformComponent* transformComponent = entity->GetComponent<TransformComponent>();
	auto position = std::find(transformComponent->children.begin(), transformComponent->children.end(), child);
	TransformComponent* childTransformComponent = child->GetComponent<TransformComponent>();

	if (position != transformComponent->children.end())
	{
		transformComponent->children.erase(position);
		childTransformComponent->parent.reset();
	}
}

std::shared_ptr<Entity> Scene::GetEntity(std::string uuid)
{
	if (entities.find(uuid) != entities.end())
	{
		return entities[uuid];
	}
	return nullptr;
}

void Scene::Update(float dt)
{
	OpenGLRenderer& renderer = OpenGLRenderer::GetSingleton();
	glm::mat4 view = renderer.GetViewMatrix();
	glm::mat4 projection = renderer.GetProjectionMatrix();
	glm::vec3 cameraPos = renderer.GetCameraPosition();
	for (auto& shader : renderer.shaders)
	{
		shader->Use();
		renderer.SetUniform(shader, "u_viewMatrix", view, 1);
		renderer.SetUniform(shader, "u_projectionMatrix", projection, 1);
		renderer.SetUniform(shader, "u_cameraPos", cameraPos, 1);
	}
	for (auto [uuid, entity] : entities)
	{
		// update transform
		// render
		TransformComponent* transformComponent = entity->GetComponent<TransformComponent>();
		MeshRendererComponent* meshRendererComponent = entity->GetComponent<MeshRendererComponent>();
		//MaterialComponent* materialComponent = entities[i]->GetComponent<MaterialComponent>();


		if (transformComponent && meshRendererComponent)
		{
			glm::mat4 transform = transformComponent->GetTransform();

			//renderer.SetUniform(materialComponent->material->shader.lock(), "u_modelMatrix", transform, 1);

			//materialComponent->material->Bind();

			for (int i = 0; i < meshRendererComponent->geometryIndices.size(); i++)
			{
				std::shared_ptr<MaterialInstance> material = renderer.materialInstances[meshRendererComponent->materialIndices[i]];
				material->shader.lock()->Use();
				renderer.SetUniform(material->shader.lock(), "u_modelMatrix", transform, 1);
				material->Bind();
				renderer.geometries[meshRendererComponent->geometryIndices[i]]->Draw();
			}
			renderer.UnbindTexture();
		}
	}
}
