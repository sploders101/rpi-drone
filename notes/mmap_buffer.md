# Mmap Buffer variable types and locations

|I/O|Offset|Size (bytes)|Type|Name|Expanded Name|
|---|---|---|---|---|---|
|I|0|2|int|throttle|Throttle|
|I|2|2|int|moveX|X direction move speed|
|I|4|2|int|moveY|Y direction move speed|
|I|6|2|int|moveRot|Clockwise rotational move speed|
|O|8|2|int|gX|Gyro X|
|O|10|2|int|gY|Gyro Y|
|O|12|2|int|gZ|Gyro Z|
|O|14|2|int|bar|Barometer / Altimeter|
|I|16|2|int|cX|Calibration X axis level|
|I|18|2|int|cY|Calibration Y axis level|
|I|20|2|int|sensorMult|Auto-level correction multiplier|
