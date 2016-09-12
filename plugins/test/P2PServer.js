var assert = require('assert'),
    http = require('http'),
    xml2js = require('xml2js');
var httpMocks = require('node-mocks-http');
var server = require('../P2PServer.js');

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

describe('P2PServer', function () {
    it('responds with HTTP 404 if url not found', function() {
        var request  = httpMocks.createRequest({
            method: 'GET',
            url: '/foo'
        });
        var response = httpMocks.createResponse();
        server.P2Pserver(request, response);
        assert.equal(response.statusCode, '404');
    });
    it('repsonds with HTTP 400 if unsupported method', function() {
        var request  = httpMocks.createRequest({
            method: 'GET',
            url: '/p2pmerge'
        });
        var response = httpMocks.createResponse();
        server.P2Pserver(request, response);
        assert.equal(response.statusCode, '400');
    });

    it('merges two osm poi', function() {

        var request  = httpMocks.createRequest({
            method: 'POST',
            url: '/p2pmerge',
            body: input
        });
        var response = httpMocks.createResponse();
        server.P2Pserver(request, response);
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
                path: '/p2pmerge',
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
                    path: '/p2pmerge'
                })
            }, Error, 'Unsupported method');
        });
    });

});
