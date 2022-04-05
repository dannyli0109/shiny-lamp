#pragma once
#include "UUID.h"
#include "Graphics.h"
class Shader;
class MeshData;
class Mesh;

struct Resource
{
public:
	std::string name;
	Util::UUID uuid;
};


struct Texture : public Resource
{
	std::string filepath;
	GLuint id;
	glm::vec2 size;
};

struct ModelData
{
	std::vector<std::shared_ptr<MeshData>> meshes;
	std::vector<std::shared_ptr<ModelData>> children;
};

struct Model : Resource
{
	std::vector<std::shared_ptr<Mesh>> meshes;
	std::vector<std::shared_ptr<Model>> children;
};

enum class MaterialAttributeType
{
	Texture,
	Float,
	Int,
	Vector2,
	Vector3,
	Vector4,
	Mat4,
	Color,
	Bool,

	None
};

struct MaterialAttribute
{
	static std::string GetNameFromType(MaterialAttributeType type);
	static MaterialAttributeType GetTypeFromName(std::string);
	static std::vector<std::pair<std::string, MaterialAttributeType>> attributePairs;

	MaterialAttribute() = default;
	MaterialAttribute(std::string name, float floatValue)
	{
		this->name = name;
		this->floatValue = floatValue;
		this->type = MaterialAttributeType::Float;
	}

	MaterialAttribute(std::string name, int intValue)
	{
		this->name = name;
		this->intValue = intValue;
		this->type = MaterialAttributeType::Int;
	}

	MaterialAttribute(std::string name, bool intValue)
	{
		this->name = name;
		this->intValue = intValue;
		this->type = MaterialAttributeType::Bool;
	}

	MaterialAttribute(std::string name, glm::vec2 vector2Value)
	{
		this->name = name;
		this->vector2Value = vector2Value;
		this->type = MaterialAttributeType::Vector2;
	}

	MaterialAttribute(std::string name, glm::vec3 vector3Value)
	{
		this->name = name;
		this->vector3Value = vector3Value;
		this->type = MaterialAttributeType::Vector3;
	}

	MaterialAttribute(std::string name, glm::vec3 vector3Value, MaterialAttributeType type)
	{
		this->name = name;
		this->vector3Value = vector3Value;
		this->type = type;
	}

	MaterialAttribute(std::string name, glm::vec4 vector4Value)
	{
		this->name = name;
		this->vector4Value = vector4Value;
		this->type = MaterialAttributeType::Vector4;
	}

	MaterialAttribute(std::string name, glm::mat4 mat4Value)
	{
		this->name = name;
		this->mat4Value = mat4Value;
		this->type = MaterialAttributeType::Mat4;
	}

	MaterialAttribute(std::string name, std::shared_ptr<Texture> textureValue)
	{
		this->name = name;
		this->textureValue = textureValue;
		this->type = MaterialAttributeType::Texture;
	}

	std::string name;
	MaterialAttributeType type;
	std::weak_ptr<Texture> textureValue;
	union
	{
		float floatValue;
		int intValue;
		glm::vec2 vector2Value;
		glm::vec3 vector3Value;
		glm::vec4 vector4Value;
		glm::mat4 mat4Value;
	};
};

struct Material : public Resource
{

public:
	Material() = default;
	void Init(std::shared_ptr<Shader> shader);
	void Init(std::string name, std::shared_ptr<Shader> shader);
	std::vector<MaterialAttribute> parameters;
	std::weak_ptr<Shader> shader;

	int Find(std::string name);
	void Set(MaterialAttribute attribute);
	void Set(std::string name, std::shared_ptr<Texture> textureValue);
	void Set(std::string name, float floatValue);
	void Set(std::string name, int intValue);
	void Set(std::string name, glm::vec2 vector2Value);
	void Set(std::string name, glm::vec3 vector3Value);
	void Set(std::string name, glm::vec4 vector4Value);
	void Set(std::string name, glm::mat4 mat4Value);
	void Bind();

};

struct MaterialInstance : public Material
{
	MaterialInstance() = default;
	MaterialInstance(std::shared_ptr<Material> material, std::string name, bool modifiable = true);

	bool modifiable;
	std::weak_ptr<Material> base;
};