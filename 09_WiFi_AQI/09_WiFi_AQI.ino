#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

char ssid[] = "沒看過帥哥嗎？";
char password [] = "12345678";
char url[] = "https://data.moenv.gov.tw/api/v2/aqx_p_432?api_key=e8dd42e6-9b8b-43f8-991e-b3dee723a52d&limit=1000&sort=ImportDate%20desc&format=JSON";


int RGB_R = 4; //PIN
int RGB_G = 2;
int RGB_B = 15;

void setup() {
  Serial.begin(115200);
  pinMode(RGB_R, OUTPUT);
  pinMode(RGB_G, OUTPUT);
  pinMode(RGB_B, OUTPUT);
  delay(1000);
  Serial.print("開始連接");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  int tryCount = 0;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    if (tryCount++ >= 20) {
      ESP.restart();
    }
  }
  Serial.println("連線完成");
}

void loop() {
  Serial.println("啟動網頁連線");
  HTTPClient http;
  http.begin(url);
  int httpCode = http.GET();
  Serial.print("httpCode=");
  Serial.println(httpCode);

  int AQIvalue = -1;

  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    Serial.print("payload");
    Serial.println(payload);

    DynamicJsonDocument AQI(payload.length() * 2);
    deserializeJson(AQI, payload);

    for (int i = 0; i <= AQI["records"].size(); i++) {
      if (AQI["records"][i]["sitename"] == "基隆") {
        String AQIvalue = AQI["records"][i]["aqi"];
        Serial.println("基隆AQI=" + AQIvalue);
        break;
      }
    }
  }

  if (AQIvalue <= 50 && AQIvalue >= 0) {
    analogWrite(RGB_R, 0);
    analogWrite(RGB_G, 255);
    analogWrite(RGB_B, 0);
  }
  else if (AQIvalue <= 100 && AQIvalue >= 51) {
    analogWrite(RGB_R, 255);
    analogWrite(RGB_G, 255);
    analogWrite(RGB_B, 0);
  }
  else if (AQIvalue <= 150 && AQIvalue >= 101) {
    analogWrite(RGB_R, 255);
    analogWrite(RGB_G, 100);
    analogWrite(RGB_B, 0);
  }
  else if (AQIvalue <= 200 && AQIvalue >= 151) {
    analogWrite(RGB_R, 255);
    analogWrite(RGB_G, 0);
    analogWrite(RGB_B, 0);
  }
  else if (AQIvalue <= 300 && AQIvalue >= 201) {
    analogWrite(RGB_R, 160);
    analogWrite(RGB_G, 38);
    analogWrite(RGB_B, 0);
  }
  else if (AQIvalue <= 500 && AQIvalue >= 301) {
    analogWrite(RGB_R, 0);
    analogWrite(RGB_G, 0);
    analogWrite(RGB_B, 0);
  }
  else {
    analogWrite(RGB_R, 0);
    analogWrite(RGB_G, 0);
    analogWrite(RGB_B, 0);
  }


  http.end();
  delay(10000);
}
