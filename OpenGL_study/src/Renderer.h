#pragma once

#include <GL/glew.h>
#include <iostream>

#include "Common.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

class VertexArray;
class IndexBuffer;
class Shader;

void GLClearError();
bool GLLogCall(const char* function_name, const char* file_name, int line);

class Renderer
{
public:
    void clear() const;
    void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};