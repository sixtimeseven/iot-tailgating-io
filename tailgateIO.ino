/*
  Tailgating Digital I/O with 1.5s output pulse

  This program will send a digital output signal (5V) on Pin D13 and
  illuminate the onboard LED labeled "L" for 1.5 seconds when it 
  receives an input signal on Pin D12. 

  *** NOTE ***
  The input pin on D12 is set to PULLUP, meaning it is inverted logic: 
    - Unlike using just an INPUT pin mode, no pull-down resistor is needed 
    - The Nano's internal 20K-ohm resistor is pulled UP to 5V
    - This causes the input to read HIGH when the switch is open,
    and LOW when it is closed.
    - This is a more reliable method because an accidental grounding
    of the wire will not trigger a false positive.

  The tailgate signal will come from the gate's 24V digital output and be passed 
  to pin D12 via an optocoupler. When the gate's output is active,
  the switch will close on the microcontroller side and the microcontroller
  will read a LOW signal: 
    - When pin 12 is LOW, tailgate signal received
    - When tailgate signal received, pulse D13 output for 1.5 second and 
    light the onboard "L" LED for 1.5 second

  * * HARDWARE * * 
    - Arduino Nano (or similar) with ATmega+328P chip
    - Optocoupler 817C Module
    - Magnetic Access gate with MGC
    - Whatever device/IoT board needed to receive the 1.5s pulse

  * * CIRCUIT * * 
  Nano D12 -> Optocoupler V1
  Nano GND -> Optocoupler G (on output side)
  Nano D13 -> IoT device input terminal
  Nano GND -> IoT device Ground terminal
  MGC DOut1 -> Optocoupler IN1
  MDC 0V -> Optocoupler G (on input side)

  Version: 1.0.0
  Created 2025-09-25
  by Steph Ostapovitch
  steph@stostx.ca
*/

const int tgPin = 12;
const int outPin = 13;

int lastTGState = 0;


void setup() {

  pinMode(tgPin, INPUT_PULLUP);
  pinMode(outPin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
}


void loop() {
  // read initial state
  int tgState = digitalRead(tgPin);

  if (lastTGState != tgState) {
    // tg state is LOW, input is ON
    if (tgState == LOW) {
      Serial.println("Tailgate ON");
      digitalWrite(LED_BUILTIN, HIGH);
      digitalWrite(outPin, HIGH);
      delay(1500);
    } else {
      // tg state is HIGH, input is OFF
      digitalWrite(LED_BUILTIN, LOW);
      digitalWrite(outPin, LOW);
      Serial.println("Tailgate OFF");
    }
    // small delay to avoid bouncing
    delay(50);
  }

  // set the state for the next loop
  lastTGState = tgState;
}
