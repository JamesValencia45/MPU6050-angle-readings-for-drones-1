#include <Wire.h>

int pin1=6;
int pin2=7;
int pin3=8;
int pin4=9;
String pin1act;
String pin2act;
String pin3act;
String pin4act;
int value1;

long accelX, accelY, accelZ;
float gForceX, gForceY, gForceZ;

long gyroX, gyroY, gyroZ;
float rotX, rotY, rotZ;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(pin1,OUTPUT);
pinMode(pin2,OUTPUT);
pinMode(pin3,OUTPUT);
pinMode(pin4,OUTPUT);
  Wire.begin();
    setupMPU();
}

void loop() {
  // put your main code here, to run repeatedly:
  recordAccelRegisters();
  recordGyroRegisters();
  led();
  printData();
  delay(100);
}
void setupMPU(){
  Wire.beginTransmission(0b1101000); //This is the I2C address of the MPU (b1101000/b1101001 for AC0 low/high datasheet sec. 9.2)
  Wire.write(0x6B); //Accessing the register 6B - Power Management (Sec. 4.28)
  Wire.write(0b00000000); //Setting SLEEP register to 0. (Required; see Note on p. 9)
  Wire.endTransmission();  
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x1B); //Accessing the register 1B - Gyroscope Configuration (Sec. 4.4) 
  Wire.write(0x00000000); //Setting the gyro to full scale +/- 250deg./s 
  Wire.endTransmission(); 
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x1C); //Accessing the register 1C - Acccelerometer Configuration (Sec. 4.5) 
  Wire.write(0b00000000); //Setting the accel to +/- 2g
  Wire.endTransmission(); 
}
void recordAccelRegisters(){
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x3B); //Starting register for Accel Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6); //Request Accel Registers (3B - 40)
  while(Wire.available() < 6);
  accelX = Wire.read()<<8|Wire.read(); //Store first two bytes into accelX
  accelY = Wire.read()<<8|Wire.read(); //Store middle two bytes into accelY
  accelZ = Wire.read()<<8|Wire.read(); //Store last two bytes into accelZ
  processAccelData();
}
void processAccelData(){
    gForceX = accelX / 16384.0;
    gForceY = accelY / 16384.0; 
    gForceZ = accelZ / 16384.0;
}
void recordGyroRegisters(){
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x43); //Starting register for Gyro Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6); //Request Gyro Registers (43 - 48)
  while(Wire.available() < 6);
  gyroX = Wire.read()<<8|Wire.read(); //Store first two bytes into accelX
  gyroY = Wire.read()<<8|Wire.read(); //Store middle two bytes into accelY
  gyroZ = Wire.read()<<8|Wire.read(); //Store last two bytes into accelZ
  processGyroData();
}
void processGyroData(){
    rotX = gyroX / 131.0;
    rotY = gyroY / 131.0; 
    rotZ = gyroZ / 131.0;
}
void led(){
  if (gForceX>0.2){
    Serial.println("Pin1,Pin2");
    pin1act="active";
    pin2act="active";
    digitalWrite(pin2,HIGH);
    digitalWrite(pin1,HIGH);
    digitalWrite(pin3,LOW);
    digitalWrite(pin4,LOW);
  }
  if  (gForceX<-0.2){
    Serial.println("Pin3,Pin4");
    pin3act="active";
    pin4act="active";
    digitalWrite(pin3,HIGH);
    digitalWrite(pin4,HIGH);
    digitalWrite(pin1,LOW);
    digitalWrite(pin2,LOW);
  }
    if (gForceY>0.2){
    Serial.println("Pin2,Pin4");
    pin2act="active";
    pin4act="active";
    digitalWrite(pin1,LOW);
    digitalWrite(pin3,LOW);
    digitalWrite(pin2,HIGH);
    digitalWrite(pin4,HIGH);
  }
  if  (gForceY<-0.2){
    Serial.println("Pin1,Pin3");
    pin3act="active";
    pin1act="active";
    digitalWrite(pin1,HIGH);
    digitalWrite(pin3,HIGH);
    digitalWrite(pin2,LOW);
    digitalWrite(pin4,LOW);
  }
  if  (gForceX>0.2 && gForceY<-0.2){
    Serial.println("Pin1,Pin3,Pin2");
    pin3act="active";
    pin1act="active";
    pin1act="active";
    digitalWrite(pin2,HIGH);
    digitalWrite(pin4,LOW);
    digitalWrite(pin3,HIGH);
    digitalWrite(pin1,HIGH);
  }
  if (gForceX>0.2 && gForceY>0.2){
    Serial.println("Pin1,Pin2,Pin4");
    digitalWrite(pin2,HIGH);
    digitalWrite(pin1,HIGH);
    digitalWrite(pin4,HIGH);
    digitalWrite(pin3,LOW);
  }
  if  (gForceX<-0.2 && gForceY>0.2){
    Serial.println("Pin1,Pin2,Pin3");
    digitalWrite(pin1,HIGH);
    digitalWrite(pin3,HIGH);
    digitalWrite(pin4,LOW);
    digitalWrite(pin2,HIGH);
  }
    if  (gForceX<-0.2 && gForceY<-0.2){
    digitalWrite(pin1,HIGH);
    digitalWrite(pin3,HIGH);
    digitalWrite(pin4,HIGH);
    digitalWrite(pin2,LOW);
  }
  if  (gForceX<-0.2 && gForceY>0.2){
    digitalWrite(pin3,HIGH);
    digitalWrite(pin4,HIGH);
    digitalWrite(pin2,HIGH);
    digitalWrite(pin1,LOW);
  }
  if  (gForceX>-0.2 && gForceY<0.2 && gForceY>-0.2 && gForceX <0.2){
    digitalWrite(pin3,LOW);
    digitalWrite(pin4,LOW);
    digitalWrite(pin2,LOW);
    digitalWrite(pin1,LOW);
}
}
void printData(){
  Serial.print("Gyro (deg)");
  Serial.print(" X=");
  Serial.print(rotX);
  Serial.print(" Y=");
  Serial.print(rotY);
  Serial.print(" Z=");
  Serial.print(rotZ);
  Serial.print(" Accel (g)");
  Serial.print(" X=");
  Serial.print(gForceX);
  Serial.print(" Y=");
  Serial.print(gForceY);
  Serial.print(" Z=");
  Serial.println(gForceZ);
}
