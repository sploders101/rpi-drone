# Lower-level control for balance and speed calculations

# CONSTANTS
smin = 800.0;
smax = 1900.0;

# CONSTANT CALCULATED VARIABLES
sMultiplier = smax-smin;

# IMPORTS
import quad_stablizer;
import Adafruit_PCA9685;
from sense_hat import SenseHat;
import sys,time,json;

# INITIALIZATION
drone = Adafruit_PCA9685.PCA9685();
sense = SenseHat();

# DRONE FUNCTIONS
def setSpeedPercentage(channel, speed): # channel (0..4), speed (0..1)
	pulseWidth = (speed*(smax-smin)) + smin;
	drone.set_pwm(channel,0,int(pulseWidth));
def setOutput(speeds):
	motor = 0;
	for speed in speeds:
		drone.set_pwm(motor, 0, int( speed*sMultiplier + smin ) );
		motor += 1;
def setAll(throttle):
	print(int( throttle*sMultiplier + smin ));
	drone.set_all_pwm(0,int( throttle*sMultiplier + smin ));

# STANDARD FUNCTIONS
def readIn():
	lines = sys.stdin.readLine();
	if lines != null:
		return lines;
	else:
		return "NULL";
# MAIN
while True:
	print(readIn());
