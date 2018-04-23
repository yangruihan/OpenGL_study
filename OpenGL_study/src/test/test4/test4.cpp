#include "Header.h"

/**
* process input
*/
void process_input(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main()
{
    Window window(480, 480, "test4");

    float pos[] = {
        -100.0f, -100.0f,  100.0f,
         100.0f, -100.0f,  100.0f,
         100.0f,  100.0f,  100.0f,
        -100.0f,  100.0f,  100.0f,
         100.0f, -100.0f, -100.0f,
         100.0f,  100.0f, -100.0f,
        -100.0f,  100.0f, -100.0f,
        -100.0f, -100.0f, -100.0f,
    };

    unsigned int index[] = {
        0, 1, 2,
        2, 3, 0,
        0, 3, 6,
        6, 3, 2,
        2, 5, 6,
        6, 0, 7,
        7, 4, 6,
        6, 5, 4,
        4, 7, 0,
        0, 2, 4,
        4, 5, 1,
        1, 2, 5,
    };
    
    VertexArray vertex_array;
    VertexBuffer vertex_buffer(pos, 3 * 8 * sizeof(float));
    VertexBufferLayout vertex_buffer_layout;
    vertex_buffer_layout.push<float>(3);
    IndexBuffer index_buffer(index, 3 * 12);
    vertex_array.add_buffer(vertex_buffer, vertex_buffer_layout, index_buffer);

    const auto proj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 480.0f);
    const auto view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -360.0f));
    auto model = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(1.0f, 1.0f, 1.0f));

    Shader shader("src/test/test4/test4.shader");
    shader.set_uniform4f("u_Color", 1, 1, 1, 1);
    shader.set_uniform_mat4f("u_MVP", proj * view * model);

    Renderer renderer;

    while (window.show())
    {
        process_input(window.get_window());

        renderer.clear();

        model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        shader.set_uniform_mat4f("u_MVP", proj * view * model);
        renderer.draw(vertex_array, shader);

        window.clean();
    }
    return 0;
}
