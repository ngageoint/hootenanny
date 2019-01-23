var assert = require('assert'),
    http = require('http'),
    xml2js = require('xml2js');
var httpMocks = require('node-mocks-http');
var server = require('../ElementMergeServer.js');

var poiToPoiInput =
    "<?xml version='1.0' encoding='UTF-8'?>\
     <osm version='0.6' upload='true' generator='hootenanny'>\
       <node id='-3200079' visible='true' lat='48.0479399' lon='11.7012814'>\
         <tag k='amenity' v='post_office' />\
         <tag k='error:circular' v='15' />\
         <tag k='hoot:status' v='1' />\
         <tag k='hoot:merge:target' v='yes' />\
       </node>\
       <node id='-3200083' visible='true' lat='48.04854' lon='11.70194'>\
         <tag k='amenity' v='post_office' />\
         <tag k='error:circular' v='150' />\
         <tag k='name' v='POST, JÄGER-VON-FALL-STRASSE' />\
         <tag k='hoot:status' v='2' />\
       </node>\
     </osm>";

var poiToPolyInput =
    "<?xml version='1.0' encoding='UTF-8'?>\
     <osm version='0.6' upload='true' generator='hootenanny'>\
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

var areaToAreaInput =
    "<?xml version='1.0' encoding='UTF-8'?>\
     <osm version='0.6' generator='JOSM'>\
     <bounds minlat='37.7081' minlon='-122.5118886' maxlat='37.8106' maxlon='-122.376' origin='hootenanny' />\
       <relation visible='true' id='-37' timestamp='1970-01-01T00:00:00Z' version='1'>\
         <member type='way' ref='-1991' role='outer'/>\
         <member type='way' ref='-1992' role='inner'/>\
         <tag k='area' v='yes'/>\
         <tag k='name' v='Mt. Davidson Park'/>\
         <tag k='type' v='multipolygon'/>\
         <tag k='hoot:status' v='2'/>\
         <tag k='error:circular' v='15'/>\
         <tag k='hoot:merge:target' v='yes'/>\
       </relation>\
       <relation visible='true' id='-38' timestamp='1970-01-01T00:00:00Z' version='1'>\
         <member type='way' ref='-1991' role='outer'/>\
         <member type='way' ref='-1992' role='inner'/>\
         <tag k='area' v='yes'/>\
         <tag k='name' v='Mt. Davidson Park 2'/>\
         <tag k='type' v='multipolygon'/>\
         <tag k='hoot:status' v='1'/>\
         <tag k='error:circular' v='15'/>\
       </relation>\
       </osm>";

var buildingToBuildingInput =
    "<?xml version='1.0' encoding='UTF-8'?>\
     <osm version='0.6' generator='JOSM'>\
       <way id='-222330' action='modify' visible='true'>\
         <nd ref='-222258' />\
         <nd ref='-222256' />\
         <nd ref='-222254' />\
         <nd ref='-222252' />\
         <nd ref='-222264' />\
         <nd ref='-222258' />\
         <tag k='building' v='yes' />\
         <tag k='name' v='building 1' />\
         <tag k='hoot:merge:target' v='yes'/>\
       </way>\
       <way id='-222332' action='modify' visible='true'>\
         <nd ref='-222264' />\
         <nd ref='-222262' />\
         <nd ref='-222260' />\
         <nd ref='-222258' />\
         <nd ref='-222264' />\
         <tag k='building' v='yes' />\
         <tag k='name' v='building 2' />\
       </way>\
     </osm>";

describe('ElementMergeServer', function () {
    it('responds with HTTP 404 if url not found', function() {
        var request  = httpMocks.createRequest({
            method: 'POST',
            url: '/foo',
            body: poiToPoiInput
        });
        var response = httpMocks.createResponse();
        server.ElementMergeserver(request, response);
        assert.equal(response.statusCode, '404');
    });
    it('responds with HTTP 400 if unsupported method', function() {
        var request  = httpMocks.createRequest({
            method: 'GET',
            url: '/elementmerge'
        });
        var response = httpMocks.createResponse();
        server.ElementMergeserver(request, response);
        assert.equal(response.statusCode, '400');
    });

    //Testing good inputs here only.  Bad inputs are thoroughly tested in the core.  Not checking
    //nodes in the polys here or overall element count, but its ok since we're testing all of
    //that in the core.

    it('merges two pois', function() {
        var request  = httpMocks.createRequest({
            method: 'POST',
            url: '/elementmerge',
            body: poiToPoiInput
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
                assert.equal(result.osm.node[0].tag[0].$.v, "3");
                assert.equal(result.osm.node[0].tag[1].$.k, "name");
                assert.equal(result.osm.node[0].tag[1].$.v, "POST, JÄGER-VON-FALL-STRASSE");
                assert.equal(result.osm.node[0].tag[2].$.k, "amenity");
                assert.equal(result.osm.node[0].tag[2].$.v, "post_office");
                assert.equal(result.osm.node[0].tag[3].$.k, "error:circular");
                assert.equal(result.osm.node[0].tag[3].$.v, "15");
            });
        });
    });

    it('merges a poi and a polygon', function() {
        var request  = httpMocks.createRequest({
            method: 'POST',
            url: '/elementmerge',
            body: poiToPolyInput
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
                assert.equal(result.osm.way[0].tag[0].$.k, "note");
                assert.equal(result.osm.way[0].tag[0].$.v, "poi1;building1");
                assert.equal(result.osm.way[0].tag[1].$.k, "hoot:poipolygon:poismerged");
                assert.equal(result.osm.way[0].tag[1].$.v, "1");
                assert.equal(result.osm.way[0].tag[2].$.k, "hoot:status");
                assert.equal(result.osm.way[0].tag[2].$.v, "3");
                assert.equal(result.osm.way[0].tag[3].$.k, "building");
               assert.equal(result.osm.way[0].tag[3].$.v, "yes");
                assert.equal(result.osm.way[0].tag[4].$.k, "amenity");
                assert.equal(result.osm.way[0].tag[4].$.v, "prison");
                assert.equal(result.osm.way[0].tag[5].$.k, "error:circular");
                assert.equal(result.osm.way[0].tag[5].$.v, "15");
            });
        });
    });

    it('merges two areas', function() {
        var request  = httpMocks.createRequest({
            method: 'POST',
            url: '/elementmerge',
            body: areaToAreaInput
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
                assert.equal(result.osm.way[0].tag[0].$.k, "hoot:status");
                assert.equal(result.osm.way[0].tag[0].$.v, "3");
                assert.equal(result.osm.way[0].tag[1].$.k, "leisure");
                assert.equal(result.osm.way[0].tag[1].$.v, "pitch");
                assert.equal(result.osm.way[0].tag[2].$.k, "alt_name");
                assert.equal(result.osm.way[0].tag[2].$.v, "Margaret S.Hayward Playground");
                assert.equal(result.osm.way[0].tag[3].$.k, "name");
                assert.equal(result.osm.way[0].tag[3].$.v, "baseball field");
                assert.equal(result.osm.way[0].tag[4].$.k, "area");
                assert.equal(result.osm.way[0].tag[4].$.v, "yes");
                assert.equal(result.osm.way[0].tag[5].$.k, "sport");
                assert.equal(result.osm.way[0].tag[5].$.v, "baseball");
            });
        });
    });

    it('merges two buildings', function() {
        var request  = httpMocks.createRequest({
            method: 'POST',
            url: '/elementmerge',
            body: buildingToBuildingInput
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
                assert.equal(result.osm.way[0].tag[0].$.k, "hoot:status");
                assert.equal(result.osm.way[0].tag[0].$.v, "3");
                assert.equal(result.osm.way[0].tag[1].$.k, "alt_name");
                assert.equal(result.osm.way[0].tag[1].$.v, "building 2");
                assert.equal(result.osm.way[0].tag[2].$.k, "building");
                assert.equal(result.osm.way[0].tag[2].$.v, "yes");
                assert.equal(result.osm.way[0].tag[3].$.k, "name");
                assert.equal(result.osm.way[0].tag[3].$.v, "building 1");
            });
        });
    });

});
