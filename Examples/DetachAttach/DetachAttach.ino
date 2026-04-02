/*
  VarSpeedServoESP32 is an adaptation of the VarSpeedServo library by Michael Margolis (2009), 
  made by Lucas Emanuel B. Alves (2026), now compatible with ESP32.

  DetachAttach

  Demonstrates how to detach and reattach
  the servo from the control pin.

  Useful to save power or release the servo.
*/

#include <VarSpeedServoESP32.h>

VarSpeedServoESP32 servo;

void setup() {

  Serial.begin(115200);

  servo.attach(18);

}

void loop() {

  // Move servo
  servo.write(90);
  delay(2000);

  // Check if attached
  Serial.print("Servo attached: ");
  Serial.println(servo.attached() ? "YES" : "NO");

  // Detach servo
  Serial.println("Detaching servo...");
  servo.detach();

  delay(3000);

  // Check again
  Serial.print("Servo attached: ");
  Serial.println(servo.attached() ? "YES" : "NO");

  // Reattach servo
  Serial.println("Reattaching servo...");
  servo.attach(18);

  delay(3000);

}
