#include <Arduino.h>
#include <Wire.h>
#include <input_manager.h>
#include <display_manager.h>
#include <time_config.h>
#include <RTClib.h>
#include <alarm.h>

UIState currentUIState = STATE_DEFAULT;

RTC_DS3231 rtc;

tm editBuffer;
tm systemTime;


#define I2C_SDA_PIN 8
#define I2C_SCL_PIN 9

void setup() {
    Serial.begin(115200);
  

   // delay(200);

    initInputManager(); // initiate each input

    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

    ledcSetup(0, 2000, 8);

    ledcAttachPin(buzzer, 0); // 1000Hz base pitch, 8-bit depth
    
    // Set duty cycle to 0% so it stays completely silent on startup
    ledcWrite(0, 0); 


    rtcFound();


    delay(1000);

    displayFound(); // check to make sure the display exists


    //attachInterrupt(digitalPinToInterrupt(ENCODER_CLK), encoderDirection, CHANGE); //these are meant to interrupt whatever function is running if a change is detected. 
    //attachInterrupt(digitalPinToInterrupt(ENCODER_DT), encoderDirection, CHANGE);  dont think its needed but keeping for now
    

    displayClear(); // clear the display

    clockUpdate(); //MUST ALWAYS RUN 
    updateDisplay();  

    //activateFlicker(10000);

    //writeUserTime();

    //delay(200);


}   

void loop() {

    automaticDimming(); // function that handles automatic dimming; always checking for change in input

    encoderDirection(); // if menu selection is taking place, the argument will be true and the encoder will act differently. no conditional yet so just false for now

    encoderSwitch();

    dstConfigure();

    formatTime();

    soundAlarm();

    //updateBuzzerState();

    playAlarmSound();

    snoozeAndSilence();

    setAlarm();

    //flickerDisplay();


   if (inputPressed(BTN_RST)) { //implement debounce for inputs where needed
        displayString("Restarting", 1, 0, 0, true);
        ESP.restart();
    } else if (inputPressed(BTN_SNOOZE_SILENCE)) {
        displayString("Snooze/Silence Button", 1, 0, 0, true);

        //rtc1.adjust(writeUserTime());
    
    } else {
        //do nothing for now
    }
    clockUpdate(); //MUST ALWAYS RUN 
    updateDisplay();  

}