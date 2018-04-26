#include <iostream>
#include <iomanip>
#include "Header.h"

float mouse_last_x = 240.0f;
float mouse_last_y = 240.0f;
bool first;

bool mouse_focus = true;

Camera camera(glm::vec3(0.0f, 0.0f, 360.0f));

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
    Window window(640, 640, "test12");

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

    auto proj = glm::perspective(glm::radians(camera.get_zoom()), 1.0f, 0.1f, 100.0f);
    auto view = camera.get_view_matrix();

    VertexBuffer vertex_buffer(cube_vertexs_nt, cube_v_nt_b_size);
    VertexBufferLayout vertex_buffer_layout;
    vertex_buffer_layout.push<float>(3);
    vertex_buffer_layout.push<float>(3);
    vertex_buffer_layout.push<float>(2);
    IndexBuffer index_buffer(cube_index, cube_ib_count);

    VertexArray obj_va;
    obj_va.add_buffer(vertex_buffer, vertex_buffer_layout, index_buffer);
    const auto obj_count = 8;
    glm::vec3 obj_pos[] = {
        {   0.0f,   0.0f,   0.0f },
        { 150.0f,   0.0f,   0.0f },
        {   0.0f, 350.0f,   0.0f },
        {   0.0f,   0.0f, 250.0f },
        { 450.0f, 450.0f,   0.0f },
        { 450.0f,   0.0f, 450.0f },
        {   0.0f, 450.0f, 450.0f },
        { 450.0f, 450.0f, 450.0f }
    };
    glm::mat4 obj_model = glm::mat4(1.0f);

    VertexArray light_va;
    light_va.add_buffer(vertex_buffer, vertex_buffer_layout, index_buffer);
    const auto light_count = 4;
    glm::vec3 light_pos[] = {
        {  520.0f,  650.0f,  500.0f },
        { -520.0f, -650.0f, -500.0f },
        { -520.0f,  550.0f,  650.0f },
        {  620.0f, -450.0f, -500.0f },
    };
    glm::mat4 light_model = glm::mat4(1.0f);

    Shader obj_shader("src/test/test12/test12_obj.shader");
    // set mvp
    obj_shader.set_mat4f("u_Proj", proj);
    obj_shader.set_mat4f("u_View", camera.get_view_matrix());

    Texture texture0("res/textures/container.png");
    Texture texture1("res/textures/container_specular.png");

    Shader light_shader("src/test/test12/test12_light.shader");

    Renderer renderer;
    renderer.set_clear_color(glm::vec4(0.1f));

    window.set_update_func([&] (const float delta_time)
    {
        process_input(window.get_window(), delta_time);
    });

    window.set_render_func([&] ()
    {
        texture0.bind();
        texture1.bind(1);
    
        proj = glm::perspective(glm::radians(camera.get_zoom()), 1.0f, 0.1f, 3000.0f);
        view = camera.get_view_matrix();

        // renderer object
        for (auto i = 0; i < obj_count; i++)
        {
            obj_model = glm::translate(glm::mat4(1.0f), obj_pos[i]);
    
            const auto angle = 20.0f * i;
            obj_model = glm::rotate(obj_model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            obj_model = glm::scale(obj_model, glm::vec3(0.3f));
    
            obj_shader.set_mat4f("u_Proj", proj);
            obj_shader.set_mat4f("u_View", view);
            obj_shader.set_mat4f("u_Model", obj_model);
            renderer.draw(obj_va, obj_shader);
        }
    
        // renderer light
        for (auto i = 0; i < light_count; i++)
        {
            light_model = glm::translate(glm::mat4(1.0f), light_pos[i]);
            light_model = glm::scale(light_model, glm::vec3(0.15f));
            light_shader.set_mat4f("u_Proj", proj);
            light_shader.set_mat4f("u_View", view);
            light_shader.set_mat4f("u_Model", light_model);
            renderer.draw(light_va, light_shader);
        }
    });

    window.set_debug_info(true);
    window.start();

    return 0;
}
