#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <functional>
#include <utility>
#include <iomanip>

#ifdef __APPLE__
#include <unistd.h>
#else
#include <Windows.h>
#endif

#include "Common.h"

enum CursorMode
{
    normal,
    disabled
};

class Window
{
private:
    unsigned int width_;            // 宽度
    unsigned int height_;           // 高度
    std::string title_;             // 标题

    unsigned int target_frame_;     // 目标帧率
    float fixed_delta_time_;        // 预计更新间隔时间
    float delta_time_;              // 实际更新间隔时间

    bool cull_face_;                // 背面剔除
    bool v_sync_;                   // 垂直同步

    CursorMode cursor_mode_;        // 指针模式

    GLFWwindow *window_;

    std::function<void(const float)> update_func_;
    std::function<void(const float)> fixed_update_func_;
    std::function<void(void)>        render_func_;

    bool debug_info_;               // 是否显示 debug 信息

public:
    Window(const unsigned int& width,
           const unsigned int& height,
           std::string title,
           const unsigned int& target_frame = 60,
           const bool& cull_face = false,
           const bool& v_sync = true,
           const bool& debug_info = false);
    ~Window();

    // --- old api --- //
    bool show(const bool& auto_clear = true) const;

    // --- new api --- //
    void start();

    void clear() const;
    void end_of_frame() const;


    // --- set && get ---//
    inline void set_update_func(const std::function<void(float)>& update_func)             { update_func_ = update_func; }
    inline void set_fixed_update_func(const std::function<void(float)>& fixed_update_func) { fixed_update_func_ = fixed_update_func; }
    inline void set_render_func(const std::function<void()>& render_func)                  { render_func_ = render_func; }
    
    inline GLFWwindow* get_window() const { return window_; }

    inline int get_target_frame() const { return target_frame_; }
    inline void set_target_frame(const unsigned int target_frame)
    {
        target_frame_ = target_frame;
        fixed_delta_time_ = 1.0f / target_frame;
    }

    inline bool get_cull_face() const { return cull_face_; }
    inline void set_cull_face(const bool cull_face)
    {
        cull_face_ = cull_face;
        if (cull_face_)
        {
            GLCall(glEnable(GL_CULL_FACE));
        }
        else
        {
            GLCall(glDisable(GL_CULL_FACE));
        }
    }

    inline bool get_v_sync() const { return v_sync_; }
    inline void set_v_sync(const bool v_sync)
    {
        v_sync_ = v_sync;
        glfwSwapInterval(v_sync_ ? 1 : 0);
    }

    inline bool get_debug_info() const { return debug_info_; }
    inline void set_debug_info(const bool debug_info) { debug_info_ = debug_info; }

    inline void set_cursor_pos_callback(const GLFWcursorposfun cbfun) const { glfwSetCursorPosCallback(window_, cbfun); }
    inline void set_scroll_callback(const GLFWscrollfun cbfun) const        { glfwSetScrollCallback(window_, cbfun); }
    inline void set_key_callback(const GLFWkeyfun cbfun) const              { glfwSetKeyCallback(window_, cbfun); }

    inline CursorMode get_cursor_mode() const { return cursor_mode_; }
    void set_cursor_mode(const CursorMode mode);

private:
    bool init();        // 初始化
    void main_loop();   // 主循环

    static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
};

