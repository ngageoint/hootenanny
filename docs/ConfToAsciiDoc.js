
var fs = require('fs');
var pathToFile = process.argv[2];

var conf = JSON.parse(fs.readFileSync(pathToFile));

var keys = Object.keys(conf).sort();
for (var i in keys)
{
    var c = conf[keys[i]];
    console.log("=== " + keys[i]);
    console.log();
    console.log("* Data Type: " + c.type);
    if (Array.isArray(c.default))
    {
        console.log("* Default Value: ");
        for (var j in c.default)
        {
            console.log("** `" + c.default[j] + "`");
        }
    }
    else
    {
        console.log("* Default Value: `" + c.default + "`");
    }
    console.log();
    console.log(c.description);
    console.log();
}
