#ifndef TIME_CONFIG_H
#define TIME_CONFIG_H

#include <ctime>

struct Alarm {
    std::tm alarmTime{};
    bool isEnabled{false};
    bool isDaily{false};
    //include tone, visual aspect
};

char dayOfTheWeek(const std::tm& displayTime);
void dstConfigure(bool dstEnabled);
//add 12/24 hour formatting

#endif // TIME_CONFIG_H