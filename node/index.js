#!/bin/node
//NOTE: Higher-level computing for drone control

// IMPORTS
let fs = require("fs");
let path = require("path");
let SteamController = require("node-steam-controller");

// INITIALIZE VARS
const fcRam = fs.openSync("/run/user/1000/mmapTest","r+");
let cRam = Buffer.alloc(44);

let projectDir = path.join(__dirname,"..");
let sc = new SteamController();
let control = {
	x: 0,
	y: 0,
	rotate: 0,
	throttle: 0
};
let calibration = JSON.parse(fs.readFileSync(`${__dirname}/../calibration.json`));

function sync() {
	cRam.writeFloatLE(control.throttle,0);
	cRam.writeFloatLE(control.x,4);
	cRam.writeFloatLE(control.y,8);
	cRam.writeFloatLE(control.rotate,12);
	cRam.writeFloatLE(calibration.x,16);
	cRam.writeFloatLE(calibration.y,20);
	cRam.writeFloatLE(calibration.sensors,24);
	fs.writeSync(fcRam,cRam,0,28,0);
	fs.readSync(fcRam,cRam,28,16,28);
}

// SETUP STEAM CONTROLLER INPUT
sc.lpad.on("touch",() => {
	control.throttle = 0.085;
	sync();
});
sc.lpad.on("move",(e) => {
	if(e.normy/2+0.5 < 0.085) {
		control.throttle = 0.085;
	} else {
		control.throttle = e.normy/2+0.5;
	}
	sync();
});
sc.lpad.on("untouch",() => {
	control.throttle = 0;
	sync();
});
// sc.rpad.on("touch",() => {
// 	control.throttle = 0.085;
// 	sync();
// });
sc.rpad.on("move",(e) => {
	control.x = e.normx;
	control.y = e.normy;
	sync();
});
sc.rpad.on("untouch",() => {
	control.x = 0;
	control.y = 0;
	sync();
});
sc.x.on('press',() => {
	calibration.x = cRam.readFloatLE(28);
	calibration.y = cRam.readFloatLE(32);
	sync();
});
sc.back.on('press',() => {

});
sc.forward.on('press',() => {

});
sc.y.on('press',() => {

});
sc.a.on('press',() => {

});
sc.connect();
