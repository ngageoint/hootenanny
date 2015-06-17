/**
 * This is a small example that loads, cleans and conflates two files. There
 * is also a little bonus ways splitting on just one of the input files.
 * 
 * This doesn't address a specific use case, but demonstrates some of the 
 * functionality exposed to node.js via hootenanny.
 */
var HOOT_HOME = process.env.HOOT_HOME                                           
var hoot = require(HOOT_HOME + '/lib/HootJs');                                  

// print hello world to gain confidence
console.log(hoot.hello());

// create a new map and populate it with two input files
var map = new hoot.OsmMap();
hoot.loadMap(map, "test-files/ToyTestA.osm", false, 1);

// clean the data
new hoot.MapCleaner().apply(map);

// artificially make the ways short in the first input
new hoot.WaySplitterOp({'way.splitter.max.length' : 20}).apply(map);

// load the second input, its ways aren't shortened.
hoot.loadMap(map, "test-files/ToyTestB.osm", false, 2);

// conflate
new hoot.UnifyingConflator().apply(map)

// write the map once w/ debug info
hoot.set({'writer.include.debug': true});
hoot.saveMap(map, "../tmp/ConflatedDebug.osm");

// write the map a second time without debug info
hoot.set({'writer.include.debug': false});
hoot.saveMap(map, "../tmp/Conflated.osm");
