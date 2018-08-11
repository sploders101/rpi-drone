# Mmap Buffer variable types and locations

|I/O|Offset|Size (bytes)|Type|Name|Expanded Name|
|---|---|---|---|---|---|
|I||4|float|throttle|Throttle|
|I||4|float|moveX|X direction move speed|
|I||4|float|moveY|Y direction move speed|
|I||4|float|moveRot|Clockwise rotational move speed|
|O||4|float|gX|Gyro X|
|O||4|float|gY|Gyro Y|
|O||4|float|gZ|Gyro Z|
|O||4|float|bar|Barometer / Altimeter|
|I||4|float|cX|Calibration X axis level|
|I||4|float|cY|Calibration Y axis level|
|I||4|float|sensorMult|Auto-level correction multiplier|
