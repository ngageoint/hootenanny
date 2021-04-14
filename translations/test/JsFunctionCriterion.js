
var assert = require('assert');

var HOOT_HOME = process.env.HOOT_HOME;
var hoot = require(HOOT_HOME + '/lib/HootJs');
hoot.Log.setLogLevel('warn');

describe('JsFunctionCriterion', function() {

    it('Should evaluate a function criterion', function() {
    
        // When last checked, js function criterion use could only be found in the regression tests.
    
        var poi = new hoot.PoiCriterion();
        function isNotPoiNode(e) {
          var poiNode = e.getElementId().getType() === "Node" &&
          poi.isSatisfied(e);
          return !poiNode;
       }
    
        var map = new hoot.OsmMap();
        hoot.loadMap(map, HOOT_HOME + "/test-files/conflate/unified/AllDataTypesA.osm", true, 1);
        // Remove everything but POIs from the map.
        new hoot.RefRemoveOp(isNotPoiNode).apply(map);
        
        assert.equal(11, map.getElementCount());

    }).timeout(5000);

});
