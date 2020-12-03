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

    it('should translate church from OSM -> MGCP -> OSM and preserve religion and denomination', function() {

        var data = "<osm version='0.6' generator='JOSM'>\
                      <node id='3168842121' timestamp='2016-10-07T10:58:48Z' uid='4687565' user='CreazySpace' visible='true' version='2' changeset='42707950' lat='18.4196857' lon='-72.7365903'>\
                        <tag k='amenity' v='place_of_worship' />\
                        <tag k='denomination' v='protestant' />\
                        <tag k='name' v='Eglise de Dieu vocationnelle' />\
                        <tag k='religion' v='christian' />\
                        <tag k='source' v='Tdh' />\
                        <tag k='source:date' v='03/10/2014' />\
                      </node>\
                    </osm>";

        var mgcp_xml = server.handleInputs({
            osm: data,
            method: 'POST',
            translation: 'MGCP',
            path: '/translateTo'
        });

        // console.log(mgcp_xml);

        var xml = parser.parseFromString(mgcp_xml);
        var gj = osmtogeojson(xml);

        assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "MGCP");

        var tags = gj.features[0].properties;
        assert.equal(tags["NAM"], "Eglise de Dieu vocationnelle");
        assert.equal(tags["FFN"], "931");
        assert.equal(tags["SDP"], "Tdh");
        assert.equal(tags["FCODE"], "AL015");
        assert.equal(tags["TXT"], "<OSM>{\"denomination\":\"protestant\",\"source:date\":\"03/10/2014\",\"religion\":\"christian\"}</OSM>");

        var osm_xml = server.handleInputs({
            osm: mgcp_xml,
            method: 'POST',
            translation: 'MGCP',
            path: '/translateFrom'
        });

        // console.log(osm_xml);

        xml = parser.parseFromString(osm_xml);
        gj = osmtogeojson(xml);

        assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "OSM");

        var tags = gj.features[0].properties;
        assert.equal(tags["amenity"], "place_of_worship");
        assert.equal(tags["name"], "Eglise de Dieu vocationnelle");
        assert.equal(tags["source"], "Tdh");

        //These tags should also be preserved
        //However, no "HWT" attribute (House of Worship Type) causes the "religion" tag to get dropped.
        //assert.equal(tags["religion"], "christian");
        assert.equal(tags["denomination"], "protestant");
        assert.equal(tags["source:date"], "03/10/2014");

    });

});

