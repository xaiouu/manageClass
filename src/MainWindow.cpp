#include "MainWindow.h"

MainWindow::MainWindow(int w, int h, const char *title)
    : Fl_Window(w, h, title)
{

    tabs = new Fl_Tabs(10, 10, w - 20, h - 20);

    // 创建标签页
    courseTab = new Fl_Group(10, 35, w - 20, h - 45, "课程管理");
    setupCourseTab();
    courseTab->end();

    classroomTab = new Fl_Group(10, 35, w - 20, h - 45, "教室管理");
    setupClassroomTab();
    classroomTab->end();

    scheduleTab = new Fl_Group(10, 35, w - 20, h - 45, "排课管理");
    setupScheduleTab();
    scheduleTab->end();

    tabs->end();
}

void MainWindow::setupCourseTab()
{
    int x = 20, y = 45;

    courseNameInput = new Fl_Input(x + 100, y, 150, 25, "课程名称:");

    y += 35;
    dayChoice = new Fl_Choice(x + 100, y, 150, 25, "星期:");
    dayChoice->add("星期一");
    dayChoice->add("星期二");
    dayChoice->add("星期三");
    dayChoice->add("星期四");
    dayChoice->add("星期五");

    y += 35;
    startTimeHour = new Fl_Spinner(x + 100, y, 70, 25, "开始时间:");
    startTimeHour->range(8, 20);
    startTimeMin = new Fl_Spinner(x + 180, y, 70, 25);
    startTimeMin->range(0, 59);

    y += 35;
    teacherInput = new Fl_Input(x + 100, y, 150, 25, "教师:");

    y += 45;
    Fl_Button *addBtn = new Fl_Button(x, y, 70, 25, "添加");
    addBtn->callback(onAddCourse, this);

    Fl_Button *deleteBtn = new Fl_Button(x + 80, y, 70, 25, "删除");
    deleteBtn->callback(onDeleteCourse, this);

    // 添加课程表格
    courseTable = new CourseTable(x, y + 35, w() - 50, h() - 200);
    courseTable->col_header(1);
    courseTable->row_header(1);
    courseTable->cols(5);
    courseTable->col_width_all(120);
    courseTable->row_height_all(25);
}

void MainWindow::onAddCourse(Fl_Widget *, void *v)
{
    MainWindow *win = (MainWindow *)v;

    // 获取输入数据
    const char *name = win->courseNameInput->value();
    const char *day = win->dayChoice->text();
    int hour = win->startTimeHour->value();
    int min = win->startTimeMin->value();
    const char *teacher = win->teacherInput->value();

    if (strlen(name) == 0)
    {
        fl_alert("请输入课程名称！");
        return;
    }

    // 添加到表格
    std::vector<std::string> rowData;
    rowData.push_back(name);
    rowData.push_back(day);
    char time[10];
    sprintf(time, "%02d:%02d", hour, min);
    rowData.push_back(time);
    rowData.push_back(teacher);

    win->courseTable->addRow(rowData);
}

void MainWindow::setupClassroomTab()
{
    int x = 20, y = 45;

    // 教室信息输入
    Fl_Input *roomNumInput = new Fl_Input(x + 100, y, 150, 25, "教室编号:");

    y += 35;
    Fl_Spinner *capacityInput = new Fl_Spinner(x + 100, y, 150, 25, "容纳人数:");
    capacityInput->range(1, 200);

    y += 35;
    Fl_Choice *typeChoice = new Fl_Choice(x + 100, y, 150, 25, "教室类型:");
    typeChoice->add("普通教室");
    typeChoice->add("多媒体教室");
    typeChoice->add("实验室");
    typeChoice->value(0);

    y += 45;
    Fl_Button *addRoomBtn = new Fl_Button(x, y, 70, 25, "添加");
    addRoomBtn->callback(onAddClassroom, this);

    Fl_Button *deleteRoomBtn = new Fl_Button(x + 80, y, 70, 25, "删除");
    deleteRoomBtn->callback(onDeleteClassroom, this);

    // 教室列表表格
    classroomTable = new CourseTable(x, y + 35, w() - 50, h() - 200);
    classroomTable->col_header(1);
    classroomTable->row_header(1);
    classroomTable->cols(3);
    classroomTable->col_width_all(120);
    classroomTable->row_height_all(25);
}

void MainWindow::setupScheduleTab()
{
    int x = 20, y = 45;

    // 排课信息选择
    courseChoice = new Fl_Choice(x + 100, y, 150, 25, "选择课程:");
    updateCourseChoice(); // 更新课程列表

    y += 35;
    roomChoice = new Fl_Choice(x + 100, y, 150, 25, "选择教室:");
    updateRoomChoice(); // 更新教室列表

    y += 35;
    Fl_Choice *weekChoice = new Fl_Choice(x + 100, y, 150, 25, "选择周次:");
    for (int i = 1; i <= 20; i++)
    {
        char buf[20];
        sprintf(buf, "第%d周", i);
        weekChoice->add(buf);
    }

    y += 45;
    Fl_Button *arrangeBtn = new Fl_Button(x, y, 90, 25, "安排课程");
    arrangeBtn->callback(onArrangeCourse, this);

    // 课表显示
    scheduleTable = new CourseTable(x, y + 35, w() - 50, h() - 200);
    scheduleTable->col_header(1);
    scheduleTable->row_header(1);
    scheduleTable->cols(6);  // 周一到周五，加上时间列
    scheduleTable->rows(13); // 12节课 + 表头
    scheduleTable->col_width_all(120);
    scheduleTable->row_height_all(25);
    initScheduleTable();
}

void MainWindow::initScheduleTable()
{
    if (!scheduleTable)
        return;

    // 设置表头
    std::vector<std::string> headers = {"时间", "周一", "周二", "周三", "周四", "周五"};
    scheduleTable->addRow(headers);

    // 设置时间列
    for (int i = 1; i <= 12; i++)
    {
        char timeStr[20];
        sprintf(timeStr, "第%d节", i);
        std::vector<std::string> row = {timeStr, "", "", "", "", ""};
        scheduleTable->addRow(row);
    }
}

void MainWindow::onAddClassroom(Fl_Widget *, void *v)
{
    MainWindow *win = (MainWindow *)v;
    Fl_Input *roomNum = (Fl_Input *)win->classroomTab->child(0);
    Fl_Spinner *capacity = (Fl_Spinner *)win->classroomTab->child(1);
    Fl_Choice *type = (Fl_Choice *)win->classroomTab->child(2);

    if (strlen(roomNum->value()) == 0)
    {
        fl_alert("请输入教室编号！");
        return;
    }

    std::vector<std::string> rowData;
    rowData.push_back(roomNum->value());
    char capStr[10];
    sprintf(capStr, "%d", (int)capacity->value());
    rowData.push_back(capStr);
    rowData.push_back(type->text());

    win->classroomTable->addRow(rowData);
    win->updateRoomChoice(); // 更新教室选择列表
}

void MainWindow::onArrangeCourse(Fl_Widget *, void *v)
{
    MainWindow *win = (MainWindow *)v;

    if (win->courseChoice->value() < 0)
    {
        fl_alert("请选择课程！");
        return;
    }
    if (win->roomChoice->value() < 0)
    {
        fl_alert("请选择教室！");
        return;
    }

    // 这里添加排课逻辑
    // 可以检查时间冲突等

    fl_message("课程安排成功！");
    win->updateScheduleDisplay();
}

void MainWindow::updateCourseChoice()
{
    courseChoice->clear();

    // 获取课程表格中的所有课程
    for (int i = 0; i < courseTable->rows() - 1; i++)
    {
        std::string courseName = courseTable->get_cell_value(i + 1, 0);
        if (!courseName.empty())
        {
            courseChoice->add(courseName.c_str());
        }
    }
}

void MainWindow::updateRoomChoice()
{
    roomChoice->clear();

    // 获取教室表格中的所有教室
    for (int i = 0; i < classroomTable->rows() - 1; i++)
    {
        std::string roomNum = classroomTable->get_cell_value(i + 1, 0);
        if (!roomNum.empty())
        {
            roomChoice->add(roomNum.c_str());
        }
    }
}

void MainWindow::updateScheduleDisplay()
{
    // 清空现有课表内容（保留表头和时间列）
    for (int row = 1; row < scheduleTable->rows(); row++)
    {
        for (int col = 1; col < scheduleTable->cols(); col++)
        {
            scheduleTable->set_cell_value(row, col, "");
        }
    }

    // 获取选中的课程和教室
    const char *courseName = courseChoice->text();
    const char *roomNum = roomChoice->text();
    if (!courseName || !roomNum)
        return;

    // 在对应时间段显示课程信息
    int row = courseChoice->value() + 1; // 假设课程顺序对应时间段
    int col = dayChoice->value() + 1;    // 星期几对应的列

    std::string cellContent = std::string(courseName) + "\n" + roomNum;
    scheduleTable->set_cell_value(row, col, cellContent.c_str());
}

void MainWindow::onDeleteCourse(Fl_Widget *, void *v)
{
    MainWindow *win = (MainWindow *)v;
    int row = win->courseTable->callback_row();
    if (row == -1)
    {
        fl_alert("请选择要删除的课程！");
        return;
    }

    if (fl_choice("确定要删除选中的课程吗？", "取消", "确定", 0) == 1)
    {
        // 从表格中删除该行
        win->courseTable->clearData();

        // 更新课程选择列表
        win->updateCourseChoice();

        fl_message("课程已删除！");
    }
}

void MainWindow::onDeleteClassroom(Fl_Widget *, void *v)
{
    MainWindow *win = (MainWindow *)v;
    int row = win->classroomTable->callback_row();
    if (row == -1)
    {
        fl_alert("请选择要删除的教室！");
        return;
    }

    if (fl_choice("确定要删除选中的教室吗？", "取消", "确定", 0) == 1)
    {
        // 从表格中删除该行
        win->classroomTable->clearData();

        // 更新教室选择列表
        win->updateRoomChoice();

        fl_message("教室已删除！");
    }
}