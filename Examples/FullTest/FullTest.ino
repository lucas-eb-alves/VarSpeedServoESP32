/*
  VarSpeedServoESP32 is an adaptation of the VarSpeedServo library by Michael Margolis (2009), 
  made by Lucas Emanuel B. Alves (2026), now compatible with ESP32.

  FullTest

  Demonstrates most features of the library
  in a single example.

  Useful for testing and debugging.
*/

#include <VarSpeedServoESP32.h>

VarSpeedServoESP32 servo;

void setup() {

  Serial.begin(115200);

  servo.attach(18);

  Serial.println("Servo attached!");

}

void loop() {

  // --- Instant movement ---
  Serial.println("\n== Instant move to 0° ==");

  servo.write(0);

  delay(1000);

  // --- Slow movement ---
  Serial.println("\n== slowmove to 180° ==");

  servo.slowmove(180, 50);

  while (servo.isMoving()) {

    Serial.print("Moving... Position: ");
    Serial.println(servo.read());

    delay(100);

  }

  // --- Write with wait ---
  Serial.println("\n== write with wait to 90° ==");

  servo.write(90, 60, true);

  Serial.println("Position reached.");

  // --- Sequence movement ---
  int seq[] = {0, 45, 90, 135, 180};

  Serial.println("\n== Sequence movement ==");

  for (int i = 0; i < 5; i++) {

    Serial.print("Moving to ");
    Serial.println(seq[i]);

    servo.write(seq[i], 70, true);

    Serial.print("Reached position: ");
    Serial.println(servo.read());

  }

  // --- Detach test ---
  Serial.print("\nServo attached? ");

  Serial.println(servo.attached() ? "YES" : "NO");

  Serial.println("Detaching servo...");

  servo.detach();

  Serial.print("Servo attached? ");

  Serial.println(servo.attached() ? "YES" : "NO");

  // --- Reattach ---
  Serial.println("Reattaching servo...");

  servo.attach(18);

  delay(3000);

}