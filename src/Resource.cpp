#include "Resource.h"
#include "OpenGLRenderer.h"

#include "Shader.h"

void Material::Init(std::shared_ptr<Shader> shader)
{
	Init("New Material", shader);
}

void Material::Init(std::string name, std::shared_ptr<Shader> shader)
{
	this->name = name;
	this->shader = shader;
}

int Material::Find(std::string name)
{
	for (int i = 0; i < parameters.size(); i++)
	{
		if (parameters[i].name == name) return i;
	}
	return -1;
}

void Material::Set(MaterialAttribute attribute)
{
	int index = Find(attribute.name);
	if (index > -1)
	{
		parameters[index] = attribute;
	}
	else
	{
		parameters.push_back(attribute);
	}
}

void Material::Set(std::string name, std::shared_ptr<Texture> textureValue)
{
	int index = Find(name);
	if (index == -1)
	{
		MaterialAttribute attribute = {};
		attribute.name = name;
		attribute.type = MaterialAttributeType::Texture;
		attribute.textureValue = textureValue;
		parameters.push_back(attribute);
	}
	else
	{
		parameters[index].type = MaterialAttributeType::Texture;
		parameters[index].textureValue = textureValue;
	}
}

void Material::Set(std::string name, float floatValue)
{
	int index = Find(name);
	if (index == -1)
	{
		MaterialAttribute attribute = {};
		attribute.name = name;
		attribute.type = MaterialAttributeType::Float;
		attribute.floatValue = floatValue;
		parameters.push_back(attribute);
	}
	else
	{
		parameters[index].type = MaterialAttributeType::Float;
		parameters[index].floatValue = floatValue;
	}
}

void Material::Set(std::string name, int intValue)
{
	int index = Find(name);
	if (index == -1)
	{
		MaterialAttribute attribute = {};
		attribute.name = name;
		attribute.type = MaterialAttributeType::Int;
		attribute.intValue = intValue;
		parameters.push_back(attribute);
	}
	else
	{
		parameters[index].type = MaterialAttributeType::Int;
		parameters[index].intValue = intValue;
	}
}

void Material::Set(std::string name, glm::vec2 vector2Value)
{
	int index = Find(name);
	if (index == -1)
	{
		MaterialAttribute attribute = {};
		attribute.name = name;
		attribute.type = MaterialAttributeType::Vector2;
		attribute.vector2Value = vector2Value;
		parameters.push_back(attribute);
	}
	else
	{
		parameters[index].type = MaterialAttributeType::Vector2;
		parameters[index].vector2Value = vector2Value;
	}
}

void Material::Set(std::string name, glm::vec3 vector3Value)
{
	int index = Find(name);
	if (index == -1)
	{
		MaterialAttribute attribute = {};
		attribute.name = name;
		attribute.type = MaterialAttributeType::Vector3;
		attribute.vector3Value = vector3Value;
		parameters.push_back(attribute);
	}
	else
	{
		parameters[index].type = MaterialAttributeType::Vector3;
		parameters[index].vector3Value = vector3Value;
	}
}

void Material::Set(std::string name, glm::vec4 vector4Value)
{
	int index = Find(name);
	if (index == -1)
	{
		MaterialAttribute attribute = {};
		attribute.name = name;
		attribute.type = MaterialAttributeType::Vector4;
		attribute.vector4Value = vector4Value;
		parameters.push_back(attribute);
	}
	else
	{
		parameters[index].type = MaterialAttributeType::Vector4;
		parameters[index].vector4Value = vector4Value;
	}
}

void Material::Set(std::string name, glm::mat4 mat4Value)
{
	int index = Find(name);
	if (index == -1)
	{
		MaterialAttribute attribute = {};
		attribute.name = name;
		attribute.type = MaterialAttributeType::Mat4;
		attribute.mat4Value = mat4Value;
		parameters.push_back(attribute);
	}
	else
	{
		parameters[index].type = MaterialAttributeType::Mat4;
		parameters[index].mat4Value = mat4Value;
	}
}

void Material::Bind()
{
	OpenGLRenderer& renderer = OpenGLRenderer::GetSingleton();
	shader.lock()->Use();
	int textureIndex = 1;
	for (int i = 0; i < parameters.size(); i++)
	{
		std::string name = parameters[i].name;
		switch (parameters[i].type)
		{
		case MaterialAttributeType::Texture:
		{
			//shader.lock()->SetUniform(name, textureIndex);
			renderer.BindTexture(shader.lock(), name, parameters[i].textureValue.lock(), textureIndex);
			textureIndex++;
			break;
		}
		case MaterialAttributeType::Float:
			shader.lock()->SetUniform(name, parameters[i].floatValue);
			break;
		case MaterialAttributeType::Int:
		case MaterialAttributeType::Bool:
			shader.lock()->SetUniform(name, parameters[i].intValue);
			break;
		case MaterialAttributeType::Vector2:
			shader.lock()->SetUniform(name, parameters[i].vector2Value, 1);
			break;
		case MaterialAttributeType::Vector3:
		case MaterialAttributeType::Color:
			shader.lock()->SetUniform(name, parameters[i].vector3Value, 1);
			break;
		case MaterialAttributeType::Vector4:
			shader.lock()->SetUniform(name, parameters[i].vector4Value, 1);
			break;
		case MaterialAttributeType::Mat4:
			shader.lock()->SetUniform(name, parameters[i].mat4Value, 1);
			break;
		default:
			break;
		}
	}
}

MaterialInstance::MaterialInstance(std::shared_ptr<Material> material, std::string name, bool modifiable)
{
	this->parameters = material->parameters;
	this->name = name;
	shader = material->shader;
	this->modifiable = modifiable;
	base = material;
};

std::string MaterialAttribute::GetNameFromType(MaterialAttributeType t)
{
	for (auto& [name, type] : attributePairs)
	{
		if (type == t) return name;
	}
	return "None";
}

MaterialAttributeType MaterialAttribute::GetTypeFromName(std::string s)
{
	for (auto& [name, type] : attributePairs)
	{
		if (name == s) return type;
	}
	return MaterialAttributeType::Texture;
}

std::vector<std::pair<std::string, MaterialAttributeType>> MaterialAttribute::attributePairs =
{
	{ "Texture", MaterialAttributeType::Texture },
	{ "Float", MaterialAttributeType::Float },
	{ "Int", MaterialAttributeType::Int },
	{ "Vector2", MaterialAttributeType::Vector2 },
	{ "Vector3", MaterialAttributeType::Vector3 },
	{ "Vector4", MaterialAttributeType::Vector4 },
	{ "Mat4", MaterialAttributeType::Mat4 },
	{ "Color", MaterialAttributeType::Color },
	{ "Bool", MaterialAttributeType::Bool },
	{ "None", MaterialAttributeType::None }
};
