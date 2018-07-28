//NOTE: Higher-level computing for drone control
let spawn = require("child_process").spawn;
let path = require("path");
let projectDir = path.join(__dirname,"..");

let fc = spawn("taskset",["-c","3","python3","python/main.py"],{cwd: projectDir});

let control = {
	x: 0,
	y: 0,
	throttle: 0.085
};

fc.stdout.on("data",(data) => {
	data = String(data);
	console.log(data);
	if(data == "Ready.\n") {
		fc.stdin.write(JSON.stringify(control)+"\n");
	}
});
