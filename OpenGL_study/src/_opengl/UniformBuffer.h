#pragma once

#include "Renderer.h"

class UniformBuffer
{
private:
    unsigned int renderer_id_;
    unsigned int size_;
    unsigned int bind_point_;

public:
    UniformBuffer(const unsigned int& size, const unsigned int& bind_point, const void *data = nullptr);
    virtual ~UniformBuffer();

    void bind() const;
    void unbind() const;
    void put_data(const unsigned int& size, const void *data, const unsigned int offset = 0) const;
};
