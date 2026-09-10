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
   currentUIState = STATE_SCROLL_MENU;
   //this function runs when the set alarm button is pressed, each character i
 }

 void changeDateTime() {
    //while(!digitalRead(ENCODER_SWITCH)); // Pause until user lets go of the button


    switch (currentUIState) {
      
      case STATE_EDIT_YEAR:
        // Year is locked in! Automatically shift focus to Month
        Serial.println("here 2");
        currentUIState = STATE_EDIT_MONTH;
        break;
        
      case STATE_EDIT_MONTH:
      Serial.println("here 3");
    
        currentUIState = STATE_EDIT_DAY;
        break;
        
      case STATE_EDIT_DAY:
        
        currentUIState = STATE_EDIT_HOUR;
        break;
        
      case STATE_EDIT_HOUR:
    
        currentUIState = STATE_EDIT_MINUTES;
        break;
        
      case STATE_EDIT_MINUTES:
         Serial.println("here 6");
      
        currentUIState = STATE_EDIT_SECONDS;
        // rtc.adjust(DateTime(userYear, userMonth, userDay, userHour, userMin, 0));
      case STATE_EDIT_SECONDS:
        systemTime = editBuffer;

        mktime(&systemTime);

        writeUserTime(systemTime);

        Serial.println("time and date saved");
        Serial.println("Time and date saved:");
  Serial.print("Year: ");  Serial.println(systemTime.tm_year + 1900);
  Serial.print("Month: "); Serial.println(systemTime.tm_mon + 1);
  Serial.print("Day: ");   Serial.println(systemTime.tm_mday);
  Serial.print("Hour: ");  Serial.println(systemTime.tm_hour);
  Serial.print("Min: ");   Serial.println(systemTime.tm_min);
        currentUIState = STATE_DEFAULT; // Automatically pop back out to the main screen
        break;
    }
    
    // Alert the system that the UI state shifted and the display needs to reflect the new field
  }
