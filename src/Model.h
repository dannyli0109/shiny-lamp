#pragma once
#include "Geometry.h"
#include <memory>

struct ModelData
{
	std::vector<std::shared_ptr<Geometry>> geometries;
	std::vector<std::shared_ptr<ModelData>> children;
};

struct Model
{
	std::vector<int> geometryIndices;
	std::vector<std::shared_ptr<Model>> children;
};