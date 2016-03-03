
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

describe('etl', function(){
  describe('load', function(){

	//TODO: fix (#441)
    /*it('should return a sensible error', function(done){
        this.timeout(15 * 1000);
        this.slow(15 * 1000);
        util.ingestFiles([],
            'TDSv40.js', 'ToyTestA',
            function(err) {
        	    assert(err == undefined, err);
                var map = new hoot.OsmMap();
                hoot.set({'services.db.reader.email': 'test@test.com'});
                hoot.loadMap(map, util.dbUrl + '/ToyTestA', false, 1);
                assert.equal(map.getElementCount(), 40);
                done();
            });
    });*/

    it('should load a .osm file', function(done){
        this.timeout(2 * 60 * 1000);
        this.slow(1 * 60 * 1000);
        util.ingestFiles([HOOT_HOME + '/test-files/DcGisRoads.osm'],
            'OSM_Ingest.js', 'DcGisRoads',
            function(err) {
                assert(err == undefined, err);
                util.readMapElementCount(util.dbUrl + '/DcGisRoads', 
                    function(count) { 
                        assert.equal(count, 4698);
                        done();
                    });
            });
    });
    
    /*it('should load a couple of UTP Shapefiles in a .zip', function(done){
        this.timeout(5 * 60 * 1000);
        this.slow(5 * 60 * 1000);
        util.ingestFiles(['tmp/Utp.zip'],'translations-local/UTP.js', 'UtpZip',
            function(err) {
                assert(err == undefined, err);
                util.readMapElementCount(util.dbUrl + '/UtpZip',
                    function(count) {
                        assert.equal(count, 25908);
                        done();
                    });
            });
    });

    it('should load a MAAX .zip FGDB', function(done){
        this.timeout(10 * 60 * 1000);
        this.slow(10 * 60 * 1000);
        util.ingestFiles(['tmp/maax_gdb.zip'], 'TDSv40.js', 'MaaxZip',
            function(err) {
                assert(err == undefined, err);
                util.readMapElementCount(util.dbUrl + '/MaaxZip',
                    function(count) {
                        assert.equal(count, 1286);
                        done();
                    });
            });
    });*/

  })
})
