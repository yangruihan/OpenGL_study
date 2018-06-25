#pragma once

#include <vector>
#include <GL/glew.h>

#include "Renderer.h"

struct VertexBufferLayoutElement
{
    unsigned int  type;         // 类型
    unsigned int  count;        // 数量
    unsigned char normalized;   // 是否需要归一化
    unsigned int  divisor;      // 除数

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
    virtual ~VertexBufferLayout();

    template <class T>
    void push(unsigned count, const unsigned int divisor = 0)
    {
        ASSERT(false);
    }

    std::vector<VertexBufferLayoutElement> get_elements() const { return elements_; }
    unsigned int get_stride() const { return stride_; }
};
