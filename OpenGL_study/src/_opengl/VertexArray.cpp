#include "VertexArray.h"

VertexArray::VertexArray()
    : vertex_buffer_(nullptr), index_buffer_(nullptr)
{
    GLCall(glGenVertexArrays(1, &renderer_id_));
}

VertexArray::~VertexArray()
{
    unbind();
    GLCall(glDeleteVertexArrays(1, &renderer_id_));
    vertex_buffer_ = nullptr;
    index_buffer_ = nullptr;
}

void VertexArray::add_buffer(VertexBuffer&       vertex_buffer, 
                             VertexBufferLayout& vertex_buffer_layout,
                             IndexBuffer&        index_buffer)
{
    vertex_buffer_ = new VertexBuffer(vertex_buffer);
    index_buffer_ = new IndexBuffer(index_buffer);

    bind();
    vertex_buffer.bind();
    index_buffer.bind();

    const auto& elements = vertex_buffer_layout.get_elements();
    unsigned int offset = 0;
    for (size_t i = 0; i < elements.size(); ++i)
    {
        GLCall(glEnableVertexAttribArray(i));
        GLCall(glVertexAttribPointer(i, 
                                     elements[i].count, 
                                     elements[i].type,
                                     elements[i].normalized,
                                     vertex_buffer_layout.get_stride(),
                                     reinterpret_cast<const void*>(offset)));

        offset += elements[i].count * VertexBufferLayoutElement::get_type_size(elements[i].type);
    }

    unbind();
}

void VertexArray::bind() const
{
    GLCall(glBindVertexArray(renderer_id_));
}

void VertexArray::unbind() const
{
    GLCall(glBindVertexArray(0));

    if (vertex_buffer_)
        vertex_buffer_->unbind();

    if (index_buffer_)
        index_buffer_->unbind();
}
