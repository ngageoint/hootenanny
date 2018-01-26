var assert = require('assert'),
    http = require('http'),
    xml2js = require('xml2js'),
    fs = require('fs'),
    httpMocks = require('node-mocks-http'),
    osmtogeojson = require('osmtogeojson'),
    DOMParser = new require('xmldom').DOMParser
    parser = new DOMParser();


var server = require('../TranslationServer.js');

// describe('getLevenshteinDistance', function() {
//     it('should return 1 as the distance from intall to install', function() {
//         var distance = server.getLevenshteinDistance('intall', 'installation')
//         assert.equal(distance, 9);
//     })
// })

describe('getLein', function() {
    it('should return I251 for installation, I213 for intall, M313 for military', 
        function() {
            var leinIntall = server.getLein('intall'),
                leinInstallation = server.getLein('installation'),
                leinMilitary = server.getLein('military');
            
            assert.equal(leinIntall.toLowerCase(), 'i213');
            assert.equal(leinInstallation.toLowerCase(), 'i251');
            assert.equal(leinMilitary.toLowerCase(), 'm313');
        }
    )
})

describe('searchSchema', function() {
    describe('fcodeMatches', function() {
        it('includes items matching "AL" first and in order', function() {
            var options = {
                    geomType: 'Area',
                    translation: 'TDSv61',
                    searchStr: 'AL',
                    limitResult: 12,
                    maxLeinDistance: 500
                },
                alResults = server.searchSchema(options);

            assert.equal(alResults[0].fcode, 'AL010');
            assert.equal(alResults[1].fcode, 'AL011');
        });
    });
    describe('descMatches', function() {
        it('includes words including "mine"', function() {
            var options = {
                    geomType: 'Area',
                    translation: 'TDSv61',
                    searchStr: 'mine',
                    limitResult: 12,
                    maxLeinDistance: 100
                },
                alResults = server.searchSchema(options),
                includesMine = alResults.filter(function(d) {
                    return /mine/.test(d.desc.toLowerCase());
                }).length > 0;

            assert.equal(includesMine, true)
        })
    })
    describe('fuzzyMatches', function() {
        describe('minDescDistance', function () {
            it('should represent minimum distance between post-leading character lein encoded strings',
                function() {
                    var leinInstallation = server.getLein('installation'),
                        leinIntall = server.getLein('intall'),
                        leinMilitary = server.getLein('military'),
                        minDist = Math.abs(
                            Number(leinIntall.substr(1, leinIntall.length)) - 
                            Number(leinInstallation.substr(1, leinInstallation.length))
                        );
                    
                    minDescDistance = Math.min.apply(
                        null, 'Military Installation'
                                .split(/\s+/)
                                .map(function(word) {
                                    var leinWord = server.getLein(word);
                                    return leinWord[0] !== leinIntall[0] ? Infinity : Math.abs(
                                        Number(leinIntall.substr(1, leinIntall.length)) -
                                        Number(leinWord.substr(1, leinWord.length))
                                    )
                                })
                        )
                        
                    assert.equal(minDescDistance, minDist)
                }
            );
        })
        it('includes items with installation in description when searchStr is intall, isnall, or insralkl', 
            function() {
                var options = {
                    geomType: 'Area',
                    translation: 'TDSv61',
                    limitResult: 100,
                    maxLeinDistance: 500
                };

                ['intall', 'isnall', 'isralkl'].forEach(function(misType) {
                    options.searchStr = misType;

                    var misTypeResults = server.searchSchema(options),
                        includesInstallation = misTypeResults.filter(function(d) {
                            return /installation/.test(d.desc.toLowerCase());
                        }).length > 0;
            
                    assert.equal(includesInstallation, true);
                })
            }
        );
        it('includes items with building in description when searchStr is bugidln, bidng, or buldng',
            function() {
                var options = {
                    geomType: 'Area',
                    translation: 'TDSv61',
                    limitResult: 100,
                    maxLeinDistance: 500
                };

                ['budigln', 'biding', 'buldng'].forEach(function(misType) {
                    options.searchStr = misType;

                    var misTypeResults = server.searchSchema(options),
                        includesBuilding = misTypeResults.filter(function(d) {
                            return /building/.test(d.desc.toLowerCase());
                        }).length > 0;

                    assert.equal(includesBuilding, true);
                });
            }
        )
    });
});
