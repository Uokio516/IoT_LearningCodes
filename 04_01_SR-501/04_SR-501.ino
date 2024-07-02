int LED_Red = 19,LED_Yello = 16,LED_Green = 15;

void setup() {
  Serial.begin(115200);
  pinMode(17, INPUT); //SR-501
  pinMode(LED_Yello, OUTPUT); //yello
  pinMode(LED_Red, OUTPUT); //red
  pinMode(LED_Green, OUTPUT); //green
}
     
void loop() {
  int value = digitalRead(17);
  if (value == HIGH) {
    Serial.println("有人經過");
    digitalWrite(LED_Red, HIGH);
    digitalWrite(LED_Green, LOW);
  }
  else {
    Serial.println("無人");
    digitalWrite(LED_Green, HIGH);
    digitalWrite(LED_Red, LOW);
  }
}
