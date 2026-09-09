#include <Arduino.h>
#include <Wire.h>
#include <input_manager.h>
#include <display_manager.h>

const int buzzer = 10;

void setup() {
    Serial.begin(115200);
    
    delay(200);
    initInputManager();
    Wire.begin(I2C_SDA, I2C_SCL);

    pinMode(buzzer, OUTPUT);


    delay(1000);

    displayFound();



    

    displayClear();

    delay(200);

        displayString("test ", 2, 0, 0, false);

}   

void loop() {
   if (inputPressed(BTN_RST)) {
        displayString("Restarting", 1, 0, 0, true);
        ESP.restart();
    } else if (inputPressed(BTN_FORMAT)) {
        displayString("Format Button", 1, 0, 0, true);
        tone(buzzer, 500, 500); // Play a tone at 1000 Hz for 500 ms
    } else if (inputPressed(BTN_DST)) {
        displayString("DST Button", 1, 0, 0, true);
    } else if (inputPressed(BTN_SNOOZE_SILENCE)) {
        displayString("Snooze/Silence Button", 1, 0, 0, true);
    } else if (inputPressed(BTN_SET_ALARM)) {
        displayString("Set Alarm Button", 1, 0, 0, true);
    } else {
        Serial.println("No button pressed");
        noTone(buzzer); 
        //displayClear();
    }
    int value = analogRead(3);

    if (value < 1000) { 
        displayDim(true);
   } else if (value >= 1000) {
        displayDim(false);

    }
    


}