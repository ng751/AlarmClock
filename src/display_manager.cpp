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

#define OLED_VCC_PIN 18

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

bool updateDisplayFlag = false;


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

  char timeBuffer[32]; // stores characters

  switch (currentUIState) {
    case STATE_DEFAULT:
      display.clearDisplay();
      snprintf(timeBuffer, sizeof(timeBuffer), "%02d:%02d:%02d", systemTime.tm_hour, systemTime.tm_min, systemTime.tm_sec); 
      displayString(timeBuffer, 2, 16, 10, false); 
      snprintf(timeBuffer, sizeof(timeBuffer), "%04d/%02d/%02d", systemTime.tm_year + 1900, systemTime.tm_mon + 1, systemTime.tm_mday); 
      displayString(timeBuffer, 1, 32, 40, false);

      display.display();
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

void flashDisplay() {
   //this function will flash the screen when an alarm is going off
}
