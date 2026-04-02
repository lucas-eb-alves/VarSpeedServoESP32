# VarSpeedServoESP32

VarSpeedServoESP32 is an adaptation of the VarSpeedServo library by Michael Margolis (2009), made by Lucas Emanuel B. Alves (2026), now compatible with ESP32.

It allows controlling servos with variable speed, maintaining the simplicity and functionality of the original version, but adapted for ESP32 timers.

---

# Features

- Control of servo position with variable speed:

```cpp
write(pos, speed);

write(pos, speed, wait); // optional: "waits" for the movement to finish

```
- Basic servo functions:

  - attach(), detach()
  
  - read(), readMicroseconds()
  
  - writeMicroseconds()
  
  - attached()
  
  - stop()
  
  - wait()

  - Compatibility with "slowmove()" from the original version.
  
  - Designed for ESP32, using compatible timers.

Note: Sequences "sequencePlay()", "sequenceStop()" and "isMoving()" are not implemented in this version.

### Similarities:

- Basic servo control functions: "attach()", "detach()", "read()", "readMicroseconds()", "writeMicroseconds()", "attached()", "stop()", and "wait()".

- "slowmove()" function maintained for compatibility.

- Allows variable speed position control.

- Sequence and "isMoving()" functions implemented.

### Differences:

- Compatible with ESP32.

- Adaptation of timers to work on the ESP32, unlike the Arduino.

# Simple Example

```cpp

#include <VarSpeedServoESP32.h>

VarSpeedServoESP32 servo;

void setup() {

servo.attach(18); // ESP32 port 18

}

void loop() {

servo.write(0, 60, true); // move to 0° at 60% speed
                  // wait finish
delay(500);

servo.write(180, 90, true); // move to 180° at 90% speed
                  // wait finish
delay(500);

}

```

# Installation

1. Download the library's .zip file from GitHub.

2. In the Arduino IDE, go to Sketch > Include Library > Add .ZIP Library… and select the file.

3. The library will be installed in Documents/Arduino/libraries.

Or install manually by extracting the .zip file to the Arduino libraries folder.

# Credits and License

- Original Author: Michael Margolis (2009) – Original VarSpeedServo
- Adaptation for ESP32: Lucas Emanuel B. Alves (2026) - VarSpeedServoESP32
- Maintains LGPL 2.1 license, guaranteeing free use, modification and distribution of the library.

