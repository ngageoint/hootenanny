var assert = require('assert'),
    http = require('http'),
    xml2js = require('xml2js');
var httpMocks = require('node-mocks-http');
var server = require('../ElementMergeServer.js');

var poiToPoiInput = "<?xml version='1.0' encoding='UTF-8'?>\
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

//The poi to poly inputs are the same as used in the core's PoiPolygonMergerTest.

var poiToPolyWayInput = "<?xml version='1.0' encoding='UTF-8'?>\
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

var poiToPolyRelationInput = "<?xml version='1.0' encoding='UTF-8'?>\
    <osm version='0.6' generator='hootenanny'>\
    <node visible='true' id='-1298' lat='-0.3638353250415207' lon='42.5533100346383151'/>\
    <node visible='true' id='-1297' lat='-0.3637964330107479' lon='42.5535070093631305'/>\
    <node visible='true' id='-1296' lat='-0.3638644940646003' lon='42.5535202527701415'/>\
    <node visible='true' id='-1295' lat='-0.3638772345574353' lon='42.5534558797536917'/>\
    <node visible='true' id='-1294' lat='-0.3638358279557110' lon='42.5534480007647176'/>\
    <node visible='true' id='-1293' lat='-0.3638438745827697' lon='42.5534067618010496'/>\
    <node visible='true' id='-1292' lat='-0.3638852811844940' lon='42.5534148084281014'/>\
    <node visible='true' id='-1291' lat='-0.3639662503692447' lon='42.5534303987680218'/>\
    <node visible='true' id='-1290' lat='-0.3639774821195090' lon='42.5534684526084845'/>\
    <node visible='true' id='-1289' lat='-0.3640286117289264' lon='42.5534783432542270'/>\
    <node visible='true' id='-1288' lat='-0.3640549309049276' lon='42.5534476654885836'/>\
    <node visible='true' id='-1287' lat='-0.3641353971754882' lon='42.5534632558285182'/>\
    <node visible='true' id='-1286' lat='-0.3641843474900811' lon='42.5534726435600703'/>\
    <node visible='true' id='-1285' lat='-0.3641761332249587' lon='42.5535140501617875'/>\
    <node visible='true' id='-1284' lat='-0.3641273505484294' lon='42.5535044947921790'/>\
    <node visible='true' id='-1283' lat='-0.3641074016188525' lon='42.5536052452684430'/>\
    <node visible='true' id='-1282' lat='-0.3641960821545354' lon='42.5536225119889764'/>\
    <node visible='true' id='-1281' lat='-0.3642160310841121' lon='42.5535217615127124'/>\
    <node visible='true' id='-1280' lat='-0.3642242453492344' lon='42.5534803549109881'/>\
    <node visible='true' id='-1279' lat='-0.3642421826220356' lon='42.5533889921663047'/>\
    <node visible='true' id='-1278' lat='-0.3640695154164604' lon='42.5533554645535546'/>\
    <node visible='true' id='-1277' lat='-0.3639853611085045' lon='42.5533392036613805'/>\
    <node visible='true' id='-1276' lat='-0.3639821759852993' lon='42.5533552969154982'/>\
    <node visible='true' id='-1275' lat='-0.3640663302932551' lon='42.5533715578076723'/>\
    <node visible='true' id='-1274' lat='-0.3640547632668641' lon='42.5534314045964095'/>\
    <node visible='true' id='-1273' lat='-0.3639704413208449' lon='42.5534151437042283'/>\
    <node visible='true' id='-532' lat='-0.3635327329999427' lon='42.5442892910000410'>\
        <tag k='note' v='poi1'/>\
        <tag k='building' v='yes'/>\
        <tag k='amenity' v='prison'/>\
        <tag k='error:circular' v='100'/>\
    </node>\
    <way visible='true' id='-103'>\
        <nd ref='-1276'/>\
        <nd ref='-1275'/>\
        <nd ref='-1274'/>\
        <nd ref='-1273'/>\
        <nd ref='-1276'/>\
        <tag k='error:circular' v='15'/>\
    </way>\
    <way visible='true' id='-102'>\
        <nd ref='-1298'/>\
        <nd ref='-1297'/>\
        <nd ref='-1296'/>\
        <nd ref='-1295'/>\
        <nd ref='-1294'/>\
        <nd ref='-1293'/>\
        <nd ref='-1292'/>\
        <nd ref='-1291'/>\
        <nd ref='-1290'/>\
        <nd ref='-1289'/>\
        <nd ref='-1288'/>\
        <nd ref='-1287'/>\
        <nd ref='-1286'/>\
        <nd ref='-1285'/>\
        <nd ref='-1284'/>\
        <nd ref='-1283'/>\
        <nd ref='-1282'/>\
        <nd ref='-1281'/>\
        <nd ref='-1280'/>\
        <nd ref='-1279'/>\
        <nd ref='-1278'/>\
        <nd ref='-1277'/>\
        <nd ref='-1298'/>\
        <tag k='error:circular' v='15'/>\
    </way>\
   <relation visible='true' id='-1'>\
        <member type='way' ref='-102' role='outer'/>\
        <member type='way' ref='-103' role='inner'/>\
        <tag k='note' v='building1'/>\
        <tag k='building' v='yes'/>\
        <tag k='amenity' v='government_administration_building'/>\
        <tag k='type' v='multipolygon'/>\
        <tag k='error:circular' v='15'/>\
    </relation>\
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
                osm: poiToPoiInput
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

        it('merges an osm poi and a way polygon', function() {
            var merged = server.handleInputs({
                path: '/elementmerge',
                method: 'POST',
                osm: poiToPolyWayInput
            });

            xml2js.parseString(merged, function(err, result) {
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
            });
        });

        //For poi/poly tests, not testing invalid inputs as that's handled fairly extensively in PoiPolygonMergerTest.

        it('merges an osm poi and a relation polygon', function() {
            var merged = server.handleInputs({
                path: '/elementmerge',
                method: 'POST',
                osm: poiToPolyRelationInput
            });

            xml2js.parseString(merged, function(err, result) {
                if (err) console.error(err);
                assert.equal(result.osm.relation[0].tag[0].$.k, "note");
                assert.equal(result.osm.relation[0].tag[0].$.v, "poi1;building1");
                assert.equal(result.osm.relation[0].tag[1].$.k, "hoot:poipolygon:poismerged");
                assert.equal(result.osm.relation[0].tag[1].$.v, "1");
                assert.equal(result.osm.relation[0].tag[2].$.k, "hoot:status");
                assert.equal(result.osm.relation[0].tag[2].$.v, "3");
                assert.equal(result.osm.relation[0].tag[3].$.k, "building");
                assert.equal(result.osm.relation[0].tag[3].$.v, "yes");
                assert.equal(result.osm.relation[0].tag[4].$.k, "amenity");
                assert.equal(result.osm.relation[0].tag[4].$.v, "prison");
                assert.equal(result.osm.relation[0].tag[5].$.k, "type");
                assert.equal(result.osm.relation[0].tag[5].$.v, "multipolygon");
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
