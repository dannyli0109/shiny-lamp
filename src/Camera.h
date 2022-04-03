#pragma once
#include "Graphics.h"

struct Camera
{
	glm::vec3 position;
	glm::vec3 up;
	float theta;
	float phi;

	float fovY;
	float aspect;
	float near;
	float far;

	glm::dvec2 lastMousePos;
};