#!/bin/node
//NOTE: Higher-level computing for drone control

// IMPORTS
let {fork} = require("child_process");
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

let fc = fork(`${__dirname}/flightController.js`);
fc.on("message", (msg) => {
	switch(msg._type) {
		case "status":
			if(msg.value=="Ready.") {
				notify.ready();
				notify.startWatchdogMode(500);
			}
			break;
	}
});
function sendControl() {
	fc.send(control);
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
sc.x.on('press',() => {
	fc.send({
		_type: "calibrate"
	});
});
sc.connect();
