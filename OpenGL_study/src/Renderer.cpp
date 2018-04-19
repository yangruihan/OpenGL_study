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
