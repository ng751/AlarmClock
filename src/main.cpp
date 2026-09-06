#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <input_manager.h>

#define I2C_SDA 8
#define I2C_SCL 9

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64

#define button1 15
#define button2 7
#define button3 5



Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


void setup() {
    Serial.begin(115200);
    delay(200);
    initInputManager();
    pinMode(button1, INPUT_PULLUP);
    pinMode(button2, INPUT_PULLUP);
    pinMode(button3, INPUT_PULLUP);
    pinMode(6, OUTPUT);
    Wire.begin(I2C_SDA, I2C_SCL);

    delay(1000);

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;);
    }

    display.clearDisplay();
    display.display();
}   

void loop() {
    if (digitalRead(button1) == LOW) {
        display.clearDisplay();
        digitalWrite(6, HIGH);
         Serial.println("Button 1");
        display.setTextSize(3);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(10, 16);
        display.println("Button 1");
        display.display();
    } else if (digitalRead(button2) == LOW) {
        display.clearDisplay();
        Serial.println("Button 2");
        display.setTextSize(3);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(10, 16);
        display.println("Button 2");
        display.display();
    } else if( digitalRead(button3) == LOW) {
        display.clearDisplay();
        Serial.println("Button 3");
        display.setTextSize(3);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(10, 16);
        display.println("Button 3");
        display.display();
    } else {
        Serial.println("No button pressed");
        digitalWrite(6, LOW);
    }
  
}