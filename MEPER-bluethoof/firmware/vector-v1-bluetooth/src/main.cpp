#include <Arduino.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

//--------------------------------------------- (PINS) ------------
//--------------- (L298n R)
#define RENA 25
#define RIN1 26
#define RIN2 27
#define RIN3 32
#define RIN4 33
#define RENB 14

//--------------- (L298n L)
#define LENA 12
#define LIN1 13
#define LIN2 15
#define LIN3 4
#define LIN4 16
#define LENB 17

//--------------------------------------------- (VARIABLES) --------
int speed = 150;
int trim = 0;  // -50 a +50: positivo = compensa lado direito (reduz motor R)

// Calcula velocidades com trim aplicado
int speedR() { return constrain(speed - trim, 0, 255); }
int speedL() { return constrain(speed + trim, 0, 255); }

//--------------------------------------------- (MOTOR FUNCTIONS) ---
void forward() {
  digitalWrite(RIN2, HIGH); digitalWrite(RIN1, LOW);
  digitalWrite(RIN3, HIGH); digitalWrite(RIN4, LOW);
  analogWrite(RENA, speedR()); analogWrite(RENB, speedR());
  digitalWrite(LIN1, HIGH); digitalWrite(LIN2, LOW);
  digitalWrite(LIN4, HIGH); digitalWrite(LIN3, LOW);
  analogWrite(LENA, speedL()); analogWrite(LENB, speedL());
}

void backward() {
  digitalWrite(RIN2, LOW); digitalWrite(RIN1, HIGH);
  digitalWrite(RIN3, LOW); digitalWrite(RIN4, HIGH);
  analogWrite(RENA, speedR()); analogWrite(RENB, speedR());
  digitalWrite(LIN1, LOW); digitalWrite(LIN2, HIGH);
  digitalWrite(LIN4, LOW); digitalWrite(LIN3, HIGH);
  analogWrite(LENA, speedL()); analogWrite(LENB, speedL());
}

void right() {
  digitalWrite(RIN2, LOW); digitalWrite(RIN1, HIGH);
  digitalWrite(RIN3, LOW); digitalWrite(RIN4, HIGH);
  analogWrite(RENA, speedR()); analogWrite(RENB, speedR());
  digitalWrite(LIN1, HIGH); digitalWrite(LIN2, LOW);
  digitalWrite(LIN4, HIGH); digitalWrite(LIN3, LOW);
  analogWrite(LENA, speedL()); analogWrite(LENB, speedL());
}

void left() {
  digitalWrite(RIN2, HIGH); digitalWrite(RIN1, LOW);
  digitalWrite(RIN3, HIGH); digitalWrite(RIN4, LOW);
  analogWrite(RENA, speedR()); analogWrite(RENB, speedR());
  digitalWrite(LIN1, LOW); digitalWrite(LIN2, HIGH);
  digitalWrite(LIN4, LOW); digitalWrite(LIN3, HIGH);
  analogWrite(LENA, speedL()); analogWrite(LENB, speedL());
}

void stopMotors() {
  digitalWrite(RIN2, LOW); digitalWrite(RIN1, LOW);
  digitalWrite(RIN3, LOW); digitalWrite(RIN4, LOW);
  analogWrite(RENA, 0); analogWrite(RENB, 0);
  digitalWrite(LIN1, LOW); digitalWrite(LIN2, LOW);
  digitalWrite(LIN4, LOW); digitalWrite(LIN3, LOW);
  analogWrite(LENA, 0); analogWrite(LENB, 0);
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
    speed = cmd.substring(6).toInt();
    speed = constrain(speed, 0, 255);
    Serial.print("Speed: "); Serial.println(speed);
  }
  else if (cmd.startsWith("trim:")) {
    trim = cmd.substring(5).toInt();
    trim = constrain(trim, -50, 50);
    Serial.print("Trim: "); Serial.println(trim);
    Serial.print("  Motor R: "); Serial.print(speedR());
    Serial.print(" | Motor L: "); Serial.println(speedL());
  }
}

//--------------------------------------------- (SETUP) ------------
void setup() {
  Serial.begin(115200);
  delay(1000);

  // Motor pins
  int pins[] = {RENA, RIN1, RIN2, RIN3, RIN4, RENB, LENA, LIN1, LIN2, LIN3, LIN4, LENB};
  for (int i = 0; i < 12; i++) pinMode(pins[i], OUTPUT);
  stopMotors();

  // Bluetooth
  SerialBT.begin("Vector V1");
  Serial.println("=== MEPER - Vector V1 (Bluetooth) ===");
  Serial.println("Bluetooth disponivel como 'Vector V1'");
}

//--------------------------------------------- (MAIN LOOP) --------
void loop() {
  if (SerialBT.available()) {
    String cmd = SerialBT.readStringUntil('\n');
    parseCommand(cmd);
  }
  delay(2);
}
