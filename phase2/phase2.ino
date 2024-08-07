#include <EEPROM.h>

const int  buttonPin = 3;    // the pin that the pushbutton is attached to
const int redPin = 0;       // the pin that the LED is attached to
const int greenPin = 2; 
const int eepromAddress = 0; // EEPROM address to store the count
// Variables will change:
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button
unsigned long lastActionTime = 0; // Store the last time an action occurred
const unsigned long timeout = 300; // 3 seconds timeout
int savedState = 0; // Variable to store the state

void setup() {
  // initialize the LED as an output:
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  // initialize the button pin as a input:
  pinMode(buttonPin, INPUT_PULLUP);
  // Read the saved count from EEPROM
  buttonPushCounter = readFromEEPROM(eepromAddress);
  // initialize serial communication:
  Serial.begin(9600);
  // Optionally, use the count to set the initial state of an output pin or for other logic
  if (buttonPushCounter > 2 && buttonPushCounter % 3 == 0) {
    digitalWrite(redPin, HIGH);
    digitalWrite(greenPin, LOW);
  } else if (buttonPushCounter > 2 && buttonPushCounter % 3 == 1){
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, HIGH);
  } else {
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, LOW);
  }
}

void loop() {
  // read the pushbutton input pin:
  buttonState = digitalRead(buttonPin);

  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == HIGH) {
      buttonPushCounter++;
      lastActionTime = millis();
    } 
  }
  lastButtonState = buttonState;

  if (millis() - lastActionTime >= timeout) {
    saveToEEPROM(eepromAddress, buttonPushCounter);
    if (buttonPushCounter > 2 && buttonPushCounter % 3 == 0) {
      digitalWrite(redPin, HIGH);
      digitalWrite(greenPin, LOW);
    }
    else if (buttonPushCounter > 2 && buttonPushCounter % 3 == 1) {
      digitalWrite(redPin, LOW);
      digitalWrite(greenPin, HIGH);
    }
    else {
      digitalWrite(redPin, LOW);
      digitalWrite(greenPin, LOW);
    }
  } 
}

void saveToEEPROM(int address, int value) {
  EEPROM.update(address, value); // Save the value to EEPROM
}

int readFromEEPROM(int address) {
  return EEPROM.read(address); // Read the value from EEPROM
}