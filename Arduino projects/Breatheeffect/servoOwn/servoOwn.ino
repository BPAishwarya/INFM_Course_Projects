const int servoPin = 9;
int pos = 0;
int trigPin = 12;
int echoPin = 11;

void setup() {
  // put your setup code here, to run once:
pinMode(servoPin, OUTPUT);
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);
Serial.begin(9600);
}

void loop() {
  
  //delay(50);
  
  // put your main code here, to run repeatedly:
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    angle(pos);            // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
    dist();
    
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    angle(pos);               // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
    dist();
   
  }

}

void angle(int a){
  int highDu, lowDu; 
  
  highDu = map(a,0, 180, 500, 2600);
  lowDu = 20000 - highDu;

  digitalWrite(servoPin, HIGH);
  delayMicroseconds(highDu);

  digitalWrite(servoPin, LOW);
  delayMicroseconds(lowDu);
}

int dist(){
  long duration, distance = -1;

  digitalWrite(trigPin, HIGH);
  delay(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) * 0.0343;

  if(distance >= 200){
    Serial.println("...");
    //delay(2);
  }
  else{
    //Serial.print("Object Located at: ");
    Serial.print(distance);
    Serial.println("cm");
        //delay(2);

  }
  
}
