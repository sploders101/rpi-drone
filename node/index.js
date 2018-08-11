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
	cRam.writeInt16LE(control.throttle*1000,0);
	cRam.writeInt16LE(control.x*1000,2);
	cRam.writeInt16LE(control.y*1000,4);
	cRam.writeInt16LE(control.rotate*1000,6);
	cRam.writeInt16LE(calibration.x,8);
	cRam.writeInt16LE(calibration.y,10);
	cRam.writeInt16LE(calibration.sensors,12);
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
	calibration.x = cRam.readInt16LE(14);
	calibration.y = cRam.readInt16LE(16);
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
