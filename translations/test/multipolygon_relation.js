var assert = require('assert'),
    DOMParser = new require('xmldom').DOMParser,
    parser = new DOMParser();

var server = require('../TranslationServer.js');

describe('TranslationServer', function () {

    it('should translate a multipolygon relation from osm -> mcgp with error', function() {

        var data = '<osm version="0.6" upload="true" generator="hootenanny">\
                        <way id="-19" visible="true">\
                            <nd ref="-10" />\
                            <nd ref="-11" />\
                            <nd ref="-12" />\
                            <nd ref="-13" />\
                            <nd ref="-10" />\
                        </way>\
                        <way id="-20" visible="true">\
                            <nd ref="-3" />\
                            <nd ref="-4" />\
                            <nd ref="-5" />\
                            <nd ref="-6" />\
                            <nd ref="-3" />\
                        </way>\
                        <relation visible="true" id="-38">\
                        <member type="way" ref="-19" role="outer"/>\
                        <member type="way" ref="-20" role="outer"/>\
                        <tag k="type" v="multipolygon"/>\
                      </relation>\
                    </osm>';

        var osm_xml = server.handleInputs({
            osm: data,
            method: 'POST',
            translation: 'MGCP',
            path: '/translateTo'
        });

        // console.log(osm_xml);

        var xml = parser.parseFromString(osm_xml);

        assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "MGCP");
        assert.equal(xml.getElementsByTagName("tag")[0].getAttribute("k"), "error");
        assert.equal(xml.getElementsByTagName("tag")[0].getAttribute("v"), "No Valid Feature Code");

    });

    it('should translate an invalid tagged relation from osm -> mcgp with error', function() {

        var data = '<osm version="0.6" upload="true" generator="hootenanny">\
                        <way id="-19" visible="true">\
                            <nd ref="-10" />\
                            <nd ref="-11" />\
                            <nd ref="-12" />\
                            <nd ref="-13" />\
                            <nd ref="-10" />\
                        </way>\
                        <way id="-20" visible="true">\
                            <nd ref="-3" />\
                            <nd ref="-4" />\
                            <nd ref="-5" />\
                            <nd ref="-6" />\
                            <nd ref="-3" />\
                        </way>\
                        <relation visible="true" id="-38">\
                        <member type="way" ref="-19" role="outer"/>\
                        <member type="way" ref="-20" role="outer"/>\
                        <tag k="foo" v="bar"/>\
                      </relation>\
                    </osm>';

        var osm_xml = server.handleInputs({
            osm: data,
            method: 'POST',
            translation: 'MGCP',
            path: '/translateTo'
        });

        // console.log(osm_xml);

        var xml = parser.parseFromString(osm_xml);

        assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "MGCP");
        assert.equal(xml.getElementsByTagName("tag")[0].getAttribute("k"), "error");
        assert.equal(xml.getElementsByTagName("tag")[0].getAttribute("v"), "No Valid Feature Code");

    });

    it('should translate an invalid tagged way from osm -> mcgp with error', function() {

        var data = '<osm version="0.6" upload="true" generator="hootenanny">\
                        <way id="-19" visible="true">\
                            <nd ref="-10" />\
                            <nd ref="-11" />\
                            <nd ref="-12" />\
                            <nd ref="-13" />\
                            <nd ref="-10" />\
                            <tag k="foo" v="bar"/>\
                        </way>\
                    </osm>';

        var osm_xml = server.handleInputs({
            osm: data,
            method: 'POST',
            translation: 'MGCP',
            path: '/translateTo'
        });

        // console.log(osm_xml);

        var xml = parser.parseFromString(osm_xml);

        assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "MGCP");
        assert.equal(xml.getElementsByTagName("tag")[0].getAttribute("k"), "error");
        assert.equal(xml.getElementsByTagName("tag")[0].getAttribute("v"), "No Valid Feature Code");

    });

});
