var assert = require('assert'),
    osmtogeojson = require('osmtogeojson'),
    DOMParser = new require('@xmldom/xmldom').DOMParser,
    parser = new DOMParser();

var server = require('../TranslationServer.js');

function assertStoneRetainingWallTds(tdsXml) {
    var xml = parser.parseFromString(tdsXml);
    var gj = osmtogeojson(xml);

    assert.strictEqual(xml.getElementsByTagName("osm")[0].getAttribute("schema"), 'TDSv71');

    var tags = gj.features[0].properties;
    assert.strictEqual(tags['F_CODE'], 'AL260');
    assert.strictEqual(tags['MCC'], '108');
    assert.strictEqual(tags['WTI'], '2');
    assert.strictEqual(tags['GTC'], undefined);
    assert.strictEqual(tags['OTH'], undefined);
    assert.strictEqual(tags['OSMTAGS'], undefined);
}

function assertStoneRetainingWallOsm(tdsXml) {
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
    assert.strictEqual(tags['barrier'], 'retaining_wall');
    assert.strictEqual(tags['material'], 'stone');
    assert.strictEqual(tags['wall'], undefined);
}

describe('Retaining Wall', function () {

    it('should translate TDSv71 barrier=retaining_wall and material=stone line to AL260 with stone retaining wall attrs', function () {

        var data = '<osm version="0.6" upload="true" generator="hootenanny">\
                        <node id="-10" action="modify" visible="true" lat="0.68307256979" lon="18.45073925651" />\
                        <node id="-11" action="modify" visible="true" lat="0.68341620728" lon="18.45091527847" />\
                        <way id="-19" action="modify" visible="true">\
                            <nd ref="-10" />\
                            <nd ref="-11" />\
                            <tag k="barrier" v="retaining_wall"/>\
                            <tag k="material" v="stone"/>\
                        </way>\
                    </osm>';

        var tdsXml = server.handleInputs({
            osm: data,
            method: 'POST',
            translation: 'TDSv71',
            path: '/translateTo'
        });

        assertStoneRetainingWallTds(tdsXml);
        assertStoneRetainingWallOsm(tdsXml);
    });

    it('should translate TDSv71 AL260 with MCC=108 and WTI=2 to barrier=retaining_wall and material=stone', function () {

        var data = '<osm version="0.6" upload="true" generator="hootenanny">\
                        <node id="-10" action="modify" visible="true" lat="0.68307256979" lon="18.45073925651" />\
                        <node id="-11" action="modify" visible="true" lat="0.68341620728" lon="18.45091527847" />\
                        <way id="-19" action="modify" visible="true">\
                            <nd ref="-10" />\
                            <nd ref="-11" />\
                            <tag k="F_CODE" v="AL260"/>\
                            <tag k="MCC" v="108"/>\
                            <tag k="WTI" v="2"/>\
                        </way>\
                    </osm>';

        assertStoneRetainingWallOsm(data);
    });
});
