#include <EEPROM.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define INTERRUPT_PIN PCINT1  // This is PB1 per the schematic
#define INT_PIN PB1           // Interrupt pin of choice: PB1 (same as PCINT1) - Pin 6
const int buttonPin = 3;    // the pin that the pushbutton is attached to
const int redPin = 0;       // the pin that the LED is attached to
const int greenPin = 4; 
const int eepromAddress = 0; // EEPROM address to store the count
#define PCINT_VECTOR PCINT0_vect      // This step is not necessary - it's a naming thing for clarit
// Variables will change:
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button
unsigned long lastActionTime = 0; // Store the last time an action occurred
const unsigned long timeout = 300; // 3 seconds timeout
int savedState = 0; // Variable to store the state
int tempCounter = 0;

void setup() {
  // initialize the LED as an output:
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  // initialize the button pin as a input:
  pinMode(buttonPin, INPUT_PULLUP);
  cli();                            // Disable interrupts during setup
  PCMSK |= (1 << INTERRUPT_PIN);    // Enable interrupt handler (ISR) for our chosen interrupt pin (PCINT1/PB1/pin 6)
  GIMSK |= (1 << PCIE);             // Enable PCINT interrupt in the general interrupt mask
  pinMode(INT_PIN, INPUT_PULLUP);   // Set our interrupt pin as input with a pullup to keep it stable
  sei();                            //last line of setup - enable interrupts after setup
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
  
  if (millis() - lastActionTime >= timeout ) {
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
    tempCounter = buttonPushCounter;
  } 
  if (millis() - lastActionTime >= 500 ) {
    buttonPushCounter = 2;
    lastActionTime = millis();
  }
}

void saveToEEPROM(int address, int value) {
  EEPROM.update(address, value); // Save the value to EEPROM
}

int readFromEEPROM(int address) {
  return EEPROM.read(address); // Read the value from EEPROM
}

ISR(PCINT_VECTOR)
{
  if( digitalRead(INT_PIN) == HIGH ) {
    buttonPushCounter = 3;
    // buttonPushCounter = readFromEEPROM(eepromAddress);
    lastActionTime = millis();
  }
  // else{
  //   buttonPushCounter = 2;
  //   lastActionTime = millis();
  // }
}
