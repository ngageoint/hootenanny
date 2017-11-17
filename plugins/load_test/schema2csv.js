"use strict";
require('../validation_mocks.js');
require('../mgcp_schema.js');
require('../ggdm30_schema.js');
require('../tds40_schema.js');
require('../tds61_schema.js');
var fs = require('fs');


var ws = fs.createWriteStream('schema.csv');
mgcp.schema.getDbSchema().forEach(s => {
    ws.write(s.fcode + ',' + s.geom + '\n');
});
ws.end();

