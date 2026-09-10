/* This script is designed to handle the time and date data that is manually entered by the user, 
 * including hours, minutes, and AM or PM, day, month, year, and day of the week.
 * This program also includes functions such as 12-24 hour formatting, DST configuration, and day of the week calculation
 */

#include <RTClib.h>
#include <time_config.h>
#include <display_manager.h>


char dayOfTheWeek(const std::tm& displayTime) {
    // Calculate the day of the week using Zeller's Congruence
    int q = displayTime.tm_mday;
    int m = displayTime.tm_mon + 1; // tm_mon is 0-based, so add 1
    int K = displayTime.tm_year % 100; // Year of the century
    int J = displayTime.tm_year / 100; // Zero-based century

    if (m < 3) {
        m += 12;
        K--;
    }

    int h = (q + (13 * (m + 1)) / 5 + K + (K / 4) + (J / 4) - (2 * J)) % 7;

    // Zeller's Congruence returns: 0=Saturday, 1=Sunday, ..., 6=Friday
    // Adjust to return: 0=Sunday, 1=Monday, ..., 6=Saturday
     switch ((h + 5) % 7) {
        case 0: return 'S'; // Saturday
        case 1: return 'U'; // Sunday
        case 2: return 'M'; // Monday
        case 3: return 'T'; // Tuesday
        case 4: return 'W'; // Wednesday
        case 5: return 'R'; // Thursday
        case 6: return 'F'; // Friday
        default: return '?'; // Invalid value
    }
}

void writeUserTime(tm userTime) {
    //this function formats user entered data in a DateTime structure
    rtc.adjust(DateTime(
        userTime.tm_year + 1900,
        userTime.tm_mon + 1,
        userTime.tm_mday,
        userTime.tm_hour,
        userTime.tm_min,
        userTime.tm_sec
    ));
}

void rtcFound() {
    if (! rtc.begin()) {
        Serial.println("RTC not found");
        Serial.flush();
        for (;;); // Infinitely loop if rtc is not found
    }
}

void formatTime() { //switches between 12 and 24 hours, will take time as argument

}

void displayTime() {
   // displayString(UserTime(), 2, 0, 0 , true)
   //this will display the user entered time
}

void dstConfigure(bool dstEnabled) { // adds or removes an hour to or from the display time
    if (dstEnabled) {
        
    } else {
        
    }
}

