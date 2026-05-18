var assert = require('assert'),
    osmtogeojson = require('osmtogeojson'),
    DOMParser = new require('@xmldom/xmldom').DOMParser,
    parser = new DOMParser();

var server = require('../TranslationServer.js');

function assertLegislativeGovernmentBuildingTds(tdsXml) {
    var xml = parser.parseFromString(tdsXml);
    var gj = osmtogeojson(xml);

    assert.strictEqual(xml.getElementsByTagName("osm")[0].getAttribute("schema"), 'TDSv71');

    var tags = gj.features[0].properties;
    assert.strictEqual(tags['F_CODE'], 'AL013');
    assert.strictEqual(tags['FFN'], '808');
    assert.strictEqual(tags['FFN2'], '811');
    assert.strictEqual(tags['FFN3'], '819');
    assert.strictEqual(tags['OTH'], undefined);
    assert.strictEqual(tags['OSMTAGS'], undefined);
}

function assertLegislativeGovernmentBuildingOsm(tdsXml) {
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
    assert.strictEqual(tags['office'], 'government');
    assert.strictEqual(tags['government'], 'legislative');
    assert.strictEqual(tags['amenity'], undefined);
    assert.strictEqual(tags['amenity:2'], undefined);
    assert.strictEqual(tags['office:3'], undefined);
    assert.strictEqual(tags['use'], undefined);
    assert.strictEqual(tags['use:2'], undefined);
}

describe('Government Office', function () {

    it('should translate TDSv71 office=government and government=legislative area to AL013 with legislative FFNs', function () {

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
                            <tag k="office" v="government"/>\
                            <tag k="government" v="legislative"/>\
                        </way>\
                    </osm>';

        var tdsXml = server.handleInputs({
            osm: data,
            method: 'POST',
            translation: 'TDSv71',
            path: '/translateTo'
        });

        assertLegislativeGovernmentBuildingTds(tdsXml);
        assertLegislativeGovernmentBuildingOsm(tdsXml);
    });

    it('should translate TDSv71 office=government and government=legislative point to AL013 with legislative FFNs', function () {

        var data = '<osm version="0.6" upload="true" generator="hootenanny">\
                        <node id="-10" action="modify" visible="true" lat="0.68307256979" lon="18.45073925651">\
                            <tag k="office" v="government"/>\
                            <tag k="government" v="legislative"/>\
                        </node>\
                    </osm>';

        var tdsXml = server.handleInputs({
            osm: data,
            method: 'POST',
            translation: 'TDSv71',
            path: '/translateTo'
        });

        assertLegislativeGovernmentBuildingTds(tdsXml);
        assertLegislativeGovernmentBuildingOsm(tdsXml);
    });

    it('should translate TDSv71 AL013 with FFN=808, FFN2=811, and FFN3=819 to building=yes, office=government, and government=legislative', function () {

        var data = '<osm version="0.6" upload="true" generator="hootenanny">\
                        <node id="-10" action="modify" visible="true" lat="0.68307256979" lon="18.45073925651">\
                            <tag k="F_CODE" v="AL013"/>\
                            <tag k="FFN" v="808"/>\
                            <tag k="FFN2" v="811"/>\
                            <tag k="FFN3" v="819"/>\
                        </node>\
                    </osm>';

        assertLegislativeGovernmentBuildingOsm(data);
    });
});
