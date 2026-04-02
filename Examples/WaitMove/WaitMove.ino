/*
  VarSpeedServoESP32 is an adaptation of the VarSpeedServo library by Michael Margolis (2009), 
  made by Lucas Emanuel B. Alves (2026), now compatible with ESP32.

  WaitMove

  Demonstrates blocking movement using "wait = true".
  The program waits until the movement finishes.
*/

#include <VarSpeedServoESP32.h>

VarSpeedServoESP32 servo;

void setup() {

  servo.attach(18);

}

void loop() {

  // Move to 90° and wait until finished
  servo.write(90, 60, true);

  delay(1000);

  // Move back to 0° and wait
  servo.write(0, 60, true);

  delay(1000);

}
