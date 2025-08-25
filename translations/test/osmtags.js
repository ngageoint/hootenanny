var assert = require('assert'),
    http = require('http'),
    xml2js = require('xml2js'),
    fs = require('fs'),
    httpMocks = require('node-mocks-http'),
    osmtogeojson = require('osmtogeojson'),
    DOMParser = new require('@xmldom/xmldom').DOMParser
    parser = new DOMParser();

var server = require('../TranslationServer.js');

describe('TranslationServer', function () {

    var schemas = [
        'TDSv70',
        'TDSv61',
        'TDSv40',
        'MGCP',
        'GGDMv30'
    ];

    schemas.forEach(schema => {
        var fcode_key = (schema === 'MGCP') ? 'FCODE' : 'F_CODE';
        var fcode_value = (schema === 'MGCP') ? 'AK190' : 'BB081';
        var osm_key = (schema === 'MGCP') ? 'man_made' : 'man_made';
        var osm_value = (schema === 'MGCP') ? 'pier' : 'shoreline_construction';

        it('should write OSMTAGS to osm tags when translating from ' + schema + ' -> OSM', function() {

            var data = '<osm version="0.6" upload="true" generator="hootenanny">\
                            <node id="-10" action="modify" visible="true" lat="0.68307256979" lon="18.45073925651" />\
                            <node id="-11" action="modify" visible="true" lat="0.68341620728" lon="18.45091527847" />\
                            <node id="-12" action="modify" visible="true" lat="0.68306209303" lon="18.45157116983" />\
                            <node id="-13" action="modify" visible="true" lat="0.68270797876" lon="18.45141400736" />\
                            <way id="-19" action="modify" visible="true">\
                                <nd ref="-10" />\
                                <nd ref="-11" />\
                                <nd ref="-12" />\
                                <nd ref="-13" />\
                                <nd ref="-10" />\
                                <tag k="' + fcode_key + '" v="' + fcode_value + '"/>\
                                <tag k="OSMTAGS" v="{&quot;area&quot;:&quot;yes&quot;,&quot;source:imagery&quot;:&quot;DigitalGlobe&quot;}"/>\
                            </way>\
                        </osm>';

            var osm_xml = server.handleInputs({
                osm: data,
                method: 'POST',
                translation: schema,
                path: '/translateFrom'
            });

            // console.log(osm_xml);

            var xml = parser.parseFromString(osm_xml);
            var gj = osmtogeojson(xml);

            assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "OSM");

            var tags = gj.features[0].properties;
            assert.equal(tags[osm_key], osm_value);
            assert.equal(tags['area'], 'yes');
            assert.equal(tags['source:imagery'], 'DigitalGlobe');
        });

        it('should write area=yes to OSMTAGS when translating from OSM -> ' + schema, function() {

            var data = '<osm version="0.6" upload="true" generator="hootenanny">\
                            <node id="-10" action="modify" visible="true" lat="0.68307256979" lon="18.45073925651" />\
                            <node id="-11" action="modify" visible="true" lat="0.68341620728" lon="18.45091527847" />\
                            <node id="-12" action="modify" visible="true" lat="0.68306209303" lon="18.45157116983" />\
                            <node id="-13" action="modify" visible="true" lat="0.68270797876" lon="18.45141400736" />\
                            <way id="-19" action="modify" visible="true">\
                                <nd ref="-10" />\
                                <nd ref="-11" />\
                                <nd ref="-12" />\
                                <nd ref="-13" />\
                                <nd ref="-10" />\
                                <tag k="' + osm_key + '" v="' + osm_value + '"/>\
                                <tag k="source:imagery" v="DigitalGlobe"/>\
                                <tag k="area" v="yes"/>\
                            </way>\
                        </osm>';

            var osm_xml = server.handleInputs({
                osm: data,
                method: 'POST',
                translation: schema,
                path: '/translateTo'
            });

            // console.log(osm_xml);

            var xml = parser.parseFromString(osm_xml);
            var gj = osmtogeojson(xml);

            assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), schema);

            var tags = gj.features[0].properties;
            assert.equal(tags[fcode_key], fcode_value);
            if (schema === 'TDSv70') {
                assert.equal(tags['OSMTAGS'], '{"area":"yes"}');
                assert.equal(tags['ZI001_SDP'], 'DigitalGlobe');
            } else if (schema === 'MGCP') {
                assert.equal(tags['OSMTAGS'], '{"area":"yes","man_made":"pier","source:imagery":"DigitalGlobe"}');
            } else {
                assert.equal(tags['OSMTAGS'], '{"area":"yes","source:imagery":"DigitalGlobe"}');
            }
        });
    });
});
