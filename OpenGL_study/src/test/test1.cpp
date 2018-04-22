#include "../Window.h"
#include "../VertexArray.h"

int main()
{
    const auto proj = glm::ortho(0.0f, 480.0f, 0.0f, 480.0f, -1.0f, 1.0f);

    Window window(480, 480, "test1");

    float pos1[] = {
        0.0f, 0.0f,
        100.0f, 0.0f,
        100.0f, 100.0f
    };

    unsigned int index1[] = { 0, 1, 2 };

    VertexArray va1;
    VertexBuffer vb1(pos1, 2 * 3 * sizeof(float));
    VertexBufferLayout vbl1;
    vbl1.push<float>(2);
    IndexBuffer ib1(index1, 3);
    va1.add_buffer(vb1, vbl1, ib1);

    Shader shader1("res/shaders/basic.shader");
    shader1.set_uniform_mat4f("u_MVP", proj);
    shader1.set_uniform4f("u_Color", 1, 0, 0, 1);

    float pos2[] = {
        100.0f, 0.0f,
        200.0f, 0.0f,
        100.0f, 100.0f
    };

    unsigned int index2[] = { 0, 1, 2 };

    VertexArray va2;
    VertexBuffer vb2(pos2, 2 * 3 * sizeof(float));
    VertexBufferLayout vbl2;
    vbl2.push<float>(2);
    IndexBuffer ib2(index2, 3);
    va2.add_buffer(vb2, vbl2, ib2);

    Shader shader2("res/shaders/basic.shader");
    shader2.set_uniform_mat4f("u_MVP", proj);
    shader2.set_uniform4f("u_Color", 0, 1, 0, 1);

    Renderer renderer;

    while (window.show())
    {
        renderer.clear();

        renderer.draw(va1, shader1);
        renderer.draw(va2, shader2);

        window.clean();
    }

    return 0;
}