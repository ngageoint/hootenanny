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

    it('should translate ZI001_SDP and ZI001_SDV from tdsv61 -> osm', function() {

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
                            <tag k="F_CODE" v="AL013"/>\
                            <tag k="ZI001_SDP" v="DigitalGlobe"/>\
                            <tag k="ZI001_SDV" v="2017-11-02 06:17:23"/>\
                        </way>\
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
        assert.equal(tags['building'], 'yes');
        assert.equal(tags['source'], 'DigitalGlobe');
        assert.equal(tags['source:datetime'], '2017-11-02 06:17:23');

    });


    it('should translate source and source:datetime to ZI001_SDP and ZI001_SDV osm -> tdsv61', function() {

        var osm_xml = '<osm version="0.6" upload="true" generator="hootenanny">\
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
                            <tag k="source" v="DigitalGlobe"/>\
                            <tag k="source:datetime" v="2017-11-02 06:17:23"/>\
                        </way>\
                    </osm>';
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
        assert.equal(tags["F_CODE"], 'AL013');
        assert.equal(tags["ZI001_SDP"], 'DigitalGlobe');
        assert.equal(tags["ZI001_SDV"], '2017-11-02 06:17:23');

    });

    it('should translate source:imagery and source:imagery:datetime to ZI001_SDP and ZI001_SDV osm -> tdsv61', function() {

        var osm_xml = '<osm version="0.6" upload="true" generator="hootenanny">\
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
                            <tag k="source:imagery" v="DigitalGlobe"/>\
                            <tag k="source:imagery:datetime" v="2017-11-02 06:17:23"/>\
                        </way>\
                    </osm>';
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
        assert.equal(tags["F_CODE"], 'AL013');
        assert.equal(tags["ZI001_SDP"], 'DigitalGlobe');
        assert.equal(tags["ZI001_SDV"], '2017-11-02 06:17:23');

    });

    it('should translate source over source:imagery to ZI001_SDP osm -> tdsv61', function() {

        var osm_xml = '<osm version="0.6" upload="true" generator="hootenanny">\
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
                            <tag k="source" v="OpenStreetMap"/>\
                            <tag k="source:imagery" v="DigitalGlobe"/>\
                        </way>\
                    </osm>';
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
        assert.equal(tags["F_CODE"], 'AL013');
        assert.equal(tags["ZI001_SDP"], 'OpenStreetMap');

    });

    it('should translate source:datetime over source:imagery:datetime to ZI001_SDV osm -> tdsv61', function() {

        var osm_xml = '<osm version="0.6" upload="true" generator="hootenanny">\
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
                            <tag k="source:datetime" v="2017-07-02"/>\
                            <tag k="source:imagery:datetime" v="2017-11-02 06:17:23"/>\
                        </way>\
                    </osm>';
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
        assert.equal(tags["F_CODE"], 'AL013');
        assert.equal(tags["ZI001_SDV"], '2017-07-02');

    });

        it('should NOT translate timestamp to source:datetime for osm -> tdsv61', function() {

            var data = '<osm version="0.6" generator="JOSM">\
                          <node id="5416906829" action="modify" visible="true" lat="0.68307256979" lon="18.45073925651" />\
                          <node id="5416906820" action="modify" visible="true" lat="0.68341620728" lon="18.45091527847" />\
                          <node id="5416906822" action="modify" visible="true" lat="0.68306209303" lon="18.45157116983" />\
                          <node id="5416906824" action="modify" visible="true" lat="0.68270797876" lon="18.45141400736" />\
                          <way id="561484830" timestamp="2018-10-25T15:14:15Z" uid="34" user="foo" visible="true" version="4" changeset="56384053">\
                            <nd ref="5416906829" />\
                            <nd ref="5416906820" />\
                            <nd ref="5416906822" />\
                            <nd ref="5416906824" />\
                            <nd ref="5416906829" />\
                            <tag k="building" v="yes" />\
                            <tag k="condition" v="functional" />\
                            <tag k="security:classification" v="UNCLASSIFIED" />\
                            <tag k="source" v="DigitalGlobe" />\
                            <tag k="source:imagery" v="DigitalGlobe" />\
                            <tag k="source:imagery:datetime" v="2017-08-13 17:43:33" />\
                            <tag k="source:imagery:id" v="6247f8c5bf2d66d9dbb458bce2c72078" />\
                            <tag k="source:imagery:layerName" v="2017-08-13 - WV02" />\
                            <tag k="source:imagery:sensor" v="WV02" />\
                            <tag k="use" v="residential" />\
                            <tag k="uuid" v="{77c030c5-9d33-4775-94c4-d34fd3d9b8f8}" />\
                          </way>\
                        </osm>';

            var osm_xml = server.handleInputs({
                osm: data,
                method: 'POST',
                translation: 'TDSv61',
                path: '/translateTo'
            });

            // console.log(osm_xml);

            var xml = parser.parseFromString(osm_xml);
            var gj = osmtogeojson(xml);

            assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "TDSv61");
            var tags = gj.features[0].properties;
            assert.equal(tags["ZI001_SDV"], "2017-08-13 17:43:33");


    });

});
