var assert = require('assert');

var HOOT_HOME = process.env.HOOT_HOME;
var hoot = require(HOOT_HOME + '/lib/HootJs');

hoot.Log.setLogLevel('warn');

describe('schema', function() {
    it('tests the ElementVisitor', function() {

        var map = new hoot.OsmMap();

        hoot.loadMapFromString(map,            
            '<?xml version="1.0" encoding="UTF-8"?>\
            <osm version="0.6" generator="hootenanny" srs="+epsg:4326">\
                <bounds minlat="38.6114872" minlon="-103.485378" maxlat="38.623341" maxlon="-103.4850048"/>\
                <node visible="true" id="177472178" timestamp="2015-05-16T14:17:23Z" version="3" changeset="31202528" user="Mark Newnham" uid="994480" lat="38.6114871999999991" lon="-103.4850047999999987"/>\
                <node visible="true" id="177472181" timestamp="2009-09-01T09:03:52Z" version="2" changeset="2334462" user="woodpeck_fixbot" uid="147510" lat="38.6233410000000035" lon="-103.4853779999999972"/>\
                <way visible="true" id="345550388" timestamp="2015-05-16T14:17:21Z" version="1" changeset="31202528" user="Mark Newnham" uid="994480">\
                    <nd ref="177472178"/>\
                    <nd ref="177472181"/>\
                    <tag k="name" v="Delete Road"/>\
                    <tag k="highway" v="tertiary"/>\
                </way>\
                <way visible="true" id="345550390" timestamp="2015-05-16T14:17:21Z" version="1" changeset="31202528" user="Mark Newnham" uid="994480">\
                <nd ref="177472178"/>\
                <nd ref="177472181"/>\
                <tag k="name" v="Add Ref Road"/>\
                <tag k="surface" v="asphalt"/>\
            </way>\
            </osm>'
        , true);

        map.visit(
            new hoot.FilteredVisitor(
                new hoot.TagCriterion({"tag.criterion.kvps":"name=Add Ref Road"}),
                new hoot.AddRef1Visitor({"add.ref1.visitor.prefix":"poi-poly-"})
            )
        );

        map.visit(
            new hoot.RemoveElementsVisitor( 
                new hoot.TagCriterion({'tag.criterion.kvps':'name=Delete Road'}),
                {'remove.elements.visitor.recursive':false}));

        assert.equal(map.getElementCount(), 3);

        // debug: hoot.saveMap(map, "result.osm");

    }).timeout(5000);
});
