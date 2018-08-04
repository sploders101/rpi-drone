#!/bin/node
//NOTE: Higher-level computing for drone control

// IMPORTS
let {spawn} = require("child_process");
let path = require("path");
let SteamController = require("node-steam-controller");
let notify = require("sd-notify");

// INITIALIZE VARS
let projectDir = path.join(__dirname,"..");
let sc = new SteamController();
let control = {
	_type: "control",
	x: 0,
	y: 0,
	rotate: 0,
	throttle: 0
};

// SPAWN PYTHON SUBSYSTEM ON CORE 3
let fc = pytalk.worker(`${projectDir}/python/main.py`);
let fcControl = fc.method("sendControl");
notify.ready();
notify.startWatchdogMode(500);
console.log("Spawned subsystem");

function sendControl() {
	// console.log(control);
	fcControl(control);
}

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
sc.connect();
