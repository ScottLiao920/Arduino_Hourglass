import serial
import io
from sympy import *


def getparas():
    x = 0
    y = 0
    z = 0
    for i in range(5):
        x += float(sio.readline())
        y += float(sio.readline())
        z += float(sio.readline())
        print("AcX AcY AcZ")
        print(x,y,z)
    x = x/5.00
    y = y/5.00
    z = z/5.00
    print("AcX AcY AcZ(averaged)")
    print(x,y,z)
    return x,y,z

arduino = serial.Serial("COM3",timeout=1, baudrate=9600)
print("hey")
sio = io.TextIOWrapper(io.BufferedRWPair(arduino, arduino))
print(sio.readline())
data = ''
print("Another Approach")
x = [None]*10
y = [None]*10
z = [None]*10
for i in range(10):
    print("Another iteration")
    x[i], y[i], z[i] = getparas();
print(x,y,z)
a,b,c,A,B,C = symbols('a b c A B C')
print(solve([
    ((x[0]-A)**2)/(a**2) + ((y[0]-B)**2)/(b**2) + ((z[0]-C)**2)/(c**2),
    ((x[1]-A)**2)/(a**2) + ((y[1]-B)**2)/(b**2) + ((z[1]-C)**2)/(c**2),
    ((x[2]-A)**2)/(a**2) + ((y[2]-B)**2)/(b**2) + ((z[2]-C)**2)/(c**2),
    ((x[3]-A)**2)/(a**2) + ((y[3]-B)**2)/(b**2) + ((z[3]-C)**2)/(c**2),
    ((x[4]-A)**2)/(a**2) + ((y[4]-B)**2)/(b**2) + ((z[4]-C)**2)/(c**2),
    ((x[5]-A)**2)/(a**2) + ((y[5]-B)**2)/(b**2) + ((z[5]-C)**2)/(c**2),
    ((x[6]-A)**2)/(a**2) + ((y[6]-B)**2)/(b**2) + ((z[6]-C)**2)/(c**2),
    ((x[7]-A)**2)/(a**2) + ((y[7]-B)**2)/(b**2) + ((z[7]-C)**2)/(c**2),
    ((x[8]-A)**2)/(a**2) + ((y[8]-B)**2)/(b**2) + ((z[8]-C)**2)/(c**2),
    ((x[9]-A)**2)/(a**2) + ((y[9]-B)**2)/(b**2) + ((z[9]-C)**2)/(c**2),
    ],[a,b,c,A,B,C]))
