var assert = require('assert'),
    osmtogeojson = require('osmtogeojson'),
    DOMParser = new require('@xmldom/xmldom').DOMParser,
    parser = new DOMParser();

var server = require('../TranslationServer.js');

describe('TranslationServer', function () {

    var schemas = [
        'TDSv40',
        'TDSv61',
        // 'TDSv70'
    ];

    schemas.forEach(schema => {
        it(`should translate amenity=school to F_CODE=AL010 & FFN=850 from osm -> ${ schema } -> osm`, function () {

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
                                <tag k="amenity" v="school"/>\
                                <tag k="isced:level" v="1"/>\
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

            assert.strictEqual(xml.getElementsByTagName("osm")[0].getAttribute("schema"), schema);

            var tags = gj.features[0].properties;
            assert.strictEqual(tags['F_CODE'], 'AL010');
            assert.strictEqual(tags['FFN'], '851');

            var tds_xml = server.handleInputs({
                osm: osm_xml,
                method: 'POST',
                translation: schema,
                path: '/translateFrom'
            });

            // console.log(tds_xml);

            xml = parser.parseFromString(tds_xml);
            gj = osmtogeojson(xml);

            assert.strictEqual(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "OSM");

            var tags = gj.features[0].properties;
            assert.strictEqual(tags["amenity"], 'school');
            assert.strictEqual(tags["isced:level"], '1');
        });
    });

    it('should translate TDSv71 amenity=school and building=no area to F_CODE=AL010 and FFN=850', function () {

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
                            <tag k="amenity" v="school"/>\
                            <tag k="building" v="no"/>\
                        </way>\
                    </osm>';

        var tds_xml = server.handleInputs({
            osm: data,
            method: 'POST',
            translation: 'TDSv71',
            path: '/translateTo'
        });

        var xml = parser.parseFromString(tds_xml);
        var gj = osmtogeojson(xml);

        assert.strictEqual(xml.getElementsByTagName("osm")[0].getAttribute("schema"), 'TDSv71');

        var tags = gj.features[0].properties;
        assert.strictEqual(tags['F_CODE'], 'AL010');
        assert.strictEqual(tags['FFN'], '850');
        assert.strictEqual(tags['OTH'], undefined);
    });

    it('should translate TDSv71 amenity=school and building=no point to F_CODE=AL010 and FFN=850', function () {

        var data = '<osm version="0.6" upload="true" generator="hootenanny">\
                        <node id="-10" action="modify" visible="true" lat="0.68307256979" lon="18.45073925651">\
                            <tag k="amenity" v="school"/>\
                            <tag k="building" v="no"/>\
                        </node>\
                    </osm>';

        var tds_xml = server.handleInputs({
            osm: data,
            method: 'POST',
            translation: 'TDSv71',
            path: '/translateTo'
        });

        var xml = parser.parseFromString(tds_xml);
        var gj = osmtogeojson(xml);

        assert.strictEqual(xml.getElementsByTagName("osm")[0].getAttribute("schema"), 'TDSv71');

        var tags = gj.features[0].properties;
        assert.strictEqual(tags['F_CODE'], 'AL010');
        assert.strictEqual(tags['FFN'], '850');
        assert.strictEqual(tags['OTH'], undefined);
    });

    it('should translate TDSv71 amenity=kindergarten area to F_CODE=AL010, FFN=850, and FFN2=851', function () {

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
                            <tag k="amenity" v="kindergarten"/>\
                        </way>\
                    </osm>';

        var tds_xml = server.handleInputs({
            osm: data,
            method: 'POST',
            translation: 'TDSv71',
            path: '/translateTo'
        });

        var xml = parser.parseFromString(tds_xml);
        var gj = osmtogeojson(xml);

        assert.strictEqual(xml.getElementsByTagName("osm")[0].getAttribute("schema"), 'TDSv71');

        var tags = gj.features[0].properties;
        assert.strictEqual(tags['F_CODE'], 'AL010');
        assert.strictEqual(tags['FFN'], '850');
        assert.strictEqual(tags['FFN2'], '851');
    });

    it('should translate TDSv71 amenity=kindergarten point to F_CODE=AL010, FFN=850, and FFN2=851', function () {

        var data = '<osm version="0.6" upload="true" generator="hootenanny">\
                        <node id="-10" action="modify" visible="true" lat="0.68307256979" lon="18.45073925651">\
                            <tag k="amenity" v="kindergarten"/>\
                        </node>\
                    </osm>';

        var tds_xml = server.handleInputs({
            osm: data,
            method: 'POST',
            translation: 'TDSv71',
            path: '/translateTo'
        });

        var xml = parser.parseFromString(tds_xml);
        var gj = osmtogeojson(xml);

        assert.strictEqual(xml.getElementsByTagName("osm")[0].getAttribute("schema"), 'TDSv71');

        var tags = gj.features[0].properties;
        assert.strictEqual(tags['F_CODE'], 'AL010');
        assert.strictEqual(tags['FFN'], '850');
        assert.strictEqual(tags['FFN2'], '851');
    });
});
