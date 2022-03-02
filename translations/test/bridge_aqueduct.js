var assert = require('assert'),
    http = require('http'),
    xml2js = require('xml2js'),
    fs = require('fs'),
    httpMocks = require('node-mocks-http'),
    osmtogeojson = require('osmtogeojson'),
    DOMParser = new require('@xmldom/xmldom').DOMParser
    parser = new DOMParser();

var server = require('../TranslationServer.js');

describe('Bridgs and Aqueduct', function () {
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
        'TDSv71',
        'TDSv70',
        'TDSv61',
        'TDSv40',
        'MGCP',
        'GGDMv30'
    ];

    schemas.forEach(schema => {
        var fcode_key = (schema === 'MGCP') ? 'FCODE' : 'F_CODE';

        it('should translate Bridge (AQ040) Area from ' + schema + ' -> osm -> ' + schema, function() {
            var data = startArea + '<tag k="' + fcode_key + '" v="AQ040"/>' + endWay;
            var osm_xml = server.handleInputs({osm: data, method: 'POST', translation: schema, path: '/translateFrom'});
            // console.log(osm_xml);
            var xml = parser.parseFromString(osm_xml);
            assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "OSM");

            var tags = osmtogeojson(xml).features[0].properties;
            assert.equal(tags["bridge"], 'yes');
            assert.equal(tags["area"], 'yes');

            var trans_xml = server.handleInputs({osm: osm_xml, method: 'POST', translation: schema, path: '/translateTo'});
            // console.log(trans_xml);
            xml = parser.parseFromString(trans_xml);
            assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), schema);

            var tags = osmtogeojson(xml).features[0].properties;
            assert.equal(tags[fcode_key], 'AQ040');
        });

        it('should translate Aqueduct (BH010) Area from ' + schema + ' -> osm -> ' + schema, function() {
            var data = startArea + '<tag k="' + fcode_key + '" v="BH010"/>' + endWay;
            var osm_xml = server.handleInputs({osm: data, method: 'POST', translation: schema, path: '/translateFrom'});
            // console.log(osm_xml);
            var xml = parser.parseFromString(osm_xml);
            assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "OSM");

            var tags = osmtogeojson(xml).features[0].properties;
            assert.equal(tags["bridge"], 'aqueduct');
            assert.equal(tags["area"], 'yes');

            var trans_xml = server.handleInputs({osm: osm_xml, method: 'POST', translation: schema, path: '/translateTo'});
            // console.log(trans_xml);
            xml = parser.parseFromString(trans_xml);
            assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), schema);

            var tags = osmtogeojson(xml).features[0].properties;
            assert.equal(tags[fcode_key], 'BH010');
        });
    });

});
