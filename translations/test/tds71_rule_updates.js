var assert = require('assert'),
    osmtogeojson = require('osmtogeojson'),
    DOMParser = new require('@xmldom/xmldom').DOMParser,
    parser = new DOMParser();

var server = require('../TranslationServer.js');

function tagXml(tags) {
    return Object.keys(tags).map(function (key) {
        return '<tag k="' + key + '" v="' + tags[key] + '"/>';
    }).join('');
}

function areaXml(tags) {
    return '<osm version="0.6" upload="true" generator="hootenanny">\
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
                    ' + tagXml(tags) + '\
                </way>\
            </osm>';
}

function pointXml(tags) {
    return '<osm version="0.6" upload="true" generator="hootenanny">\
                <node id="-10" action="modify" visible="true" lat="0.68307256979" lon="18.45073925651">\
                    ' + tagXml(tags) + '\
                </node>\
            </osm>';
}

function lineXml(tags) {
    return '<osm version="0.6" upload="true" generator="hootenanny">\
                <node id="-10" action="modify" visible="true" lat="0.68307256979" lon="18.45073925651" />\
                <node id="-13" action="modify" visible="true" lat="0.68270797876" lon="18.45141400736" />\
                <way id="-19" action="modify" visible="true">\
                    <nd ref="-10" />\
                    <nd ref="-13" />\
                    ' + tagXml(tags) + '\
                </way>\
            </osm>';
}

function translateToTds(osmXml) {
    return server.handleInputs({
        osm: osmXml,
        method: 'POST',
        translation: 'TDSv71',
        path: '/translateTo'
    });
}

function translateFromTds(tdsXml) {
    return server.handleInputs({
        osm: tdsXml,
        method: 'POST',
        translation: 'TDSv71',
        path: '/translateFrom'
    });
}

function getTags(xmlStr, schema) {
    var xml = parser.parseFromString(xmlStr);
    var gj = osmtogeojson(xml);

    assert.strictEqual(xml.getElementsByTagName("osm")[0].getAttribute("schema"), schema);

    return gj.features[0].properties;
}

function assertForward(osmXml, expectedTags) {
    var tags = getTags(translateToTds(osmXml), 'TDSv71');

    Object.keys(expectedTags).forEach(function (key) {
        assert.strictEqual(tags[key], expectedTags[key]);
    });

    assert.strictEqual(tags['OTH'], undefined);
}

function assertRoundTrip(osmXml, expectedTags, removedTags) {
    var osmTags = getTags(translateFromTds(translateToTds(osmXml)), 'OSM');

    Object.keys(expectedTags).forEach(function (key) {
        assert.strictEqual(osmTags[key], expectedTags[key]);
    });

    removedTags.forEach(function (key) {
        assert.strictEqual(osmTags[key], undefined);
    });
}

describe('TDS71 Rule Updates', function () {

    it('should translate tourism=hotel and barrier=wall area to AL020 accommodation hotel', function () {
        var data = areaXml({
            'tourism': 'hotel',
            'barrier': 'wall'
        });

        assertForward(data, {
            'F_CODE': 'AL020',
            'FFN': '550',
            'FFN2': '551'
        });
        assertRoundTrip(data, {'tourism': 'hotel'}, ['barrier', 'landuse', 'use']);
    });

    it('should translate amenity=cinema and building=retail area to AL013 cinema', function () {
        var data = areaXml({
            'amenity': 'cinema',
            'building': 'retail'
        });

        assertForward(data, {
            'F_CODE': 'AL013',
            'FFN': '890',
            'FFN2': '594'
        });
        assertRoundTrip(data, {'amenity': 'cinema', 'building': 'retail'}, ['amenity:2', 'use', 'use:2']);
    });

    it('should translate man_made=wastewater_plant area to AL010 water treatment plant', function () {
        var data = areaXml({
            'man_made': 'wastewater_plant'
        });

        assertForward(data, {
            'F_CODE': 'AL010',
            'FFN': '350',
            'FFN2': '360',
            'FFN3': '362'
        });
        assertRoundTrip(data, {'man_made': 'wastewater_plant'}, ['facility', 'use', 'use:2', 'use:3']);
    });

    it('should translate amenity=exhibition_centre and building=commercial area to AL020 exhibition centre', function () {
        var data = areaXml({
            'amenity': 'exhibition_centre',
            'building': 'commercial'
        });

        assertForward(data, {
            'F_CODE': 'AL020',
            'FFN': '970',
            'FFN2': '579'
        });
        assertRoundTrip(data, {'amenity': 'exhibition_centre', 'building': 'commercial'}, ['landuse', 'use', 'amenity:2']);
    });

    it('should translate amenity=courthouse area to AL013 courthouse', function () {
        var data = areaXml({
            'amenity': 'courthouse'
        });

        assertForward(data, {
            'F_CODE': 'AL013',
            'FFN': '830',
            'FFN2': '831',
            'FFN3': '840'
        });
        assertRoundTrip(data, {'amenity': 'courthouse', 'building': 'yes'}, ['use', 'use:2', 'amenity:3']);
    });

    it('should translate landuse=civic_admin and office=government area to AL010 civic administration', function () {
        var data = areaXml({
            'landuse': 'civic_admin',
            'office': 'government'
        });

        assertForward(data, {
            'F_CODE': 'AL010',
            'FFN': '808',
            'FFN2': '811',
            'FFN3': '822'
        });
        assertRoundTrip(data, {'landuse': 'civic_admin', 'office': 'government'}, ['facility', 'use', 'use:2', 'use:3']);
    });

    it('should translate leisure=sports_centre area to AL010 sports centre', function () {
        var data = areaXml({
            'leisure': 'sports_centre'
        });

        assertForward(data, {
            'F_CODE': 'AL010',
            'FFN': '900',
            'FFN2': '912'
        });
        assertRoundTrip(data, {'leisure': 'sports_centre'}, ['facility', 'use', 'use:2']);

        data = areaXml({
            'leisure': 'sports_centre',
            'building': 'yes'
        });

        assertForward(data, {
            'F_CODE': 'AL010',
            'FFN': '900',
            'FFN2': '912'
        });
        assertRoundTrip(data, {'leisure': 'sports_centre'}, ['building', 'facility', 'use', 'use:2']);
    });

    it('should translate depot=bus and industrial=depot area to AL010 bus depot', function () {
        var data = areaXml({
            'depot': 'bus',
            'industrial': 'depot',
            'landuse': 'industrial'
        });

        assertForward(data, {
            'F_CODE': 'AL010',
            'FFN': '99',
            'FFN2': '340',
            'FFN3': '343'
        });
        assertRoundTrip(data, {'depot': 'bus', 'industrial': 'depot'}, ['facility', 'landuse', 'use', 'use:2', 'use:3', 'repair', 'repair:2', 'repair:3', 'shop', 'shop:2', 'shop:3']);
    });

    it('should drop problematic OSM tags before translating TDS71 sports centre areas', function () {
        var data = areaXml({
            'leisure': 'sports_centre',
            'building': 'part',
            'demolished:building': 'yes',
            'government': 'administrative',
            'wall': 'wall',
            'proposed:building': 'yes',
            'police': 'yes'
        });

        assertForward(data, {
            'F_CODE': 'AL010',
            'FFN': '900',
            'FFN2': '912'
        });
        assertRoundTrip(data, {'leisure': 'sports_centre', 'government': 'administrative'}, ['building', 'demolished:building', 'wall', 'proposed:building', 'police', 'facility', 'use', 'use:2']);
    });

    it('should translate waterway=drystream to dry hydrologic persistence', function () {
        var data = lineXml({
            'waterway': 'drystream'
        });
        var tags = getTags(translateToTds(data), 'TDSv71');

        assert.strictEqual(tags['F_CODE'], 'BH140');
        assert.strictEqual(tags['ZI024_HYP'], '4');
        assert.strictEqual(tags['WCC'], '999');
        assert.strictEqual(tags['OTH'], '(WCC:drystream)');
    });

    it('should translate lighting masts to AL110 in both directions', function () {
        var data = pointXml({
            'man_made': 'mast',
            'tower:type': 'lighting'
        });

        var tags = getTags(translateToTds(data), 'TDSv71');
        assert.strictEqual(tags['F_CODE'], 'AL110');
        assert.strictEqual(tags['TOS'], '6');
        assert.strictEqual(tags['TTC'], undefined);
        assert.strictEqual(tags['OSMTAGS'], undefined);

        assertRoundTrip(data, {'man_made': 'mast', 'tower:type': 'lighting'}, ['tower:shape']);
    });

    it('should translate masts to AL241 in both directions', function () {
        var data = pointXml({'man_made': 'mast'});

        var tags = getTags(translateToTds(data), 'TDSv71');
        assert.strictEqual(tags['F_CODE'], 'AL241');
        assert.strictEqual(tags['TOS'], '6');
        assert.strictEqual(tags['TTC'], undefined);
        assert.strictEqual(tags['OSMTAGS'], undefined);

        assertRoundTrip(data, {'man_made': 'mast'}, ['tower:shape', 'tower:type']);
    });

    it('should translate observation masts to AL241 with TTC=2 in both directions', function () {
        var data = pointXml({
            'man_made': 'mast',
            'tower:type': 'observation'
        });

        var tags = getTags(translateToTds(data), 'TDSv71');
        assert.strictEqual(tags['F_CODE'], 'AL241');
        assert.strictEqual(tags['TOS'], '6');
        assert.strictEqual(tags['TTC'], '2');
        assert.strictEqual(tags['TTC2'], undefined);
        assert.strictEqual(tags['OSMTAGS'], undefined);

        assertRoundTrip(data, {'man_made': 'mast', 'tower:type': 'observation'}, ['tower:shape']);
    });

    it('should preserve communication mast translation behavior', function () {
        var data = pointXml({
            'man_made': 'mast',
            'tower:type': 'communication'
        });

        var tags = getTags(translateToTds(data), 'TDSv71');
        assert.strictEqual(tags['F_CODE'], 'AL241');
        assert.strictEqual(tags['TOS'], '6');
        assert.strictEqual(tags['TTC'], '20');

        assertRoundTrip(data, {'man_made': 'mast', 'tower:type': 'communication'}, ['tower:shape']);
    });

    it('should not preserve a translated petroleum well in OSMTAGS', function () {
        var data = pointXml({'man_made': 'petroleum_well'});
        var tags = getTags(translateToTds(data), 'TDSv71');

        assert.strictEqual(tags['F_CODE'], 'AA054');
        assert.strictEqual(tags['OSMTAGS'], undefined);
    });
});
