#!/usr/bin/node
/**
 * This is a small example that loads, cleans and conflates two files. There
 * is also a little bonus ways splitting on just one of the input files.
 * 
 * This doesn't address a specific use case, but demonstrates some of the 
 * functionality exposed to node.js via hootenanny.
 */
var hoot = require('../lib/HootJs');

var input = process.argv[2]
var output = process.argv[3]

// create a new map and populate it with two input files
var map = new hoot.OsmMap();
hoot.loadMap(map, input, false, 1);

map.visit(new hoot.SplitNameVisitor());

hoot.saveMap(map, output);
