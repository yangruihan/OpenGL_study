#pragma once

#include "Renderer.h"

class VertexBuffer
{
private:
    unsigned int renderer_id_;

public:
    VertexBuffer(const void* data, const unsigned int& size);
    ~VertexBuffer();

    void bind() const;
    void un_bind() const;
};

