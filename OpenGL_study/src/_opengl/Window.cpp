#include "Window.h"
#include <utility>

Window::Window(const unsigned int& width,
               const unsigned int& height,
               std::string title,
               const unsigned int& target_frame)
    : width_(width), height_(height), 
      title_(std::move(title)),
      target_frame_(target_frame), fixed_delta_time_(1.0 / target_frame), delta_time_(0), 
      window_(nullptr), 
      update_func_(nullptr), fixed_update_func_(nullptr), render_func_(nullptr)
{
    init();
}

Window::~Window()
{
    glfwTerminate();
}

bool Window::show(const bool& auto_clear) const
{
    const auto result = !glfwWindowShouldClose(window_);

    if (auto_clear && result)
    {
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
    }

    return result;
}

void Window::start()
{
    if (window_ == nullptr)
    {
        std::cout << "[ERROR] Window Start failed, glfwWindow is null" << std::endl;
        return;
    }

    float previous_time = glfwGetTime();
    auto lag = 0.0f;
    auto start_time = 0.0f;
    auto end_time = 0.0f;
    auto duration_time = 0.0f;
    auto sleep_time = 0.0f;

    while (!glfwWindowShouldClose(window_))
    {
        start_time = glfwGetTime();
        delta_time_ = start_time - previous_time;
        previous_time = start_time;
        lag += delta_time_;

        // update call
        if (update_func_ != nullptr)
            (update_func_)(delta_time_);

        while (lag - fixed_delta_time_ >= 0)
        {
            // fixed update call
            if (fixed_update_func_ != nullptr)
                (fixed_update_func_)(fixed_delta_time_);
            lag -= fixed_delta_time_;
        }

        // clear screen
        clear();

        // render call
        if (render_func_ != nullptr)
            (render_func_)();

        end_of_frame();

        end_time = glfwGetTime();
        duration_time = end_time - start_time;
        sleep_time = duration_time < fixed_delta_time_ ? fixed_delta_time_ - duration_time : 0;

        if (sleep_time > 0)
        {

#ifdef __APPLE__
            _sleep(sleep_time);
#else
            Sleep(sleep_time);
#endif

        }

    }
}

void Window::clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}

void Window::end_of_frame() const
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

    // 开启深度缓冲测试
    GLCall(glEnable(GL_DEPTH_TEST));
    // 开启模板缓冲测试
    GLCall(glEnable(GL_STENCIL_TEST));

    // 开启 ALPHA 混合
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
