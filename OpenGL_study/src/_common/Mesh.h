#pragma once

#include <string>
#include <vector>

#include "MOS_glm.h"
#include "Shader.h"
#include "Texture.h"

class VertexArray;
class Shader;
class Renderer;

/**
 * 顶点数据
 */
struct VertexData
{
    glm::vec3 position;     // 位置向量
    glm::vec3 normal;       // 法线向量
    glm::vec2 tex_coords;   // 纹理坐标
    glm::vec3 tangent;      // 切线向量
    glm::vec3 bitangent;    // 双切线向量
};

/**
 * 纹理数据
 */
struct TextureData
{
    Texture *texture;       // 纹理数据
    std::string type;       // 纹理类型
    std::string path;       // 路径
};

class Mesh
{
private:
    std::vector<VertexData>   vertices_;
    std::vector<unsigned int> indices_;
    std::vector<TextureData>  texture_datas_;

    VertexArray *vertex_array_;

public:
    Mesh(std::vector<VertexData>  vertices,
         std::vector<unsigned>    indices,
         std::vector<TextureData> textures);
    ~Mesh();

    void draw(const Renderer& renderer, Shader& shader);

private:
    void setup_mesh();
};
