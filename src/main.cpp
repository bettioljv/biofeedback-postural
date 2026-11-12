/*
  TCC - Sistema de Biofeedback Postural
  Autor: João Vitor Bettiol

  Versão inicial para o protótipo físico:
  - ESP32 + MPU6050;
  - leitura da inclinação do tronco;
  - calibração da postura neutra ao iniciar;
  - alerta sonoro quando a inclinação relativa ultrapassa 30 graus.

  Conexões previstas:
  MPU6050 -> ESP32
  VCC     -> 3V3
  GND     -> GND
  SDA     -> GPIO 21
  SCL     -> GPIO 22

  Buzzer -> GPIO 25 e GND
*/

#include <Arduino.h>
#include <Wire.h>

const byte MPU6050_ADDR = 0x68;

const int PIN_BUZZER = 25;
const int PIN_SDA = 21;
const int PIN_SCL = 22;

const float LIMITE_INCLINACAO = 30.0;
const int AMOSTRAS_CALIBRACAO = 100;

float anguloReferencia = 0.0;

// Escreve um valor em um registrador do MPU6050.
void escreverMPU(byte registrador, byte valor) {
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(registrador);
  Wire.write(valor);
  Wire.endTransmission();
}

// Realiza a leitura dos eixos do acelerômetro.
bool lerAcelerometro(float &ax, float &ay, float &az) {
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x3B); // Primeiro registrador do acelerômetro.
  Wire.endTransmission(false);

  if (Wire.requestFrom(MPU6050_ADDR, (byte)6) != 6) {
    return false;
  }

  int16_t axRaw = (Wire.read() << 8) | Wire.read();
  int16_t ayRaw = (Wire.read() << 8) | Wire.read();
  int16_t azRaw = (Wire.read() << 8) | Wire.read();

  // Faixa padrão: ±2 g.
  ax = axRaw / 16384.0;
  ay = ayRaw / 16384.0;
  az = azRaw / 16384.0;

  return true;
}

// Calcula a inclinação considerando a orientação inicial do sensor.
float calcularAngulo(float ax, float ay, float az) {
  return atan2(ay, sqrt(ax * ax + az * az)) * 180.0 / PI;
}

// Define a postura atual como referência neutra.
void calibrarPosturaNeutra() {
  float somaAngulos = 0.0;
  int leiturasValidas = 0;

  Serial.println();
  Serial.println("Calibracao iniciada.");
  Serial.println("Mantenha o sensor parado na postura neutra.");

  for (int i = 0; i < AMOSTRAS_CALIBRACAO; i++) {
    float ax, ay, az;

    if (lerAcelerometro(ax, ay, az)) {
      somaAngulos += calcularAngulo(ax, ay, az);
      leiturasValidas++;
    }

    delay(20);
  }

  if (leiturasValidas > 0) {
    anguloReferencia = somaAngulos / leiturasValidas;

    Serial.print("Calibracao concluida. Angulo de referencia: ");
    Serial.print(anguloReferencia, 2);
    Serial.println(" graus.");
  } else {
    Serial.println("ERRO: nao foi possivel calibrar o MPU6050.");
  }
}

void setup() {
  Serial.begin(115200);

  Wire.begin(PIN_SDA, PIN_SCL);
  pinMode(PIN_BUZZER, OUTPUT);
  noTone(PIN_BUZZER);

  // Desativa o modo de repouso do MPU6050.
  escreverMPU(0x6B, 0x00);

  Serial.println("Sistema de biofeedback postural iniciado.");
  delay(1000);

  calibrarPosturaNeutra();
}

void loop() {
  float ax, ay, az;

  if (!lerAcelerometro(ax, ay, az)) {
    Serial.println("ERRO: MPU6050 nao respondeu.");
    noTone(PIN_BUZZER);
    delay(500);
    return;
  }

  float anguloAtual = calcularAngulo(ax, ay, az);
  float inclinacaoRelativa = anguloAtual - anguloReferencia;

  bool alerta = fabs(inclinacaoRelativa) > LIMITE_INCLINACAO;

  if (alerta) {
    tone(PIN_BUZZER, 1200);
  } else {
    noTone(PIN_BUZZER);
  }

  Serial.println("------------------------------");
  Serial.print("Inclinacao relativa: ");
  Serial.print(inclinacaoRelativa, 1);
  Serial.println(" graus");

  Serial.print("Estado: ");
  Serial.println(alerta
    ? "ALERTA: inclinacao excessiva"
    : "POSTURA ADEQUADA");

  delay(200);
}