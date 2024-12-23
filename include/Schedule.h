#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <algorithm> // for std::find_if
#include "Course.h"
#include "Classroom.h"
#include "Course.h"
#include "Classroom.h"

using namespace std;

struct ScheduleEntry
{
    Course course;
    Classroom classroom;
};

class Schedule
{
public:
    std::vector<ScheduleEntry> entries;

    // 添加排课记录
    bool addEntry(const Course &course, const Classroom &classroom)
    {
        if (classroom.isAvailable(course.time.toString()))
        {
            std::cout << "无法安排课程 \"" << course.name
                      << "\" 到教室 \"" << classroom.name
                      << "\"：该教室在此时间段已有其他课程。\n";
            return false;
        }

        entries.push_back(ScheduleEntry{course, classroom});
        std::cout << "成功安排课程 \"" << course.name
                  << "\" 到教室 \"" << classroom.name << "\"。\n";
        return true;
    }

    // 方法：为一个课程分配最合适的教室
    bool assignClassroom(const Course &course, ClassroomManager &classroomManager)
    {
        Classroom *bestRoom = nullptr;
        int minCapacityDiff = std::numeric_limits<int>::max();

        for (auto &room : classroomManager.getClassrooms())
        {
            if (room.capacity >= course.studentCount && room.isAvailable(course.time.toString()))
            {
                int capacityDiff = room.capacity - course.studentCount;
                if (capacityDiff < minCapacityDiff ||
                    (capacityDiff == minCapacityDiff && room.floor < (bestRoom ? bestRoom->floor : std::numeric_limits<int>::max())))
                {
                    bestRoom = &room;
                    minCapacityDiff = capacityDiff;
                }
            }
        }

        if (bestRoom)
        {
            entries.push_back(ScheduleEntry{course, *bestRoom});
            bestRoom->schedule(course.time.toString());
            std::cout << "课程 \"" << course.name << "\" 已分配到教室 \"" << bestRoom->name << "\"。\n";
            return true;
        }
        else
        {
            std::cout << "无法为课程 \"" << course.name << "\" 分配教室。\n";
            return false;
        }
    }

    // 方法：显示排课结果
    void display() const
    {
        for (const auto &entry : entries)
        {
            std::cout << "课程: " << entry.course.name
                      << ", 时间: " << entry.course.time.toString()
                      << ", 教师: " << entry.course.teacher
                      << ", 教室: " << entry.classroom.name << "\n";
        }
    }

    // 存储到文件
    void saveToFile(const std::string &filename) const
    {
        std::ofstream file(filename);
        if (!file.is_open())
        {
            throw std::runtime_error("无法打开文件进行写入: " + filename);
        }

        for (const auto &entry : entries)
        {
            file << entry.course.name << " "
                 << entry.course.time.toString() << " "
                 << entry.course.studentCount << " "
                 << entry.course.teacher << " "
                 << entry.classroom.name << "\n";
        }
    }

    // 从文件载入
    void loadFromFile(const std::string &filename,
                      const CourseManager &courseManager,
                      const ClassroomManager &classroomManager)
    {
        entries.clear();
        std::ifstream file(filename);
        if (!file.is_open())
        {
            std::cerr << "无法打开文件: " << filename << std::endl;
            throw std::runtime_error("文件打开失败: " + filename);
        }
        std::string line;
        while (std::getline(file, line))
        {
            std::istringstream iss(line);
            std::string courseName, timeString, teacher, classroomName;
            int studentCount;

            if (iss >> courseName >> timeString >> studentCount >> teacher >> classroomName)
            {
                // 查找对应的教室
                const auto &classrooms = classroomManager.getClassrooms();
                auto classroomIt = std::find_if(classrooms.begin(), classrooms.end(),
                                                [&classroomName](const Classroom &c)
                                                {
                                                    return c.name == classroomName;
                                                });

                if (classroomIt != classrooms.end())
                {
                    // 直接使用现有的 Course 对象
                    const auto &courses = courseManager.getCourses();
                    auto courseIt = std::find_if(courses.begin(), courses.end(),
                                                 [&courseName](const Course &c)
                                                 {
                                                     return c.name == courseName;
                                                 });

                    if (courseIt != courses.end())
                    {
                        entries.push_back(ScheduleEntry{*courseIt, *classroomIt});
                    }
                }
            }
        }
    }
};
