#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Arduino.h>

#define I2C_SDA 8
#define I2C_SCL 9
#define SERIAL = 0x3C

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64

bool displayFound();
void displayString(String message, int textSize, int cursorX, int cursorY, bool clearDisplay); 
void displayClear();
void displayDim(bool dim);
//add blinking function for alarm
//add function to display a selectable list with arguments that include # of items and the list of items 

#endif // DISPLAY_MANAGER_H