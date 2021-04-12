
var assert = require('assert');

var HOOT_HOME = process.env.HOOT_HOME;
var hoot = require(HOOT_HOME + '/lib/HootJs');
hoot.Log.setLogLevel('warn');

describe('ElementCriterionJs', function() {

    it('Should see if a criterion is satisfied', function() {
    
        // remove all buildings; When last checked, js criterion usage could only be found in the regression tests.
        
        var map = new hoot.OsmMap();
        hoot.loadMap(map, HOOT_HOME + "/test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon1.osm", true, 1);
        var building = new hoot.BuildingCriterion(map);
        var buildingWayNode = new hoot.BuildingWayNodeCriterion(map);
        var buildingOrBuildingWayNode = new hoot.OrCriterion(building, buildingWayNode);
        var elementRemover = new hoot.RefRemoveOp(buildingOrBuildingWayNode);
        elementRemover.apply(map);
        
        // check the output
        //hoot.saveMap(map, HOOT_HOME + "/test-output/ElementCriterionJs.osm");
        
        assert.equal(1042, map.getElementCount());  

    }).timeout(5000);

});
