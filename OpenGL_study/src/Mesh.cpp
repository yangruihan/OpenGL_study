#include "Mesh.h"
#include <utility>

Mesh::Mesh(std::vector<Vertex>      vertices,
           std::vector<unsigned>    indices,
           std::vector<TextureData> textures)
    : vertices_ (std::move(vertices)), 
      indices_  (std::move(indices)), 
      texture_datas_ (std::move(textures)),
      vertex_array_(nullptr)
{
    setup_mesh();
}

Mesh::~Mesh()
{
    vertex_array_ = nullptr;
}

void Mesh::draw(const Renderer& renderer, Shader& shader)
{
    unsigned int diffuse_n = 1;
    unsigned int specular_n = 1;

    for (size_t i = 0, count = texture_datas_.size(); i < count; i++)
    {
        texture_datas_[i].texture->bind(i);
        std::string number;
        const auto type = texture_datas_[i].type;
        if (type == "texture_diffuse")
            number = std::to_string(diffuse_n++);
        else if (type == "texture_specular")
            number = std::to_string(specular_n++);
        shader.set_int("u_Material." + type + number, i);
    }

    renderer.draw(*vertex_array_, shader);
}

void Mesh::setup_mesh()
{
    vertex_array_ = new VertexArray();
    VertexBuffer vertex_buffer(&vertices_[0], vertices_.size() * sizeof(Vertex));
    VertexBufferLayout vertex_buffer_layout;
    vertex_buffer_layout.push<float>(3);        // 顶点位置
    vertex_buffer_layout.push<float>(3);        // 顶点法线
    vertex_buffer_layout.push<float>(2);        // 顶点纹理
    IndexBuffer index_buffer(&indices_[0], indices_.size());
    vertex_array_->add_buffer(vertex_buffer, vertex_buffer_layout, index_buffer);
}
