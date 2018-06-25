#pragma once

#include <unordered_map>
#include "Renderer.h"

enum FB_ATTACHMENT_TYPE
{
    Color,
    Depth,
    Stencil,
    Depth_Stencil,
};

class FrameBuffer
{
private:
    unsigned int renderer_id_;
    
    std::unordered_map<unsigned int, unsigned int> attach_color_textures_;
    int attach_depth_texture_;
    int attach_stencil_texture_;
    int attach_depth_stencil_texture_;
    
    std::unordered_map<unsigned int, unsigned int> attach_color_rbos_;
    int attach_depth_rbo_;
    int attach_stencil_rbo_;
    int attach_depth_stencil_rbo_;

public:
    FrameBuffer();
    virtual ~FrameBuffer();
    
    // 添加纹理附件
    void add_texture_attachment(const FB_ATTACHMENT_TYPE& type,
                                const unsigned int& width,
                                const unsigned int& height,
                                const unsigned int& offset = 0);
    // 添加渲染对象附件
    void add_render_buffer_attachment(const FB_ATTACHMENT_TYPE& type,
                                      const unsigned int& width,
                                      const unsigned int& height,
                                      const unsigned int& offset = 0);
    
    void bind() const;
    void unbind() const;
    void bind_texture(const FB_ATTACHMENT_TYPE& type,
                      const unsigned int& offset = 0);
    void bind_render_buffer(const FB_ATTACHMENT_TYPE& type,
                            const unsigned int& offset = 0);
    bool check() const;
};
