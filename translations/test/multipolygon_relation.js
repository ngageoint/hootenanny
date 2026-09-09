var assert = require('assert'),
    DOMParser = new require('xmldom').DOMParser,
    parser = new DOMParser();

var server = require('../TranslationServer.js');

describe('TranslationServer', function () {

    it('should translate a multipolygon relation from osm -> mcgp with error', function() {

        var data = "<?xml version='1.0' encoding='UTF-8'?> <osm version='0.6' generator='JOSM'> \
          <node id='-101752' action='modify' visible='true' lat='0.0260960581' lon='-0.03260884481' /> \
          <node id='-101754' action='modify' visible='true' lat='0.00696194343' lon='-0.03808856805' /> \
          <node id='-101756' action='modify' visible='true' lat='-0.0042669976' lon='-0.026859627' /> \
          <node id='-101757' action='modify' visible='true' lat='-0.00552463899' lon='0.00936493684' /> \
          <node id='-101758' action='modify' visible='true' lat='0.01693324286' lon='0.01068995188' /> \
          <node id='-101761' action='modify' visible='true' lat='0.01540610694' lon='-0.02317653433' /> \
          <node id='-101762' action='modify' visible='true' lat='0.00920773162' lon='-0.02641046935' /> \
          <node id='-101763' action='modify' visible='true' lat='0.00768059566' lon='-0.0170679904' /> \
          <node id='-101765' action='modify' visible='true' lat='0.01226200353' lon='-0.00494073406' /> \
          <node id='-101766' action='modify' visible='true' lat='0.00884840551' lon='0.00458140795' /> \
          <node id='-101767' action='modify' visible='true' lat='0.00399750301' lon='-0.00314410349' /> \
          <way id='-101782' action='modify' visible='true'> \
            <nd ref='-101752' /> <nd ref='-101754' /> <nd ref='-101756' /> <nd ref='-101757' /> <nd ref='-101758' /> <nd ref='-101752' /> \
          </way> \
          <way id='-101790' action='modify' visible='true'> \
            <nd ref='-101761' /> <nd ref='-101762' /> <nd ref='-101763' /> <nd ref='-101761' /> \
          </way> \
          <way id='-101793' action='modify' visible='true'> \
            <nd ref='-101765' /> <nd ref='-101766' /> <nd ref='-101767' /> <nd ref='-101765' /> \
          </way> \
          <relation id='-99748' action='modify' visible='true'> \
            <member type='way' ref='-101790' role='inner' /> \
            <member type='way' ref='-101793' role='inner' /> \
            <member type='way' ref='-101782' role='outer' /> \
            <tag k='type' v='multipolygon' /> \
          </relation> \
        </osm>";

        var osm_xml = server.handleInputs({
            osm: data,
            method: 'POST',
            translation: 'MGCP',
            path: '/translateTo'
        });

        // console.log(osm_xml);

        var xml = parser.parseFromString(osm_xml);

        assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "MGCP");

        // We expect to see  "type=multipolygon" as the ONLY tag
        assert.equal(xml.getElementsByTagName("tag")[0].getAttribute("k"), "type");
        assert.equal(xml.getElementsByTagName("tag")[0].getAttribute("v"), "multipolygon");

        assert.equal(xml.getElementsByTagName("tag").length,1);
    });

    it('should translate an invalid tagged relation from osm -> mcgp with error', function() {

        var data = "<?xml version='1.0' encoding='UTF-8'?> <osm version='0.6' generator='JOSM'> \
          <node id='-101752' action='modify' visible='true' lat='0.0260960581' lon='-0.03260884481' /> \
          <node id='-101754' action='modify' visible='true' lat='0.00696194343' lon='-0.03808856805' /> \
          <node id='-101756' action='modify' visible='true' lat='-0.0042669976' lon='-0.026859627' /> \
          <node id='-101757' action='modify' visible='true' lat='-0.00552463899' lon='0.00936493684' /> \
          <node id='-101758' action='modify' visible='true' lat='0.01693324286' lon='0.01068995188' /> \
          <node id='-101761' action='modify' visible='true' lat='0.01540610694' lon='-0.02317653433' /> \
          <node id='-101762' action='modify' visible='true' lat='0.00920773162' lon='-0.02641046935' /> \
          <node id='-101763' action='modify' visible='true' lat='0.00768059566' lon='-0.0170679904' /> \
          <node id='-101765' action='modify' visible='true' lat='0.01226200353' lon='-0.00494073406' /> \
          <node id='-101766' action='modify' visible='true' lat='0.00884840551' lon='0.00458140795' /> \
          <node id='-101767' action='modify' visible='true' lat='0.00399750301' lon='-0.00314410349' /> \
          <way id='-101782' action='modify' visible='true'> \
            <nd ref='-101752' /> <nd ref='-101754' /> <nd ref='-101756' /> <nd ref='-101757' /> <nd ref='-101758' /> <nd ref='-101752' /> \
          </way> \
          <way id='-101790' action='modify' visible='true'> \
            <nd ref='-101761' /> <nd ref='-101762' /> <nd ref='-101763' /> <nd ref='-101761' /> \
          </way> \
          <way id='-101793' action='modify' visible='true'> \
            <nd ref='-101765' /> <nd ref='-101766' /> <nd ref='-101767' /> <nd ref='-101765' /> \
          </way> \
          <relation id='-99748' action='modify' visible='true'> \
            <member type='way' ref='-101790' role='inner' /> \
            <member type='way' ref='-101793' role='inner' /> \
            <member type='way' ref='-101782' role='outer' /> \
            <tag k='type' v='multipolygon' /> \
            <tag k='foo' v='bar' /> \
          </relation> \
        </osm>";

        var osm_xml = server.handleInputs({
            osm: data,
            method: 'POST',
            translation: 'MGCP',
            path: '/translateTo'
        });

        // console.log(osm_xml);

        var xml = parser.parseFromString(osm_xml);

        assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "MGCP");
        // NOTE: tag[0] == "type=multipolygon"
        assert.equal(xml.getElementsByTagName("tag")[1].getAttribute("k"), "error");
        assert.equal(xml.getElementsByTagName("tag")[1].getAttribute("v"), "No Valid Feature Code");

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
