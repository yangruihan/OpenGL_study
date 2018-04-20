#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"

using namespace std;

struct ShaderProgramSource
{
    string vertex_source;
    string fragment_source;
};

static ShaderProgramSource ParseShader(const string& filepath)
{
    ifstream stream(filepath);

    enum class ShaderType
    {
        none = -1,
        vertext = 0,
        fragment = 1,
    };

    string line;
    stringstream ss[2];
    auto type = ShaderType::none;
    while (getline(stream, line))
    {
        if (line.find("#shader") != string::npos)
        {
            if (line.find("vertex") != string::npos)
                type = ShaderType::vertext;
            else if (line.find("fragment") != string::npos)
                type = ShaderType::fragment;
            else
                cout << "parse shader error: syntax error" << endl;
        }
        else
        {
            ss[static_cast<int>(type)] << line << "\n";       
        }
    }

    return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src ,nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = static_cast<char*>(alloca(length * sizeof(char)));
        glGetShaderInfoLog(id, length, &length, message);
        cout << "Failed to compile " <<
            (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
            <<"shader!" << endl;
        cout << message << endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const string& vertexShader, const string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main()
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow * window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
    GLCall(glfwSwapInterval(1));

    if (glewInit() != GLEW_OK)
    {
        cout << "glew init error" << endl;
    }

    cout << glGetString(GL_VERSION) << endl;

    {
        float positions[] = {
            -0.5f, -0.5f,
             0.5f, -0.5f,
             0.5f,  0.5f,
            -0.5f,  0.5f,
        };

        unsigned int index[] = {
            0, 1, 2,
            2, 3, 0,
        };

        VertexArray vertex_array;

        VertexBuffer vertex_buffer(positions, 8 * sizeof(float));
        
        VertexBufferLayout vertex_buffer_layout;
        vertex_buffer_layout.push<float>(2);

        vertex_array.add_buffer(vertex_buffer,
                                vertex_buffer_layout);

        IndexBuffer index_buffer(index, 6);

        // 解析 shader 文件
        const auto shader_src = ParseShader("res/shaders/basic.shader");

        // 创建 shader program
        const auto shader = CreateShader(shader_src.vertex_source, shader_src.fragment_source);

        // 使用 shader program
        GLCall(glUseProgram(shader));

        const auto location = glGetUniformLocation(shader, "u_Color");

        GLCall(glUseProgram(0));
        GLCall(glBindVertexArray(0));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

        float r = 0.0f;
        float increament = 0.01f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            GLCall(glClear(GL_COLOR_BUFFER_BIT));

    //        glDrawArrays(GL_TRIANGLES, 0, 3);
            if (r >= 1.0f)
                increament = -0.01f;
            else if (r <= 0.0f)
                increament = 0.01f;

            r += increament;

            GLCall(glUseProgram(shader));
            GLCall(glUniform4f(location, r, 0.0f, 0.0f, 1.0f));

            vertex_array.bind();
            index_buffer.bind();

            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            /* Swap front and back buffers */
            GLCall(glfwSwapBuffers(window));

            /* Poll for and process events */
            GLCall(glfwPollEvents());
        }

        GLCall(glDeleteProgram(shader));
    }

    glfwTerminate();

    return 0;
}