import sys;

sensorMultiplier = 0.125;

def stablizer(sensors,control):
	motors = [0,0,0,0];
	if control["throttle"] > 0:
		print(control["throttle"]);
		sys.stdout.flush();
		motors[0] = ( sensors["y"]/2)*sensorMultiplier + ( sensors["x"]/2)*sensorMultiplier + ( control["x"]/2) + (-control["y"]/2) + (-control["rotate"]/2) + (control["throttle"]);
		motors[1] = (-sensors["y"]/2)*sensorMultiplier + ( sensors["x"]/2)*sensorMultiplier + (-control["x"]/2) + (-control["y"]/2) + ( control["rotate"]/2) + (control["throttle"]);
		motors[2] = ( sensors["y"]/2)*sensorMultiplier + (-sensors["x"]/2)*sensorMultiplier + ( control["x"]/2) + ( control["y"]/2) + ( control["rotate"]/2) + (control["throttle"]);
		motors[3] = (-sensors["y"]/2)*sensorMultiplier + (-sensors["x"]/2)*sensorMultiplier + (-control["x"]/2) + ( control["y"]/2) + (-control["rotate"]/2) + (control["throttle"]);

		for value in motors:
			if value < 0:
				return [control["throttle"],control["throttle"],control["throttle"],control["throttle"]];

	return motors;
