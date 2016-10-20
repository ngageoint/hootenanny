var HOOT_HOME = process.env.HOOT_HOME;
var assert = require('assert');
if (typeof hoot === 'undefined') {
hoot = require(HOOT_HOME + '/lib/HootJs');
}
if (typeof hoot !== 'undefined') {
var emgcp_osm = require('../emgcp_osm.js');
var emgcp_rules = require('../emgcp_rules.js');
var mgcp_schema = require('../mgcp_schema.js');
var etds40_osm = require('../etds40_osm.js');
var etds40_rules = require('../etds40_rules.js');
var tds40_schema = require('../tds40_schema.js');
var etds61_osm = require('../etds61_osm.js');
var etds61_rules = require('../etds61_rules.js');
var tds61_schema = require('../tds61_schema.js');
}

describe('compare translation methods', function() {

    it('should be lossy to go from osm -> mgcp -> osm', function() {

        var osm_xml = '<osm version="0.6" upload="true" generator="JOSM"><node id="-4" lon="-105.24014094121263" lat="39.28928610944744" version="0"><tag k="poi" v="yes"/><tag k="amenity" v="cafe"/><tag k="uuid" v="{4632d15b-7c44-4ba1-a0c4-8cfbb30e39d4}"/></node></osm>';

        var toMgcp = new hoot.TranslationOp({
            'translation.script': HOOT_HOME + '/translations/OSM_to_englishMGCP.js',
            'translation.direction': 'toogr'});

        var map = new hoot.OsmMap();
        hoot.loadMapFromString(map, osm_xml);
        toMgcp.apply(map);
        var mgcp_xml = hoot.OsmWriter.toString(map);

        var osm_json = emgcp_osm.toOSM({
            "Feature Code": "AL015:General Building",
            "Feature Function": "Restaurant",
            "MGCP Feature universally unique identifier": "4632d15b-7c44-4ba1-a0c4-8cfbb30e39d4"
        }, '', 'Point');

        var expected = {
            "building": "yes",
            "uuid": "{4632d15b-7c44-4ba1-a0c4-8cfbb30e39d4}",
            "amenity": "restaurant"
        };

        //Assert that poi=yes, amenity=cafe become building=yes, amenity=restaurant
        assert.equal(JSON.stringify(osm_json.attrs), JSON.stringify(expected));
    });

    it('should translate from osm -> mgcp', function() {

        var osm = {
            "poi": "yes",
            "place": "town",
            "name": "Manitou Springs",
            "uuid": "{4632d15b-7c44-4ba1-a0c4-8cfbb30e39d4}"
        };

        // Test the conversion of osm to emgcp without using hoot to translate an osm map
        var osm2mgcp = emgcp.toEnglish(osm, '', 'Point');

        var expected = { 'Feature Code': 'AL020:Built-Up Area',
                         Name: 'Manitou Springs',
                         'MGCP Feature universally unique identifier': '4632d15b-7c44-4ba1-a0c4-8cfbb30e39d4',
                         'Associated Text': '<OSM>{"poi":"yes"}</OSM>',
                         'Functional Use': 'Other'
                    };

        assert.equal(JSON.stringify(osm2mgcp.attrs), JSON.stringify(expected));
    });

//Comment out for now due to:
//     TypeError: Cannot call method 'getDbSchema' of undefined
    // it('should translate from osm -> tds61', function() {

    //     var osm = {
    //         "poi": "yes",
    //         "place": "town",
    //         "name": "Manitou Springs",
    //         "uuid": "{4632d15b-7c44-4ba1-a0c4-8cfbb30e39d4}"
    //     };

    //     // Test the conversion of osm to emgcp without using hoot to translate an osm map
    //     var osm2tds61 = etds61.toEnglish(osm, '', 'Point');

    //     var expected = { 'Feature Code': 'AL020:Built-up Area',
    //                      'Unique Entity Identifier': '4632d15b-7c44-4ba1-a0c4-8cfbb30e39d4',
    //                      'Geographic Name Information : Full Name': 'Manitou Springs',
    //                      'Note : Memorandum': '<OSM>{"poi":"yes"}</OSM>'
    //                 };

    //     assert.equal(JSON.stringify(osm2tds61.attrs), JSON.stringify(expected));
    // });

    // it('should translate from osm -> tds40', function() {

    //     var osm = {
    //         "poi": "yes",
    //         "place": "town",
    //         "name": "Manitou Springs",
    //         "uuid": "{4632d15b-7c44-4ba1-a0c4-8cfbb30e39d4}"
    //     };

    //     // Test the conversion of osm to emgcp without using hoot to translate an osm map
    //     var osm2tds40 = etds40.toEnglish(osm, '', 'Point');

    //     var expected = { 'Feature Code': 'AL020:Built-up Area',
    //                      'Unique Entity Identifier': '4632d15b-7c44-4ba1-a0c4-8cfbb30e39d4',
    //                      'Geographic Name Information : (first) Full Name': 'Manitou Springs',
    //                      'Note : Memorandum': '<OSM>{"poi":"yes"}</OSM>'
    //                 };

    //     assert.equal(JSON.stringify(osm2tds40.attrs), JSON.stringify(expected));
    // });

});
