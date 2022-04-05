#pragma once
#include "Singleton.h"
#include <vector>
#include <memory>
#include "Model.h"
#include "Graphics.h"
#include "Resource.h"

class Texture;
class Material;
class MaterialAttribute;

class MaterialInstance;
class Mesh;

class ResourceManager : public Singleton<ResourceManager>
{
public:
	enum class ResourceType
	{
		BaseMaterial,
		Material,
		Mesh,
		Model,
		Scene,
		Shader,
		Texture
	};

	void LoadModel(std::string name, std::string path);

	std::shared_ptr<Texture> LoadTexture(std::string name, std::string path);
	std::shared_ptr<Texture> LoadTextureWithId(std::string name, std::string path, std::string uuid);

	std::shared_ptr<Model> LoadModel(std::string name, std::shared_ptr<ModelData> data);
	std::shared_ptr<Model> LoadModelWithId(std::string name, std::shared_ptr<ModelData> data, std::string uuid);

	std::shared_ptr<Shader> LoadShader(std::string name, std::vector<std::pair<GLenum, std::string>> shaderDatas);
	std::shared_ptr<Shader> LoadShaderWithId(std::string name, std::vector<std::pair<GLenum, std::string>>& shaderDatas, std::string uuid);

	std::shared_ptr<Material> LoadMaterial(std::shared_ptr<Shader> shader, std::string name, std::vector<MaterialAttribute> attributes);
	std::shared_ptr<Material> LoadMaterialWithId(std::shared_ptr<Shader> shader, std::string name, std::vector<MaterialAttribute> attributes, std::string uuid);

	std::shared_ptr<MaterialInstance> LoadMaterialInstance(std::shared_ptr<Material> material, std::string name, bool modifiable);
	std::shared_ptr<MaterialInstance> LoadMaterialInstanceWithId(std::shared_ptr<Material> material, std::string name, bool modifiable, std::string uuid);

	std::shared_ptr<Mesh> LoadMesh(std::string name, std::shared_ptr<MeshData> data);
	std::shared_ptr<Mesh> LoadMeshWithId(std::string name, std::shared_ptr<MeshData> data, std::string uuid);

	std::shared_ptr<Texture> GetTexture(std::string uuid);
	std::shared_ptr<Shader> GetShader(std::string uuid);
	std::shared_ptr<Material> GetMaterial(std::string uuid);
	std::shared_ptr<MaterialInstance> GetMaterialInstance(std::string uuid);
	std::shared_ptr<Model> GetModel(std::string uuid);
	std::shared_ptr<Mesh> GetMesh(std::string uuid);

	int GetTextureIndex(std::string uuid);
	int GetShaderIndex(std::string uuid);
	int GetMaterialIndex(std::string uuid);
	int GetMaterialInstanceIndex(std::string uuid);
	int GetModelIndex(std::string uuid);
	int GetMeshIndex(std::string uuid);


	std::vector<std::shared_ptr<Model>> models;
	std::vector<std::shared_ptr<Texture>> textures;
	std::vector<std::shared_ptr<Material>> materials;
	std::vector<std::shared_ptr<Shader>> shaders;
	std::vector<std::shared_ptr<MaterialInstance>> materialInstances;
	std::vector<std::shared_ptr<Mesh>> meshes;


	template <typename T>
	int GetResourceIndex(std::string uuid, std::vector<std::shared_ptr<T>>& data)
	{
		auto it = std::find_if(data.begin(), data.end(), [&](const std::shared_ptr<T> item)
			{
				return std::string(item->uuid) == uuid;
			}
		);
		if (it == data.end()) return -1;
		return std::distance(data.begin(), it);
	}

	template <typename T>
	std::shared_ptr<T> GetResource(std::string uuid, std::vector<std::shared_ptr<T>>& data)
	{
		int index = GetResourceIndex<T>(uuid, data);
		if (index == -1) return nullptr;
		return data[index];
	}

private:
	std::shared_ptr<Texture> LoadTextureInternal(std::string name, std::string path);
	std::shared_ptr<Shader> LoadShaderInternal(std::string name, std::vector<std::pair<GLenum, std::string>>& shaderDatas);
	std::shared_ptr<Material> LoadMaterialInternal(std::shared_ptr<Shader> shader, std::string name, std::vector<MaterialAttribute> attributes);
	std::shared_ptr<MaterialInstance> LoadMaterialInstanceInternal(std::shared_ptr<Material> material, std::string name, bool modifiable = true);
	std::shared_ptr<Model> LoadModelInternal(std::string name, std::shared_ptr<ModelData> data);
	std::shared_ptr<Mesh> LoadMeshInternal(std::string name, std::shared_ptr<MeshData> data);
};

