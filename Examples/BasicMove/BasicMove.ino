/*
  VarSpeedServoESP32 is an adaptation of the VarSpeedServo library by Michael Margolis (2009), 
  made by Lucas Emanuel B. Alves (2026), now compatible with ESP32.

  BasicMove

  Demonstrates the most basic use of the VarSpeedServoESP32 library.
  The servo moves instantly between two positions.

  This is the first example users should try.
*/

#include <VarSpeedServoESP32.h>

VarSpeedServoESP32 servo;

void setup() {

  // Attach the servo to GPIO 18
  servo.attach(18);

}

void loop() {

  // Move instantly to 0 degrees
  servo.write(0);
  delay(1000);

  // Move instantly to 180 degrees
  servo.write(180);
  delay(1000);

}
