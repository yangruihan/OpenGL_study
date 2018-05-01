#include <iostream>
#include <iomanip>
#include "Header.h"

float delta_time = 0.0f; // 当前帧与上一帧的时间差
float last_frame = 0.0f; // 上一帧的时间

float mouse_last_x = 240.0f;
float mouse_last_y = 240.0f;
bool first;

bool mouse_focus = true;

Camera camera(glm::vec3(0.0f, 0.0f, 360.0f));
Window window(640, 640, "test11_model");

/**
* process input
*/
void process_input(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.process_keyboard(FORWARD, delta_time);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.process_keyboard(BACKWARD, delta_time);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.process_keyboard(LEFT, delta_time);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.process_keyboard(RIGHT, delta_time);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.process_keyboard(UP, delta_time);

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.process_keyboard(DOWN, delta_time);
}

/**
 * key callback
 */
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
    {
        if (mouse_focus)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        
        mouse_focus = !mouse_focus;
    }
    
    // set default size
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        glfwSetWindowSize(window, ::window.get_width(), ::window.get_height());
    }
    
    if (key == GLFW_KEY_LEFT_BRACKET && action == GLFW_PRESS)
    {
        glfwSetWindowSize(window, ::window.get_width() + 100, ::window.get_height() + 100);
    }
    
    if (key == GLFW_KEY_RIGHT_BRACKET && action == GLFW_PRESS)
    {
        glfwSetWindowSize(window, ::window.get_width() - 100, ::window.get_height() - 100);
    }
}

/**
* mouse callback
*/
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (first)
    {
        mouse_last_x = xpos;
        mouse_last_y = ypos;
        first = false;
    }

    const auto xoffset = xpos - mouse_last_x;
    const auto yoffset = mouse_last_y - ypos; // 注意这里是相反的，因为y坐标是从底部往顶部依次增大的
    mouse_last_x = xpos;
    mouse_last_y = ypos;

    camera.process_mouse_movement(xoffset, yoffset);
}

/**
* mouse scroll callback
*/
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.process_mouse_scroll(yoffset);
}

/**
 * 模型
 */
int main()
{
    // set mouse mode
    if (mouse_focus)
        glfwSetInputMode(window.get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // add mouse callback
    glfwSetCursorPosCallback(window.get_window(), mouse_callback);
    first = true;

    // mouse scroll callback
    glfwSetScrollCallback(window.get_window(), scroll_callback);

    // key callback
    glfwSetKeyCallback(window.get_window(), key_callback);

    auto proj_mat = glm::perspective(glm::radians(camera.get_zoom()), 1.0f, 0.1f, 3000.0f);
    auto view_mat = camera.get_view_matrix();
    auto model_mat = glm::mat4(1.0f);
    model_mat = glm::scale(model_mat, glm::vec3(2.0f));

    Shader obj_shader("src/test/test11/test11_obj.shader");
    obj_shader.set_mat4f("u_Proj", proj_mat);
    obj_shader.set_mat4f("u_View", view_mat);

    Model model("res/model/nanosuit.obj");

    Renderer renderer;
    renderer.set_clear_color(glm::vec4(0.5f));

    auto current_frame = 0.0f;

    while (window.show())
    {
        current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        process_input(window.get_window());

        proj_mat = glm::perspective(glm::radians(camera.get_zoom()), 1.0f, 0.1f, 3000.0f);
        view_mat = camera.get_view_matrix();

        obj_shader.set_mat4f("u_Proj", proj_mat);
        obj_shader.set_mat4f("u_View", view_mat);
        obj_shader.set_mat4f("u_Model", model_mat);

        renderer.draw(model, obj_shader);

        window.end_of_frame();
    }
    return 0;
}
