# Arduino_Hourglass
For NTU DV Club AY2018
## Purpose
    This is for NTU Da Vinci 3D Printing and Robomasters Club AY2018-2019 Arduino Hourglass Workshop.
## Material List
    8*8 LED matrix with MAX7219 register (qty:2)
    MPU6050 GY521 6 AXIS ACCELEROMETER + GYRO (qyt:2)
    Arduino Uno(for early versions) (qty:1)
    Arduino Nano(for later versions and formal one) (qty:1)
    Jumper wires
   
## Development Log
#### 2018.9.18 Version 1 
    With no margin hence it may drop outside of LED matrix.
    Need to do calibration calculated by matrix.
    Only 1 led will light up, which means an hourglass with only one sand.
#### 2018.10.1 Version 2
    Happy birthday, PRC!
    Uploaded version 2 with more led bulbs.
    Margin feature is done.
    Go_up function seems a little wrong.
    I am really a little genius!
#### 2018.10.2 Version 3
    Done with the go_up function part.
    Need to implement some function like wakeup and sleep;
    Can try add inertia to the model. 
## File explanation
### tryout
    Version 1, including all features.
### gy521
    Including:
    ino file to get the reading for acceleration and write those on serial port for calibration by python
    py file which is meant to read the accelerations in serial port and do calibration.
    Need to adjust offset manually after run this program.
### ledMatrixTest
    Including:
    An ino file which can test wiring and led matrix.
### version 2
    Code for version 2;
    need to implement go_up function;
### version 3
    Version 2 with better if_else structures and working go-up function.
