void setup() {
  Serial.begin(115200);
  pinMode(35,INPUT);
}

void loop() {
  int value = analogRead(35);
  Serial.println(value);
}
