// This script depends on:
// npm install json-stable-stringify
"use strict";
require('./fcode_common.js');
require('./validation_mocks.js');
require('./mgcp_rules.js');
require('./ggdm30_rules.js');
require('./tds40_rules.js');
require('./tds61_rules.js');
var fs = require('fs');
var stringify = require('json-stable-stringify');


const output = 'validation_mappings.json';
const objs = [fcodeCommon, tds.rules, tds61.rules, mgcp.rules, ggdm30.rules];
const mapping = {};

objs.forEach(f => {
    const one2one = f.one2one.reduce((tagmap, tag) => {
        let temp = {};
        temp[tag[1]] = tag[2] + '=' + tag[3];
        tagmap[tag[0]] = Object.assign({}, tagmap[tag[0]], temp);
        return tagmap;
    }, {});
    Object.assign(mapping, f.txtBiased, f.numBiased, one2one)
});

fs.writeFile(output, stringify(mapping, {space: 4}), (err) => {
    if (err) throw err;
    console.log(`${output} file has been saved.`);
});
