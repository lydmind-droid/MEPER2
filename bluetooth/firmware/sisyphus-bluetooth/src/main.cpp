#include <Arduino.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

//--------------------------------------------- (PINS) ------------
#define RENA 25
#define RIN1 26
#define RIN2 27
#define RIN3 32
#define RIN4 33
#define RENB 14

//--------------------------------------------- (VARIABLES) --------
int speedLinear = 150;
int speedTurn = 150;

//--------------------------------------------- (MOTOR FUNCTIONS) ---
void forward() {
  digitalWrite(RIN1, HIGH);
  digitalWrite(RIN2, LOW);
  analogWrite(RENA, speedLinear);
  digitalWrite(RIN3, HIGH);
  digitalWrite(RIN4, LOW);
  analogWrite(RENB, speedLinear);
}

void backward() {
  digitalWrite(RIN1, LOW);
  digitalWrite(RIN2, HIGH);
  analogWrite(RENA, speedLinear);
  digitalWrite(RIN3, LOW);
  digitalWrite(RIN4, HIGH);
  analogWrite(RENB, speedLinear);
}

void right() {
  digitalWrite(RIN1, LOW);
  digitalWrite(RIN2, HIGH);
  analogWrite(RENA, speedTurn);
  digitalWrite(RIN3, HIGH);
  digitalWrite(RIN4, LOW);
  analogWrite(RENB, speedTurn);
}

void left() {
  digitalWrite(RIN1, HIGH);
  digitalWrite(RIN2, LOW);
  analogWrite(RENA, speedTurn);
  digitalWrite(RIN3, LOW);
  digitalWrite(RIN4, HIGH);
  analogWrite(RENB, speedTurn);
}

void stopMotors() {
  digitalWrite(RIN1, LOW);
  digitalWrite(RIN2, LOW);
  digitalWrite(RIN3, LOW);
  digitalWrite(RIN4, LOW);
  analogWrite(RENA, 0);
  analogWrite(RENB, 0);
}

//--------------------------------------------- (COMMAND PARSER) ---
void parseCommand(String cmd) {
  cmd.trim();
  if (cmd == "forward") { forward(); Serial.println("CMD: forward"); }
  else if (cmd == "backward") { backward(); Serial.println("CMD: backward"); }
  else if (cmd == "left") { left(); Serial.println("CMD: left"); }
  else if (cmd == "right") { right(); Serial.println("CMD: right"); }
  else if (cmd == "stop") { stopMotors(); Serial.println("CMD: stop"); }
  else if (cmd.startsWith("speed:")) {
    speedLinear = cmd.substring(6).toInt();
    speedLinear = constrain(speedLinear, 0, 255);
    Serial.print("Speed Linear: "); Serial.println(speedLinear);
  }
  else if (cmd.startsWith("turnspeed:")) {
    speedTurn = cmd.substring(10).toInt();
    speedTurn = constrain(speedTurn, 0, 255);
    Serial.print("Speed Turn: "); Serial.println(speedTurn);
  }
}

//--------------------------------------------- (SETUP) ------------
void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(RENA, OUTPUT);
  pinMode(RIN1, OUTPUT);
  pinMode(RIN2, OUTPUT);
  pinMode(RIN3, OUTPUT);
  pinMode(RIN4, OUTPUT);
  pinMode(RENB, OUTPUT);

  stopMotors();

  // Bluetooth
  SerialBT.begin("Sisyphus");
  Serial.println("=== MEPER - Sisyphus (Bluetooth) ===");
  Serial.println("Bluetooth disponivel como 'Sisyphus'");
}

//--------------------------------------------- (MAIN LOOP) --------
void loop() {
  if (SerialBT.available()) {
    String cmd = SerialBT.readStringUntil('\n');
    parseCommand(cmd);
  }
  delay(10);
}
