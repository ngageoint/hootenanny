var expect  = require("chai").expect;
var server = require("../server.js");
var fs = require('fs');
var path = require('path');

describe("RenderDb Export Server", function() {
    describe("Validate BBOX", function() {
        it("fails for empty string", function() {
            var bbox = '';
            expect(server.validateBbox(bbox)).to.equal(null);
        });
        it("fails for words", function() {
            var bbox = 'foo';
            expect(server.validateBbox(bbox)).to.equal(null);
        });
        it("fails for command injection", function() {
            var bbox = '38.4902,35.7982,38.6193,35.8536; rm *';
            expect(server.validateBbox(bbox)).to.equal(null);
        });
        it("fails for bad min/max order", function() {
            var bbox = '39.4902,35.7982,38.6193,35.8536';
            expect(server.validateBbox(bbox)).to.equal(null);
        });
        it("fails for greater than max bounds", function() {
            var bbox = '-999,35.7982,189,35.8536';
            expect(server.validateBbox(bbox)).to.equal(null);
        });

        it("works for positive numbers", function() {
            var bbox = '38.4902,35.7982,38.6193,35.8536';
            expect(server.validateBbox(bbox)).to.equal(bbox);
        });
        it("works for negative numbers", function() {
            var bbox = '38.4902,-35.9982,38.6193,-35.8536';
            expect(server.validateBbox(bbox)).to.equal(bbox);
        });
        it("works for whole numbers", function() {
            var bbox = '38,-36,38.6193,-35.8536';
            expect(server.validateBbox(bbox)).to.equal(bbox);
        });
    });
    describe("writeExportFile", function() {
        it ('takes posted json data and writes it to a .json file', function(done) {
            // for all this function knows, the req object it is given,
            // is just a readable stream with a params property
            // for simple testing, we can give it just that.
            var osmFile = path.join(__dirname, 'test_overpass.json');
            var mockJSONRequest = fs.createReadStream(osmFile);
            mockJSONRequest.params = {
                datasource: 'Overpass',
                schema: 'TDSv70',
                format: 'json'
            }
            server.writeExportFile(mockJSONRequest, function(hash, fileName) {
                expect(fileName.split('.')[1]).to.eql('json');
                fs.unlink(fileName, function(err) {
                    done();
                })
            })
        });
        it("takes posted osm xml data and writes it to a .osm file", function(done) {
            var osmFile = path.join(__dirname, 'test_overpass.osm')
            var mockXMLRequest = fs.createReadStream(osmFile);
            mockXMLRequest.params = {
                datasource: 'Overpass',
                schema: 'TDSv70',
                format: 'json'
            }
            server.writeExportFile(mockXMLRequest, function(hash, fileName) {
                expect(fileName.split('.')[1]).to.eql('osm');
                fs.unlink(fileName, function(err) {
                    done();
                })
            })
        });
        it("takes posted osm xml data from non-overpass source and makes it a .osm file", function() {
            var osmFile = path.join(__dirname, 'test_overpass.osm')
            var mockXMLRequest = fs.createReadStream(osmFile);
            mockXMLRequest.params = {
                datasource: 'not_overpass',
                schema: 'TDSv70',
                format: 'json'
            }
            server.writeExportFile(mockXMLRequest, function(hash, fileName) {
                expect(fileName.split('.')[1]).to.eql('osm');
                fs.unlink(fileName, function(err) {
                    done();
                })
            })

        })
    })
});
