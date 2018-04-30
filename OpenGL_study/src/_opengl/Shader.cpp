#include "Shader.h"
 #include <utility>

Shader::Shader(std::string filepath)
    : filepath_(std::move(filepath)),
      vertex_shader_id_(-1),
      fragment_shader_id_(-1),
      geometry_shader_id_(-1)
{
    // 解析 shader 文件
    const auto shader_src = parse_shader();

    // 创建 shader program
    renderer_id_ = create_shader(shader_src.vertex_source, 
                                 shader_src.fragment_source,
                                 shader_src.geometry_source);

    // 使用 shader program
    GLCall(glUseProgram(renderer_id_));
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(renderer_id_));
}

void Shader::set_vec4f(const std::string& name, const float v0, const float v1, 
                                                const float v2, const float v3)
{
    bind();
    GLCall(glUniform4f(get_uniform_location(name), v0, v1, v2, v3));
}

void Shader::set_int(const std::string& name, const int value)
{
    bind();
    GLCall(glUniform1i(get_uniform_location(name), value));
}

void Shader::set_float(const std::string& name, const float value)
{
    bind();
    GLCall(glUniform1f(get_uniform_location(name), value));
}

void Shader::set_mat4f(const std::string& name, const glm::mat4 mat4)
{
    bind();
    GLCall(glUniformMatrix4fv(get_uniform_location(name), 1, GL_FALSE, &mat4[0][0]));
}

int Shader::get_uniform_location(const std::string& name)
{
    if (uniform_cache_.find(name) != uniform_cache_.end())
        return uniform_cache_[name];

    auto location = -1;
    GLCall(location = glGetUniformLocation(renderer_id_, name.c_str()));
    if (location == -1)
        std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;


    uniform_cache_[name] = location;
    return location;
}   

void Shader::bind() const
{
    GLCall(glUseProgram(renderer_id_));
}

void Shader::unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::uniform_block_bind(const std::string& name, const unsigned int bind_point) const
{
    auto uniform_index = 0;
    GLCall(uniform_index = glGetUniformBlockIndex(renderer_id_, name.c_str()));
    GLCall(glUniformBlockBinding(renderer_id_, uniform_index, bind_point));
}

void Shader::set_vec3f(const std::string& name, const float v0, const float v1, const float v2)
{
    bind();
    GLCall(glUniform3f(get_uniform_location(name), v0, v1, v2));
}

void Shader::set_vec3f(const std::string& name, const glm::vec3 value)
{
    bind();
    GLCall(glUniform3f(get_uniform_location(name), value.x, value.y, value.z));
}

void Shader::set_vec4f(const std::string& name, const glm::vec4 value)
{
    bind();
    GLCall(glUniform4f(get_uniform_location(name), value.x, value.y, value.z, value.w));
}

ShaderProgramSource Shader::parse_shader() const
{
    std::ifstream stream(filepath_);

    enum class ShaderType
    {
        none     = -1,
        vertext  =  0,
        fragment =  1,
        geometry =  2,
    };

    std::string line;
    std::stringstream ss[3];
    auto type = ShaderType::none;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::vertext;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::fragment;
            else if (line.find("geometry") != std::string::npos)
                type = ShaderType::geometry;
            else
                std::cout << "parse shader error: syntax error" << std::endl;
        }
        else
        {
            ss[static_cast<int>(type)] << line << "\n";
        }
    }

    return { ss[0].str(), ss[1].str(), ss[2].str() };
}

unsigned Shader::compile_shader(const unsigned type, const std::string& source) const
{
    unsigned int id = 0;
    GLCall(id = glCreateShader(type));
    auto src = source.c_str();
    GLCall(glShaderSource(id, 1, &src ,nullptr));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        const auto message = static_cast<char*>(alloca(length * sizeof(char)));
        glGetShaderInfoLog(id, length, &length, message);
        
        std::string shader_type;
        if (type == GL_VERTEX_SHADER)
            shader_type = "vertex ";
        else if (type == GL_FRAGMENT_SHADER)
            shader_type = "fragment ";
        else
            shader_type = "geometry ";
        
        std::cout << "Failed to compile "
                  << shader_type
                  << "shader!"
                  << " [" << filepath_ << "]"
                  << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned Shader::create_shader(const std::string& vertex_shader, 
                               const std::string& fragment_shader,
                               const std::string& geometry_shader)
{
    const auto program = glCreateProgram();
    
    if (!vertex_shader.empty())
    {
        vertex_shader_id_ = compile_shader(GL_VERTEX_SHADER, vertex_shader);
        GLCall(glAttachShader(program, vertex_shader_id_));
    }

    if (!fragment_shader.empty())
    {
        fragment_shader_id_ = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);
        GLCall(glAttachShader(program, fragment_shader_id_));
    }

    if (!geometry_shader.empty())
    {
        geometry_shader_id_ = compile_shader(GL_GEOMETRY_SHADER, geometry_shader);
        GLCall(glAttachShader(program, geometry_shader_id_));
    }

    GLCall(glLinkProgram(program));

    int result;
    GLCall(glGetProgramiv(program, GL_LINK_STATUS, &result));
    if (result == GL_FALSE)
    {
        int length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        char* message = static_cast<char*>(alloca(length * sizeof(char)));
        glGetProgramInfoLog(program, length, &length, message);
        std::cout << "Failed to link program!" << std::endl;
        std::cout << message << std::endl;
        glDeleteProgram(program);
        return 0;
    }

    GLCall(glValidateProgram(program));

    if (vertex_shader_id_ != -1)
        GLCall(glDeleteShader(vertex_shader_id_));

    if (fragment_shader_id_ != -1)
        GLCall(glDeleteShader(fragment_shader_id_));

    if (geometry_shader_id_ != -1)
        GLCall(glDeleteShader(geometry_shader_id_));

    return program;
}
