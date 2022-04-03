#pragma once

#include "Matrices.h"
struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 vertTangent;
	glm::vec3 vertBitangent;
	glm::vec4 color;
	glm::vec2 uv;
};