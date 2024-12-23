#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

class Classroom
{
public:
    std::string name;
    int capacity;
    int floor;

    // 存储教室的已安排课程时间段
    std::vector<std::string> scheduledTimes;

    Classroom(const std::string &n, int cap, int fl)
        : name(n), capacity(cap), floor(fl) {}

    // 检查教室在特定时间是否可用
    bool isAvailable(const std::string &time) const
    {
        for (const auto &scheduledTime : scheduledTimes)
        {
            if (scheduledTime == time)
            {
                return false;
            }
        }
        return true;
    }

    // 安排课程时间
    void schedule(const std::string &time)
    {
        scheduledTimes.push_back(time);
    }
};

class ClassroomManager
{
private:
    std::vector<Classroom> classrooms;

public:
    void addClassroom(const Classroom &classroom)
    {
        classrooms.push_back(classroom);
        std::cout << "教室 \"" << classroom.name << "\" 添加成功。\n";
    }

    void listClassrooms() const
    {
        for (const auto &room : classrooms)
        {
            std::cout << "教室名称: " << room.name
                      << ", 容纳人数: " << room.capacity
                      << ", 楼层: " << room.floor << "\n";
        }
    }

    // 存储到文件
    void saveToFile(const std::string& filename) const {
        std::ofstream file(filename);
        for (const auto& room : classrooms) {
            file << room.name << " " << room.capacity << " " << room.floor << "\n";
        }
    }

    // 从文件载入
    void loadFromFile(const std::string& filename) {
        classrooms.clear();
        std::ifstream file(filename);
        if (!file.is_open())
        {
            std::cerr << "无法打开文件: " << filename << std::endl;
            throw std::runtime_error("文件打开失败: " + filename);
        }
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string name;
            int capacity, floor;
            if (iss >> name >> capacity >> floor) {
                classrooms.emplace_back(name, capacity, floor);
            }
        }
    }

    std::vector<Classroom> &getClassrooms()
    {
        return classrooms;
    }

    const std::vector<Classroom> &getClassrooms() const
    {
        return classrooms;
    }
};
