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
 * 2个相连不同颜色三角形
 */
int main()
{
    const auto proj = glm::ortho(0.0f, 480.0f, 0.0f, 480.0f, -1.0f, 1.0f);

    Window window(480, 480, "test1_hello_triangles");

    unsigned int index[] = { 0, 1, 2 };
    IndexBuffer index_buffer(index, 3);

    Shader shader("res/shaders/basic.shader");
    shader.set_mat4f("u_MVP", proj);

    VertexBufferLayout vertex_buffer_layout;
    vertex_buffer_layout.push<float>(2);

    float pos1[] = {
        0.0f, 0.0f,
        100.0f, 0.0f,
        100.0f, 100.0f
    };

    VertexArray va1;
    VertexBuffer vb1(pos1, 2 * 3 * sizeof(float));
    va1.add_buffer(vb1, vertex_buffer_layout, index_buffer);

    float pos2[] = {
        100.0f, 0.0f,
        200.0f, 0.0f,
        100.0f, 100.0f
    };

    VertexArray va2;
    VertexBuffer vb2(pos2, 2 * 3 * sizeof(float));
    va2.add_buffer(vb2, vertex_buffer_layout, index_buffer);

    Renderer renderer;

    while (window.show())
    {
        process_input(window.get_window());

        renderer.clear();

        shader.set_vec4f("u_Color", 1, 0, 0, 1);
        renderer.draw(va1, shader);

        shader.set_vec4f("u_Color", 0, 1, 0, 1);
        renderer.draw(va2, shader);

        window.end_of_frame();
    }

    return 0;
}