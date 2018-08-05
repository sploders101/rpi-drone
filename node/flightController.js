// Libraries and hardware constants
const i2c = require("i2c-bus");
const {Pca9685Driver} = require("pca9685");
const i2cBus = i2c.openSync(1);
const pcaOptions = {
	i2c: i2cBus,
	address: 0x40,
	frequency: 50,
	debug: false
};
let pwm = new Pca9685Driver(pcaOptions, function(err) {
	if(err) {
		console.error("Error initializing pwm");
		process.exit(-1);
	}
});

// User-configurable constants
const pwmMin = 800.0;
const pwmMax = 1900.0;
const pwmRange = pwmMax - pwmMin;
const gyro = 0x1c;
const register = 0x28;// Read 4 bytes for [x,y]
// const x = 0x2a;
const xMultiplier = 0.000030519;
// const y = 0x28;
const yMultiplier = -0.000030519;

let control = {
	_type: "control",
	x: 0,
	y: 0,
	rotate: 0,
	throttle: 0
};
let motors = new Array(4);

process.on("message",(msg) => {
	switch(msg._type) {
		case "control":
			control = msg;
	}
});

driveLoop();

process.send({"_type": "state","value": "Ready."});

function driveLoop() {
	let sensorData = Buffer.allocUnsafe(4);
	i2cBus.readI2cBlock(gyro,register,4,sensorData,(err) => {
		if(err) {
			console.error(err);
			return;
		}
		let x = sensorData.readInt16LE(0) * xMultiplier;
		let y = sensorData.readInt16LE(2) * yMultiplier;

		motors[0] = ( y/2) + ( x/2) + ( control.x/2) + (-control.y/2) + (-control.rotate/2) + (control.throttle);
		motors[1] = (-y/2) + ( x/2) + (-control.x/2) + (-control.y/2) + ( control.rotate/2) + (control.throttle);
		motors[2] = ( y/2) + (-x/2) + ( control.x/2) + ( control.y/2) + ( control.rotate/2) + (control.throttle);
		motors[3] = (-y/2) + (-x/2) + (-control.x/2) + ( control.y/2) + (-control.rotate/2) + (control.throttle);

		for (var i = 0; i < 4; i++) {
			if(motors[i] > 1) {
				let offset = motors[i] - 1;
				for (var i = 0; i < 4; i++) {
					motors[i] -= offset;
					if(motors[i] < 0) {
						motors[i] = pwmMin;
					}
				}
			} else if(motors[i] < 0) {
				motors[i] = pwmMin;
			}
		}

		let pwmSetters = new Array(4);
		for (var i = 0; i < motors.length; i++) {
			pwmSetters[i] = new Promise((resolve,reject) => {
				pwm.setPulseRange(i,0,motors[i] * pwmRange + pwmMin,(err) => {
					if(err) {
						reject("Error setting range");
					} else {
						resolve();
					}
				});
			});
		}
		Promise.all(pwmSetters).then(() => {
			process.nextTick(driveLoop);
		});

	});
}
