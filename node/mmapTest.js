let fs = require("fs");
let mmap = require("/home/shaun/.nvm/versions/node/v8.9.4/lib/node_modules/mmap.js");

const fd = fs.openSync("/run/user/1000/mmapTest","r+");
let fileBuf = mmap.alloc(
    2048,
    mmap.PROT_READ | mmap.PROT_WRITE,
    mmap.MAP_SHARED,
    fd,
    0);

fileBuf.writeInt16LE(Number(process.argv[2]),0);
