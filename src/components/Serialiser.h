#pragma once
#include "OpenGLRenderer.h"
#include "yaml-cpp/yaml.h"
#include "Util.h"

class Scene;
class Entity;
class TagComponent;
class TransformComponent;
class MeshRendererComponent;
class Shader;

class SceneSerialiser
{
public:
	SceneSerialiser(std::shared_ptr<Scene> scene);
	bool Serialise(std::string folder);
	bool SerialiseEntity(YAML::Emitter& out, std::shared_ptr<Entity> entity);
	bool SerialiseTag(YAML::Emitter& out, std::shared_ptr<Entity> entity);
	bool SerialiseTransform(YAML::Emitter& out, std::shared_ptr<Entity> entity);
	bool SerialiseMeshRenderer(YAML::Emitter& out, std::shared_ptr<Entity> entity);
	
	static std::shared_ptr<Scene> Deserialise(std::string path);
	static std::shared_ptr<Entity> DeserialiseEntity(YAML::Node& node);
	static void DeserialiseComponents(YAML::Node& node, std::shared_ptr<Scene> scene, std::shared_ptr<Entity> entity);
	static std::shared_ptr<TagComponent> DeserialiseTag(YAML::Node& node);
	static std::shared_ptr<TransformComponent> DeserialiseTransform(YAML::Node& node, std::shared_ptr<Scene> scene);
	static std::shared_ptr<MeshRendererComponent> DeserialiseMeshRenderer(YAML::Node& node);
private:
	std::weak_ptr<Scene> scene;
};


class MaterialSerialiser
{
public:
	MaterialSerialiser(std::shared_ptr<Material> material);
	std::weak_ptr<Material> material;

	bool Serialise(std::string folder);
	bool SerialiseAttribute(YAML::Emitter& out, MaterialAttribute& attribute);

	static void Deserialise(std::string path);
	static MaterialAttribute DeserialiseAttribute(YAML::Node& node);
};

class MaterialInstanceSerialiser
{
public:
	MaterialInstanceSerialiser(std::shared_ptr<MaterialInstance> instance);
	std::weak_ptr<MaterialInstance> instance;

	bool Serialise(std::string folder);
	bool SerialiseAttribute(YAML::Emitter& out, MaterialAttribute& attribute);

	static void Deserialise(std::string path);
	static MaterialAttribute DeserialiseAttribute(YAML::Node& node);
};

class TextureSerialiser
{
public:
	TextureSerialiser(std::shared_ptr<Texture> texture);
	std::weak_ptr<Texture> texture;

	bool Serialise(std::string folder);

	static void Deserialise(std::string path);
};

class ShaderSerialiser
{
public:
	ShaderSerialiser(std::shared_ptr<Shader> shader);
	std::weak_ptr<Shader> shader;

	bool Serialise(std::string folder);
	bool SerialiseShaderData(YAML::Emitter& out, GLuint type, ShaderData& data);

	static void Deserialise(std::string path);
	static std::pair<GLuint, std::string> DeserialiseShaderData(const std::string& name, YAML::Node& data);
};

class MeshSerialiser
{
public:
	MeshSerialiser(std::shared_ptr<Mesh> mesh);
	std::weak_ptr<Mesh> mesh;

	bool Serialise(std::string folder);
	bool SerialiseVertex(YAML::Emitter& out, Vertex& vertex);

	static void Deserialise(std::string path);
	static Vertex&& DeserialiseVertex(YAML::Node& node);
};

class ModelSerialiser
{
public:
	ModelSerialiser(std::shared_ptr<Model> model);
	std::weak_ptr<Model> model;

	bool Serialise(std::string folder);
	bool SerialiseModel(YAML::Emitter& out, std::shared_ptr<Model> model);
	static void Deserialise(std::string path);
	static std::shared_ptr<Model> DeserialiseModel(YAML::Node& node);
};

class ResourceSerialiser
{
public:
	//ResourceSerialiser()
};