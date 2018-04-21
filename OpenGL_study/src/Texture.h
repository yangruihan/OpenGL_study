#pragma once

#include <string>

#include "Common.h"
#include "stb/stb_image.h"

class Texture
{
private:
    unsigned int renderer_id_;
    int width_, height_;
    int bpp_;
    std::string filepath_;
    unsigned char* img_buffer_;

public:
    Texture(const std::string& filepath);
    ~Texture();
   
    void bind(unsigned int slot = 0) const;
    void unbind() const;

    inline int get_width() const { return width_; }
    inline int get_height() const { return height_; }
};

