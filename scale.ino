#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino.h>
#include <string>
// https://github.com/bogde/HX711
#include "HX711.h"

// Wi-Fi関係
#define WIFI_SSID   "xxxxxxx"
#define WIFI_PASS   "xxxxxxx"

const char URL[] = "xxxxxxx";

const int DT_PIN = 2;
const int SCK_PIN = 4;

HX711 Scale;

// プロトタイプ宣言
void connectWiFi(char* ssid, char* passwd);
void getWebLink(char*);
// セットアップ
void setup() {
  // ボーレート設定
  Serial.begin(115200);
  // Wi-Fi接続
  connectWiFi(WIFI_SSID, WIFI_PASS);
  // 必要があれば（機種判別とか）
  // uint64_t chipid = ESP.getEfuseMac(); //  Macアドレス
  // IPAddress ipAddress = WiFi.localIP();//  ローカルのIPアドレス

  Scale.begin(DT_PIN, SCK_PIN);
 
  //パラメータ設定
  Scale.set_scale(386.3);
  Scale.tare();
}

int gram = 0;


// メイン処理
void loop() {
  // 接続失敗時に再接続
  if ( WiFi.status() == WL_DISCONNECTED) {
    connectWiFi(WIFI_SSID, WIFI_PASS);
  }

  //重量を四捨五入
  gram = Scale.get_units(10) + 0.5;
  Serial.println(gram);
  Scale.power_down();
  delay(500);
  Scale.power_up();

  if(gram >= 500){
  // GET
  getWebLink(URL);
  delay(5000);
  }
}
// --------------------
//  自作関数
// --------------------
// Wi-Fi接続関数
void connectWiFi(char* ssid, char* passwd)
{
    // 接続開始
    WiFi.begin(ssid, passwd);
    Serial.print("WiFi connecting...");
    // 接続成功までループ
    while(WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(100);
    }
    Serial.println(".");
    // 接続完了
    Serial.print("connected : ");
    Serial.println(WiFi.localIP());
}
// GET関数
void getWebLink(const char* API_URL){
  // HTTPのインスタンス作成
  HTTPClient http;
  // GET
  http.begin(API_URL);
  int httpCode = http.GET();
  // 結果表示
  Serial.printf("Response: %d", httpCode);
  Serial.println();
  if (httpCode == HTTP_CODE_OK) {
    String body = http.getString();
    Serial.print("Response Body: ");
    Serial.println(body);
  }
}
