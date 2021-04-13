
var assert = require('assert');

var HOOT_HOME = process.env.HOOT_HOME;
var hoot = require(HOOT_HOME + '/lib/HootJs');
hoot.Log.setLogLevel('warn');

describe('OsmMapJs', function() {

    it('Should clone a map', function() {
    
        // When last checked, js map cloning could only found in the regression tests.
    
        var map = new hoot.OsmMap();
        hoot.loadMap(map, HOOT_HOME + "/test-files/ToyTestA.osm", true, 1);
        var mapCopy = map.clone();
        
        // check the output
        //hoot.saveMap(map, HOOT_HOME + "/test-output/OsmMapJs.osm");
        
        // Oddly, this isn't true.
        assert.equal(map.getElementCount(), mapCopy.getElementCount());

    }).timeout(5000);

});
