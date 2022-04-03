#include "Shader.h"
#include "Util.h"

//void Shader::LoadShader(GLenum shaderType, const std::string& shaderCode)
//{
//	GLuint id = glCreateShader(shaderType);
//	ShaderData shaderData = { id, shaderCode };
//	data[shaderType] = shaderData;
//}

void Shader::LoadShaderFromFile(GLenum shaderType, const std::string& path)
{
	GLuint id = glCreateShader(shaderType);
	std::string& shaderCode = Util::LoadFileAsString(path);
	ShaderData shaderData = { id, shaderCode, path };
	data[shaderType] = shaderData;
}

void Shader::Link()
{
	programId = glCreateProgram();

	LinkShader(GL_VERTEX_SHADER);

	LinkShader(GL_FRAGMENT_SHADER);

	glLinkProgram(programId);

	GLchar errorLog[256];
	GLint compileSuccess;
	glGetProgramiv(programId, GL_LINK_STATUS, &compileSuccess);

	if (compileSuccess == GL_FALSE)
	{
		std::cout << "Error linking shaders" << std::endl;
		glGetProgramInfoLog(programId, 256, nullptr, errorLog);
		std::cout << errorLog;

		//glDeleteShader(vertexShader);
		//glDeleteShader(fragmentShader);
		//glDeleteProgram(program);
	}
}

void Shader::Use()
{
	glUseProgram(programId);
}

void Shader::Begin()
{
	//textureUnit = 1;
}

void Shader::SetUniform(std::string varname, glm::mat4& value, int count)
{
	GLuint varloc = glGetUniformLocation(programId, varname.c_str());
	glUniformMatrix4fv(varloc, count, GL_FALSE, &value[0][0]);
}

void Shader::SetUniform(std::string varname, int value)
{
	GLuint varloc = glGetUniformLocation(programId, varname.c_str());
	glUniform1i(varloc, value);
}

void Shader::SetUniform(std::string varname, float value)
{
	GLuint varloc = glGetUniformLocation(programId, varname.c_str());
	glUniform1f(varloc, value);
}

void Shader::SetUniform(std::string varname, glm::vec2& value, int count)
{
	GLuint varloc = glGetUniformLocation(programId, varname.c_str());
	glUniform2fv(varloc, count, &value.x);
}

void Shader::SetUniform(std::string varname, glm::vec3& value, int count)
{
	GLuint varloc = glGetUniformLocation(programId, varname.c_str());
	glUniform3fv(varloc, count, &value.x);
}

void Shader::SetUniform(std::string varname, glm::vec4& value, int count)
{
	GLuint varloc = glGetUniformLocation(programId, varname.c_str());
	glUniform4fv(varloc, count, &value.x);
}

void Shader::BindTexture(std::string varname, GLuint textureId, int textureUnit)
{
	glBindTexture(GL_TEXTURE_2D, textureId);
	glBindTextureUnit(textureUnit, textureId);
	SetUniform(varname.c_str(), textureUnit);
	//textureUnit++;
}

Shader::~Shader()
{
	for (auto shader : data)
	{
		glDeleteShader(shader.second.id);
	}
	glDeleteProgram(programId);
}

void Shader::LinkShader(GLenum shaderType)
{
	ShaderData shaderData = data[shaderType];
	const std::string& shaderCode = shaderData.code;
	const char* pCode = shaderCode.c_str();
	GLint length = shaderCode.length();

	glShaderSource(shaderData.id, 1, &pCode, &length);
	glCompileShader(shaderData.id);
	glAttachShader(programId, shaderData.id);


	GLchar errorLog[256];
	GLint compileSuccess;
	glGetShaderiv(shaderData.id, GL_COMPILE_STATUS, &compileSuccess);
	if (compileSuccess == GL_FALSE)
	{
		//std::cout << "Error compiling vertex shader: " << vertexFilename << std::endl;
		glGetShaderInfoLog(shaderData.id, 256, nullptr, errorLog);
		std::cout << errorLog;
	}
}
