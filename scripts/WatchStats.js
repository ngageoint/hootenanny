#!/usr/bin/node

var child = require('child_process');
var process = require('process');

var commandDone = false;
var netStatDone = false;
var ramDone = false;
var result = 0;
var highestTcpCount = 0;
var highestRam = 0;
var MAX_TCP_COUNT = 2000;
var MAX_RAM = 8000000;

console.time('Elapsed');

function done() {
    if (commandDone && netStatDone && ramDone) {
        if (highestTcpCount > MAX_TCP_COUNT) {
            result = 1;
        }
        if (highestRam > MAX_RAM) {
            result = 1;
        }
        console.log("Highest TCP Count: " + highestTcpCount);
        console.log("Highest RAM: " + highestRam);
        console.timeEnd('Elapsed');
        process.exit(result);
    }
}

function checkNetStat(error, stdout, stderr) {
    var tcpCount = parseInt(stdout);
    if (highestTcpCount == 0) {
        console.log("Starting TCP Count: " + tcpCount);
    }
    highestTcpCount = Math.max(tcpCount, highestTcpCount);
    netStatDone = true;
    done();
}

function checkRamStat(error, stdout, stderr) {
    var s = stdout.split(/\s+/);
    var ram = parseInt(s[16]);
    if (highestRam == 0) {
        console.log("Starting RAM: " + ram);
    }
    highestRam = Math.max(highestRam, s[16]);
    ramDone = true;
}

child.exec(process.argv[2], function(error, stdout, stderr) {
    if (error) {
        console.log("Error running command.");
        console.log(error);
        console.log(stdout);
        console.log(stderr);
        result = 1;
    } else {
        console.log(stdout);
    }
    commandDone = true;
    done();
});

child.exec("netstat -t | wc -l", checkNetStat);
child.exec("free", checkRamStat);
setInterval(function() {
    child.exec("netstat -t | wc -l", checkNetStat);
}, 5000);
setInterval(function() {
    child.exec("free", checkRamStat);
}, 5000);
