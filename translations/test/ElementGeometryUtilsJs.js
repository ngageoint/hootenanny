
var assert = require('assert');

var HOOT_HOME = process.env.HOOT_HOME;
var hoot = require(HOOT_HOME + '/lib/HootJs');
hoot.Log.setLogLevel('warn');

describe('ElementGeometryUtilsJs', function() {

    it('Should calculate way length', function() {
    
        var map = new hoot.OsmMap();
        hoot.loadMap(map, HOOT_HOME + "/test-files/ToyTestA.osm", true, 1);
        hoot.MapProjector.projectToPlanar(map);
        var element = hoot.MapUtils.getFirstElementWithNote(map, "0");
        var length = hoot.ElementGeometryUtils.calculateLength(map, element)
        assert.equal(881.3103894391232, length);

    }).timeout(5000);

});
