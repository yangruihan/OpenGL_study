#include "VertexBufferLayout.h"

VertexBufferLayout::VertexBufferLayout()
    : stride_(0)
{
}

VertexBufferLayout::~VertexBufferLayout()
{
    stride_ = 0;
    elements_.clear();
}
