# Mmap Buffer variable types and locations

|I/O|Offset|Size (bytes)|Type|Name|Expanded Name|
|---|---|---|---|---|---|
|I|0|4|float|throttle|Throttle|
|I|4|4|float|moveX|X direction move speed|
|I|8|4|float|moveY|Y direction move speed|
|I|12|4|float|moveRot|Clockwise rotational move speed|
|I|16|4|float|cX|Calibration X axis level|
|I|20|4|float|cY|Calibration Y axis level|
|I|24|4|float|sensorMult|Auto-level correction multiplier|
|O|28|4|float|gX|Gyro X|
|O|32|4|float|gY|Gyro Y|
|O|36|4|float|gZ|Gyro Z|
|O|40|4|float|bar|Barometer / Altimeter|

##Total size: 44B
