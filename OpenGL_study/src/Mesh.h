#pragma once

#include <string>
#include <vector>

#include "MOS_glm.h"
#include "Shader.h"
#include "Texture.h"

class VertexArray;
class Shader;

class Mesh
{
public:
    /**
     * 顶点数据
     */
    struct Vertex
    {
        glm::vec3 position;     // 位置向量
        glm::vec3 normal;       // 法线向量
        glm::vec3 tex_coords;   // 纹理坐标
    };

    /**
     * 纹理数据
     */
    struct TextureData
    {
        Texture *texture;       // 纹理数据
        std::string type;       // 纹理类型
    };

private:
    std::vector<Vertex>       vertices_;
    std::vector<unsigned int> indices_;
    std::vector<TextureData>  textures_;

    VertexArray *vertex_array_;

public:
    Mesh(std::vector<Vertex>      vertices,
         std::vector<unsigned>    indices,
         std::vector<TextureData> textures);
    ~Mesh();

    void before_draw(Shader& shader);

    inline VertexArray* get_vertex_array() const { return vertex_array_; }

private:
    void setup_mesh();
};
