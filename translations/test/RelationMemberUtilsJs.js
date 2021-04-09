
var assert = require('assert');

var HOOT_HOME = process.env.HOOT_HOME;
var hoot = require(HOOT_HOME + '/lib/HootJs');
hoot.Log.setLogLevel('warn');

describe('RelationMemberUtilsJs', function() {

    it('Should determine if a relation member is a member of a relation with a specific type', function() {
    
        var map = new hoot.OsmMap();
        hoot.loadMap(map, HOOT_HOME + "/test-files/criterion/ComplexBuildings.osm", true, 1);
        var element = hoot.MapUtils.getFirstElementWithTag(map, "name", "Panera Bread", "Way");
        
        var isMember = 
          hoot.RelationMemberUtils.isMemberOfRelationWithType(
            map, element.getElementId(), "building");
        assert.equal(true, isMember);
        
        isMember = 
          hoot.RelationMemberUtils.isMemberOfRelationWithType(map, element.getElementId(), "blah");
        assert.equal(false, isMember);

    }).timeout(5000);
    
    it('Should determine if a relation member is a member of a relation with a specific tag key', function() {
    
        var map = new hoot.OsmMap();
        hoot.loadMap(map, HOOT_HOME + "/test-files/criterion/ComplexBuildings.osm", true, 1);
        var element = hoot.MapUtils.getFirstElementWithTag(map, "name", "Panera Bread", "Way");
        
        var isMember = 
          hoot.RelationMemberUtils.isMemberOfRelationWithTagKey(
            map, element.getElementId(), "hoot:building:match");
        assert.equal(true, isMember);
        
        isMember = 
          hoot.RelationMemberUtils.isMemberOfRelationWithTagKey(
            map, element.getElementId(), "blah");
        assert.equal(false, isMember);

    }).timeout(5000);

});
