# VarSpeedServoESP32
VarSpeedServoESP32 é uma adaptação da biblioteca VarSpeedServo de Michael Margolis (2009), feita por Lucas Emanuel B. Alves (2026), agora compatível com ESP32.

# VarSpeedServoESP32

Biblioteca para controle de servos com **velocidade variável**, adaptada para o **ESP32**, mantendo a simplicidade e funcionalidades da biblioteca **VarSpeedServo** original.

Permite controlar servos com velocidade variável, mantendo a simplicidade e funcionalidades da versão original, mas adaptada para os timers do ESP32.

---

# Funcionalidades

## Controle de posição com velocidade variável

```cpp
write(pos, speed);
write(pos, speed, wait); // opcional: espera o movimento terminar
