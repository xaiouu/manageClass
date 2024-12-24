#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Spinner.H>
#include <FL/Fl_Button.H>
#include <FL/fl_ask.H>
#include "CourseTable.h"
#include "Classroom.h"
#include "Course.h"
#include "Schedule.h"
#include <vector>
#include <string>

/**
 * @brief 主窗口类，继承自 Fl_Window
 * @details 包含三个主要功能标签页：课程管理、教室管理和排课管理
 */
class MainWindow : public Fl_Window
{
private:
    // === 管理器对象 ===
    ClassroomManager *classroomManager; ///< 教室管理器
    CourseManager *courseManager;
    Schedule *schedule; ///< 添加排课管理器

    // === 界面控件 ===
    Fl_Tabs *tabs;          ///< 标签页容器
    Fl_Group *courseTab;    ///< 课程管理标签页
    Fl_Group *classroomTab; ///< 教室管理标签页
    Fl_Group *scheduleTab;  ///< 排课管理标签页

    // === 课程管理控件 ===
    Fl_Input *courseNameInput;     ///< 课程名称输入框
    Fl_Choice *dayChoice;          ///< 星期选择下拉框
    Fl_Spinner *startTimeHour;     ///< 开始时间（小时）选择器
    Fl_Spinner *startTimeMin;      ///< 开始时间（分钟）选择器
    Fl_Spinner *endTimeHour;       // 新增结束时间
    Fl_Spinner *endTimeMin;        // 新增结束时间
    Fl_Spinner *studentCountInput; // 新增
    Fl_Input *teacherInput;        ///< 教师姓名输入框
    CourseTable *courseTable;      ///< 课程列表表格

    // === 教室和排课控件 ===
    CourseTable *classroomTable; ///< 教室列表表格
    CourseTable *scheduleTable;  ///< 课表显示表格
    Fl_Choice *courseChoice;     ///< 课程选择下拉框
    Fl_Choice *roomChoice;       ///< 教室选择下拉框

    // === 回调函数 ===
    static void onAddCourse(Fl_Widget *, void *v);       ///< 添加课程按钮回调
    static void onDeleteCourse(Fl_Widget *, void *v);    ///< 删除课程按钮回调
    static void onAddClassroom(Fl_Widget *, void *v);    ///< 添加教室按钮回调
    static void onDeleteClassroom(Fl_Widget *, void *v); ///< 删除教室按钮回调
    static void onArrangeCourse(Fl_Widget *, void *v);   ///< 安排课程按钮回调

    // === 辅助函数 ===
    void setupCourseTab();        ///< 初始化课程管理标签页
    void setupClassroomTab();     ///< 初始化教室管理标签页
    void setupScheduleTab();      ///< 初始化排课管理标签页
    void updateCourseChoice();    ///< 更新课程选择下拉框
    void updateRoomChoice();      ///< 更新教室选择下拉框
    void updateScheduleDisplay(); ///< 更新课表显示
    void initScheduleTable();     ///< 初始化课表

public:
    /**
     * @brief 构造函数
     * @param w 窗口宽度
     * @param h 窗口高度
     * @param title 窗口标题
     */
    MainWindow(int w, int h, const char *title);
    ~MainWindow() {}
};

#endif