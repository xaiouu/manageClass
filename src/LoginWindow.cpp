#include "LoginWindow.h"

/**
 * @brief 登录窗口构造函数实现
 * @details 创建并初始化登录界面的所有控件
 */
LoginWindow::LoginWindow(int w, int h, const char *title)
    : Fl_Window(w, h, title), isRegistering(false)
{
    // 计算控件的水平居中位置
    int center_x = (w - 200) / 2;

    // 创建用户名输入框
    username = new Fl_Input(center_x, 50, 200, 25, "用户名:");
    username->align(FL_ALIGN_LEFT); // 标签左对齐

    // 创建密码输入框（显示为星号）
    password = new Fl_Secret_Input(center_x, 100, 200, 25, "密码:");
    password->align(FL_ALIGN_LEFT); // 标签左对齐

    confirmPassword = new Fl_Secret_Input(center_x, 150, 200, 25, "确认密码:");
    confirmPassword->align(FL_ALIGN_LEFT);
    confirmPassword->hide(); // 初始隐藏

    // 创建登录按钮并设置回调函数
    loginBtn = new Fl_Button(center_x, 200, 95, 25, "登录");
    loginBtn->callback(onLogin, this); // 点击时调用onLogin，传入this指针

    registerBtn = new Fl_Button(center_x + 105, 200, 95, 25, "注册");
    registerBtn->callback(onSwitchMode, this);

    changePassBtn = new Fl_Button(center_x, 235, 200, 25, "修改密码");
    changePassBtn->callback(onChangePass, this);
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
    if (win->admin.login(user, pass))
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

void LoginWindow::onRegister(Fl_Widget *, void *v)
{
    LoginWindow *win = (LoginWindow *)v;
    const char *user = win->username->value();
    const char *pass = win->password->value();
    const char *confirm = win->confirmPassword->value();

    if (strcmp(pass, confirm) != 0)
    {
        fl_alert("两次输入的密码不一致！");
        return;
    }

    if (win->admin.registerAccount(user, pass))
    {
        fl_message("注册成功！");
        win->switchToLoginMode();
    }
    else
    {
        fl_alert("用户名已存在！");
    }
}

void LoginWindow::onChangePass(Fl_Widget *, void *v)
{
    LoginWindow *win = (LoginWindow *)v;
    const char *user = win->username->value();
    const char *oldPass = win->password->value();

    // 首先验证当前用户名和密码是否正确
    if (!win->admin.login(user, oldPass))
    {
        fl_alert("请先输入正确的用户名和当前密码！");
        return;
    }

    // 弹出新密码输入框
    const char *newPass = fl_input("请输入新密码：");
    if (!newPass)
        return; // 用户取消

    // 确认新密码
    const char *confirmPass = fl_input("请再次输入新密码：");
    if (!confirmPass)
        return; // 用户取消

    // 检查两次输入的新密码是否一致
    if (strcmp(newPass, confirmPass) != 0)
    {
        fl_alert("两次输入的新密码不一致！");
        return;
    }

    // 修改密码
    if (win->admin.changePassword(user, oldPass, newPass))
    {
        fl_message("密码修改成功！请使用新密码重新登录。");
        // 清空密码输入框
        win->password->value("");
    }
    else
    {
        fl_alert("密码修改失败！");
    }
}

void LoginWindow::onSwitchMode(Fl_Widget *, void *v)
{
    LoginWindow *win = (LoginWindow *)v;
    if (win->isRegistering)
    {
        win->switchToLoginMode();
    }
    else
    {
        win->switchToRegisterMode();
    }
}

void LoginWindow::switchToRegisterMode()
{
    isRegistering = true;
    confirmPassword->show();
    loginBtn->label("确认注册");
    loginBtn->callback(onRegister, this);
    registerBtn->label("返回登录");
    changePassBtn->hide();
    this->redraw();
}

void LoginWindow::switchToLoginMode()
{
    isRegistering = false;
    confirmPassword->hide();
    loginBtn->label("登录");
    loginBtn->callback(onLogin, this);
    registerBtn->label("注册");
    changePassBtn->show();
    this->redraw();
}