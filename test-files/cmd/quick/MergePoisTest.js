var HOOT_HOME = process.env.HOOT_HOME;
var hoot = require(HOOT_HOME + '/lib/HootJs');
hoot.Settings.set({"tag.merger.default":"hoot::OverwriteTagMerger"});
var script = 'PoiGeneric.js';

var input1 = "<?xml version='1.0' encoding='UTF-8'?>\
<osm version='0.6' upload='true' generator='JOSM'>\
  <node id='-3200079' visible='true' lat='48.0479399' lon='11.7012814'>\
    <tag k='amenity' v='post_office' />\
    <tag k='error:circular' v='15' />\
    <tag k='hoot:status' v='1' />\
  </node>\
  <node id='-3200083' visible='true' lat='48.04854' lon='11.70194'>\
    <tag k='amenity' v='post_office' />\
    <tag k='error:circular' v='150' />\
    <tag k='name' v='POST, JÄGER-VON-FALL-STRASSE' />\
    <tag k='hoot:status' v='2' />\
  </node>\
</osm>";

var map = new hoot.OsmMap();
hoot.loadMapFromString(map, input1);
var mergedMap = hoot.poiMerge(script, map, -1);
var xml = hoot.OsmWriter.toString(mergedMap);
console.log(xml);

var input2 = "<?xml version='1.0' encoding='UTF-8'?>\
<osm version='0.6' upload='true' generator='JOSM'>\
  <node id='-320' visible='true' lat='12.34567' lon='23.45678'>\
    <tag k='poi' v='yes' />\
    <tag k='error:circular' v='15' />\
    <tag k='hoot:status' v='1' />\
  </node>\
  <node id='-9999' visible='true' lat='12.3456' lon='23.4567'>\
    <tag k='amenity' v='post_office' />\
    <tag k='error:circular' v='150' />\
    <tag k='name' v='Test post office' />\
    <tag k='hoot:status' v='2' />\
  </node>\
  <node id='-999' visible='true' lat='12.345678' lon='23.456789'>\
    <tag k='poi' v='yes' />\
    <tag k='error:circular' v='1' />\
    <tag k='name' v='Test post office - More Complete' />\
    <tag k='hoot:status' v='2' />\
  </node>\
  <node id='-9' visible='true' lat='12.3455' lon='23.4566'>\
    <tag k='amenity' v='post_office' />\
    <tag k='hoot:status' v='2' />\
  </node>\
  <node id='-9090' visible='true' lat='12.3456782' lon='23.4567891'>\
    <tag k='poi' v='yes' />\
    <tag k='error:circular' v='2' />\
    <tag k='hoot:status' v='1' />\
  </node>\
</osm>";

var map = new hoot.OsmMap();
map.setIdGenerator(new hoot.DefaultIdGenerator());
hoot.loadMapFromString(map, input2);
var mergedMap = hoot.poiMerge(script, map, -1);
var xml = hoot.OsmWriter.toString(mergedMap);
console.log(xml);
