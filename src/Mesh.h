#pragma once
#include "Matrices.h"
#include "Vertex.h"
#include <vector>
#include "Graphics.h"
#include "Resource.h"

struct MeshData
{
	std::string name;
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;
};


class Mesh : public Resource
{
public:
	struct MeshAttribute
	{
		GLint size;
		GLenum type;
		int dataSize;
	};

	Mesh() = default;

	void Init(std::shared_ptr<MeshData> data);
	void Draw();
	void AddAttribute(MeshAttribute attribute);
	std::shared_ptr<MeshData> data;
private:
	std::vector<MeshAttribute> attributes;
	GLuint vbo, ebo, vao;
	bool initialised = false;
	float CalculateStride();
};