# VarSpeedServoESP32

VarSpeedServoESP32 é uma adaptação da biblioteca VarSpeedServo de Michael Margolis (2009), feita por Lucas Emanuel B. Alves (2026), agora compatível com ESP32.  
Permite controlar servos com velocidade variável, mantendo a simplicidade e funcionalidades da versão original, mas adaptada para os timers do ESP32.

---

# Funcionalidades

- Controle de posição do servo com velocidade variável:

```cpp
write(pos, speed);
write(pos, speed, wait); // opcional: espera o movimento terminar

```
- Funções básicas de servo:
  - attach(), detach()
  - read(), readMicroseconds()
  - writeMicroseconds()
  - attached()
  - stop()
  - wait()
- Compatibilidade com slowmove() da versão original.
- Projetada para ESP32, usando timers compatíveis.
  
  Observação: Sequências (sequencePlay(), sequenceStop()) e isMoving() não estão implementadas nesta versão.


### Semelhanças: 
  - Funções básicas de controle de servo: attach(), detach(), read(), readMicroseconds(), writeMicroseconds(), attached(), stop() e wait().
  - Função slowmove() mantida para compatibilidade.
  - Permite controlar posição com velocidade variável.
  - Funções de sequências e isMoving() implementadas.

    
### Diferenças:
  - Compatível com ESP32.
  - Adaptação dos timers para funcionar no ESP32, diferente do Arduino.


# Exemplo Simples

```cpp
#include <VarSpeedServoESP32.h>


VarSpeedServoESP32 servo;


void setup() {
  servo.attach(18); // porta 18 do ESP32
}


void loop() {
  servo.write(0, 60, true);   // move para 0° com velocidade 60% 
              // espera terminar
  delay(500);
  servo.write(180, 90, true); // move para 180° com velocidade 90%
             // espera terminar
  delay(500);
}

```
# Instalação

1. Baixe o arquivo .zip da biblioteca no GitHub.
2. No Arduino IDE, vá em Sketch > Include Library > Add .ZIP Library… e selecione o arquivo.
3. A biblioteca será instalada em Documentos/Arduino/libraries.

  Ou instale manualmente descompactando o .zip na pasta de libraries do Arduino.

# Créditos e Licença

- Autor Original: Michael Margolis (2009) – VarSpeedServo original
- Adaptação para ESP32: Lucas Emanuel B. Alves
- Mantém licença LGPL 2.1, garantindo uso, modificação e distribuição livre da biblioteca.

