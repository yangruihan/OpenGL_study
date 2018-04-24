#include "Header.h"

float delta_time = 0.0f; // 当前帧与上一帧的时间差
float last_frame = 0.0f; // 上一帧的时间

float mouse_last_x;
float mouse_last_y;
bool first;

bool mouse_focus = true;

Camera camera(glm::vec3(0.0f, 0.0f, 360.0f));

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
 * 颜色与光
 */
int main()
{
    Window window(480, 480, "test4");

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

    mouse_last_x = 240.0f;
    mouse_last_y = 240.0f;

    VertexBuffer vertex_buffer(cube_vertexs, cube_vb_size);
    VertexBufferLayout vertex_buffer_layout;
    vertex_buffer_layout.push<float>(3);
    vertex_buffer_layout.push<float>(3);
    IndexBuffer index_buffer(cube_index, cube_ib_count);

    auto proj = glm::perspective(glm::radians(camera.get_zoom()), 1.0f, 0.1f, 3000.0f);
    auto view = camera.get_view_matrix();

    VertexArray obj_va;
    obj_va.add_buffer(vertex_buffer, vertex_buffer_layout, index_buffer);
    glm::vec3 obj_pos(0.0f, 0.0f, 0.0f);
    auto obj_model = glm::translate(glm::mat4(1.0f), obj_pos);
    
    VertexArray light_va;
    light_va.add_buffer(vertex_buffer, vertex_buffer_layout, index_buffer);
    glm::vec3 light_pos(120.0f, 150.0f, 200.0f);
    auto light_model = glm::translate(glm::mat4(1.0f), light_pos);
    light_model = glm::scale(light_model, glm::vec3(0.2f));

    Shader obj_shader("src/test/test7/test7_obj.shader");
    // set mvp
    obj_shader.set_mat4f("u_Proj", proj);
    obj_shader.set_mat4f("u_View", camera.get_view_matrix());
    obj_shader.set_mat4f("u_Model", obj_model);

    // set light & view
    obj_shader.set_vec3f("u_ViewPos", camera.get_position());

    obj_shader.set_vec3f("u_Light.ambient", glm::vec3(0.1f));
    obj_shader.set_vec3f("u_Light.diffuse", glm::vec3(0.8f));
    obj_shader.set_vec3f("u_Light.specular", glm::vec3(1.0f));
    obj_shader.set_vec3f("u_Light.position", light_pos);

    // set obj material
    obj_shader.set_vec3f("u_Material.ambient", 1.0f, 0.5f, 0.31f);
    obj_shader.set_vec3f("u_Material.diffuse", 1.0f, 0.5f, 0.31f);
    obj_shader.set_vec3f("u_Material.specular", 0.5f, 0.5f, 0.5f);
    obj_shader.set_float("u_Material.shininess", 32.0f);

    Shader light_shader("src/test/test7/test7_light.shader");

    Renderer renderer;

    auto current_frame = 0.0f;

    while (window.show())
    {
        current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        process_input(window.get_window());

        renderer.clear();

        proj = glm::perspective(glm::radians(camera.get_zoom()), 1.0f, 0.1f, 3000.0f);
        view = camera.get_view_matrix();

        obj_shader.set_mat4f("u_Proj", proj);
        obj_shader.set_mat4f("u_View", camera.get_view_matrix());
        obj_shader.set_mat4f("u_Model", obj_model);
        obj_shader.set_vec3f("u_ViewPos", camera.get_position());
        renderer.draw(obj_va, obj_shader);

        light_shader.set_mat4f("u_MVP", proj * view * light_model);
        renderer.draw(light_va, light_shader);

        window.clean();
    }
    return 0;
}
