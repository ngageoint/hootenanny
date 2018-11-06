var assert = require('assert'),
    http = require('http'),
    xml2js = require('xml2js'),
    fs = require('fs'),
    httpMocks = require('node-mocks-http'),
    osmtogeojson = require('osmtogeojson'),
    DOMParser = new require('xmldom').DOMParser
    parser = new DOMParser();

var server = require('../TranslationServer.js');

describe('TranslationServer', function () {

    var cases = {
        GB055: {aeroway: 'runway'},
        // AP030: {highway: 'road'},
        AL013: {building: 'yes'},
        // BH070: {ford: 'yes'},
        BH140: {waterway: 'riverbank'}
    }

    Object.keys(cases).forEach(k => {

        var fcode = k;
        var tag = cases[k];
        var tagKey = Object.keys(tag)[0];

        it('should translate ' + fcode + ' from tdsv61 -> osm -> tdsv61', function() {

            var data = '<osm version="0.6" upload="true" generator="hootenanny">\
                            <node id="-10" lat="0.68307256979" lon="18.45073925651" />\
                            <node id="-11" lat="0.68341620728" lon="18.45091527847" />\
                            <node id="-12" lat="0.68306209303" lon="18.45157116983" />\
                            <node id="-13" lat="0.68270797876" lon="18.45141400736" />\
                            <way id="-19" >\
                                <nd ref="-10" />\
                                <nd ref="-11" />\
                                <nd ref="-12" />\
                                <nd ref="-13" />\
                                <nd ref="-10" />\
                                <tag k="F_CODE" v="' + fcode + '"/>\
                            </way>\
                        </osm>';

            var osm_xml = server.handleInputs({
                osm: data,
                method: 'POST',
                translation: 'TDSv61',
                path: '/translateFrom'
            });

            // console.log(osm_xml);

            var xml = parser.parseFromString(osm_xml);
            var gj = osmtogeojson(xml);

            assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "OSM");

            var tags = gj.features[0].properties;
            assert.equal(tags[tagKey], tag[tagKey]);

            var tds_xml = server.handleInputs({
                osm: osm_xml,
                method: 'POST',
                translation: 'TDSv61',
                path: '/translateTo'
            });

            // console.log(tds_xml);
            
            xml = parser.parseFromString(tds_xml);
            gj = osmtogeojson(xml);

            assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "TDSv61");

            var tags = gj.features[0].properties;
            assert.equal(tags["F_CODE"], fcode);


            osm_xml = server.handleInputs({
                osm: tds_xml,
                method: 'POST',
                translation: 'TDSv61',
                path: '/translateFrom'
            });

            // console.log(osm_xml);

            var xml = parser.parseFromString(osm_xml);
            var gj = osmtogeojson(xml);

            assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "OSM");

            var tags = gj.features[0].properties;
            assert.equal(tags[tagKey], tag[tagKey]);

        });

    });


});
