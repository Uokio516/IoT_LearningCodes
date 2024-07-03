#include <WiFi.h>
//請修改以下參數--------------------------------------------
char ssid[] = "C220MIS";
char password[] = "misc220c220";

int Gled = 5; //宣告綠色Led在 GPIO 15
int Yled = 4; //宣告黃色Led在 GPIO 2
int Rled = 15; //宣告紅色Led在 GPIO 4
WiFiServer server(80); //宣告伺服器位在80 port

int tryCount = 0;

void setup()
{
  Serial.begin(115200);
  Serial.print("開始連線到無線網路SSID:");
  Serial.println(ssid);
  //無法連線時esp重開
  while(WiFi.status() != WL_CONNECTED){
    Serial.println(tryCount);
    if(tryCount++ >= 20)ESP.restart();
    delay(1000);
  }

  WiFi.begin(ssid, password);
  
  Serial.println("連線完成");
  server.begin();
  Serial.print("伺服器已啟動，http://");
  Serial.println(WiFi.localIP());
  pinMode(Gled, OUTPUT);
  pinMode(Yled, OUTPUT);
  pinMode(Rled, OUTPUT);
}

void loop()
{
  //宣告一個連線
  WiFiClient client = server.available();
  if (client) {
    //有人連入時
    Serial.println("使用者連入");
    //-------------網頁的html部份開始--------------
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("");
    client.println("<!DOCTYPE HTML>");
    client.println("<html><head><meta charset='utf-8'></head>");
    client.println("<br>");
    client.println("<h1>ESP32 Web Server</h1>");
    //HTML超連結指令
    client.println("<a href='/Gled=ON'>開啟綠色LED</a><br>"); // a href 超連結
    client.println("<a href='/Gled=OFF'>關閉綠色LED</a><br>");
    client.println("<a href='/Yled=ON'>開啟黃色LED</a><br>");
    client.println("<a href='/Yled=OFF'>關閉黃色LED</a><br>");
    client.println("<a href='/Rled=ON'>開啟紅色LED</a><br>");
    client.println("<a href='/Rled=OFF'>關閉紅色LED</a><br>");

    client.println("</html>");
    //-------------網頁的html部份結束--------------
    //取得使用者輸入的網址
    String request = client.readStringUntil('\r');
    Serial.println(request);
    //判斷超連結指令
    //網址內包含Gled=ON，就開啟綠燈，如果Gled=OFF，關閉綠燈
    if (request.indexOf("Gled=ON") >= 0) { digitalWrite(Gled, HIGH); }
    if (request.indexOf("Gled=OFF") >= 0) { digitalWrite(Gled, LOW); }
    if (request.indexOf("Yled=ON") >= 0) { digitalWrite(Yled, HIGH); }
    if (request.indexOf("Yled=OFF") >= 0) { digitalWrite(Yled, LOW); }
    if (request.indexOf("Rled=ON") >= 0) { digitalWrite(Rled, HIGH); }
    if (request.indexOf("Rled=OFF") >= 0) { digitalWrite(Rled, LOW); }

    Serial.println("完成");
    client.stop();//停止連線
  }
}
