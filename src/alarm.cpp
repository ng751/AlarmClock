/* This script is designed to handle user configurations and data for the alarm clock, as well as snoozing, silencing, and buzzing.
 *
 */

 #include <Arduino.h>
 #include <display_manager.h>
 #include <input_manager.h>

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
    while(!digitalRead(ENCODER_SWITCH)); // Pause until user lets go of the button

    // AUTOMATIC PROGRESSION VIEW
    switch (currentUIState) {
      
      case STATE_DEFAULT:
        // Clicking from the main screen jumps straight into the wizard
        currentUIState = STATE_EDIT_YEAR;
        break;
        
      case STATE_EDIT_YEAR:
        // Year is locked in! Automatically shift focus to Month
        currentUIState = STATE_EDIT_MONTH;
        break;
        
      case STATE_EDIT_MONTH:
        // Month is locked in! Automatically shift focus to Day
        currentUIState = STATE_EDIT_DAY;
        break;
        
      case STATE_EDIT_DAY:
        // Day is locked in! Automatically shift focus to Hours
        currentUIState = STATE_EDIT_HOUR;
        break;
        
      case STATE_EDIT_HOUR:
        // Hour is locked in! Automatically shift focus to Minutes
        currentUIState = STATE_EDIT_MINUTES;
        break;
        
      case STATE_EDIT_MINUTES:
        // Minutes are set! Everything is completely finished.
        currentUIState = STATE_EDIT_SECONDS;

        // --- OPTIONAL: Sync changes to an external RTC chip here if you have one ---
        // rtc.adjust(DateTime(userYear, userMonth, userDay, userHour, userMin, 0));
      case STATE_EDIT_SECONDS:
        currentUIState = STATE_EDIT_AMPM;
      case STATE_EDIT_AMPM:
         
        Serial.println("Time & Date configuration saved successfully!");
        currentUIState = STATE_DEFAULT; // Automatically pop back out to the main screen
        break;
    }
    
    // Alert the system that the UI state shifted and the display needs to reflect the new field
  }
