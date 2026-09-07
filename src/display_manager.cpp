/* This script is designed to handle the OLED display of the alarm clock, including...
 * This script receives information and updates the OLED display accordingly.
 */

#include <display_manager.h>
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

bool displayFound() {
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;); // Infinitely loop if display is not found
    }
    display.clearDisplay();
    display.display();
    return true;
}

void displayClear() {
    display.clearDisplay();
    display.display();
}

void displayString(String message, int textSize, int cursorX, int cursorY, bool clearDisplay) {
    if (clearDisplay) {
        display.clearDisplay();
    }
    display.setTextSize(textSize);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(cursorX, cursorY);
    display.println(message);
    display.display();
}
