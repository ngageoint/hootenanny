var assert = require('assert'),
    osmtogeojson = require('osmtogeojson'),
    DOMParser = new require('@xmldom/xmldom').DOMParser,
    parser = new DOMParser();

var server = require('../TranslationServer.js');

function assertPyramidTombTds(tdsXml) {
    var xml = parser.parseFromString(tdsXml);
    var gj = osmtogeojson(xml);

    assert.strictEqual(xml.getElementsByTagName("osm")[0].getAttribute("schema"), 'TDSv71');

    var tags = gj.features[0].properties;
    assert.strictEqual(tags['F_CODE'], 'AL036');
    assert.strictEqual(tags['SSC'], '12');
    assert.strictEqual(tags['SSR'], undefined);
    assert.strictEqual(tags['FFN'], undefined);
    assert.strictEqual(tags['VCM'], undefined);
    assert.strictEqual(tags['OTH'], undefined);
    assert.strictEqual(tags['OSMTAGS'], undefined);
}

function assertPyramidTombOsm(tdsXml) {
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
    assert.strictEqual(tags['historic'], 'tomb');
    assert.strictEqual(tags['tomb'], 'pyramid');
    assert.strictEqual(tags['shape'], undefined);
}

describe('Pyramid Tomb', function () {

    it('should translate TDSv71 pyramid tomb area to AL036 with pyramidal SSC', function () {

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
                            <tag k="historic" v="tomb"/>\
                            <tag k="tomb" v="pyramid"/>\
                            <tag k="tourism" v="attraction"/>\
                            <tag k="material" v="stone"/>\
                        </way>\
                    </osm>';

        var tdsXml = server.handleInputs({
            osm: data,
            method: 'POST',
            translation: 'TDSv71',
            path: '/translateTo'
        });

        assertPyramidTombTds(tdsXml);
        assertPyramidTombOsm(tdsXml);
    });

    it('should translate TDSv71 pyramid tomb point to AL036 with pyramidal SSC', function () {

        var data = '<osm version="0.6" upload="true" generator="hootenanny">\
                        <node id="-10" action="modify" visible="true" lat="0.68307256979" lon="18.45073925651">\
                            <tag k="building" v="yes"/>\
                            <tag k="historic" v="tomb"/>\
                            <tag k="tomb" v="pyramid"/>\
                            <tag k="tourism" v="attraction"/>\
                            <tag k="material" v="stone"/>\
                        </node>\
                    </osm>';

        var tdsXml = server.handleInputs({
            osm: data,
            method: 'POST',
            translation: 'TDSv71',
            path: '/translateTo'
        });

        assertPyramidTombTds(tdsXml);
        assertPyramidTombOsm(tdsXml);
    });

    it('should translate TDSv71 AL036 with SSC=12 to historic=tomb and tomb=pyramid', function () {

        var data = '<osm version="0.6" upload="true" generator="hootenanny">\
                        <node id="-10" action="modify" visible="true" lat="0.68307256979" lon="18.45073925651">\
                            <tag k="F_CODE" v="AL036"/>\
                            <tag k="SSC" v="12"/>\
                        </node>\
                    </osm>';

        assertPyramidTombOsm(data);
    });
});
