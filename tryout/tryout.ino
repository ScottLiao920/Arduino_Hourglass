//import libraries
#include <LedControl.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include <Wire.h>

//define those parameters
#define LightDensity 6 //Default light density
const int MPU = 0x68; // MPU address, 0x69 if 3.3V is provided
int16_t AcX, AcY, AcZ; //Acceleration in 3 axis
unsigned long delaytime = 500; //delaytime
//The following three are for LED matrix with MAX7219
#define DIN 12
#define CS 10
#define CLK 11
//ledcontrol setup:
LedControl lc = LedControl(DIN, CLK, CS, 2);

//Hourglass display setup & Physical paras
#define total_no 40 //number of balls to display
#define I_fall 5  // ball inetrtia before falling
#define I_roll 2  //ball inetrtia before rolling
struct ball {
  int matrix_no = 0;
  int row = 0;
  int col = 0;
};
ball ball;

void setup() {
  // put your setup code here, to run once:
  //Setup LED matrixes:
  lc.shutdown(0, false);
  lc.setIntensity(0, LightDensity);
  lc.clearDisplay(0);
  lc.shutdown(1, false);
  lc.setIntensity(1, LightDensity);
  lc.clearDisplay(1);

  ball.matrix_no = 0;//From here
  ball.row = 0;
  ball.col = 0;
  lc.setLed(ball.matrix_no,ball.row,ball.col,true);//These 5 lines should be replaced by a function later

  //Setup MPU-GY-521:
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(9600);
}

void LightUp() {
  int prePos[3] = {ball.matrix_no,ball.row,ball.col};
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true);
  AcX = Wire.read() << 8 | Wire.read();
  AcY = Wire.read() << 8 | Wire.read();
  AcZ = Wire.read() << 8 | Wire.read();//Get acceleration in 3 axis.
  bool cangodown = (AcY > 4000 ? true : false);
  Serial.print(" | Go down?:"); Serial.print(cangodown);
  bool cangoleft = (AcX > 4000 ? true : false);
  Serial.print(" | Go left?:"); Serial.print(cangoleft);
  bool cangoright = (AcX < -4000 ? true : false);
  Serial.print(" | Go right?:"); Serial.println(cangoright);
  bool sleepornot = (AcZ > 16000 || AcZ < -16000 ? true : false);
  Serial.print(" | Sleep?:"); Serial.println(sleepornot);
  bool cangoup = (AcY < -4000 ? true : false);
  Serial.print(" | Go up?:"); Serial.print(cangoup);//see which way to go  
  if(cangodown){
    godown();
  }
  if(cangoleft){
    goleft();
  }
  if(cangoright){
    goright();
  }
  if(cangoup){
    goup();
  }
  delay(delaytime);
  lc.setLed(prePos[0],prePos[1],prePos[2],false);  
  lc.setLed(ball.matrix_no,ball.row,ball.col,true);
}
void godown(){
    ball.row ++;
    ball.col ++;
    if((ball.row>7 || ball.col>7)&&ball.matrix_no == 0){
      ball.matrix_no ++;
      ball.col = 0;
      ball.row = 0;
    }
    else if ((ball.row>7 || ball.col>7)&&ball.matrix_no != 0){
      ball.row = 7;
      ball.col = 7;
    }
}
void goleft(){
    ball.col --;
    if((ball.row>7 || ball.col>7)&&ball.matrix_no == 0){
      ball.matrix_no ++;
      ball.col = 0;
      ball.row = 0;
    }
    else if ((ball.row>7 || ball.col>7)&&ball.matrix_no != 0){
      ball.row = 7;
      ball.col = 7;
    }
}
void goright(){
    ball.col ++;
    if((ball.row>7 || ball.col>7)&&ball.matrix_no == 0){
      ball.matrix_no ++;
      ball.col = 0;
      ball.row = 0;
    }
    else if ((ball.row>7 || ball.col>7)&&ball.matrix_no != 0){
      ball.row = 7;
      ball.col = 7;
    }
}
void goup(){
    ball.col --;
    ball.row --;
    if((ball.row>7 || ball.col>7)&&ball.matrix_no == 0){
      ball.matrix_no ++;
      ball.col = 0;
      ball.row = 0;
    }
    else if ((ball.row>7 || ball.col>7)&&ball.matrix_no != 0){
      ball.row = 7;
      ball.col = 7;
    }
}
void loop() {
  // put your main code here, to run repeatedly:
  LightUp();
}
