var assert = require('assert'),
    http = require('http'),
    xml2js = require('xml2js');
var httpMocks = require('node-mocks-http');
var server = require('../ElementMergeServer.js');

var input = "<?xml version='1.0' encoding='UTF-8'?>\
    <osm version='0.6' upload='true' generator='hootenanny'>\
    <node id='-3200079' visible='true' lat='48.0479399' lon='11.7012814'>\
    <tag k='amenity' v='post_office' />\
    <tag k='error:circular' v='15' />\
    <tag k='hoot:status' v='1' />\
    </node>\
    <node id='-3200083' visible='true' lat='48.04854' lon='11.70194'>\
    <tag k='amenity' v='post_office' />\
    <tag k='error:circular' v='150' />\
    <tag k='name' v='POST, JÄGER-VON-FALL-STRASSE' />\
    <tag k='hoot:status' v='2' />\
    </node>\
    </osm>";

var input2 = "<?xml version='1.0' encoding='UTF-8'?>\
    <osm version='0.6' upload='true' generator='hootenanny'>\
    <node visible='true' id='-270' lat='-0.3636403619999555' lon='42.5439798030000702'/>\
    <node visible='true' id='-269' lat='-0.3636266159999764' lon='42.5440572520000586'/>\
    <node visible='true' id='-268' lat='-0.3643189609999239' lon='42.5441789570000424'/>\
    <node visible='true' id='-267' lat='-0.3643228169999588' lon='42.5441571640000689'/>\
    <node visible='true' id='-266' lat='-0.3644458629999577' lon='42.5441787890000569'/>\
    <node visible='true' id='-265' lat='-0.3644557539999482' lon='42.5441231330000278'/>\
    <node visible='true' id='-530' lat='-0.3635327329999427' lon='42.5442892910000410'>\
        <tag k='building' v='yes'/>\
        <tag k='amenity' v='prison'/>\
        <tag k='note' v='poi1'/>\
        <tag k='error:circular' v='100'/>\
    </node>\
    <way visible='true' id='-56'>\
        <nd ref='-270'/>\
        <nd ref='-269'/>\
        <nd ref='-268'/>\
        <nd ref='-267'/>\
        <nd ref='-266'/>\
        <nd ref='-265'/>\
        <nd ref='-270'/>\
        <tag k='building' v='yes'/>\
        <tag k='amenity' v='prison'/>\
        <tag k='note' v='building1'/>\
        <tag k='error:circular' v='15'/>\
    </way>\
</osm>";

describe('ElementMergeServer', function () {
    it('responds with HTTP 404 if url not found', function() {
        var request  = httpMocks.createRequest({
            method: 'GET',
            url: '/foo'
        });
        var response = httpMocks.createResponse();
        server.ElementMergeserver(request, response);
        assert.equal(response.statusCode, '404');
    });
    it('repsonds with HTTP 400 if unsupported method', function() {
        var request  = httpMocks.createRequest({
            method: 'GET',
            url: '/elementmerge'
        });
        var response = httpMocks.createResponse();
        server.ElementMergeserver(request, response);
        assert.equal(response.statusCode, '400');
    });

    it('merges two osm poi', function() {

        var request  = httpMocks.createRequest({
            method: 'POST',
            url: '/elementmerge',
            body: input
        });
        var response = httpMocks.createResponse();
        server.ElementMergeserver(request, response);
        assert.equal(response.statusCode, '200');
        var body = '';
        response.on('data', function(chunk){
            body += chunk;
        })
        response.on('end', function(body){
            xml2js.parseString(body, function(err, result) {
                if (err) console.error(err);
                assert.equal(result.osm.node[0].$.lat, "48.0479399000000029");
                assert.equal(result.osm.node[0].$.lon, "11.7012813999999992");
                assert.equal(result.osm.node[0].tag[0].$.k, "hoot:status");
                assert.equal(result.osm.node[0].tag[0].$.v, "1");
                assert.equal(result.osm.node[0].tag[1].$.k, "name");
                assert.equal(result.osm.node[0].tag[1].$.v, "POST, JÄGER-VON-FALL-STRASSE");
                assert.equal(result.osm.node[0].tag[2].$.k, "amenity");
                assert.equal(result.osm.node[0].tag[2].$.v, "post_office");
            });
        });
    });

    describe('handleInputs', function() {
        it('merges two osm poi', function() {
            var merged = server.handleInputs({
                path: '/elementmerge',
                method: 'POST',
                osm: input
            });

            xml2js.parseString(merged, function(err, result) {
                if (err) console.error(err);
                assert.equal(result.osm.node[0].$.lat, "48.0479399000000029");
                assert.equal(result.osm.node[0].$.lon, "11.7012813999999992");
                assert.equal(result.osm.node[0].tag[0].$.k, "hoot:status");
                assert.equal(result.osm.node[0].tag[0].$.v, "3");
                assert.equal(result.osm.node[0].tag[1].$.k, "name");
                assert.equal(result.osm.node[0].tag[1].$.v, "POST, JÄGER-VON-FALL-STRASSE");
                assert.equal(result.osm.node[0].tag[2].$.k, "amenity");
                assert.equal(result.osm.node[0].tag[2].$.v, "post_office");
            });
        });

        it('throws error if url not found', function() {
            assert.throws(function error() {
                server.handleInputs({
                    method: 'POST',
                    path: '/foo'
                })
            }, Error, 'Not found');
        });

        it('throws error if unsupported method', function() {
            assert.throws(function error() {
                server.handleInputs({
                    method: 'GET',
                    path: '/elementmerge'
                })
            }, Error, 'Unsupported method');
        });
    });

    describe('handleInputs', function() {
        it('merges an osm poi and a way polygon', function() {
            var merged = server.handleInputs({
                path: '/elementmerge',
                method: 'POST',
                osm: input2
            });

            xml2js.parseString(merged, function(err, result) {
                if (err) console.error(err);
                console.log(result);
                //assert.equal(result.osm.way[0].tag[0].$.k, "hoot:status");
                //assert.equal(result.osm.way[0].tag[0].$.v, "3");
                assert.equal(result.osm.way[0].tag[0].$.k, "note");
                assert.equal(result.osm.way[0].tag[0].$.v, "poi1;building1");
                assert.equal(result.osm.way[0].tag[1].$.k, "building");
                assert.equal(result.osm.way[0].tag[1].$.v, "yes");
                assert.equal(result.osm.way[0].tag[2].$.k, "amenity");
                assert.equal(result.osm.way[0].tag[2].$.v, "prison");
            });
        });

        it('throws error if url not found', function() {
            assert.throws(function error() {
                server.handleInputs({
                    method: 'POST',
                    path: '/foo'
                })
            }, Error, 'Not found');
        });

        it('throws error if unsupported method', function() {
            assert.throws(function error() {
                server.handleInputs({
                    method: 'GET',
                    path: '/elementmerge'
                })
            }, Error, 'Unsupported method');
        });
    });

});
