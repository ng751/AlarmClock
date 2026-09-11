/* This script is designed to handle user configurations and data for the alarm clock, as well as snoozing, silencing, and buzzing.
 *
 */

 #include <Arduino.h>
 #include <display_manager.h>
 #include <input_manager.h>
 #include <alarm.h>
 #include <time_config.h>

 



 void buzzerSound() {
    //this will be responsible for activating the alarm at the correct time
 }

 void buzzerOptions() {
    //this can include all 3 different buzzer sounds as well as a switch statement possibly
 }

 void snooze() {
    //this will be responsible for snoozing the buzzer and behaving according to the user set configurations
 }

 void snoozeConfigurations() {
    //the snooze settings that are outlined in the functional requirements
 }
 void silence() {
    //this will silence the buzzer altogether
 }

 
void setAlarm() {
  bool currentButtonState = (digitalRead(INPUTS[BTN_SET_ALARM].pin) == LOW);
  static bool lastButtonState = false;
  static unsigned long lastDebounceTime = 0;
  const unsigned long debounceDelay = 50;
  if (currentButtonState != lastButtonState) {
    if (millis() - lastDebounceTime > debounceDelay) {
      lastDebounceTime = millis(); // Reset debounce timer

      if (currentButtonState == true) {
      
         if (currentUIState == STATE_SCROLL_MENU) {
          currentUIState = STATE_DEFAULT;
         } else {
          currentUIState = STATE_SCROLL_MENU;
          drawAlarmMenu();
         }
        updateDisplayFlag = true;
      }
    }
  }  
    
  lastButtonState = currentButtonState;
}

 void changeDateTime() {

  

    Serial.println("test");

    switch (currentUIState) {

    case STATE_EDIT_YEAR:
      
      currentUIState = STATE_EDIT_MONTH;
      break;
        
    case STATE_EDIT_MONTH:
   
      currentUIState = STATE_EDIT_DAY;
      break;
        
    case STATE_EDIT_DAY:
     
      currentUIState = STATE_EDIT_HOUR;
      break;

    case STATE_EDIT_HOUR:
     
      currentUIState = STATE_EDIT_MINUTES;
      break;

    case STATE_EDIT_MINUTES:
    
     
      currentUIState = STATE_EDIT_SECONDS;
      break;
    case STATE_EDIT_SECONDS:

      
       systemTime = editBuffer;

      mktime(&systemTime);

      writeUserTime(systemTime);

     // Serial.println("Time and date saved:");
     // Serial.print("Year: ");  Serial.println(systemTime.tm_year + 1900);
     // Serial.print("Month: "); Serial.println(systemTime.tm_mon + 1);
    //  Serial.print("Day: ");   Serial.println(systemTime.tm_mday);
      //Serial.print("Hour: ");  Serial.println(systemTime.tm_hour);
     // Serial.print("Min: ");   Serial.println(systemTime.tm_min);
      currentUIState = STATE_DEFAULT; // Automatically pop back out to the main screen
     break;
    }
     updateDisplayFlag = true;
    // alert the system that the UI state changed and the display needs to reflect the new field
  }
