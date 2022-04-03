#include "SceneSerialiser.h"
#include "Scene.h"
#include "Shader.h"
#include <fstream>

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
    for (auto [uuid, entity] : scene.lock()->entities)
    {
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

        out << YAML::Key << "Geometries" << YAML::Value;
        out << YAML::Flow;
        out << YAML::BeginSeq;
        for (int i = 0; i < meshRenderer->geometryIndices.size(); i++)
        {
            out << meshRenderer->geometryIndices[i];
        }
        out << YAML::EndSeq;

        out << YAML::Key << "Materials" << YAML::Value;
        out << YAML::Flow;
        out << YAML::BeginSeq;
        for (int i = 0; i < meshRenderer->materialIndices.size(); i++)
        {
            out << meshRenderer->materialIndices[i];
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
        scene->AddEntity(entity);
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
    std::vector<int> geometries = node["Geometries"].as<std::vector<int>>();
    std::vector<int> materials = node["Materials"].as<std::vector<int>>();
    std::shared_ptr<MeshRendererComponent> meshRenderer = std::make_shared<MeshRendererComponent>();
    meshRenderer->geometryIndices = geometries;
    meshRenderer->materialIndices = materials;
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

bool MaterialSerialiser::SerialiseAttribute(YAML::Emitter& out, Material::MaterialAttribute& attribute)
{
    out << YAML::BeginMap;
    out << YAML::Key << "Name" << YAML::Value << attribute.name;
    switch (attribute.type)
    {
    case Material::MaterialAttributeType::Texture:
    {
        OpenGLRenderer& renderer = OpenGLRenderer::GetSingleton();
        out << YAML::Key << "Type" << YAML::Value << "Texture";
        out << YAML::Key << "Value" << YAML::Value << attribute.textureValue.lock()->uuid;
        break;
    }
    case Material::MaterialAttributeType::Float:
    {
        out << YAML::Key << "Type" << YAML::Value << "Float";
        out << YAML::Key << "Value" << YAML::Value << attribute.floatValue;
        break;
    }
    case Material::MaterialAttributeType::Int:
    {
        out << YAML::Key << "Type" << YAML::Value << "Int";
        out << YAML::Key << "Value" << YAML::Value << attribute.intValue;
        break;
    }
    case Material::MaterialAttributeType::Vector2:
    {
        out << YAML::Key << "Type" << YAML::Value << "Vector2";
        out << YAML::Key << "Value" << YAML::Value << attribute.vector2Value;
        break;
    }
    case Material::MaterialAttributeType::Vector3:
    {
        out << YAML::Key << "Type" << YAML::Value << "Vector3";
        out << YAML::Key << "Value" << YAML::Value << attribute.vector3Value;
        break;
    }
    case Material::MaterialAttributeType::Vector4:
    {
        out << YAML::Key << "Type" << YAML::Value << "Vector4";
        out << YAML::Key << "Value" << YAML::Value << attribute.vector4Value;
        break;
    }
    case Material::MaterialAttributeType::Mat4:
    {
        out << YAML::Key << "Type" << YAML::Value << "Mat4";
        out << YAML::Key << "Value" << YAML::Value << attribute.mat4Value;
        break;
    }
    case Material::MaterialAttributeType::Color:
    {
        out << YAML::Key << "Type" << YAML::Value << "Color";
        out << YAML::Key << "Value" << YAML::Value << attribute.vector3Value;
        break;
    }
    case Material::MaterialAttributeType::Bool:
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


    OpenGLRenderer& renderer = OpenGLRenderer::GetSingleton();
    std::string materialName = materialNode.as<std::string>();
    std::string uuid = uuidNode.as<std::string>();
    std::string shaderId = shaderNode.as<std::string>();
    std::shared_ptr<Shader> shader = renderer.GetShader(shaderId);

    std::vector<Material::MaterialAttribute> attributes;
    for (int i = 0; i < attributeNodes.size(); i++)
    {
        YAML::Node attributeNode = attributeNodes[i];
        attributes.push_back(DeserialiseAttribute(attributeNode));
    }

    renderer.LoadMaterialWithId(shader, materialName, attributes, uuid);
}

Material::MaterialAttribute MaterialSerialiser::DeserialiseAttribute(YAML::Node& node)
{
    OpenGLRenderer& renderer = OpenGLRenderer::GetSingleton();
    YAML::Node nameNode = node["Name"];
    YAML::Node typeNode = node["Type"];
    YAML::Node valueNode = node["Value"];
    std::string attributeName = nameNode.as<std::string>();
    std::string typeName = typeNode.as<std::string>();
    Material::MaterialAttributeType type = Material::attributeNameMap[typeName];

    switch (type)   
    {
    case Material::MaterialAttributeType::Texture:
    {
        std::string textureId = valueNode.as<std::string>();
        return Material::MaterialAttribute(attributeName, renderer.GetTexture(textureId));
    }
    case Material::MaterialAttributeType::Float:
    {
        float floatValue = valueNode.as<float>();
        return Material::MaterialAttribute(attributeName, floatValue);
    }
    case Material::MaterialAttributeType::Int:
    {
        int intValue = valueNode.as<int>();
        return Material::MaterialAttribute(attributeName, intValue);
    }
    case Material::MaterialAttributeType::Vector2:
    {
        glm::vec2 vec2Value = valueNode.as<glm::vec2>();
        return Material::MaterialAttribute(attributeName, vec2Value);
    }
    case Material::MaterialAttributeType::Vector3:
    {
        glm::vec3 vec3Value = valueNode.as<glm::vec3>();
        return Material::MaterialAttribute(attributeName, vec3Value);
    }
    case Material::MaterialAttributeType::Vector4:
    {
        glm::vec4 vec4Value = valueNode.as<glm::vec4>();
        return Material::MaterialAttribute(attributeName, vec4Value);
    }
    case Material::MaterialAttributeType::Mat4:
    {
        glm::mat4 mat4Value = valueNode.as<glm::mat4>();
        return Material::MaterialAttribute(attributeName, mat4Value);
    }
    case Material::MaterialAttributeType::Color:
    {
        glm::vec3 vec3Value = valueNode.as<glm::vec3>();
        return Material::MaterialAttribute(attributeName, vec3Value, Material::MaterialAttributeType::Color);
    }
    case Material::MaterialAttributeType::Bool:
    {
        // store as int init as bool
        bool boolValue = valueNode.as<int>();
        return Material::MaterialAttribute(attributeName, boolValue);
    }
    default:
        return Material::MaterialAttribute();
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

    std::string path = folder + instance.lock()->name + ".mi";
    std::ofstream fout(path);
    fout << out.c_str();
    fout.close();
    return true;
}

bool MaterialInstanceSerialiser::SerialiseAttribute(YAML::Emitter& out, Material::MaterialAttribute& attribute)
{
    out << YAML::BeginMap;
    out << YAML::Key << "Name" << YAML::Value << attribute.name;
    out << YAML::Key << "Type" << YAML::Value << Material::attributeTypeMap[attribute.type];
    switch (attribute.type)
    {
    case Material::MaterialAttributeType::Texture:
    {
        out << YAML::Key << "Value" << YAML::Value << attribute.textureValue.lock()->uuid;
        break;
    }
    case Material::MaterialAttributeType::Float:
    {
        out << YAML::Key << "Value" << YAML::Value << attribute.floatValue;
        break;
    }
    case Material::MaterialAttributeType::Bool:
    case Material::MaterialAttributeType::Int:
    {
        out << YAML::Key << "Value" << YAML::Value << attribute.intValue;
        break;
    }
    case Material::MaterialAttributeType::Vector2:
    {
        out << YAML::Key << "Value" << YAML::Value << attribute.vector2Value;
        break;
    }
    case Material::MaterialAttributeType::Vector3:
    case Material::MaterialAttributeType::Color:
    {
        out << YAML::Key << "Value" << YAML::Value << attribute.vector3Value;
        break;
    }
    case Material::MaterialAttributeType::Vector4:
    {
        out << YAML::Key << "Value" << YAML::Value << attribute.vector4Value;
        break;
    }
    case Material::MaterialAttributeType::Mat4:
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
    OpenGLRenderer& renderer = OpenGLRenderer::GetSingleton();
    YAML::Node file = YAML::LoadFile(path);
    YAML::Node materialInstanceNode = file["Material Instance"];
    YAML::Node materialNode = file["Material"];
    YAML::Node uuidNode = file["UUID"];
    YAML::Node modifiableNode = file["Modifiable"];
    YAML::Node attributeNodes = file["Attributes"];

    std::string uuid = uuidNode.as<std::string>();
    std::string baseMaterialId = materialNode.as<std::string>();
    std::shared_ptr<Material> baseMaterial = renderer.GetMaterial(baseMaterialId);
    std::string name = materialInstanceNode.as<std::string>();
    
    bool modifiable = modifiableNode.as<bool>();

    std::shared_ptr<MaterialInstance> mi = renderer.LoadMaterialInstanceWithId(baseMaterial, name, modifiable, uuid);

    std::vector<Material::MaterialAttribute> attributes;
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

Material::MaterialAttribute MaterialInstanceSerialiser::DeserialiseAttribute(YAML::Node& node)
{
    OpenGLRenderer& renderer = OpenGLRenderer::GetSingleton();
    YAML::Node nameNode = node["Name"];
    YAML::Node typeNode = node["Type"];
    YAML::Node valueNode = node["Value"];
    std::string attributeName = nameNode.as<std::string>();
    std::string typeName = typeNode.as<std::string>();
    Material::MaterialAttributeType type = Material::attributeNameMap[typeName];

    switch (type)
    {
    case Material::MaterialAttributeType::Texture:
    {
        std::string textureId = valueNode.as<std::string>();
        return Material::MaterialAttribute(attributeName, renderer.GetTexture(textureId));
    }
    case Material::MaterialAttributeType::Float:
    {
        float floatValue = valueNode.as<float>();
        return Material::MaterialAttribute(attributeName, floatValue);
    }
    case Material::MaterialAttributeType::Int:
    {
        int intValue = valueNode.as<int>();
        return Material::MaterialAttribute(attributeName, intValue);
    }
    case Material::MaterialAttributeType::Vector2:
    {
        glm::vec2 vec2Value = valueNode.as<glm::vec2>();
        return Material::MaterialAttribute(attributeName, vec2Value);
    }
    case Material::MaterialAttributeType::Vector3:
    {
        glm::vec3 vec3Value = valueNode.as<glm::vec3>();
        return Material::MaterialAttribute(attributeName, vec3Value);
    }
    case Material::MaterialAttributeType::Vector4:
    {
        glm::vec4 vec4Value = valueNode.as<glm::vec4>();
        return Material::MaterialAttribute(attributeName, vec4Value);
    }
    case Material::MaterialAttributeType::Mat4:
    {
        glm::mat4 mat4Value = valueNode.as<glm::mat4>();
        return Material::MaterialAttribute(attributeName, mat4Value);
    }
    case Material::MaterialAttributeType::Color:
    {
        glm::vec3 vec3Value = valueNode.as<glm::vec3>();
        return Material::MaterialAttribute(attributeName, vec3Value, Material::MaterialAttributeType::Color);
    }
    case Material::MaterialAttributeType::Bool:
    {
        // store as int init as bool
        bool boolValue = valueNode.as<int>();
        return Material::MaterialAttribute(attributeName, boolValue);
    }
    default:
        return Material::MaterialAttribute();
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
    OpenGLRenderer& renderer = OpenGLRenderer::GetSingleton();
    YAML::Node file = YAML::LoadFile(path);
    YAML::Node textureNode = file["Texture"];
    YAML::Node uuidNode = file["UUID"];
    YAML::Node filepathNode = file["Filepath"];
    std::string textureName = textureNode.as<std::string>();
    std::string uuid = uuidNode.as<std::string>();
    std::string filepath = filepathNode.as<std::string>();
    renderer.LoadTextureWithId(textureName, filepath, uuid);
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
    OpenGLRenderer& renderer = OpenGLRenderer::GetSingleton();

    std::vector<std::pair<GLuint, std::string>> shaderDatas;
    for (int i = 0; i < dataNodes.size(); i++)
    {
        YAML::Node dataNode = dataNodes[i];
        shaderDatas.push_back(DeserialiseShaderData(shaderName, dataNode));
    }
    renderer.LoadShaderWithId(shaderName, shaderDatas, uuid);
}

std::pair<GLuint, std::string> ShaderSerialiser::DeserialiseShaderData(const std::string& name, YAML::Node& data)
{
    YAML::Node typeNode = data["Type"];
    YAML::Node filepathNode = data["Filepath"];
    GLuint type = typeNode.as<GLuint>();
    std::string filepath = filepathNode.as<std::string>();
    return { type, filepath };
}
