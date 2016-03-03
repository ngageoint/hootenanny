
var assert = require("assert")
async = require('async');
express = require('express');
fs = require('fs');
htmlToText = require('html-to-text');
http = require('http');
path = require('path');
restler = require('restler');
var HOOT_HOME = process.env.HOOT_HOME
util = require(HOOT_HOME + '/test-files/services-system/TestUtils.js');

var HOOT_HOME = process.env.HOOT_HOME

function conflate(testCount, input1, input2, outputBase, expectedCount, done) {
    var completed = 0;

    for (var i = 0; i < testCount; i++) {
        var uid = Math.floor(Math.random() * 100000);
        util.conflate(input1, input2,
            outputBase + uid, function(err, mapName) {
                assert(err == undefined, err);
                var count = util.readMapElementCount(util.dbUrl + '/' +
                    mapName, function(count) {
                        // .9 and 1.1 are rather arbitrary. We just don't
                        // want dramatic changes.
                        if (count < expectedCount * .9 || 
                            count > expectedCount * 1.1) {
                            assert.fail(count, expectedCount, 
                                "Element count is out of range. " + count);
                        }
                        completed++;
                        if (completed >= testCount) {
                            done();
                        }
                        // TODO: delete over services
                    });
            });
    }
}

describe('etl', function(){
  describe('conflate', function(){

    it('should load and conflate DC roads', function(done){
        this.timeout(10 * 60 * 1000);
        this.slow(2 * 60 * 1000);
        var dc1Done = false;
        var dc2Done = false;

        // when both ingests are done, conflate the layers.
        var conflateWhenDone = function() {
            if (dc1Done && dc2Done) {
                conflate(1, 'DcGisRoadsConflate', 'DcTigerRoadsConflate', 
                    'MergedDcRoads_', 6075, done);
            }
        }

        conflateWhenDone();

        // spawn two ingests at once
        util.ingestIfMissing([HOOT_HOME + '/test-files/DcGisRoads.osm'],
            'OSM_Ingest.js', 'DcGisRoadsConflate',
            function(err) {
                assert(err == undefined, err);
                dc1Done = true;
                conflateWhenDone();
            });

        util.ingestIfMissing([HOOT_HOME + '/test-files/DcTigerRoads.osm'],
            'OSM_Ingest.js', 'DcTigerRoadsConflate',
            function(err) {
                assert(err == undefined, err);
                dc2Done = true;
                conflateWhenDone();
            });
    });

  })
})
