"use strict";
require('./validation_mocks.js');
var mgcp_schema = require('./mgcp_schema.js').getDbSchema();
var ggdm30_schema = require('./ggdm30_full_schema.js').getDbSchema();
var tds40_schema = require('./tds40_full_schema.js').getDbSchema();
var tds61_schema = require('./tds61_full_schema.js').getDbSchema();
var fs = require('fs');
var stringify = require('json-stable-stringify');


const output = 'tds40_schema.json';
const objs = [tds40_schema];//, tds61_schema, mgcp_schema, ggdm30_schema];

objs.forEach(f => {
    // const one2one = f.one2one.reduce((tagmap, tag) => {
    //     let temp = {};
    //     temp[tag[1]] = tag[2] + '=' + tag[3];
    //     tagmap[tag[0]] = Object.assign({}, tagmap[tag[0]], temp);
    //     return tagmap;
    // }, {});
    // Object.assign(mapping, f.txtBiased, f.numBiased, one2one)

    fs.writeFile(output, stringify(f, {space: 4}), (err) => {
        if (err) throw err;
        console.log(`${output} file has been saved.`);
    });

});

