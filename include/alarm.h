#ifndef ALARM_H
#define ALARM_H

#include <Arduino.h>

extern tm systemTime;
extern tm editBuffer;

struct Alarm {
    tm alarmTime{};
    bool isEnabled{false};
    bool isDaily{false};
    char chosenAlarm{0};
    int snoozeDelay;
    int snoozesBeforeSilence;
    int timeUntilSilence;

    //include tone, visual aspect
};

extern int activeSlotIndex; 

extern int snoozeDelay;
extern int snoozesBeforeSilence;
extern int timeUntilSilence;
extern bool isEditingAlarm;

extern bool alarmDateBool; 
extern Alarm editAlarmBuffer;
extern Alarm alarmSlots[3]; 

extern const char* alarmTones[3];


void buzzerSound();
void snooze();
void silence();
void setAlarm();
void changeDateTime();
void playAlarmSound(int index);
void soundAlarm();
void alarmData();

#endif // ALARM_H