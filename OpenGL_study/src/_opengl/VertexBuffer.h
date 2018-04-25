#pragma once

#include "Renderer.h"

class VertexBuffer
{
private:
    unsigned int renderer_id_;

public:
    VertexBuffer(const void* data, const unsigned int& size);
    VertexBuffer(const VertexBuffer& other);
    ~VertexBuffer();

    void bind() const;
    void unbind() const;
};

