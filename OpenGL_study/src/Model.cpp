#include "Model.h"

Model::Model(const std::string& path)
    : path_(path)
{
    load_model(path);
}

Model::~Model() = default;

void Model::draw(const Renderer& renderer, Shader& shader)
{
    for (auto& meshe : meshes_)
        meshe.draw(renderer, shader);
}

void Model::load_model(const std::string& path)
{
    Assimp::Importer importer;
    // 后期指令参考 http://assimp.sourceforge.net/lib_html/postprocess_8h.html
    const auto scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "[ERROR] Load model fail, ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    directory_ = path.substr(0, path.find_last_of('/'));
    process_node(scene->mRootNode, scene);
}

void Model::process_node(aiNode* node, const aiScene* scene)
{
    for (size_t i = 0, count = node->mNumMeshes; i < count; i++)
    {
        const auto mesh = scene->mMeshes[node->mMeshes[i]];
        meshes_.push_back(process_mesh(mesh, scene));
    }

    for (size_t i = 0, count = node->mNumChildren; i < count; i++)
    {
        process_node(node->mChildren[i], scene);
    }
}

Mesh Model::process_mesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<TextureData> texture_datas;

    for (size_t i = 0, count = mesh->mNumVertices; i < count; i++)
    {
        Vertex vertex {};

        // 位置坐标
        vertex.position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
        // 法线向量
        vertex.normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
        // 纹理坐标，只关心第一组
        if (mesh->mTextureCoords[0])
            vertex.tex_coords = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
        else
            vertex.tex_coords = glm::vec2(0.0f);

        vertices.push_back(vertex);
    }

    for (size_t i = 0, count = mesh->mNumFaces; i < count; i++)
    {
        const auto face = mesh->mFaces[i];
        for (size_t j = 0, acount = face.mNumIndices; j < acount; j++)
            indices.push_back(face.mIndices[j]);
    }

    if (mesh->mMaterialIndex >= 0)
    {
        const auto material = scene->mMaterials[mesh->mMaterialIndex];
        auto diffuse_maps = load_material_textures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        texture_datas.insert(texture_datas.end(), diffuse_maps.begin(), diffuse_maps.end());

        auto specular_maps = load_material_textures(material, aiTextureType_SPECULAR, "texture_specular");
        texture_datas.insert(texture_datas.end(), specular_maps.begin(), specular_maps.end());
    }

    return Mesh(vertices, indices, texture_datas);
}

std::vector<TextureData> Model::load_material_textures(aiMaterial* mat,
                                                             const aiTextureType type,
                                                             const std::string& type_name)
{
    std::vector<TextureData> texture_datas;

    for (size_t i = 0, count = mat->GetTextureCount(type); i < count; i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        auto skip = false;
        for (auto &loaded_texture_data : loaded_texture_datas_)
        {
            if (std::strcmp(loaded_texture_data.path.data(), str.C_Str()) == 0)
            {
                texture_datas.push_back(loaded_texture_data);
                skip = true;
                break;
            }
        }

        if (!skip)
        {
            TextureData texture_data {};
            texture_data.texture = new Texture(directory_ + "/" + str.C_Str());
            texture_data.type = type_name;
            texture_data.path = str.C_Str();
            texture_datas.push_back(texture_data);
            loaded_texture_datas_.push_back(texture_data);
        }
    }

    return texture_datas;
}
