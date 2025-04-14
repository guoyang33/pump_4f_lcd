/*
 * 一心四樓澆灌系統程式
 * 
 * changelog:
 *  2025-04-14
 *    取消使用可調泵浦啟用時間，因為只有一個旋鈕，所以會需要使用記憶體來儲存該設定值，結果就是會在不穩定的電源供應下自動回朔，即不實用功能。
 *    之後若有需要對砅浦啟用時間進行調整得在程式碼中設定再燒錄進 Arduino。
 *    
 *    在初始化階段，若按鈕為下壓狀態，要等待一個時間(initButtonDownErrorDuration)，先設為 5 分鐘，過了就視為非人為不當操作，並在當次運行中不再監聽按鈕事件。
 */
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

const int timerDefault    = 16200000;   // 4:30:00  = 4*60*60*1000 + 30*60*1000 = 16200000
const int timerMax        = 32400000;   // 9:00:00  = 9 * 60 * 60 * 1000        = 32400000
const int timerMin        = 1800000;    // 0:30:00  = 30 * 60 * 1000            = 1800000
const int timerDelta      = 1800000;    // 0:30:00  = 30 * 60 * 1000            = 1800000
const float rotationMid   = 0.5;        // Half     = 1 / 2                     = 0.5
const float rotationStep  = 0.05;       // 20 Steps = 1 / 20                    = 0.05
const int pumpDuration    = 1800000;    // 0:30:00  = 30 * 60 * 1000            = 1800000
const int initDuration    = 3000;       // 0:00:03  = 3 * 1000                  = 3000
const int initButtonDownErrorDuration = 300000; // 0:05:00 = 5 * 60 * 1000      = 300000

bool is_button_down() {
  /*
   * Return true if button is down
   * otherwise return false
   */
  if (digitalRead(BUTTON_PIN)) {
    return false;
  }
  return true;
}

int rotation_read() {
  return analogRead(ROTATION_SENSOR);
}

void lcd_clear() {}

void led_on() {
  digitalWrite(LED_PIN, HIGH);
}
void led_off() {
  digitalWrite(LED_PIN, LOW);
}

void pump_on() {
  digitalWrite(PUMP_RELAY, HIGH);
}
void pump_off() {
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
  
  delay(initDuration);

  while (is_button_down()) {
    if (millis() >= initDuration + initButtonDownErrorDuration) {
      
    }
  }

  /* LED: TEST OFF */
  ledOff();
  /* PUMP RELAY: TEST OFF */
  pumpOff();
  /* LCD: Clear output */
  lcdClear();
}

void loop() {
  /* 計時器 */
  int timer = 0;
  
}
