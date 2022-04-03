#pragma once
#include "Graphics.h"
#include <string>
#include <map>
#include <iostream>
#include "Resource.h"

struct ShaderData
{
	GLuint id;
	std::string code;
	std::string filepath;
};

class Shader : public Resource
{
public:
	//void LoadShader(GLenum shaderType, const std::string& shaderCode);
	void LoadShaderFromFile(GLenum shaderType, const std::string& shaderCode);
	virtual void Link();
	virtual void Use();
	virtual void Begin();
	virtual void SetUniform(std::string varname, glm::mat4& value, int count);
	virtual void SetUniform(std::string varname, int value);
	virtual void SetUniform(std::string varname, float);
	virtual void SetUniform(std::string varname, glm::vec2& value, int count);
	virtual void SetUniform(std::string varname, glm::vec3& value, int count);
	virtual void SetUniform(std::string varname, glm::vec4& value, int count);
	virtual void BindTexture(std::string varname, GLuint textureId, int textureUint);
	std::map<GLenum, ShaderData> data;
	~Shader();
protected:
	virtual void LinkShader(GLenum shaderType);
protected:
	GLuint programId;
	//int textureUnit;
private:
};