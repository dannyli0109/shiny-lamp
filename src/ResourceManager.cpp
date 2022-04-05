#include "ResourceManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Util.h"
#include "Mesh.h"
#include "Shader.h"
#include "UUID.h"


std::shared_ptr<Texture> ResourceManager::GetTexture(std::string uuid)
{
	return GetResource<Texture>(uuid, textures);
}

std::shared_ptr<Shader> ResourceManager::GetShader(std::string uuid)
{
	return GetResource<Shader>(uuid, shaders);
}

std::shared_ptr<Material> ResourceManager::GetMaterial(std::string uuid)
{
	return GetResource<Material>(uuid, materials);
}

std::shared_ptr<MaterialInstance> ResourceManager::GetMaterialInstance(std::string uuid)
{
	return GetResource<MaterialInstance>(uuid, materialInstances);
}

std::shared_ptr<Model> ResourceManager::GetModel(std::string uuid)
{
	return GetResource<Model>(uuid, models);
}

std::shared_ptr<Mesh> ResourceManager::GetMesh(std::string uuid)
{
	return GetResource<Mesh>(uuid, meshes);
}

int ResourceManager::GetTextureIndex(std::string uuid)
{
	return GetResourceIndex<Texture>(uuid, textures);
}

int ResourceManager::GetShaderIndex(std::string uuid)
{
	return GetResourceIndex<Shader>(uuid, shaders);
}

int ResourceManager::GetMaterialIndex(std::string uuid)
{
	return GetResourceIndex<Material>(uuid, materials);
}

int ResourceManager::GetMaterialInstanceIndex(std::string uuid)
{
	return GetResourceIndex<MaterialInstance>(uuid, materialInstances);
}

int ResourceManager::GetModelIndex(std::string uuid)
{
	return GetResourceIndex<Model>(uuid, models);
}

int ResourceManager::GetMeshIndex(std::string uuid)
{
	return GetResourceIndex<Mesh>(uuid, meshes);
}

std::shared_ptr<Mesh> ResourceManager::LoadMesh(std::string name, std::shared_ptr<MeshData> data)
{
	std::shared_ptr<Mesh> mesh = LoadMeshInternal(name, data);
	mesh->uuid.Init();
	meshes.push_back(mesh);
	return mesh;
}

std::shared_ptr<Mesh> ResourceManager::LoadMeshWithId(std::string name, std::shared_ptr<MeshData> data, std::string uuid)
{
	std::shared_ptr<Mesh> mesh = LoadMeshInternal(name, data);
	mesh->uuid.Init(uuid);
	meshes.push_back(mesh);
	return mesh;
}


void ResourceManager::LoadModel(std::string name, std::string path)
{
	std::shared_ptr<ModelData> data = Util::LoadModel(path);
	std::shared_ptr<Model> model = LoadModel(name, data);
	models.push_back(model);
}


std::shared_ptr<Model> ResourceManager::LoadModel(std::string name, std::shared_ptr<ModelData> data)
{

	std::shared_ptr<Model> model = LoadModelInternal(name, data);
	model->uuid.Init();
	for (int i = 0; i < data->children.size(); i++)
	{
		model->children.push_back(LoadModel(name, data->children[i]));
	}

	for (int i = 0; i < data->meshes.size(); i++)
	{
		std::shared_ptr<Mesh> mesh = LoadMesh(data->meshes[i]->name, data->meshes[i]);
		model->meshes.push_back(mesh);
	}
	return model;
}

std::shared_ptr<Model> ResourceManager::LoadModelWithId(std::string name, std::shared_ptr<ModelData> data, std::string uuid)
{
	std::shared_ptr<Model> model = LoadModelInternal(name, data);
	model->uuid.Init(uuid);
	return model;
}


std::shared_ptr<Shader> ResourceManager::LoadShader(std::string name, std::vector<std::pair<GLenum, std::string>> shaderDatas)
{
	std::shared_ptr<Shader> shader = LoadShaderInternal(name, shaderDatas);
	shaders.push_back(shader);
	shader->uuid.Init();
	return shader;
}

std::shared_ptr<Shader> ResourceManager::LoadShaderWithId(std::string name, std::vector<std::pair<GLenum, std::string>>& shaderDatas, std::string uuid)
{
	std::shared_ptr<Shader> shader = LoadShaderInternal(name, shaderDatas);
	shaders.push_back(shader);
	shader->uuid.Init(uuid);
	return shader;
}

std::shared_ptr<Texture> ResourceManager::LoadTexture(std::string name, std::string path)
{
	std::shared_ptr<Texture> texture = LoadTextureInternal(name, path);
	textures.push_back(texture);
	texture->uuid.Init();
	return texture;
}

std::shared_ptr<Texture> ResourceManager::LoadTextureWithId(std::string name, std::string path, std::string uuid)
{
	std::shared_ptr<Texture> texture = LoadTextureInternal(name, path);
	textures.push_back(texture);
	texture->uuid.Init(uuid);
	return texture;
}

std::shared_ptr<MaterialInstance> ResourceManager::LoadMaterialInstance(std::shared_ptr<Material> material, std::string name, bool modifiable)
{
	std::shared_ptr<MaterialInstance> mi = LoadMaterialInstanceInternal(material, name, modifiable);
	mi->uuid.Init();
	materialInstances.push_back(mi);
	return mi;
}

std::shared_ptr<MaterialInstance> ResourceManager::LoadMaterialInstanceWithId(std::shared_ptr<Material> material, std::string name, bool modifiable, std::string uuid)
{
	std::shared_ptr<MaterialInstance> mi = LoadMaterialInstanceInternal(material, name, modifiable);
	mi->uuid.Init(uuid);
	materialInstances.push_back(mi);
	return mi;
}

std::shared_ptr<Material> ResourceManager::LoadMaterial(std::shared_ptr<Shader> shader, std::string name, std::vector<MaterialAttribute> attributes)
{
	std::shared_ptr<Material> material = LoadMaterialInternal(shader, name, attributes);
	material->uuid.Init();
	materials.push_back(material);
	return material;
}

std::shared_ptr<Material> ResourceManager::LoadMaterialWithId(std::shared_ptr<Shader> shader, std::string name, std::vector<MaterialAttribute> attributes, std::string uuid)
{
	std::shared_ptr<Material> material = LoadMaterialInternal(shader, name, attributes);
	material->uuid.Init(uuid);
	materials.push_back(material);
	return material;
}


std::shared_ptr<Texture> ResourceManager::LoadTextureInternal(std::string name, std::string path)
{
	int width, height, channels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 4);
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);

	std::shared_ptr<Texture> texture = std::make_shared<Texture>();

	texture->id = textureID;
	texture->size = { width, height };
	texture->name = name;
	texture->filepath = path;
	return texture;
}

std::shared_ptr<Material> ResourceManager::LoadMaterialInternal(std::shared_ptr<Shader> shader, std::string name, std::vector<MaterialAttribute> attributes)
{
	std::shared_ptr<Material> material = std::make_shared<Material>();
	material->Init(name, shader);
	material->parameters = attributes;
	return material;
}

std::shared_ptr<MaterialInstance> ResourceManager::LoadMaterialInstanceInternal(std::shared_ptr<Material> material, std::string name, bool modifiable)
{
	std::shared_ptr<MaterialInstance> mi = std::make_shared<MaterialInstance>(material, name, modifiable);
	return mi;
}

std::shared_ptr<Mesh> ResourceManager::LoadMeshInternal(std::string name, std::shared_ptr<MeshData> data)
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
	mesh->AddAttribute(Mesh::MeshAttribute({ 3, GL_FLOAT, sizeof(float) }));
	mesh->AddAttribute(Mesh::MeshAttribute({ 3, GL_FLOAT, sizeof(float) }));
	mesh->AddAttribute(Mesh::MeshAttribute({ 3, GL_FLOAT, sizeof(float) }));
	mesh->AddAttribute(Mesh::MeshAttribute({ 3, GL_FLOAT, sizeof(float) }));
	mesh->AddAttribute(Mesh::MeshAttribute({ 4, GL_FLOAT, sizeof(float) }));
	mesh->AddAttribute(Mesh::MeshAttribute({ 2, GL_FLOAT, sizeof(float) }));
	mesh->Init(data);
	mesh->name = name;
	return mesh;
}

std::shared_ptr<Model> ResourceManager::LoadModelInternal(std::string name, std::shared_ptr<ModelData> data)
{
	std::shared_ptr<Model> model = std::make_shared<Model>();
	model->name = name;
	return model;
}

std::shared_ptr<Shader> ResourceManager::LoadShaderInternal(std::string name, std::vector<std::pair<GLenum, std::string>>& shaderDatas)
{
	std::shared_ptr<Shader> shader = std::make_shared<Shader>();
	shader->name = name;
	for (auto& [type, filepath] : shaderDatas)
	{
		shader->LoadShaderFromFile(type, filepath);
	}
	return shader;
}


