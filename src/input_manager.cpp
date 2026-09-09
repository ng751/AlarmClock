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

void encoderDirection(bool inMenu) { //by default it changes screen brightness, if inMenu uis true it jumps into menu protocol
    CLK_current = digitalRead(INPUTS[ENCODER_CLK].pin);
   // DT_previous = digitalRead(ENCODER_DT)

   if (CLK_current == LOW && CLK_previous == HIGH) {

     if (digitalRead(INPUTS[ENCODER_DT].pin) != CLK_current) {
        counter++;
        displayString("CCW", 1, 0, 0, true);
     } else {
        counter--;
        displayString("CW", 1, 0, 0, true);
     }
   }  
   CLK_previous = CLK_current;
}

void automaticDimming() {
    Photoresistor_value = analogRead(3); //reads photoresistor input

    if (Photoresistor_value < 1000) { //automatic dimming, this can be cleaned up and probably mov
        displayDim(true);
    } else if (Photoresistor_value >= 1000) {
        displayDim(false);
    }
}

void manualDimming(bool dim) {
 
}

//int retrieveEncoderDirection() {
  //  return counter;
//}