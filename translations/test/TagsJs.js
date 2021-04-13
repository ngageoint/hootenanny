
var assert = require('assert');

var HOOT_HOME = process.env.HOOT_HOME;
var hoot = require(HOOT_HOME + '/lib/HootJs');
hoot.Log.setLogLevel('warn');

describe('TagsJs', function() {

   it('Should determine the number of informational tags', function() {
    
        var map = new hoot.OsmMap();
        hoot.loadMap(map, HOOT_HOME + "/test-files/ToyTestA.osm", true, 1);
        var element = hoot.MapUtils.getFirstElementWithNote(map, "0");
        var tags = element.getTags();
        assert.equal(2, tags.getInformationCount());

    }).timeout(5000);

});
