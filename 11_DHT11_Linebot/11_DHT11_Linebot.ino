#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <SimpleDHT.h>

//請修改以下參數--------------------------------------------
char ssid[] = "Uokio";
char password[] = "00000000";
String Linetoken = "5Rou4Me1xG1iNM7V7gd4E4CooOCHRSxdGhV9OQfCPfb";
int pinDHT11 = 26;//假設DHT11 接在腳位GPIO4，DHT11 接線圖請參考P74
//---------------------------------------------------------
SimpleDHT11 dht11(pinDHT11);
WiFiClientSecure client;
char host[] = "notify-api.line.me";

void setup() {
  Serial.begin(115200);
  //連線到指定的WiFi SSID
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  //連線成功，顯示取得的IP
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);
  client.setInsecure(); // 讀者若使用ESP32 1.0.6 版核心須加上這句避免SSL 問題
}

void loop() {

  //嘗試讀取溫濕度內容
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err="); Serial.println(err); delay(1000);
    return;
  }
  //讀取成功，將溫濕度顯示在序列視窗
  Serial.print("Sample OK: ");
  Serial.print((int)temperature); Serial.print(" *C, ");
  Serial.print((int)humidity); Serial.println(" H");

  //設定觸發LINE訊息條件為溫度超過35或濕度超過80
  if ((int)temperature >= 35 || (int)humidity >= 80) {
    //傳遞Line訊息
    String message = "檢測環境發生異常，請協助儘速派人查看處理，目前環境狀態：";
    message += "\n溫度=" + String(temperature) + " *C";
    message += "\n濕度=" + String(humidity) + " H";
    Serial.println(message);
    //連線到Line API網站
    if (client.connect(host, 443)) {      
      int LEN = message.length();
      //1.傳遞網站
      String url = "https://notify-api.line.me/api/notify"; //Line API網址
      client.println("POST " + url + " HTTP/1.1");
      client.print("Host: "); client.println(host);//Line API網站      
      //2.資料表頭
      client.print("Authorization: Bearer "); client.println(Linetoken);
      //3.內容格式
      client.println("Content-Type: application/x-www-form-urlencoded");
      //4.資料內容
      client.print("Content-Length: "); client.println( String((LEN + 8)) ); //訊息長度      
      client.println();      
      client.print("message="); client.println(message); //訊息內容      
      //等候回應
      delay(2000);
      String response = client.readString();
      //顯示傳遞結果
      Serial.println(response);
      client.stop();//斷線，否則只能傳5次
    }
    else {
      //傳送失敗
      Serial.println("connected fail");
    }
  }
  //每5秒讀取一次溫濕度
  delay(5000);
}
