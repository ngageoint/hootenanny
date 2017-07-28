#!/usr/bin/node

var input = process.argv[2]
var output = process.argv[3]

var HOOT_HOME = process.env.HOOT_HOME
var hoot = require(HOOT_HOME + '/lib/HootJs');

// create a new map and populate it with two input files
hoot.debug("Loading map...");
var map = new hoot.OsmMap();
hoot.loadMap(map, input, false, 1);

// if it is not a POI
var poi = new hoot.PoiCriterion();
var not = new hoot.NotCriterion(poi);

// remove the feature from the map
var rro = new hoot.RefRemoveOp(not);
hoot.debug("Removing features from the map...");
rro.apply(map);

hoot.debug("Saving map...");
hoot.saveMap(map, output);
