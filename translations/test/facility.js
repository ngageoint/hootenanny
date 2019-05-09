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
        'TDSv40',
        // 'MGCP',
        'GGDMv30'
    ]

    schemas.forEach(schema => {
        Object.keys(cases).forEach(k => {

            var code = k;
            var tag = cases[k];
            var tagKey = Object.keys(tag)[0];
            var fcode_key = (schema === 'MGCP') ? 'FCODE' : 'F_CODE';

            it('should translate Facilities ' + fcode_key + '=AL010 and FFN = ' + code + ' without adding extra FFN2 from ' + schema + ' -> osm -> ' + schema, function() {

                var tds_xml = '<osm version="0.6">\
                                <way id="-19">\
                                    <nd ref="-10" />\
                                    <nd ref="-11" />\
                                    <nd ref="-12" />\
                                    <nd ref="-13" />\
                                    <nd ref="-10" />\
                                    <tag k="' + fcode_key + '" v="AL010"/>\
                                    <tag k="FFN" v="' + code + '"/>\
                                </way>\
                            </osm>';

                var osm_xml = server.handleInputs({
                    osm: tds_xml,
                    method: 'POST',
                    translation: schema,
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
                    translation: schema,
                    path: '/translateTo'
                });

                // console.log(tds_xml);

                xml = parser.parseFromString(tds_xml);

                assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), schema);
                assert.equal(xml.getElementsByTagName("tag")[1].getAttribute("k"), fcode_key);
                assert.equal(xml.getElementsByTagName("tag")[1].getAttribute("v"), "AL010");
                assert.equal(xml.getElementsByTagName("tag")[0].getAttribute("k"), "FFN");
                assert.equal(xml.getElementsByTagName("tag")[0].getAttribute("v"), code);

            });
        });
    });
});

