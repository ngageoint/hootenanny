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

    it('should translate OTH from tdsv61 -> osm -> tdsv61', function() {

        var data = '<osm version="0.6" upload="true" generator="hootenanny">\
                        <node id="-19" lon="9.304397440128325" lat="41.65083522130027" version="0">\
                            <tag k="FCSUBTYPE" v="100080"/><tag k="ZI001_SDP" v="DigitalGlobe"/>\
                            <tag k="UFI" v="0d8b2563-81cf-44d4-8ef7-52c0e862651f"/>\
                            <tag k="F_CODE" v="AL010"/><tag k="ZI006_MEM" v="&lt;OSM&gt;{&quot;source:imagery:datetime&quot;:&quot;2017-11-11 10:45:15&quot;,&quot;source:imagery:sensor&quot;:&quot;WV02&quot;,&quot;source:imagery:id&quot;:&quot;756b80e1f695fb591caca8e7ce0f9ef5&quot;}&lt;/OSM&gt;"/>\
                            <tag k="ZSAX_RS0" v="U"/>\
                            <tag k="OTH" v="(FFN:foo)"/>\
                        </node>\
                    </osm>';

        var osm_xml = server.handleInputs({
            osm: data,
            method: 'POST',
            translation: 'TDSv61',
            path: '/translateFrom'
        });

        // console.log(osm_xml);

        var xml = parser.parseFromString(osm_xml);
        var gj = osmtogeojson(xml);

        assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "OSM");

        var tags = gj.features[0].properties;
        assert.equal(tags["facility"], "yes");
        assert.equal(tags["note:oth"], "(FFN:foo)");
        assert.equal(tags["security:classification"], "UNCLASSIFIED");
        assert.equal(tags["uuid"], "{0d8b2563-81cf-44d4-8ef7-52c0e862651f}");
        assert.equal(tags["source"], "DigitalGlobe");
        assert.equal(tags["source:imagery:id"], "756b80e1f695fb591caca8e7ce0f9ef5");
        assert.equal(tags["source:imagery:datetime"], "2017-11-11 10:45:15");
        assert.equal(tags["source:imagery:sensor"], "WV02");

        var tds_xml = server.handleInputs({
            osm: osm_xml,
            method: 'POST',
            translation: 'TDSv61',
            path: '/translateTo'
        });

        // console.log(tds_xml);
        
        xml = parser.parseFromString(tds_xml);
        gj = osmtogeojson(xml);

        assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "TDSv61");

        var tags = gj.features[0].properties;
        assert.equal(tags["F_CODE"], "AL010");
        assert.equal(tags["ZI001_SDP"], "DigitalGlobe");
        assert.equal(tags["ZSAX_RS0"], "U");
        assert.equal(tags["ZI006_MEM"], "<OSM>{\"source:imagery:sensor\":\"WV02\",\"source:imagery:id\":\"756b80e1f695fb591caca8e7ce0f9ef5\",\"source:imagery:datetime\":\"2017-11-11 10:45:15\"}</OSM>");
        assert.equal(tags["OTH"], "(FFN:foo)");
        assert.equal(tags["UFI"], "0d8b2563-81cf-44d4-8ef7-52c0e862651f");

    });

});

