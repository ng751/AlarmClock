#include <Arduino.h>
#include <Wire.h>
#include <input_manager.h>
#include <display_manager.h>

const int buzzer = 10;

void setup() {
    Serial.begin(115200);
    
    delay(200);
    initInputManager();
    encoderInit();
    Wire.begin(I2C_SDA, I2C_SCL);

    pinMode(buzzer, OUTPUT);


    delay(1000);

    displayFound();


    //attachInterrupt(digitalPinToInterrupt(ENCODER_CLK), encoderDirection, CHANGE); //these are meant to interrupt whatever function is running if a change is detected. 
    //attachInterrupt(digitalPinToInterrupt(ENCODER_DT), encoderDirection, CHANGE);  dont think its needed but keeping for now
    

    displayClear();

    delay(200);


}   

void loop() {

    encoderDirection(false); //if menu selection is taking place, this will be true and the encoder will act differently. no conditional yet so just false for now
    automaticDimming(); 

   if (inputPressed(BTN_RST)) { //implement debounce for inputs
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
    } else if (inputPressed(ENCODER_SWITCH)) {
      displayString("Select", 1, 0, 0, true);
    } else {
        //do nothing for now
    }

    


}