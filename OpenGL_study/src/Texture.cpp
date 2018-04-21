#include "Texture.h"

Texture::Texture(const std::string& filepath)
    : renderer_id_(0), width_(0), height_(0),
      bpp_(0), filepath_(filepath), img_buffer_(nullptr)
{
    // OpenGL 原点在左下角，PNG 原点在左上角，因此要翻转图片
    stbi_set_flip_vertically_on_load(1);
    // load image
    img_buffer_ = stbi_load(filepath.c_str(), &width_, &height_, &bpp_, 0);

    GLCall(glGenTextures(1, &renderer_id_));
    GLCall(glBindTexture(GL_TEXTURE_2D, renderer_id_));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
                        width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, img_buffer_));

    GLCall(glBindTexture(GL_TEXTURE_2D, 0));

    if (img_buffer_)
        stbi_image_free(img_buffer_);
}

Texture::~Texture()
{
    GLCall(glDeleteTextures(1, &renderer_id_));
}

void Texture::bind(const unsigned int slot) const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, renderer_id_));
}

void Texture::unbind() const
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
