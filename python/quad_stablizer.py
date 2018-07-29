sensorMultiplier = 0.125;

def stablizer(sensors,control):
	motors = [0,0,0,0];
	if control["throttle"] > 0:
		motors[0] = ( sensors["x"]/2)*sensorMultiplier + ( sensors["y"]/2)*sensorMultiplier + ( control["x"]/2) + (-control["y"]/2) + (-control["rotate"]/2) + (control["throttle"]);
		motors[1] = (-sensors["x"]/2)*sensorMultiplier + ( sensors["y"]/2)*sensorMultiplier + (-control["x"]/2) + (-control["y"]/2) + ( control["rotate"]/2) + (control["throttle"]);
		motors[2] = ( sensors["x"]/2)*sensorMultiplier + (-sensors["y"]/2)*sensorMultiplier + ( control["x"]/2) + ( control["y"]/2) + ( control["rotate"]/2) + (control["throttle"]);
		motors[3] = (-sensors["x"]/2)*sensorMultiplier + (-sensors["y"]/2)*sensorMultiplier + (-control["x"]/2) + ( control["y"]/2) + (-control["rotate"]/2) + (control["throttle"]);
	return motors;
