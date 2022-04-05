#include "OpenGLRenderer.h"
#include "ResourceManager.h"

void OpenGLRenderer::InitRenderBuffer(int width, int height, int samples)
{
	std::shared_ptr<FrameBuffer> framebuffer = framebuffers[InputFramebuffer];
	if (framebuffer == nullptr)
	{
		framebuffers[InputFramebuffer] = std::make_shared<FrameBuffer>();
		framebuffer = framebuffers[InputFramebuffer];
	}
	if (framebuffer->initialised)
	{
		glDeleteFramebuffers(1, &framebuffer->fbo);
		glDeleteRenderbuffers(1, &framebuffer->rbo);
		glDeleteTextures(1, &framebuffer->textureId);
		framebuffer->initialised = false;
	}

	glGenFramebuffers(1, &framebuffer->fbo);
	glGenTextures(1, &framebuffer->textureId);
	glGenRenderbuffers(1, &framebuffer->rbo);
	framebuffer->width = width;
	framebuffer->height = height;


	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, framebuffer->textureId);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGBA, width, height, GL_TRUE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, framebuffer->textureId, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, framebuffer->rbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH32F_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, framebuffer->rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Frame buffer initialised successfully" << std::endl;
		framebuffer->initialised = true;
	}
	else {
		std::cout << "Failed to initialise frame buffer" << std::endl;
		glDeleteFramebuffers(1, &framebuffer->fbo);
		glDeleteRenderbuffers(1, &framebuffer->rbo);
		glDeleteTextures(1, &framebuffer->textureId);
		framebuffer->initialised = false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

	glEnable(GL_DEPTH_TEST);
}

void OpenGLRenderer::InitFrameBuffer(int width, int height)
{
	std::shared_ptr<FrameBuffer> framebuffer = framebuffers[OutputFramebuffer];
	if (framebuffer == nullptr)
	{
		framebuffers[OutputFramebuffer] = std::make_shared<FrameBuffer>();
		framebuffer = framebuffers[OutputFramebuffer];
	}
	if (framebuffer->initialised)
	{
		glDeleteFramebuffers(1, &framebuffer->fbo);
		glDeleteTextures(1, &framebuffer->textureId);
		framebuffer->initialised = false;
	}

	glGenFramebuffers(1, &framebuffer->fbo);
	glGenTextures(1, &framebuffer->textureId);
	framebuffer->width = width;
	framebuffer->height = height;

	glBindTexture(GL_TEXTURE_2D, framebuffer->textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->fbo);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, framebuffer->textureId, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Frame buffer initialised successfully" << std::endl;
		framebuffer->initialised = true;
	}
	else {
		std::cout << "Failed to initialise frame buffer" << std::endl;
		glDeleteFramebuffers(1, &framebuffer->fbo);
		glDeleteTextures(1, &framebuffer->textureId);
		framebuffer->initialised = false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGLRenderer::InitCamera(glm::vec3 position, glm::vec3 up, float theta, float phi, float fovY, float aspect, float near, float far)
{
	camera.position = position;
	camera.up = up;
	camera.theta = theta;
	camera.phi = phi;
	camera.fovY = fovY;
	camera.aspect = aspect;
	camera.near = near;
	camera.far = far;
}

void OpenGLRenderer::HandleCameraMovement(float dt, float moveSpeed, float turnSpeed)
{
	glm::vec3 forward(cosf(camera.phi) * cosf(camera.theta), sinf(camera.phi), sinf(camera.theta));
	glm::vec3 right(-sinf(camera.theta), 0, cos(camera.theta));
	glm::vec3 up(0, 1, 0);
	InputManager& input = InputManager::GetSingleton();
	if (input.GetKey(InputManager::W_KEY))
		camera.position += forward * moveSpeed * dt;
	if (input.GetKey(InputManager::S_KEY))
		camera.position -= forward * moveSpeed * dt;
	if (input.GetKey(InputManager::A_KEY))
		camera.position -= right * moveSpeed * dt;
	if (input.GetKey(InputManager::D_KEY))
		camera.position += right * moveSpeed * dt;

	if (input.GetKey(InputManager::RIGHT_MOUSE))
	{
		camera.theta += turnSpeed * (input.cursorPos.x - input.lastCursorPos.x) * dt;
		camera.phi -= turnSpeed * (input.cursorPos.y - input.lastCursorPos.y) * dt;
	}
}



void OpenGLRenderer::BindFrameBuffer()
{
	std::shared_ptr<FrameBuffer> framebuffer = framebuffers[InputFramebuffer];
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->fbo);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderer::ResolveFrameBuffer(int width, int height)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffers[InputFramebuffer]->fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffers[OutputFramebuffer]->fbo);
	glViewport(0, 0, width, height);
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}


void OpenGLRenderer::BindTexture(int type, std::string varname, int textureType, int textureUint)
{
	ResourceManager& resources = ResourceManager::GetSingleton();
	resources.shaders[type]->BindTexture(varname, resources.textures[textureType]->id, textureUint);
}

void OpenGLRenderer::BindTexture(std::shared_ptr<Shader> shader, std::string varname, std::shared_ptr<Texture> texture, int textureUint)
{
	shader->BindTexture(varname, texture->id, textureUint);
}

void OpenGLRenderer::UnbindTexture()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}


void OpenGLRenderer::LinkShaders()
{
	ResourceManager& resources = ResourceManager::GetSingleton();
	for (auto& shader : resources.shaders)
	{
		shader->Link();
	}
}

//void OpenGLRenderer::SetUniform(std::string shaderName, std::string varname, glm::mat4& value, int count)
//{
//	shaders[shaderName]->SetUniform(varname, value, count);
//}
//
//void OpenGLRenderer::SetUniform(std::string shaderName, std::string varname, int value)
//{
//	shaders[shaderName]->SetUniform(varname, value);
//}
//
//void OpenGLRenderer::SetUniform(std::string shaderName, std::string varname, glm::vec3& value, int count)
//{
//	shaders[shaderName]->SetUniform(varname, value, count);
//}

void OpenGLRenderer::SetUniform(std::shared_ptr<Shader> shader, std::string varname, glm::mat4& value, int count)
{
	shader->SetUniform(varname, value, count);
}

void OpenGLRenderer::SetUniform(std::shared_ptr<Shader> shader, std::string varname, int value)
{
	shader->SetUniform(varname, value);
}

void OpenGLRenderer::SetUniform(std::shared_ptr<Shader> shader, std::string varname, glm::vec3& value, int count)
{
	shader->SetUniform(varname, value, count);
}


glm::mat4 OpenGLRenderer::GetProjectionMatrix()
{
	return glm::perspective(camera.fovY, camera.aspect, camera.near, camera.far);
}

glm::mat4 OpenGLRenderer::GetViewMatrix()
{
	glm::vec3 forward(std::cos(camera.phi) * std::cos(camera.theta), std::sin(camera.phi), std::cos(camera.phi) * std::sin(camera.theta));
	return glm::lookAt(camera.position, camera.position + forward, camera.up);
}

glm::vec3 OpenGLRenderer::GetCameraPosition()
{
	return camera.position;
}

