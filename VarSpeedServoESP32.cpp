//VarSpeedServoESP32 - based on the original VarSpeedServo, GNU LGPL 2.1

/*
VarSpeedServoESP32 - Servo Library for ESP32

Modified by Lucas Emanuel B. Alves, 2026

Based on the original VarSpeedServo library:
- Copyright (c) 2009 Michael Margolis
- "slowmove()" function added in 2010 by Korman
- Updates 2013 by Philip van Allen (pva)

All rights reserved by the original authors.

Licensed under the GNU LGPL v2.1 or later.
https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html
*/

#include "VarSpeedServoESP32.h"
#include <Arduino.h>

// ================= PWM =================

static const int MIN_PULSE = 500;
static const int MAX_PULSE = 2400;

// ================= STATIC =================

int VarSpeedServoESP32::nextChannel = 0;

// ================= CONSTRUCTOR =================

VarSpeedServoESP32::VarSpeedServoESP32() {
  servoPin     = -1;
  servoChannel = -1;

  currentPos   = 90.0f;
  targetPos    = 90;
  movingPos    = 90.0f;

  moveSpeed    = 50;

  steps        = 0;
  stepSize     = 0.0f;
  stepDelay    = 10;

  taskHandle   = NULL;
}

// ================= ATTACH =================

uint8_t VarSpeedServoESP32::attach(int pin) {
  if (nextChannel >= 16) return 0;

  servoPin     = pin;
  servoChannel = nextChannel++;

  ledcSetup(servoChannel, 50, 16);
  ledcAttachPin(servoPin, servoChannel);

  currentPos = 90.0f;
  targetPos  = 90;
  movingPos  = 90.0f;

  ledcWrite(servoChannel, angleToDuty(90));

  // Task de tracking — criada uma vez, roda para sempre
  xTaskCreatePinnedToCore(
    servoTask,
    "servoTask",
    2048,
    this,
    1,
    &taskHandle,
    1
  );

  return 1;
}

// ================= DETACH =================

void VarSpeedServoESP32::detach() {
  if (servoPin >= 0) {
    if (taskHandle != NULL) {
      vTaskDelete(taskHandle);
      taskHandle = NULL;
    }
    ledcDetachPin(servoPin);
    servoPin     = -1;
    servoChannel = -1;
  }
}

// ================= ATTACHED =================

bool VarSpeedServoESP32::attached() {
  return servoPin >= 0;
}

// ================= ANGLE TO DUTY =================

uint32_t VarSpeedServoESP32::angleToDuty(int angle) {
  angle = constrain(angle, 0, 180);
  return map(
    angle,
    0, 180,
    MIN_PULSE * 65536 / 20000,
    MAX_PULSE * 65536 / 20000
  );
}

// ================= WRITE =================

void VarSpeedServoESP32::write(int value) {
  write(value, moveSpeed, false);
}

void VarSpeedServoESP32::write(int value, uint8_t speed) {
  write(value, speed, false);
}

void VarSpeedServoESP32::write(int value, uint8_t speed, bool waitFlag) {
  if (!attached()) return;

  targetPos = constrain(value, 0, 180);
  moveSpeed = constrain(speed, 1, 100);

  if (waitFlag) wait();
}

// ================= SLOWMOVE =================

void VarSpeedServoESP32::slowmove(int value, uint8_t speed, bool waitFlag) {
  write(value, speed, waitFlag);
}

// ================= TASK (EXPONENTIAL SMOOTHING) =================
//
// currentPos += alpha * (targetPos - currentPos)
//
// Tick fixo de 10ms garante cadência constante.
// Alpha controla quão rápido o servo segue o alvo:
//   speed 1   → alpha 0.01 → movimento muito lento
//   speed 50  → alpha 0.20 → movimento médio
//   speed 100 → alpha 0.40 → quase instantâneo
//
// Como o passo é proporcional ao erro, o movimento é
// naturalmente suave: rápido quando longe, preciso quando perto.

void VarSpeedServoESP32::servoTask(void* param) {
  VarSpeedServoESP32* s = (VarSpeedServoESP32*)param;

  const TickType_t tickRate = 10 / portTICK_PERIOD_MS;

  while (true) {
    float target  = (float)s->targetPos;
    float current = s->currentPos;
    float diff    = target - current;

    if (fabsf(diff) > 0.1f) {

      // Alpha derivado da velocidade configurada
      float alpha = (float)s->moveSpeed / 250.0f;
      alpha = constrain(alpha, 0.01f, 0.40f);

      s->currentPos += alpha * diff;

      // Snap final: evita ficar oscilando perto do alvo
      if (fabsf(target - s->currentPos) < 0.2f) {
        s->currentPos = target;
      }

      ledcWrite(
        s->servoChannel,
        s->angleToDuty((int)roundf(s->currentPos))
      );
    }

    vTaskDelay(tickRate);
  }
}

// ================= READ =================

int VarSpeedServoESP32::read() {
  return (int)roundf(currentPos);
}

// ================= IS MOVING =================

bool VarSpeedServoESP32::isMoving() {
  return fabsf(currentPos - (float)targetPos) > 0.2f;
}

// ================= WAIT =================

void VarSpeedServoESP32::wait() {
  while (isMoving()) {
    delay(5);
  }
}

// ================= SEQUENCE =================

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
