#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

char ssid[] = "沒看過帥哥嗎？";
char password [] = "12345678";
char url[] = "https://data.moenv.gov.tw/api/v2/aqx_p_02?api_key=e8dd42e6-9b8b-43f8-991e-b3dee723a52d&limit=1000&sort=datacreationdate%20desc&format=JSON";

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.print("開始連接");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  int tryCount = 0;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    if(tryCount++ >= 20){
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
  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    Serial.print("payload");
    Serial.println(payload);

    DynamicJsonDocument pm25(payload.length()*2);
    deserializeJson(pm25,payload);

    for(int i=0;i<=pm25["records"].size();i++){
      if(pm25["records"][i]["site"]=="左營"){
        String pm25value = pm25["records"][i]["pm25"];
        Serial.println("左營pm2.5=" + pm25value);
        break;
      }
    }
  }
  http.end();
  delay(10000);
}
