unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;
unsigned long previousMillis3 = 0;

const long interval1 = 3000;  // 时间间隔1 (1秒)
const long interval2 = 3000;  // 时间间隔2 (2秒)
const long interval3 = 3000;  // 时间间隔3 (3秒)

void setup() {
  Serial.begin(115200);
  pinMode(15, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(19, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis1 >= interval1) {
    previousMillis1 = currentMillis;
    for (int i = 1; i < 255; i++) {
      analogWrite(15, i);
      delay(5);
    }

    for (int i = 255; i >= 0; i--) {
      analogWrite(15, i);
      delay(5);
    }
  }

  // 处理时间轴2
  if (currentMillis+500 - previousMillis2 >= interval2) {
    previousMillis2 = currentMillis;
    for (int i = 1; i < 255; i++) {
      analogWrite(16, i);
      delay(5);
    }

    for (int i = 255; i >= 0; i--) {
      analogWrite(16, i);
      delay(5);
    }
  }

  // 处理时间轴3
  if (currentMillis+1000 - previousMillis3 >= interval3) {
    previousMillis3 = currentMillis;
    for (int i = 1; i < 255; i++) {
      analogWrite(19, i);
      delay(5);
    }

    for (int i = 255; i >= 0; i--) {
      analogWrite(19, i);
      delay(5);
    }
  }

}
