#include <iostream>
#include <iomanip>
#include "Header.h"

float mouse_last_x = 240.0f;
float mouse_last_y = 240.0f;
bool first;

bool mouse_focus = true;

Camera camera(glm::vec3(0.0f, 0.0f, 360.0f));
Window window(640, 640, "test14_blend");

/**
* process input
*/
void process_input(GLFWwindow *window, const float delta_time)
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
* z test
*/
int main()
{
    // set mouse mode
    if (mouse_focus)
        window.set_cursor_mode(CursorMode::disabled);

    // add mouse callback
    window.set_cursor_pos_callback(mouse_callback);
    first = true;

    // mouse scroll callback
    window.set_scroll_callback(scroll_callback);

    // key callback
    window.set_key_callback(key_callback);

    auto proj = glm::perspective(glm::radians(camera.get_zoom()), 1.0f, 0.1f, 3000.0f);
    auto view = camera.get_view_matrix();

    VertexBuffer vertex_buffer(rect_vertexs_t, rect_v_t_b_size);
    VertexBufferLayout vertex_buffer_layout;
    vertex_buffer_layout.push<float>(3);
    vertex_buffer_layout.push<float>(2);
    IndexBuffer index_buffer(rect_index, rect_ib_count);

    VertexArray obj_va;
    obj_va.add_buffer(vertex_buffer, vertex_buffer_layout, index_buffer);
    const auto obj_count = 1;
    glm::vec3 obj_pos{ 0.0f, 0.0f, 0.0f };
    glm::mat4 model = glm::mat4(1.0f);

    Shader obj_shader("src/test/test14/test14_obj.shader");

    Texture texture0("res/textures/grass.png");
    Texture texture1("res/textures/blending_transparent_window.png");

    Renderer renderer;
    renderer.set_clear_color(glm::vec4(0.1f));

    window.set_update_func([&] (const float delta_time)
    {
        process_input(window.get_window(), delta_time);
    });

    texture0.bind();
    texture1.bind(1);

    window.set_render_func([&]()
    {
        proj = glm::perspective(glm::radians(camera.get_zoom()), 1.0f, 0.1f, 3000.0f);
        view = camera.get_view_matrix();
        model = glm::translate(glm::mat4(1.0f), obj_pos);

        obj_shader.set_int("u_Texture", 0);
        obj_shader.set_mat4f("u_Proj", proj);
        obj_shader.set_mat4f("u_View", view);

        for (int i = 0; i < 4; i++)
        {
            auto new_model = glm::rotate(model, glm::radians(45.0f * i), glm::vec3(0.0f, 1.0f, 0.0f));
            obj_shader.set_mat4f("u_Model", new_model);
            renderer.draw(obj_va, obj_shader);
        }

        auto window_model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 100.0f));
        obj_shader.set_int("u_Texture", 1);
        obj_shader.set_mat4f("u_Model", window_model);
        renderer.draw(obj_va, obj_shader);
    });

    window.set_debug_info(true);
    window.start();

    return 0;
}
