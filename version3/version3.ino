//import libraries
#include <LedControl.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include <Wire.h>

//define those parameters
#define LightDensity 4 //Default light density
const int MPU = 0x68; // MPU address, 0x69 if 3.3V is provided
int16_t AcX, AcY, AcZ; //Acceleration in 3 axis
unsigned long delaytime = 90; //delaytime

//The following three are for LED matrix with MAX7219
#define DIN 12
#define CS 10
#define CLK 11
//ledcontrol setup:
LedControl lc = LedControl(DIN, CLK, CS, 2);

//Hourglass display setup & Physical paras
#define init_lines 9
#define total_no init_lines*(init_lines+1)/2 //number of balls to display
//#define I_fall 5  // ball inetrtia before falling
//#define I_roll 2  //ball inetrtia before rolling
#define MAX_row 7
#define MAX_col 7
#define MIN_row 0
#define MIN_col 0
#define MAX_matrix 1
#define MIN_matrix 0

struct ball {
  int matrix_no = 0;
  int row = 0;
  int col = 0;
};
ball ball[total_no];

void setup() {
  // put your setup code here, to run once:
  //Setup LED matrixes:
  lc.shutdown(0, false);
  lc.setIntensity(0, LightDensity);
  lc.clearDisplay(0);
  lc.shutdown(1, false);
  lc.setIntensity(1, LightDensity);
  lc.clearDisplay(1);
  for (int line_count = 0; line_count < init_lines; line_count++) {

    for (int i = 0; i <= line_count; i++) {
      ball[i + line_count * (line_count + 1) / 2].matrix_no = 0;
      ball[i + line_count * (line_count + 1) / 2].row = line_count - i;
      ball[i + line_count * (line_count + 1) / 2].col = i;
      lc.setLed(ball[i + line_count * (line_count + 1) / 2].matrix_no, ball[i + line_count * (line_count + 1) / 2].row, ball[i + line_count * (line_count + 1) / 2].col, true);
    }
    //These lines should be replaced by a function later
  }
  //Setup MPU-GY-521:
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(9600);
}

void LightUp() {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true);
  AcX = Wire.read() << 8 | Wire.read();
  AcY = Wire.read() << 8 | Wire.read();
  AcZ = Wire.read() << 8 | Wire.read();//Get acceleration in 3 axis.
  bool cangodown = (AcY > 4000 ? true : false);
  Serial.print(" | Go down?:"); Serial.print(cangodown);
  bool cangoleft = (AcX > 8000 ? true : false);
  Serial.print(" | Go left?:"); Serial.print(cangoleft);
  bool cangoright = (AcX < -4000 ? true : false);
  Serial.print(" | Go right?:"); Serial.println(cangoright);
  bool sleepornot = (AcZ > 16000 || AcZ < -16000 ? true : false);
  Serial.print(" | Sleep?:"); Serial.println(sleepornot);
  bool cangoup = (AcY < -4000 ? true : false);
  Serial.print(" | Go up?:"); Serial.print(cangoup);//see which way to go
  Serial.print("X:"); Serial.print(AcX);
  Serial.print("Y:"); Serial.print(AcY);
  Serial.print("Z:"); Serial.println(AcZ);
  int prePos[total_no][3];
  for (int i = total_no - 1; i >= 0 ; i--)
  {
    prePos[i][0] = ball[i].matrix_no;
    prePos[i][1] = ball[i].row;
    prePos[i][2] = ball[i].col;
    if (cangodown) {
      go_down(1 + cangoleft - cangoright, i);
    }
    if (cangoup) {
      go_up(1 + cangoleft - cangoright, i);
    }
    if (cangoleft) {
      go_left(i);
    }
    if (cangoright) {
      go_right(i);
    }

    /*if (hasBall(ball[i].matrix_no, ball[i].row, ball[i].col)) {
      if (cangodown) {
        if (!hasBall(ball[i].matrix_no, ball[i].row + 1, ball[i].col)) {
          ball[i].row++;
        }
        else if (!hasBall(ball[i].matrix_no, ball[i].row, ball[i].col + 1)) {
          ball[i].col++;
        }
      }
      else {
        ball[i].matrix_no = prePos[0];
        ball[i].row = prePos[1];
        ball[i].col = prePos[2];
      }
      }*/
  }
  delay(delaytime);
  for (int i = total_no - 1; i >= 0 ; i--)
  {
    lc.setLed(prePos[i][0], prePos[i][1], prePos[i][2], false);
    lc.setLed(ball[i].matrix_no, ball[i].row, ball[i].col, true);
  }
}
bool hasBall(int x, int y, int z) {
  for (int j = total_no - 1; j >= 0; j--) {
    if (x == ball[j].matrix_no && y == ball[j].row && z == ball[j].col) {
      return true;
    }
  }
  return false;
}
void go_down(int side, int i) {
  if (ball[i].row == MAX_row && ball[i].col != MAX_col && !hasBall(ball[i].matrix_no, ball[i].row, ball[i].col + 1) && side != 0) {
    ball[i].col ++;
  }
  else if (ball[i].col == MAX_col && ball[i].row != MAX_row && side != 2 && !hasBall(ball[i].matrix_no, ball[i].row + 1, ball[i].col)) {
    ball[i].row ++;
  }
  else if (ball[i].row == MAX_row && ball[i].col == MAX_col) {
    if (ball[i].matrix_no != MAX_matrix && !hasBall(1 - ball[i].matrix_no, 7 - ball[i].row, 7 - ball[i].col)) {
      gonextmatrix(i);
    }
  }
  else {
    if (side != 2 && !hasBall(ball[i].matrix_no, ball[i].row + 1, ball[i].col) && ball[i].row != MAX_row) {
      ball[i].row++;
    }//not going left
    if (side != 0 && !hasBall(ball[i].matrix_no, ball[i].row, ball[i].col + 1) && ball[i].col != MAX_col) {
      ball[i].col++;
    }//not going right
  }
}

void go_up(int side, int i) {
  if (ball[i].row == MIN_row && ball[i].col != MIN_col && !hasBall(ball[i].matrix_no, ball[i].row, ball[i].col - 1)) {
    ball[i].col--;
  }
  else if (ball[i].col == MIN_col && ball[i].row != MIN_row && !hasBall(ball[i].matrix_no, ball[i].row - 1, ball[i].col)) {
    ball[i].row--;
  }
  else if (ball[i].row == MIN_row && ball[i].col == MIN_col && ball[i].matrix_no != MIN_matrix && !hasBall(1 - ball[i].matrix_no, 7 - ball[i].row, 7 - ball[i].col)) {
    gonextmatrix(i);
  }
  else {
    if (side != 2 && !hasBall(ball[i].matrix_no, ball[i].row - 1, ball[i].col) && ball[i].row != MIN_row) {
      ball[i].row--;
    }
    if (side != 0 && !hasBall(ball[i].matrix_no, ball[i].row, ball[i].col - 1) && ball[i].col != MIN_col) {
      ball[i].col--;
    }
  }
}

void go_left(int i) {
  if (ball[i].col != MIN_col  && !hasBall(ball[i].matrix_no, ball[i].row, ball[i].col - 1)) {
    ball[i].col --;
  }
  if (ball[i].row != MAX_row  && !hasBall(ball[i].matrix_no, ball[i].row + 1, ball[i].col)) {
    ball[i].row++;
  }
}

void go_right(int i) {
  if (ball[i].col != MAX_col  && !hasBall(ball[i].matrix_no, ball[i].row, ball[i].col + 1)) {
    ball[i].col ++;
  }
  if (ball[i].row != MIN_row  && !hasBall(ball[i].matrix_no, ball[i].row - 1, ball[i].col)) {
    ball[i].row --;
  }
}
void gonextmatrix(int i) {
  ball[i].matrix_no = 1 - ball[i].matrix_no;
  ball[i].row = 7 - ball[i].row;
  ball[i].col = 7 - ball[i].col;
}
void loop() {
  // put your main code here, to run repeatedly:
  LightUp();
  /*for(int i = 0;i<2;i++){
    for( int j=0;j<8;j++){
      for (int k=0;k<8;k++){
        lc.setLed(i,j,k,true);
        delay(200);
        lc.setLed(i,j,k,false);
        delay(200);
      }
    }
    }*/
}
