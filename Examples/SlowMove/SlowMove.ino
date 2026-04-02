/*
  VarSpeedServoESP32 is an adaptation of the VarSpeedServo library by Michael Margolis (2009), 
  made by Lucas Emanuel B. Alves (2026), now compatible with ESP32.

  SlowMove

  Demonstrates the use of slowmove().
  This function moves the servo gradually
  using a defined speed.
*/

#include <VarSpeedServoESP32.h>

VarSpeedServoESP32 servo;

void setup() {

  servo.attach(18);

}

void loop() {

  // Move slowly (40% speed) to 180°
  servo.slowmove(180, 40);

  // Wait until movement finishes
  while (servo.isMoving()) {
    delay(50);
  }

  delay(1000);

  // Move back slowly (40% speed) to 0°
  servo.slowmove(0, 40);

  while (servo.isMoving()) {
    delay(50);
  }

  delay(1000);

}