const int ledPin = 11; 
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
int analogVal = analogRead(A0);
Serial.println(analogVal);
delay(100);
int mapValue = constrain(map(analogVal, 0, 1023, 0, 255), 0, 100);

analogWrite(ledPin, mapValue);
Serial.println(mapValue);
delay(100);
}
