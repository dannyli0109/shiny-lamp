#pragma once
#include "Matrices.h"
#include "Vertex.h"
#include <vector>
#include "Graphics.h"

class Geometry
{
public:
	Geometry(std::vector<Vertex> vertices, std::vector<unsigned short> indices) : vertices(vertices), indices(indices) {
		Init();
	};
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;
	void Init();
	void Draw();
private:
	GLuint vbo, ebo, vao;
};