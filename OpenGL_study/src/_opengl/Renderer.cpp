#include "Renderer.h"

Renderer::Renderer()
    : clear_color_(glm::vec4(0.0f))
{
    GLCall(glClearColor(clear_color_.x, clear_color_.y, clear_color_.z, clear_color_.w));
}

Renderer::~Renderer() = default;

void Renderer::clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::draw(const VertexArray& va, const Shader& shader) const
{
    shader.bind();
    va.bind();

    GLCall(glDrawElements(GL_TRIANGLES, 
                          va.get_index_buffer()->get_count(), 
                          GL_UNSIGNED_INT, nullptr));
}

void Renderer::draw(Mesh& mesh, Shader& shader)
{
    mesh.draw(*this, shader);
}

void Renderer::draw(Model& model, Shader& shader)
{
    model.draw(*this, shader);
}

void Renderer::set_clear_color(const glm::vec4 color)
{
    if (color != clear_color_)
    {
        clear_color_ = color;
        GLCall(glClearColor(color.x, color.y, color.z, color.w));
    }
}
