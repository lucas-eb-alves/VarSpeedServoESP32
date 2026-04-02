/*
  VarSpeedServoESP32 - Biblioteca de servo para ESP32
  Modificado por Lucas Emanuel B. Alves, 2026
  Baseado na biblioteca VarSpeedServo original:
    - Copyright (c) 2009 Michael Margolis
    - Função slowmove adicionada em 2010 por Korman
    - Atualizações 2013 por Philip van Allen (pva)
  Todos os direitos reservados pelos autores originais.

  Esta biblioteca é licenciada sob a GNU Lesser General Public License (LGPL) versão 2.1 ou superior.
  Você pode redistribuir e/ou modificar sob os termos da LGPL, disponível em:
  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html

  Esta biblioteca é distribuída na esperança de que seja útil, mas sem
  qualquer garantia; sem mesmo a garantia implícita de comercialização
  ou adequação a um propósito específico.

  Modificações por Lucas Emanuel B. Alves:
    - Adaptada para ESP32
    - Controle de velocidade sem necessidade de função loop separada
    - Outros ajustes para compatibilidade com o hardware
*/
#ifndef VarSpeedServoESP32_h
#define VarSpeedServoESP32_h

#include <Arduino.h>

class VarSpeedServoESP32 {
public:
  VarSpeedServoESP32();

  uint8_t attach(int pin);

  void write(int value);                           // movimento instantâneo
  void write(int value, uint8_t speed);           // movimento com velocidade controlada
  void write(int value, uint8_t speed, bool wait); // movimento com velocidade controlado e com espera opcional

  void slowmove(int value, uint8_t speed, bool waitFlag = false);  //igual o write, porem n espera

  void wait();                                     // bloqueia até finalizar o movimento
  int read();                                      // posição atual

  bool isMoving();  //retorna se o servo ainda está se movendo
  void moveSequence(int* positions, int size, uint8_t speed, bool waitFlag = true); //move o servo por várias posições em sequência
  void detach();  //desliga o servo e libera o pino
  bool attached(); //desliga o servo e libera o pino

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

  // Task handle para movimento assíncrono
  TaskHandle_t taskHandle;

  // Função que roda na task FreeRTOS
  static void servoTask(void* param);
};

#endif