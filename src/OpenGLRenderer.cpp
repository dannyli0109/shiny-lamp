#include "OpenGLRenderer.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Util.h"

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

std::shared_ptr<Texture> OpenGLRenderer::LoadTexture(std::string name, std::string path)
{
	std::shared_ptr<Texture> texture = LoadTextureInternal(name, path);
	textures.push_back(texture);
	texture->uuid.Init();
	textureMap[texture->uuid] = textures.size() - 1;
	return texture;
}

std::shared_ptr<Texture> OpenGLRenderer::LoadTextureWithId(std::string name, std::string path, std::string uuid)
{
	std::shared_ptr<Texture> texture = LoadTextureInternal(name, path);
	textures.push_back(texture);
	texture->uuid.Init(uuid);
	textureMap[texture->uuid] = textures.size() - 1;
	return texture;
}

void OpenGLRenderer::BindTexture(int type, std::string varname, int textureType, int textureUint)
{
	shaders[type]->BindTexture(varname, textures[textureType]->id, textureUint);
}

void OpenGLRenderer::BindTexture(std::shared_ptr<Shader> shader, std::string varname, std::shared_ptr<Texture> texture, int textureUint)
{
	shader->BindTexture(varname, texture->id, textureUint);
}

void OpenGLRenderer::UnbindTexture()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGLRenderer::LoadModel(std::string path)
{
	std::shared_ptr<ModelData> data = Util::LoadModel(path);
	models.push_back(LoadModel(data));
}

std::shared_ptr<Model> OpenGLRenderer::LoadModel(std::shared_ptr<ModelData> modelData)
{

	std::shared_ptr<Model> model = std::make_shared<Model>();
	for (int i = 0; i < modelData->geometries.size(); i++)
	{
		geometries.push_back(modelData->geometries[i]);
		model->geometryIndices.push_back(geometries.size() - 1);
	}

	for (int i = 0; i < modelData->children.size(); i++)
	{
		model->children.push_back(LoadModel(modelData->children[i]));
	}
	return model;
}

//void OpenGLRenderer::DrawModel(int type)
//{
//	models[type]->Draw();
//}

std::shared_ptr<Shader> OpenGLRenderer::LoadShader(std::string name, std::vector<std::pair<GLenum, std::string>> shaderDatas)
{
	std::shared_ptr<Shader> shader = LoadShaderInternal(name, shaderDatas);
	shaders.push_back(shader);
	shader->uuid.Init();
	shaderMap[shader->uuid] = shaders.size() - 1;
	return shader;
}

std::shared_ptr<Shader> OpenGLRenderer::LoadShaderWithId(std::string name, std::vector<std::pair<GLenum, std::string>>& shaderDatas, std::string uuid)
{
	std::shared_ptr<Shader> shader = LoadShaderInternal(name, shaderDatas);
	shaders.push_back(shader);
	shader->uuid.Init(uuid);
	shaderMap[shader->uuid] = shaders.size() - 1;
	return shader;
}

std::shared_ptr<Shader> OpenGLRenderer::LoadShaderInternal(std::string name, std::vector<std::pair<GLenum, std::string>>& shaderDatas)
{
	std::shared_ptr<Shader> shader = std::make_shared<Shader>();
	shader->name = name;
	for (auto& [type, filepath] : shaderDatas)
	{
		shader->LoadShaderFromFile(type, filepath);
	}
	return shader;
}


void OpenGLRenderer::LinkShaders()
{
	for (auto& shader : shaders)
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

std::shared_ptr<Material> OpenGLRenderer::LoadMaterialInternal(std::shared_ptr<Shader> shader, std::string name, std::vector<Material::MaterialAttribute> attributes)
{
	std::shared_ptr<Material> material = std::make_shared<Material>();
	material->Init(name, shader);
	material->parameters = attributes;
	return material;
}

std::shared_ptr<MaterialInstance> OpenGLRenderer::LoadMaterialInstanceInternal(std::shared_ptr<Material> material, std::string name, bool modifiable)
{
	std::shared_ptr<MaterialInstance> mi = std::make_shared<MaterialInstance>(material, name, modifiable);
	return mi;
}

std::shared_ptr<MaterialInstance> OpenGLRenderer::LoadMaterialInstance(std::shared_ptr<Material> material, std::string name, bool modifiable)
{
	std::shared_ptr<MaterialInstance> mi = LoadMaterialInstanceInternal(material, name, modifiable);
	mi->uuid.Init();
	materialInstances.push_back(mi);
	miMap[mi->uuid] = materialInstances.size() - 1;
	return mi;
}

std::shared_ptr<MaterialInstance> OpenGLRenderer::LoadMaterialInstanceWithId(std::shared_ptr<Material> material, std::string name, bool modifiable, std::string uuid)
{
	std::shared_ptr<MaterialInstance> mi = LoadMaterialInstanceInternal(material, name, modifiable);
	mi->uuid.Init(uuid);
	materialInstances.push_back(mi);
	miMap[mi->uuid] = materialInstances.size() - 1;
	return mi;
}

std::shared_ptr<Material> OpenGLRenderer::LoadMaterial(std::shared_ptr<Shader> shader, std::string name, std::vector<Material::MaterialAttribute> attributes)
{
	std::shared_ptr<Material> material = LoadMaterialInternal(shader, name, attributes);
	material->uuid.Init();
	materials.push_back(material);
	materialMap[material->uuid] = materials.size() - 1;
	return material;
}

std::shared_ptr<Material> OpenGLRenderer::LoadMaterialWithId(std::shared_ptr<Shader> shader, std::string name, std::vector<Material::MaterialAttribute> attributes, std::string uuid)
{
	std::shared_ptr<Material> material = LoadMaterialInternal(shader, name, attributes);
	material->uuid.Init(uuid);
	materials.push_back(material);
	materialMap[material->uuid] = materials.size() - 1;
	return material;
}

std::shared_ptr<Texture> OpenGLRenderer::GetTexture(std::string uuid)
{
	return textures[textureMap[uuid]];
}

std::shared_ptr<Shader> OpenGLRenderer::GetShader(std::string uuid)
{
	return shaders[shaderMap[uuid]];
}

std::shared_ptr<Material> OpenGLRenderer::GetMaterial(std::string uuid)
{
	if (materialMap.find(uuid) != materialMap.end())
	{
		return materials[materialMap[uuid]];
	}
	return nullptr;
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

std::shared_ptr<Texture> OpenGLRenderer::LoadTextureInternal(std::string name, std::string path)
{
	int width, height, channels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 4);
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);

	std::shared_ptr<Texture> texture = std::make_shared<Texture>();

	texture->id = textureID;
	texture->size = { width, height };
	texture->name = name;
	texture->filepath = path;
	return texture;
}


void Material::Init(std::shared_ptr<Shader> shader)
{
	Init("New Material", shader);
}

void Material::Init(std::string name, std::shared_ptr<Shader> shader)
{
	this->name = name;
	this->shader = shader;
}

int Material::Find(std::string name)
{
	for (int i = 0; i < parameters.size(); i++)
	{
		if (parameters[i].name == name) return i;
	}
	return -1;
}

void Material::Set(MaterialAttribute attribute)
{
	int index = Find(attribute.name);
	if (index > -1)
	{
		parameters[index] = attribute;
	}
	else
	{
		parameters.push_back(attribute);
	}
}

void Material::Set(std::string name, std::shared_ptr<Texture> textureValue)
{
	int index = Find(name);
	if (index == -1)
	{
		MaterialAttribute attribute = {};
		attribute.name = name;
		attribute.type = MaterialAttributeType::Texture;
		attribute.textureValue = textureValue;
		parameters.push_back(attribute);
	}
	else
	{
		parameters[index].type = MaterialAttributeType::Texture;
		parameters[index].textureValue = textureValue;
	}
}

void Material::Set(std::string name, float floatValue)
{
	int index = Find(name);
	if (index == -1)
	{
		MaterialAttribute attribute = {};
		attribute.name = name;
		attribute.type = MaterialAttributeType::Float;
		attribute.floatValue = floatValue;
		parameters.push_back(attribute);
	}
	else
	{
		parameters[index].type = MaterialAttributeType::Float;
		parameters[index].floatValue = floatValue;
	}
}

void Material::Set(std::string name, int intValue)
{
	int index = Find(name);
	if (index == -1)
	{
		MaterialAttribute attribute = {};
		attribute.name = name;
		attribute.type = MaterialAttributeType::Int;
		attribute.intValue = intValue;
		parameters.push_back(attribute);
	}
	else
	{
		parameters[index].type = MaterialAttributeType::Int;
		parameters[index].intValue = intValue;
	}
}

void Material::Set(std::string name, glm::vec2 vector2Value)
{
	int index = Find(name);
	if (index == -1)
	{
		MaterialAttribute attribute = {};
		attribute.name = name;
		attribute.type = MaterialAttributeType::Vector2;
		attribute.vector2Value = vector2Value;
		parameters.push_back(attribute);
	}
	else
	{
		parameters[index].type = MaterialAttributeType::Vector2;
		parameters[index].vector2Value = vector2Value;
	}
}

void Material::Set(std::string name, glm::vec3 vector3Value)
{
	int index = Find(name);
	if (index == -1)
	{
		MaterialAttribute attribute = {};
		attribute.name = name;
		attribute.type = MaterialAttributeType::Vector3;
		attribute.vector3Value = vector3Value;
		parameters.push_back(attribute);
	}
	else
	{
		parameters[index].type = MaterialAttributeType::Vector3;
		parameters[index].vector3Value = vector3Value;
	}
}

void Material::Set(std::string name, glm::vec4 vector4Value)
{
	int index = Find(name);
	if (index == -1)
	{
		MaterialAttribute attribute = {};
		attribute.name = name;
		attribute.type = MaterialAttributeType::Vector4;
		attribute.vector4Value = vector4Value;
		parameters.push_back(attribute);
	}
	else
	{
		parameters[index].type = MaterialAttributeType::Vector4;
		parameters[index].vector4Value = vector4Value;
	}
}

void Material::Set(std::string name, glm::mat4 mat4Value)
{
	int index = Find(name);
	if (index == -1)
	{
		MaterialAttribute attribute = {};
		attribute.name = name;
		attribute.type = MaterialAttributeType::Mat4;
		attribute.mat4Value = mat4Value;
		parameters.push_back(attribute);
	}
	else
	{
		parameters[index].type = MaterialAttributeType::Mat4;
		parameters[index].mat4Value = mat4Value;
	}
}

void Material::Bind()
{
	OpenGLRenderer& renderer = OpenGLRenderer::GetSingleton();
	shader.lock()->Use();
	int textureIndex = 1;
	for (int i = 0; i < parameters.size(); i++)
	{
		std::string name = parameters[i].name;
		switch (parameters[i].type)
		{
		case MaterialAttributeType::Texture:
		{
			//shader.lock()->SetUniform(name, textureIndex);
			renderer.BindTexture(shader.lock(), name, parameters[i].textureValue.lock(), textureIndex);
			textureIndex++;
			break;
		}
		case MaterialAttributeType::Float:
			shader.lock()->SetUniform(name, parameters[i].floatValue);
			break;
		case MaterialAttributeType::Int:
		case MaterialAttributeType::Bool:
			shader.lock()->SetUniform(name, parameters[i].intValue);
			break;
		case MaterialAttributeType::Vector2:
			shader.lock()->SetUniform(name, parameters[i].vector2Value, 1);
			break;
		case MaterialAttributeType::Vector3:
		case MaterialAttributeType::Color:
			shader.lock()->SetUniform(name, parameters[i].vector3Value, 1);
			break;
		case MaterialAttributeType::Vector4:
			shader.lock()->SetUniform(name, parameters[i].vector4Value, 1);
			break;
		case MaterialAttributeType::Mat4:
			shader.lock()->SetUniform(name, parameters[i].mat4Value, 1);
			break;
		default:
			break;
		}
	}
}

std::vector<std::pair<std::string, Material::MaterialAttributeType>> Material::attributeNames =
{
	{ "Texture", Material::MaterialAttributeType::Texture },
	{ "Float", Material::MaterialAttributeType::Float },
	{ "Int", Material::MaterialAttributeType::Int },
	{ "Vector2", Material::MaterialAttributeType::Vector2 },
	{ "Vector3", Material::MaterialAttributeType::Vector3 },
	{ "Vector4", Material::MaterialAttributeType::Vector4 },
	{ "Mat4", Material::MaterialAttributeType::Mat4 },
	{ "Color", Material::MaterialAttributeType::Color },
	{ "Bool", Material::MaterialAttributeType::Bool }
};

std::map<std::string, Material::MaterialAttributeType> Material::attributeNameMap =
{
	{ Material::attributeNames[0].first, Material::attributeNames[0].second },
	{ Material::attributeNames[1].first, Material::attributeNames[1].second },
	{ Material::attributeNames[2].first, Material::attributeNames[2].second },
	{ Material::attributeNames[3].first, Material::attributeNames[3].second },
	{ Material::attributeNames[4].first, Material::attributeNames[4].second },
	{ Material::attributeNames[5].first, Material::attributeNames[5].second },
	{ Material::attributeNames[6].first, Material::attributeNames[6].second },
	{ Material::attributeNames[7].first, Material::attributeNames[7].second },
	{ Material::attributeNames[8].first, Material::attributeNames[8].second }
};

std::map<Material::MaterialAttributeType, std::string> Material::attributeTypeMap =
{
	{ Material::attributeNames[0].second, Material::attributeNames[0].first },
	{ Material::attributeNames[1].second, Material::attributeNames[1].first },
	{ Material::attributeNames[2].second, Material::attributeNames[2].first },
	{ Material::attributeNames[3].second, Material::attributeNames[3].first },
	{ Material::attributeNames[4].second, Material::attributeNames[4].first },
	{ Material::attributeNames[5].second, Material::attributeNames[5].first },
	{ Material::attributeNames[6].second, Material::attributeNames[6].first },
	{ Material::attributeNames[7].second, Material::attributeNames[7].first },
	{ Material::attributeNames[8].second, Material::attributeNames[8].first }
};