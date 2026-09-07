#include <Arduino.h>
#include <Wire.h>
#include <input_manager.h>
#include <display_manager.h>

void setup() {
    Serial.begin(115200);
    
    delay(200);
    initInputManager();
    Wire.begin(I2C_SDA, I2C_SCL);

    delay(1000);

    displayFound();

    displayClear();
}   

void loop() {
   if (inputPressed(BTN_RST)) {
        displayString("Restarting", 1, 0, 0, true);
        ESP.restart();
    } else if (inputPressed(BTN_FORMAT)) {
        displayString("Format Button", 1, 0, 0, true);
    } else if (inputPressed(BTN_DST)) {
        displayString("DST Button", 1, 0, 0, true);
    } else if (inputPressed(BTN_SNOOZE_SILENCE)) {
        displayString("Snooze/Silence Button", 1, 0, 0, true);
    } else if (inputPressed(BTN_SET_ALARM)) {
        displayString("Set Alarm Button", 1, 0, 0, true);
    } else {
        Serial.println("No button pressed");
        displayClear();
    }
    delay(50);
}