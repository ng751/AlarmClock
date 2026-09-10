#include <Arduino.h>
#include <Wire.h>
#include <input_manager.h>
#include <display_manager.h>
#include <time_config.h>
#include <RTClib.h>
#include <alarm.h>

UIState currentUIState;
//const int buzzer = 10;

RTC_DS3231 rtc;

tm editBuffer;
tm systemTime;

#define I2C_SDA_PIN 8 // Replace with the physical GPIO pin number wired to SDA
#define I2C_SCL_PIN 9

void setup() {
    Serial.begin(115200);
    
    currentUIState = STATE_DEFAULT;

   // delay(200);

    initInputManager(); // initiate each input

    encoderInit(); // read the current rotary encoder value to set up the encoderDirection function

    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
    Wire.setClock(400000);

    if (!rtc.begin()) {
        Serial.println("Hardware connection to RTC failed!");
    while(1); 
  }

    //pinMode(buzzer, OUTPUT);

    delay(1000);

    displayFound(); // check to make sure the display exists


    //attachInterrupt(digitalPinToInterrupt(ENCODER_CLK), encoderDirection, CHANGE); //these are meant to interrupt whatever function is running if a change is detected. 
    //attachInterrupt(digitalPinToInterrupt(ENCODER_DT), encoderDirection, CHANGE);  dont think its needed but keeping for now
    

    displayClear(); // clear the display

    displayString("hello world", 2, 0, 0, true);

    //writeUserTime();

    //delay(200);


}   

void loop() {

    automaticDimming(); // function that handles automatic dimming; always checking for change in input

    encoderDirection(); // if menu selection is taking place, the argument will be true and the encoder will act differently. no conditional yet so just false for now


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
        //rtc1.adjust(writeUserTime());
    } else if (inputPressed(ENCODER_SWITCH)) { // when select is pressed and default display is showing, it prompts for time and date change
      // 1. HARDWARE DEBOUNCE: Wait 50ms for physical contact vibrations to stop
  
  // 2. STATE LOCK: Freeze the code while the user's finger is STILL holding the button down.
  // This prevents the code from advancing past the first case until they let go.
  // Replace 'digitalRead' with your own input function if you use an array matrix.
 
  
  // 3. DEBOUNCE RELEASE: Wait another 50ms to ensure the release contact noise is cleared

  // 4. THE ROUTER: Now it is 100% safe to advance the state machine exactly ONE step
  if (currentUIState == STATE_DEFAULT) {
    displayString("Select", 1, 0, 0, true);
    editBuffer = systemTime;           // Copy hard time to soft buffer
    currentUIState = STATE_EDIT_YEAR;  // Move to Year
  } 
  else if (currentUIState != STATE_DEFAULT && currentUIState != STATE_SCROLL_MENU) {
    // This will now execute perfectly exactly once per physical click!
    displayString("Select 5", 1, 0, 0, true);
    changeDateTime(); 
  }
  

    } else {
        //do nothing for now
    }


    //char buffer[] = "YYYY/MM/DD at hh:mm:ss";
      
     // Serial.println("\n--- Time Received! ---");
     // Serial.print("You entered: ");
    //  Serial.println(writeUserTime().toString(buffer));
    //  displayString(writeUserTime().toString(buffer), 2, 0, 0, true);

}