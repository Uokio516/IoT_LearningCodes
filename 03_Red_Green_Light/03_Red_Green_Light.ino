void setup() {
  pinMode(15,OUTPUT);
  pinMode(17,OUTPUT);
  pinMode(19,OUTPUT);
}

void loop() {
  digitalWrite(15,HIGH);
  delay(3000);
  digitalWrite(15,LOW);
  digitalWrite(17,HIGH);
  delay(1000);
  digitalWrite(17,LOW);
  digitalWrite(19,HIGH);
  delay(3000);
  digitalWrite(19,LOW);
}
