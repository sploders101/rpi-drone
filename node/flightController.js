// Libraries and hardware constants
const i2c = require("i2c-bus");
const {Pca9685Driver} = require("pca9685");
const i2cBus = {device: "/dev/i2c-1"};
const pcaOptions = {
	i2c: i2c.openSync(1),
	address: 0x40,
	frequency: 50,
	debug: false;
};
let pwm = new Pca9685Driver(pcaOptions, function(err) {
	if(err) {
		console.error("Error initializing pwm");
		process.exit(-1);
	}
});

// User-configurable constants
const gyro = new i2c(0x1c,i2cBus);
const register = 0x28;// Read 4 bytes for [x,y]
// const x = 0x2a;
const xMultiplier = 1;
// const y = 0x28;
const yMultiplier = -1;

let control = {
	_type: "control",
	x: 0,
	y: 0,
	rotate: 0,
	throttle: 0
};

process.on("message",(msg) => {
	switch(msg._type) {
		case "control":
			control = msg;
	}
});

driveLoop();

process.send({"_type": "state","value": "Ready."});

function driveLoop() {

}
