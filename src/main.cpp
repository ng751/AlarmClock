#include <Arduino.h>
#include <Wire.h>
#include <input_manager.h>
#include <display_manager.h>

//const int buzzer = 10;

void setup() {
    Serial.begin(115200);
    
    delay(200);

    initInputManager(); // initiate each input
    encoderInit(); // read the current rotary encoder value to set up the encoderDirection function

    Wire.begin(I2C_SDA, I2C_SCL);

    //pinMode(buzzer, OUTPUT);

    delay(1000);

    displayFound(); // check to make sure the display exists


    //attachInterrupt(digitalPinToInterrupt(ENCODER_CLK), encoderDirection, CHANGE); //these are meant to interrupt whatever function is running if a change is detected. 
    //attachInterrupt(digitalPinToInterrupt(ENCODER_DT), encoderDirection, CHANGE);  dont think its needed but keeping for now
    

    displayClear(); // clear the display

    delay(200);


}   

void loop() {

    encoderDirection(false); // if menu selection is taking place, the argument will be true and the encoder will act differently. no conditional yet so just false for now

    automaticDimming(); // function that handles automatic dimming; always checking for change in input

   if (inputPressed(BTN_RST)) { //implement debounce for inputs where needed
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