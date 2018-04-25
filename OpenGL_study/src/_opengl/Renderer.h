#pragma once

#include <GL/glew.h>
#include <iostream>

#include "Common.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Mesh.h"
#include "MOS_glm.h"

class VertexArray;
class IndexBuffer;
class Shader;
class Mesh;

class Renderer
{
private:
    glm::vec4 clear_color_;

public:
    Renderer();
    ~Renderer();

    void clear() const;
    void draw(const VertexArray& va, const Shader& shader) const;
    void draw(Mesh& mesh, Shader& shader);

    void set_clear_color(glm::vec4 color = glm::vec4(0.0f));
};
