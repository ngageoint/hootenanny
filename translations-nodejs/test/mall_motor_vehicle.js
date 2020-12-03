var assert = require('assert'),
    http = require('http'),
    xml2js = require('xml2js'),
    fs = require('fs'),
    httpMocks = require('node-mocks-http'),
    osmtogeojson = require('osmtogeojson'),
    DOMParser = new require('xmldom').DOMParser,
    parser = new DOMParser();

var server = require('../TranslationServer.js');

describe('TranslationServer', function () {
    this.timeout(5000);


    it('should translate Motor Vehicle Station (AQ170) from tdsv61 -> osm -> tdsv61', function() {

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
                            <tag k="F_CODE" v="AQ170"/>\
                            <tag k="FFN" v="470"/>\
                            <tag k="FFN2" v="382"/>\
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
        assert.equal(tags["amenity"], "fuel");
        assert.equal(tags["toilets"], "yes");

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
        assert.equal(tags["F_CODE"], "AQ170");
        assert.equal(tags["FFN"], "470");
        assert.equal(tags["FFN2"], "382");
    });


    it('should translate Water Park (F_CODE=AK030 & FFN=915) from tdsv61 -> osm -> tdsv61', function() {

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
                            <tag k="F_CODE" v="AK030"/>\
                            <tag k="FFN" v="915"/>\
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
        assert.equal(tags['leisure'], 'water_park');

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
        assert.equal(tags["F_CODE"], 'AK030');
        assert.equal(tags["FFN"], '915');

    });


    //Check multiple schemas and simple FCODE translations

    var cases = {
        AG030: {shop: 'mall'},
        AD030: {power: 'substation'}
    }

    var schemas = [
        'TDSv61',
        'TDSv40',
        // 'MGCP', //No AG030 in MGCP
        'GGDMv30'
    ]

    schemas.forEach(schema => {
        Object.keys(cases).forEach(k => {

            var fcode = k;
            var tag = cases[k];
            var tagKey = Object.keys(tag)[0];
            var fcode_key = (schema === 'MGCP') ? 'FCODE' : 'F_CODE';

            it('should translate ' + fcode + ' from ' + schema + ' -> osm -> ' + schema, function() {

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
                                    <tag k="' + fcode_key + '" v="' + fcode + '"/>\
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
                assert.equal(tags[tagKey], tag[tagKey]);

                var tds_xml = server.handleInputs({
                    osm: osm_xml,
                    method: 'POST',
                    translation: schema,
                    path: '/translateTo'
                });

                // console.log(tds_xml);

                xml = parser.parseFromString(tds_xml);
                gj = osmtogeojson(xml);

                assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), schema);

                var tags = gj.features[0].properties;
                assert.equal(tags[fcode_key], fcode);

            });

        });
    });
});

