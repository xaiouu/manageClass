#ifndef LOGIN_WINDOW_H
#define LOGIN_WINDOW_H

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Secret_Input.H>
#include <FL/Fl_Button.H>
#include <FL/fl_ask.H>
#include "MainWindow.h"

/**
 * @brief 登录窗口类
 * @inherit Fl_Window FLTK的窗口基类
 * @details 实现了一个简单的登录界面，包含用户名输入框、密码输入框和登录按钮
 */
class LoginWindow : public Fl_Window
{
private:
    Fl_Input *username;        // 用户名输入框
    Fl_Secret_Input *password; // 密码输入框（显示为星号）
    Fl_Button *loginBtn;       // 登录按钮

    /**
     * @brief 登录按钮的回调函数
     * @param v 通常是LoginWindow对象指针
     */
    static void onLogin(Fl_Widget *, void *v);

    /**
     * @brief 验证用户名和密码
     * @param user 用户名
     * @param pass 密码
     * @return 验证是否通过
     */
    bool checkCredentials(const char *user, const char *pass);

public:
    /**
     * @brief 构造函数
     * @param w,h 窗口大小
     * @param title 窗口标题
     */
    LoginWindow(int w, int h, const char *title);
    ~LoginWindow() {}
};

#endif