// VarSpeedServoESP32 - baseado em VarSpeedServo original, LGPL 2.1

#include "VarSpeedServoESP32.h"

#define SERVO_FREQ 50
#define SERVO_RESOLUTION 16

int VarSpeedServoESP32::nextChannel = 0;

VarSpeedServoESP32::VarSpeedServoESP32() {
  currentPos = 90;
  targetPos = 90;
  movingPos = 90;
  moveSpeed = 100;
  steps = 0;
  stepSize = 0;
  stepDelay = 20;
  taskHandle = NULL;
}

uint32_t VarSpeedServoESP32::angleToDuty(int angle) {
  int minUs = 500;
  int maxUs = 2500;
  int pulse = map(angle, 0, 180, minUs, maxUs);
  return (pulse * ((1 << SERVO_RESOLUTION) - 1)) / 20000;
}

uint8_t VarSpeedServoESP32::attach(int pin) {
  servoPin = pin;
  servoChannel = nextChannel++;
  if (servoChannel > 15) return 0;

  ledcAttach(servoPin, SERVO_FREQ, SERVO_RESOLUTION);
  write(90);

  xTaskCreatePinnedToCore(
    servoTask,
    "ServoTask",
    2048,
    this,
    1,
    &taskHandle,
    1
  );

  return 1;
}

void VarSpeedServoESP32::write(int value) {
  currentPos = value;
  movingPos = value;
  targetPos = value;
  ledcWrite(servoPin, angleToDuty(value));
}

void VarSpeedServoESP32::write(int value, uint8_t speed) {
  targetPos = value;
  if (speed == 0) speed = 100;
  moveSpeed = speed;

  int diff = targetPos - currentPos;
  steps = map(moveSpeed, 1, 100, 200, 5);
  stepSize = (float)diff / steps;
  stepDelay = 20;
}

void VarSpeedServoESP32::write(int value, uint8_t speed, bool waitFlag) {
  write(value, speed);
  if (waitFlag) wait();
}

void VarSpeedServoESP32::servoTask(void* param) {
  VarSpeedServoESP32* self = (VarSpeedServoESP32*)param;

  for (;;) {
    if (self->movingPos != self->targetPos) {
      self->movingPos += self->stepSize;

      if ((self->stepSize > 0 && self->movingPos > self->targetPos) ||
          (self->stepSize < 0 && self->movingPos < self->targetPos))
        self->movingPos = self->targetPos;

      self->currentPos = self->movingPos;
      ledcWrite(self->servoPin, self->angleToDuty((int)self->movingPos));
    }
    vTaskDelay(pdMS_TO_TICKS(self->stepDelay));
  }
}

void VarSpeedServoESP32::wait() {
  while (movingPos != targetPos) {
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

bool VarSpeedServoESP32::isMoving() {
  return movingPos != targetPos;
}

void VarSpeedServoESP32::moveSequence(int* positions, int size, uint8_t speed, bool waitFlag) {
  for (int i = 0; i < size; i++) {
    write(positions[i], speed);
    if (waitFlag) wait();
  }
}

void VarSpeedServoESP32::detach() {
  ledcDetach(servoChannel);
  servoPin = -1;
}

bool VarSpeedServoESP32::attached() {
  return servoPin != -1;
}

void VarSpeedServoESP32::slowmove(int value, uint8_t speed, bool waitFlag) {
  write(value, speed, waitFlag);
}

int VarSpeedServoESP32::read() {
  return currentPos;
}