#ifndef TIME_H
#define TIME_H
#include <Arduino.h>

struct UserDate {
    uint8_t day;
    uint8_t month;
    uint16_t year;
};

struct UserTime {
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
};

char dayOfTheWheek(UserDate.day, uint8_t month, uint16_t year);

#endif // TIME_H