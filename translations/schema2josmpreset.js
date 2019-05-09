"use strict";
require('./validation_mocks.js');
var mgcp_schema = require('./mgcp_schema.js').getDbSchema();
var ggdm30_schema = require('./ggdm30_full_schema.js').getDbSchema();
var tds40_schema = require('./tds40_full_schema.js').getDbSchema();
var tds61_schema = require('./tds61_full_schema.js').getDbSchema();
var fs = require('fs');
var builder = require('xmlbuilder');
var crypto = require('crypto');

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
    var listEntries = {};
    var texts = {};
    var combos = {};
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
                columns: i.columns,
                hashes: []
            };

            //Build de-duped map of unique enumerations (list entry),
            //combo and text elements keyed by hash
            i.columns.forEach(col => {
                let colHash = 'hash' + crypto.createHash('md5').update(JSON.stringify(col)).digest('hex').replace(/[0-9]/g, '');
                items[i.desc].hashes.push(colHash);

                if (col.type === 'enumeration') {

                    let key = 'hash' + crypto.createHash('md5').update(JSON.stringify(col.enumerations)).digest('hex').replace(/[0-9]/g, '');

                    if (!listEntries[key])
                        listEntries[key] = col.enumerations;

                    if (!combos[colHash]) {
                        col.enumHash = key;
                        combos[colHash] = col;
                    }

                } else {
                    if (!texts[colHash])
                        texts[colHash] = col;
                }
            });

        }

    });

    //Write the unique list entries out as chunks
    Object.keys(listEntries).forEach(k => {
        let chunk = presets.ele('chunk', {id: k});
        listEntries[k].sort((a,b) => {
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
            chunk.ele('list_entry',
                {
                    value: e.value,
                    //Don't display name if equal to value
                    display_value: (e.value === e.name) ? e.value : e.value + ' - ' + e.name
                });
        });
    });

    //Write the unique combos out as chunks
    Object.keys(combos).forEach(k => {
        let col = combos[k];
        let chunk = presets.ele('chunk', {id: k});
        let attrs = {
            key: col.name,
            text: col.desc,
            values_searchable: true,
            // default: col.defValue
        };

        let combo = chunk.ele('combo', attrs);
        combo.ele('reference', 
            {
                ref: col.enumHash
            });
    });

    //Write the unique texts out as chunks
    Object.keys(texts).forEach(k => {
        let col = texts[k];
        let chunk = presets.ele('chunk', {id: k});
        let attrs = {
            key: col.name,
            text: col.desc,
            // default: col.defValue
        };
        if (col.length) {
            attrs.length = col.length;
        }
        chunk.ele('text', attrs);
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

        items[i].columns.forEach((col, j) => {
            if (col.name !== 'FCODE' && col.name !== 'F_CODE') { //FCODE is set as a key above
                it.ele('reference', 
                    {
                        ref: items[i].hashes[j]
                    });
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
