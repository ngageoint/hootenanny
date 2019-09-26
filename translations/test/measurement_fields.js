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

    var schemas = [
        'TDSv70',
        'TDSv61',
        'TDSv40',
        // 'MGCP', //used LEN not LZN
        'GGDMv30',
    ];

    var measurement_fields = {
        AOO: 'angle',
        LZN: 'length',
        WID: 'width',
        ARA: 'feature_area'
    }

    schemas.forEach(k => {

        var schema = k;

        it('should translate measurement tags from ' + schema + ' -> osm -> ' + schema, function() {

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
                                <tag k="F_CODE" v="AL019"/>\
                                <tag k="AOO" v="45"/>\
                                <tag k="LZN" v="100"/>\
                                <tag k="WID" v="10"/>\
                                <tag k="ARA" v="1000"/>\
                            </way>\
                        </osm>';

            var xml = parser.parseFromString(data);
            var gj = osmtogeojson(xml);

            var translated_tags = gj.features[0].properties;

            var osm_xml = server.handleInputs({
                osm: data,
                method: 'POST',
                translation: schema,
                path: '/translateFrom'
            });

            // console.log(osm_xml);

            xml = parser.parseFromString(osm_xml);
            gj = osmtogeojson(xml);

            assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "OSM");

            var osm_tags = gj.features[0].properties;
            Object.keys(measurement_fields).forEach(function(f) {
                assert.equal(osm_tags[measurement_fields[f]], translated_tags[f]);
            });

            var translated_xml = server.handleInputs({
                osm: osm_xml,
                method: 'POST',
                translation: schema,
                path: '/translateTo'
            });

            // console.log(translated_xml);

            xml = parser.parseFromString(translated_xml);
            gj = osmtogeojson(xml);

            assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), schema);

            var tags = gj.features[0].properties;
            Object.keys(measurement_fields).forEach(function(f) {
                assert.equal(tags[f], translated_tags[f]);
            });

        });

    });


    it('should translate length to LZN from osm -> tdsv61 even while missing nodes from the OSM', function() {

        var osm_xml = '<osm version="0.6" upload="true" generator="hootenanny">\
                        <way id="-19" action="modify" visible="true">\
                            <nd ref="-10" />\
                            <nd ref="-11" />\
                            <nd ref="-12" />\
                            <nd ref="-13" />\
                            <nd ref="-10" />\
                            <tag k="highway" v="primary"/>\
                            <tag k="length" v="332.2"/>\
                        </way>\
                    </osm>';

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
        assert.equal(xml.getElementsByTagName("tag")[1].getAttribute("v"), "AP030");
        assert.equal(xml.getElementsByTagName("tag")[4].getAttribute("k"), "LZN");
        assert.equal(xml.getElementsByTagName("tag")[4].getAttribute("v"), "332.2");

    });

});
