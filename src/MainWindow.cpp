#include "MainWindow.h"
#include "Classroom.h"
#include "Course.h"
#include "Schedule.h"

/**
 * @brief 主窗口构造函数
 * @details 创建主窗口的基本框架，包含三个功能标签页
 * @param w 窗口宽度
 * @param h 窗口高度
 * @param title 窗口标题
 */
MainWindow::MainWindow(int w, int h, const char *title)
    : Fl_Window(w, h, title),
      classroomManager(new ClassroomManager()),
      courseManager(new CourseManager()),
      schedule(new Schedule())
{
    // 创建标签页容器，设置边距为10像素
    tabs = new Fl_Tabs(10, 10, w - 20, h - 20);

    // 创建并初始化三个功能标签页
    courseTab = new Fl_Group(10, 35, w - 20, h - 45, "课程管理");
    setupCourseTab(); // 设置课程管理页面的控件
    courseTab->end(); // 结束当前组件的添加

    classroomTab = new Fl_Group(10, 35, w - 20, h - 45, "教室管理");
    setupClassroomTab(); // 设置教室管理页面的控件
    classroomTab->end();

    scheduleTab = new Fl_Group(10, 35, w - 20, h - 45, "排课管理");
    setupScheduleTab(); // 设置排课管理页面的控件
    scheduleTab->end();

    tabs->end(); // 结束标签页容器的设置

    // 加载数据并更新界面
    loadData();
    updateCourseChoice();
    updateRoomChoice();
}

/**
 * @brief 设置课程管理标签页
 * @details 创建并布局课程信息输入区域和课程列表表格
 */
void MainWindow::setupCourseTab()
{
    int x = 20, y = 45; // 起始坐标

    // === 课程信息输入区域 ===
    // 课程名称输入框
    courseNameInput = new Fl_Input(x + 100, y, 150, 25, "课程名称:");

    // 星期选择下拉框
    y += 35; // 向下移动35像素
    dayChoice = new Fl_Choice(x + 100, y, 150, 25, "星期:");
    dayChoice->add("星期一");
    dayChoice->add("星期二");
    dayChoice->add("星期三");
    dayChoice->add("星期四");
    dayChoice->add("星期五");

    // 开始时间选择器
    y += 35;
    startTimeHour = new Fl_Spinner(x + 100, y, 70, 25, "开始时间:");
    startTimeHour->range(8, 20); // 设置小时范围：8:00-20:00
    startTimeMin = new Fl_Spinner(x + 180, y, 70, 25);
    startTimeMin->range(0, 59); // 设置分钟范围：0-59

    // 添加结束时间选择器
    y += 35;
    endTimeHour = new Fl_Spinner(x + 100, y, 70, 25, "结束时间:");
    endTimeHour->range(8, 20);
    endTimeMin = new Fl_Spinner(x + 180, y, 70, 25);
    endTimeMin->range(0, 59);

    // 添加学生人数输入
    y += 35;
    studentCountInput = new Fl_Spinner(x + 100, y, 150, 25, "学生人数:");
    studentCountInput->range(1, 200); // 设置范围1-200人

    // 教师姓名输入框
    y += 35;
    teacherInput = new Fl_Input(x + 100, y, 150, 25, "教师:");

    // === 操作按钮区域 ===
    y += 45;
    // 添加课程按钮
    Fl_Button *addBtn = new Fl_Button(x, y, 70, 25, "添加");
    addBtn->callback(onAddCourse, this); // 设置点击回调函数

    // 删除课程按钮
    Fl_Button *deleteBtn = new Fl_Button(x + 80, y, 70, 25, "删除");
    deleteBtn->callback(onDeleteCourse, this);

    // === 课程列表表格 ===
    y += 35;
    courseTable = new CourseTable(x, y + 35, w() - 50, h() - y - 80);
    courseTable->selection_color(FL_YELLOW);
    courseTable->when(FL_WHEN_RELEASE | FL_WHEN_CHANGED);
    courseTable->col_header(1);      // 显示列表头
    courseTable->row_header(0);      // 不显示行表头
    courseTable->cols(4);            // 设置4列：课程名称、时间段、教师、学生人数
    courseTable->col_width_all(120); // 设置列宽
    courseTable->row_height_all(25); // 设置行高

    // 设置表格为可调整大小的组件
    courseTab->resizable(courseTable);

    // 添加表格列标题
    std::vector<std::string> headers = {
        "课程名称",
        "上课时间",
        "任课教师",
        "学生人数"};
    courseTable->addRow(headers);
}

/**
 * @brief 添加课程按钮的回调函数
 * @param w 触发回调的控件指针（未使用）
 * @param v MainWindow对象指针
 * @details 验证并收集输入数据，添加到课程表格中
 */
void MainWindow::onAddCourse(Fl_Widget *, void *v)
{
    MainWindow *win = (MainWindow *)v;

    // 获取输入数据
    const char *name = win->courseNameInput->value();
    const char *teacher = win->teacherInput->value();
    int startHour = win->startTimeHour->value();
    int startMin = win->startTimeMin->value();
    int endHour = win->endTimeHour->value();
    int endMin = win->endTimeMin->value();
    int studentCount = win->studentCountInput->value();
    std::string day = win->dayChoice->text();

    // 创建时间段对象
    TimeSlot timeSlot{day, startHour, startMin, endHour, endMin};

    // 创建课程对象并添加到管理器
    Course newCourse(name, timeSlot, studentCount, teacher);
    if (win->courseManager->addCourse(newCourse))
    {
        // 更新表格显示
        win->updateCourseTable();

        win->updateCourseChoice();

        win->saveData();
    }
}

/**
 * @brief 设置教室管理标签���
 * @details 创建并布局教室信息输入区域和教室列表表格
 */
void MainWindow::setupClassroomTab()
{
    int x = 20, y = 45;

    // 教室信息输入区域
    Fl_Input *roomNumInput = new Fl_Input(x + 100, y, 150, 25, "教室编号:");

    y += 35;
    Fl_Spinner *capacityInput = new Fl_Spinner(x + 100, y, 150, 25, "容纳人数:");
    capacityInput->range(1, 200);

    y += 35;
    Fl_Spinner *floorInput = new Fl_Spinner(x + 100, y, 150, 25, "楼层:");
    floorInput->range(1, 20);

    // 操作按钮
    y += 45;
    Fl_Button *addRoomBtn = new Fl_Button(x, y, 70, 25, "添加");
    addRoomBtn->callback(onAddClassroom, this);

    Fl_Button *deleteRoomBtn = new Fl_Button(x + 80, y, 70, 25, "删除");
    deleteRoomBtn->callback(onDeleteClassroom, this);

    // 教室列表表格
    classroomTable = new CourseTable(x, y + 35, w() - 50, h() - 200);
    classroomTable->col_header(1);
    classroomTable->row_header(1);
    classroomTable->cols(3); // 教室编号、容量、类型
    classroomTable->col_width_all(120);
    classroomTable->row_height_all(25);
}

/**
 * @brief 设置排课管理标签页
 * @details 创建并布局排课信息选择区域和课表显示
 */
void MainWindow::setupScheduleTab()
{
    int x = 20, y = 45;

    // 下拉选择区域
    courseChoice = new Fl_Choice(x + 100, y, 150, 25, "选择课程:");
    updateCourseChoice();

    y += 35;
    roomChoice = new Fl_Choice(x + 100, y, 150, 25, "选择教室:");
    updateRoomChoice();

    // 下拉选择的按钮组
    Fl_Button *suggestBtn = new Fl_Button(x + 260, y - 35, 70, 25, "建议");
    suggestBtn->callback(onSuggestFromChoice, this);

    Fl_Button *arrangeBtn = new Fl_Button(x + 260, y, 70, 25, "排课");
    arrangeBtn->callback(onArrangeCourse, this);

    // 手动输入区域
    y += 45;
    manualCourseInput = new Fl_Input(x + 100, y, 150, 25, "课程名称:");
    Fl_Button *suggestBtn1 = new Fl_Button(x + 260, y, 70, 25, "建议");
    suggestBtn1->callback(onSuggestFromInput, this);

    y += 35;
    manualClassroomInput = new Fl_Input(x + 100, y, 150, 25, "教室编号:");

    // 手动排课按钮
    Fl_Button *manualBtn = new Fl_Button(x + 260, y, 70, 25, "排课");
    manualBtn->callback(onManualArrange, this);

    // 添加删除排课按钮
    Fl_Button *deleteScheduleBtn = new Fl_Button(x + 340, y, 70, 25, "删除");
    deleteScheduleBtn->callback(onDeleteSchedule, this);

    // 课表显示
    y += 45;
    scheduleTable = new CourseTable(x, y, w() - 50, h() - y - 30);
    scheduleTable->cols(6);
    scheduleTable->col_width_all(120);
    scheduleTable->row_height_all(25);
    updateScheduleDisplay();
}

/**
 * @brief 添加教室的回调函数
 * @param w 触发回调的控件指针（未使用）
 * @param v MainWindow对象指针
 */
void MainWindow::onAddClassroom(Fl_Widget *, void *v)
{
    MainWindow *win = (MainWindow *)v;
    Fl_Input *roomNum = (Fl_Input *)win->classroomTab->child(0);
    Fl_Spinner *capacity = (Fl_Spinner *)win->classroomTab->child(1);
    Fl_Spinner *floor = (Fl_Spinner *)win->classroomTab->child(2);

    // 使用 ClassroomManager 添加教室
    Classroom newRoom(
        roomNum->value(),
        (int)capacity->value(),
        (int)floor->value());

    if (win->classroomManager->addClassroom(newRoom))
    {
        // 更新表格显示
        win->updateClassroomTable();
        win->updateRoomChoice();

        win->saveData();
    }
}

void MainWindow::onArrangeCourse(Fl_Widget *, void *v)
{
    MainWindow *win = (MainWindow *)v;

    if (win->courseChoice->value() < 0 || win->roomChoice->value() < 0)
    {
        fl_alert("请选择课程和教室！");
        return;
    }

    // 获取选中的课程和教室
    const char *courseName = win->courseChoice->text();
    const char *roomName = win->roomChoice->text();

    // 从管理器中获取课程和教室对象
    Course *course = win->courseManager->findCourse(courseName);
    Classroom *classroom = win->classroomManager->findClassroom(roomName);

    if (course && classroom)
    {
        // 使用 Schedule 类进行排课
        if (win->schedule->addEntry(*course, *classroom))
        {
            win->updateScheduleDisplay();
            fl_message("课程安排成功！");

            win->saveData();
        }
    }
}

void MainWindow::updateCourseChoice()
{
    courseChoice->clear();
    courseChoice->add("请选择课程"); // 添加默认选项
    courseChoice->value(0);          // 设置默认选中项

    // 获取所有课程并添加到选择框
    for (const auto &course : courseManager->getCourses())
    {
        courseChoice->add(course.name.c_str());
    }
}

void MainWindow::updateRoomChoice()
{
    roomChoice->clear();
    roomChoice->add("请选择教室"); // 添加默认选项
    roomChoice->value(0);          // 设置默认选中项

    // 获取所有教室并添加到选择框
    for (const auto &room : classroomManager->getClassrooms())
    {
        roomChoice->add(room.name.c_str());
    }
}

void MainWindow::updateScheduleDisplay()
{
    // 清空现有课表
    scheduleTable->clearData();

    // 添加表头
    std::vector<std::string> headers = {
        "课程名称",
        "上课时间",
        "学生人数",
        "任课教师",
        "教室"};
    scheduleTable->addRow(headers);

    // 显示所有已安排的课程
    for (const auto &entry : schedule->getEntries())
    {
        std::vector<std::string> rowData;
        rowData.push_back(entry.course.name);
        rowData.push_back(entry.course.time.toString());
        rowData.push_back(std::to_string(entry.course.studentCount));
        rowData.push_back(entry.course.teacher);
        rowData.push_back(entry.classroom.name);
        scheduleTable->addRow(rowData);
    }
}

void MainWindow::onDeleteCourse(Fl_Widget *, void *v)
{
    MainWindow *win = (MainWindow *)v;

    // 立即将第一个输入保存到 string 中
    std::string courseName = fl_input("请输入要删除的课程名称：");
    if (courseName == "")
        return;

    std::string teacher = fl_input("请输入该课程的授课教师：");
    if (teacher == "")
        return;

    // 查找匹配的课程
    bool found = false;
    for (const auto &course : win->courseManager->getCourses())
    {
        std::cout << "课程名：[" << course.name
                  << "]，教师：[" << course.teacher << "]" << std::endl;

        if (courseName == course.name && teacher == course.teacher)
        {
            found = true;
            if (fl_choice("确定要删除课程 '%s'（授课教师：%s)吗?", "取消", "确定", 0,
                          courseName.c_str(), teacher.c_str()) == 1)
            {
                win->schedule->removeEntriesForCourse(courseName, teacher);
                if (win->courseManager->removeCourse(courseName, teacher))
                {
                    win->updateCourseTable();
                    win->updateCourseChoice();
                    win->updateScheduleDisplay();
                    win->saveData();
                    fl_message("课程及其相关排课已删除！");
                }
            }
            break;
        }
    }

    if (!found)
    {
        fl_alert("未找到该教师教授的该课程！");
    }
}

void MainWindow::onDeleteClassroom(Fl_Widget *, void *v)
{
    MainWindow *win = (MainWindow *)v;

    const char *roomName = fl_input("请输入要删除的教室编号：");
    if (!roomName)
        return; // 用户取消输入

    if (fl_choice("确定要删除教室 '%s' 吗？", "取消", "确定", 0, roomName) == 1)
    {
        if (win->classroomManager->removeClassroom(roomName))
        {
            win->updateClassroomTable();
            win->updateRoomChoice();
            win->updateScheduleDisplay();
            win->saveData();
            fl_message("教室已删除！");
        }
        else
        {
            fl_alert("未找到该教室！");
        }
    }
}

void MainWindow::loadData()
{
    try
    {
        // 从文件加载课程数据
        courseManager->loadFromFile("../data/courses.txt");
        classroomManager->loadFromFile("../data/classrooms.txt");
        schedule->loadFromFile("../data/schedule.txt", *courseManager, *classroomManager);

        // 更新界面显示
        updateCourseTable();
        updateClassroomTable();
        updateScheduleDisplay();
    }
    catch (const std::runtime_error &e)
    {
        fl_alert("%s", e.what());
    }
}

void MainWindow::saveData()
{
    try
    {
        // 保存数据到文件
        courseManager->saveToFile("../data/courses.txt");
        classroomManager->saveToFile("../data/classrooms.txt");
        schedule->saveToFile("../data/schedule.txt");
    }
    catch (const std::runtime_error &e)
    {
        fl_alert("%s", e.what());
    }
}

void MainWindow::updateCourseTable()
{
    courseTable->clearData();

    // 添加表头
    std::vector<std::string> headers = {
        "课程名称", "上课时间", "任课教师", "学生人数"};
    courseTable->addRow(headers);

    // 从 CourseManager 获取所有课程并显示
    for (const auto &course : courseManager->getCourses())
    {
        std::vector<std::string> rowData;
        rowData.push_back(course.name);
        rowData.push_back(course.time.toString());
        rowData.push_back(course.teacher);
        rowData.push_back(std::to_string(course.studentCount));
        courseTable->addRow(rowData);
    }
}

void MainWindow::updateClassroomTable()
{
    classroomTable->clearData();

    // 添加表头
    std::vector<std::string> headers = {
        "教室编号", "容纳人数", "楼层"};
    classroomTable->addRow(headers);

    // 从 ClassroomManager 获取所有教室并显示
    for (const auto &room : classroomManager->getClassrooms())
    {
        std::vector<std::string> rowData;
        rowData.push_back(room.name);
        rowData.push_back(std::to_string(room.capacity));
        rowData.push_back(std::to_string(room.floor));
        classroomTable->addRow(rowData);
    }
}

// 下拉框的建议回调
void MainWindow::onSuggestFromChoice(Fl_Widget *, void *v)
{
    MainWindow *win = (MainWindow *)v;

    if (win->courseChoice->value() < 0)
    {
        fl_alert("请先选择课程！");
        return;
    }

    Course *course = win->courseManager->findCourse(win->courseChoice->text());
    if (!course)
    {
        fl_alert("未找到选中的课程！");
        return;
    }

    auto suggestedRooms = win->schedule->suggestClassrooms(*course, *win->classroomManager);
    showSuggestions(suggestedRooms);
}

// 手动输入的建议回调
void MainWindow::onSuggestFromInput(Fl_Widget *, void *v)
{
    MainWindow *win = (MainWindow *)v;

    const char *courseName = win->manualCourseInput->value();
    if (strlen(courseName) == 0)
    {
        fl_alert("请输入课程名称！");
        return;
    }

    Course *course = win->courseManager->findCourse(courseName);
    if (!course)
    {
        fl_alert("未找到该课程！");
        return;
    }

    auto suggestedRooms = win->schedule->suggestClassrooms(*course, *win->classroomManager);
    showSuggestions(suggestedRooms);
}

// 显示建议结果的辅助函数
void MainWindow::showSuggestions(const std::vector<const Classroom *> &rooms)
{
    if (!rooms.empty())
    {
        std::string message = "建议教室列表：\n\n";
        for (size_t i = 0; i < rooms.size(); i++)
        {
            message += std::to_string(i + 1) + ". " + rooms[i]->name +
                       "\n   容量：" + std::to_string(rooms[i]->capacity) + "人" +
                       "\n   楼层：" + std::to_string(rooms[i]->floor) + "\n\n";
        }
        fl_message("%s", message.c_str());
    }
    else
    {
        fl_alert("没有找到合适的教室！");
    }
}

void MainWindow::onManualArrange(Fl_Widget *, void *v)
{
    MainWindow *win = (MainWindow *)v;
    Course *course = win->courseManager->findCourse(win->manualCourseInput->value());
    Classroom *classroom = win->classroomManager->findClassroom(win->manualClassroomInput->value());

    if (course && classroom && win->schedule->addEntry(*course, *classroom))
    {
        win->updateScheduleDisplay();
        win->saveData();
    }
}

// 在析构函数中保存数据
MainWindow::~MainWindow()
{
    saveData();
    delete courseManager;
    delete classroomManager;
    delete schedule;
}

// 在 MainWindow 类中添加新的静态回调函数
void MainWindow::onDeleteSchedule(Fl_Widget *, void *v)
{
    MainWindow *win = (MainWindow *)v;

    // 立即将第一个输入保存到 string 中
    const char *input1 = fl_input("请输入要删除排课的课程名称：");
    if (!input1)
        return;
    std::string courseName(input1);

    const char *input2 = fl_input("请输入该课程的授课教师：");
    if (!input2)
        return;
    std::string teacher(input2);

    // 打印调试信息
    std::cout << "\n正在查找排课记录..." << std::endl;
    std::cout << "输入的课程名：[" << courseName << "]" << std::endl;
    std::cout << "输入的教师名：[" << teacher << "]" << std::endl;

    // 查找并删除排课记录
    bool found = false;
    for (const auto &entry : win->schedule->getEntries())
    {
        if (entry.course.name == courseName && entry.course.teacher == teacher)
        {
            found = true;
            if (fl_choice("确定要删除课程 '%s'（授课教师：%s)的排课记录吗?",
                          "取消", "确定", 0,
                          courseName.c_str(), teacher.c_str()) == 1)
            {
                win->schedule->removeEntriesForCourse(courseName, teacher);
                win->updateScheduleDisplay();
                win->saveData();
                fl_message("排课记录已删除！");
            }
            break;
        }
    }

    if (!found)
    {
        fl_alert("未找到该课程的排课记录！");
    }
}