#ifndef TIME_CONFIG_H
#define TIME_CONFIG_H

#include <ctime>
#include <RTClib.h>

struct Alarm {
    std::tm alarmTime{};
    bool isEnabled{false};
    bool isDaily{false};
    //include tone, visual aspect
};

char dayOfTheWeek(const std::tm& displayTime);
DateTime writeUserTime(int year, int month, int day, int hour, int minute, int second);
void displayTime();
void dstConfigure(bool dstEnabled);
void formatTime(); //switches between 12 and 24 hours, will take time as an argument

#endif // TIME_CONFIG_H