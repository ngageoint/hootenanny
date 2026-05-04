var assert = require('assert'),
    osmtogeojson = require('osmtogeojson'),
    DOMParser = new require('@xmldom/xmldom').DOMParser,
    parser = new DOMParser();

var server = require('../TranslationServer.js');

function assertDiplomaticEmbassyTds(tdsXml) {
    var xml = parser.parseFromString(tdsXml);
    var gj = osmtogeojson(xml);

    assert.strictEqual(xml.getElementsByTagName("osm")[0].getAttribute("schema"), 'TDSv71');

    var tags = gj.features[0].properties;
    assert.strictEqual(tags['F_CODE'], 'AL010');
    assert.strictEqual(tags['FFN'], '808');
    assert.strictEqual(tags['FFN2'], '825');
    assert.strictEqual(tags['FFN3'], '827');
    assert.strictEqual(tags['OTH'], undefined);
}

function assertDiplomaticEmbassyBuildingTds(tdsXml) {
    var xml = parser.parseFromString(tdsXml);
    var gj = osmtogeojson(xml);

    assert.strictEqual(xml.getElementsByTagName("osm")[0].getAttribute("schema"), 'TDSv71');

    var tags = gj.features[0].properties;
    assert.strictEqual(tags['F_CODE'], 'AL013');
    assert.strictEqual(tags['FFN'], '808');
    assert.strictEqual(tags['FFN2'], '825');
    assert.strictEqual(tags['FFN3'], '827');
    assert.strictEqual(tags['OTH'], undefined);
}

function assertDiplomaticEmbassyOsm(tdsXml) {
    var osmXml = server.handleInputs({
        osm: tdsXml,
        method: 'POST',
        translation: 'TDSv71',
        path: '/translateFrom'
    });

    var xml = parser.parseFromString(osmXml);
    var gj = osmtogeojson(xml);

    assert.strictEqual(xml.getElementsByTagName("osm")[0].getAttribute("schema"), 'OSM');

    var tags = gj.features[0].properties;
    assert.strictEqual(tags['office'], 'diplomatic');
    assert.strictEqual(tags['diplomatic'], 'embassy');
    assert.strictEqual(tags['amenity'], undefined);
    assert.strictEqual(tags['facility'], undefined);
}

function assertDiplomaticEmbassyBuildingOsm(tdsXml) {
    var osmXml = server.handleInputs({
        osm: tdsXml,
        method: 'POST',
        translation: 'TDSv71',
        path: '/translateFrom'
    });

    var xml = parser.parseFromString(osmXml);
    var gj = osmtogeojson(xml);

    assert.strictEqual(xml.getElementsByTagName("osm")[0].getAttribute("schema"), 'OSM');

    var tags = gj.features[0].properties;
    assert.strictEqual(tags['building'], 'yes');
    assert.strictEqual(tags['office'], 'diplomatic');
    assert.strictEqual(tags['diplomatic'], 'embassy');
    assert.strictEqual(tags['amenity'], undefined);
    assert.strictEqual(tags['facility'], undefined);
}

describe('Diplomatic Facility', function () {

    it('should translate TDSv71 office=diplomatic and diplomatic=embassy area to AL010 with embassy FFNs', function () {

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
                            <tag k="office" v="diplomatic"/>\
                            <tag k="diplomatic" v="embassy"/>\
                        </way>\
                    </osm>';

        var tdsXml = server.handleInputs({
            osm: data,
            method: 'POST',
            translation: 'TDSv71',
            path: '/translateTo'
        });

        assertDiplomaticEmbassyTds(tdsXml);
        assertDiplomaticEmbassyOsm(tdsXml);
    });

    it('should translate TDSv71 office=diplomatic and diplomatic=embassy point to AL010 with embassy FFNs', function () {

        var data = '<osm version="0.6" upload="true" generator="hootenanny">\
                        <node id="-10" action="modify" visible="true" lat="0.68307256979" lon="18.45073925651">\
                            <tag k="office" v="diplomatic"/>\
                            <tag k="diplomatic" v="embassy"/>\
                        </node>\
                    </osm>';

        var tdsXml = server.handleInputs({
            osm: data,
            method: 'POST',
            translation: 'TDSv71',
            path: '/translateTo'
        });

        assertDiplomaticEmbassyTds(tdsXml);
        assertDiplomaticEmbassyOsm(tdsXml);
    });

    it('should translate TDSv71 AL010 with FFN=808, FFN2=825, and FFN3=827 to office=diplomatic and diplomatic=embassy', function () {

        var data = '<osm version="0.6" upload="true" generator="hootenanny">\
                        <node id="-10" action="modify" visible="true" lat="0.68307256979" lon="18.45073925651">\
                            <tag k="F_CODE" v="AL010"/>\
                            <tag k="FFN" v="808"/>\
                            <tag k="FFN2" v="825"/>\
                            <tag k="FFN3" v="827"/>\
                        </node>\
                    </osm>';

        assertDiplomaticEmbassyOsm(data);
    });

    it('should translate TDSv71 building=yes, office=diplomatic, and diplomatic=embassy area to AL013 with embassy FFNs', function () {

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
                            <tag k="building" v="yes"/>\
                            <tag k="office" v="diplomatic"/>\
                            <tag k="diplomatic" v="embassy"/>\
                        </way>\
                    </osm>';

        var tdsXml = server.handleInputs({
            osm: data,
            method: 'POST',
            translation: 'TDSv71',
            path: '/translateTo'
        });

        assertDiplomaticEmbassyBuildingTds(tdsXml);
        assertDiplomaticEmbassyBuildingOsm(tdsXml);
    });

    it('should translate TDSv71 building=yes, office=diplomatic, and diplomatic=embassy point to AL013 with embassy FFNs', function () {

        var data = '<osm version="0.6" upload="true" generator="hootenanny">\
                        <node id="-10" action="modify" visible="true" lat="0.68307256979" lon="18.45073925651">\
                            <tag k="building" v="yes"/>\
                            <tag k="office" v="diplomatic"/>\
                            <tag k="diplomatic" v="embassy"/>\
                        </node>\
                    </osm>';

        var tdsXml = server.handleInputs({
            osm: data,
            method: 'POST',
            translation: 'TDSv71',
            path: '/translateTo'
        });

        assertDiplomaticEmbassyBuildingTds(tdsXml);
        assertDiplomaticEmbassyBuildingOsm(tdsXml);
    });

    it('should translate TDSv71 building=government, office=diplomatic, and diplomatic=embassy area to AL013 with embassy FFNs', function () {

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
                            <tag k="building" v="government"/>\
                            <tag k="office" v="diplomatic"/>\
                            <tag k="diplomatic" v="embassy"/>\
                        </way>\
                    </osm>';

        var tdsXml = server.handleInputs({
            osm: data,
            method: 'POST',
            translation: 'TDSv71',
            path: '/translateTo'
        });

        assertDiplomaticEmbassyBuildingTds(tdsXml);
    });

    it('should translate TDSv71 building=government, office=diplomatic, and diplomatic=embassy point to AL013 with embassy FFNs', function () {

        var data = '<osm version="0.6" upload="true" generator="hootenanny">\
                        <node id="-10" action="modify" visible="true" lat="0.68307256979" lon="18.45073925651">\
                            <tag k="building" v="government"/>\
                            <tag k="office" v="diplomatic"/>\
                            <tag k="diplomatic" v="embassy"/>\
                        </node>\
                    </osm>';

        var tdsXml = server.handleInputs({
            osm: data,
            method: 'POST',
            translation: 'TDSv71',
            path: '/translateTo'
        });

        assertDiplomaticEmbassyBuildingTds(tdsXml);
    });

    it('should translate TDSv71 AL013 with FFN=808, FFN2=825, and FFN3=827 to building=yes, office=diplomatic, and diplomatic=embassy', function () {

        var data = '<osm version="0.6" upload="true" generator="hootenanny">\
                        <node id="-10" action="modify" visible="true" lat="0.68307256979" lon="18.45073925651">\
                            <tag k="F_CODE" v="AL013"/>\
                            <tag k="FFN" v="808"/>\
                            <tag k="FFN2" v="825"/>\
                            <tag k="FFN3" v="827"/>\
                        </node>\
                    </osm>';

        assertDiplomaticEmbassyBuildingOsm(data);
    });
});
