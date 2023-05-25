
// See docs/developer/ElementMergeService.asciidoc

var assert = require('assert');
var httpMocks = require('node-mocks-http');
var server = require('../ElementMergeServer.js');
var osmtogeojson = require('osmtogeojson'),
    DOMParser = new require('@xmldom/xmldom').DOMParser,
    parser = new DOMParser(),
    xml2js = require('xml2js');

var HOOT_HOME = process.env.HOOT_HOME;
var hoot = require(HOOT_HOME + '/lib/HootJs');

var poisInput =
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

var areasInput =
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

var buildingsInput =
    "<?xml version='1.0' encoding='UTF-8'?>\
     <osm version='0.6' generator='JOSM'>\
       <way id='222330' action='modify' visible='true'>\
         <nd ref='222258' />\
         <nd ref='222256' />\
         <nd ref='222254' />\
         <nd ref='222252' />\
         <nd ref='222264' />\
         <nd ref='222258' />\
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

var railsInput =
"<?xml version='1.0' encoding='UTF-8'?>\
<osm version='0.6' generator='JOSM'>\
  <node id='-98888685' action='modify' visible='true' lat='38.93508466921' lon='-9.25677124088'>\
    <tag k='error:circular' v='15' />\
    <tag k='name' v='Malveira' />\
    <tag k='railway' v='station' />\
    <tag k='source:datetime' v='2008-08-05T01:51:17.000Z' />\
  </node>\
  <node id='-98888686' action='modify' visible='true' lat='38.93543393745' lon='-9.25464924088' />\
  <node id='-98888687' action='modify' visible='true' lat='38.93550203126' lon='-9.25436514088' />\
  <node id='-98888688' action='modify' visible='true' lat='38.93562132042' lon='-9.25400304088' />\
  <node id='-98888689' action='modify' visible='true' lat='38.93578000599' lon='-9.25367904088' />\
  <node id='-98888690' action='modify' visible='true' lat='38.9359107941' lon='-9.25335584088' />\
  <node id='-98888691' action='modify' visible='true' lat='38.93608587818' lon='-9.25298684088' />\
  <node id='-98888692' action='modify' visible='true' lat='38.93625386291' lon='-9.25277084088' />\
  <node id='-98888693' action='modify' visible='true' lat='38.93644584545' lon='-9.25253084088' />\
  <node id='-98888694' action='modify' visible='true' lat='38.93669782254' lon='-9.25227884088' />\
  <node id='-98888695' action='modify' visible='true' lat='38.93690190399' lon='-9.25211084088' />\
  <node id='-98888696' action='modify' visible='true' lat='38.93709388653' lon='-9.25195484088' />\
  <node id='-98888697' action='modify' visible='true' lat='38.93729786799' lon='-9.25182284088' />\
  <node id='-98888698' action='modify' visible='true' lat='38.93750184944' lon='-9.25173884088' />\
  <node id='-98888699' action='modify' visible='true' lat='38.93816178944' lon='-9.25155884088' />\
  <node id='-98888700' action='modify' visible='true' lat='38.94197744251' lon='-9.25041884088' />\
  <node id='-98888701' action='modify' visible='true' lat='38.94219342287' lon='-9.25032284088' />\
  <node id='-98888702' action='modify' visible='true' lat='38.94248139668' lon='-9.25021484088' />\
  <node id='-98888703' action='modify' visible='true' lat='38.94269737704' lon='-9.25009484088' />\
  <node id='-98888704' action='modify' visible='true' lat='38.94293115579' lon='-9.24993224088' />\
  <node id='-98888705' action='modify' visible='true' lat='38.94315653529' lon='-9.24978574088' />\
  <node id='-98888706' action='modify' visible='true' lat='38.94339011405' lon='-9.24957784088' />\
  <node id='-98888707' action='modify' visible='true' lat='38.94359609532' lon='-9.24936864088' />\
  <node id='-98888708' action='modify' visible='true' lat='38.94382147483' lon='-9.24914324088' />\
  <node id='-98888709' action='modify' visible='true' lat='38.94402435638' lon='-9.24892904088' />\
  <node id='-98888710' action='modify' visible='true' lat='38.94421593896' lon='-9.24869234088' />\
  <node id='-98888711' action='modify' visible='true' lat='38.94449771334' lon='-9.24835414088' />\
  <node id='-98888712' action='modify' visible='true' lat='38.94467809693' lon='-9.24803854088' />\
  <node id='-98888713' action='modify' visible='true' lat='38.94484718156' lon='-9.24773424088' />\
  <node id='-98888714' action='modify' visible='true' lat='38.94498236927' lon='-9.24740734088' />\
  <node id='-98888715' action='modify' visible='true' lat='38.94511765696' lon='-9.24712554088' />\
  <node id='-98888716' action='modify' visible='true' lat='38.9465039309' lon='-9.24313534088' />\
  <node id='-98888717' action='modify' visible='true' lat='38.9466392186' lon='-9.24283094088' />\
  <node id='-98888718' action='modify' visible='true' lat='38.94675310824' lon='-9.24266054088' />\
  <node id='-98888742' action='modify' visible='true' lat='38.93557889956' lon='-9.25371438849' />\
  <node id='-98888743' action='modify' visible='true' lat='38.93569928888' lon='-9.25343278849' />\
  <node id='-98888744' action='modify' visible='true' lat='38.93594426717' lon='-9.25298458849' />\
  <node id='-98888745' action='modify' visible='true' lat='38.93654751368' lon='-9.25227488849' />\
  <node id='-98888746' action='modify' visible='true' lat='38.93688818348' lon='-9.25200838849' />\
  <node id='-98888747' action='modify' visible='true' lat='38.93726425014' lon='-9.25180368849' />\
  <node id='-98888748' action='modify' visible='true' lat='38.93761121938' lon='-9.25167328849' />\
  <node id='-98888749' action='modify' visible='true' lat='38.94150827386' lon='-9.25053198849' />\
  <node id='-98888750' action='modify' visible='true' lat='38.94197153279' lon='-9.25036018849' />\
  <node id='-98888751' action='modify' visible='true' lat='38.94246608894' lon='-9.25011678849' />\
  <node id='-98888752' action='modify' visible='true' lat='38.94277496155' lon='-9.24992198849' />\
  <node id='-98888753' action='modify' visible='true' lat='38.94309103352' lon='-9.24968698849' />\
  <node id='-98888754' action='modify' visible='true' lat='38.94346710018' lon='-9.24936228849' />\
  <node id='-98888755' action='modify' visible='true' lat='38.94387126434' lon='-9.24892948849' />\
  <node id='-98888756' action='modify' visible='true' lat='38.944055548' lon='-9.24865928849' />\
  <node id='-98888757' action='modify' visible='true' lat='38.94433732302' lon='-9.24832108849' />\
  <node id='-98888758' action='modify' visible='true' lat='38.94451770702' lon='-9.24800548849' />\
  <node id='-98888759' action='modify' visible='true' lat='38.94468679203' lon='-9.24770118849' />\
  <node id='-98888760' action='modify' visible='true' lat='38.94493876969' lon='-9.24716908849' />\
  <node id='-98888761' action='modify' visible='true' lat='38.94517634862' lon='-9.24653728849' />\
  <node id='-98888762' action='modify' visible='true' lat='38.94615576177' lon='-9.24368568849' />\
  <node id='-98888763' action='modify' visible='true' lat='38.94633284607' lon='-9.24322438849' />\
  <node id='-98888764' action='modify' visible='true' lat='38.94659272303' lon='-9.24262748849' />\
  <way id='-98867085' action='modify' visible='true'>\
    <nd ref='-98888685' />\
    <nd ref='-98888686' />\
    <nd ref='-98888687' />\
    <nd ref='-98888688' />\
    <nd ref='-98888689' />\
    <nd ref='-98888690' />\
    <nd ref='-98888691' />\
    <nd ref='-98888692' />\
    <nd ref='-98888693' />\
    <nd ref='-98888694' />\
    <nd ref='-98888695' />\
    <nd ref='-98888696' />\
    <nd ref='-98888697' />\
    <nd ref='-98888698' />\
    <nd ref='-98888699' />\
    <nd ref='-98888700' />\
    <nd ref='-98888701' />\
    <nd ref='-98888702' />\
    <nd ref='-98888703' />\
    <nd ref='-98888704' />\
    <nd ref='-98888705' />\
    <nd ref='-98888706' />\
    <nd ref='-98888707' />\
    <nd ref='-98888708' />\
    <nd ref='-98888709' />\
    <nd ref='-98888710' />\
    <nd ref='-98888711' />\
    <nd ref='-98888712' />\
    <nd ref='-98888713' />\
    <nd ref='-98888714' />\
    <nd ref='-98888715' />\
    <nd ref='-98888716' />\
    <nd ref='-98888717' />\
    <nd ref='-98888718' />\
    <tag k='created_by' v='Potlatch 0.8a' />\
    <tag k='error:circular' v='15' />\
    <tag k='name' v='Linha do Oeste' />\
    <tag k='railway' v='rail' />\
    <tag k='source:datetime' v='2008-08-05T01:55:41.000Z' />\
    <tag k='hoot:merge:target' v='yes'/>\
    <tag k='hoot:status' v='1'/>\
  </way>\
  <way id='-98867087' action='modify' visible='true'>\
    <nd ref='-98888742' />\
    <nd ref='-98888743' />\
    <nd ref='-98888744' />\
    <nd ref='-98888745' />\
    <nd ref='-98888746' />\
    <nd ref='-98888747' />\
    <nd ref='-98888748' />\
    <nd ref='-98888749' />\
    <nd ref='-98888750' />\
    <nd ref='-98888751' />\
    <nd ref='-98888752' />\
    <nd ref='-98888753' />\
    <nd ref='-98888754' />\
    <nd ref='-98888755' />\
    <nd ref='-98888756' />\
    <nd ref='-98888757' />\
    <nd ref='-98888758' />\
    <nd ref='-98888759' />\
    <nd ref='-98888760' />\
    <nd ref='-98888761' />\
    <nd ref='-98888762' />\
    <nd ref='-98888763' />\
    <nd ref='-98888764' />\
    <tag k='electrified' v='no' />\
    <tag k='error:circular' v='15' />\
    <tag k='gauge' v='1668' />\
    <tag k='maxspeed' v='90' />\
    <tag k='name' v='Linha do Oeste' />\
    <tag k='railway' v='rail' />\
    <tag k='railway:atc' v='no' />\
    <tag k='railway:etcs' v='no' />\
    <tag k='railway:lzb' v='no' />\
    <tag k='railway:pzb' v='no' />\
    <tag k='source:datetime' v='2020-01-17T17:39:21Z' />\
    <tag k='usage' v='main' />\
    <tag k='hoot:status' v='2'/>\
  </way>\
</osm>";

var railsOneToManyInput =
"<?xml version='1.0' encoding='UTF-8'?>\
<osm version='0.6' generator='JOSM'>\
  <node id='-98889113' action='modify' visible='true' lat='39.12841652664' lon='-76.69622451335' />\
  <node id='-98889114' action='modify' visible='true' lat='39.12821470044' lon='-76.69618964463' />\
  <node id='-98889115' action='modify' visible='true' lat='39.12800247021' lon='-76.69614404708' />\
  <node id='-98889116' action='modify' visible='true' lat='39.12753639373' lon='-76.69605016977' />\
  <node id='-98889117' action='modify' visible='true' lat='39.12696419741' lon='-76.69595092803' />\
  <node id='-98889126' action='modify' visible='true' lat='39.12844565614' lon='-76.69628620416' />\
  <node id='-98889127' action='modify' visible='true' lat='39.12812523091' lon='-76.69620305568' />\
  <node id='-98889128' action='modify' visible='true' lat='39.12745316547' lon='-76.69607699186' />\
  <node id='-98889129' action='modify' visible='true' lat='39.12690801789' lon='-76.69599920779' />\
  <way id='-98867418' action='modify' visible='true'>\
    <nd ref='-98889126' />\
    <nd ref='-98889127' />\
    <nd ref='-98889128' />\
    <nd ref='-98889129' />\
    <tag k='length' v='173.0' />\
    <tag k='railway' v='rail' />\
    <tag k='security:classification' v='UNCLASSIFIED' />\
    <tag k='source:imagery' v='DigitalGlobe' />\
    <tag k='source:imagery:datetime' v='2021-01-24 18:58:59' />\
    <tag k='source:imagery:id' v='397a1b477b5a791a8ec7943edc6f9ce5' />\
    <tag k='source:imagery:layerName' v='Maxar Basemap +Vivid' />\
    <tag k='source:imagery:sensor' v='WV01' />\
    <tag k='hoot:status' v='1' />\
  </way>\
  <way id='-98867419' action='modify' visible='true'>\
    <nd ref='-98889113' />\
    <nd ref='-98889114' />\
    <nd ref='-98889115' />\
    <nd ref='-98889116' />\
    <nd ref='-98889117' />\
    <tag k='arrangement' v='multiple' />\
    <tag k='lanes' v='3' />\
    <tag k='length' v='163.4' />\
    <tag k='lines' v='3' />\
    <tag k='name' v='AwesomeRail' />\
    <tag k='railway' v='rail' />\
    <tag k='security:classification' v='UNCLASSIFIED' />\
    <tag k='source' v='DigitalGlobe' />\
    <tag k='source:datetime' v='2021-01-24 18:58:59' />\
    <tag k='source:imagery' v='DigitalGlobe' />\
    <tag k='source:imagery:datetime' v='2021-01-24 18:58:59' />\
    <tag k='source:imagery:id' v='397a1b477b5a791a8ec7943edc6f9ce5' />\
    <tag k='source:imagery:layerName' v='Maxar Basemap +Vivid' />\
    <tag k='source:imagery:sensor' v='WV01' />\
    <tag k='hoot:status' v='2' />\
    <tag k='hoot:railway:one:to:many:match:secondary' v='yes' />\
  </way>\
</osm>";

var waysNodeRefOnly =
'<osm version="0.6" upload="true" generator="hootenanny">\
    <way id="-14080" version="1">\
        <nd ref="-178676" />\
        <nd ref="-178497" />\
        <nd ref="-178580" />\
        <tag k="railway" v="Principal Arterial" />\
        <tag k="hoot:merge:target" v="yes" />\
    </way>\
    <way id="-25982" version="1">\
        <nd ref="-49583" />\
        <nd ref="-178676" />\
        <tag k="error:circular" v="5" />\
        <tag k="highway" v="road" />\
        <tag k="hoot:hash" v="sha1sum:15990d2f5b2e2504c5044e30c0dd3c2d9ab802e3" />\
        <tag k="hoot:status" v="Input1" />\
        <tag k="name" v="PENNSYLVANIA AVE NW" />\
    </way>\
</osm>';

var twoTargetYesInput =
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
         <tag k='hoot:merge:target' v='yes' />\
       </node>\
     </osm>";

function testMerge(input, callback) {
    var request  = httpMocks.createRequest({
        method: 'POST',
        url: '/elementmerge'
    });
    var response = httpMocks.createResponse({
        eventEmitter: require('events').EventEmitter
    });
    response.on('end', function() {
        var xml = parser.parseFromString(response._getData());
        try {
          var gj = osmtogeojson(xml);
          callback(gj);
        } catch(err) {
          console.log(err);
        }
    });
    server.ElementMergeserver(request, response);
    request.send(input);
    assert.equal(response.statusCode, 200);
}

// Use this function for incomplete osm files (missing node refs, etc)
// that can't be converted to geojson
function testMergeXml(input, callback) {
    var request  = httpMocks.createRequest({
        method: 'POST',
        url: '/elementmerge'
    });
    var response = httpMocks.createResponse({
        eventEmitter: require('events').EventEmitter
    });
    response.on('end', function() {
        xml2js.parseString(response._getData(), function(err, result) {
            if (err) {
                console.log(err);
            }
            var tags = {};
            function addTag(e) {
                e.tag.forEach(function(t) {
                    tags[t.$.k] = t.$.v;
                });
            }
            if (result.osm.node) result.osm.node.forEach(addTag);
            if (result.osm.way) result.osm.way.forEach(addTag);
            if (result.osm.relation) result.osm.relation.forEach(addTag);
            callback(tags);
        });
    });
    server.ElementMergeserver(request, response);
    request.send(input);
    assert.equal(response.statusCode, 200);
}

function testError(input, callback) {
    var request  = httpMocks.createRequest({
        method: 'POST',
        url: '/elementmerge'
    });
    var response = httpMocks.createResponse({
        eventEmitter: require('events').EventEmitter
    });
    response.on('end', function() {
         callback(response._getData());
    });
    server.ElementMergeserver(request, response);
    request.send(input);
    assert.equal(response.statusCode, 400);
    assert.equal(response.statusMessage, "IllegalArgumentException");

}

describe('ElementMergeServer', function () {

    before(function() {
        hoot.Settings.set({'writer.include.circular.error.tags': 'true'});
        hoot.Settings.set({'map.writer.schema':'OSM'});
    });

    after(function() {
        hoot.Settings.set({'writer.include.circular.error.tags': 'false'});
    });

    it('responds with HTTP 404 if url not found', function() {
        var request  = httpMocks.createRequest({
            method: 'POST',
            url: '/foo'
        });
        var response = httpMocks.createResponse();
        server.ElementMergeserver(request, response);
        assert.equal(response.statusCode, 404);
    });

    it('responds with HTTP 400 if unsupported method', function() {
        var request  = httpMocks.createRequest({
            method: 'GET',
            url: '/elementmerge'
        });
        var response = httpMocks.createResponse();
        server.ElementMergeserver(request, response);
        assert.equal(response.statusCode, 400);
    });

    it('merges two pois', function() {
        testMerge(poisInput, function(gj) {
            var coords = gj.features[0].geometry.coordinates;
            assert.equal(coords[0], 11.7012814);
            assert.equal(coords[1], 48.0479399);

            var tags = gj.features[0].properties;
            assert.equal(tags["hoot:status"], "3");
            assert.equal(tags["name"], "POST, JÄGER-VON-FALL-STRASSE");
            assert.equal(tags["amenity"], "post_office");
            assert.equal(tags["error:circular"], "15");
        });
    });

    it('merges a poi and a polygon', function() {
        testMergeXml(poiToPolyInput, function(tags) {
            assert.equal(tags["hoot:status"], "3");
            assert.equal(tags["note"], "poi1;building1");
            assert.equal(tags["hoot:poipolygon:poismerged"], "1");
            assert.equal(tags["building"], "yes");
            assert.equal(tags["amenity"], "prison");
            assert.equal(tags["error:circular"], "15");
        });
    });

    it('merges two areas', function() {
        testMergeXml(areasInput, function(tags) {
            assert.equal(tags["hoot:status"], "3");
            assert.equal(tags["name"], "Mt. Davidson Park");
            assert.equal(tags["error:circular"], "15");
            assert.equal(tags["alt_name"], "Mt. Davidson Park 2");
            assert.equal(tags["area"], "yes");
            assert.equal(tags["type"], "multipolygon");
        });
    });

    it('merges two buildings', function() {
        testMergeXml(buildingsInput, function(tags) {
            assert.equal(tags["hoot:status"], "3");
            assert.equal(tags["name"], "building 1");
            assert.equal(tags["alt_name"], "building 2");
            assert.equal(tags["building"], "yes");
        });
    });

    it('merges two rails with the one-to-many workflow', function() {
        testMerge(railsOneToManyInput, function(gj) {
            var tags = gj.features[0].properties;
            assert.equal(tags["arrangement"], "multiple");
            assert.equal(tags["hoot:status"], "3");
            assert.equal(tags["name"], "AwesomeRail");
            assert.equal(tags["railway"], "rail");
        });
    });

    it('merges two rails', function() {
        testMerge(railsInput, function(gj) {
            var tags = gj.features[2].properties;
            assert.equal(tags["hoot:status"], "3");
            assert.equal(tags["name"], "Linha do Oeste");
            assert.equal(tags["railway"], "rail");
        });
    });

    it('errors for two roads', function() {
        testError(waysNodeRefOnly, function(err) {
            assert.equal(err, 'Invalid inputs passed to the element merger. Input must contain only one combination of the following: 1) two or more POIs, 2) two or more buildings, 3)two or more areas, 4) one POI and one polygon, or 5) two railways');
        });
    });

    it('errors for two target yes tags', function() {
        testError(twoTargetYesInput, function(err) {
            assert.equal(err, 'Input map must have exactly one feature marked with a hoot:merge:target tag.');
        });
    });

});
