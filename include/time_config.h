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

extern RTC_DS3231 rtc;

char dayOfTheWeek(const std::tm& displayTime);
void writeUserTime(tm userTime);
void displayTime();
void dstConfigure(bool dstEnabled);
void formatTime(); //switches between 12 and 24 hours, will take time as an argument
void rtcFound();
void clockUpdate();

#endif // TIME_CONFIG_H