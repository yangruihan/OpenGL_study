#include "Renderer.h"

void Renderer::clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::draw(const VertexArray& va, const Shader& shader) const
{
    shader.bind();
    va.bind();

    GLCall(glDrawElements(GL_TRIANGLES, 
                          va.get_index_buffer()->get_count(), 
                          GL_UNSIGNED_INT, nullptr));
}
