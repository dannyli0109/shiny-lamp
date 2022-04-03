#include "Entity.h"
#include "Scene.h"

Entity::Entity()
{
	uuid.Init();
	AddComponent(std::make_shared<TagComponent>());
	AddComponent(std::make_shared<TransformComponent>(TransformComponent({ 0, 0, 0 }, { 0, 0, 0 }, { 1, 1, 1 })));
}

Entity::Entity(std::string name)
{
	uuid.Init();
	AddComponent(std::make_shared<TagComponent>(name));
	AddComponent(std::make_shared<TransformComponent>(TransformComponent({ 0, 0, 0 }, { 0, 0, 0 }, { 1, 1, 1 })));
}

Entity::Entity(std::string id, std::shared_ptr<TagComponent> tagComponent, std::shared_ptr<TransformComponent> transformComponent)
{
	uuid.Init(id);
	if (tagComponent)
	{
		AddComponent(tagComponent);
	}
	if (transformComponent)
	{
		AddComponent(transformComponent);
	}
}

void Entity::AddComponent(std::shared_ptr<Component> component)
{
	component->entity = this;
	components.push_back(component);
}

//void Entity::AddChild(std::shared_ptr<Entity> child)
//{
//	TransformComponent* transformComponent = GetComponent<TransformComponent>();
//	TransformComponent* childTransformComponent = child->GetComponent<TransformComponent>();
//
//	childTransformComponent->parent = this;
//	transformComponent->children.push_back(child);
//}

//void Entity::RemoveChild(std::shared_ptr<Entity> child)
//{
//	TransformComponent* transformComponent = GetComponent<TransformComponent>();
//	auto position = std::find(transformComponent->children.begin(), transformComponent->children.end(), child);
//	TransformComponent* childTransformComponent = child->GetComponent<TransformComponent>();
//
//	if (position != transformComponent->children.end())
//	{
//		transformComponent->children.erase(position);
//		childTransformComponent->parent = nullptr;
//	}
//}

void Entity::RemoveComponent(std::shared_ptr<Component> component)
{
	auto position = std::find(components.begin(), components.end(), component);
	if (position != components.end())
	{
		components.erase(position);
	}
}

std::vector<std::shared_ptr<Component>> Entity::GetComponents()
{
	return components;
}

std::shared_ptr<Entity> ModelConverter::Convert(std::shared_ptr<Model> model, std::shared_ptr<Scene> scene)
{
	std::shared_ptr<Entity> entity = std::make_shared<Entity>();
	scene->AddEntity(entity);
	OpenGLRenderer& renderer = OpenGLRenderer::GetSingleton();
	std::shared_ptr<MeshRendererComponent> meshRenderer = std::make_shared<MeshRendererComponent>();
	if (model->geometryIndices.size() > 0)
	{
		entity->AddComponent(meshRenderer);
	}

	for (int i = 0; i < model->geometryIndices.size(); i++)
	{
		meshRenderer->geometryIndices.push_back(model->geometryIndices[i]);
		meshRenderer->materialIndices.push_back(0);
	}

	for (int i = 0; i < model->children.size(); i++)
	{
		scene->AddChildToEntity(Convert(model->children[i], scene), entity);
	}
	return entity;
}
