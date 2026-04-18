/*
VarSpeedServoESP32 - Servo Library for ESP32

Modified by Lucas Emanuel B. Alves, 2026

Based on the original VarSpeedServo library:

- Copyright (c) 2009 Michael Margolis

- "slowmove()" function added in 2010 by Korman

- Updates 2013 by Philip van Allen (pva)

All rights reserved by the original authors.

This library is licensed under the GNU Lesser General Public License (LGPL) version 2.1 or later.

You may redistribute and/or modify it under the terms of the LGPL, available at:

https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html

This library is distributed in the hope that it will be useful, but without
any warranty; without even the implied warranty of merchantability
or fitness for a particular purpose.

Modifications by Lucas Emanuel B. Alves:

- Adapted for ESP32

- Speed control without the need for a separate loop function

- Other adjustments for hardware compatibility
*/

#ifndef VarSpeedServoESP32_h
#define VarSpeedServoESP32_h

#include <Arduino.h>

class VarSpeedServoESP32 {
public:
  VarSpeedServoESP32();

  uint8_t attach(int pin);

  void write(int value);                           // instant movement.
  void write(int value, uint8_t speed);           // movement with controlled speed.
  void write(int value, uint8_t speed, bool wait); // Movement with controlled speed and optional waiting time.

  void slowmove(int value, uint8_t speed, bool waitFlag = false);  //Like "write()", but doesn't "wait()".

  void wait();                                     // Block until the movement is complete.
  int read();                                      // current position.

  bool isMoving();  //returns if the servo is still moving.
  void moveSequence(int* positions, int size, uint8_t speed, bool waitFlag = true); //The servo moves through several positions in sequence.
  void detach();  //Turn off the servo and release the pin.
  bool attached(); //Checks whether the servo is connected (attached) or not.

  static void servoTask_trackingMove(void* param);
  void trackingMove(int value);

TaskHandle_t trackingTaskHandle;
private:
  int servoPin; 
  int servoChannel;

  int currentPos;
  int targetPos;
  float movingPos;

  uint8_t moveSpeed;
  int steps;
  float stepSize;
  unsigned long stepDelay;

  uint32_t angleToDuty(int angle);
  static int nextChannel;

  //Task handle for asynchronous motion
  TaskHandle_t taskHandle;

  //Function that runs in the FreeRTOS task.
  static void servoTask(void* param);
};



#endif
