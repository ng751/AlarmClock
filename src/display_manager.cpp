/* This script is designed to handle the OLED display of the alarm clock, including...
 * This script receives information and updates the OLED display accordingly.
 */

#include <display_manager.h>
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <time_config.h>
#include <alarm.h>
#include <elapsedMillis.h>
#include <data_handling.h>

#define OLED_VCC_PIN 18

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


tm displayTime = systemTime;
bool updateDisplayFlag = false;
elapsedMillis sinceLastFlicker;
elapsedMillis flickerDuration;
bool displayIsFlickering;
bool displayOn;
int maxFlickerTime;

char alarmMenu[3][16] = {
  "Alarm 1",
  "Alarm 2",
  "Alarm 3"
};

int alarmCount = 3;
int currentMenuScrollPosition = 0;

bool displayFound() { //this function checks to make sure the display exists
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;); // Infinitely loop if display is not found
    }
    display.clearDisplay();
    display.display();
    return true;
}

void displayClear() { //this function clears the display
    display.clearDisplay();
    display.display();
}

void displayString(String message, int textSize, int cursorX, int cursorY, bool clearDisplay) { //function to print a string to the oled with some parameters
    if (clearDisplay) {
        display.clearDisplay();
    }
    display.setTextSize(textSize);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(cursorX, cursorY);
    display.println(message);
}


bool dimmed = false;
bool screenOff = false;
int dimState = 0; // 0 is fully illuminated
int oldState;

void updateDisplay() {
  if (!updateDisplayFlag) return; // return if updateflag was not updated to true

  Serial.println(systemTime.tm_hour);

  char timeBuffer[32]; // stores characters

  displayTime = systemTime; 

  if (dstEnabled) {
    displayTime.tm_hour += 1;
    mktime(&displayTime); 
  }

  switch (currentUIState) {
    case STATE_SCROLL_MENU:
        drawAlarmMenu();
        break;

    
    
    case STATE_DEFAULT:
      if (isSnoozing) {
            displayClear();
            
            displayString("Snoozing... zZz", 1, 20, 10, false);

        
            unsigned long currentMillis = millis();
            unsigned long remainingSecs = (snoozeEndTime > currentMillis) ? (snoozeEndTime - currentMillis) / 1000 : 0;
            snprintf(timeBuffer, sizeof(timeBuffer), "%02lu:%02lu", remainingSecs / 60, remainingSecs % 60);
            displayString(timeBuffer, 2, 35, 28, false);
            
            displayString("Tap again to Snooze", 1, 10, 52, false);
            break;
        }

        if (isBuzzerActive) {
            flickerDisplay(); 
        }
      display.clearDisplay();

      if (standardFormat) { //this is a little sloppy but i dont care to clean it up at the moment
        int hour12 = displayTime.tm_hour % 12;
        if (hour12 == 0) hour12 = 12;
          const char* period = (displayTime.tm_hour >= 12) ? "PM" : "AM";
          snprintf(timeBuffer, sizeof(timeBuffer), "%02d:%02d:%02d %s %s", hour12, displayTime.tm_min, displayTime.tm_sec, period, dayOfTheWeek(displayTime));
        } else {
          snprintf(timeBuffer, sizeof(timeBuffer), "%02d:%02d:%02d %s", displayTime.tm_hour, displayTime.tm_min, displayTime.tm_sec, dayOfTheWeek(displayTime));
        }

      displayString(timeBuffer, 2, 16, 10, false);
      snprintf(timeBuffer, sizeof(timeBuffer), "%04d/%02d/%02d", displayTime.tm_year + 1900, displayTime.tm_mon + 1, displayTime.tm_mday);
      displayString(timeBuffer, 1, 32, 45, false);
      break;
    case STATE_EDIT_YEAR:
      displayString("SET YEAR:", 1, 0, 0, true); 
      snprintf(timeBuffer, sizeof(timeBuffer), "%04d", editBuffer.tm_year + 1900); 
      displayString(timeBuffer, 3, 0, 16, false); 
      display.display();
      break;

    case STATE_EDIT_MONTH:
      displayString("SET MONTH:", 1, 0, 0, true); 
      snprintf(timeBuffer, sizeof(timeBuffer), "%02d", editBuffer.tm_mon + 1); 
      displayString(timeBuffer, 3, 0, 16, false); 
      display.display();
      break;

    case STATE_EDIT_DAY:
      displayString("SET DAY:", 1, 0, 0, true); 
      snprintf(timeBuffer, sizeof(timeBuffer), "%02d", editBuffer.tm_mday); 
      displayString(timeBuffer, 3, 0, 16, false); 
      display.display();
      break;

    case STATE_EDIT_HOUR:
      displayString("SET HOUR:", 1, 0, 0, true); 
      snprintf(timeBuffer, sizeof(timeBuffer), "%02d", editBuffer.tm_hour); 
      displayString(timeBuffer, 3, 0, 16, false);
      display.display();
      break;

    case STATE_EDIT_MINUTES:
      displayString("SET MINUTE:", 1, 0, 0, true); 
      snprintf(timeBuffer, sizeof(timeBuffer), "%02d", editBuffer.tm_min); 
      displayString(timeBuffer, 3, 0, 16, false); 
      display.display();
      break;
    case STATE_EDIT_SECONDS:
      displayString("SET SECOND:", 1, 0, 0, true); 
      snprintf(timeBuffer, sizeof(timeBuffer), "%02d", editBuffer.tm_sec); 
      displayString(timeBuffer, 3, 0, 16, false); 
      display.display();
      break;
    case STATE_EDIT_SELECTALARM:
        displayString("SELECT ALARM:", 1, 0, 0, true);
        snprintf(timeBuffer, sizeof(timeBuffer), "%02d", editAlarmBuffer.chosenAlarm + 1);
        displayString(timeBuffer, 3, 0, 16, false);
        display.display();
        break;
   case STATE_EDIT_SNOOZEDELAY:
        displayString("TIME BTWN SNOOZES:", 1, 0, 0, true);
        snprintf(timeBuffer, sizeof(timeBuffer), "%02d MINS", editAlarmBuffer.snoozeDelay);
        displayString(timeBuffer, 3, 0, 16, false);
        display.display();
        break;

    case STATE_EDIT_SNOOZESBEFORESILENCE:
        displayString("SNOOZES TIL SILENCE:",1, 0, 0, true);
        
        if (editAlarmBuffer.snoozesBeforeSilence == 0) {
            snprintf(timeBuffer, sizeof(timeBuffer), "INDEFINITE");
        } else {
            snprintf(timeBuffer, sizeof(timeBuffer), "%02d TIMES", editAlarmBuffer.snoozesBeforeSilence);
        }
        
        displayString(timeBuffer, 2, 0, 16, false);
        display.display();
        break;

    case STATE_EDIT_TIMEUNTILSILENCE:
        displayString("DURATION TIL SILENCE:", 1, 0, 0, true);

         if (editAlarmBuffer.timeUntilSilence == 0) {
            snprintf(timeBuffer, sizeof(timeBuffer), "INDEFINITE");
        } else {
            snprintf(timeBuffer, sizeof(timeBuffer), "%02d MINS", editAlarmBuffer.timeUntilSilence);
        }
        displayString(timeBuffer, 2, 0, 16, false);
        display.display();
        break;

    case STATE_EDIT_ALARMDATEBOOL:
      displayString("ADD A DATE?:", 1, 0, 0, true);
    if (alarmDateBool) {
        displayString("> YES", 2, 0,  20, false);
        displayString("  NO",  2, 70, 20, false); 
    } else {
        displayString("  YES", 2, 0,  20, false);
        displayString("> NO",  2, 70, 20, false); 
    }
    display.display();
    break;
  }

  if (currentUIState != STATE_DEFAULT && currentUIState != STATE_SCROLL_MENU) {
    displayString("[Select to Confirm]", 1, 0, 55, false);
  }
  display.display();
  updateDisplayFlag = false; 
}

void displayDim(bool dimFurther, bool isManual) { //prototype for the dimming featur
   oldState = dimState;

   if (isManual) {
    if (dimFurther) {
      dimState++; 
      if (dimState > 2) dimState = 2; // off stays off
    } else {
      dimState--;
      if (dimState < 0) dimState = 0; // max stays max
    }
  } 
  else {
    if (dimState != 2) { 
      if (dimFurther) {
        dimState = 1; // if photoresistor is dark then dim
      } else {
        dimState = 0; // if photoresistor says its light then illuminate
      }
    }
  }

  // only change if dimstate and oldstate are different, suggests user has turned the knob
  if (dimState != oldState) {
    switch (dimState) {
      
      case 0: // DIMMED to FULLY ILLUMINATED
        display.ssd1306_command(SSD1306_DISPLAYON);     // ensure screen is on
        display.ssd1306_command(SSD1306_SETCONTRAST); 
        display.ssd1306_command(255);                    // max brightness
        Serial.println("State 0: Fully Illuminated");
        break;

      case 1: // Moving from either off or fully illuminated into dim
        display.ssd1306_command(SSD1306_DISPLAYON);     // turn display back on if off
        display.ssd1306_command(SSD1306_SETCONTRAST); 
        display.ssd1306_command(1);                      // dimmed
        Serial.println("State 1: Dimmed");
        break;

      case 2: // DIMMED to OFF
        display.ssd1306_command(SSD1306_DISPLAYOFF);    // sleep mode
        Serial.println("State 2: Off");
        break;
    }
  }
    
}

void activateFlicker(int flickerTime){
  maxFlickerTime = flickerTime;
  flickerDuration = 0;
  sinceLastFlicker = 0;
  displayIsFlickering = true;
}

void flickerDisplay() {
   if (!isBuzzerActive) {
        if (!displayOn) {
            display.ssd1306_command(SSD1306_DISPLAYON);
            displayOn = true;
        }
        return; 
    }

    if (sinceLastFlicker >= 50) {
        sinceLastFlicker = 0; 

        if (displayOn) {
            display.ssd1306_command(SSD1306_DISPLAYOFF);
            displayOn = false;
        } else {
            display.ssd1306_command(SSD1306_DISPLAYON);
            displayOn = true;
        }
    }

}

  void drawAlarmMenu() {

    displayString("--- SELECT ALARM ---", 1, 0, 0, true);

    for (int i = 0; i < 3; i++) {
        char slotText[24];
        int yPosition = 16 + (i * 12); // stack rows neatly down the screen (Y: 16, 28, 40)

        // draw a selection arrow '>' only next to the active scroll position
        if (i == currentMenuScrollPosition) {
            displayString(">", 1, 0, yPosition, false);
        } else {
            displayString(" ", 1, 0, yPosition, false);
        }

        // format the text depending on whether the alarm is set or empty
        if (alarmSlots[i].isEnabled && !alarmSlots[i].isDaily) {
           // if active, print the saved time configuration 
            snprintf(slotText, sizeof(slotText), "Alm %d: %02d:%02d", 
                     i + 1, 
                     alarmSlots[i].alarmTime.tm_hour, 
                     alarmSlots[i].alarmTime.tm_min);
        } else  if (alarmSlots[i].isEnabled && alarmSlots[i].isDaily) {
                    snprintf(slotText, sizeof(slotText), "Al %d: %02d:%02d %02d/%02d/%02d",
                    i + 1,
                    alarmSlots[i].alarmTime.tm_hour,
                    alarmSlots[i].alarmTime.tm_min,
                    alarmSlots[i].alarmTime.tm_mon + 1,  
                    alarmSlots[i].alarmTime.tm_mday,     
                    alarmSlots[i].alarmTime.tm_year % 100); 
        } else {
            // if unconfigured or overwritten, show an empty placeholder slot
            snprintf(slotText, sizeof(slotText), "Alm %d: --:-- [EMPTY]", i + 1);
        }

        // output the slot details string to the screen buffer
        displayString(slotText, 1, 8, yPosition, false);
    }

}

void snoozeDisplay() {
  // display.invertDisplay(false); 

  //       displayString("Snoozing... zZz", 1, 20, 10, false);

  //       // Compute the minutes and seconds remaining on your snoozeEndTime timeline
  //       unsigned long currentMillis = millis();
  //       unsigned long remainingSecs = 0;
  //       if (snoozeEndTime > currentMillis) {
  //           remainingSecs = (snoozeEndTime - currentMillis) / 1000;
  //       }
  //       unsigned long displayMins = remainingSecs / 60;
  //       unsigned long displaySecs = remainingSecs % 60;

  //       // Render a countdown time look using your exact string framework
  //       snprintf(alertBuffer, sizeof(alertBuffer), "%02lu:%02lu", displayMins, displaySecs);
  //       displayString(alertBuffer, 2, 35, 28, false);

  //       displayString("Tap again to Snooze", 1, 10, 52, false);
}