#include <Wire.h>
const int MPU = 0x68;
int16_t AcX, AcY, AcZ;//, Tmp, GyX, GyY, GyZ;

void setup() {
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(9600);
  /*for(int i = 0; i<6; i++)
    {
    Wire.beginTransmission(MPU);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU,12,true);
    AcX=Wire.read()<<8|Wire.read();
    AcY=Wire.read()<<8|Wire.read();
    AcZ=Wire.read()<<8|Wire.read();

    }*/
}
void loop() {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true);
  AcX = Wire.read() << 8 | Wire.read();
  AcY = Wire.read() << 8 | Wire.read();
  AcZ = Wire.read() << 8 | Wire.read();
  /*GyX=Wire.read()<<8|Wire.read();
    GyY=Wire.read()<<8|Wire.read();
    GyZ=Wire.read()<<8|Wire.read();*/

    Serial.println(AcX);
    Serial.println(AcY);
    Serial.println(AcZ);
  /*
    Serial.print("Gyroscope: ");
    Serial.print("X = "); Serial.print(GyX);
    Serial.print(" | Y = "); Serial.print(GyY);
    Serial.print(" | Z = "); Serial.println(GyZ);
    Serial.println(" ");
    bool cangodown = (AcY>4000 ? true : false);
    Serial.print(" | Go down?:");Serial.print(cangodown);
    bool cangoleft = (AcX>4000? true : false);
    Serial.print(" | Go left?:");Serial.print(cangoleft);
    bool cangoright = (AcX<-4000? true : false);
    Serial.print(" | Go right?:");Serial.println(cangoright);
    bool sleepornot = (AcZ > 16000 || AcZ < -16000 ? true : false);
    Serial.print(" | Sleep?:"); Serial.println(sleepornot);
    bool cangoup = (AcY < -4000 ? true : false);
    Serial.print(" | Go up?:"); Serial.print(cangoup);
  Serial.print("Accelerometer: ");
  Serial.print("X = ");
  Serial.print(AcX);
  Serial.print(" | Y = ");
  Serial.print(AcY);
  Serial.print(" | Z = ");
  Serial.println(AcZ);*/
  
  delay(1000);
}
