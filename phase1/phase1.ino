
// this constant won't change:
const int switchPin = 5;   // Switch connected to digital pin 2
const int redLedPin = 3;       // the pin that the LED is attached to
const int greenLedPin = 4; 
const int tempPin = 0;
// Variables will change:
int count = 0;
void setup() {
  // initialize the button pin as a input:
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(switchPin, INPUT_PULLUP);
  // initialize serial communication:
  Serial.begin(9600);
  digitalWrite(redLedPin, LOW);   // Turn off red LED
    digitalWrite(greenLedPin, LOW);
}


void loop() {
  // read the pushbutton input pin:
  int switchState = digitalRead(switchPin);

  int tempvalue = analogRead(tempPin);
  float value = tempvalue * 1/2;
  if (switchState == HIGH) {
    if(count == 0) delay(300);
    if(value > 30) {
      digitalWrite(redLedPin, HIGH);   // Turn on red LED
      digitalWrite(greenLedPin, LOW);  // Turn off green LED
    }
    else {
      digitalWrite(redLedPin, LOW);   // Turn off red LED
      digitalWrite(greenLedPin, HIGH);  // Turn on green LED
    }
    count++;
  } else {
    delay(300);
    digitalWrite(redLedPin, LOW);   // Turn off red LED
    digitalWrite(greenLedPin, LOW);  // Turn off green LED
  }
}