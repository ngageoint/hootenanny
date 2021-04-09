
var assert = require('assert');

var HOOT_HOME = process.env.HOOT_HOME;
var hoot = require(HOOT_HOME + '/lib/HootJs');
hoot.Log.setLogLevel('error');

describe('WayJs', function() {

    it('Should determine the number of nodes in a way', function() {
    
        var map = new hoot.OsmMap();
        hoot.loadMap(map, "test-files/ToyTestA.osm", true, 1);
        var element = hoot.MapUtils.getFirstElementWithNote(map, "0");
        assert.equal(30, element.getNodeCount());

    }).timeout(5000);

});
