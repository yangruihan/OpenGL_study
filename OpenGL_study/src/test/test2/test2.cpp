#include "Header.h"

/**
* process input
*/
void process_input(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

/**
 * 三角形上下颠倒
 * vertex shader 传值到 fragment shader
 */
int main()
{
    const auto proj = glm::ortho(0.0f, 480.0f, 0.0f, 480.0f, -1.0f, 1.0f);

    Window window(480, 480, "test2");

    unsigned int index[] = { 0, 1, 2 };
    IndexBuffer index_buffer(index, 3);

    Shader shader("src/test/test2/test2.shader");
    shader.set_uniform_mat4f("u_MVP", proj);

    VertexBufferLayout vertex_buffer_layout;
    vertex_buffer_layout.push<float>(2);

    float pos1[] = {
         120.0f,  120.0f,
         360.0f,  120.0f,
         360.0f,  360.0f
    };

    VertexArray va1;
    VertexBuffer vb1(pos1, 2 * 3 * sizeof(float));
    va1.add_buffer(vb1, vertex_buffer_layout, index_buffer);

    Renderer renderer;

    while (window.show())
    {
        process_input(window.get_window());

        renderer.clear();

        renderer.draw(va1, shader);

        window.clean();
    }

    return 0;
}
