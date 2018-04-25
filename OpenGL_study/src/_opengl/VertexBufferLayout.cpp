#include "VertexBufferLayout.h"

VertexBufferLayout::VertexBufferLayout()
    : stride_(0)
{
}

VertexBufferLayout::~VertexBufferLayout()
{
    elements_.clear();
}
