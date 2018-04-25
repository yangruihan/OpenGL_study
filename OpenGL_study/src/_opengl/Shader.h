#pragma once

#include <string>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#ifdef __APPLE__
    #include "libs/glm/gtc/matrix_transform.hpp"
#else
    #include "glm/gtc/matrix_transform.hpp"
#endif

struct ShaderProgramSource
{
    std::string vertex_source;
    std::string fragment_source;
};

class Shader
{
private:
    unsigned int renderer_id_;
    std::string filepath_;
    unsigned int vertex_shader_id_;
    unsigned int fragment_shader_id_;
    std::unordered_map<std::string, int> uniform_cache_;

public:
    Shader(std::string  filepath);
    ~Shader();

    void bind() const;
    void unbind() const;

    void set_int(const std::string& name, int value);
    void set_float(const std::string& name, float value);
    void set_vec3f(const std::string& name, float v0, float v1, float v2);
    void set_vec3f(const std::string& name, glm::vec3 value);
    void set_vec4f(const std::string& name, float v0, float v1, float v2, float v3);
    void set_vec4f(const std::string& name, glm::vec4 value);
    void set_mat4f(const std::string& name, const glm::mat4 mat4);

private:
    static unsigned int compile_shader(unsigned int type, const std::string& source);
    
    ShaderProgramSource parse_shader() const;
    unsigned int create_shader(const std::string& vertex_shader, const std::string& fragment_shader);
    
    int get_uniform_location(const std::string& name);
};
