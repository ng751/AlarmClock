/* This script handles button inputs including the rotary encoder, switches, and photoresistor.
*/

#include <input_manager.h>

void initInputManager() {
    // Loop through the structures and extract the exact pin numbers
    for (int i = 0; i < NUM_INPUTS; i++) {
        pinMode(INPUTS[i].pin, INPUT_PULLUP);
    }
}

bool inputPressed(InputIndex btn) {
    if (digitalRead(INPUTS[btn].pin) == LOW) {
        return true;
    } else {
        return false;
    }
}