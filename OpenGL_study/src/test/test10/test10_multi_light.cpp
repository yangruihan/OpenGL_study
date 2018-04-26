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
    Window window(640, 640, "test10");

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

    auto proj = glm::perspective(glm::radians(camera.get_zoom()), 1.0f, 0.1f, 3000.0f);
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

    Shader obj_shader("src/test/test10/test10_obj.shader");
    // set mvp
    obj_shader.set_mat4f("u_Proj", proj);
    obj_shader.set_mat4f("u_View", camera.get_view_matrix());

    // set view pos
    obj_shader.set_vec3f("u_ViewPos", camera.get_position());

    // set obj material
    obj_shader.set_int  ("u_Material.diffuse",   0);
    obj_shader.set_int  ("u_Material.specular",  1);
    obj_shader.set_float("u_Material.shininess", 32.0f);

    auto ambient  = glm::vec3(0.1f);
    auto diffuse  = glm::vec3(0.6f);
    auto specular = glm::vec3(1.0f);


    // set dir light
    obj_shader.set_vec3f("u_DirLight.direction", -320.0f, -1500.0f, -400.0f);
    obj_shader.set_vec3f("u_DirLight.ambient",  ambient);
    obj_shader.set_vec3f("u_DirLight.diffuse",  diffuse);
    obj_shader.set_vec3f("u_DirLight.specular", specular);

    // set point light
    std::ostringstream string_stream;
    for (auto i = 0; i < light_count; i++)
    {
        string_stream.str("");
        string_stream.clear();
        string_stream << "u_PointLights[" << i << "].";
        // set point lights
        obj_shader.set_vec3f(string_stream.str() + "position",  light_pos[0]);
        obj_shader.set_vec3f(string_stream.str() + "ambient",   ambient);
        obj_shader.set_vec3f(string_stream.str() + "diffuse",   diffuse);
        obj_shader.set_vec3f(string_stream.str() + "specular",  specular);
        // 参考 http://www.ogre3d.org/tikiwiki/tiki-index.php?page=-Point+Light+Attenuation
        obj_shader.set_float(string_stream.str() + "constant",  1.0f);
        obj_shader.set_float(string_stream.str() + "linear",    0.07f);
        obj_shader.set_float(string_stream.str() + "quadratic", 0.017f);
    }

    // set spot light
    obj_shader.set_vec3f("u_SpotLight.position",      camera.get_position());
    obj_shader.set_vec3f("u_SpotLight.direction",     camera.get_direction());
    obj_shader.set_vec3f("u_SpotLight.ambient",       ambient);
    obj_shader.set_vec3f("u_SpotLight.diffuse",       diffuse);
    obj_shader.set_vec3f("u_SpotLight.specular",      specular);
    obj_shader.set_float("u_SpotLight.cut_off",       glm::cos(glm::radians(12.5f)));
    obj_shader.set_float("u_SpotLight.outer_cut_off", glm::cos(glm::radians(17.5f)));

    obj_shader.set_float("u_DistanceRate", 100.0f);

    Texture texture0("res/textures/container.png");
    Texture texture1("res/textures/container_specular.png");

    Shader light_shader("src/test/test10/test10_light.shader");

    Renderer renderer;
    renderer.set_clear_color(glm::vec4(0.1f));

    auto current_frame = 0.0f;

    while (window.show())
    {
        current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        process_input(window.get_window());

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
            obj_shader.set_vec3f("u_ViewPos", camera.get_position());

            obj_shader.set_vec3f("u_SpotLight.position",  camera.get_position());
            obj_shader.set_vec3f("u_SpotLight.direction", camera.get_direction());

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

        window.end_of_frame();
    }
    return 0;
}
