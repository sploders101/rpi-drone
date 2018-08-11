#!/bin/node
//NOTE: Higher-level computing for drone control

// IMPORTS
let fs = require("fs");
let mmap = require("mmap.js");
let path = require("path");
let SteamController = require("node-steam-controller");

// INITIALIZE VARS
const fd = fs.openSync("/run/user/1000/mmapTest","r+");
let cRam = mmap.alloc(
    50,
    mmap.PROT_READ | mmap.PROT_WRITE,
    mmap.MAP_SHARED,
    fd,
    0);

let projectDir = path.join(__dirname,"..");
let sc = new SteamController();
let control = {
	x: 0,
	y: 0,
	rotate: 0,
	throttle: 0
};
let calibration = JSON.parse(fs.readFileSync(`${__dirname}/../calibration.json`));

function sendControl() {
	console.log(cRam.readFloatLE(16));
	cRam.writeFloatLE(control.throttle,0);
	cRam.writeFloatLE(control.x,4);
	cRam.writeFloatLE(control.y,8);
	cRam.writeFloatLE(control.rotate,12);
	cRam.writeFloatLE(calibration.x,32);
	cRam.writeFloatLE(calibration.y,36);
	cRam.writeFloatLE(calibration.sensors,40);
	console.log(cRam.readFloatLE(40));
}
function syncRam() {
	mmap.sync(cRam, 0, mmap.PAGE_SIZE, mmap.MS_SYNC);
}
setInterval(syncRam,10);

// SETUP STEAM CONTROLLER INPUT
sc.lpad.on("touch",() => {
	control.throttle = 0.085;
	sendControl();
});
sc.lpad.on("move",(e) => {
	if(e.normy/2+0.5 < 0.085) {
		control.throttle = 0.085;
	} else {
		control.throttle = e.normy/2+0.5;
	}
	sendControl();
});
sc.lpad.on("untouch",() => {
	control.throttle = 0;
	sendControl();
});
// sc.rpad.on("touch",() => {
// 	control.throttle = 0.085;
// 	sendControl();
// });
sc.rpad.on("move",(e) => {
	control.x = e.normx;
	control.y = e.normy;
	sendControl();
});
sc.rpad.on("untouch",() => {
	control.x = 0;
	control.y = 0;
	sendControl();
});
sc.x.on('press',() => {
	calibration.x = cRam.readFloatLE(16);
	calibration.y = cRam.readFloatLE(20);
	sendControl();
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
