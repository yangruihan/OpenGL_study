#pragma once

#include <vector>

#include "Renderer.h"

class CubeTexture
{
private:
    unsigned int renderer_id_;

    std::vector<std::string> face_paths_;

public:
    CubeTexture(std::vector<std::string> face_paths);
    ~CubeTexture();

    void bind(const unsigned int slot = 0) const;
    void unbind() const;
};

