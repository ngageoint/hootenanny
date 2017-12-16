"use strict";
require('./validation_mocks.js');
var mgcp_schema = require('./mgcp_schema.js').getDbSchema();
var ggdm30_schema = require('./ggdm30_full_schema.js').getDbSchema();
var tds40_schema = require('./tds40_full_schema.js').getDbSchema();
var tds61_schema = require('./tds61_full_schema.js').getDbSchema();
var fs = require('fs');
var builder = require('xmlbuilder');

// <?xml version="1.0" encoding="UTF-8"?>
// <presets xmlns="http://josm.openstreetmap.de/tagging-preset-1.0">


const objs = {
    tds40: tds40_schema,
    tds61: tds61_schema,
    mgcp: mgcp_schema,
    ggdm30: ggdm30_schema
};

function lookupType(geoms) {
    var types = geoms.reduce((s, g) => {
        switch(g) {
            case 'Point':
                s.add('node');
                break;
            case 'Line':
                s.add('way');
                s.add('closedway');
                break;
            case 'Area':
                s.add('closedway');
                s.add('multipolygon');
                break;
        }
        return s;

    }, new Set());

    return Array.from(types).join(',');
}

Object.keys(objs).forEach(s => {

    //Build a normalized map of items and chunks
    var items = {};
    var chunks = {};
    var groups = require('./facc_groups.json');
    var subgroups = require('./facc_subgroups.json');
    var subGroupCodeElements = {};
    var presets = builder.create('presets');

    objs[s].forEach(i => {
        //Must de-dup the items and build up the geom array
        if (items[i.desc]) {
            items[i.desc].geoms.push(i.geom);
        } else {
            items[i.desc] = {
                fcode: i.fcode,
                geoms: [i.geom],
                columns: i.columns.map(c => c.name)
            };
        }

        //Build de-duped map of columns
        i.columns.forEach(c => {
            if (!chunks[c.name]) {
                chunks[c.name] = c;
            }
        });

    });

    Object.keys(chunks).forEach(c => {
        let chunk = presets.ele('chunk', {id: c});
        if (chunks[c].type === 'enumeration') {
            let attrs = {
                key: chunks[c].name, 
                text: chunks[c].desc, 
                values_searchable: true, 
                default: chunks[c].defValue
            };

            if (chunks[c].length) {
                attrs.length = chunks[c].length;

            }
            let combo = chunk.ele('combo', attrs);
            chunks[c].enumerations.forEach(e => {
                combo.ele('list_entry', 
                    {value: e.value, 
                        display_value: e.value + ' - ' + e.name
                    });
            })
        } else {
            let attrs = {
                key: chunks[c].name, 
                text: chunks[c].desc, 
                default: chunks[c].defValue
            };
            if (chunks[c].length) {
                attrs.length = chunks[c].length;

            }
            chunk.ele('text', attrs);
        }
    });

    let schemaGroup = presets.ele('group', {name: s.toUpperCase()});
    Object.keys(groups).forEach(g => {
        let group = schemaGroup.ele('group', {name: groups[g]});
        Object.keys(subgroups).filter(s => {
            return s.charAt(0) == g;
        }).forEach(s => {
            let sub = group.ele('group', {name: subgroups[s]});
            subGroupCodeElements[s] = sub; //use this to find the subgroup element to stick the items
        });
    });

    Object.keys(items).forEach(i => {
        //Get the right subgroup element to append to
        let code = items[i].fcode.slice(0,2);

        let it = subGroupCodeElements[code].ele('item', {name: i, type: lookupType(items[i].geoms), preset_name_label: true});

        it.ele('key', {key: 'FCODE', value: items[i].fcode});

        items[i].columns.forEach(col => {
            it.ele('reference', {ref: col});
        });
    });

    let xml = presets.end({pretty: true});

    let output = s + '_preset.xml';
    fs.writeFile(output, xml, (err) => {
        if (err) throw err;
        console.log(`${output} file has been saved.`);
    });

});
