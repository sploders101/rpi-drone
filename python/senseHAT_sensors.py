from smbus2 import SMBus;
import time;
import sys;
bus = SMBus(1);
gyro = 0x1c;
x = 0x2A;
xMultiplier = 1;
y = 0x28;
yMultiplier = -1;

def int162c(dev,reg):
        num = bus.read_word_data(dev,reg);
        isNeg = num & 0x8000;
        if isNeg == 0x8000:
                num = num & 0x7FFF;
                num = num - 32768;
        return num;

def getSensors(orientation):
	orientation["x"] = int162c(gyro,x) * xMultiplier;
	orientation["y"] = int162c(gyro,y) * yMultiplier;
	return orientation;
