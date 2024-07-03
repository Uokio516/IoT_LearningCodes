#include <WiFi.h>

// 請修改以下參數--------------------------------------------
char ssid[] = "C220MIS";
char password[] = "misc220c220";

int Gled = 5; // 宣告綠色LED在 GPIO 15
int Yled = 4; // 宣告黃色LED在 GPIO 2
int Rled = 15; // 宣告紅色LED在 GPIO 4
int FAN = 25; // 宣告USB風扇在 GPIO 14

WiFiServer server(80); // 宣告伺服器位在80 port

void setup()
{
  Serial.begin(115200);
  Serial.print("開始連線到無線網路SSID:");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("連線完成");
  server.begin();
  Serial.print("伺服器已啟動，http://");
  Serial.println(WiFi.localIP());
  pinMode(Gled, OUTPUT);
  pinMode(Yled, OUTPUT);
  pinMode(Rled, OUTPUT);
  pinMode(FAN, OUTPUT);
}

void loop()
{
  // 宣告一個連線
  WiFiClient client = server.available();
  if (client) {
    // 有人連入時
    Serial.println("使用者連入");
    //-------------網頁的html部份開始--------------
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("");
    client.println("<!DOCTYPE HTML>");
    client.println("<html><head><meta charset='utf-8'>");
    client.println("<style>");
    client.println("body { font-family: Arial, sans-serif; text-align: center; background-color: #f0f0f0; }");
    client.println("h1 { color: #333; }");
    client.println("a { display: inline-block; margin: 10px; padding: 10px 20px; color: white; text-decoration: none; background-color: #007BFF; border-radius: 5px; }");
    client.println("a:hover { background-color: #0056b3; }");
    client.println("</style>");
    client.println("<script>");
    client.println("function sendRequest(url) { fetch(url); }");
    client.println("</script>");
    client.println("</head>");
    client.println("<body>");
    client.println("<h1>ESP32 Web Server</h1>");
    // HTML超連結指令
    client.println("<a href='javascript:void(0)' onclick=\"sendRequest('/Gled=ON')\">開啟綠色LED</a>");
    client.println("<a href='javascript:void(0)' onclick=\"sendRequest('/Gled=OFF')\">關閉綠色LED</a>");
    client.println("<a href='javascript:void(0)' onclick=\"sendRequest('/Yled=ON')\">開啟黃色LED</a>");
    client.println("<a href='javascript:void(0)' onclick=\"sendRequest('/Yled=OFF')\">關閉黃色LED</a>");
    client.println("<a href='javascript:void(0)' onclick=\"sendRequest('/Rled=ON')\">開啟紅色LED</a>");
    client.println("<a href='javascript:void(0)' onclick=\"sendRequest('/Rled=OFF')\">關閉紅色LED</a>");
    client.println("<a href='javascript:void(0)' onclick=\"sendRequest('/FAN=ON')\">開啟USB風扇</a>");
    client.println("<a href='javascript:void(0)' onclick=\"sendRequest('/FAN=OFF')\">關閉USB風扇</a>");
    client.println("</body>");
    client.println("</html>");
    //-------------網頁的html部份結束--------------
    // 取得使用者輸入的網址
    String request = client.readStringUntil('\r');
    Serial.println(request);
    // 判斷超連結指令
    // 網址內包含Gled=ON，就開啟綠燈，如果Gled=OFF，關閉綠燈
    if (request.indexOf("Gled=ON") >= 0) { digitalWrite(Gled, HIGH); }
    if (request.indexOf("Gled=OFF") >= 0) { digitalWrite(Gled, LOW); }
    if (request.indexOf("Yled=ON") >= 0) { digitalWrite(Yled, HIGH); }
    if (request.indexOf("Yled=OFF") >= 0) { digitalWrite(Yled, LOW); }
    if (request.indexOf("Rled=ON") >= 0) { digitalWrite(Rled, HIGH); }
    if (request.indexOf("Rled=OFF") >= 0) { digitalWrite(Rled, LOW); }
    if (request.indexOf("FAN=ON") >= 0) { digitalWrite(FAN, HIGH); }
    if (request.indexOf("FAN=OFF") >= 0) { digitalWrite(FAN, LOW); }

    Serial.println("完成");
    client.stop(); // 停止連線
  }
}
