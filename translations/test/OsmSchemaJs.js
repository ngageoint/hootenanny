
var assert = require('assert');

var HOOT_HOME = process.env.HOOT_HOME;
var hoot = require(HOOT_HOME + '/lib/HootJs');
hoot.Log.setLogLevel('warn');

describe('OsmSchemaJs', function() {

    it('Should determine if an element has a generic type', function() {
    
        var map = new hoot.OsmMap();
        hoot.loadMap(map, HOOT_HOME + "/test-files/ToyTestA.osm", true, 1);
        var element = hoot.MapUtils.getFirstElementWithNote(map, "0");
        var isGeneric = hoot.OsmSchema.isGeneric(element);
        assert.equal(false, isGeneric);

    }).timeout(5000);

});
