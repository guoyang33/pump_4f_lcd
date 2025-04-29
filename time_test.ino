#include <TimeLib.h>

// Constants
//  Pins
//    Output: LED 燈號模組
const int LED_PIN               = 10;

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println(millis());

  delay(500);
  // put your main code here, to run repeatedly:

}
