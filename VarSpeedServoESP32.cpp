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

// Dead zone para evitar micro-ajustes desnecessários
static const int DEADZONE = 2;

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
  stepDelay    = 20;

  taskHandle   = NULL;
}

// ================= ATTACH =================

uint8_t VarSpeedServoESP32::attach(int pin) {
  if (nextChannel >= 16) return 0;

  servoPin = pin;

  // Reutiliza o canal se já teve um antes (reattach)
  if (servoChannel < 0) {
    servoChannel = nextChannel++;
  }

  ledcSetup(servoChannel, 50, 16);
  ledcAttachPin(servoPin, servoChannel);

  // Mantém a posição atual ao reattach
  ledcWrite(servoChannel, angleToDuty((int)currentPos));

  return 1;
}

// ================= DETACH =================

void VarSpeedServoESP32::detach() {
  if (servoPin >= 0) {
    // Para a task se estiver rodando
    if (taskHandle != NULL) {
      vTaskDelete(taskHandle);
      taskHandle = NULL;
    }
    ledcDetachPin(servoPin);
    servoPin = -1;
    // Preserva servoChannel para reattach funcionar
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

// Sem speed: movimento instantâneo
void VarSpeedServoESP32::write(int value) {
  if (!attached()) return;

  // Para qualquer task em andamento
  if (taskHandle != NULL) {
    vTaskDelete(taskHandle);
    taskHandle = NULL;
  }

  targetPos  = constrain(value, 0, 180);
  currentPos = (float)targetPos;
  movingPos  = (float)targetPos;

  ledcWrite(servoChannel, angleToDuty(targetPos));
}

// Com speed: movimento suave via task
void VarSpeedServoESP32::write(int value, uint8_t speed) {
  write(value, speed, false);
}

void VarSpeedServoESP32::write(int value, uint8_t speed, bool waitFlag) {
  if (!attached()) return;

  targetPos = constrain(value, 0, 180);

  // Dead zone: ignora se já está perto o suficiente
  if (abs((int)currentPos - targetPos) <= DEADZONE) return;

  moveSpeed = constrain(speed, 1, 100);

  // Calcula passos e direção
  steps    = abs(targetPos - (int)currentPos);
  stepSize = (float)(targetPos - (int)currentPos) / (float)steps;

  // speed 1 → 20ms/passo (lento), speed 100 → 1ms/passo (rápido)
  stepDelay = map(moveSpeed, 1, 100, 20, 1);

  // Para task anterior se existir
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

// ================= SLOWMOVE =================

void VarSpeedServoESP32::slowmove(int value, uint8_t speed, bool waitFlag) {
  write(value, speed, waitFlag);
}

// ================= TASK =================

void VarSpeedServoESP32::servoTask(void* param) {
  VarSpeedServoESP32* s = (VarSpeedServoESP32*)param;

  s->movingPos = s->currentPos;

  for (int i = 0; i < s->steps; i++) {
    s->movingPos += s->stepSize;

    int angle = (int)roundf(s->movingPos);
    angle = constrain(angle, 0, 180);

    ledcWrite(s->servoChannel, s->angleToDuty(angle));

    vTaskDelay(s->stepDelay / portTICK_PERIOD_MS);
  }

  s->currentPos = (float)s->targetPos;
  s->movingPos  = s->currentPos;
  s->taskHandle = NULL;

  vTaskDelete(NULL);
}

// ================= READ =================

int VarSpeedServoESP32::read() {
  return (int)roundf(currentPos);
}

// ================= IS MOVING =================

bool VarSpeedServoESP32::isMoving() {
  return taskHandle != NULL;
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
