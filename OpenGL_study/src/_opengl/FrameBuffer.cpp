#include "FrameBuffer.h"

FrameBuffer::FrameBuffer()
    : renderer_id_(0),
      attach_depth_texture_(-1),
      attach_stencil_texture_(-1),
      attach_depth_stencil_texture_(-1),
      attach_depth_rbo_(-1),
      attach_stencil_rbo_(-1),
      attach_depth_stencil_rbo_(-1)
{
    GLCall(glGenFramebuffers(1, &renderer_id_));
}

FrameBuffer::~FrameBuffer()
{
    GLCall(glDeleteFramebuffers(1, &renderer_id_));
}

void FrameBuffer::bind() const
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, renderer_id_));
}

void FrameBuffer::unbind() const
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void FrameBuffer::add_texture_attachment(const FB_ATTACHMENT_TYPE& type,
                                         const unsigned int& width,
                                         const unsigned int& height,
                                         const unsigned int& offset)
{
    bind();
    
    unsigned int texture;
    GLCall(glGenTextures(1, &texture));
    GLCall(glBindTexture(GL_TEXTURE_2D, texture));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    
    switch (type) {
        case FB_ATTACHMENT_TYPE::Color:
        {
            GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                                width, height, 0, GL_RGBA,
                                GL_UNSIGNED_BYTE, NULL));
            GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER,
                                          GL_COLOR_ATTACHMENT0 + offset,
                                          GL_TEXTURE_2D, texture, 0));
            attach_color_textures_[offset] = texture;
            break;
        }
            
        case FB_ATTACHMENT_TYPE::Depth:
        {
            GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                                width, height, 0, GL_DEPTH_COMPONENT,
                                GL_UNSIGNED_BYTE, NULL));
            GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER,
                                          GL_DEPTH_ATTACHMENT,
                                          GL_TEXTURE_2D, texture, 0));
            attach_depth_texture_ = texture;
            break;
        }
            
        case FB_ATTACHMENT_TYPE::Stencil:
        {
            GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_STENCIL_INDEX,
                                width, height, 0, GL_STENCIL_INDEX,
                                GL_UNSIGNED_BYTE, NULL));
            GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER,
                                          GL_STENCIL_ATTACHMENT,
                                          GL_TEXTURE_2D, texture, 0));
            attach_stencil_texture_ = texture;
            break;
        }
            
        case FB_ATTACHMENT_TYPE::Depth_Stencil:
        {
            GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8,
                                width, height, 0, GL_DEPTH_STENCIL,
                                GL_UNSIGNED_INT_24_8, NULL));
            GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER,
                                          GL_UNSIGNED_INT_24_8,
                                          GL_TEXTURE_2D, texture, 0));
            attach_depth_stencil_texture_ = texture;
            break;
        }
    }
    
    ASSERT(check());
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
    unbind();
}

void FrameBuffer::add_render_buffer_attachment(const FB_ATTACHMENT_TYPE& type,
                                                  const unsigned int& width,
                                                  const unsigned int& height,
                                                  const unsigned int& offset)
{
    bind();
    
    unsigned int rbo;
    GLCall(glGenRenderbuffers(1, &rbo));
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, rbo));
    
    switch (type) {
        case FB_ATTACHMENT_TYPE::Color:
        {
            GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8,
                                         width, height));
            GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                                             GL_COLOR_ATTACHMENT0 + offset,
                                             GL_RENDERBUFFER, rbo));
            attach_color_rbos_[offset] = rbo;
            break;
        }
            
        case FB_ATTACHMENT_TYPE::Depth:
        {
            GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
                                         width, height));
            GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                                             GL_DEPTH_ATTACHMENT,
                                             GL_RENDERBUFFER, rbo));
            attach_depth_rbo_ = rbo;
            break;
        }
            
        case FB_ATTACHMENT_TYPE::Stencil:
        {
            GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX,
                                         width, height));
            GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                                             GL_STENCIL_ATTACHMENT,
                                             GL_RENDERBUFFER, rbo));
            attach_stencil_rbo_ = rbo;
            break;
        }
            
        case FB_ATTACHMENT_TYPE::Depth_Stencil:
        {
            GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
                                         width, height));
            GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                                             GL_DEPTH_STENCIL_ATTACHMENT,
                                             GL_RENDERBUFFER, rbo));
            attach_depth_stencil_rbo_ = rbo;
            break;
        }
    }
    
    ASSERT(check());
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
    unbind();
}

bool FrameBuffer::check() const
{
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "[ERROR]FrameBuffer: Framebuffer is not complete!" << std::endl;
        return false;
    }

    return true;
}

void FrameBuffer::bind_texture(const FB_ATTACHMENT_TYPE& type,
                               const unsigned int& offset)
{
    GLCall(glActiveTexture(GL_TEXTURE0 + offset));
    switch (type) {
        case FB_ATTACHMENT_TYPE::Color:
        {
            GLCall(glBindTexture(GL_TEXTURE_2D, attach_color_textures_[offset]));
            break;
        }
            
        case FB_ATTACHMENT_TYPE::Depth:
        {
            GLCall(glBindTexture(GL_TEXTURE_2D, attach_depth_texture_));
            break;
        }
            
        case FB_ATTACHMENT_TYPE::Stencil:
        {
            GLCall(glBindTexture(GL_TEXTURE_2D, attach_stencil_texture_));
            break;
        }
            
        case FB_ATTACHMENT_TYPE::Depth_Stencil:
        {
            GLCall(glBindTexture(GL_TEXTURE_2D, attach_depth_stencil_texture_));
            break;
        }
    }
}

void FrameBuffer::bind_render_buffer(const FB_ATTACHMENT_TYPE& type,
                                     const unsigned int& offset)
{
    switch (type) {
        case FB_ATTACHMENT_TYPE::Color:
        {
            GLCall(glBindRenderbuffer(GL_RENDERBUFFER, attach_color_rbos_[offset]));
            break;
        }
            
        case FB_ATTACHMENT_TYPE::Depth:
        {
            GLCall(glBindRenderbuffer(GL_RENDERBUFFER, attach_depth_rbo_));
            break;
        }
            
        case FB_ATTACHMENT_TYPE::Stencil:
        {
            GLCall(glBindRenderbuffer(GL_RENDERBUFFER, attach_stencil_rbo_));
            break;
        }
            
        case FB_ATTACHMENT_TYPE::Depth_Stencil:
        {
            GLCall(glBindRenderbuffer(GL_RENDERBUFFER, attach_depth_rbo_));
            break;
        }
    }
}
