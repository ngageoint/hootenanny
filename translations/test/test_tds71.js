var assert = require('assert'),
    http = require('http'),
    xml2js = require('xml2js'),
    fs = require('fs'),
    httpMocks = require('node-mocks-http'),
    osmtogeojson = require('osmtogeojson'),
    DOMParser = new require('@xmldom/xmldom').DOMParser
    parser = new DOMParser();

var server = require('../TranslationServer.js');

describe('TDSv71', function () {
    this.timeout(5000);

    // Strings used to build OSM XML features
    var startNode = '<osm version="0.6" upload="true" generator="hootenanny">\
       <node id="-13" lat="0.68270797876" lon="18.45141400736" >';

    var startWay = '<osm version="0.6" upload="true" generator="hootenanny">\
       <node id="-10" lat="0.68307256979" lon="18.45073925651" /> <node id="-13" lat="0.68270797876" lon="18.45141400736" />\
       <way id="-19" >\
       <nd ref="-10" /> <nd ref="-13" />';

    var startArea = '<osm version="0.6" upload="true" generator="hootenanny">\
       <node id="-10" lat="0.68307256979" lon="18.45073925651" /> <node id="-11" lat="0.68341620728" lon="18.45091527847" />\
       <node id="-12" lat="0.68306209303" lon="18.45157116983" /> <node id="-13" lat="0.68270797876" lon="18.45141400736" />\
       <way id="-19" >\
       <nd ref="-10" /> <nd ref="-11" /> <nd ref="-12" /> <nd ref="-13" /> <nd ref="-10" />';

    var endNode = '</node></osm>';
    var endWay = '</way></osm>'; // NOTE: This is also for Areas as well

    it('should translate a mixture of valid and invalid tags from osm -> tdsv71', function() {
        var data = startArea + '<tag k="building" v="yes"/></way>\
                   <way id="-20" action="modify" visible="true"><nd ref="-3" /><nd ref="-4" /><nd ref="-5" /><nd ref="-6" /><nd ref="-3" />\
                   <tag k="use" v="residential"/>' + endWay;

        var osm_xml = server.handleInputs({osm: data, method: 'POST', translation: 'TDSv71', path: '/translateTo'});
        // console.log(osm_xml);
        var xml = parser.parseFromString(osm_xml);
        assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "TDSv71");
        assert.equal(xml.getElementsByTagName("tag")[0].getAttribute("k"), "error");
        assert.equal(xml.getElementsByTagName("tag")[0].getAttribute("v"), "No Valid Feature Code");
        assert.equal(xml.getElementsByTagName("tag")[1].getAttribute("k"), "F_CODE");
        assert.equal(xml.getElementsByTagName("tag")[1].getAttribute("v"), "AL013");
    });

    it('should translate another mixture of valid and invalid tags from osm -> tdsv71', function() {
        var data = startArea + '<tag k="use" v="residential"/></way>\
                   <way id="-20" action="modify" visible="true"><nd ref="-3" /><nd ref="-4" /><nd ref="-5" /><nd ref="-6" /><nd ref="-3" />\
                   <tag k="building" v="yes"/>' + endWay;

        var osm_xml = server.handleInputs({osm: data, method: 'POST', translation: 'TDSv71', path: '/translateTo'});
        // console.log(osm_xml);
        var xml = parser.parseFromString(osm_xml);
        assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "TDSv71");
        assert.equal(xml.getElementsByTagName("tag")[0].getAttribute("k"), "F_CODE");
        assert.equal(xml.getElementsByTagName("tag")[0].getAttribute("v"), "AL013");
        assert.equal(xml.getElementsByTagName("tag")[1].getAttribute("k"), "error");
        assert.equal(xml.getElementsByTagName("tag")[1].getAttribute("v"), "No Valid Feature Code");
    });

    it('should translate error from tdsv71 -> osm', function() {
        var data = startArea + '<tag k="error" v="No Valid Feature Code"/></way>\
                   <way id="-20" action="modify" visible="true"><nd ref="-3" /><nd ref="-4" /><nd ref="-5" /><nd ref="-6" /><nd ref="-3" />\
                   <tag k="tag1" v="{\'use\':\'residential\'}"/>' + endWay;

        var osm_xml = server.handleInputs({osm: data, method: 'POST', translation: 'TDSv71', path: '/translateFrom'});
        // console.log(osm_xml);
        var xml = parser.parseFromString(osm_xml);
        assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "OSM");
        assert.equal(xml.getElementsByTagName("tag")[0].getAttribute("k"), "tag1");
        assert.equal(xml.getElementsByTagName("tag")[0].getAttribute("v"), "{\'use\':\'residential\'}");
        assert.equal(xml.getElementsByTagName("tag")[1].getAttribute("k"), "error");
        assert.equal(xml.getElementsByTagName("tag")[1].getAttribute("v"), "No Valid Feature Code");
    });

    it('should translate Motor Vehicle Station (AQ170) from tdsv71 -> osm -> tdsv71', function() {
        var data = startArea + '<tag k="F_CODE" v="AQ170"/><tag k="FFN" v="470"/><tag k="FFN2" v="382"/>' + endWay;

        var osm_xml = server.handleInputs({osm: data, method: 'POST', translation: 'TDSv71', path: '/translateFrom'});
        // console.log(osm_xml);
        var xml = parser.parseFromString(osm_xml);
        assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "OSM");

        var tags = osmtogeojson(xml).features[0].properties;
        assert.equal(tags["amenity"], "fuel");
        assert.equal(tags["toilets"], "yes");

        var tds_xml = server.handleInputs({osm: osm_xml, method: 'POST', translation: 'TDSv71', path: '/translateTo'});
        // console.log(tds_xml);
        xml = parser.parseFromString(tds_xml);
        assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "TDSv71");

        var tags = osmtogeojson(xml).features[0].properties;
        assert.equal(tags["F_CODE"], "AQ170");
        assert.equal(tags["FFN"], "470");
        assert.equal(tags["FFN2"], "382");
    });

    it('should translate Water Park (F_CODE=AK030 & FFN=915) from tdsv71 -> osm -> tdsv71', function() {
        var data = startArea + '<tag k="F_CODE" v="AK030"/><tag k="FFN" v="915"/>' + endWay;

        var osm_xml = server.handleInputs({osm: data, method: 'POST', translation: 'TDSv71', path: '/translateFrom'});
        // console.log(osm_xml);
        var xml = parser.parseFromString(osm_xml);
        assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "OSM");

        var tags = osmtogeojson(xml).features[0].properties;
        assert.equal(tags['leisure'], 'water_park');

        var tds_xml = server.handleInputs({osm: osm_xml, method: 'POST', translation: 'TDSv71', path: '/translateTo'});
        // console.log(tds_xml);
        xml = parser.parseFromString(tds_xml);
        assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "TDSv71");

        var tags = osmtogeojson(xml).features[0].properties;
        assert.equal(tags["F_CODE"], 'AK030');
        assert.equal(tags["FFN"], '915');
    });

});
