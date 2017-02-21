#!/usr/bin/node
/**
 * Prints all the tags that are in a given category.
 * E.g.
 *    scripts/DisplayCategory.js hgispoi --small
 *
 * If --small is specified then any tag with a wildcard in that category will
 * cause all other tags in that category to be surpressed. This may be a bit
 * misleading if there are specific tags that use that key that are omitted
 * from the category.
 */
var hoot = require('../lib/HootJs');

var category = process.argv[2]
var small = process.argv[3]

if (small == '--small') {
    small = true;
} else {
    small = false;
}

var tvs = hoot.OsmSchema.getTagByCategory(category);

var wildCards = [];

for (i in tvs) {
    var tv = tvs[i];
    if (tv.value == '*') {
        wildCards.push(tv.key);
    }
}

for (i in tvs) {
    var tv = tvs[i];
    var str = tv.name;
    if (tv.description != '') {
        str = tv.name + ": " + tv.description;
    }
    if (small) {
        if (wildCards.indexOf(tv.key) > -1) {
            if (tv.value == '*') {
                console.log(str);
            }
        } else if (tv.value != '') {
            console.log(str);
        }
    } else if (tv.value != '') {
        console.log(str);
    }
}
