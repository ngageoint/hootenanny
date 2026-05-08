var assert = require('assert'),
    osmtogeojson = require('osmtogeojson'),
    DOMParser = new require('@xmldom/xmldom').DOMParser,
    parser = new DOMParser();

var server = require('../TranslationServer.js');

function assertRuinsTds(tdsXml) {
    var xml = parser.parseFromString(tdsXml);
    var gj = osmtogeojson(xml);

    assert.strictEqual(xml.getElementsByTagName("osm")[0].getAttribute("schema"), 'TDSv71');

    var tags = gj.features[0].properties;
    assert.strictEqual(tags['F_CODE'], 'AL200');
    assert.strictEqual(tags['FFN'], undefined);
    assert.strictEqual(tags['SSR'], undefined);
    assert.strictEqual(tags['VCM'], undefined);
    assert.strictEqual(tags['ZI037_RFA'], undefined);
    assert.strictEqual(tags['OTH'], undefined);
}

function assertAmenityRuinsOsm(tdsXml) {
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
    assert.strictEqual(tags['amenity'], 'ruins');
    assert.strictEqual(tags['building'], undefined);
    assert.strictEqual(tags['historic'], undefined);
    assert.strictEqual(tags['material'], undefined);
    assert.strictEqual(tags['tourism'], undefined);
}

describe('Ruins', function () {

    it('should translate TDSv71 amenity=ruins area to AL200 and override temple building tags', function () {

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
                            <tag k="amenity" v="ruins"/>\
                            <tag k="building" v="temple"/>\
                            <tag k="historic" v="temple"/>\
                            <tag k="material" v="stone"/>\
                        </way>\
                    </osm>';

        var tdsXml = server.handleInputs({
            osm: data,
            method: 'POST',
            translation: 'TDSv71',
            path: '/translateTo'
        });

        assertRuinsTds(tdsXml);
        assertAmenityRuinsOsm(tdsXml);
    });

    it('should translate TDSv71 amenity=ruins point to AL200 and override temple building tags', function () {

        var data = '<osm version="0.6" upload="true" generator="hootenanny">\
                        <node id="-10" action="modify" visible="true" lat="0.68307256979" lon="18.45073925651">\
                            <tag k="amenity" v="ruins"/>\
                            <tag k="building" v="temple"/>\
                            <tag k="historic" v="temple"/>\
                            <tag k="material" v="stone"/>\
                        </node>\
                    </osm>';

        var tdsXml = server.handleInputs({
            osm: data,
            method: 'POST',
            translation: 'TDSv71',
            path: '/translateTo'
        });

        assertRuinsTds(tdsXml);
        assertAmenityRuinsOsm(tdsXml);
    });

    it('should translate TDSv71 amenity=ruins with tourism=attraction to AL200', function () {

        var data = '<osm version="0.6" upload="true" generator="hootenanny">\
                        <node id="-10" action="modify" visible="true" lat="0.68307256979" lon="18.45073925651">\
                            <tag k="amenity" v="ruins"/>\
                            <tag k="building" v="temple"/>\
                            <tag k="historic" v="temple"/>\
                            <tag k="material" v="stone"/>\
                            <tag k="tourism" v="attraction"/>\
                        </node>\
                    </osm>';

        var tdsXml = server.handleInputs({
            osm: data,
            method: 'POST',
            translation: 'TDSv71',
            path: '/translateTo'
        });

        assertRuinsTds(tdsXml);
        assertAmenityRuinsOsm(tdsXml);
    });
});
