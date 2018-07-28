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
import sys,select,time,json;

# INITIALIZATION
drone = Adafruit_PCA9685.PCA9685();
sense = SenseHat();
control = {};
control["throttle"] = 0.0;
control["x"] = 0.0;
control["y"] = 0.0;

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
	# print(int( throttle*sMultiplier + smin ));
	drone.set_all_pwm(0,int( throttle*sMultiplier + smin ));

# STANDARD FUNCTIONS
def readIn():
	if sys.stdin in select.select([sys.stdin], [], [], 0)[0]:
		line = json.loads(sys.stdin.readline());
		if line:
			return line;
		else:
			return false;

# MAIN

print("Ready.");

while 1:
	# Accept changes
	stdin = readIn();
	if stdin:
		control = stdin;

	# Drone instruction
	# print(control);
	setAll(control["throttle"]);
	time.sleep(0.001);
