#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "TimeSlot.h"

using namespace std;

class Course
{
public:
    std::string name;
    TimeSlot time;
    int studentCount;
    std::string teacher;

    Course(const std::string &n, const TimeSlot &t, int sc, const std::string &tea)
        : name(n), time(t), studentCount(sc), teacher(tea) {}
};

class CourseManager
{
private:
    std::vector<Course> courses;

public:
    // 检查课程是否重复
    bool isCourseConflict(const Course &newCourse) const
    {
        return any_of(courses.begin(), courses.end(),
                           [&newCourse](const Course &course)
                           {
                               // 检查课程名、时间和教师是否同时重复
                               bool timeConflict = course.time.overlaps(newCourse.time);
                               bool sameTeacher = course.teacher == newCourse.teacher;
                               bool sameName = course.name == newCourse.name;

                               // 如果同一时间段的课程由同一个教师教授，认为是冲突
                               if (timeConflict && sameTeacher)
                               {
                                   return true;
                               }
                               return false;
                           });
    }

    // 修改添加课程的方法
    bool addCourse(const Course &course)
    {
        if (isCourseConflict(course))
        {
            std::cout << "课程 \"" << course.name << "\" 存在冲突，无法添加。\n";
            std::cout << "该教师在此时间段已有其他课程。\n";
            return false;
        }
        courses.push_back(course);
        std::cout << "课程添加成功。\n";
        return true;
    }

    void listCourses() const
    {
        for (const auto &course : courses)
        {
            std::cout << "课程名称: " << course.name
                      << ", 时间: " << course.time.toString()
                      << ", 选课人数: " << course.studentCount
                      << ", 教师: " << course.teacher << "\n";
        }
    }

    // 存储到文件
    void saveToFile(const std::string &filename) const
    {
        std::ofstream file(filename);
        for (const auto &course : courses)
        {
            file << course.name << " " << course.time.toString() << " "
                 << course.studentCount << " " << course.teacher << "\n";
        }
    }

    // 从文件载入
    void loadFromFile(const std::string &filename)
    {
        courses.clear();
        std::ifstream file(filename);
        if (!file.is_open())
        {
            std::cerr << "无法打开文件: " << filename << std::endl;
            throw std::runtime_error("文件打开失败: " + filename);
        }

        std::string line;
        int lineNum = 0;
        while (std::getline(file, line))
        {
            lineNum++;
            try
            {
                std::istringstream iss(line);
                std::string name, dayStr, timeStr, teacher;
                int studentCount;

                // 分步读取并打印调试信息
                if (iss >> name >> dayStr >> timeStr >> studentCount >> teacher)
                {
                    // 解析时间字符串 (格式应该是 "09:00-11:00")
                    size_t dashPos = timeStr.find('-');
                    if (dashPos == std::string::npos)
                    {
                        throw std::runtime_error("时间格式错误，缺少'-'符号");
                    }

                    std::string startTime = timeStr.substr(0, dashPos);
                    std::string endTime = timeStr.substr(dashPos + 1);

                    // 解析开始时间
                    size_t startColonPos = startTime.find(':');
                    if (startColonPos == std::string::npos)
                    {
                        throw std::runtime_error("开始时间格式错误，缺少':'符号");
                    }

                    int startHour = std::stoi(startTime.substr(0, startColonPos));
                    int startMinute = std::stoi(startTime.substr(startColonPos + 1));

                    // 解析结束时间
                    size_t endColonPos = endTime.find(':');
                    if (endColonPos == std::string::npos)
                    {
                        throw std::runtime_error("结束时间格式错误，缺少':'符号");
                    }

                    int endHour = std::stoi(endTime.substr(0, endColonPos));
                    int endMinute = std::stoi(endTime.substr(endColonPos + 1));

                    // 创建 TimeSlot 对象
                    TimeSlot timeSlot{dayStr, startHour, startMinute, endHour, endMinute};

                    // 创建新课程并检查是否可以添加
                    Course newCourse(name, timeSlot, studentCount, teacher);
                    if (!isCourseConflict(newCourse))
                    {
                        courses.push_back(newCourse);
                        std::cout << "成功加载课程: " << name << std::endl;
                    }
                    else
                    {
                        std::cout << "跳过重复课程: " << name << std::endl;
                    }
                }
            }
            catch (const std::exception &e)
            {
                std::cerr << "第 " << lineNum << " 行解析错误: " << e.what() << std::endl;
                std::cerr << "原始行内容: " << line << std::endl;
            }
        }
    }

    const std::vector<Course> &getCourses() const
    {
        return courses;
    }
};