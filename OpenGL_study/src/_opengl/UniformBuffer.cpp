#include "UniformBuffer.h"

UniformBuffer::UniformBuffer(const unsigned& size, const unsigned& bind_point, const void *data)
    : renderer_id_(0), size_(size), bind_point_(bind_point)
{
    GLCall(glGenBuffers(1, &renderer_id_));
    GLCall(glBindBuffer(GL_UNIFORM_BUFFER, renderer_id_));
    GLCall(glBufferData(GL_UNIFORM_BUFFER, size_, data, GL_STATIC_DRAW));
    GLCall(glBindBufferRange(GL_UNIFORM_BUFFER, bind_point_, renderer_id_, 0, size_));
    GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}

UniformBuffer::~UniformBuffer()
{
    GLCall(glDeleteBuffers(1, &renderer_id_));
    unbind();
    renderer_id_= 0;
}

void UniformBuffer::bind() const
{
    GLCall(glBindBuffer(GL_UNIFORM_BUFFER, renderer_id_));
}

void UniformBuffer::unbind() const
{
    GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}

void UniformBuffer::put_data(const unsigned int& size, const void* data, const unsigned offset) const
{
    bind();

    GLCall(glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data));

    unbind();
}
