#include <iostream>
#include "Admin.h"
#include "Course.h"
#include "Classroom.h"
#include "Schedule.h"
#include <filesystem>
using namespace std;

int main()
{
    try {
        CourseManager courseManager;
        ClassroomManager classroomManager;
        Schedule schedule;

        // 正确工作目录
        std::cout << "当前工作目录：" << std::filesystem::current_path() << std::endl;
        
        // 文件路径
        const string DATA_DIR = "../data/";
        string coursesPath = DATA_DIR + "courses.txt";
        string classroomsPath = DATA_DIR + "classrooms.txt";
        string schedulePath = DATA_DIR + "schedule.txt";
        
        // 路径测试
        std::cout << "尝试打开文件：" << std::endl;
        std::cout << "课程文件：" << coursesPath << std::endl;
        std::cout << "教室文件：" << classroomsPath << std::endl;
        std::cout << "排课文件：" << schedulePath << std::endl;

        // 检查文件是否存在
        if (!std::filesystem::exists(coursesPath)) {
            std::cout << "课程文件不存在！" << std::endl;
        }
        if (!std::filesystem::exists(classroomsPath)) {
            std::cout << "教室文件不存在！" << std::endl;
        }
        if (!std::filesystem::exists(schedulePath)) {
            std::cout << "排课文件不存在！" << std::endl;
        }

        // 从文件加载数据
        courseManager.loadFromFile(coursesPath);
        classroomManager.loadFromFile(classroomsPath);
        schedule.loadFromFile(schedulePath, courseManager, classroomManager);

        // 显示加载的数据
        std::cout << "\n当前课程列表:\n";
        courseManager.listCourses();

        std::cout << "\n当前教室列表:\n";
        classroomManager.listClassrooms();

        std::cout << "\n当前排课方案:\n";
        schedule.display();

        // 添加新的数据
        TimeSlot newTime{"周五", 13, 0, 15, 0};
        courseManager.addCourse(Course("生物", newTime, 25, "赵老师"));
        classroomManager.addClassroom(Classroom("E102", 35, 2));
        schedule.addEntry(Course("生物", newTime, 25, "赵老师"), Classroom("E102", 35, 2));

        // 存储数据到文件
        courseManager.saveToFile("courses.txt");
        classroomManager.saveToFile("classrooms.txt");
        schedule.saveToFile("schedule.txt");

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    }
}
