#!/usr/bin/node

var HOOT_HOME = process.env.HOOT_HOME
var hoot = require(HOOT_HOME + '/lib/HootJs');

hoot.Settings.set({"api.db.email":"test@test.com"});

var map = new hoot.OsmMap();
hoot.loadMap(map, process.argv[2], false, 1);

map.visit(function(e) {
    if (hoot.OsmSchema.isHgisPoi(e) == false) {
        map.removeElement(e.getElementId());
    }
});

// save the unknown1 elements to one map
hoot.saveMap(map, process.argv[3]);
