#include "Mesh.h"

void Mesh::Init(std::shared_ptr<MeshData> data)
{
	this->data = data;
	if (initialised)
	{
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ebo);
		initialised = false;
	}

	// generate buffers
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	// bind vertex array
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, data->vertices.size() * sizeof(Vertex), data->vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(vao);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data->indices.size() * sizeof(unsigned short), data->indices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	float stride = CalculateStride();
	int offset = 0;
	for (int i = 0; i < attributes.size(); i++)
	{
		glVertexAttribPointer(i, attributes[i].size, attributes[i].type, GL_FALSE, stride, (void*)offset);
		offset += attributes[i].size * attributes[i].dataSize;
		glEnableVertexAttribArray(i);
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	initialised = true;
}

void Mesh::Draw()
{
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, data->indices.size(), GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::AddAttribute(MeshAttribute attribute)
{
	attributes.push_back(attribute);
}

float Mesh::CalculateStride()
{
	float result = 0;
	for (int i = 0; i < attributes.size(); i++)
	{
		result += attributes[i].dataSize * attributes[i].size;

	}
	return result;
}
