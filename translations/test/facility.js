var assert = require('assert'),
    osmtogeojson = require('osmtogeojson'),
    DOMParser = new require('xmldom').DOMParser,
    parser = new DOMParser();

var server = require('../TranslationServer.js');

describe('TranslationServer', function () {

    var cases = {
        850: {},
        851: {},
        852: {},
        857: {}
    };

    var schemas = [
        'TDSv61',
        'TDSv40',
        // 'MGCP',
        'GGDMv30'
    ];

    schemas.forEach(schema => {
        Object.keys(cases).forEach(k => {

            var code = k;
            var tag = cases[k];
            var tagKey = Object.keys(tag)[0];
            var fcode_key = (schema === 'MGCP') ? 'FCODE' : 'F_CODE';

            it('should translate Facilities ' + fcode_key + '=AL010 and FFN = ' + code + ' without adding extra FFN2 from ' + schema + ' -> osm -> ' + schema, function() {

                var tds_xml = '<osm version="0.6" upload="true" generator="hootenanny">\
                                <node id="-10" action="modify" visible="true" lat="0.68307256979" lon="18.45073925651" />\
                                <node id="-11" action="modify" visible="true" lat="0.68341620728" lon="18.45091527847" />\
                                <node id="-12" action="modify" visible="true" lat="0.68306209303" lon="18.45157116983" />\
                                <node id="-13" action="modify" visible="true" lat="0.68270797876" lon="18.45141400736" />\
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

                var tds_xml = server.handleInputs({
                    osm: osm_xml,
                    method: 'POST',
                    translation: schema,
                    path: '/translateTo'
                });

                // console.log(tds_xml);

                xml = parser.parseFromString(tds_xml);
                var gj = osmtogeojson(xml);

                assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), schema);

                var tags = gj.features[0].properties;

                assert.equal(tags[fcode_key], 'AL010');
                assert.equal(tags['FFN'], code);

            });
        });
    });
});

