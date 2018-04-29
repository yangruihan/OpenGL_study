#include <iostream>
#include <iomanip>
#include "Header.h"

float mouse_last_x = 240.0f;
float mouse_last_y = 240.0f;
bool first;

bool mouse_focus = true;

Camera camera(glm::vec3(0.0f, 0.0f, 360.0f));
Window window(640, 640, "test15");

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

    VertexBuffer cube_vb(cube_vertexs_nt, cube_v_nt_b_size);
    VertexBufferLayout cube_vb_layout;
    cube_vb_layout.push<float>(3);
    cube_vb_layout.push<float>(3);
    cube_vb_layout.push<float>(2);
    IndexBuffer cube_ib(cube_index, cube_ib_count);
    VertexArray cube_va;
    cube_va.add_buffer(cube_vb, cube_vb_layout, cube_ib);
    glm::vec3 cube_pos{ 0.0f, 0.0f, 0.0f };
    glm::mat4 cube_model = glm::mat4(1.0f);
    
    VertexBuffer rect_vb(rect_vertexs_t, rect_v_t_b_size);
    VertexBufferLayout rect_vb_layout;
    rect_vb_layout.push<float>(3);
    rect_vb_layout.push<float>(2);
    IndexBuffer rect_ib(rect_index, rect_ib_count);
    VertexArray rect_va;
    rect_va.add_buffer(rect_vb, rect_vb_layout, rect_ib);
    glm::vec3 rect_pos{ 0.0f, 0.0f, 0.0f };
    glm::mat4 rect_model = glm::mat4(1.0f);

    Shader cube_shader("src/test/test15/test15_cube.shader");
    Shader rect_shader("src/test/test15/test15_rect.shader");

    Texture texture0("res/textures/container.png");
    
    FrameBuffer framebuffer;
    framebuffer.add_texture_attachment(FB_ATTACHMENT_TYPE::Color,
                                       window.get_width(),
                                       window.get_height());
    framebuffer.add_render_buffer_attachment(FB_ATTACHMENT_TYPE::Depth_Stencil,
                                                window.get_width(),
                                                window.get_height());

    Renderer renderer;
    renderer.set_clear_color(glm::vec4(0.1f));

    window.set_update_func([&] (const float delta_time)
    {
        process_input(window.get_window(), delta_time);
    });

    texture0.bind();

    window.set_render_func([&]()
    {
        proj = glm::perspective(glm::radians(camera.get_zoom()), 1.0f, 0.1f, 3000.0f);
        view = camera.get_view_matrix();
        cube_model = glm::translate(glm::mat4(1.0f), cube_pos);
        cube_model = glm::rotate(cube_model, glm::radians(45.0f), glm::vec3(1.0f, 1.0f, 0.0f));
        
        rect_model = glm::translate(glm::mat4(1.0f), rect_pos);
        rect_model = glm::scale(rect_model, glm::vec3(window.get_width() / 100.0f,
                                                      window.get_height() / 100.0f,
                                                      1.0f));

        // step one
        framebuffer.bind();
        texture0.bind();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        
        cube_shader.set_int("u_Texture", 0);
        cube_shader.set_mat4f("u_Proj", proj);
        cube_shader.set_mat4f("u_View", view);
        cube_shader.set_mat4f("u_Model", cube_model);
        renderer.draw(cube_va, cube_shader);
        
        framebuffer.unbind();
        framebuffer.bind_texture(FB_ATTACHMENT_TYPE::Color);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);
        rect_shader.set_int("u_Texture", 0);
        rect_shader.set_mat4f("u_Proj", proj);
        rect_shader.set_mat4f("u_View", view);
        rect_shader.set_mat4f("u_Model", rect_model);
        renderer.draw(rect_va, rect_shader);
    });

    window.set_debug_info(false);
    window.start();

    return 0;
}
