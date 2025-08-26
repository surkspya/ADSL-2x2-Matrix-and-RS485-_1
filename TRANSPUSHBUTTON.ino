#include <SoftwareSerial.h>

SoftwareSerial RS485Serial(10, 11);

const int buttonpin = 9;

int buttonState;
int lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

void setup() {
  pinMode(buttonpin, INPUT_PULLUP);
  buttonState = digitalRead(buttonpin);

  RS485Serial.begin(9600);  // baudrate for RS485 link
}

void loop() {
  int reading = digitalRead(buttonpin);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading == LOW && buttonState == HIGH) {
      // Button just pressed → send message
      RS485Serial.println("BUTTON_PRESSED");
    }
    buttonState = reading;
  }

  lastButtonState = reading;
}
