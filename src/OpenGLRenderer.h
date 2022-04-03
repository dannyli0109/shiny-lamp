#pragma once
#include "Singleton.h"
#include "Renderer.h"
#include "Graphics.h"
#include "Camera.h"
#include <iostream>
#include "InputManager.h"
#include "Shader.h"
#include "Model.h"
#include "UUID.h"
#include "Resource.h"

struct FrameBuffer
{
	GLuint fbo;
	GLuint rbo;
	GLuint textureId;
	bool initialised;
	int width;
	int height;
};


struct Texture : public Resource
{
	std::string filepath;
	GLuint id;
	glm::vec2 size;
};

struct Material : public Resource
{
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
		Bool
	};

	struct MaterialAttribute
	{
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

public:
	static std::vector<std::pair<std::string, MaterialAttributeType>> attributeNames;
	static std::map<std::string, MaterialAttributeType> attributeNameMap;
	static std::map<MaterialAttributeType, std::string> attributeTypeMap;

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
	MaterialInstance(std::shared_ptr<Material> material, std::string name, bool modifiable = true) {
		parameters = material->parameters;
		this->name = name;
		shader = material->shader;
		this->modifiable = modifiable;
		base = material;
	};
	bool modifiable;
	std::weak_ptr<Material> base;
};



class OpenGLRenderer : public Singleton<OpenGLRenderer>
{
public:
	enum
	{
		InputFramebuffer,
		OutputFramebuffer,
		DefaultFramebufferCount
	};
	
	enum
	{
		PhongShader,
		ColorShader,
		DefaultShaderCount
	};

	enum
	{
		SoulSpearModel,
		DefaultModelCount
	};

	enum
	{
		Black,
		White,
		SoulSpearDiffuse,
		SoulSpearNormal,
		SoulSpearSpecular,
		DefaultTextureCount
	};

	enum
	{
		PhongLighitngMaterial,
		ColorMaterial,
		DefaultMaterialCount
	};

	enum
	{
		SoulSpearPhongLighitngMaterialInstance,
		ColorMaterialInstance,
		DefaultMaterialInstanceCount
	};

	void InitRenderBuffer(int width, int height, int samples);
	void InitFrameBuffer(int width, int height);

	void InitCamera(
		glm::vec3 position,
		glm::vec3 up,
		float theta,
		float phi,
		float fovY,
		float aspect,
		float near,
		float far
	);

	void HandleCameraMovement(float dt, float moveSpeed, float turnSpeed);
	void BindFrameBuffer();
	void ResolveFrameBuffer(int width, int height);
	
	std::shared_ptr<Texture> LoadTexture(std::string name, std::string path);
	std::shared_ptr<Texture> LoadTextureWithId(std::string name, std::string path, std::string uuid);
	void BindTexture(int type, std::string varname, int textureType, int textureUnit);
	void BindTexture(std::shared_ptr<Shader> shader, std::string varname, std::shared_ptr<Texture> texture, int textureUnit);
	void UnbindTexture();

	void LoadModel(std::string path);
	std::shared_ptr<Model> LoadModel(std::shared_ptr<ModelData> model);
	//void DrawModel(int type);
	std::shared_ptr<Shader> LoadShader(std::string name, std::vector<std::pair<GLenum, std::string>> shaderDatas);
	std::shared_ptr<Shader> LoadShaderWithId(std::string name, std::vector<std::pair<GLenum, std::string>>& shaderDatas, std::string uuid);
	void LinkShaders();

	void SetUniform(std::shared_ptr<Shader> shader, std::string varname, glm::mat4& value, int count);
	void SetUniform(std::shared_ptr<Shader> shader, std::string varname, int value);
	void SetUniform(std::shared_ptr<Shader> shader, std::string varname, glm::vec3& value, int count);

	std::shared_ptr<Material> LoadMaterial(std::shared_ptr<Shader> shader, std::string name, std::vector<Material::MaterialAttribute> attributes);
	std::shared_ptr<Material> LoadMaterialWithId(std::shared_ptr<Shader> shader, std::string name, std::vector<Material::MaterialAttribute> attributes, std::string uuid);

	std::shared_ptr<MaterialInstance> LoadMaterialInstance(std::shared_ptr<Material> material, std::string name, bool modifiable);
	std::shared_ptr<MaterialInstance> LoadMaterialInstanceWithId(std::shared_ptr<Material> material, std::string name, bool modifiable, std::string uuid);

	std::shared_ptr<Texture> GetTexture(std::string uuid);
	std::shared_ptr<Shader> GetShader(std::string uuid);
	std::shared_ptr<Material> GetMaterial(std::string uuid);

	std::vector<std::shared_ptr<FrameBuffer>> framebuffers = std::vector<std::shared_ptr<FrameBuffer>>(DefaultFramebufferCount);
	std::vector<std::shared_ptr<Model>> models;
	std::vector<std::shared_ptr<Texture>> textures;
	std::vector<std::shared_ptr<Material>> materials;
	std::vector<std::shared_ptr<Shader>> shaders;
	std::vector<std::shared_ptr<MaterialInstance>> materialInstances;
	std::vector<std::shared_ptr<Geometry>> geometries;
	std::map<std::string, int> shaderMap;
	std::map<std::string, int> textureMap;
	std::map<std::string, int> materialMap;
	std::map<std::string, int> miMap;

	Camera camera;
	glm::mat4 GetProjectionMatrix();
	glm::mat4 GetViewMatrix();
	glm::vec3 GetCameraPosition();

private:
	std::shared_ptr<Texture> LoadTextureInternal(std::string name, std::string path);
	std::shared_ptr<Shader> LoadShaderInternal(std::string name, std::vector<std::pair<GLenum, std::string>>& shaderDatas);
	std::shared_ptr<Material> LoadMaterialInternal(std::shared_ptr<Shader> shader, std::string name, std::vector<Material::MaterialAttribute> attributes);
	std::shared_ptr<MaterialInstance> LoadMaterialInstanceInternal(std::shared_ptr<Material> material, std::string name, bool modifiable = true);
};