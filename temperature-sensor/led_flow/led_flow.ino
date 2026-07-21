/*
  Breathing light.
*/
const int ledPin = 8;

void setup() {
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);


  Serial.println("READY");
}

void loop() {
  digitalWrite(ledPin, HIGH);
  Serial.println("LED=1");
  delay(1000);

  digitalWrite(ledPin, LOW);
  Serial.println("LED=0");
  delay(1000);
}
