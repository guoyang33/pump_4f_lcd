#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>

/* Pin */
const int LED_PIN = 10;
const int BUTTON_PIN = 11;
const int ROTATION_SENSOR = A0;
const int PUMP_RELAY = 9;

bool isButtonDown() {
  /*
   * Return true if button is down
   * otherwise return false
   */
  if (digitalRead(BUTTON_PIN)) {
    return false;
  }
  return true;
}

int rotationRead() {
  return analogRead(ROTATION_SENSOR);
}

void lcdClear() {}

void ledOn() {
  digitalWrite(LED_PIN, HIGH);
}
void ledOff() {
  digitalWrite(LED_PIN, LOW);
}

void pumpOn() {
  digitalWrite(PUMP_RELAY, HIGH);
}
void pumpOff() {
  digitalWrite(PUMP_RELAY, LOW);
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  pinMode(ROTATION_SENSOR, INPUT);
  pinMode(PUMP_RELAY, OUTPUT);

  Serial.begin(9600);
  
  /* ROTATION SENSOR: READ */
  rotation = analogRead(ROTATION_SENSOR);
  /* LCD: SHOW HELLO and BUTTON and ROTATION SENSOR status */
  /* ... */
  Serial.print("B: ");
  Serial.print(isButtonDown());
  Serial.print("\tR: ");
  Serial.println(rotationRead());
  /* LED: TEST ON */
  ledOn();
  /* PUMP RELAY: TEST ON */
  pumpOn();
  
  delay(3000);
  
  /* LED: TEST OFF */
  ledOff();
  /* PUMP RELAY: TEST OFF */
  pumpOff();
  /* LCD: Clear output */
  lcdClear();
}

void loop() {
  
}
