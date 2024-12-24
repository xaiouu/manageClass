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
#include <vector>
#include <string>

class MainWindow : public Fl_Window
{
private:
    Fl_Tabs *tabs;
    Fl_Group *courseTab;
    Fl_Group *classroomTab;
    Fl_Group *scheduleTab;

    // 课程管理控件
    Fl_Input *courseNameInput;
    Fl_Choice *dayChoice;
    Fl_Spinner *startTimeHour;
    Fl_Spinner *startTimeMin;
    Fl_Input *teacherInput;
    CourseTable *courseTable;

    CourseTable *classroomTable;
    CourseTable *scheduleTable;
    Fl_Choice *courseChoice;
    Fl_Choice *roomChoice;

    static void onAddCourse(Fl_Widget *, void *v);
    static void onDeleteCourse(Fl_Widget *, void *v);
    void setupCourseTab();
    void setupClassroomTab();
    void setupScheduleTab();

    static void onAddClassroom(Fl_Widget *, void *v);
    static void onDeleteClassroom(Fl_Widget *, void *v);
    static void onArrangeCourse(Fl_Widget *, void *v);
    void updateCourseChoice();
    void updateRoomChoice();
    void updateScheduleDisplay();
    void initScheduleTable();

public:
    MainWindow(int w, int h, const char *title);
    ~MainWindow() {}
};

#endif