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
 * 三角形旋转
 */
int main()
{
    Window window(480, 480, "test3");

    unsigned int index[] = { 0, 1, 2 };
    IndexBuffer index_buffer(index, 3);

    Shader shader("src/test/test3/test3.shader");

    const auto proj_mat = glm::ortho(0.0f, 480.0f, 0.0f, 480.0f, -1.0f, 1.0f);
    auto rotate_mat = glm::mat4(1.0f);
    rotate_mat = glm::translate(rotate_mat, glm::vec3(240, 240 , 0));

    shader.set_mat4f("u_MVP", proj_mat * rotate_mat);
    shader.set_vec4f("u_Color", 1, 0, 0, 1);

    VertexBufferLayout vertex_buffer_layout;
    vertex_buffer_layout.push<float>(2);

    float pos1[] = {
         -120.0f,  -120.0f,
          120.0f,  -120.0f,
          120.0f,   120.0f
    };

    VertexArray va1;
    VertexBuffer vb1(pos1, 2 * 3 * sizeof(float));
    va1.add_buffer(vb1, vertex_buffer_layout, index_buffer);

    Renderer renderer;

    while (window.show())
    {
        process_input(window.get_window());

        renderer.clear();

        rotate_mat = glm::rotate(rotate_mat, 0.01f, glm::vec3(0, 0, 1));

        shader.set_mat4f("u_MVP", proj_mat * rotate_mat);
        renderer.draw(va1, shader);

        window.clean();
    }

    return 0;
}
