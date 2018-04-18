#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

#define ASSERT(x) if (!(x)) __debugbreak();

#ifdef DEBUG
    #define GLCall(x) do { GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__)) } while (0);
#else
    #define GLCall(x) x
#endif

static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function_name, const char* file_name, int line)
{
    while (const GLenum error = glGetError())
    {
        cout << "[OpenGL Error] (" << error << ") " 
             << function_name << " " 
             << file_name << " : " 
             << line << endl;
        return false;
    }
    return true;
}

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

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow * window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        cout << "glew init error" << endl;
    }

    cout << glGetString(GL_VERSION) << endl;

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
    
    unsigned int buffer;
    // 生成缓冲区对象名称
    glGenBuffers(1, &buffer);
    // 指定缓冲区对象格式
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    // 创建并初始化一个缓冲区对象的数据存储
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), positions, GL_STATIC_DRAW);

    // vertex index
    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), index, GL_STATIC_DRAW);

    // 启用顶点属性数组
    glEnableVertexAttribArray(0);
    // 定义一个通用顶点数组属性
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

    // 解析 shader 文件
    const auto shader_src = ParseShader("res/shaders/basic.shader");

    // 创建 shader program
    const auto shader = CreateShader(shader_src.vertex_source, shader_src.fragment_source);

    // 使用 shader program
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

//        glDrawArrays(GL_TRIANGLES, 0, 3);

        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}