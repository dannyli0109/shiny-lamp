#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <memory>
#include <iostream>
#include "Model.h";

namespace Util
{
	std::string LoadFileAsString(std::string filename);
	void ProcessNode(aiNode* node, const aiScene* scene, std::shared_ptr<ModelData> nodePtr);
	std::shared_ptr<ModelData> LoadModel(std::string filename);
    bool FileExists(std::string filename);
    std::string GetValidFilename(std::string filename, std::string extension);
}

