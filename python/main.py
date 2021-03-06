# Lower-level control for balance and speed calculations

# CONSTANTS
smin = 800.0;
smax = 1900.0;

# CONSTANT CALCULATED VARIABLES
sMultiplier = smax-smin;

# IMPORTS
from quad_stablizer import stablizer;
from senseHAT_sensors import getSensors;
import Adafruit_PCA9685;
import sys,select,time,json;

# INITIALIZATION
drone = Adafruit_PCA9685.PCA9685();
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
		send(speed);
		drone.set_pwm(motor, 0, int( speed*sMultiplier + smin ) );
		motor += 1;
def setAll(throttle):
	# print(int( throttle*sMultiplier + smin ));
	drone.set_all_pwm(0,int( throttle*sMultiplier + smin ));

# STANDARD FUNCTIONS
def readIn():
	if select.select([sys.stdin], [], [], 0)[0]:
		sline = sys.stdin.readline();
		send(sline);
		if sline.find("{")>=0:
			line = json.loads(sline);
			send(line["throttle"]);
			if line["_type"] == "control":
				control = line;
def send(str1):
	print(str1);
	sys.stdout.flush();
# MAIN
send("Ready.");

sense = {}

while 1:
    # Handle physical inputs
	# for event in sense.stick.get_events():
	# 	if event.action == "pressed" and event.direction == "middle":
	# 		send("Shutdown.");

	# Handle incoming messages
	readIn();
	# Instruct drone limbs
	setOutput(stablizer(getSensors(sense),control));
	# getSensors(sense);
	# setAll(control["throttle"]);
	time.sleep(0.001);
