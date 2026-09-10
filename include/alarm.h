#ifndef ALARM_H
#define ALARM_H

#include <Arduino.h>

extern tm systemTime;
extern tm editBuffer;

void buzzerSound();
void buzzerOptions();
void snooze();
void snoozeConfigurations();
void silence();
void setAlarm();
void changeDateTime();

#endif // ALARM_H