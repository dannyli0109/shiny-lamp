#include "Components.h"
#include "Entity.h"

glm::mat4 TransformComponent::GetTransform()
{
    glm::mat4 parentTransform = GetParentTransform();
    glm::mat4 localTransform = GetLocalTransform();

    return parentTransform * localTransform;
}

glm::mat4 TransformComponent::GetLocalTransform()
{
	glm::mat4 rotationMat = glm::toMat4(glm::quat(rotation));
	glm::mat4 localTransform =
		glm::translate(glm::mat4(1.0f), translation)
		* rotationMat
		* glm::scale(glm::mat4(1.0f), scale);

	return localTransform;
}

glm::mat4 TransformComponent::GetParentTransform()
{
	glm::mat4 rotationMat = glm::toMat4(glm::quat(rotation));
	glm::mat4 parentTransform = glm::mat4(1.0f);

	if (!parent.expired())
	{
		TransformComponent* parentTransformComponent = parent.lock()->GetComponent<TransformComponent>();
		parentTransform = parentTransformComponent->GetTransform();
	}
	return parentTransform;
}
