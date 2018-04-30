#include "CubeTexture.h"
#include <utility>

CubeTexture::CubeTexture(std::vector<std::string> face_paths)
    : renderer_id_(0), face_paths_(std::move(face_paths))
{
    GLCall(glGenTextures(1, &renderer_id_));
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, renderer_id_));

    stbi_set_flip_vertically_on_load(1);

    int width, height, nr_channels;
    for (size_t i = 0, count = face_paths_.size(); i < count; i++)
    {
        auto *data = stbi_load(face_paths_[i].c_str(), &width, &height, &nr_channels, 0);
        if (data)
        {
            GLenum format = GL_RGBA;
            if (nr_channels == 1)
                format = GL_RED;
            else if (nr_channels == 3)
                format = GL_RGB;
            else if (nr_channels == 4)
                format = GL_RGBA;

            GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format,
                         width, height, 0, format, GL_UNSIGNED_BYTE, data));
        }
        else
        {
            std::cout << "[ERROR]CubeTexture: failed to load texture at path " << face_paths_[i] << std::endl;
        }

        stbi_image_free(data);
    }

    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}

CubeTexture::~CubeTexture()
{
    GLCall(glDeleteTextures(1, &renderer_id_));
}

void CubeTexture::bind(const unsigned int slot) const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, renderer_id_));
}

void CubeTexture::unbind() const
{
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}
