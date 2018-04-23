#include "Header.h"

float delta_time = 0.0f; // 当前帧与上一帧的时间差
float last_frame = 0.0f; // 上一帧的时间

glm::vec3 camera_pos     = glm::vec3(0.0f, 0.0f, 360.0f);
glm::vec3 camera_front   = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camera_up      = glm::vec3(0.0f, 1.0f, 0.0f);

/**
* process input
*/
void process_input(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    const auto camera_speed = 300.0f * delta_time;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera_pos += camera_speed * camera_front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera_pos -= camera_speed * camera_front;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera_pos -= glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera_pos += glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
}

int main()
{
    Window window(480, 480, "test4");

    float pos[] = {
        -100.0f, -100.0f,  100.0f,  1.0f,  1.0f,  1.0f,
         100.0f, -100.0f,  100.0f,  1.0f,  0.0f,  0.0f,
         100.0f,  100.0f,  100.0f,  0.0f,  1.0f,  0.0f,
        -100.0f,  100.0f,  100.0f,  0.0f,  0.0f,  1.0f,
         100.0f, -100.0f, -100.0f,  1.0f,  1.0f,  0.0f,
         100.0f,  100.0f, -100.0f,  1.0f,  0.0f,  1.0f,
        -100.0f,  100.0f, -100.0f,  0.0f,  1.0f,  1.0f,
        -100.0f, -100.0f, -100.0f,  0.0f,  0.0f,  0.0f,
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
    VertexBuffer vertex_buffer(pos, 3 * 2 * 8 * sizeof(float));
    VertexBufferLayout vertex_buffer_layout;
    vertex_buffer_layout.push<float>(3);
    vertex_buffer_layout.push<float>(3);
    IndexBuffer index_buffer(index, 3 * 12);
    vertex_array.add_buffer(vertex_buffer, vertex_buffer_layout, index_buffer);

    const auto proj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 480.0f);
    auto view = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);
    auto model = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 1.0f));

    Shader shader("src/test/test5/test5.shader");
    shader.set_uniform4f("u_Color", 1, 1, 1, 1);
    shader.set_uniform_mat4f("u_MVP", proj * view * model);

    Renderer renderer;

    auto current_frame = 0.0f;

    while (window.show())
    {
        current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        process_input(window.get_window());

        renderer.clear();

        view = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);
        shader.set_uniform_mat4f("u_MVP", proj * view * model);
        renderer.draw(vertex_array, shader);

        window.clean();
    }
    return 0;
}
