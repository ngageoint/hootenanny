var assert = require('assert'),
    http = require('http'),
    xml2js = require('xml2js'),
    fs = require('fs'),
    httpMocks = require('node-mocks-http'),
    osmtogeojson = require('osmtogeojson'),
    DOMParser = new require('@xmldom/xmldom').DOMParser
    parser = new DOMParser();

var server = require('../TranslationServer.js');

describe('Service Road', function () {
    this.timeout(5000);

    // Strings used to build OSM XML features
    var startNode = '<osm version="0.6" upload="true" generator="hootenanny">\
       <node id="-13" lat="0.68270797876" lon="18.45141400736" >';

    var startWay = '<osm version="0.6" upload="true" generator="hootenanny">\
       <node id="-10" lat="0.68307256979" lon="18.45073925651" /> <node id="-13" lat="0.68270797876" lon="18.45141400736" />\
       <way id="-19" >\
       <nd ref="-10" /> <nd ref="-13" />';

    var startArea = '<osm version="0.6" upload="true" generator="hootenanny">\
       <node id="-10" lat="0.68307256979" lon="18.45073925651" /> <node id="-11" lat="0.68341620728" lon="18.45091527847" />\
       <node id="-12" lat="0.68306209303" lon="18.45157116983" /> <node id="-13" lat="0.68270797876" lon="18.45141400736" />\
       <way id="-19" >\
       <nd ref="-10" /> <nd ref="-11" /> <nd ref="-12" /> <nd ref="-13" /> <nd ref="-10" />';

    var endNode = '</node></osm>';
    var endWay = '</way></osm>'; // NOTE: This is also for Areas as well

    var schemas = [
        // 'TDSv71',
        // 'TDSv70',
        // 'TDSv61',
        // 'TDSv40',
        // 'MGCP', // Different F_CODE and uuid
        'GGDMv30'
    ];

    schemas.forEach(schema => {
        var fcode_key = (schema === 'MGCP') ? 'FCODE' : 'F_CODE';

        it('should handle translation of power line feature from ' + schema + ' -> osm -> ' + schema, function() {
            var data = startWay + '<tag k="highway" v="service"/><tag k="name" v="Frithwood Lane"/><tag k="surface" v="paved"/><tag k="tracktype" v="grade1"/>' + endWay;
            var trans_xml = server.handleInputs({osm: data, method: 'POST', translation: schema, path: '/translateTo'})
            console.log(trans_xml);
            var xml = parser.parseFromString(trans_xml);

            var tags = osmtogeojson(xml).features[0].properties;
            assert.equal(tags[fcode_key], 'AP030');
            // assert.equal(tags['CAB'], '6');
            // assert.equal(tags['UFI'], 'd7cdbdfe-88c6-4d8a-979d-ad88cfc65ef1');

            var osm_xml = server.handleInputs({osm: trans_xml, method: 'POST', translation: schema, path: '/translateFrom'});
            console.log(osm_xml);
            xml = parser.parseFromString(osm_xml);
            tags = osmtogeojson(xml).features[0].properties;
            assert.equal(tags['highway'], 'service');
            assert.equal(tags['surface'], 'paved');
        });
    });

});
