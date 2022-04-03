#include "Util.h"

std::string Util::LoadFileAsString(std::string filename)
{
    std::stringstream fileSoFar;
    std::ifstream file(filename.c_str());

    if (file.is_open())
    {
        while (!file.eof())
        {
            std::string line;
            std::getline(file, line);
            fileSoFar << line << std::endl;
        }
        file.close();
        return fileSoFar.str();
    }
    else
    {
        return "";
    }
}

void Util::ProcessNode(aiNode* node, const aiScene* scene, std::shared_ptr<ModelData> nodePtr)
{
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		std::vector<Vertex> vertices;
		std::vector<unsigned short> indices;

		vertices.reserve(mesh->mNumVertices);
		indices.reserve(mesh->mNumFaces);


		for (int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			vertex.position = {
				mesh->mVertices[i].x,
				mesh->mVertices[i].y,
				mesh->mVertices[i].z
			};

			if (mesh->HasVertexColors(0))
			{
				vertex.color = {
					mesh->mColors[0][i].r,
					mesh->mColors[0][i].g,
					mesh->mColors[0][i].b,
					mesh->mColors[0][i].a
				};
			}
			else
			{
				vertex.color = {
					0, 0, 0, 1
				};
			}

			if (mesh->HasNormals())
			{
				vertex.normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
			}
			else
			{
				vertex.normal = { 0, 0, 0 };
			}

			if (mesh->mTextureCoords[0])
			{
				vertex.uv = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
			}
			else
			{
				vertex.uv = { 0, 0 };
			}

			if (mesh->HasTangentsAndBitangents())
			{
				vertex.vertTangent = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
				vertex.vertBitangent = { mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z };
			}
			else
			{
				vertex.vertTangent = { 0, 0, 0 };
				vertex.vertBitangent = { 0, 0, 0 };
			}

			vertices.push_back(vertex);
		}

		for (int i = 0; i < mesh->mNumFaces; i++)
		{
			for (int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
			{
				indices.push_back(mesh->mFaces[i].mIndices[j]);
			}
		}
		std::shared_ptr<Geometry> newGeometry = std::make_shared<Geometry>(Geometry({ vertices, indices }));
		nodePtr->geometries.push_back(newGeometry);
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		std::shared_ptr<ModelData> childPtr = std::make_shared<ModelData>();
		nodePtr->children.push_back(childPtr);
		ProcessNode(node->mChildren[i], scene, childPtr);
	}
}

std::shared_ptr<ModelData> Util::LoadModel(std::string filename)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filename.c_str(), aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return nullptr;
    }
	std::shared_ptr<ModelData> node = std::make_shared<ModelData>();

    ProcessNode(scene->mRootNode, scene, node);
	return node;
}

std::string Util::GetValidFilename(std::string filename, std::string extension)
{
	if (!FileExists(filename + "." + extension)) return filename + "." + extension;
	int index = 1;
	while (true)
	{
		std::string name = filename + "(" + std::to_string(index) + ")" + "." + extension;
		if (!FileExists(name))
		{
			return name;
		}
		index++;
	}
	return filename + "." + extension;
}

bool Util::FileExists(std::string filename)
{
	std::ifstream ifile;
	ifile.open(filename);
	if (ifile) {
		ifile.close();
		return true;
	}
	else {
		return false;
	}
}
