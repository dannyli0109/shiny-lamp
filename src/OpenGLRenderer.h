#pragma once
#include "Singleton.h"
#include "Renderer.h"
#include "Graphics.h"
#include "Camera.h"
#include <iostream>
#include "InputManager.h"
#include "Shader.h"
#include "Model.h"
#include "UUID.h"
//#include "Resource.h"
//#include "ResourceManager.h"


class Texture;
class Shader;
struct FrameBuffer
{
	GLuint fbo;
	GLuint rbo;
	GLuint textureId;
	bool initialised;
	int width;
	int height;
};



class OpenGLRenderer : public Singleton<OpenGLRenderer>
{
public:
	enum
	{
		InputFramebuffer,
		OutputFramebuffer,
		DefaultFramebufferCount
	};
	
	//enum
	//{
	//	PhongShader,
	//	ColorShader,
	//	DefaultShaderCount
	//};

	//enum
	//{
	//	SoulSpearModel,
	//	DefaultModelCount
	//};

	//enum
	//{
	//	Black,
	//	White,
	//	SoulSpearDiffuse,
	//	SoulSpearNormal,
	//	SoulSpearSpecular,
	//	DefaultTextureCount
	//};

	//enum
	//{
	//	PhongLighitngMaterial,
	//	ColorMaterial,
	//	DefaultMaterialCount
	//};

	//enum
	//{
	//	SoulSpearPhongLighitngMaterialInstance,
	//	ColorMaterialInstance,
	//	DefaultMaterialInstanceCount
	//};

	void InitRenderBuffer(int width, int height, int samples);
	void InitFrameBuffer(int width, int height);

	void InitCamera(
		glm::vec3 position,
		glm::vec3 up,
		float theta,
		float phi,
		float fovY,
		float aspect,
		float near,
		float far
	);

	void HandleCameraMovement(float dt, float moveSpeed, float turnSpeed);
	void BindFrameBuffer();
	void ResolveFrameBuffer(int width, int height);
	
	void BindTexture(int type, std::string varname, int textureType, int textureUnit);
	void BindTexture(std::shared_ptr<Shader> shader, std::string varname, std::shared_ptr<Texture> texture, int textureUnit);
	void UnbindTexture();

	//void DrawModel(int type);
	void LinkShaders();

	void SetUniform(std::shared_ptr<Shader> shader, std::string varname, glm::mat4& value, int count);
	void SetUniform(std::shared_ptr<Shader> shader, std::string varname, int value);
	void SetUniform(std::shared_ptr<Shader> shader, std::string varname, glm::vec3& value, int count);

	std::vector<std::shared_ptr<FrameBuffer>> framebuffers = std::vector<std::shared_ptr<FrameBuffer>>(DefaultFramebufferCount);

	Camera camera;
	glm::mat4 GetProjectionMatrix();
	glm::mat4 GetViewMatrix();
	glm::vec3 GetCameraPosition();

};