#include "Renderer.h"

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function_name, const char* file_name, int line)
{
    while (const GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << ") "
            << function_name << " "
            << file_name << " : "
            << line << std::endl;
        return false;
    }
    return true;
}

void Renderer::clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    va.bind();
    ib.bind();
    shader.bind();

    GLCall(glDrawElements(GL_TRIANGLES, ib.get_count(), GL_UNSIGNED_INT, nullptr));
}
