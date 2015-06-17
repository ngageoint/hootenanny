
fs = require('fs');
xml2js = require('xml2js');

process.stdin.setEncoding('utf8');

function initials(name) {
    var result = '';
    var n = name.split(' ');
    for (i in n)
    {
        result = result + n[i][0];
    }
    return result;
}

function removeFirstLine(str) {
    var lines = str.split("\n");
    lines.splice(0, 1);
    return lines.join("\n");
}

var input = "";
process.stdin.on('data', function(chunk) {
    input = input + chunk;
});
process.stdin.on('end', function() {
    var lines = input.split('\n');
    var revs = []
    for (l in lines)
    {
        // split on very unlikely string
        var e = lines[l].split('|$|');
        if (e.length > 1)
        {
            var r = { revnumber: e[0],
                      date: new Date(Number(e[1]* 1000)).toLocaleDateString(),
                      authorinitials: initials(e[3]),
                      revremark: e[2] };
            revs.push(r);
        }
    }

    var builder = new xml2js.Builder();
    console.log(removeFirstLine(builder.buildObject({revhistory:{revision:revs}})));
    console.log(fs.readFileSync('docinfo.xml').toString());
});
process.stdin.resume();

