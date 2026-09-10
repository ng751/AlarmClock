/* This script is designed to handle the OLED display of the alarm clock, including...
 * This script receives information and updates the OLED display accordingly.
 */

#include <display_manager.h>
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

bool blinkOn;

#define OLED_VCC_PIN 18

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

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

void displayString(String message, int textSize, int cursorX, int cursorY, bool clearDisplay) { //functio to print a string to the oled with some parameters
    if (clearDisplay) {
        display.clearDisplay();
    }
    display.setTextSize(textSize);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(cursorX, cursorY);
    display.println(message);
    display.display();
}


bool dimmed = false;
bool screenOff = false;
int dimState = 0; // Starts at 0 (Fully Illuminated)
int oldState;

void displayDim(bool dimFurther, bool isManual) { //prototype for the dimming feature. this will require much discussion
    // Global variable to keep track of the current step
   oldState = dimState;

   if (isManual) {
    // --- ENCODER CONTROL (Manual Override) ---
    // Allows cycling through all 3 states: 0 <-> 1 <-> 2
    if (dimFurther) {
      dimState++; 
      if (dimState > 2) dimState = 2; // Clamp at Off
    } else {
      dimState--;
      if (dimState < 0) dimState = 0; // Clamp at Max Brightness
    }
  } 
  else {
    // --- PHOTORESISTOR CONTROL (Automatic) ---
    // Only allowed to touch states if the user hasn't forced the screen OFF (State 2)
    if (dimState != 2) { 
      if (dimFurther) {
        dimState = 1; // Light sensor says it's dark -> Go to Dimmed
      } else {
        dimState = 0; // Light sensor says it's bright -> Go to Max Brightness
      }
    }
  }

  // 2. Only talk to the screen hardware if the state actually changed
  if (dimState != oldState) {
    switch (dimState) {
      
      case 0: // DIMMED -> FULLY ILLUMINATED
        display.ssd1306_command(SSD1306_DISPLAYON);     // Ensure screen is on
        display.ssd1306_command(SSD1306_SETCONTRAST); 
        display.ssd1306_command(255);                    // Max brightness
        Serial.println("State 0: Fully Illuminated");
        break;

      case 1: // Moving from either Off or Fully Illuminated into DIMMED
        display.ssd1306_command(SSD1306_DISPLAYON);     // Wake up if coming from Off
        display.ssd1306_command(SSD1306_SETCONTRAST); 
        display.ssd1306_command(1);                      // Low brightness
        Serial.println("State 1: Dimmed");
        break;

      case 2: // DIMMED -> OFF
        display.ssd1306_command(SSD1306_DISPLAYOFF);    // Put display to sleep
        Serial.println("State 2: Off");
        break;
    }
    // Note: No display.display() here! The hardware handles these commands instantly.
  }
    
}

void flashDisplay(bool editMode, int componentToFlicker) {
    //this function will flash the display for when the alarm goes off as well data that is being edited
    if (!editMode) {
        // simply flicker entire display
    } else {
        //
    }
}
