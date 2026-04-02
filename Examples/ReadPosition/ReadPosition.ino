/*
  VarSpeedServoESP32 is an adaptation of the VarSpeedServo library by Michael Margolis (2009), 
  made by Lucas Emanuel B. Alves (2026), now compatible with ESP32.

  ReadPosition

  Demonstrates how to read the current
  servo position during movement.
*/

#include <VarSpeedServoESP32.h>

VarSpeedServoESP32 servo;

void setup() {

  Serial.begin(115200);

  servo.attach(18);

}

void loop() {

  // Start movement
  servo.write(180, 40);

  // Print position while moving
  while (servo.isMoving()) {

    Serial.print("Current position: ");
    Serial.println(servo.read());

    delay(100);

  }

  delay(2000);

  // Move back
  servo.write(0, 40);

  while (servo.isMoving()) {

    Serial.print("Current position: ");
    Serial.println(servo.read());

    delay(100);

  }

  delay(2000);

}