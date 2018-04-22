#include "Window.h"

Window::Window(const unsigned width, const unsigned height, const std::string title)
    : width_(width), height_(height), title_(title), window_(nullptr)
{
    init();
}

Window::~Window()
{
    glfwTerminate();
}

bool Window::show() const
{
    return !glfwWindowShouldClose(window_);
}

void Window::clean() const
{
    /* Swap front and back buffers */
    GLCall(glfwSwapBuffers(window_));

    /* Poll for and process events */
    GLCall(glfwPollEvents());
}

bool Window::init()
{
    /* Initialize the library */
    if (!glfwInit())
    {
        std::cout << "Window Create Error: glfw init fail!" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window_ = glfwCreateWindow(width_,
                               height_,
                               title_.c_str(), nullptr, nullptr);
    if (!window_)
    {
        std::cout << "Window Create Error: glfw create window fail!" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window_);

    glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback);
    
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
    {   
        std::cout << "Window Create Error: glew init error" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    // set view port
    GLCall(glViewport(0, 0, width_, height_));

    // line mode or fill mode
#if 0
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);   
#endif

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    return true;
}

/**
 * frame buffer resize callback 
 */
void Window::framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    GLCall(glViewport(0, 0, width, height));
}
