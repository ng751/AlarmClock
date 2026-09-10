/* This script handles button inputs including the rotary encoder, switches, and photoresistor.
*/

#include <input_manager.h>
#include <display_manager.h>

int CLK_current; //initalizes variables that will hold the state of both encoder_clk and encoder_dt
int CLK_previous;

int counter = 0;
int DT_current;
int DT_previous;
int Photoresistor_value;
bool override;
bool updateDisplay;


void initInputManager() {
    // Loop through the structures and extract the exact pin numbers
    for (int i = 0; i < NUM_INPUTS; i++) {
        pinMode(INPUTS[i].pin, INPUT_PULLUP);
    }

}
void encoderInit() {
    CLK_previous = digitalRead(INPUTS[ENCODER_CLK].pin);
}

bool inputPressed(InputIndex btn) { //for buttons/switches. rotary encoder cw and ccw are handles separately
    if (digitalRead(INPUTS[btn].pin) == LOW) {
        return true;
    } else {
        return false;
    }
}

void encoderDirection() {
  // read clk pin
  CLK_current = digitalRead(INPUTS[ENCODER_CLK].pin); // Assuming your clock pin setup

  if (CLK_current == LOW && CLK_previous == HIGH) {
    override = true;
    
    // determine direction of the turn
    bool isClockwise = (digitalRead(INPUTS[ENCODER_DT].pin) != CLK_current);
    
    bool updateDisplay = false; //dont update since nothing has happened

    if (currentUIState != STATE_DEFAULT && currentUIState != STATE_SCROLL_MENU) {
        if (isClockwise) {
            if (currentUIState == STATE_EDIT_YEAR)  userYear++;
            if (currentUIState == STATE_EDIT_MONTH) { userMonth++; if(userMonth > 12) userMonth = 1; }
            if (currentUIState == STATE_EDIT_DAY)   { userDay++;   if(userDay > 31)   userDay = 1; }
            if (currentUIState == STATE_EDIT_HOUR)  { userHour++;  if(userHour > 23)  userHour = 0; }
            if (currentUIState == STATE_EDIT_MINUTES)   { userMin++;   if(userMin > 59)   userMin = 0; }
            if (currentUIState == STATE_EDIT_SECONDS)   { userSec--;   if(userMin < 0)   userMin = 59; }
            if (currentUIState == STATE_EDIT_AMPM)   { userAMPM;   if(userAMPM = "AM")   userAMPM = "PM"; }
        } else {
            if (currentUIState == STATE_EDIT_YEAR)  userYear--;
            if (currentUIState == STATE_EDIT_MONTH) { userMonth--; if(userMonth < 1)  userMonth = 12; }
            if (currentUIState == STATE_EDIT_DAY)   { userDay--;   if(userDay < 1)   userDay = 31; }
            if (currentUIState == STATE_EDIT_HOUR)  { userHour--;  if(userHour < 0)   userHour = 23; }
            if (currentUIState == STATE_EDIT_MINUTES)   { userMin--;   if(userMin < 0)   userMin = 59; }
            if (currentUIState == STATE_EDIT_SECONDS)   { userSec--;   if(userMin < 0)   userMin = 59; }
             if (currentUIState == STATE_EDIT_AMPM)   { userAMPM;   if(userAMPM = "PM")   userAMPM = "AM"; }
        }

    }

    switch (currentUIState) {
      
      case STATE_DEFAULT:
        // default screen, turning the knob dims or brightens the display
        if (isClockwise) {
          Serial.println("illuminate");
          displayDim(false, true); // go backward toward full illumination (2 -> 1 -> 0)
        } else {
          Serial.println("dim");
          displayDim(true, true);  // go further into dimming (0 -> 1 -> 2)
        }
        break;

      case STATE_SCROLL_MENU:
        // in a menu list
        if (isClockwise) {
          counter++; // scroll down
          Serial.println("Menu: Scroll Down");
        } else {
          counter--; // to scroll up
          Serial.println("Menu: Scroll Up");
        }
        updateDisplay = true; // Flag that the menu text needs to change
        break;

      
    }

    if (updateDisplay) {
      if (isClockwise) {
        displayString("CW", 4, 0, 0, true);
      } else {
        displayString("CCW", 4, 0, 0, true);
      }
    }
  }
  
  CLK_previous = CLK_current;
}

void automaticDimming() {
    Photoresistor_value = analogRead(3); //reads photoresistor input
    Serial.println(Photoresistor_value);
    if (Photoresistor_value < 1000) { //automatic dimming, this can be cleaned up
        displayDim(true, false);
        override = false; //override waits until the user covers the photoresistor again to reactivate its capabilities
    } else if (Photoresistor_value >= 1000 && (override == false)) {
        displayDim(false, false);
    } 
}

