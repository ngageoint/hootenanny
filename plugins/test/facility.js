var assert = require('assert'),
    xml2js = require('xml2js'),
    DOMParser = new require('xmldom').DOMParser
    parser = new DOMParser();

var server = require('../TranslationServer.js');

describe('TranslationServer', function () {

    var cases = {
        850: {},
        851: {},
        852: {},
        857: {}
    }

    var schemas = [
        'TDSv61',
        // 'TDSv40',
        // 'GGDMv30'
    ]

    schemas.forEach(schema => {
        Object.keys(cases).forEach(k => {

            var code = k;
            var tag = cases[k];
            var tagKey = Object.keys(tag)[0];

            it('should translate Facilities F_CODE=AL010 and FFN = ' + code + ' without adding extra FFN2 from ' + schema + ' -> osm -> ' + schema, function() {

                var tds_xml = '<osm version="0.6">\
                                <way id="-19">\
                                    <nd ref="-10" />\
                                    <nd ref="-11" />\
                                    <nd ref="-12" />\
                                    <nd ref="-13" />\
                                    <nd ref="-10" />\
                                    <tag k="F_CODE" v="AL010"/>\
                                    <tag k="FFN" v="' + code + '"/>\
                                </way>\
                            </osm>';

                var osm_xml = server.handleInputs({
                    osm: tds_xml,
                    method: 'POST',
                    translation: 'TDSv61',
                    path: '/translateFrom'
                });

                // console.log(osm_xml);

                var xml = parser.parseFromString(osm_xml);

                assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "OSM");
                // assert.equal(xml.getElementsByTagName("tag")[1].getAttribute("k"), "aeroway");
                // assert.equal(xml.getElementsByTagName("tag")[1].getAttribute("v"), "stopway");


                var tds_xml = server.handleInputs({
                    osm: osm_xml,
                    method: 'POST',
                    translation: 'TDSv61',
                    path: '/translateTo'
                });

                // console.log(tds_xml);

                xml = parser.parseFromString(tds_xml);

                assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "TDSv61");
                assert.equal(xml.getElementsByTagName("tag")[1].getAttribute("k"), "F_CODE");
                assert.equal(xml.getElementsByTagName("tag")[1].getAttribute("v"), "AL010");
                assert.equal(xml.getElementsByTagName("tag")[2].getAttribute("k"), "FFN");
                assert.equal(xml.getElementsByTagName("tag")[2].getAttribute("v"), code);

            });
        });
    });
});

