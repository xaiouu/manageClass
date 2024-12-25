#include "LoginWindow.h"

/**
 * @brief 登录窗口构造函数实现
 * @details 创建并初始化登录界面的所有控件
 */
LoginWindow::LoginWindow(int w, int h, const char *title)
    : Fl_Window(w, h, title)
{
    // 计算控件的水平居中位置
    int center_x = (w - 200) / 2;

    // 创建用户名输入框
    username = new Fl_Input(center_x, 50, 200, 25, "用户名:");
    username->align(FL_ALIGN_LEFT); // 标签左对齐

    // 创建密码输入框（显示为星号）
    password = new Fl_Secret_Input(center_x, 100, 200, 25, "密码:");
    password->align(FL_ALIGN_LEFT); // 标签左对齐

    // 创建登录按钮并设置回调函数
    loginBtn = new Fl_Button(center_x, 150, 200, 25, "登录");
    loginBtn->callback(onLogin, this); // 点击时调用onLogin，传入this指针
}

/**
 * @brief 登录按钮点击事件处理
 * @details 验证用户输入，成功则创建主窗口，失败则显示错误提示
 */
void LoginWindow::onLogin(Fl_Widget *, void *v)
{
    LoginWindow *win = (LoginWindow *)v;
    const char *user = win->username->value(); // 获取用户名
    const char *pass = win->password->value(); // 获取密码

    // 验证用户名和密码
    if (win->checkCredentials(user, pass))
    {
        // 登录成功：创建并显示主窗口
        MainWindow *mainWin = new MainWindow(1000, 800, "课程管理系统");
        mainWin->show();
        win->hide(); // 隐藏登录窗口
    }
    else
    {
        // 登录失败：显示错误提示
        fl_alert("用户名或密码错误！");
    }
}

/**
 * @brief 验证用户凭据
 * @details 目前使用硬编码的用户名和密码，实际应用中应该连接数据库验证
 * @return 验证是否通过
 */
bool LoginWindow::checkCredentials(const char *user, const char *pass)
{
    // 简单的验证：用户名为"admin"，密码为"123456"
    return strcmp(user, "admin") == 0 && strcmp(pass, "123456") == 0;
}