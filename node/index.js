//NOTE: Higher-level computing for drone control

// IMPORTS
let {spawn} = require("child_process");
let path = require("path");
let SteamController = require("node-steam-controller");

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
let fc = spawn("taskset",["-c","3","python3","python/main.py"],{cwd: projectDir});
fc.stderr.pipe(process.stderr);
console.log("Spawned subsystem");
// SETUP PYTHON IPC
fc.stdout.setEncoding("ascii");
fc.stdout.on("data",(data) => { //Input from python script
	console.log(data);
	if(data == "Ready.\n") { //When python reports it is ready...
		sendControl(); //Send the current state of control
	} else if(data == "Shutdown.\n") { //When python says it's time to shutdown...
		spawn("shutdown",["-h","now"]);
	}
});
function sendControl() {
	console.log(control);
	fc.stdin.write(JSON.stringify(control)+"\n");
	fc.stdin.uncork();
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
