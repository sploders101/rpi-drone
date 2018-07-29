def getSensors(sense):
	orientation = {};
	sensors = sense.get_orientation();
	orientation["x"] = sensors["roll"];
	orientation["y"] = sensors["pitch"];
	return orientation;
