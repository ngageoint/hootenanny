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
    });
    describe("Polygon Functions", function() {
        describe("validatePoly", function(){
            it('returns null if an empty string', function() {
                expect(server.validatePoly('')).to.equal(null);
            })
            it('ignores any command injection attempts', function() {
                expect(server.validatePoly(
                    '-72.471,18.545;-72.471,18.548;-72.4705,18.548;-72.4705,18.545;-72.471,18.545;rm *'
                )).to.eql(null)
            })
            it('returns null if any of the coordinates are outside the valid bounds', function() {
                expect(server.validatePoly(
                    '-702.471,18.545;-72.471,18.548;-72.4705,18.548;-72.4705,18.545;-702.471,18.545'
                )).to.equal(null);
            })
            it('returns null if coordinates are a line', function(){
                expect(server.validatePoly(
                    '-72.471,18.545;-72.471,18.548;-72.4705,18.548;-72.4705,18.545;-72.471,18.546'
                )).to.equal(null);
            })
            it('returns null if polygon is self intersecting', function() {
                expect(server.validatePoly(
                    '-92.20550811768,34.9760037637;'  +
                    '-92.20825469971,34.78222986237;' +
                    '-91.82647979736,34.87016067944;' +
                    '-92.42523468018,34.87241408106;' +
                    '-92.20550811768,34.9760037637'
                )).to.equal(null);
            })
            it('return input string if a valid polygon', function() {
                var poly = '-72.471,18.545;-72.471,18.548;-72.4705,18.548;-72.4705,18.545;-72.471,18.545';
                expect(server.validatePoly(poly)).to.equal(poly);
            })
            it('returns input string if a valid array of polygons', function() {
                var multipolygon = '-114.65705403328,32.53082221463;-114.65859898567,32.52872363268;-114.65705403328,32.52582895645;-114.65362080574,32.52517764144;-114.65147503853,32.52756577338;-114.65259083748,32.53060512222;-114.65705403328,32.53082221463;-114.66632374763,32.53834776051;-114.66786870003,32.53624935438;-114.66632374763,32.53335492065;-114.6628905201,32.53270366021;-114.66074475288,32.53509159208;-114.66186055183,32.53813068628;-114.66632374763,32.53834776051;-114.65739735603,32.54558326764;-114.65894230843,32.5434850306;-114.65739735603,32.54059083007;-114.65396412849,32.5399396221;-114.65181836128,32.54232736158;-114.65293416023,32.54536621091;-114.65739735603,32.54558326764;-114.66555127144,32.55129890601;-114.66709622383,32.54920080255;-114.66555127144,32.54630678628;-114.6621180439,32.54565561977;-114.65997227669,32.54804320724;-114.66108807564,32.5510818631;-114.66555127144,32.55129890601;-114.67233189583,32.5439915061;-114.67387684822,32.54189323185;-114.67233189583,32.53899898002;-114.66889866829,32.53834776051;-114.66675290108,32.54073554232;-114.66786870003,32.54377444552;-114.67233189583,32.5439915061;-114.68155869484,32.55166064303;-114.68310364723,32.54956254803;-114.68155869484,32.54666854342;-114.6781254673,32.54601737953;-114.67597970009,32.54840495738;-114.67709549904,32.55144360099;-114.68155869484,32.55166064303';
                expect(server.validatePoly(multipolygon)).to.equal(multipolygon);
            });
        })
        describe("polyToBbox", function(){
            it('extracts a bounding box from polygon', function() {
                expect(server.polyToBbox(
                    '-72.471,18.545;-72.471,18.548;-72.4705,18.548;-72.4705,18.545;-72.471,18.545'
                )).to.eql('-72.471,18.545,-72.4705,18.548')
            })
        })
        describe("polyQuotes", function(){
            it('makes sure poly strings for crop command have needed quotes', function() {
                expect(server.polyQuotes(
                    '-72.471,18.545;-72.471,18.548;-72.4705,18.548;-72.4705,18.545;-72.471,18.545'
                )).to.equal(
                    '"-72.471,18.545;-72.471,18.548;-72.4705,18.548;-72.4705,18.545;-72.471,18.545"'
                );
                expect(server.polyQuotes(
                    '"-72.471,18.545;-72.471,18.548;-72.4705,18.548;-72.4705,18.545;-72.471,18.545"'
                )).to.equal(
                    '"-72.471,18.545;-72.471,18.548;-72.4705,18.548;-72.4705,18.545;-72.471,18.545"'
                )
            })
        })
    })
});
