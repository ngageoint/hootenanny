#!/usr/bin/node

/* This is useful for debugging ElementMergeServer.js issues until #2231 is fixed. */

var HOOT_HOME = process.env.HOOT_HOME
var hoot = require(HOOT_HOME + '/lib/HootJs');
hoot.Log.setLogLevel('trace');
hoot.Settings.set({"log.class.include.filter":"PoiPolygonMerger;ElementMergerJs;PreserveTypesTagMerger"});

// Don't forget to replace " with '
var data = "<osm version='0.6' upload='true' generator='hootenanny'><way id='167' version='2'><nd ref='1524'/><nd ref='1525'/><nd ref='1526'/><nd ref='1527'/><nd ref='1528'/><nd ref='1529'/><nd ref='1530'/><nd ref='1531'/><nd ref='1532'/><nd ref='1533'/><nd ref='1534'/><nd ref='1535'/><nd ref='1536'/><nd ref='1524'/><tag k='addr:full' v='Al-Rabwah, Jeddah 23448, Saudi Arabia'/><tag k='amenity' v='place_of_worship'/><tag k='attribution' v='osm'/><tag k='building' v='yes'/><tag k='error:circular' v='15'/><tag k='hoot:poipolygon:poismerged' v='1'/><tag k='hoot:status' v='3'/><tag k='license' v='This data is made available under the Open Database License: http://opendatacommons.org/licenses/odbl/1.0/.'/><tag k='name' v='Sherbalty Mosque'/><tag k='religion' v='muslim'/><tag k='security:classification' v='UNCLASSIFIED'/><tag k='source' v='DigitalGlobe;osm'/><tag k='source:datetime' v='2017-04-15T22:17:51Z'/><tag k='source:imagery:datetime' v='2017-03-21 08:14:26'/><tag k='source:imagery:id' v='15b60aa1243d556e0232bc21a85aa3f9'/><tag k='source:imagery:sensor' v='WV03_VNIR'/><tag k='source:ingest:datetime' v='2019-04-15T18:06:46.626Z'/><tag k='uuid' v='{b0654ffa-b479-41bb-a3b9-5140230350c4}'/><tag k='hoot:merge:target' v='yes'/></way><node id='-103' lon='39.190346' lat='21.58882699999999' version='1'><tag k='addr:full' v='Al-Rabwah, Jeddah 23448, Saudi Arabia'/><tag k='amenity' v='place_of_worship'/><tag k='building' v='mosque'/><tag k='error:circular' v='15'/><tag k='hoot:status' v='Input2'/><tag k='name' v='Sherbatly mosque'/><tag k='name:ar' v='جامع الشربتلي'/><tag k='religion' v='muslim'/><tag k='source:ingest:datetime' v='2019-04-15T18:06:46.630Z'/></node></osm>";

var map = new hoot.OsmMap();
hoot.loadMapFromStringPreserveIdAndStatus(map, data);
var mergedMap = hoot.mergeElements(map);
var mergedMapStr = hoot.OsmWriter.toString(mergedMap);
console.log("\n\nOutput:\n\n" + mergedMapStr);
