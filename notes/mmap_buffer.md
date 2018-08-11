# Mmap Buffer variable types and locations

|I/O|Offset|Size (bytes)|Type|Name|Expanded Name|
|---|---|---|---|---|---|
|I|0|2|short|throttle|Throttle|
|I|2|2|short|moveX|X direction move speed|
|I|4|2|short|moveY|Y direction move speed|
|I|6|2|short|moveRot|Clockwise rotational move speed|
|I|8|2|short|cX|Calibration X axis level|
|I|10|2|short|cY|Calibration Y axis level|
|I|12|2|short|sensorMult|Auto-level correction multiplier|
|O|14|2|short|gX|Gyro X|
|O|16|2|short|gY|Gyro Y|
|O|18|2|short|gZ|Gyro Z|
|O|20|2|short|bar|Barometer / Altimeter|

##Total size: 22B
