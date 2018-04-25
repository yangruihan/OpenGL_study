#pragma once

#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.h"
#include "Mesh.h"

class Mesh;
class Shader;
class Renderer;
struct TextureData;
struct Vertex;

class Model
{
private:
    std::vector<Mesh> meshes_ {};
    std::string path_;
    std::string directory_;

    std::vector<TextureData> loaded_texture_datas_;

public:
    Model(const std::string& path);
    ~Model();

    void draw(const Renderer& renderer, Shader& shader);

private:
    void load_model(const std::string& path);
    void process_node(aiNode *node, const aiScene *scene);
    Mesh process_mesh(aiMesh *mesh, const aiScene *scene);
    std::vector<TextureData> load_material_textures(aiMaterial *mat,
                                                          aiTextureType type,
                                                          const std::string& type_name);
};

