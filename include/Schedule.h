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
private:
    std::vector<ScheduleEntry> entries;

public:
    // 添加教室分配
    bool addEntry(const Course &course, const Classroom &classroom)
    {
        // 检查该课程是否已经被安排
        for (const auto &entry : entries)
        {
            if (entry.course.name == course.name)
            {
                std::cout << "课程 \"" << course.name
                          << "\" 已经被安排到教室 \"" << entry.classroom.name
                          << "\"，无法重复安排。\n";
                return false;
            }
        }

        // 检查该教室在这个时间段是否已被占用
        for (const auto &entry : entries)
        {
            if (entry.classroom.name == classroom.name &&
                entry.course.time.overlaps(course.time))
            {
                std::cout << "无法安排课程 \"" << course.name
                          << "\" 到教室 \"" << classroom.name
                          << "\"：该教室在此时间段已有其他课程。\n";
                return false;
            }
        }

        entries.push_back(ScheduleEntry{course, classroom});
        std::cout << "成功安排课程 \"" << course.name
                  << "\" 到教室 \"" << classroom.name << "\"。\n";
        return true;
    }

    // 为一个课程分配最合适的教室
    bool assignClassroom(const Course &course, ClassroomManager &classroomManager)
    {
        const Classroom *bestRoom = nullptr;
        int minCapacityDiff = std::numeric_limits<int>::max();

        for (const auto &room : classroomManager.getClassrooms())
        {
            // 检查教室容量和时间冲突
            if (room.capacity >= course.studentCount)
            {
                bool hasConflict = false;
                for (const auto &entry : entries)
                {
                    if (entry.classroom.name == room.name &&
                        entry.course.time.overlaps(course.time))
                    {
                        hasConflict = true;
                        break;
                    }
                }

                if (!hasConflict)
                {
                    int capacityDiff = room.capacity - course.studentCount;
                    if (capacityDiff < minCapacityDiff)
                    {
                        bestRoom = &room;
                        minCapacityDiff = capacityDiff;
                    }
                }
            }
        }

        if (bestRoom)
        {
            entries.push_back(ScheduleEntry{course, *bestRoom});
            std::cout << "课程 \"" << course.name
                      << "\" 已分配到教室 \"" << bestRoom->name << "\"。\n";
            return true;
        }

        std::cout << "无法为课程 \"" << course.name
                  << "\" 分配教室：所有合适的教室都已被占用或容量不足。\n";
        return false;
    }

    // 方法：显示排课结果
    void display() const
    {
        for (const auto &entry : entries)
        {
            std::cout << "课程: " << entry.course.name
                      << " (" << entry.course.time.toString()
                      << ", " << entry.course.studentCount << "人"
                      << ", 教师: " << entry.course.teacher
                      << ") -> 教室: " << entry.classroom.name
                      << " (容量: " << entry.classroom.capacity
                      << ", 楼层: " << entry.classroom.floor << ")\n";
        }
    }

    // 从文件加载排课方案
    void loadFromFile(const std::string &filename,
                      const CourseManager &courseManager,
                      const ClassroomManager &classroomManager)
    {
        entries.clear();
        std::ifstream file(filename);
        if (!file.is_open())
        {
            throw std::runtime_error("无法打开文件: " + filename);
        }

        std::string line;
        while (std::getline(file, line))
        {
            std::istringstream iss(line);
            std::string courseName, dayStr, timeStr, teacher, classroomName;
            int studentCount;

            if (iss >> courseName >> dayStr >> timeStr >> studentCount >> teacher >> classroomName)
            {
                const Course *course = nullptr;
                const Classroom *classroom = nullptr;

                // 查找课程
                for (const auto &c : courseManager.getCourses())
                {
                    if (c.name == courseName)
                    {
                        course = &c;
                        break;
                    }
                }

                // 查找教室
                classroom = classroomManager.findClassroom(classroomName);

                if (course && classroom)
                {
                    addEntry(*course, *classroom);
                }
            }
        }
    }

    // 保存排课方案到文件
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

    // 获取所有课程安排
    const std::vector<ScheduleEntry> &getEntries() const
    {
        return entries;
    }

    // 修改为返回多个建议教室
    std::vector<const Classroom *> suggestClassrooms(const Course &course, const ClassroomManager &classroomManager) const
    {
        std::vector<const Classroom *> suggestions;

        // 存储所有符合条件的教室及其容量差值
        std::vector<std::pair<const Classroom *, int>> validRooms;

        for (const auto &room : classroomManager.getClassrooms())
        {
            // 检查教室容量和时间冲突
            if (room.capacity >= course.studentCount)
            {
                bool hasConflict = false;
                for (const auto &entry : entries)
                {
                    if (entry.classroom.name == room.name &&
                        entry.course.time.overlaps(course.time))
                    {
                        hasConflict = true;
                        break;
                    }
                }

                if (!hasConflict)
                {
                    int capacityDiff = room.capacity - course.studentCount;
                    validRooms.push_back({&room, capacityDiff});
                }
            }
        }

        // 按容量差值排序
        std::sort(validRooms.begin(), validRooms.end(),
                  [](const auto &a, const auto &b)
                  {
                      return a.second < b.second;
                  });

        // 取前5个最合适的教室
        for (const auto &room : validRooms)
        {
            suggestions.push_back(room.first);
            if (suggestions.size() >= 5)
                break;
        }

        return suggestions;
    }

    // 删除指定课程的所有排课记录
    void removeEntriesForCourse(const std::string &courseName, const std::string &teacher)
    {
        entries.erase(
            std::remove_if(entries.begin(), entries.end(),
                           [&](const ScheduleEntry &entry)
                           {
                               return entry.course.name == courseName &&
                                      entry.course.teacher == teacher;
                           }),
            entries.end());
    }
};
