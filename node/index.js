//NOTE: Higher-level computing for drone control

// IMPORTS
let spawn = require("child_process").spawn;
let path = require("path");
let SteamController = require("node-steam-controller");

// SPAWN PYTHON SUBSYSTEM ON CORE 3
let fc = spawn("taskset",["-c","3","python3","python/main.py"],{cwd: projectDir});

// INITIALIZE VARS
let projectDir = path.join(__dirname,"..");
let sc = new SteamController();
let control = {
	_type: "control",
	x: 0,
	y: 0,
	r: 0,
	throttle: 0
};

// SETUP PYTHON IPC
fc.stdout.setEncoding("ascii");
fc.stdout.on("data",(data) => { //Input from python script
	if(data == "Ready.\n") { //When python reports it is ready...
		fc.stdin.write(JSON.stringify(control)+"\n"); //Send the current state of control
	}
});
function sendControl() {
	fc.stdin.write(json.stringify(control)+"\n");
}

// SETUP STEAM CONTROLLER INPUT
sc.lpad.on("touch",() => {
	control.throttle = 0.085;
	sendControl();
});
sc.lpad.on("untouch",() => {
	control.throttle = 0;
	sendControl();
})
