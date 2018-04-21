#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const unsigned int index[], const unsigned& count)
    : count_(count)
{
    GLCall(glGenBuffers(1, &renderer_id_));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id_));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), index, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
    renderer_id_ = 0;
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void IndexBuffer::bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id_));
}

void IndexBuffer::un_bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
