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
 bool isBuzzerActive = false;
 unsigned long buzzerTurnOffTime = 0;
 bool isIndefiniteAlarm = false;
 int activeToneIndex = 0;
 bool wasSnoozePressed = false;
bool wasStopPressed = false;

const int pwmChannel = 0;    
const int pwmResolution = 8;  



int activeMaxSnoozes = 0;      
int activeSnoozeMinutes = 0;   
int currentSnoozeCount = 0;  

const unsigned long DURATION_VALUES[] = {
    900000,   // 15 minutes
    1800000,  // 30 minutes
    3600000   // 60 minutes
};

int configSnoozeMinutes = 5; 

int configMaxSnoozes = 3; 


unsigned long snoozeEndTime = 0;
bool isSnoozing = false;


 Alarm editAlarmBuffer;


const char* alarmTones[] = { "tone 1", "tone 2", "tone 3" };
int selectedToneIndex = 0; 
    //this can include all 3 different buzzer sounds as well as a switch statement possibly

void snoozeAndSilence() {
    unsigned long currentMillis = millis();

    bool isButtonPressed = (digitalRead(INPUTS[BTN_SNOOZE_SILENCE].pin) == LOW);
    static bool lastButtonState = false;
    static unsigned long buttonPressedStartTime = 0;
    static bool holdActionExecuted = false;

    bool localSnoozeTriggered = false;
    bool localStopTriggered = false;

    if (isPreviewActive && (currentMillis >= previewTurnOffTime)) {
    isPreviewActive = false; // Turn off the preview sound wave generation!
    Serial.println("Menu preview duration expired. Restoring silence.");
}
    
    if (isButtonPressed && !lastButtonState) {
        buttonPressedStartTime = currentMillis;
        holdActionExecuted = false; 
    }

    
    if (isButtonPressed && lastButtonState) {
        if (!holdActionExecuted && (currentMillis - buttonPressedStartTime >= 2000)) {
            localStopTriggered = true; 
            holdActionExecuted = true; 
        }
    }

  
    if (!isButtonPressed && lastButtonState) {
        unsigned long holdDuration = currentMillis - buttonPressedStartTime;
        if (!holdActionExecuted && (holdDuration >= 50)) { // 50ms noise debounce floor
            localSnoozeTriggered = true;
        }
    }
    lastButtonState = isButtonPressed; 
  
    if ((isBuzzerActive || isSnoozing) && localStopTriggered) {
        isBuzzerActive = false;
        isIndefiniteAlarm = false;
        isSnoozing = false;
        currentSnoozeCount = 0; 
        Serial.println("Alarm manually stopped via 2-second hold.");
        return;
    }

  
    if (isBuzzerActive && localSnoozeTriggered) {
    
        if (activeMaxSnoozes != 0 && currentSnoozeCount >= activeMaxSnoozes) {
            isBuzzerActive = false;
            isIndefiniteAlarm = false;
            Serial.println("Max snoozes reached for this alarm. Auto-silenced on tap.");
            return;
        }

        currentSnoozeCount++; 

        isBuzzerActive = false;
        isIndefiniteAlarm = false; 
        
        
        snoozeEndTime = currentMillis + ((unsigned long)activeSnoozeMinutes * 60 * 1000); 
        isSnoozing = true;
        Serial.print("Alarm snoozed. Current snooze count: ");
       Serial.println(currentSnoozeCount);
        return;
    }

    if (isSnoozing && (currentMillis >= snoozeEndTime)) {
        isSnoozing = false;
        isBuzzerActive = true; 
        Serial.println("Snooze time expired! Alarm ringing again.");
        

        if (isIndefiniteAlarm) {
        
            buzzerTurnOffTime = 0;
        } else {
    
            buzzerTurnOffTime = currentMillis + DURATION_VALUES[activeSlotIndex];
        }
        return;
    }


    if (isIndefiniteAlarm) return; 

    if (isBuzzerActive && (currentMillis >= buzzerTurnOffTime)) {
        isBuzzerActive = false; // Automatically shut off because user-configured time ran out
        Serial.println("Alarm auto-silenced after continuous sound limit.");
    }
}

void soundAlarm() {
    if (isBuzzerActive || isSnoozing) return;

    static unsigned long lastCheckTime = 0;
    if (millis() - lastCheckTime < 1000) return;
    lastCheckTime = millis();

    if (currentUIState == STATE_SCROLL_MENU) return;

    for (int i = 0; i < 3; i++) {
        if (alarmSlots[i].isEnabled && !alarmSlots[i].isDaily) {
            if (displayTime.tm_hour == alarmSlots[i].alarmTime.tm_hour &&
                displayTime.tm_min  == alarmSlots[i].alarmTime.tm_min  &&
                displayTime.tm_sec  == alarmSlots[i].alarmTime.tm_sec) {
        
                activeToneIndex = alarmSlots[i].chosenAlarm; 
                isBuzzerActive = true;
            
                currentSnoozeCount = 0; 

              
                activeMaxSnoozes = alarmSlots[i].snoozesBeforeSilence;
                activeSnoozeMinutes = alarmSlots[i].snoozeDelay;

                int slotDurationChoice = alarmSlots[i].timeUntilSilence;
                if (slotDurationChoice == 3) {
                    isIndefiniteAlarm = true; 
                    buzzerTurnOffTime = 0; 
                } else {
                    isIndefiniteAlarm = false;
                    buzzerTurnOffTime = millis() + DURATION_VALUES[slotDurationChoice]; 
                }
                break;
            }
        }
        else if (alarmSlots[i].isEnabled && alarmSlots[i].isDaily) {
            if (displayTime.tm_hour == alarmSlots[i].alarmTime.tm_hour &&
                displayTime.tm_min  == alarmSlots[i].alarmTime.tm_min  &&
                displayTime.tm_sec  == alarmSlots[i].alarmTime.tm_sec  &&
                displayTime.tm_year == alarmSlots[i].alarmTime.tm_year &&                                                     
                displayTime.tm_mon  == alarmSlots[i].alarmTime.tm_mon  &&                                                        
                displayTime.tm_mday == alarmSlots[i].alarmTime.tm_mday) {
        
                activeToneIndex = alarmSlots[i].chosenAlarm; 
                isBuzzerActive = true;
            
                currentSnoozeCount = 0; 

              
                activeMaxSnoozes = alarmSlots[i].snoozesBeforeSilence;
                activeSnoozeMinutes = alarmSlots[i].snoozeDelay;

                int slotDurationChoice = alarmSlots[i].timeUntilSilence;
                 Serial.println("time till silence");
                Serial.println(alarmSlots[i].timeUntilSilence);
                if (slotDurationChoice == 0) {
                    isIndefiniteAlarm = true; 
                    buzzerTurnOffTime = 0; 
                } else {
                    isIndefiniteAlarm = false;
                    buzzerTurnOffTime = millis() + DURATION_VALUES[slotDurationChoice]; 
                }
                break;
            }
        }

    }
}


void playAlarmSound() {
    static int lastToneIndex = -1;
    static bool wasPlaying = false;

    if (!isBuzzerActive) {
        if (wasPlaying) {
        
            ledcWriteTone(0, 0); 
            
        
            ledcWrite(0, 0); 
            
            wasPlaying = false;
            lastToneIndex = -1;
            Serial.println("Hardware registers forcefully cleared to 0.");
        }
        return; 
    }


    if (!wasPlaying || activeToneIndex != lastToneIndex) {
        switch (activeToneIndex) {
            case 0:
                ledcWriteTone(0, 1000); 
                ledcWrite(0, 127);      
                break;
            case 1:
                ledcWriteTone(0, 600);  
                ledcWrite(0, 127);
                break;
            case 2:
                ledcWriteTone(0, 300);  
                ledcWrite(0, 127);
                break;
            default:
                ledcWriteTone(0, 0);
                ledcWrite(0, 0);
                wasPlaying = false;
                break;
        }

        if (isBuzzerActive) {
            wasPlaying = true;
            lastToneIndex = activeToneIndex;
        //} else if (isPreviewActive) {
        //    wasPlaying = true;

        }
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
            editAlarmBuffer.isDaily = false;
        } else {
            currentUIState = STATE_EDIT_YEAR;
            editAlarmBuffer.isDaily = true;
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
        editAlarmBuffer.snoozeDelay = 5;
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