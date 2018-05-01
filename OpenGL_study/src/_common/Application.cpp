#include "Header.h"

using namespace std;

void process_input(GLFWwindow *window);

int main()
{
    Window window(640, 480, "Hello OpenGL");

    float positions[] = {
        -100.0f, -100.0f,  0.0f,  0.0f,  0.0f,
         100.0f, -100.0f,  0.0f,  1.0f,  0.0f,
         100.0f,  100.0f,  0.0f,  1.0f,  1.0f,
        -100.0f,  100.0f,  0.0f,  0.0f,  1.0f,
    };

    unsigned int index[] = {
        0, 1, 2,
        2, 3, 0,
    };

    VertexArray vertex_array;
    VertexBuffer vertex_buffer(positions, 5 * 4 * sizeof(float));
    VertexBufferLayout vertex_buffer_layout;
    vertex_buffer_layout.push<float>(3);
    vertex_buffer_layout.push<float>(2);
    IndexBuffer index_buffer(index, 6);
    
    vertex_array.add_buffer(vertex_buffer,
                            vertex_buffer_layout,
                            index_buffer);

    // projection matrix
    const auto proj = glm::ortho(-320.0f, 320.0f, -240.0f, 240.0f, -1.0f, 1.0f);
    // view transform matrix
    const auto view = glm::translate(glm::mat4(1.0f), -glm::vec3(-100.0f, 100.0f, 0.0f));
    // model transform matrix
    const auto model = glm::rotate(glm::mat4(1.0f), 1.57f, glm::vec3(0.0f, 0.0f, 1.0f));

    const auto mvp = proj * view * model;

    Shader shader("res/shaders/texture.shader");
    shader.bind();

    shader.set_mat4f("u_MVP", mvp);
    //shader.set_uniform4f("u_Color", 1.0f, 0.0f, 0.0f, 1.0f);
    
    Texture texture("res/textures/hello.png");
    texture.bind();

    // set texture
    shader.set_int("m_Texture", 0);

    vertex_array.unbind();
    shader.unbind();

    window.set_update_func([&](float deltatime)
    {
        process_input(window.get_window());
    });

    window.set_render_func([&]()
    {
        window.draw(vertex_array, shader);
    });

    window.set_debug_info(true);
    window.start();

    return 0;
}

/**
* process input
*/
void process_input(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
