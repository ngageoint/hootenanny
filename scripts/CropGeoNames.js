#!/usr/bin/nodejs

var fs = require('fs');
var readline = require('readline');

if (process.argv.length == 2)
{
    console.log("Crops a given input GeoNames.org file to the given bounds.");
    console.log("Usage: " + process.argv.join(' ') +
        " <input> <output> <minx,miny,maxx,maxy>");
    process.exit(-1);
}

var bounds = process.argv[4].split(',');
bounds = bounds.map(function(v) { return Number(v); });

var rd = readline.createInterface({
    input: fs.createReadStream(process.argv[2]),
    output: process.stdout,
    terminal: false
});

var LAT=4;
var LON=5;

fd = fs.openSync(process.argv[3], 'w');
rd.on('line', function(line) {
    cols = line.split('\t');
    var lat = cols[LAT];
    var lon = cols[LON];

    if (lon < bounds[0] || lon > bounds[2] ||
        lat < bounds[1] || lat > bounds[3])
    {
        // pass
    } else {
        fs.write(fd, line + "\n");
    }
});
