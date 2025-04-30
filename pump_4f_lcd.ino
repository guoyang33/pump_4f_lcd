/*
 * 一心四樓澆灌系統程式
 * 
 * changelog:
 *  2025-04-14
 *    取消使用可調泵浦啟用時間，因為只有一個旋鈕，所以會需要使用記憶體來儲存該設定值，結果就是會在不穩定的電源供應下自動回朔，即不實用功能。
 *    之後若有需要對泵浦啟用時間進行調整得在程式碼中設定再燒錄進 Arduino。
 *  2025-04-30
 *    待改問題：
 *      pumpInterval要跟顯示的 (ETA) 分開計算
 *      ETA 還沒有顯示泵浦的通電剩餘時間
 */
#include <Wire.h> 
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
//    單位轉度              先設有 20 個刻度: 1/20 = 0.05
const float ROTATION_STEP = 0.05;
//    預設中間轉度          先設為正中間，即 1/2 = 0.5
const float ROTATION_MID = 0.5;
//    預設間隔時間          先設為 4:30:00 = ((4.5 * 60) * 60) * 1000 = 16200000
const long INTERVAL_DEFAULT = 16200000;
//    間隔時間最大值         先設為 9:00:00 = ((9 * 60) * 60) * 1000 = 32400000 
const long INTERVAL_MAX = 32400000;
//    間隔時間最小值         先設為 0:30:00 = 30 * 60 * 1000 = 1800000
const long INTERVAL_MIN = 1800000;
//    間隔時間單位轉度變化量  先設為 0:30:00 
const long INTERVAL_DELTA = 1800000;
//   泵浦抽水時間            0:02:00 = 2 * 60 * 1000 = 120000
const long PUMP_DURATION = 120000;

unsigned long previousMillis = 0;
bool pumpIsOn = false;
long pumpInterval;

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

int rotation_read() {
  return analogRead(ROTATION_SENSOR_PIN);
}
float calc_rotation_standardize() {
  Serial.println("call: calc_rotation_standardize");
  Serial.println(rotation_read());
  float rotationNormalize = (float)rotation_read() / ROTATION_SENSOR_MAX;
  Serial.println(rotationNormalize);
  return rotationNormalize;
}
int calc_rotation_step() {
  Serial.println((calc_rotation_standardize() - ROTATION_MID) / ROTATION_STEP);
  return (calc_rotation_standardize() - ROTATION_MID) / ROTATION_STEP;
}
long calc_pump_interval() {
  long pumpInterval = INTERVAL_DEFAULT + (calc_rotation_step() * INTERVAL_DELTA);
  if (pumpInterval > INTERVAL_MAX) {
    return INTERVAL_MAX;
  }
  if (pumpInterval < INTERVAL_MIN) {
    return INTERVAL_MIN;
  }
  return pumpInterval;
}

void lcd_print_time(long t) {
  Serial.println(t);
  int ts = t / 1000;
  int h = ts / 3600;
  int m = (ts % 3600) / 60;
  int s = ts % 60;
  lcd.print(h/10);
  lcd.print(h%10);
  lcd.print(":");
  lcd.print(m/10);
  lcd.print(m%10);
  lcd.print(":");
  lcd.print(s/10);
  lcd.print(s%10);
}
void lcd_print_setting() {
  lcd.setCursor(8, 0);
  lcd_print_time(calc_pump_interval());
}
void lcd_print_eta(long eta) {
  lcd.setCursor(8, 1);
  lcd_print_time(eta);
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  pinMode(ROTATION_SENSOR_PIN, INPUT);
  pinMode(PUMP_RELAY_PIN, OUTPUT);

  Serial.begin(9600);
  Serial.println("setup: Serial initialized");
 
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("SET=");
  lcd.setCursor(4, 1);
  lcd.print("ETA=");

  pumpInterval = calc_pump_interval();
}

void loop() {
  unsigned long currentMillis = millis();
  long duration = currentMillis - previousMillis;
  Serial.println(duration);
  
  if (pumpIsOn) {
    pumpInterval = calc_pump_interval();
    
    if (is_button_down()) {
        while (is_button_down()) {}
        pump_off();
        previousMillis = millis();
    } else {
      if (duration >= PUMP_DURATION) {
        pump_off();
        previousMillis = millis();
      }
    }
    
    lcd_print_eta(PUMP_DURATION - duration);
    
  } else {
    if (is_button_down()) {
      while (is_button_down()) {}
      pump_on();
      Serial.println(pumpInterval);
      previousMillis = millis();
      
    } else {
      if (duration >= pumpInterval) {
        pump_on();
        Serial.println(pumpInterval);
        previousMillis = millis();
      }  
    }
  
    lcd_print_eta(pumpInterval - duration);
  }
  
  lcd_print_setting();
}
