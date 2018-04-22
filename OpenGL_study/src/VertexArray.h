#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"

class VertexBuffer;
class VertexBufferLayout;
class IndexBuffer;

class VertexArray
{
private:
    unsigned int renderer_id_;

    VertexBuffer *vertex_buffer_;
    IndexBuffer  *index_buffer_;

public:
    VertexArray();
    ~VertexArray();

    void add_buffer(VertexBuffer&       vertex_buffer, 
                    VertexBufferLayout& vertex_buffer_layout,
                    IndexBuffer&        index_buffer);

    void bind() const;
    void unbind() const;

    inline VertexBuffer* get_vertex_buffer() const { return vertex_buffer_; }
    inline IndexBuffer* get_index_buffer() const { return index_buffer_; }
};

