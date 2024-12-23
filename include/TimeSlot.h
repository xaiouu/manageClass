#pragma once
#include <iostream>
#include <string>

using namespace std;

struct TimeSlot
{
    std::string day; // 星期几，例如 "周一"
    int startHour;   // 开始时间的小时部分
    int startMinute; // 开始时间的分钟部分
    int endHour;     // 结束时间的小时部分
    int endMinute;   // 结束时间的分钟部分

    // 将时间段转换为字符串
    std::string toString() const
    {
        return day + " " + std::to_string(startHour) + ":" +
               (startMinute < 10 ? "0" : "") + std::to_string(startMinute) + "-" +
               std::to_string(endHour) + ":" +
               (endMinute < 10 ? "0" : "") + std::to_string(endMinute);
    }

    // 检查时间段是否重叠
    bool overlaps(const TimeSlot &other) const
    {
        if (day != other.day)
            return false;

        int thisStart = startHour * 60 + startMinute;
        int thisEnd = endHour * 60 + endMinute;
        int otherStart = other.startHour * 60 + other.startMinute;
        int otherEnd = other.endHour * 60 + other.endMinute;

        return !(thisEnd <= otherStart || thisStart >= otherEnd);
    }
};