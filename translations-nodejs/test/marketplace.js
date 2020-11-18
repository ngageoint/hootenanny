var assert = require('assert'),
    xml2js = require('xml2js'),
    DOMParser = new require('xmldom').DOMParser
    parser = new DOMParser();

var server = require('../TranslationServer.js');

describe('TranslationServer maintains Building and Facilty with same FFN', function () {
    this.timeout(5000);

    var fcodes = [
        'AL010',
        'AL013'
    ];

    var ffns = [272, 343, 459, 460, 464, 465, 466, 474, 475, 476, 478, 640, 691, 701, 706, 711, 714, 717, 752, 760, 770, 775, 801, 810, 811, 812, 813, 814, 818, 819, 961, 962]

    var schemas = [
        'TDSv70',
        'TDSv61',
        'TDSv40',
        // 'MGCP',
        'GGDMv30'
    ]

    schemas.forEach(schema => {
        var fcode_key = (schema === 'MGCP') ? 'FCODE' : 'F_CODE';
        fcodes.forEach(fcode => {
            if (schema === 'MGCP' && fcode === 'AL013') fcode = 'AL015';
            ffns.forEach(ffn => {

            it('should translate ' + fcode_key + '=' + fcode + ' and FFN=' + ffn + ' from ' + schema + ' -> osm -> ' + schema, function() {

                var tds_xml = '<osm version="0.6">\
                                <way id="-19">\
                                    <nd ref="-10" />\
                                    <nd ref="-11" />\
                                    <nd ref="-12" />\
                                    <nd ref="-13" />\
                                    <nd ref="-10" />\
                                    <tag k="' + fcode_key + '" v="' + fcode + '"/>\
                                    <tag k="FFN" v="' + ffn + '"/>\
                                </way>\
                            </osm>';

                var osm_xml = server.handleInputs({
                    osm: tds_xml,
                    method: 'POST',
                    translation: schema,
                    path: '/translateFrom'
                });

                // console.log(osm_xml);

                var tds_xml = server.handleInputs({
                    osm: osm_xml,
                    method: 'POST',
                    translation: schema,
                    path: '/translateTo'
                });

                // console.log(tds_xml);

                xml = parser.parseFromString(tds_xml);

                for (var i=0; i < xml.getElementsByTagName("tag").length; i++) {
                    var tag = xml.getElementsByTagName("tag")[i];
                    if (tag.getAttribute("k") === fcode_key) {
                        assert.equal(tag.getAttribute("v"), fcode);
                        continue;
                    }
                    if (tag.getAttribute("k") === "FFN") {
                        assert.equal(tag.getAttribute("v"), ffn);
                    }
                }
                assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), schema);

            });
        });
        });
    });
});

