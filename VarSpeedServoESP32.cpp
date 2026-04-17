//VarSpeedServoESP32 - based on the original VarSpeedServo, GNU LGPL 2.1

#include "VarSpeedServoESP32.h"

// PWM range
static const int MIN_PULSE = 500;
static const int MAX_PULSE = 2400;

// Dead zone
static const int DEADZONE = 2;

int VarSpeedServoESP32::nextChannel = 0;

VarSpeedServoESP32::VarSpeedServoESP32() {

  servoPin = -1;
  servoChannel = -1;

  currentPos = 90;
  targetPos = 90;
  movingPos = 90;

  moveSpeed = 50;

  steps = 0;
  stepSize = 0;
  stepDelay = 20;

  taskHandle = NULL;
}

uint8_t VarSpeedServoESP32::attach(int pin) {

  if (nextChannel >= 16) return 0;

  servoPin = pin;
  servoChannel = nextChannel++;

  ledcSetup(servoChannel, 50, 16);
  ledcAttachPin(servoPin, servoChannel);

  ledcWrite(servoChannel, angleToDuty(currentPos));

  return 1;
}

void VarSpeedServoESP32::detach() {

  if (servoPin >= 0) {

    ledcDetachPin(servoPin);

    if (taskHandle != NULL) {
      vTaskDelete(taskHandle);
      taskHandle = NULL;
    }

    servoPin = -1;
    servoChannel = -1;
  }
}

bool VarSpeedServoESP32::attached() {

  return servoPin >= 0;
}

uint32_t VarSpeedServoESP32::angleToDuty(int angle) {

  angle = constrain(angle, 0, 180);

  uint32_t duty = map(
    angle,
    0,
    180,
    MIN_PULSE * 65536 / 20000,
    MAX_PULSE * 65536 / 20000
  );

  return duty;
}

void VarSpeedServoESP32::write(int value) {

  write(value, moveSpeed, false);
}

void VarSpeedServoESP32::write(
  int value,
  uint8_t speed,
  bool waitFlag
) {

  if (!attached()) return;

  targetPos = constrain(value, 0, 180);

  if (abs(currentPos - targetPos) <= DEADZONE)
    return;

  moveSpeed = constrain(speed, 1, 100);

  steps = abs(targetPos - currentPos);

  stepSize = (float)(targetPos - currentPos) / steps;

  stepDelay = map(moveSpeed, 1, 100, 20, 1);

  if (taskHandle != NULL) {

    vTaskDelete(taskHandle);
    taskHandle = NULL;
  }

  xTaskCreatePinnedToCore(
    servoTask,
    "servoTask",
    2048,
    this,
    1,
    &taskHandle,
    1
  );

  if (waitFlag) wait();
}

void VarSpeedServoESP32::slowmove(
  int value,
  uint8_t speed,
  bool waitFlag
) {

  write(value, speed, waitFlag);
}

void VarSpeedServoESP32::servoTask(void* param) {

  VarSpeedServoESP32* servo =
    (VarSpeedServoESP32*)param;

  servo->movingPos = servo->currentPos;

  for (int i = 0; i < servo->steps; i++) {

    servo->movingPos += servo->stepSize;

    int angle = (int)servo->movingPos;

    ledcWrite(
      servo->servoChannel,
      servo->angleToDuty(angle)
    );

    vTaskDelay(
      servo->stepDelay / portTICK_PERIOD_MS
    );
  }

  servo->currentPos = servo->targetPos;

  servo->taskHandle = NULL;

  vTaskDelete(NULL);
}

void VarSpeedServoESP32::wait() {

  while (taskHandle != NULL) {

    delay(5);
  }
}

int VarSpeedServoESP32::read() {

  return currentPos;
}

bool VarSpeedServoESP32::isMoving() {

  return taskHandle != NULL;
}

void VarSpeedServoESP32::moveSequence(
  int* positions,
  int size,
  uint8_t speed,
  bool waitFlag
) {

  for (int i = 0; i < size; i++) {

    write(positions[i], speed, true);
  }
}
