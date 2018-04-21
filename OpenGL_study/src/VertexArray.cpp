#include "VertexArray.h"

VertexArray::VertexArray()
{
    GLCall(glGenVertexArrays(1, &renderer_id_));
}

VertexArray::~VertexArray()
{
    unbind();
    GLCall(glDeleteVertexArrays(1, &renderer_id_));
}

void VertexArray::add_buffer(const VertexBuffer& vertex_buffer, 
                             const VertexBufferLayout& vertex_buffer_layout) const
{
    bind();
    vertex_buffer.bind();

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
}

void VertexArray::bind() const
{
    GLCall(glBindVertexArray(renderer_id_));
}

void VertexArray::unbind() const
{
    GLCall(glBindVertexArray(0));
}
