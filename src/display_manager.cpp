/* This script is designed to handle the OLED display of the alarm clock, including...
 * This script receives information and updates the OLED display accordingly.
 */

#include <display_manager.h>
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

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
void displayDim(bool dim) { //prototype for the dimming feature. this will require much discussion
    if (dim && !dimmed)  {
        display.ssd1306_command(SSD1306_SETCONTRAST); //these lines dim the OLED display
        display.ssd1306_command(1);

       // display.ssd1306_command(0xD9); //these four lines may make it slightly dimmer. i sincerely cannot tell
       // display.ssd1306_command(0x11); 
       // display.ssd1306_command(0xDB); 
       // display.ssd1306_command(0x00); 

        display.display();
        dimmed = true;
    } else if (!dim && dimmed) {
        display.ssd1306_command(SSD1306_SETCONTRAST); //these lines illuminate the oled display
        display.ssd1306_command(255);

        display.display();
        dimmed = false;
    }
}

void flashDisplay() {
    //this function will flash the display for when the alarm goes off
}
