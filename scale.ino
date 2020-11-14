#include <Arduino.h>
// https://github.com/bogde/HX711
#include "HX711.h"
 
const int DT_PIN = 2;
const int SCK_PIN = 4;
 
HX711 Scale;
 
void setup() {
  Serial.begin(9600);
  Scale.begin(DT_PIN, SCK_PIN);
 
  //パラメータ設定
  Scale.set_scale(386.2);
  Scale.tare();
}
 
//重量格納変数
int gram = 0;
 
void loop() {
  //重量を四捨五入
  gram = Scale.get_units(10) + 0.5;
 
  //重量表示
  Serial.println(gram);
 
  Scale.power_down();
  delay(500);
  Scale.power_up();
}
