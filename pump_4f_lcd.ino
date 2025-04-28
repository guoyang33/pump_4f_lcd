/*
 * 一心四樓澆灌系統程式
 * 
 * changelog:
 *  2025-04-14
 *    取消使用可調泵浦啟用時間，因為只有一個旋鈕，所以會需要使用記憶體來儲存該設定值，結果就是會在不穩定的電源供應下自動回朔，即不實用功能。
 *    之後若有需要對砅浦啟用時間進行調整得在程式碼中設定再燒錄進 Arduino。
 */
#include <LiquidCrystal_I2C.h>

/* LCD */
/* set the LCD address */
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Constants
//  Pins
//    Output: LED 燈號模組
const int LED_PIN               = 10;
//    Input: 鈕扣按鈕模組
const int BUTTON_PIN            = 11;
//    Input: 旋鈕 轉度感測模組
const int ROTATION_SENSOR_PIN   = A0;
//    Output: 抽水泵浦 繼電器模組
const int PUMP_RELAY_PIN        = 9;

//  Variables
//    轉度感測模組透過類比訊號接收到的最大值
const int ROTATION_SENSOR_MAX = 1023;
//    單位轉度
const float ROTATION_STEP = 0.05;
//    預設中間轉度
const float ROTATION_MID = 0.5;
//    預設間隔時間
const int INTERVAL_DEFAULT = 16200000;
//    間隔時間最大值
const int INTERVAL_MAX = 32400000;
//    間隔時間最小值
const int INTERVAL_MIN = 1800000;
//    間隔時間單位轉度變化量
const int INTERVAL_DELTA = 1800000;

const int intervalDefault = 16200000;
const int intervalMax = 32400000;
const int intervalMin = 1800000;
const int timerDefault    = 16200000;             // 4:30:00  = (4 * 60 + 30) * 60 * 1000     = 16200000
const int timerMax        = 32400000;             // 9:00:00  = 9 * 60 * 60 * 1000            = 32400000
const int timerMin        = 1800000;              // 0:30:00  = 30 * 60 * 1000                = 1800000
const int timerDelta      = 1800000;              // 0:30:00  = 30 * 60 * 1000                = 1800000
const float rotationMid   = 0.5;                  // Half     = 1 / 2                         = 0.5
const float rotationStep  = 0.05;                 // 20 Steps = 1 / 20                        = 0.05
const int pumpDuration    = 1800000;              // 0:30:00  = 30 * 60 * 1000                = 1800000
const int initDuration    = 3000;                 // 0:00:03  = 3 * 1000                      = 3000

unsigned long previousMillis = 0;

bool pumpIsOn = false;
int pumpInterval;

bool is_button_down() {
  /*
   * Return true if button is down
   * otherwise return false
   */
  if (digitalRead(BUTTON_PIN) == LOW) {
    return true;
  }
  return false;
}

const int INIT_CODE_ERROR         = 0;
const int INIT_CODE_OK            = 1;
const int INIT_CODE_BUTTON_ERROR  = 2;
int main_init() {
  
  return INIT_CODE_ERROR;
}

int rotation_read() {
  return analogRead(ROTATION_SENSOR_PIN);
}

void lcd_clear() {}

void led_on() {
  digitalWrite(LED_PIN, HIGH);
}
void led_off() {
  digitalWrite(LED_PIN, LOW);
}

void pump_on() {
  if (!pumpIsOn) {
    pumpIsOn = true;
    led_on();
    digitalWrite(PUMP_RELAY_PIN, HIGH);
  }
}
void pump_off() {
  if (pumpIsOn) {
    pumpIsOn = false;
    led_off();
    digitalWrite(PUMP_RELAY_PIN, LOW); 
  }
}

int calc_rotation_standardize() {
  return rotation_read() / ROTATION_SENSOR_MAX;
}

int calc_rotation_step() {
  return (calc_rotation_standardize() / ROTATION_STEP) - ROTATION_MID;
}

int calc_pump_interval() {
  return INTERVAL_DEFAULT + (calc_rotation_step() * INTERVAL_DELTA);
}

void lcd_print_setting() {
  lcd.setCursor(8, 0);
  lcd.print(pumpInterval
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  pinMode(ROTATION_SENSOR_PIN, INPUT);
  pinMode(PUMP_RELAY_PIN, OUTPUT);

  Serial.begin(9600);
 
  lcd.init();
  lcd.backlight();
  lcd.setCursor(4, 0);
  lcd.print("SET=");
  lcd.setCursor(4, 1);
  lcd.print("ETA=");

  pumpInterval = INTERVAL_DEFAULT;
}

void loop() {
  unsigned long currentMillis = millis();
  if (pumpIsOn) {
    if (currentMillis - previousMillis >= pumpDuration) {
      pump_off();
      previousMillis = millis();
    }
    if (is_button_down()) {
      while (is_button_down()) {}
      pump_off();
      previousMillis = millis();
    }
  } else {
    if (currentMillis - previousMillis >= pumpInterval) {
      pump_on();
      previousMillis = millis();
    }
    if (is_button_down()) {
      while (is_button_down()) {}
      pump_on();
      previousMillis = millis();
    }
  }
  pumpInterval = calc_pump_interval();
  
}
