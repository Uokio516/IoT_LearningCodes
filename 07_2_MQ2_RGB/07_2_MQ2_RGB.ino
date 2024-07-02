void setup() {
  Serial.begin(115200);
  pinMode(4,OUTPUT); //R
  pinMode(2,OUTPUT); //G
  pinMode(15,OUTPUT); //B
  pinMode(35,INPUT);
}

void loop() {
  int GasValue = analogRead(35);
  Serial.println(GasValue);
  if(GasValue >= 1000){
    analogWrite(15,255);
    analogWrite(2,0);
    analogWrite(4,255);
  }
  else if(GasValue >= 400 && GasValue < 1000){
    analogWrite(15,255);
    analogWrite(2,0);
    analogWrite(4,0);
  }
  
  else if(GasValue >= 200&& GasValue < 400){
    analogWrite(15,255);
    analogWrite(2,255);
    analogWrite(4,0);
  }
  
  else if(GasValue >= 0) {
    analogWrite(15,0);
    analogWrite(2,255);
    analogWrite(4,0);
  }
  delay(100);
}
