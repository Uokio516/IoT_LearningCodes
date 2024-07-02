void setup() {
  Serial.begin(115200);
  pinMode(36,INPUT);
  pinMode(19,OUTPUT);
  pinMode(16,OUTPUT);
  pinMode(15,OUTPUT);
}

void loop() {
  int value = analogRead (36);
  value = map(value , 0 , 4095 , 0 , 100);
  Serial.println(value);
  delay(100);

  if(value >= 80){
    digitalWrite(19,HIGH);
    digitalWrite(16,HIGH);
    digitalWrite(15,HIGH);
  }
  else if (value >= 60){
    digitalWrite(19,HIGH);
    digitalWrite(16,HIGH);
    digitalWrite(15,LOW);
  }
  else if(value >= 40){
    digitalWrite(19,HIGH);
    digitalWrite(16,LOW);
    digitalWrite(15,LOW);
  }
  else{
    digitalWrite(19,LOW);
    digitalWrite(16,LOW);
    digitalWrite(15,LOW);
    
  }
  
}
