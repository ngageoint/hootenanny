var assert = require('assert'),
    osmtogeojson = require('osmtogeojson'),
    DOMParser = new require('@xmldom/xmldom').DOMParser,
    parser = new DOMParser();

var server = require('../TranslationServer.js');

describe('Railway Gauge', function () {
    var schemas = [
        'TDSv71',
        'TDSv70',
        'TDSv61',
        'TDSv40',
        'MGCP',
        'GGDMv30'
    ];

    var gauges = ['standard', '1435'];

    schemas.forEach(schema => {
        gauges.forEach(gauge => {
            it('should translate "' + gauge + '" gauge value to ' + schema, function() {
                var railway = '<osm version="0.6" upload="true" generator="hootenanny">\
                <node id="-10" lat="0.68307256979" lon="18.45073925651" />\
                <node id="-11" lat="0.68341620728" lon="18.45091527847" />\
                <node id="-12" lat="0.68306209303" lon="18.45157116983" />\
                <node id="-13" lat="0.68270797876" lon="18.45141400736" />\
                <way id="-19" >\
                <nd ref="-10" />\
                <nd ref="-11" />\
                <nd ref="-12" />\
                <nd ref="-13" />\
                <nd ref="-10" />\
                <tag k="railway" v="rail"/>\
                <tag k="gauge" v="' + gauge + '"/>\
                <tag k="bridge" v="yes"/>\
                </way></osm>';

                var osm_xml = server.handleInputs({osm: railway, method: 'POST', translation: schema, path: '/translateTo'});
                // console.log(osm_xml);
                var xml = parser.parseFromString(osm_xml);
                assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), schema);

                var tags = osmtogeojson(xml).features[0].properties;

                //We don't really care about verifying tags
                //but this translation should produce both a rail and a bridge feature
                //and it seems non-deterministic which one is returned
                var fcode = (schema == 'MGCP') ? 'FCODE' : 'F_CODE';
                assert.equal(tags[fcode] === 'AN010' || tags[fcode] === 'AQ040', true);
            });
        });
    });
});
