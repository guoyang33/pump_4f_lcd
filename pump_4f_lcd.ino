#include <TimeLib.h>

#include <Bonezegei_LCD1602_I2C.h>

const int timerInitial = (4 * 60 + 30) * 60 * 1000;   // 4:30:00
const int timerMax = 9 * 60 * 60 * 1000;              // 9:00:00
const int timerMin = 30 * 60 * 1000;                  // 0:30:00
const int timerDelta = 30 * 60 * 1000;                // 0:30:00
const float rotationMid = 1 / 2;                      // Half
const float rotationStep = 1 / 20;                    // 20 Steps

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
