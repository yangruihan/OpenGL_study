#pragma once

#include <GL/glew.h>
#include <iostream>

#ifdef DEBUG
    #ifdef __APPLE__
        #define ASSERT(x) assert(x)
    #else
        #define ASSERT(x) if (!(x)) __debugbreak();
    #endif
#else
    #define ASSERT(x) x
#endif

#ifdef DEBUG
    #define GLCall(x) do { GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__)); } while (0);
#else
    #define GLCall(x) x
#endif

inline void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

inline bool GLLogCall(const char* function_name, const char* file_name, int line)
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
