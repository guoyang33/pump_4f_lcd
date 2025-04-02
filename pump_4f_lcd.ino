#include <LiquidCrystal_I2C.h>

#include <TimeLib.h>

/* LCD */
/* set the LCD address */
LiquidCrystal_I2C lcd(0x27, 16, 2);

/* Pin */
const int LED_PIN = 10;
const int BUTTON_PIN = 11;
const int ROTATION_SENSOR = A0;
const int PUMP_RELAY = 9;

const int timerInitial    = 16200000;   // 4:30:00  = 4*60*60*1000 + 30*60*1000 = 16200000
const int timerMax        = 32400000;   // 9:00:00  = 9 * 60 * 60 * 1000        = 32400000
const int timerMin        = 1800000;    // 0:30:00  = 30 * 60 * 1000            = 1800000
const int timerDelta      = 1800000;    // 0:30:00  = 30 * 60 * 1000            = 1800000
const float rotationMid   = 0.5;        // Half     = 1 / 2                     = 0.5
const float rotationStep  = 0.05;       // 20 Steps = 1 / 20                    = 0.05

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
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("HOLA PUMP 4F LCD");
  String str = "";
  if (isButtonDown()) {
    str += "B:D R:";
  } else {
    str += "B:U R:";
  }
  str += rotationRead();
  Serial.println(str);
  lcd.setCursor(0, 1);
  lcd.print(String(str));

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
