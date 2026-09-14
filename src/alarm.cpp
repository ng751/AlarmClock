/* This script is designed to handle user configurations and data for the alarm clock, as well as snoozing, silencing, and buzzing.
 *
 */

 #include <Arduino.h>
 #include <display_manager.h>
 #include <input_manager.h>
 #include <alarm.h>
 #include <time_config.h>

 int activeSlotIndex = 0;
 bool alarmDateBool = false;

 Alarm editAlarmBuffer;

 const int buzzer = 10;

const char* alarmTones[] = { "tone 1", "tone 2", "tone 3" };
int selectedToneIndex = 0; 
    //this can include all 3 different buzzer sounds as well as a switch statement possibly

 void snooze() {
    //this will be responsible for snoozing the buzzer and behaving according to the user set configurations
 }

void soundAlarm() {
    static unsigned long lastCheckTime = 0;
    if (millis() - lastCheckTime < 1000) return; 
    lastCheckTime = millis();

    for (int i = 0; i < 3; i++) {
    
        if (alarmSlots[i].isEnabled) {  
    
            if (displayTime.tm_hour == alarmSlots[i].alarmTime.tm_hour &&
                displayTime.tm_min  == alarmSlots[i].alarmTime.tm_min && displayTime.tm_sec == alarmSlots[i].alarmTime.tm_sec) {
                
                  Serial.println("hello alarm");
                    playAlarmSound(alarmSlots[i].chosenAlarm); 
                    break; // Exit loop early once an alarm is triggered
                }
            }
        }
}


 void silence() {
    //this will silence the buzzer altogether
 }

 void playAlarmSound(int toneIndex) {
    switch (toneIndex) {
        case 0:
            tone(buzzer, 1000);
            delay(1000);
            noTone(buzzer);
            break;
            
        case 1:
            tone(buzzer, 500);
            delay(1000);
            noTone(buzzer);
            break;
            
        case 2:
            tone(buzzer, 300);
            delay(1000);
            noTone(buzzer);
      
            break;
            
        default:
            break;
    }
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
          isEditingAlarm = true; 
           Serial.print("in statescroll menu");
          activeSlotIndex = currentMenuScrollPosition;
          editBuffer = alarmSlots[activeSlotIndex].alarmTime; 
          drawAlarmMenu;
         }
        updateDisplayFlag = true;
      }
    }
  }  
    
  lastButtonState = currentButtonState;
}

 void changeDateTime() {

  

    Serial.println("in changedatetime");

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
     currentUIState = STATE_DEFAULT; // automatically return to the main screen
     break;
    }
     updateDisplayFlag = true;
    // alert the system that the UI state changed and the display needs to reflect the new field
  }

void alarmData() {

    Serial.println("in alarmData");


  switch (currentUIState) {
    case STATE_SCROLL_MENU:
        Serial.println("in statescreen menu in alarm data"); 
        currentUIState = STATE_EDIT_HOUR;
        break;
        
    case STATE_EDIT_HOUR:
        currentUIState = STATE_EDIT_MINUTES;
        break;
        
    case STATE_EDIT_MINUTES:
        currentUIState = STATE_EDIT_SECONDS;
        break;
    case STATE_EDIT_SECONDS:
    Serial.println("edit seconds");
        currentUIState = STATE_EDIT_ALARMDATEBOOL;
        break;
        
    case STATE_EDIT_ALARMDATEBOOL:
    Serial.println("alarm date bool");
        if (!alarmDateBool) {
            currentUIState = STATE_EDIT_SELECTALARM; 
        } else {
            currentUIState = STATE_EDIT_YEAR;
        }
        break;
        
    case STATE_EDIT_YEAR:
        currentUIState = STATE_EDIT_MONTH;
        break;
        
    case STATE_EDIT_MONTH:
        currentUIState = STATE_EDIT_DAY;
        break;
        
    case STATE_EDIT_DAY:
        currentUIState = STATE_EDIT_SELECTALARM;
        break;
    case STATE_EDIT_SELECTALARM:
        currentUIState = STATE_EDIT_SNOOZEDELAY;
        break;
        
    case STATE_EDIT_SNOOZEDELAY:
        currentUIState = STATE_EDIT_SNOOZESBEFORESILENCE;
        break;
        
    case STATE_EDIT_SNOOZESBEFORESILENCE:
        currentUIState = STATE_EDIT_TIMEUNTILSILENCE; 
        break;
        
    case STATE_EDIT_TIMEUNTILSILENCE:
    //create new alarm
        alarmSlots[activeSlotIndex] = editAlarmBuffer;
        alarmSlots[activeSlotIndex].alarmTime = editBuffer;
    
    // alarm enabled 
    alarmSlots[activeSlotIndex].isEnabled = true;
    Serial.println(alarmSlots[activeSlotIndex].alarmTime.tm_hour);

    // clear working data for alarm structs
    editAlarmBuffer = Alarm(); 

    Serial.print("Alarm configuration saved cleanly to slot: ");
    Serial.println(activeSlotIndex);

    // no longer editing and return to display
    isEditingAlarm = false;
    currentUIState = STATE_DEFAULT;
        break;

    }
     updateDisplayFlag = true;
  }
