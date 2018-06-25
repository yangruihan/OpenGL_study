#include "VertexBufferLayout.h"

VertexBufferLayout::VertexBufferLayout()
    : stride_(0)
{
}

VertexBufferLayout::~VertexBufferLayout()
{
    elements_.clear();
}

template <>
void VertexBufferLayout::push<float>(unsigned count, const unsigned int divisor)
{
    elements_.push_back({ GL_FLOAT, count, GL_FALSE, divisor });
    stride_ += count * VertexBufferLayoutElement::get_type_size(GL_FLOAT);
}

template <>
void VertexBufferLayout::push<unsigned>(unsigned count, const unsigned int divisor)
{
    elements_.push_back({ GL_UNSIGNED_INT, count, GL_FALSE, divisor });
    stride_ += count * VertexBufferLayoutElement::get_type_size(GL_UNSIGNED_INT);
}

template <>
void VertexBufferLayout::push<unsigned char>(unsigned count, const unsigned int divisor)
{
    elements_.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE, divisor });
    stride_ += count * VertexBufferLayoutElement::get_type_size(GL_UNSIGNED_BYTE);
}
