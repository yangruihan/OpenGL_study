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

float yaw = -90.0f;
float pitch;
float mouse_last_x;
float mouse_last_y;
bool first;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (first)
    {
        mouse_last_x = xpos;
        mouse_last_y = ypos;
        first = false;
    }

    float xoffset = xpos - mouse_last_x;
    float yoffset = mouse_last_y - ypos; // 注意这里是相反的，因为y坐标是从底部往顶部依次增大的
    mouse_last_x = xpos;
    mouse_last_y = ypos;

    const auto sensitivity = 0.15f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if (pitch > 60.0f)
        pitch = 60.0f;
    if (pitch < -60.0f)
        pitch = -60.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    front.y = sin(glm::radians(pitch));
    front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    camera_front = glm::normalize(front);
}

float fov;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    const auto sensitivity = 5.0f;
    if (fov >= 1.0f && fov <= 120.0f)
        fov -= yoffset * sensitivity;
    if (fov <= 1.0f)
        fov = 1.0f;
    if (fov >= 120.0f)
        fov = 120.0f;
}

/* 漫游镜头 */
int main()
{
    Window window(480, 480, "test4");

    // set mouse mode
    glfwSetInputMode(window.get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // add mouse callback
    glfwSetCursorPosCallback(window.get_window(), mouse_callback);
    first = true;

    glfwSetScrollCallback(window.get_window(), scroll_callback);

    mouse_last_x = 240.0f;
    mouse_last_y = 240.0f;

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
        0, 1, 4,
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

    fov = 90;
    auto proj = glm::perspective(glm::radians(fov), 1.0f, 0.1f, 480.0f);
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

        proj = glm::perspective(glm::radians(fov), 1.0f, 0.1f, 480.0f);
        view = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);
        shader.set_uniform_mat4f("u_MVP", proj * view * model);
        renderer.draw(vertex_array, shader);

        window.clean();
    }
    return 0;
}
