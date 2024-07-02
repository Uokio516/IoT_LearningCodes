int value = digitalRead(17);
void setup() {
  Serial.begin(115200);
  pinMode(17, INPUT);
  pinMode(16, OUTPUT);
}

void loop() {
  int value = digitalRead(17);
  if (value == HIGH) {
    Serial.println("有人經過");
    digitalWrite(16, HIGH);
    delay(1000);
    digitalWrite(16, LOW);
  }
  else {
    Serial.println("無人");
  }
  delay(500);
}
