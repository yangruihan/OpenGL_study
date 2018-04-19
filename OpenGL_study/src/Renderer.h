#pragma once

#include <GL/glew.h>
#include <iostream>

#define ASSERT(x) if (!(x)) __debugbreak();

#ifdef DEBUG
    #define GLCall(x) do { GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__)) } while (0);
#else
    #define GLCall(x) x
#endif

void GLClearError();
bool GLLogCall(const char* function_name, const char* file_name, int line);
