#include <iostream>
#include "LoginWindow.h"
#include "MainWindow.h"
#include "CourseTable.h"
#include <FL/Fl.H>

int main(int argc, char *argv[])
{
    // 创建登录窗口
    LoginWindow *login = new LoginWindow(400, 300, "课程管理系统 - 登录");

    // 窗口居中显示
    int screen_w = Fl::w(); // 获取屏幕宽度
    int screen_h = Fl::h(); // 获取屏幕高度
    login->position(
        (screen_w - login->w()) / 2,
        (screen_h - login->h()) / 2);

    // 显示窗口
    login->show(argc, argv);

    // 运行事件循环
    return Fl::run();
}