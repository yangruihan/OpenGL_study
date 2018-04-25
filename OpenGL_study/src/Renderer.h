#pragma once

#include <GL/glew.h>
#include <iostream>

#include "Common.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#ifdef __APPLE__
#include "libs/glm/glm.hpp"
#else
#include "glm/glm.hpp"
#endif


class VertexArray;
class IndexBuffer;
class Shader;

class Renderer
{
private:
    glm::vec4 clear_color_;

public:
    Renderer();
    ~Renderer();

    void clear() const;
    void draw(const VertexArray& va, const Shader& shader) const;

    void set_clear_color(glm::vec4 color = glm::vec4(0.0f));
};
