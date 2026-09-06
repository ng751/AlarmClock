#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Arduino.h>

#define I2C_SDA 8
#define I2C_SCL 9
#define SERIAL = 0x3C

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64

bool displayFound();
void displayData(String message, int textSize, int cursorX, int cursorY, bool clearDisplay); 
void displayClear();

#endif // DISPLAY_MANAGER_H