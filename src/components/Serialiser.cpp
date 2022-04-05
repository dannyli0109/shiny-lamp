#include "Serialiser.h"
#include "Scene.h"
#include "Shader.h"
#include <fstream>
#include "ResourceManager.h"
#include "Resource.h"


namespace YAML {
    template <>
    struct convert<glm::vec2> {
        static Node encode(const glm::vec2& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            return node;
        }

        static bool decode(const Node& node, glm::vec2& rhs) {
            if (!node.IsSequence() || node.size() != 2) {
                return false;
            }

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            return true;
        }
    };

    template <>
    struct convert<glm::vec3> {
        static Node encode(const glm::vec3& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            return node;
        }

        static bool decode(const Node& node, glm::vec3& rhs) {
            if (!node.IsSequence() || node.size() != 3) {
                return false;
            }

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            return true;
        }
    };

    template <>
    struct convert<glm::vec4> {
        static Node encode(const glm::vec4& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.push_back(rhs.w);
            return node;
        }

        static bool decode(const Node& node, glm::vec4& rhs) {
            if (!node.IsSequence() || node.size() != 4) {
                return false;
            }
            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            rhs.w = node[3].as<float>();
            return true;
        }
    };

    template <>
    struct convert<glm::mat4> {
        static Node encode(const glm::mat4& rhs) {
            Node node;
            for (int i = 0; i < rhs.length(); i++) {
                for (int j = 0; j < rhs[i].length(); j++) {
                    node.push_back(rhs[i][j]);
                }
            }
            return node;
        }

        static bool decode(const Node& node, glm::mat4& rhs) {
            if (!node.IsSequence() || node.size() != 16) {
                return false;
            }

            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    rhs[i][j] = node[i * 4 + j].as<float>();
                }
            }
            return true;
        }
    };

    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v) {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v) {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v) {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::mat4& m) {
        out << YAML::Flow;
        out << YAML::BeginSeq;
        for (int i = 0; i < m.length(); i++) {
            for (int j = 0; j < m[i].length(); j++) {
                out << m[i][j];
            }
        }
        out << YAML::EndSeq;
        return out;
    }
}

SceneSerialiser::SceneSerialiser(std::shared_ptr<Scene> scene) : scene(scene)
{

}

bool SceneSerialiser::Serialise(std::string folder)
{
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "Scene" << YAML::Value << scene.lock()->name;
    out << YAML::Key << "Entities" << YAML::Value;
    
    out << YAML::BeginSeq;
    for (auto entity : scene.lock()->entities)
    {
        if (!entity) continue;
        SerialiseEntity(out, entity);
    }
    out << YAML::EndSeq;
    out << YAML::EndMap;

    std::string path = folder + scene.lock()->name + ".scene";
    std::ofstream fout(path);
    fout << out.c_str();
    fout.close();
    return true;
}

bool SceneSerialiser::SerialiseEntity(YAML::Emitter& out, std::shared_ptr<Entity> entity)
{
    out << YAML::BeginMap;
    out << YAML::Key << "Entity" << YAML::Value << entity->uuid;
    SerialiseTag(out, entity);
    SerialiseTransform(out, entity);
    SerialiseMeshRenderer(out, entity);
    out << YAML::EndMap;
    return true;
}

bool SceneSerialiser::SerialiseTag(YAML::Emitter& out, std::shared_ptr<Entity> entity)
{
    TagComponent* tag = entity->GetComponent<TagComponent>();
    if (tag)
    {
        out << YAML::Key << "Tag" << YAML::Value;
        out << YAML::BeginMap;
        out << YAML::Key << "Name" << YAML::Value << tag->name;
        out << YAML::EndMap;
        return true;
    }
    return false;
}

bool SceneSerialiser::SerialiseTransform(YAML::Emitter& out, std::shared_ptr<Entity> entity)
{
    TransformComponent* transform = entity->GetComponent<TransformComponent>();
    if (transform)
    {
        out << YAML::Key << "Transform" << YAML::Value;
        out << YAML::BeginMap;
        out << YAML::Key << "Translation" << YAML::Value << transform->translation;
        out << YAML::Key << "Rotation" << YAML::Value << transform->rotation;
        out << YAML::Key << "Scale" << YAML::Value << transform->scale;
        if (!transform->parent.expired())
        {
            out << YAML::Key << "Parent" << YAML::Value << transform->parent.lock()->uuid;
        }

        out << YAML::Key << "Children" << YAML::Value;
        out << YAML::Flow;
        out << YAML::BeginSeq;
        for (int i = 0; i < transform->children.size(); i++)
        {
            out << transform->children[i]->uuid;
        }
        out << YAML::EndSeq;
        out << YAML::EndMap;
        return true;
    }
    return false;
}

bool SceneSerialiser::SerialiseMeshRenderer(YAML::Emitter& out, std::shared_ptr<Entity> entity)
{
    MeshRendererComponent* meshRenderer = entity->GetComponent<MeshRendererComponent>();
    if (meshRenderer)
    {
        out << YAML::Key << "MeshRenderer" << YAML::Value;
        out << YAML::BeginMap;

        out << YAML::Key << "Meshes" << YAML::Value;
        out << YAML::Flow;
        out << YAML::BeginSeq;
        for (int i = 0; i < meshRenderer->meshes.size(); i++)
        {
            out << meshRenderer->meshes[i].lock()->uuid;
        }
        out << YAML::EndSeq;

        out << YAML::Key << "Materials" << YAML::Value;
        out << YAML::BeginSeq;
        OpenGLRenderer& renderer = OpenGLRenderer::GetSingleton();
        ResourceManager& resources = ResourceManager::GetSingleton();
        for (int i = 0; i < meshRenderer->materials.size(); i++)
        {
            out << meshRenderer->materials[i].lock()->uuid;
        }
        out << YAML::EndSeq;

        out << YAML::EndMap;
        return true;
    }
    return false;
}

std::shared_ptr<Scene> SceneSerialiser::Deserialise(std::string path)
{
    YAML::Node file = YAML::LoadFile(path);
    YAML::Node sceneNode = file["Scene"];
    YAML::Node entitiesNode = file["Entities"];

    std::shared_ptr<Scene> scene = std::make_shared<Scene>(sceneNode.as<std::string>());
    // create entity first to make sure the uuid reference can be found
    for (int i = 0; i < entitiesNode.size(); i++)
    {
        YAML::Node entityNode = entitiesNode[i];
        std::shared_ptr<Entity> entity = DeserialiseEntity(entityNode);
        scene->AddEntityInternal(entity);
    }

    for (int i = 0; i < entitiesNode.size(); i++)
    {
        YAML::Node entityNode = entitiesNode[i];
        DeserialiseComponents(entityNode, scene, scene->GetEntity(entityNode["Entity"].as<std::string>()));
    }
    //if (file[""])
    return scene;
}

std::shared_ptr<Entity> SceneSerialiser::DeserialiseEntity(YAML::Node & node)
{
    std::string uuid = node["Entity"].as<std::string>();
    std::shared_ptr<Entity> entity = std::make_shared<Entity>(uuid, nullptr, nullptr);
    return entity;
}

void SceneSerialiser::DeserialiseComponents(YAML::Node& node, std::shared_ptr<Scene> scene, std::shared_ptr<Entity> entity)
{
    std::shared_ptr<TagComponent> tag = DeserialiseTag(node["Tag"]);
    if (tag) entity->AddComponent(tag);
    std::shared_ptr<TransformComponent> tranform = DeserialiseTransform(node["Transform"], scene);
    if (tranform) entity->AddComponent(tranform);
    std::shared_ptr<MeshRendererComponent> meshRenderer = DeserialiseMeshRenderer(node["MeshRenderer"]);
    if (meshRenderer) entity->AddComponent(meshRenderer);
}

std::shared_ptr<TagComponent> SceneSerialiser::DeserialiseTag(YAML::Node& node)
{
    YAML::Node nameNode = node["Name"];
    if (nameNode)
    {
        return std::make_shared<TagComponent>(nameNode.as<std::string>());
    }
    return std::make_shared<TagComponent>();
}

std::shared_ptr<TransformComponent> SceneSerialiser::DeserialiseTransform(YAML::Node& node, std::shared_ptr<Scene> scene)
{
    if (!node.IsDefined()) return nullptr;
    YAML::Node translationNode = node["Translation"];
    YAML::Node rotationNode = node["Rotation"];
    YAML::Node scaleNode = node["Scale"];
    YAML::Node parentNode = node["Parent"];
    YAML::Node childrenNode = node["Children"];

    glm::vec3 translation = translationNode.as<glm::vec3>();
    glm::vec3 rotation = rotationNode.as<glm::vec3>();
    glm::vec3 scale = scaleNode.as<glm::vec3>();
    std::shared_ptr<TransformComponent> transform = std::make_shared<TransformComponent>(translation, rotation, scale);
    if (parentNode.IsDefined())
    {
        std::shared_ptr<Entity> parent = scene->GetEntity(parentNode.as<std::string>());
        transform->parent = parent;
    }
    for (int i = 0; i < childrenNode.size(); i++)
    {
        std::shared_ptr<Entity> child = scene->GetEntity(childrenNode[i].as<std::string>());
        transform->children.push_back(child);
    }
    return transform;
}

std::shared_ptr<MeshRendererComponent> SceneSerialiser::DeserialiseMeshRenderer(YAML::Node& node)
{
    if (!node.IsDefined()) return nullptr;
    //OpenGLRenderer& renderer = OpenGLRenderer::GetSingleton();
    ResourceManager& resources = ResourceManager::GetSingleton();
    std::vector<std::string> meshIds = node["Meshes"].as<std::vector<std::string>>();
    std::vector<std::string> materials = node["Materials"].as<std::vector<std::string>>();
    std::shared_ptr<MeshRendererComponent> meshRenderer = std::make_shared<MeshRendererComponent>();
    // getmesh
    for (int i = 0; i < meshIds.size(); i++)
    {
        meshRenderer->meshes.push_back(resources.GetMesh(meshIds[i]));
    }
    //meshRenderer->geometryIndices = geometries;
    for (int i = 0; i < materials.size(); i++)
    {
        meshRenderer->materials.push_back(resources.GetMaterialInstance(materials[i]));
    }
    return meshRenderer;
}

MaterialSerialiser::MaterialSerialiser(std::shared_ptr<Material> material)
{
    this->material = material;
}

bool MaterialSerialiser::Serialise(std::string folder)
{
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "Material" << YAML::Value << material.lock()->name;
    out << YAML::Key << "UUID" << YAML::Value << material.lock()->uuid;
    out << YAML::Key << "Shader" << YAML::Value << material.lock()->shader.lock()->uuid;
    out << YAML::Key << "Attributes" << YAML::Value;

    out << YAML::BeginSeq;
    for (auto& parameter : material.lock()->parameters)
    {
        SerialiseAttribute(out, parameter);
    }
    out << YAML::EndSeq;
    out << YAML::EndMap;

    std::string path = folder + material.lock()->name + ".material";
    std::ofstream fout(path);
    fout << out.c_str();
    fout.close();
    return true;
}

bool MaterialSerialiser::SerialiseAttribute(YAML::Emitter& out, MaterialAttribute& attribute)
{
    out << YAML::BeginMap;
    out << YAML::Key << "Name" << YAML::Value << attribute.name;
    switch (attribute.type)
    {
    case MaterialAttributeType::Texture:
    {
        OpenGLRenderer& renderer = OpenGLRenderer::GetSingleton();
        out << YAML::Key << "Type" << YAML::Value << "Texture";
        out << YAML::Key << "Value" << YAML::Value << attribute.textureValue.lock()->uuid;
        break;
    }
    case MaterialAttributeType::Float:
    {
        out << YAML::Key << "Type" << YAML::Value << "Float";
        out << YAML::Key << "Value" << YAML::Value << attribute.floatValue;
        break;
    }
    case MaterialAttributeType::Int:
    {
        out << YAML::Key << "Type" << YAML::Value << "Int";
        out << YAML::Key << "Value" << YAML::Value << attribute.intValue;
        break;
    }
    case MaterialAttributeType::Vector2:
    {
        out << YAML::Key << "Type" << YAML::Value << "Vector2";
        out << YAML::Key << "Value" << YAML::Value << attribute.vector2Value;
        break;
    }
    case MaterialAttributeType::Vector3:
    {
        out << YAML::Key << "Type" << YAML::Value << "Vector3";
        out << YAML::Key << "Value" << YAML::Value << attribute.vector3Value;
        break;
    }
    case MaterialAttributeType::Vector4:
    {
        out << YAML::Key << "Type" << YAML::Value << "Vector4";
        out << YAML::Key << "Value" << YAML::Value << attribute.vector4Value;
        break;
    }
    case MaterialAttributeType::Mat4:
    {
        out << YAML::Key << "Type" << YAML::Value << "Mat4";
        out << YAML::Key << "Value" << YAML::Value << attribute.mat4Value;
        break;
    }
    case MaterialAttributeType::Color:
    {
        out << YAML::Key << "Type" << YAML::Value << "Color";
        out << YAML::Key << "Value" << YAML::Value << attribute.vector3Value;
        break;
    }
    case MaterialAttributeType::Bool:
    {
        out << YAML::Key << "Type" << YAML::Value << "Bool";
        out << YAML::Key << "Value" << YAML::Value << attribute.intValue;
        break;
    }
    default:
        break;
    }
    out << YAML::EndMap;
    return true;
}

void MaterialSerialiser::Deserialise(std::string path)
{
    YAML::Node file = YAML::LoadFile(path);
    YAML::Node materialNode = file["Material"];
    YAML::Node uuidNode = file["UUID"];
    YAML::Node shaderNode = file["Shader"];
    YAML::Node attributeNodes = file["Attributes"];


    //OpenGLRenderer& renderer = OpenGLRenderer::GetSingleton();
    ResourceManager& resources = ResourceManager::GetSingleton();

    std::string materialName = materialNode.as<std::string>();
    std::string uuid = uuidNode.as<std::string>();
    std::string shaderId = shaderNode.as<std::string>();
    std::shared_ptr<Shader> shader = resources.GetShader(shaderId);

    std::vector<MaterialAttribute> attributes;
    for (int i = 0; i < attributeNodes.size(); i++)
    {
        YAML::Node attributeNode = attributeNodes[i];
        attributes.push_back(DeserialiseAttribute(attributeNode));
    }

    resources.LoadMaterialWithId(shader, materialName, attributes, uuid);
}

MaterialAttribute MaterialSerialiser::DeserialiseAttribute(YAML::Node& node)
{
    //OpenGLRenderer& renderer = OpenGLRenderer::GetSingleton();
    ResourceManager& resources = ResourceManager::GetSingleton();

    YAML::Node nameNode = node["Name"];
    YAML::Node typeNode = node["Type"];
    YAML::Node valueNode = node["Value"];
    std::string attributeName = nameNode.as<std::string>();
    std::string typeName = typeNode.as<std::string>();
    MaterialAttributeType type = MaterialAttribute::GetTypeFromName(typeName);

    switch (type)   
    {
    case MaterialAttributeType::Texture:
    {
        std::string textureId = valueNode.as<std::string>();
        return MaterialAttribute(attributeName, resources.GetTexture(textureId));
    }
    case MaterialAttributeType::Float:
    {
        float floatValue = valueNode.as<float>();
        return MaterialAttribute(attributeName, floatValue);
    }
    case MaterialAttributeType::Int:
    {
        int intValue = valueNode.as<int>();
        return MaterialAttribute(attributeName, intValue);
    }
    case MaterialAttributeType::Vector2:
    {
        glm::vec2 vec2Value = valueNode.as<glm::vec2>();
        return MaterialAttribute(attributeName, vec2Value);
    }
    case MaterialAttributeType::Vector3:
    {
        glm::vec3 vec3Value = valueNode.as<glm::vec3>();
        return MaterialAttribute(attributeName, vec3Value);
    }
    case MaterialAttributeType::Vector4:
    {
        glm::vec4 vec4Value = valueNode.as<glm::vec4>();
        return MaterialAttribute(attributeName, vec4Value);
    }
    case MaterialAttributeType::Mat4:
    {
        glm::mat4 mat4Value = valueNode.as<glm::mat4>();
        return MaterialAttribute(attributeName, mat4Value);
    }
    case MaterialAttributeType::Color:
    {
        glm::vec3 vec3Value = valueNode.as<glm::vec3>();
        return MaterialAttribute(attributeName, vec3Value, MaterialAttributeType::Color);
    }
    case MaterialAttributeType::Bool:
    {
        // store as int init as bool
        bool boolValue = valueNode.as<int>();
        return MaterialAttribute(attributeName, boolValue);
    }
    default:
        return MaterialAttribute();
    }    
}


MaterialInstanceSerialiser::MaterialInstanceSerialiser(std::shared_ptr<MaterialInstance> instance)
{
    this->instance = instance;
}

bool MaterialInstanceSerialiser::Serialise(std::string folder)
{
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "Material Instance" << YAML::Value << instance.lock()->name;
    out << YAML::Key << "UUID" << YAML::Value << instance.lock()->uuid;
    out << YAML::Key << "Material" << YAML::Value << instance.lock()->base.lock()->uuid;
    out << YAML::Key << "Modifiable" << YAML::Value << instance.lock()->modifiable;
    out << YAML::Key << "Attributes" << YAML::Value;

    out << YAML::BeginSeq;
    for (auto& parameter : instance.lock()->parameters)
    {
        SerialiseAttribute(out, parameter);
    }
    out << YAML::EndSeq;
    out << YAML::EndMap;

    std::string path =  folder + instance.lock()->name + ".mi";
    std::ofstream fout(path);
    fout << out.c_str();
    fout.close();
    return true;
}

bool MaterialInstanceSerialiser::SerialiseAttribute(YAML::Emitter& out, MaterialAttribute& attribute)
{
    out << YAML::BeginMap;
    out << YAML::Key << "Name" << YAML::Value << attribute.name;
    out << YAML::Key << "Type" << YAML::Value << MaterialAttribute::GetNameFromType(attribute.type);
    switch (attribute.type)
    {
    case MaterialAttributeType::Texture:
    {
        out << YAML::Key << "Value" << YAML::Value << attribute.textureValue.lock()->uuid;
        break;
    }
    case MaterialAttributeType::Float:
    {
        out << YAML::Key << "Value" << YAML::Value << attribute.floatValue;
        break;
    }
    case MaterialAttributeType::Bool:
    case MaterialAttributeType::Int:
    {
        out << YAML::Key << "Value" << YAML::Value << attribute.intValue;
        break;
    }
    case MaterialAttributeType::Vector2:
    {
        out << YAML::Key << "Value" << YAML::Value << attribute.vector2Value;
        break;
    }
    case MaterialAttributeType::Vector3:
    case MaterialAttributeType::Color:
    {
        out << YAML::Key << "Value" << YAML::Value << attribute.vector3Value;
        break;
    }
    case MaterialAttributeType::Vector4:
    {
        out << YAML::Key << "Value" << YAML::Value << attribute.vector4Value;
        break;
    }
    case MaterialAttributeType::Mat4:
    {
        out << YAML::Key << "Value" << YAML::Value << attribute.mat4Value;
        break;
    }
    default:
        break;
    }
    out << YAML::EndMap;
    return true;
}

void MaterialInstanceSerialiser::Deserialise(std::string path)
{
    //OpenGLRenderer& renderer = OpenGLRenderer::GetSingleton();
    ResourceManager& resources = ResourceManager::GetSingleton();

    YAML::Node file = YAML::LoadFile(path);
    YAML::Node materialInstanceNode = file["Material Instance"];
    YAML::Node materialNode = file["Material"];
    YAML::Node uuidNode = file["UUID"];
    YAML::Node modifiableNode = file["Modifiable"];
    YAML::Node attributeNodes = file["Attributes"];

    std::string uuid = uuidNode.as<std::string>();
    std::string baseMaterialId = materialNode.as<std::string>();
    std::shared_ptr<Material> baseMaterial = resources.GetMaterial(baseMaterialId);
    std::string name = materialInstanceNode.as<std::string>();
    
    bool modifiable = modifiableNode.as<bool>();

    std::shared_ptr<MaterialInstance> mi = resources.LoadMaterialInstanceWithId(baseMaterial, name, modifiable, uuid);

    std::vector<MaterialAttribute> attributes;
    for (int i = 0; i < attributeNodes.size(); i++)
    {
        YAML::Node attributeNode = attributeNodes[i];
        attributes.push_back(DeserialiseAttribute(attributeNode));
    }

    for (int i = 0; i < attributes.size(); i++)
    {
        mi->Set(attributes[i]);
    }
}

MaterialAttribute MaterialInstanceSerialiser::DeserialiseAttribute(YAML::Node& node)
{
    //OpenGLRenderer& renderer = OpenGLRenderer::GetSingleton();
    ResourceManager& resources = ResourceManager::GetSingleton();

    YAML::Node nameNode = node["Name"];
    YAML::Node typeNode = node["Type"];
    YAML::Node valueNode = node["Value"];
    std::string attributeName = nameNode.as<std::string>();
    std::string typeName = typeNode.as<std::string>();
    MaterialAttributeType type = MaterialAttribute::GetTypeFromName(typeName);

    switch (type)
    {
    case MaterialAttributeType::Texture:
    {
        std::string textureId = valueNode.as<std::string>();
        return MaterialAttribute(attributeName, resources.GetTexture(textureId));
    }
    case MaterialAttributeType::Float:
    {
        float floatValue = valueNode.as<float>();
        return MaterialAttribute(attributeName, floatValue);
    }
    case MaterialAttributeType::Int:
    {
        int intValue = valueNode.as<int>();
        return MaterialAttribute(attributeName, intValue);
    }
    case MaterialAttributeType::Vector2:
    {
        glm::vec2 vec2Value = valueNode.as<glm::vec2>();
        return MaterialAttribute(attributeName, vec2Value);
    }
    case MaterialAttributeType::Vector3:
    {
        glm::vec3 vec3Value = valueNode.as<glm::vec3>();
        return MaterialAttribute(attributeName, vec3Value);
    }
    case MaterialAttributeType::Vector4:
    {
        glm::vec4 vec4Value = valueNode.as<glm::vec4>();
        return MaterialAttribute(attributeName, vec4Value);
    }
    case MaterialAttributeType::Mat4:
    {
        glm::mat4 mat4Value = valueNode.as<glm::mat4>();
        return MaterialAttribute(attributeName, mat4Value);
    }
    case MaterialAttributeType::Color:
    {
        glm::vec3 vec3Value = valueNode.as<glm::vec3>();
        return MaterialAttribute(attributeName, vec3Value, MaterialAttributeType::Color);
    }
    case MaterialAttributeType::Bool:
    {
        // store as int init as bool
        bool boolValue = valueNode.as<int>();
        return MaterialAttribute(attributeName, boolValue);
    }
    default:
        return MaterialAttribute();
    }
}

TextureSerialiser::TextureSerialiser(std::shared_ptr<Texture> texture)
{
    this->texture = texture;
}

bool TextureSerialiser::Serialise(std::string folder)
{
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "Texture" << YAML::Value << texture.lock()->name;
    out << YAML::Key << "UUID" << YAML::Value << texture.lock()->uuid;
    out << YAML::Key << "Filepath" << YAML::Value << texture.lock()->filepath;
    out << YAML::EndMap;

    std::string path = folder + texture.lock()->name + ".texture";
    std::ofstream fout(path);
    fout << out.c_str();
    fout.close();
    return true;
}

void TextureSerialiser::Deserialise(std::string path)
{
    /*
        Texture: black
        UUID: 89b9c788-c8fd-4de3-ab77-34e141cbdb31
        Filepath: black.png
    */
    //OpenGLRenderer& renderer = OpenGLRenderer::GetSingleton();
    ResourceManager& resources = ResourceManager::GetSingleton();

    YAML::Node file = YAML::LoadFile(path);
    YAML::Node textureNode = file["Texture"];
    YAML::Node uuidNode = file["UUID"];
    YAML::Node filepathNode = file["Filepath"];
    std::string textureName = textureNode.as<std::string>();
    std::string uuid = uuidNode.as<std::string>();
    std::string filepath = filepathNode.as<std::string>();
    resources.LoadTextureWithId(textureName, filepath, uuid);
}

ShaderSerialiser::ShaderSerialiser(std::shared_ptr<Shader> shader)
{
    this->shader = shader;
}

bool ShaderSerialiser::Serialise(std::string folder)
{
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "Shader" << YAML::Value << shader.lock()->name;
    out << YAML::Key << "UUID" << YAML::Value << shader.lock()->uuid;
    out << YAML::Key << "Data" << YAML::Value;
    out << YAML::BeginSeq;
    for (auto& [type, data] : shader.lock()->data)
    {
        SerialiseShaderData(out, type, data);
    }
    out << YAML::EndSeq;
    out << YAML::EndMap;

    std::string path = folder + shader.lock()->name + ".shader";
    std::ofstream fout(path);
    fout << out.c_str();
    fout.close();
    return true;
}

bool ShaderSerialiser::SerialiseShaderData(YAML::Emitter& out, GLuint type, ShaderData& data)
{
    out << YAML::BeginMap;
    out << YAML::Key << "Type" << YAML::Value << type;
    out << YAML::Key << "Filepath" << YAML::Value << data.filepath;
    out << YAML::EndMap;
    return true;
}

void ShaderSerialiser::Deserialise(std::string path)
{
    YAML::Node file = YAML::LoadFile(path);
    YAML::Node shaderNode = file["Shader"];
    YAML::Node uuidNode = file["UUID"];
    YAML::Node dataNodes = file["Data"];
    std::string shaderName = shaderNode.as<std::string>();
    std::string uuid = uuidNode.as<std::string>();
    //OpenGLRenderer& renderer = OpenGLRenderer::GetSingleton();
    ResourceManager& resources = ResourceManager::GetSingleton();


    std::vector<std::pair<GLuint, std::string>> shaderDatas;
    for (int i = 0; i < dataNodes.size(); i++)
    {
        YAML::Node dataNode = dataNodes[i];
        shaderDatas.push_back(DeserialiseShaderData(shaderName, dataNode));
    }
    resources.LoadShaderWithId(shaderName, shaderDatas, uuid);
}

std::pair<GLuint, std::string> ShaderSerialiser::DeserialiseShaderData(const std::string& name, YAML::Node& data)
{
    YAML::Node typeNode = data["Type"];
    YAML::Node filepathNode = data["Filepath"];
    GLuint type = typeNode.as<GLuint>();
    std::string filepath = filepathNode.as<std::string>();
    return { type, filepath };
}

MeshSerialiser::MeshSerialiser(std::shared_ptr<Mesh> mesh)
{
    this->mesh = mesh;
}

bool MeshSerialiser::Serialise(std::string folder)
{
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "Mesh" << YAML::Value << mesh.lock()->name;
    out << YAML::Key << "UUID" << YAML::Value << mesh.lock()->uuid;
    out << YAML::Key << "Vertices" << YAML::Value;

    out << YAML::BeginSeq;
    std::vector<Vertex>& vertices = mesh.lock()->data->vertices;
    for (int i = 0; i < vertices.size(); i++)
    {
        SerialiseVertex(out, vertices[i]);
    }
    out << YAML::EndSeq;

    std::vector<unsigned short>& indices = mesh.lock()->data->indices;
    out << YAML::Key << "Indices" << YAML::Value;
    out << YAML::Flow;
    out << indices;
    out << YAML::EndMap;
    std::string path = folder + mesh.lock()->name + ".mesh";
    std::ofstream fout(path);
    fout << out.c_str();
    fout.close();
    return true;
}

bool MeshSerialiser::SerialiseVertex(YAML::Emitter& out, Vertex& vertex)
{
    out << YAML::Flow;
    out << YAML::BeginSeq;
    out << vertex.position;
    out << vertex.normal;
    out << vertex.vertTangent;
    out << vertex.vertBitangent;
    out << vertex.color;
    out << vertex.uv;
    out << YAML::EndSeq;
    return true;
}

void MeshSerialiser::Deserialise(std::string path)
{
    YAML::Node file = YAML::LoadFile(path);
    YAML::Node meshNode = file["Mesh"];
    YAML::Node uuidNode = file["UUID"];
    YAML::Node vertexNodes = file["Vertices"];
    YAML::Node indexNodes = file["Indices"];

    std::string meshName = meshNode.as<std::string>();
    std::string uuid = uuidNode.as<std::string>();

    std::shared_ptr<MeshData> meshData = std::make_shared<MeshData>();

    for (int i = 0; i < vertexNodes.size(); i++)
    {
         meshData->vertices.push_back(DeserialiseVertex(vertexNodes[i]));
    }

    meshData->indices = indexNodes.as<std::vector<unsigned short>>();
    //OpenGLRenderer& renderer = OpenGLRenderer::GetSingleton();
    ResourceManager& resources = ResourceManager::GetSingleton();

    
    resources.LoadMeshWithId(meshName, meshData, uuid);

}

Vertex&& MeshSerialiser::DeserialiseVertex(YAML::Node& node)
{
    return Vertex(
        {
            node[0].as<glm::vec3>(),
            node[1].as<glm::vec3>(),
            node[2].as<glm::vec3>(),
            node[3].as<glm::vec3>(),
            node[4].as<glm::vec4>(),
            node[5].as<glm::vec2>()
        }
    );
}

ModelSerialiser::ModelSerialiser(std::shared_ptr<Model> model)
{
    this->model = model;
}

bool ModelSerialiser::Serialise(std::string folder)
{
    YAML::Emitter out;
    SerialiseModel(out, model.lock());
    std::string path = Util::GetValidFilename(folder + model.lock()->name, "model");
    std::ofstream fout(path);
    fout << out.c_str();
    fout.close();
    return true;
}

bool ModelSerialiser::SerialiseModel(YAML::Emitter& out, std::shared_ptr<Model> model)
{
    out << YAML::BeginMap;
    out << YAML::Key << "Model" << YAML::Value << model->name;
    out << YAML::Key << "UUID" << YAML::Value << model->uuid;
    out << YAML::Key << "Meshes" << YAML::Value;

    out << YAML::Flow;
    out << YAML::BeginSeq;
    std::vector<std::shared_ptr<Mesh>> meshes = model->meshes;
    for (int i = 0; i < meshes.size(); i++)
    {
        out << meshes[i]->uuid;
    }
    out << YAML::EndSeq;

    out << YAML::Key << "Children" << YAML::Value;
    out << YAML::BeginSeq;
    std::vector<std::shared_ptr<Model>> children = model->children;
    for (int i = 0; i < children.size(); i++)
    {
        SerialiseModel(out, children[i]);
    }
    out << YAML::EndSeq;
    out << YAML::EndMap;
    return true;
}

void ModelSerialiser::Deserialise(std::string path)
{
    //OpenGLRenderer& renderer = OpenGLRenderer::GetSingleton();
    ResourceManager& resources = ResourceManager::GetSingleton();

    YAML::Node file = YAML::LoadFile(path);
    std::shared_ptr<Model> model = DeserialiseModel(file);
    resources.models.push_back(model);
    //resources.modelMap[model->uuid] = renderer.models.size() - 1;
}

std::shared_ptr<Model> ModelSerialiser::DeserialiseModel(YAML::Node& node)
{
    //OpenGLRenderer& renderer = OpenGLRenderer::GetSingleton();
    ResourceManager& resources = ResourceManager::GetSingleton();

    YAML::Node modelNode = node["Model"];
    YAML::Node uuidNode = node["UUID"];
    YAML::Node meshNodes = node["Meshes"];
    YAML::Node childrenNodes = node["Children"];

    std::string modelName = modelNode.as<std::string>();
    std::string uuid = uuidNode.as<std::string>();

    //renderer.LoadModelWithId()
    std::shared_ptr<ModelData> modelData = std::make_shared<ModelData>();

    for (int i = 0; i < meshNodes.size(); i++)
    {
        modelData->meshes.push_back(resources.GetMesh(meshNodes[i].as<std::string>())->data);
    }

    std::shared_ptr<Model> model = resources.LoadModelWithId(modelName, modelData, uuid);
    for (int i = 0; i < childrenNodes.size(); i++)
    {
        model->children.push_back(DeserialiseModel(childrenNodes[i]));
    }
    return model;
}

