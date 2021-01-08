#include <FastLED.h>
#include <Wire.h>

//Gyro
const int MPU = 0x68; 
float AccX, AccY, AccZ;
float GyroX, GyroY, GyroZ;
float accAngleX, accAngleY, gyroAngleX, gyroAngleY, gyroAngleZ;
float roll, pitch, yaw;
float AccErrorX, AccErrorY, GyroErrorX, GyroErrorY, GyroErrorZ;
float elapsedTime, currentTime, previousTime;
int c = 0;

//LED
const int LED_PIN = 7;
const int NUM_LEDS = 60;
CRGB leds[NUM_LEDS];

//FLEX
const int flexPin = A0;
int flexvalue;
int counter = 0;


void setup() {
// put your setup code here, to run once:
Serial.begin(19200);
Wire.begin();
FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);

  Wire.beginTransmission(MPU);       // Start communication with MPU6050 // MPU=0x68
  Wire.write(0x6B);                  // Talk to the register 6B
  Wire.write(0x00);                  // Make reset - place a 0 into the 6B register
  Wire.endTransmission(true);        //end the transmission
  
  // Configure Accelerometer Sensitivity - Full Scale Range (default +/- 8g)
  Wire.beginTransmission(MPU);
  Wire.write(0x1C);                  //Talk to the ACCEL_CONFIG register (1C hex)
  Wire.write(0x10);                  //Set the register bits as 00010000 (+/- 8g full scale range)
  Wire.endTransmission(true);
  // Configure Gyro Sensitivity - Full Scale Range (default +/- 1000deg/s)
  Wire.beginTransmission(MPU);
  Wire.write(0x1B);                   // Talk to the GYRO_CONFIG register (1B hex)
  Wire.write(0x10);                   // Set the register bits as 00010000 (1000deg/s full scale)
  Wire.endTransmission(true);
  delay(20);
  
  //Getting IMU error values 
  calculate_IMU_error();
  delay(20);

}

void loop() {
// put your main code here, to run repeatedly:

//Reading Flex
  flexvalue = analogRead(flexPin);
  flexvalue = map(flexvalue, 600, 900, 0, 255); //Map value 0-1023 to 0-255 (PWM)
  Serial.print(F("Flex:"));
  Serial.print(flexvalue);               //Print value
  Serial.print(F(","));
  delay(10);  

//get roll,pitch and yaw value
  float prevRoll = roll;
  float prevYaw = yaw;
  getRPY();
  float rollVal = roll;
  float yawVal = yaw;
  float rollDiff = fabsf(prevRoll - rollVal);
  float yawDiff = fabsf(prevYaw - yawVal);
//Serial.print(F("Diff : ")); 
//Serial.println(rollDiff);
 

//Checking posture with flex, rollDiff and yawDiff values from sensors
  if(flexvalue < 90){
    counter = counter + 1;
    if(counter > 10){
     badPosture();
     counter = 0;
     playWarning();
    }
  }
  else if(flexvalue > 200 || rollDiff > 3 || yawDiff > 3 ){
    counter = counter + 1;
    if(counter > 10){
     badPosture();
     counter = 0;
     playWarning();
    }
  }
  else{
    goodPosture();
  }

}

//melody for warning
void playWarning(){
     tone(3, 100, 1000);
     delay(100);
     noTone(3);
     tone(3, 500, 1000);
     delay(100);
     noTone(3);
     tone(3, 1000, 1000);
     delay(100);
     noTone(3);
}

//led lighting for bad posture
void badPosture(){
  for (int i = 0; i <= 20; i++) {
    leds[i] = CRGB::Red;
    FastLED.show();
    //delay(40);
    }
}

//led lighting for good posture
void goodPosture(){

 for (int i = 0; i <= 20; i++) {
    leds[i] = CRGB::Cyan;
    FastLED.show();
    }
}

void getRPY(){
  // Reading Accelerometer data //
  Wire.beginTransmission(MPU);
  Wire.write(0x3B); // Start with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
  //For a range of +-8g, we need to divide the raw values by 4096.0, according to the datasheet
  AccX = (Wire.read() << 8 | Wire.read()) / 4096.0; // X-axis value
  AccY = (Wire.read() << 8 | Wire.read()) / 4096.0; // Y-axis value
  AccZ = (Wire.read() << 8 | Wire.read()) / 4096.0; // Z-axis value
  // Calculating Roll and Pitch from the accelerometer data
  accAngleX = (atan(AccY / sqrt(pow(AccX, 2) + pow(AccZ, 2))) * 180 / PI) + 8.46; // AccErrorX ~(8.46)
  accAngleY = (atan(-1 * AccX / sqrt(pow(AccY, 2) + pow(AccZ, 2))) * 180 / PI) - 10.97; // AccErrorY ~(10.97)
  // === Read gyroscope data === //
  previousTime = currentTime;        // Previous time is stored before the actual time read
  currentTime = millis();            // Current time actual time read
  elapsedTime = (currentTime - previousTime) / 1000; // Divide by 1000 to get seconds
  Wire.beginTransmission(MPU);
  Wire.write(0x43); // Gyro data first register address 0x43
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // Read 4 registers total, each axis value is stored in 2 registers
  GyroX = (Wire.read() << 8 | Wire.read()) / 32.8; // For a 1000deg/s range we have to divide first the raw value by 32.8, according to the datasheet
  GyroY = (Wire.read() << 8 | Wire.read()) / 32.8;
  GyroZ = (Wire.read() << 8 | Wire.read()) / 32.8;
  // Correct the outputs with the calculated error values
  GyroX = GyroX - 2.44; // GyroErrorX ~(2.44)
  GyroY = GyroY + 3.02; // GyroErrorY ~(-3.02)
  GyroZ = GyroZ + 1.73; // GyroErrorZ ~ (-1.73)
  // Currently the raw values are in degrees per seconds, deg/s, so we need to multiply by sendonds (s) to get the angle in degrees
  gyroAngleX = gyroAngleX + GyroX * elapsedTime; // deg/s * s = deg
  gyroAngleY = gyroAngleY + GyroY * elapsedTime;
  //calculate the yaw value
  yaw =  yaw + GyroZ * elapsedTime;
  // filter - combine acceleromter(4%) and gyro(96%) angle values 
  roll = 0.96 * gyroAngleX + 0.04 * accAngleX;
  pitch = 0.96 * gyroAngleY + 0.04 * accAngleY;
  
  // Print the values on the serial monitor
  Serial.print(F("Roll:"));
  Serial.print(roll);
  Serial.print(F(","));
  Serial.print(F("Pitch:"));
  Serial.print(pitch);
  Serial.print(F(","));
  Serial.print(F("Yaw:"));
  Serial.println(yaw);
}

void calculate_IMU_error() {
  //executed once in setup
  // Read accelerometer values 200 times
  while (c < 200) {
    Wire.beginTransmission(MPU);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 6, true);
    AccX = (Wire.read() << 8 | Wire.read()) / 4096.0 ;
    AccY = (Wire.read() << 8 | Wire.read()) / 4096.0 ;
    AccZ = (Wire.read() << 8 | Wire.read()) / 4096.0 ;
    // Sum all readings
    AccErrorX = AccErrorX + ((atan((AccY) / sqrt(pow((AccX), 2) + pow((AccZ), 2))) * 180 / PI));
    AccErrorY = AccErrorY + ((atan(-1 * (AccX) / sqrt(pow((AccY), 2) + pow((AccZ), 2))) * 180 / PI));
    c++;
  }
  //Divide the sum by 200 to get the error value
  AccErrorX = AccErrorX / 200;
  AccErrorY = AccErrorY / 200;
  c = 0;
  // Read gyro values 200 times
  while (c < 200) {
    Wire.beginTransmission(MPU);
    Wire.write(0x43);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 6, true);
    GyroX = Wire.read() << 8 | Wire.read();
    GyroY = Wire.read() << 8 | Wire.read();
    GyroZ = Wire.read() << 8 | Wire.read();
    // Sum all readings
    GyroErrorX = GyroErrorX + (GyroX / 32.8);
    GyroErrorY = GyroErrorY + (GyroY / 32.8);
    GyroErrorZ = GyroErrorZ + (GyroZ / 32.8);
    c++;
  }
  //Divide the sum by 200 to get the error value
  GyroErrorX = GyroErrorX / 200;
  GyroErrorY = GyroErrorY / 200;
  GyroErrorZ = GyroErrorZ / 200;
  // Print the error values on the Serial Monitor
//  Serial.print(F("AccErrorX: "));
//  Serial.println(AccErrorX);
//  Serial.print(F("AccErrorY: "));
//  Serial.println(AccErrorY);
//  Serial.print(F("GyroErrorX: "));
//  Serial.println(GyroErrorX);
//  Serial.print(F("GyroErrorY: "));
//  Serial.println(GyroErrorY);
//  Serial.print(F("GyroErrorZ: "));
//  Serial.println(GyroErrorZ);
}

//----- Accelerometer and Gyroscope code inspired by : https://howtomechatronics.com/tutorials/arduino/arduino-and-mpu6050-accelerometer-and-gyroscope-tutorial/
