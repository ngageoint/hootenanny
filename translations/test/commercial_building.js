var assert = require('assert'),
    osmtogeojson = require('osmtogeojson'),
    DOMParser = new require('@xmldom/xmldom').DOMParser,
    parser = new DOMParser();

var server = require('../TranslationServer.js');

function assertCommercialDepartmentStore(tdsXml) {
    var xml = parser.parseFromString(tdsXml);
    var gj = osmtogeojson(xml);

    assert.strictEqual(xml.getElementsByTagName("osm")[0].getAttribute("schema"), 'TDSv71');

    var tags = gj.features[0].properties;
    assert.strictEqual(tags['F_CODE'], 'AL013');
    assert.strictEqual(tags['FFN'], '440');
    assert.strictEqual(tags['FFN2'], '460');
    assert.strictEqual(tags['FFN3'], '465');
    assert.strictEqual(tags['OTH'], undefined);
}

function assertDepartmentStore(tdsXml) {
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
    assert.strictEqual(tags['building'], 'commercial');
    assert.strictEqual(tags['shop'], 'department_store');
    assert.strictEqual(tags['use'], undefined);
    assert.strictEqual(tags['use:2'], undefined);
}

describe('Commercial Building', function () {

    it('should translate TDSv71 building=commercial and shop=department_store area to AL013 with retail department store FFNs', function () {

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
                            <tag k="building" v="commercial"/>\
                            <tag k="shop" v="department_store"/>\
                        </way>\
                    </osm>';

        var tdsXml = server.handleInputs({
            osm: data,
            method: 'POST',
            translation: 'TDSv71',
            path: '/translateTo'
        });

        assertCommercialDepartmentStore(tdsXml);
        assertDepartmentStore(tdsXml);
    });

    it('should translate TDSv71 building=commercial and shop=department_store point to AL013 with retail department store FFNs', function () {

        var data = '<osm version="0.6" upload="true" generator="hootenanny">\
                        <node id="-10" action="modify" visible="true" lat="0.68307256979" lon="18.45073925651">\
                            <tag k="building" v="commercial"/>\
                            <tag k="shop" v="department_store"/>\
                        </node>\
                    </osm>';

        var tdsXml = server.handleInputs({
            osm: data,
            method: 'POST',
            translation: 'TDSv71',
            path: '/translateTo'
        });

        assertCommercialDepartmentStore(tdsXml);
        assertDepartmentStore(tdsXml);
    });

    it('should translate TDSv71 AL013 with FFN=440, FFN2=460, and FFN3=465 to building=commercial and shop=department_store', function () {

        var data = '<osm version="0.6" upload="true" generator="hootenanny">\
                        <node id="-10" action="modify" visible="true" lat="0.68307256979" lon="18.45073925651">\
                            <tag k="F_CODE" v="AL013"/>\
                            <tag k="FFN" v="440"/>\
                            <tag k="FFN2" v="460"/>\
                            <tag k="FFN3" v="465"/>\
                        </node>\
                    </osm>';

        assertDepartmentStore(data);
    });
});
