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
extern bool dstEnabled;
extern bool standardFormat;

char dayOfTheWeek(const std::tm& displayTime);
void writeUserTime(tm userTime);
void displayTime();
void dstConfigure();
void formatTime(); 
void rtcFound();
void clockUpdate();

#endif // TIME_CONFIG_H