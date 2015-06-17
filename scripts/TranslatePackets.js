/**
 * 
 */
var HOOT_HOME = process.env.HOOT_HOME
var hoot = require(HOOT_HOME + '/lib/HootJs');
var util = require('util');
var JSONParseStream = require("./JSONParseStream.js");

function write(obj) {
    process.stdout.write(JSON.stringify(obj) + "\n");
}

var tran = {}

function getTranslationOp(script, direction) {
    var key = script + '###' + direction;
    if (key in tran == false) {
        tran[key] = new hoot.TranslationOp({
            'translation.script':script,
            'translation.direction':direction
        });
    }
    return tran[key];
}


var jp = JSONParseStream.JSONParseStream({objectMode:true});
process.stdin.pipe(jp);
jp.on('readable', function() {
    var msg;
    while (msg = jp.read()) {
        var start = new Date().getTime();
        var result = {};

        if (msg.uid) {
            result.uid = msg.uid;
        }
        if (msg.command) {
            result.command = msg.command;
        }

        if (msg.command == 'translate') {
            var map = new hoot.OsmMap();
            hoot.loadMapFromString(map, msg.input);
            var tran = getTranslationOp(msg.script, msg.direction);
            tran.apply(map);
            xml = hoot.OsmWriter.toString(map);
            result.output = xml;
        } else {
            result.status = -1;
            result.error = 'Unrecognized command';
        }

        result.elapsed = new Date().getTime() - start;

        write(result);
    }
});



