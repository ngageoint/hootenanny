var fs      = require('fs');
var chai    = require('chai');
var expect  = require('chai').expect;
var chaiXml = require('chai-xml');

//loads the plugin
chai.use(chaiXml);

var translator = require('../hoot-translate.js');

describe('hoot osm xml translations', function () {

    it('should translate xml from OSM to MGCP while preserving bounds tag and element ids', function() {
        var data = '<osm version="0.6" upload="true" generator="hootenanny"><bounds minlat="30.6425644" minlon="34.2201805" maxlat="30.6558557" maxlon="34.2422819" origin="MapEdit server"/><way id="23" version="0"><nd ref="6"/><nd ref="7"/><nd ref="8"/><nd ref="9"/><nd ref="7"/><tag k="building" v="yes"/><tag k="uuid" v="fee4529b-5ecc-4e5c-b06d-1b26a8e830e6"/></way></osm>';

        var expected = '<osm version="0.6" upload="true" generator="hootenanny"><bounds minlat="30.6425644" minlon="34.2201805" maxlat="30.6558557" maxlon="34.2422819" origin="MapEdit server"/><way id="23" version="0"><nd ref="6"/><nd ref="7"/><nd ref="8"/><nd ref="9"/><nd ref="7"/><tag k="UID" v="fee4529b-5ecc-4e5c-b06d-1b26a8e830e6"/><tag k="FCODE" v="AL015"/></way></osm>';

        translator.translateTo(data, 'MGCP', function(actual) {
            expect(actual).xml.to.be.valid();
            expect(actual).xml.to.deep.equal(expected);
        });
    });

    it('should translate a sample size 100kb osm file in less than 300ms', function(done) {
        this.timeout(300);
        fs.readFile(__dirname + '/bluemont.osm', function(err, data) {
            if (err) console.log(err);
            translator.translateTo(data, 'MGCP', function(actual) {
                expect(actual).xml.to.be.valid();
                done();
            });
        });
    });

});