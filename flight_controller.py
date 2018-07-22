#!/usr/bin/env python

# CONSTANTS
smin = 800.0;
smax = 1900.0;

# DERRIVED VARIABLES
sMultiplier = smax-smin;

# IMPORTS
import Adafruit_PCA9685;
from sense_hat import SenseHat;
import sys;
import time;

# INITIALIZE DEVICES
drone = Adafruit_PCA9685.PCA9685();
sense = SenseHat();

# FUNCTIONS
def setSpeedPercentage(channel, speed): # channel (0..4), speed (0..1)
	pulseWidth = (speed*(smax-smin)) + smin;
	drone.set_pwm(channel,0,int(pulseWidth));
def setOutput(speed0, speed1, speed2, speed3):
	drone.set_pwm(0, 0, int( speed0*sMultiplier + smin ) );
	drone.set_pwm(1, 0, int( speed1*sMultiplier + smin ) );
	drone.set_pwm(2, 0, int( speed2*sMultiplier + smin ) );
	drone.set_pwm(3, 0, int( speed3*sMultiplier + smin ) );
def setAll(throttle):
	print(int( throttle*sMultiplier + smin ));
	drone.set_all_pwm(0,int( throttle*sMultiplier + smin ));

# MAIN
setAll(float(sys.argv[1]));
