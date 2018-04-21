#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include "Common.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"

using namespace std;

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
            -0.5f, -0.5f,  0.0f,
             0.5f, -0.5f,  0.0f,
             0.5f,  0.5f,  0.0f,
            -0.5f,  0.5f,  0.0f,
        };

        unsigned int index[] = {
            0, 1, 2,
            2, 3, 0,
        };

        VertexArray vertex_array;
        VertexBuffer vertex_buffer(positions, 12 * sizeof(float));
        VertexBufferLayout vertex_buffer_layout;
        vertex_buffer_layout.push<float>(3);
        vertex_array.add_buffer(vertex_buffer,
                                vertex_buffer_layout);

        IndexBuffer index_buffer(index, 6);

        Shader shader("res/shaders/basic.shader");

        vertex_array.unbind();
        vertex_buffer.unbind();
        index_buffer.unbind();
        shader.unbind();

        Renderer renderer;

        float r = 0.0f;
        float increament = 0.01f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            renderer.clear();

            if (r >= 1.0f)
                increament = -0.01f;
            else if (r <= 0.0f)
                increament = 0.01f;

            r += increament;

            shader.bind();
            shader.set_uniform4f("u_Color", r, 0.0f, 0.0f, 1.0f);

            renderer.draw(vertex_array, index_buffer, shader);

            /* Swap front and back buffers */
            GLCall(glfwSwapBuffers(window));

            /* Poll for and process events */
            GLCall(glfwPollEvents());
        }
    }

    glfwTerminate();

    return 0;
}