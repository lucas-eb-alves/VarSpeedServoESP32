/*
  VarSpeedServoESP32 is an adaptation of the VarSpeedServo library by Michael Margolis (2009), 
  made by Lucas Emanuel B. Alves (2026), now compatible with ESP32.

  SpeedControl

  Demonstrates how to control servo speed.
  Lower speed values = slower movement.
  Higher speed values = faster movement.
*/

#include <VarSpeedServoESP32.h>

VarSpeedServoESP32 servo;

void setup() {

  servo.attach(18);

}

void loop() {

  // Move slowly (20% speed) to 180 degrees
  servo.write(180, 20);
  delay(2000);

  // Move faster (80% speed) back to 0 degrees
  servo.write(0, 80);
  delay(2000);

}