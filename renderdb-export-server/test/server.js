var expect  = require("chai").expect;
var server = require("../server.js");

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
});

