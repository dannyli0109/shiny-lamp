#include "Scene.h"
#include "ResourceManager.h"

void Scene::CreateChild(std::shared_ptr<Entity> entity)
{
	std::shared_ptr<Entity> child = std::make_shared<Entity>();
	AddChildToEntity(child, entity);
	created.push_back(child);
}

void Scene::CreateEntity()
{
	std::shared_ptr<Entity> entity = std::make_shared<Entity>();
	created.push_back(entity);
}

void Scene::AddEntity(std::shared_ptr<Entity> entity)
{
	created.push_back(entity);
}

void Scene::AddChildToEntity(std::shared_ptr<Entity> child, std::shared_ptr<Entity> entity)
{
	TransformComponent* transformComponent = entity->GetComponent<TransformComponent>();
	TransformComponent* childTransformComponent = child->GetComponent<TransformComponent>();

	childTransformComponent->parent = entity;
	transformComponent->children.push_back(child);
}

std::vector<std::shared_ptr<Entity>>& Scene::GetChildrenForEntity(std::shared_ptr<Entity> entity)
{
	TransformComponent* transformComponent = entity->GetComponent<TransformComponent>();
	return transformComponent->children;
}

std::shared_ptr<Entity> Scene::GetParentForEntity(std::shared_ptr<Entity> entity)
{
	TransformComponent* transformComponent = entity->GetComponent<TransformComponent>();
	return transformComponent->parent.lock();
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

void Scene::RemoveEntity(std::shared_ptr<Entity> entity)
{
	expired.push_back(entity);
}

std::shared_ptr<Entity> Scene::GetEntity(std::string uuid)
{
	auto it = std::find_if(entities.begin(), entities.end(), [&](const std::shared_ptr<Entity> entity)
		{
			return std::string(entity->uuid) == uuid;
		}
	);
	if (it == entities.end()) return nullptr;
	return entities[std::distance(entities.begin(), it)];
}

void Scene::Update(float dt)
{
	OpenGLRenderer& renderer = OpenGLRenderer::GetSingleton();
	ResourceManager& resources = ResourceManager::GetSingleton();
	glm::mat4 view = renderer.GetViewMatrix();
	glm::mat4 projection = renderer.GetProjectionMatrix();
	glm::vec3 cameraPos = renderer.GetCameraPosition();
	for (auto& shader : resources.shaders)
	{
		shader->Use();
		renderer.SetUniform(shader, "u_viewMatrix", view, 1);
		renderer.SetUniform(shader, "u_projectionMatrix", projection, 1);
		renderer.SetUniform(shader, "u_cameraPos", cameraPos, 1);
	}
	for (auto entity : entities)
	{
		if (!entity) continue;
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

			for (int i = 0; i < meshRendererComponent->meshes.size(); i++)
			{
				std::shared_ptr<MaterialInstance> material = meshRendererComponent->materials[i].lock();
				material->shader.lock()->Use();
				renderer.SetUniform(material->shader.lock(), "u_modelMatrix", transform, 1);
				material->Bind();
				meshRendererComponent->meshes[i].lock()->Draw();
			}
			renderer.UnbindTexture();
		}
	}
}

void Scene::CleanUp()
{
	for (auto e : expired)
	{
		RemoveEntityInternal(e);
	}
	expired.clear();


	for (auto e : created)
	{
		AddEntityInternal(e);
	}
	created.clear();
}

void Scene::RemoveEntityInternal(std::shared_ptr<Entity> entity)
{
	std::shared_ptr<Entity> parent = GetParentForEntity(entity);
	if (parent)
	{
		RemoveChildFromEntity(entity, parent);
	}
	std::vector<std::shared_ptr<Entity>> children = GetChildrenForEntity(entity);
	for (auto child : children)
	{
		RemoveEntityInternal(child);
	}
	children.clear();
	auto it = std::find(entities.begin(), entities.end(), entity);
	if (it != entities.end())
	{
		entities.erase(it);
	}
	entity.reset();
}

void Scene::AddEntityInternal(std::shared_ptr<Entity> entity)
{
	entities.push_back(entity);
}
