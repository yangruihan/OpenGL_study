#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include "Common.h"

class Window
{
private:
    unsigned int width_;
    unsigned int height_;
    std::string title_;

    GLFWwindow *window_;

public:
    Window(const unsigned int width, const unsigned int height, const std::string title);
    ~Window();

    bool show() const;
    void clean() const;

    inline GLFWwindow* get_window() const { return window_; }

private:
    bool init();

    static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
};

