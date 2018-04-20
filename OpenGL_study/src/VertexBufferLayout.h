#pragma once

#include <vector>
#include <GL/glew.h>

#include "Renderer.h"

struct VertexBufferLayoutElement
{
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int get_type_size(unsigned int type)
    {
        switch (type)
        {
        case GL_FLOAT:          return 4;
        case GL_UNSIGNED_INT:   return 4;
        case GL_UNSIGNED_BYTE:  return 1;
        }

        // TODO: log error info : type not support
        ASSERT(false);
        return 0;
    }
};

class VertexBufferLayout
{
private:
    std::vector<VertexBufferLayoutElement> elements_;
    unsigned int stride_;

public:
    VertexBufferLayout();
    ~VertexBufferLayout();

    template <typename T>
    void push(unsigned count)
    {
        static_assert(false);
    }

    template <>
    void push<float>(unsigned count)
    {
        elements_.push_back({ GL_FLOAT, count, GL_FALSE });
        stride_ += count * VertexBufferLayoutElement::get_type_size(GL_FLOAT);
    }

    template <>
    void push<unsigned>(unsigned count)
    {
        elements_.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
        stride_ += count * VertexBufferLayoutElement::get_type_size(GL_UNSIGNED_INT);
    }

    template <>
    void push<unsigned char>(unsigned count)
    {
        elements_.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
        stride_ += count * VertexBufferLayoutElement::get_type_size(GL_UNSIGNED_BYTE);
    }

    std::vector<VertexBufferLayoutElement> get_elements() const { return elements_; }
    unsigned int get_stride() const { return stride_; }
};
