var assert = require('assert'),
    http = require('http'),
    xml2js = require('xml2js'),
    fs = require('fs'),
    httpMocks = require('node-mocks-http'),
    osmtogeojson = require('osmtogeojson'),
    DOMParser = new require('xmldom').DOMParser
    parser = new DOMParser();

var server = require('../TranslationServer.js');

describe('TranslationServer', function () {

    var cases = {
        // GB055: {aeroway: 'runway'},
        // AD030: {power: 'substation'},
        // AQ170: {ford: 'yes'},
        BH140: {waterway: 'riverbank'}
    }

    Object.keys(cases).forEach(k => {

        var fcode = k;
        var tag = cases[k];
        var tagKey = Object.keys(tag)[0];

        it('should translate ' + fcode + ' from tdsv61 -> osm -> tdsv61', function() {

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
                                <tag k="F_CODE" v="' + fcode + '"/>\
                                <tag k="ZI006_MEM" v="&lt;OSM&gt;{&quot;area&quot;:&quot;yes&quot;}&lt;/OSM&gt;"/>\
                            </way>\
                        </osm>';

            var osm_xml = server.handleInputs({
                osm: data,
                method: 'POST',
                translation: 'TDSv61',
                path: '/translateFrom'
            });

            console.log(osm_xml);

            var xml = parser.parseFromString(osm_xml);
            var gj = osmtogeojson(xml);

            assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "OSM");

            var tags = gj.features[0].properties;
            assert.equal(tags[tagKey], tag[tagKey]);
            assert.equal(tags.area, "yes");

            var tds_xml = server.handleInputs({
                osm: osm_xml,
                method: 'POST',
                translation: 'TDSv61',
                path: '/translateTo'
            });

            console.log(tds_xml);
            
            xml = parser.parseFromString(tds_xml);
            gj = osmtogeojson(xml);

            assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "TDSv61");

            var tags = gj.features[0].properties;
            assert.equal(tags["F_CODE"], fcode);
            assert.equal(tags["ZI006_MEM"], '<OSM>{"area":"yes"}</OSM>');


            osm_xml = server.handleInputs({
                osm: xml,
                method: 'POST',
                translation: 'TDSv61',
                path: '/translateFrom'
            });

            console.log(osm_xml);

            var xml = parser.parseFromString(osm_xml);
            var gj = osmtogeojson(xml);

            assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "OSM");

            var tags = gj.features[0].properties;
            assert.equal(tags[tagKey], tag[tagKey]);
            assert.equal(tags.area, "yes");

        });

    });



    // it('should translate Water Park (F_CODE=AK030 & FFN=915) from tdsv61 -> osm -> tdsv61', function() {

    //     var data = '<osm version="0.6" upload="true" generator="hootenanny">\
    //                     <node id="-10" action="modify" visible="true" lat="0.68307256979" lon="18.45073925651" />\
    //                     <node id="-11" action="modify" visible="true" lat="0.68341620728" lon="18.45091527847" />\
    //                     <node id="-12" action="modify" visible="true" lat="0.68306209303" lon="18.45157116983" />\
    //                     <node id="-13" action="modify" visible="true" lat="0.68270797876" lon="18.45141400736" />\
    //                     <way id="-19" action="modify" visible="true">\
    //                         <nd ref="-10" />\
    //                         <nd ref="-11" />\
    //                         <nd ref="-12" />\
    //                         <nd ref="-13" />\
    //                         <nd ref="-10" />\
    //                         <tag k="F_CODE" v="AK030"/>\
    //                         <tag k="FFN" v="915"/>\
    //                     </way>\
    //                 </osm>';

    //     var osm_xml = server.handleInputs({
    //         osm: data,
    //         method: 'POST',
    //         translation: 'TDSv61',
    //         path: '/translateFrom'
    //     });

    //     // console.log(osm_xml);

    //     var xml = parser.parseFromString(osm_xml);
    //     var gj = osmtogeojson(xml);

    //     assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "OSM");

    //     var tags = gj.features[0].properties;
    //     assert.equal(tags['leisure'], 'water_park');

    //     var tds_xml = server.handleInputs({
    //         osm: osm_xml,
    //         method: 'POST',
    //         translation: 'TDSv61',
    //         path: '/translateTo'
    //     });

    //     // console.log(tds_xml);
        
    //     xml = parser.parseFromString(tds_xml);
    //     gj = osmtogeojson(xml);

    //     assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "TDSv61");

    //     var tags = gj.features[0].properties;
    //     assert.equal(tags["F_CODE"], 'AK030');
    //     assert.equal(tags["FFN"], '915');

    // });


});
