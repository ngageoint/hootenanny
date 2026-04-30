var assert = require('assert'),
    osmtogeojson = require('osmtogeojson'),
    DOMParser = new require('@xmldom/xmldom').DOMParser,
    parser = new DOMParser();

var server = require('../TranslationServer.js');

describe('Religious Facility', function () {

    it('should translate TDSv71 landuse=religious area to F_CODE=AL010, FFN=930, and FFN2=931', function () {

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
                            <tag k="landuse" v="religious"/>\
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
        assert.strictEqual(tags['FFN'], '930');
        assert.strictEqual(tags['FFN2'], '931');
    });

    it('should translate TDSv71 landuse=religious point to F_CODE=AL010, FFN=930, and FFN2=931', function () {

        var data = '<osm version="0.6" upload="true" generator="hootenanny">\
                        <node id="-10" action="modify" visible="true" lat="0.68307256979" lon="18.45073925651">\
                            <tag k="landuse" v="religious"/>\
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
        assert.strictEqual(tags['FFN'], '930');
        assert.strictEqual(tags['FFN2'], '931');
    });

    it('should translate TDSv71 amenity=place_of_worship area to F_CODE=AL010, FFN=930, and FFN2=931', function () {

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
                            <tag k="amenity" v="place_of_worship"/>\
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
        assert.strictEqual(tags['FFN'], '930');
        assert.strictEqual(tags['FFN2'], '931');
    });

    it('should translate TDSv71 amenity=place_of_worship point to F_CODE=AL010, FFN=930, and FFN2=931', function () {

        var data = '<osm version="0.6" upload="true" generator="hootenanny">\
                        <node id="-10" action="modify" visible="true" lat="0.68307256979" lon="18.45073925651">\
                            <tag k="amenity" v="place_of_worship"/>\
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
        assert.strictEqual(tags['FFN'], '930');
        assert.strictEqual(tags['FFN2'], '931');
    });

    it('should translate TDSv71 amenity=place_of_worship and building=no area to F_CODE=AL010, FFN=930, and FFN2=931', function () {

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
                            <tag k="amenity" v="place_of_worship"/>\
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
        assert.strictEqual(tags['FFN'], '930');
        assert.strictEqual(tags['FFN2'], '931');
    });

    it('should translate TDSv71 amenity=place_of_worship and building=no point to F_CODE=AL010, FFN=930, and FFN2=931', function () {

        var data = '<osm version="0.6" upload="true" generator="hootenanny">\
                        <node id="-10" action="modify" visible="true" lat="0.68307256979" lon="18.45073925651">\
                            <tag k="amenity" v="place_of_worship"/>\
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
        assert.strictEqual(tags['FFN'], '930');
        assert.strictEqual(tags['FFN2'], '931');
    });

    it('should translate TDSv71 F_CODE=AL010, FFN=930, and FFN2=931 area to amenity=place_of_worship', function () {

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
                            <tag k="F_CODE" v="AL010"/>\
                            <tag k="FFN" v="930"/>\
                            <tag k="FFN2" v="931"/>\
                        </way>\
                    </osm>';

        var osm_xml = server.handleInputs({
            osm: data,
            method: 'POST',
            translation: 'TDSv71',
            path: '/translateFrom'
        });

        var xml = parser.parseFromString(osm_xml);
        var gj = osmtogeojson(xml);

        assert.strictEqual(xml.getElementsByTagName("osm")[0].getAttribute("schema"), 'OSM');
        assert.strictEqual(gj.features[0].properties['amenity'], 'place_of_worship');
    });

    it('should translate TDSv71 F_CODE=AL010, FFN=930, and FFN2=931 point to amenity=place_of_worship', function () {

        var data = '<osm version="0.6" upload="true" generator="hootenanny">\
                        <node id="-10" action="modify" visible="true" lat="0.68307256979" lon="18.45073925651">\
                            <tag k="F_CODE" v="AL010"/>\
                            <tag k="FFN" v="930"/>\
                            <tag k="FFN2" v="931"/>\
                        </node>\
                    </osm>';

        var osm_xml = server.handleInputs({
            osm: data,
            method: 'POST',
            translation: 'TDSv71',
            path: '/translateFrom'
        });

        var xml = parser.parseFromString(osm_xml);
        var gj = osmtogeojson(xml);

        assert.strictEqual(xml.getElementsByTagName("osm")[0].getAttribute("schema"), 'OSM');
        assert.strictEqual(gj.features[0].properties['amenity'], 'place_of_worship');
    });
});
