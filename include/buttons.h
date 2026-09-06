#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>

constexpr uint8_t BTN_RESTART = 10; // Restarts the microcontroller
constexpr uint8_t BTN_FORMAT_TIME = 15; // Formats the time to 12-hour or 24-hour format
constexpr uint8_t BTN_DST = 7; // Toggles Daylight Saving Time by adding or subtracting one hour from the current time
constexpr uint8_t BTN_SNOOZE_SILENCE = 6; /* If the alarm is sounding, one click snoozes, double click disables it. If the alarm
                                           * is not sounding, one click allows user to configure snooze settings. 
                                           */
constexpr uint8_t BTN_DATE_TIME = 5; // Allows the user to set date and time
constexpr uint8_t BTN_CONFIRM = 100; // Rotary encoder switch, used to confirm user selections

#endif // BUTTONS_H