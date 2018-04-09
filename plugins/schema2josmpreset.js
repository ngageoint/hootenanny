"use strict";
require('./validation_mocks.js');
var mgcp_schema = require('./mgcp_schema.js').getDbSchema();
var ggdm30_schema = require('./ggdm30_full_schema.js').getDbSchema();
var tds40_schema = require('./tds40_full_schema.js').getDbSchema();
var tds61_schema = require('./tds61_full_schema.js').getDbSchema();
var fs = require('fs');
var builder = require('xmlbuilder');
//These fields in translated schemas are shared across FCODEs but
//each has their own unique domain
var uniqueSharedFields = ['FFN', 'FFN2', 'FFN3'];

// <?xml version="1.0" encoding="UTF-8"?>
// <presets xmlns="http://josm.openstreetmap.de/tagging-preset-1.0">


const objs = {
    tds40: {
        schema: tds40_schema,
        name: 'TDSv40',
        icon: 'presets/misc/nga_logo.png'
    },
    tds61: {
        schema: tds61_schema,
        name: 'TDSv61',
        icon: 'presets/misc/nga_logo.png'
    },
    mgcp: {
        schema: mgcp_schema,
        name: 'MGCP',
        icon: 'presets/misc/MGCP_logo.png'
    },
    ggdm30: {
        schema: ggdm30_schema,
        name: 'GGDMv30',
        icon: 'presets/misc/usace_logo.png'
    },
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
    var groupCodeElements = {};
    var subGroupCodeElements = {};
    var presets = builder.create('presets');

    objs[s].schema.forEach(i => {
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
            var id = c.name;
            //unique shared fields with have fcode in chunk id
            if (uniqueSharedFields.some(f => { return f === id; })) {
                id = `${id}_${i.fcode}`;
            }
            if (!chunks[id]) {
                chunks[id] = c;
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
                // default: chunks[c].defValue
            };

            if (chunks[c].length) {
                attrs.length = chunks[c].length;

            }
            let combo = chunk.ele('combo', attrs);
            chunks[c].enumerations.sort((a,b) => {
                // numeric comparison
                if (!isNaN(a.value) && !isNaN(b.value)) {
                    return a.value - b.value;
                }

                // case-insensitive string comparison
                var stringA = a.value.toUpperCase();
                var stringB = b.value.toUpperCase();
                if (stringA < stringB) {
                    return -1;
                }
                if (stringA > stringB) {
                    return 1;
                }
                return 0;

            }).forEach(e => {
                combo.ele('list_entry',
                    {
                        value: e.value,
                        //Don't display name if equal to value
                        display_value: (e.value === e.name) ? e.value : e.value + ' - ' + e.name
                    });
            })
        } else {
            let attrs = {
                key: chunks[c].name,
                text: chunks[c].desc,
                // default: chunks[c].defValue
            };
            if (chunks[c].length) {
                attrs.length = chunks[c].length;

            }
            chunk.ele('text', attrs);
        }
    });

    let schemaGroup = presets.ele('group', {name: objs[s].name, icon: objs[s].icon});
    Object.keys(groups).forEach(g => {
        let group = schemaGroup.ele('group', {name: groups[g].title, icon: groups[g].icon});
        groupCodeElements[g] = group;
        Object.keys(subgroups).filter(s => {
            return s.charAt(0) == g;
        }).forEach(s => {
            let sub = group.ele('group', {name: subgroups[s].title, icon: subgroups[s].icon});
            subGroupCodeElements[s] = sub; //use this to find the subgroup element to stick the items
        });
    });

    let usedGroups = new Set(); //Keep a list of used sub groups

    Object.keys(items).forEach(i => {
        //Get the right subgroup (two-letter) element to append to
        let code = items[i].fcode.slice(0,2);

        let it = subGroupCodeElements[code].ele('item', {name: i, type: lookupType(items[i].geoms), preset_name_label: true});

        usedGroups.add(code); //Sub-group
        usedGroups.add(code.charAt(0)); //Group

        it.ele('key', {key: (s === 'mgcp') ? 'FCODE' : 'F_CODE', value: items[i].fcode}); //MGCP uses 'FCODE' as key

        items[i].columns.forEach(col => {
            if (col !== 'FCODE' && col !== 'F_CODE') { //FCODE is set as a key above
                var refval = col;
                //unique shared fields with have fcode in chunk id
                if (uniqueSharedFields.some(f => { return f === col; })) {
                    refval = `${col}_${items[i].fcode}`;
                }
                it.ele('reference', {ref: refval});
            }
        });
    });

    //Remove empty groups
    Object.keys(subGroupCodeElements).forEach(sg => {
        if (!usedGroups.has(sg))
            subGroupCodeElements[sg].remove();
    });
    Object.keys(groupCodeElements).forEach(g => {
        if (!usedGroups.has(g))
            groupCodeElements[g].remove();
    });

    let xml = presets.end({pretty: true});

    let output = s + '_preset.xml';
    fs.writeFile(output, xml, (err) => {
        if (err) throw err;
        console.log(`${output} file has been saved.`);
    });

});
