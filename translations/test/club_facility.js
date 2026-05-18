var assert = require('assert'),
    osmtogeojson = require('osmtogeojson'),
    DOMParser = new require('@xmldom/xmldom').DOMParser,
    parser = new DOMParser();

var server = require('../TranslationServer.js');

function assertClubFacilityTds(tdsXml) {
    var xml = parser.parseFromString(tdsXml);
    var gj = osmtogeojson(xml);

    assert.strictEqual(xml.getElementsByTagName("osm")[0].getAttribute("schema"), 'TDSv71');

    var tags = gj.features[0].properties;
    assert.strictEqual(tags['F_CODE'], 'AL010');
    assert.strictEqual(tags['FFN'], '950');
    assert.strictEqual(tags['FFN2'], '954');
    assert.strictEqual(tags['OTH'], undefined);
    assert.strictEqual(tags['OSMTAGS'], undefined);
}

function assertClubOsm(tdsXml) {
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
    assert.strictEqual(tags['club'], 'yes');
    assert.strictEqual(tags['amenity'], undefined);
    assert.strictEqual(tags['amenity:2'], undefined);
    assert.strictEqual(tags['facility'], undefined);
    assert.strictEqual(tags['use'], undefined);
    assert.strictEqual(tags['use:2'], undefined);
}

describe('Club Facility', function () {

    it('should translate TDSv71 club=yes area to AL010 with club FFNs', function () {

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
                            <tag k="club" v="yes"/>\
                        </way>\
                    </osm>';

        var tdsXml = server.handleInputs({
            osm: data,
            method: 'POST',
            translation: 'TDSv71',
            path: '/translateTo'
        });

        assertClubFacilityTds(tdsXml);
        assertClubOsm(tdsXml);
    });

    it('should translate TDSv71 club=yes point to AL010 with club FFNs', function () {

        var data = '<osm version="0.6" upload="true" generator="hootenanny">\
                        <node id="-10" action="modify" visible="true" lat="0.68307256979" lon="18.45073925651">\
                            <tag k="club" v="yes"/>\
                        </node>\
                    </osm>';

        var tdsXml = server.handleInputs({
            osm: data,
            method: 'POST',
            translation: 'TDSv71',
            path: '/translateTo'
        });

        assertClubFacilityTds(tdsXml);
        assertClubOsm(tdsXml);
    });

    it('should translate TDSv71 AL010 with FFN=950 and FFN2=954 to club=yes', function () {

        var data = '<osm version="0.6" upload="true" generator="hootenanny">\
                        <node id="-10" action="modify" visible="true" lat="0.68307256979" lon="18.45073925651">\
                            <tag k="F_CODE" v="AL010"/>\
                            <tag k="FFN" v="950"/>\
                            <tag k="FFN2" v="954"/>\
                        </node>\
                    </osm>';

        assertClubOsm(data);
    });
});
