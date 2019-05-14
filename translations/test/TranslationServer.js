var assert = require('assert'),
    http = require('http'),
    xml2js = require('xml2js'),
    fs = require('fs'),
    httpMocks = require('node-mocks-http'),
    osmtogeojson = require('osmtogeojson'),
    DOMParser = new require('xmldom').DOMParser
    parser = new DOMParser(),
    hashseedzero = true;

var server = require('../TranslationServer.js');

describe('TranslationServer', function () {

    describe('fcodes', function() {
        this.timeout(3000);

        it('should return fcodes for MGCP Line', function(){
            assert.equal(server.getFCodes({
                method: 'GET',
                translation: 'MGCP',
                geometry: 'line'
            }).length, 59);
        });

        it('should return fcodes for TDSv61 Point', function(){
            assert.equal(server.getFCodes({
                method: 'GET',
                translation: 'TDSv61',
                geometry: 'Point'
            }).length, 193);
        });

        it('should return fcodes for GGDMv30 Area', function(){
            assert.equal(server.getFCodes({
                method: 'GET',
                translation: 'GGDMv30',
                geometry: 'Area'
            }).length, 280);
        });

        it('should return fcodes for TDSv40 Vertex', function(){
            assert.equal(server.getFCodes({
                method: 'GET',
                translation: 'TDSv40',
                geometry: 'vertex'
            }).length, 190);
        });

        it('should return fcodes for MGCP with no geometry', function(){
            var fcodes = server.getFCodes({
                method: 'GET',
                translation: 'MGCP'
            });

            assert.equal(fcodes.length, 196);
            assert.equal(fcodes[0].geom[0], 'Area');
        });

    });

    describe('handleInputs', function() {

        it('should handle translateTo GET', function() {
            // example url
            // http://localhost:8094/translateTo?idval=AL015&geom=Point&translation=MGCP&idelem=fcode
            var schema = server.handleInputs({
                idval: 'AL015',
                geom: 'Point',
                translation: 'MGCP',
                idelem: 'fcode',
                method: 'GET',
                path: '/translateTo'
            });
            assert.equal(schema.desc, 'General Building');
            assert.equal(schema.columns[0].name, 'ACC');
            assert.equal(schema.columns[0].enumerations[0].name, 'Accurate');
            assert.equal(schema.columns[0].enumerations[0].value, '1');
        });

        it('should handle translateTo GET with key & value param', function() {
            var schema = server.handleInputs({
                value: 'AL015',
                geom: 'Point',
                translation: 'MGCP',
                key: 'fcode',
                method: 'GET',
                path: '/translateTo'
            });
            assert.equal(schema.desc, 'General Building');
            assert.equal(schema.columns[0].name, 'ACC');
            assert.equal(schema.columns[0].enumerations[0].name, 'Accurate');
            assert.equal(schema.columns[0].enumerations[0].value, '1');
        });

        it('should handle no matches translateTo GET for MGCP', function() {
            assert.throws(function error() {
                server.handleInputs({
                    idval: 'FB123',
                    geom: 'area',
                    translation: 'TDSv61',
                    idelem: 'fcode',
                    method: 'GET',
                    path: '/translateTo'
                })
            }, Error, 'TDSv61 for Area with fcode=FB123 not found');
        });
        it('builds intersection of between geometry schemas', function() {
            var pointSchema = server.handleInputs({
                idval: 'AL013',
                geom: 'Point',
                translation: 'TDSv61',
                idelem: 'fcode',
                method: 'GET',
                path: '/translateTo'
            });

            var areaSchema = server.handleInputs({
                idval: 'AL013',
                geom: 'Area',
                translation: 'TDSv61',
                idelem: 'fcode',
                method: 'GET',
                path: '/translateTo'
            });
            var allSchema = server.handleInputs({
                idval: 'AL013',
                geom: 'Area,Point',
                translation: 'TDSv61',
                idelem: 'fcode',
                method: 'GET',
                path: '/translateTo'
            });

            var pointSchemaColumns = pointSchema.columns.map(function(col) { return col.name; });
            var areaSchemaColumns = areaSchema.columns.map(function(col) { return col.name; } );
            var allSchemaColumns = allSchema.columns.map(function(col) { return col.name; });

            var intersection = pointSchemaColumns.filter(function(col) { return areaSchemaColumns.indexOf(col) !== -1 }).length > 0;
            assert.equal(allSchemaColumns.length > 0, true);
            assert.equal(intersection, true);
        })

        it('should handle translateFrom GET for TDSv61', function() {
            //http://localhost:8094/translateFrom?fcode=AL013&translation=TDSv61
            var attrs = server.handleInputs({
                fcode: 'AL013',
                translation: 'TDSv61',
                method: 'GET',
                path: '/translateFrom'
            }).attrs;
            assert.equal(attrs.building, 'yes');
        });

        it('should handle translateFrom GET for TDSv40', function() {
            //http://localhost:8094/tdstoosm?fcode=AL013&translation=TDSv61
            var attrs = server.handleInputs({
                fcode: 'AP030',
                translation: 'TDSv40',
                method: 'GET',
                path: '/translateFrom'
            }).attrs;
            assert.equal(attrs.highway, 'road');
        });

        it('should handle translateFrom GET for MGCP', function() {
            //http://localhost:8094/translateFrom?fcode=AL013&translation=TDSv61
            var attrs = server.handleInputs({
                fcode: 'BH140',
                translation: 'MGCP',
                method: 'GET',
                path: '/translateFrom'
            }).attrs;
            assert.equal(attrs.waterway, 'river');
        });

        it('should handle translateFrom GET for MGCP', function() {
            //http://localhost:8094/tdstoosm?fcode=AL013&translation=TDSv61
            var attrs = server.handleInputs({
                fcode: 'BH140',
                translation: 'GGDMv30',
                method: 'GET',
                path: '/translateFrom'
            }).attrs;
            assert.equal(attrs.waterway, 'river');
        });
        it('should handle invalid F_CODE in translateFrom GET for MGCP', function() {
            var attrs = server.handleInputs({
                fcode: 'ZZTOP',
                translation: 'MGCP',
                method: 'GET',
                path: '/translateFrom'
            }).attrs;
            assert.equal(attrs.error, 'Feature Code ZZTOP is not valid for MGCP');
        });

        it('should handle translateTo TDSv61 POST', function() {
            var output = server.handleInputs({
                osm: '<osm version="0.6" upload="true" generator="JOSM"><node id="-1" lon="-105.21811763904256" lat="39.35643172777992" version="0"><tag k="building" v="yes"/><tag k="uuid" v="{bfd3f222-8e04-4ddc-b201-476099761302}"/></node></osm>',
                method: 'POST',
                translation: 'TDSv61',
                path: '/translateTo'
            });

            var xml = parser.parseFromString(output);
            var gj = osmtogeojson(xml);

            assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "TDSv61");

            var tags = gj.features[0].properties;
            assert.equal(tags["F_CODE"], "AL013");
            assert.equal(tags["UFI"], "bfd3f222-8e04-4ddc-b201-476099761302");
        }).timeout(3000);

        it('should handle translateTo POST and preserve bounds tag and ids', function() {
            //http://localhost:8094/osmtotds
            var osm2trans = server.handleInputs({
                osm: '<osm version="0.6" upload="true" generator="JOSM"><bounds minlat="39.35643172777992" minlon="-105.21811763904256" maxlat="39.35643172777992" maxlon="-105.21811763904256" origin="MapEdit server" /><node id="777" lon="-105.21811763904256" lat="39.35643172777992" version="0"><tag k="building" v="yes"/><tag k="uuid" v="{bfd3f222-8e04-4ddc-b201-476099761302}"/></node></osm>',
                method: 'POST',
                translation: 'TDSv61',
                path: '/translateTo'
            });
            xml2js.parseString(osm2trans, function(err, result) {
                if (err) console.error(err);
                assert.equal(result.osm.node[0].$.id, "777");
                assert.equal(parseFloat(result.osm.bounds[0].$.minlat).toFixed(6), 39.356432);
                assert.equal(parseFloat(result.osm.bounds[0].$.minlon).toFixed(6), -105.218118);
                assert.equal(parseFloat(result.osm.bounds[0].$.maxlat).toFixed(6), 39.356432);
                assert.equal(parseFloat(result.osm.bounds[0].$.maxlon).toFixed(6), -105.218118);
            });
        });
        it('should handle translating way or node w/osm amenity=ferry_terminal to AQ080 (now for tdsv61 only)', function() {
            var ferryWay = server.handleInputs({
                osm: '<osm version="0.6" upload="true" generator="hootenanny"><way id="-3" version="0"><nd ref="-7"/><nd ref="-8"/><nd ref="-9"/><nd ref="-7"/><tag k="amenity" v="ferry_terminal"/></way></osm>',
                method: 'POST',
                translation: 'TDSv61',
                path: '/translateTo'
            })

            xml2js.parseString(ferryWay, function(err, result) {
                if (err) console.error(err);
                assert(result.osm.way[0].tag[0].$.k, 'F_CODE')
                assert(result.osm.way[0].tag[0].$.v, 'AQ080')
            })

            var ferryNode = server.handleInputs({
                osm: '<osm version="0.6" upload="true" generator="hootenanny"><node id="-1" lon="33.731597320098885" lat="-24.919578904988793" version="0"><tag k="amenity" v="ferry_terminal"/></node></osm>',
                method: 'POST',
                translation: 'TDSv61',
                path: '/translateTo'
            })

            xml2js.parseString(ferryNode, function(err, result) {
                if (err) console.error(err);
                assert(result.osm.node[0].tag[0].$.k, 'F_CODE')
                assert(result.osm.node[0].tag[0].$.v, 'AQ080')
            })
        })
        it('should handle OSM to MGCP POST of building area feature', function() {
            var osm2trans = server.handleInputs({
                osm: '<osm version="0.6" upload="true" generator="hootenanny"><way id="-1" version="0"><nd ref="-1"/><nd ref="-4"/><nd ref="-7"/><nd ref="-10"/><nd ref="-1"/><tag k="building" v="yes"/><tag k="uuid" v="{d7cdbdfe-88c6-4d8a-979d-ad88cfc65ef1}"/></way></osm>',
                method: 'POST',
                translation: 'MGCP',
                path: '/translateTo'
            });
            xml2js.parseString(osm2trans, function(err, result) {
                if (err) console.error(err);
                assert.equal(result.osm.$.schema, "MGCP");
                assert.equal(result.osm.way[0].tag[0].$.k, "FCODE");
                assert.equal(result.osm.way[0].tag[0].$.v, "AL015");
                assert.equal(result.osm.way[0].tag[1].$.k, "UID");
                assert.equal(result.osm.way[0].tag[1].$.v, "d7cdbdfe-88c6-4d8a-979d-ad88cfc65ef1");
            });
        });


        it('should translate Coastline (BA010) from ogr -> osm -> ogr', function() {

            var data = '<osm version="0.6" generator="JOSM"><way id="-38983" visible="true"><nd ref="-38979" /><nd ref="-38982" /> <tag k="F_CODE" v="BA010" /></way></osm>'

            var osm_xml = server.handleInputs({
                osm: data,
                method: 'POST',
                translation: 'TDSv61',
                path: '/translateFrom'
            });

            xml2js.parseString(osm_xml, function(err, result) {
                if (err) console.error(err);
                assert.equal(result.osm.way[0].tag[0].$.k, "natural")
                assert.equal(result.osm.way[0].tag[0].$.v, "coastline")

            });

            var tds61_xml = server.handleInputs({
                osm: osm_xml,
                method: 'POST',
                translation: 'TDSv61',
                path: '/translateTo'
            });

            xml2js.parseString(tds61_xml, function(err, result) {
                if (err) console.error(err);
                assert.equal(result.osm.way[0].tag[0].$.k, "F_CODE")
                assert.equal(result.osm.way[0].tag[0].$.v, "BA010")
            });

            var tds40_xml = server.handleInputs({
                osm: osm_xml,
                method: 'POST',
                translation: 'TDSv40',
                path: '/translateTo'
            });

            xml2js.parseString(tds40_xml, function(err, result) {
                if (err) console.error(err);
                assert.equal(result.osm.way[0].tag[0].$.k, "F_CODE")
                assert.equal(result.osm.way[0].tag[0].$.v, "BA010")
            });

            var mgcp_xml = server.handleInputs({
                osm: osm_xml,
                method: 'POST',
                translation: 'MGCP',
                path: '/translateTo'
            });

            xml2js.parseString(mgcp_xml, function(err, result) {
                if (err) console.error(err);
                assert.equal(result.osm.way[0].tag[0].$.k, "FCODE")
                assert.equal(result.osm.way[0].tag[0].$.v, "BA010")
            });

            var ggdmv30_xml = server.handleInputs({
                osm: osm_xml,
                method: 'POST',
                translation: 'GGDMv30',
                path: '/translateTo'
            });

            xml2js.parseString(ggdmv30_xml, function(err, result) {
                if (err) console.error(err);
                assert.equal(result.osm.way[0].tag[0].$.k, "F_CODE")
                assert.equal(result.osm.way[0].tag[0].$.v, "BA010")
            });

        }).timeout(8000);

        it('should persist Land Water Boundary (BA010) FCODE and SLT=6 (mangrove) from TDSv61-> osm -> TDSv61', function() {

            var data = '<osm version="0.6" generator="JOSM"><way id="-38983" visible="true"><nd ref="-38979" /><nd ref="-38982" /> <tag k="F_CODE" v="BA010" /><tag k="SLT" v="6" /></way></osm>'

                var osm_xml = server.handleInputs({
                    osm: data,
                    method: 'POST',
                    translation: 'TDSv61',
                    path: '/translateFrom'
                });


                xml2js.parseString(osm_xml, function(err, result) {
                    if (err) console.error(err);

                    assert.equal(result.osm.way[0].tag[0].$.k, 'shoreline:type')
                    assert.equal(result.osm.way[0].tag[0].$.v, 'mangrove')
                    assert.equal(result.osm.way[0].tag[1].$.k, 'natural')
                    assert.equal(result.osm.way[0].tag[1].$.v, 'coastline')
                })

                var tds_xml = server.handleInputs({
                    osm: osm_xml,
                    method: 'POST',
                    translation: 'TDSv61',
                    path: '/translateTo'
                })

                xml2js.parseString(tds_xml, function(err, result) {
                    if (err) console.error(err);
                    assert.equal(result.osm.way[0].tag[0].$.k, 'SLT')
                    assert.equal(result.osm.way[0].tag[0].$.v, '6')
                    assert.equal(result.osm.way[0].tag[1].$.k, 'F_CODE')
                    assert.equal(result.osm.way[0].tag[1].$.v, 'BA010')
                })
        })

        it('should handle OSM to MGCP POST of power line feature', function() {
            var osm2trans = server.handleInputs({
                osm: '<osm version="0.6" upload="true" generator="hootenanny"><way id="-1" version="0"><nd ref="-1"/><nd ref="-4"/><nd ref="-7"/><nd ref="-10"/><nd ref="-1"/><tag k="power" v="line"/><tag k="uuid" v="{d7cdbdfe-88c6-4d8a-979d-ad88cfc65ef1}"/></way></osm>',
                method: 'POST',
                translation: 'MGCP',
                path: '/translateTo'
            });
            xml2js.parseString(osm2trans, function(err, result) {
                if (err) console.error(err);
                assert.equal(result.osm.$.schema, "MGCP");
                assert.equal(result.osm.way[0].tag[1].$.k, "UID");
                assert.equal(result.osm.way[0].tag[1].$.v, "d7cdbdfe-88c6-4d8a-979d-ad88cfc65ef1");
                assert.equal(result.osm.way[0].tag[0].$.k, "FCODE");
                assert.equal(result.osm.way[0].tag[0].$.v, "AT030");
            });
        });

        it('should handle OSM to TDSv40 POST of power line feature', function() {
            var osm2trans = server.handleInputs({
                osm: '<osm version="0.6" upload="true" generator="hootenanny"><way id="-1" version="0"><nd ref="-1"/><nd ref="-4"/><nd ref="-7"/><nd ref="-10"/><nd ref="-1"/><tag k="power" v="line"/><tag k="uuid" v="{d7cdbdfe-88c6-4d8a-979d-ad88cfc65ef1}"/></way></osm>',
                method: 'POST',
                translation: 'TDSv40',
                path: '/translateTo'
            });
            xml2js.parseString(osm2trans, function(err, result) {
                if (err) console.error(err);
                assert.equal(result.osm.$.schema, "TDSv40");
                assert.equal(result.osm.way[0].tag[1].$.k, "CAB");
                assert.equal(result.osm.way[0].tag[1].$.v, "2");
                assert.equal(result.osm.way[0].tag[2].$.k, "UFI");
                assert.equal(result.osm.way[0].tag[2].$.v, "d7cdbdfe-88c6-4d8a-979d-ad88cfc65ef1");
                assert.equal(result.osm.way[0].tag[0].$.k, "F_CODE");
                assert.equal(result.osm.way[0].tag[0].$.v, "AT005");
            });
        });

        it('should handle OSM to TDSv61 POST of power line feature', function() {
            var osm2trans = server.handleInputs({
                osm: '<osm version="0.6" upload="true" generator="hootenanny"><way id="-1" version="0"><nd ref="-1"/><nd ref="-4"/><nd ref="-7"/><nd ref="-10"/><nd ref="-1"/><tag k="power" v="line"/><tag k="uuid" v="{d7cdbdfe-88c6-4d8a-979d-ad88cfc65ef1}"/></way></osm>',
                method: 'POST',
                translation: 'TDSv61',
                path: '/translateTo'
            });
            xml2js.parseString(osm2trans, function(err, result) {
                if (err) console.error(err);
                assert.equal(result.osm.$.schema, "TDSv61");
                assert.equal(result.osm.way[0].tag[1].$.k, "CAB");
                assert.equal(result.osm.way[0].tag[1].$.v, "2");
                assert.equal(result.osm.way[0].tag[2].$.k, "UFI");
                assert.equal(result.osm.way[0].tag[2].$.v, "d7cdbdfe-88c6-4d8a-979d-ad88cfc65ef1");
                assert.equal(result.osm.way[0].tag[0].$.k, "F_CODE");
                assert.equal(result.osm.way[0].tag[0].$.v, "AT005");
            });
        });

        it('should handle OSM to GGDMv30 POST of power line feature', function() {
            this.timeout(3000);

            var osm2trans = server.handleInputs({
                osm: '<osm version="0.6" upload="true" generator="hootenanny"><way id="-1" version="0"><nd ref="-1"/><nd ref="-4"/><nd ref="-7"/><nd ref="-10"/><nd ref="-1"/><tag k="power" v="line"/><tag k="uuid" v="{d7cdbdfe-88c6-4d8a-979d-ad88cfc65ef1}"/></way></osm>',
                method: 'POST',
                translation: 'GGDMv30',
                path: '/translateTo'
            });
            xml2js.parseString(osm2trans, function(err, result) {
                if (err) console.error(err);
                assert.equal(result.osm.$.schema, "GGDMv30");
                assert.equal(result.osm.way[0].tag[1].$.k, "CAB");
                assert.equal(result.osm.way[0].tag[1].$.v, "2");
                assert.equal(result.osm.way[0].tag[2].$.k, "UFI");
                assert.equal(result.osm.way[0].tag[2].$.v, "d7cdbdfe-88c6-4d8a-979d-ad88cfc65ef1");
                assert.equal(result.osm.way[0].tag[0].$.k, "F_CODE");
                assert.equal(result.osm.way[0].tag[0].$.v, "AT005");
            });
        });

        it('should handle OSM to MGCP POST of road line feature with width', function() {
            var osm2trans = server.handleInputs({
                osm: '<osm version="0.6" upload="true" generator="hootenanny"><way id="-8" version="0"><nd ref="-21"/><nd ref="-24"/><nd ref="-27"/><tag k="highway" v="road"/><tag k="uuid" v="{8cd72087-a7a2-43a9-8dfb-7836f2ffea13}"/><tag k="width" v="20"/><tag k="lanes" v="2"/></way></osm>',
                method: 'POST',
                translation: 'MGCP',
                path: '/translateTo'
            });

            xml2js.parseString(osm2trans, function(err, result) {
                if (err) console.error(err);
                assert.equal(result.osm.$.schema, "MGCP");
                assert.equal(result.osm.way[0].tag[0].$.k, "FCODE");
                assert.equal(result.osm.way[0].tag[0].$.v, "AP030");
                assert.equal(result.osm.way[0].tag[2].$.k, "UID");
                assert.equal(result.osm.way[0].tag[2].$.v, "8cd72087-a7a2-43a9-8dfb-7836f2ffea13");
                assert.equal(result.osm.way[0].tag[3].$.k, "HCT");
                assert.equal(result.osm.way[0].tag[3].$.v, "0");
                assert.equal(result.osm.way[0].tag[4].$.k, "WD1");
                assert.equal(result.osm.way[0].tag[4].$.v, "20");
                assert.equal(result.osm.way[0].tag[1].$.k, "LTN");
                assert.equal(result.osm.way[0].tag[1].$.v, "2");
            });
        });

       it('should handle OSM to GGDMv30 POST of road line feature with width', function() {
            var osm2trans = server.handleInputs({
                osm: '<osm version="0.6" upload="true" generator="hootenanny"><way id="-8" version="0"><nd ref="-21"/><nd ref="-24"/><nd ref="-27"/><tag k="highway" v="road"/><tag k="uuid" v="{8cd72087-a7a2-43a9-8dfb-7836f2ffea13}"/><tag k="width" v="20"/><tag k="lanes" v="2"/></way></osm>',
                method: 'POST',
                translation: 'GGDMv30',
                path: '/translateTo'
            });
            xml2js.parseString(osm2trans, function(err, result) {
                if (err) console.error(err);
                assert.equal(result.osm.$.schema, "GGDMv30");
                assert.equal(result.osm.way[0].tag[0].$.k, "LTN");
                assert.equal(result.osm.way[0].tag[0].$.v, "2");
                assert.equal(result.osm.way[0].tag[4].$.k, "RTY");
                assert.equal(result.osm.way[0].tag[4].$.v, "-999999");
                assert.equal(result.osm.way[0].tag[5].$.k, "UFI");
                assert.equal(result.osm.way[0].tag[5].$.v, "8cd72087-a7a2-43a9-8dfb-7836f2ffea13");
                assert.equal(result.osm.way[0].tag[3].$.k, "F_CODE");
                assert.equal(result.osm.way[0].tag[3].$.v, "AP030");
                assert.equal(result.osm.way[0].tag[1].$.k, "ZI016_WD1");
                assert.equal(result.osm.way[0].tag[1].$.v, "20");
                assert.equal(result.osm.way[0].tag[2].$.k, "RIN_ROI");
                assert.equal(result.osm.way[0].tag[2].$.v, "5");
            });
        });

        it('should handle OSM to MGCP POST of facility area feature', function() {
            var osm2trans = server.handleInputs({
                osm: '<osm version="0.6" upload="true" generator="hootenanny"><way id="-1" version="0"><nd ref="-1"/><nd ref="-4"/><nd ref="-7"/><nd ref="-10"/><nd ref="-1"/><tag k="facility" v="yes"/><tag k="uuid" v="{fee4529b-5ecc-4e5c-b06d-1b26a8e830e6}"/><tag k="area" v="yes"/></way></osm>',
                method: 'POST',
                translation: 'MGCP',
                path: '/translateTo'
            });
            xml2js.parseString(osm2trans, function(err, result) {
                if (err) console.error(err);
                assert.equal(result.osm.way[0].tag[1].$.k, "UID");
                assert.equal(result.osm.way[0].tag[1].$.v, "fee4529b-5ecc-4e5c-b06d-1b26a8e830e6");
                assert.equal(result.osm.way[0].tag[0].$.k, "FCODE");
                assert.equal(result.osm.way[0].tag[0].$.v, "AL010");
            });
        });

        it('should handle OSM to TDSv61 Raw POST of a complete osm file and preserve bounds tag and element ids', function() {
            var data = fs.readFileSync('../test-files/ToyTestA.osm', 'utf8');//, function(err, data) {
            var osm2trans = server.handleInputs({
                osm: data,
                method: 'POST',
                translation: 'TDSv61',
                path: '/translateTo'
            });
            xml2js.parseString(osm2trans, function(err, result) {
                if (err) console.error(err);
                assert.equal(result.osm.$.schema, "TDSv61");

                assert.equal(result.osm.bounds[0].$.minlat, "38.85324242720166");
                assert.equal(result.osm.bounds[0].$.minlon, "-104.9024316099691");
                assert.equal(result.osm.bounds[0].$.maxlat, "38.85496143739888");
                assert.equal(result.osm.bounds[0].$.maxlon, "-104.8961823052624");

                assert.equal(result.osm.way[0].$.id, "-1669801");
                assert.equal(result.osm.way[0].nd[0].$.ref, "-1669731");
                assert.equal(result.osm.way[0].nd[1].$.ref, "-1669791");
                assert.equal(result.osm.way[0].nd[2].$.ref, "-1669793");
            });
        });

        it('should be lossy to go from osm -> mgcp -> osm', function() {

            var data = '<osm version="0.6" upload="true" generator="JOSM"><node id="-4" lon="-105.24014094121263" lat="39.28928610944744" version="0"><tag k="poi" v="yes"/><tag k="amenity" v="cafe"/><tag k="uuid" v="{4632d15b-7c44-4ba1-a0c4-8cfbb30e39d4}"/></node></osm>';

            var mgcp_xml = server.handleInputs({
                osm: data,
                method: 'POST',
                translation: 'MGCP',
                path: '/translateTo'
            });

            var xml = parser.parseFromString(mgcp_xml);
            var gj = osmtogeojson(xml);

            assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "MGCP");

            var tags = gj.features[0].properties;
            assert.equal(tags["FCODE"], "AL015");
            assert.equal(tags["FFN"], "572");
            assert.equal(tags["HWT"], "998");
            assert.equal(tags["UID"], "4632d15b-7c44-4ba1-a0c4-8cfbb30e39d4");

            var osm_xml = server.handleInputs({
                osm: mgcp_xml,
                method: 'POST',
                translation: 'MGCP',
                path: '/translateFrom'
            });

            xml = parser.parseFromString(osm_xml);
            gj = osmtogeojson(xml);

            assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "OSM");

            var tags = gj.features[0].properties;
            assert.equal(tags["building"], "yes");
            assert.equal(tags["amenity"], "restaurant");
            assert.equal(tags["uuid"], "{4632d15b-7c44-4ba1-a0c4-8cfbb30e39d4}");
        });

        it('should translate from osm -> mgcp', function() {

            var data = '<osm version="0.6" upload="true" generator="JOSM"><node id="-4" lon="-105.24014094121263" lat="39.28928610944744" version="0"><tag k="poi" v="yes"/><tag k="place" v="town"/><tag k="name" v="Manitou Springs"/><tag k="uuid" v="{4632d15b-7c44-4ba1-a0c4-8cfbb30e39d4}"/></node></osm>';

            var mgcp_xml = server.handleInputs({
                osm: data,
                method: 'POST',
                translation: 'MGCP',
                path: '/translateTo'
            });

            var xml = parser.parseFromString(mgcp_xml);
            var gj = osmtogeojson(xml);

            assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "MGCP");

            var tags = gj.features[0].properties;
            assert.equal(tags["FCODE"], "AL020");
            assert.equal(tags["NAM"], "Manitou Springs");
            assert.equal(tags["FUC"], "999");
            assert.equal(tags["UID"], "4632d15b-7c44-4ba1-a0c4-8cfbb30e39d4");
        });

        it ('translates multiple features in an OSM dataset', function() {
            var osm2trans = server.handleInputs({
                osm: '<osm version="0.6" generator="JOSM"><node id="-41300" action="modify" lat="39.28775629713" lon="-74.55540463462"/><node id="-41301" action="modify" lat="39.28766391666" lon="-74.55532148615"/><node id="-41302" action="modify" lat="39.28770588277" lon="-74.55524365216"/><node id="-41303" action="modify" lat="39.28779826318" lon="-74.55532680064"/><node id="-41306" action="modify" lat="39.28768156238" lon="-74.55568894878"/><node id="-41307" action="modify" lat="39.28766910658" lon="-74.5555467917"/><node id="-41308" action="modify" lat="39.28757547786" lon="-74.55556048652"/><node id="-41309" action="modify" lat="39.28758793368" lon="-74.5557026436"/><node id="-41312" action="modify" lat="39.28771270187" lon="-74.55567687884"/><node id="-41313" action="modify" lat="39.28771581582" lon="-74.55556824937"/><node id="-41314" action="modify" lat="39.28779720499" lon="-74.55557214409"/><node id="-41315" action="modify" lat="39.28779409104" lon="-74.55568077355"/><node id="-41318" action="modify" lat="39.28815176719" lon="-74.55595985189"/><node id="-41319" action="modify" lat="39.28800541239" lon="-74.55586195126"/><node id="-41321" action="modify" lat="39.28771477783" lon="-74.55583378807"/><node id="-41323" action="modify" lat="39.28744490181" lon="-74.5558740212"/><node id="-41325" action="modify" lat="39.28738677453" lon="-74.5556956543"/><node id="-41327" action="modify" lat="39.28747085147" lon="-74.55527320638"/><node id="-41329" action="modify" lat="39.28775837309" lon="-74.55493256584"/><node id="-41331" action="modify" lat="39.28811647599" lon="-74.55479040876"/><node id="-41347" action="modify" lat="39.28804589353" lon="-74.55582037702"/><node id="-41348" action="modify" lat="39.28798659571" lon="-74.55573682457"/><node id="-41350" action="modify" lat="39.28800356093" lon="-74.55572434074"/><node id="-41351" action="modify" lat="39.2880230383" lon="-74.55572260378"/><node id="-41352" action="modify" lat="39.2880411701" lon="-74.55573195772"/><node id="-41353" action="modify" lat="39.2880543651" lon="-74.55575054989"/><node id="-41354" action="modify" lat="39.28806000988" lon="-74.55577469788"/><node id="-41355" action="modify" lat="39.28805698641" lon="-74.5557996189"/><node id="-41356" action="modify" lat="39.28802892832" lon="-74.55583286085"/><node id="-41357" action="modify" lat="39.28800945095" lon="-74.55583459781"/><node id="-41358" action="modify" lat="39.28799131914" lon="-74.55582524388"/><node id="-41359" action="modify" lat="39.28797812413" lon="-74.55580665171"/><node id="-41360" action="modify" lat="39.28797247935" lon="-74.55578250371"/><node id="-41361" action="modify" lat="39.28797550282" lon="-74.5557575827"/><way id="-41304" action="modify"><nd ref="-41300"/><nd ref="-41303"/><nd ref="-41302"/><nd ref="-41301"/><nd ref="-41300"/><tag k="building" v="yes"/></way><way id="-41310" action="modify"><nd ref="-41306"/><nd ref="-41307"/><nd ref="-41308"/><nd ref="-41309"/><nd ref="-41306"/><tag k="building" v="yes"/></way><way id="-41316" action="modify"><nd ref="-41312"/><nd ref="-41315"/><nd ref="-41314"/><nd ref="-41313"/><nd ref="-41312"/><tag k="building" v="yes"/></way><way id="-41320" action="modify"><nd ref="-41318"/><nd ref="-41319"/><nd ref="-41321"/><nd ref="-41323"/><nd ref="-41325"/><nd ref="-41327"/><nd ref="-41329"/><nd ref="-41331"/><tag k="highway" v="tertiary"/></way></osm>',
                method: 'POST',
                translation: 'TDSv61',
                path: '/translateTo'
            });

            xml2js.parseString(osm2trans, function(err, result) {
                if (err) console.log(err)
                // road
                assert.equal(result.osm.way[0].tag[1].$.k, 'F_CODE')
                assert.equal(result.osm.way[0].tag[1].$.v, 'AP030')
                assert.equal(result.osm.way[0].tag[3].$.k, 'RTY')
                assert.equal(result.osm.way[0].tag[3].$.v, '3')
                assert.equal(result.osm.way[0].tag[2].$.k, 'ZI016_WTC')
                assert.equal(result.osm.way[0].tag[2].$.v, '1')
                assert.equal(result.osm.way[0].tag[0].$.k, 'RIN_ROI')
                assert.equal(result.osm.way[0].tag[0].$.v, '5')
                // buildings
                assert.equal(result.osm.way[1].tag[0].$.k, 'F_CODE')
                assert.equal(result.osm.way[1].tag[0].$.v, 'AL013')
                assert.equal(result.osm.way[2].tag[0].$.k, 'F_CODE')
                assert.equal(result.osm.way[2].tag[0].$.v, 'AL013')
                assert.equal(result.osm.way[3].tag[0].$.k, 'F_CODE')
                assert.equal(result.osm.way[3].tag[0].$.v, 'AL013')
            });
        });

        it('include a non-translated feature if feature cannot be translated from OSM to TDSv61', function() {
            var osm2trans = server.handleInputs({
                osm: '<osm version="0.6" generator="JOSM"><node id="-41300" action="modify" lat="39.28775629713" lon="-74.55540463462"/><node id="-41301" action="modify" lat="39.28766391666" lon="-74.55532148615"/><node id="-41302" action="modify" lat="39.28770588277" lon="-74.55524365216"/><node id="-41303" action="modify" lat="39.28779826318" lon="-74.55532680064"/><node id="-41306" action="modify" lat="39.28768156238" lon="-74.55568894878"/><node id="-41307" action="modify" lat="39.28766910658" lon="-74.5555467917"/><node id="-41308" action="modify" lat="39.28757547786" lon="-74.55556048652"/><node id="-41309" action="modify" lat="39.28758793368" lon="-74.5557026436"/><node id="-41312" action="modify" lat="39.28771270187" lon="-74.55567687884"/><node id="-41313" action="modify" lat="39.28771581582" lon="-74.55556824937"/><node id="-41314" action="modify" lat="39.28779720499" lon="-74.55557214409"/><node id="-41315" action="modify" lat="39.28779409104" lon="-74.55568077355"/><node id="-41318" action="modify" lat="39.28815176719" lon="-74.55595985189"/><node id="-41319" action="modify" lat="39.28800541239" lon="-74.55586195126"/><node id="-41321" action="modify" lat="39.28771477783" lon="-74.55583378807"/><node id="-41323" action="modify" lat="39.28744490181" lon="-74.5558740212"/><node id="-41325" action="modify" lat="39.28738677453" lon="-74.5556956543"/><node id="-41327" action="modify" lat="39.28747085147" lon="-74.55527320638"/><node id="-41329" action="modify" lat="39.28775837309" lon="-74.55493256584"/><node id="-41331" action="modify" lat="39.28811647599" lon="-74.55479040876"/><node id="-41347" action="modify" lat="39.28804589353" lon="-74.55582037702"/><node id="-41348" action="modify" lat="39.28798659571" lon="-74.55573682457"/><node id="-41350" action="modify" lat="39.28800356093" lon="-74.55572434074"/><node id="-41351" action="modify" lat="39.2880230383" lon="-74.55572260378"/><node id="-41352" action="modify" lat="39.2880411701" lon="-74.55573195772"/><node id="-41353" action="modify" lat="39.2880543651" lon="-74.55575054989"/><node id="-41354" action="modify" lat="39.28806000988" lon="-74.55577469788"/><node id="-41355" action="modify" lat="39.28805698641" lon="-74.5557996189"/><node id="-41356" action="modify" lat="39.28802892832" lon="-74.55583286085"/><node id="-41357" action="modify" lat="39.28800945095" lon="-74.55583459781"/><node id="-41358" action="modify" lat="39.28799131914" lon="-74.55582524388"/><node id="-41359" action="modify" lat="39.28797812413" lon="-74.55580665171"/><node id="-41360" action="modify" lat="39.28797247935" lon="-74.55578250371"/><node id="-41361" action="modify" lat="39.28797550282" lon="-74.5557575827"/><way id="-41304" action="modify"><nd ref="-41300"/><nd ref="-41303"/><nd ref="-41302"/><nd ref="-41301"/><nd ref="-41300"/><tag k="building" v="yes"/></way><way id="-41310" action="modify"><nd ref="-41306"/><nd ref="-41307"/><nd ref="-41308"/><nd ref="-41309"/><nd ref="-41306"/><tag k="building" v="yes"/></way><way id="-41316" action="modify"><nd ref="-41312"/><nd ref="-41315"/><nd ref="-41314"/><nd ref="-41313"/><nd ref="-41312"/><tag k="building" v="yes"/></way><way id="-41320" action="modify"><nd ref="-41318"/><nd ref="-41319"/><nd ref="-41321"/><nd ref="-41323"/><nd ref="-41325"/><nd ref="-41327"/><nd ref="-41329"/><nd ref="-41331"/><tag k="highway" v="tertiary"/></way><way id="-38983" visible="true"><nd ref="-38979" /><nd ref="-38982" /><nd ref="-38986" /><nd ref="-38979" /><tag k="natural" v="tree" /><tag k="security:classification" v="UNCLASSIFIED" /><tag k="source" v="Unknown" /></way></osm>',
                method: 'POST',
                translation: 'TDSv61',
                path: '/translateTo'
            });

            xml2js.parseString(osm2trans, function(err, result) {
              if (err) console.log(err)
              // tree that cannot be translated...
              assert.equal(result.osm.way[4].tag[0].$.k, 'error')
              assert.equal(result.osm.way[4].tag[0].$.v, 'Area geometry is not valid for EC005 in TDSv61')
            })
        });

        it('should translate OTH from tdsv61 -> osm -> tdsv61', function() {

            var data = '<osm version="0.6" upload="true" generator="hootenanny"><node id="-19" lon="9.304397440128325" lat="41.65083522130027" version="0"><tag k="FCSUBTYPE" v="100080"/><tag k="ZI001_SDP" v="DigitalGlobe"/><tag k="UFI" v="0d8b2563-81cf-44d4-8ef7-52c0e862651f"/><tag k="F_CODE" v="AL010"/><tag k="ZI006_MEM" v="&lt;OSM&gt;{&quot;source:imagery:datetime&quot;:&quot;2017-11-11 10:45:15&quot;,&quot;source:imagery:sensor&quot;:&quot;WV02&quot;,&quot;source:imagery:id&quot;:&quot;756b80e1f695fb591caca8e7ce0f9ef5&quot;}&lt;/OSM&gt;"/><tag k="ZSAX_RS0" v="U"/><tag k="OTH" v="(FFN:foo)"/></node></osm>';

            var osm_xml = server.handleInputs({
                osm: data,
                method: 'POST',
                translation: 'TDSv61',
                path: '/translateFrom'
            });
            //console.log(osm_xml);

            var xml = parser.parseFromString(osm_xml);
            var gj = osmtogeojson(xml);

            assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "OSM");

            var tags = gj.features[0].properties;
            assert.equal(tags["facility"], "yes");
            assert.equal(tags["note:oth"], "(FFN:foo)");
            assert.equal(tags["security:classification"], "UNCLASSIFIED");
            assert.equal(tags["uuid"], "{0d8b2563-81cf-44d4-8ef7-52c0e862651f}");
            assert.equal(tags["source"], "DigitalGlobe");
            assert.equal(tags["source:imagery:id"], "756b80e1f695fb591caca8e7ce0f9ef5");
            assert.equal(tags["source:imagery:datetime"], "2017-11-11 10:45:15");
            assert.equal(tags["source:imagery:sensor"], "WV02");

            var tds_xml = server.handleInputs({
                osm: osm_xml,
                method: 'POST',
                translation: 'TDSv61',
                path: '/translateTo'
            });
            //console.log(tds_xml);
            xml = parser.parseFromString(tds_xml);
            gj = osmtogeojson(xml);

            assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "TDSv61");

            var tags = gj.features[0].properties;
            assert.equal(tags["F_CODE"], "AL010");
            assert.equal(tags["ZI001_SDP"], "DigitalGlobe");
            assert.equal(tags["ZSAX_RS0"], "U");
            assert.equal(tags["OTH"], "(FFN:foo)");
            assert.equal(tags["UFI"], "0d8b2563-81cf-44d4-8ef7-52c0e862651f");

        });

        it('should handle tdstoosm POST of power line feature', function() {
            var trans2osm = server.handleInputs({
                osm: '<osm version="0.6" upload="true" generator="hootenanny"><way id="-6" version="0"><nd ref="-13"/><nd ref="-14"/><nd ref="-15"/><nd ref="-16"/><tag k="UID" v="fee4529b-5ecc-4e5c-b06d-1b26a8e830e6"/><tag k="FCODE" v="AT030"/></way></osm>',
                method: 'POST',
                translation: 'MGCP',
                path: '/translateFrom'
            });
            var output = xml2js.parseString(trans2osm, function(err, result) {
                if (err) console.error(err);
                assert.equal(result.osm.way[0].tag[1].$.k, "uuid");
                assert.equal(result.osm.way[0].tag[1].$.v, "{fee4529b-5ecc-4e5c-b06d-1b26a8e830e6}");
                assert.equal(result.osm.way[0].tag[0].$.k, "power");
                assert.equal(result.osm.way[0].tag[0].$.v, "line");
            });
        });
        it('should handle tdstoosm POST of power line feature', function() {
            var trans2osm = server.handleInputs({
                osm: '<osm version="0.6" upload="true" generator="hootenanny"><way id="-6" version="0"><nd ref="-13"/><nd ref="-14"/><nd ref="-15"/><nd ref="-16"/><tag k="UFI" v="fee4529b-5ecc-4e5c-b06d-1b26a8e830e6"/><tag k="F_CODE" v="AT005"/><tag k="CAB" v="2"/></way></osm>',
                method: 'POST',
                translation: 'TDSv61',
                path: '/translateFrom'
            });
            var output = xml2js.parseString(trans2osm, function(err, result) {
                if (err) console.error(err);
                assert.equal(result.osm.way[0].tag[1].$.k, "uuid");
                assert.equal(result.osm.way[0].tag[1].$.v, "{fee4529b-5ecc-4e5c-b06d-1b26a8e830e6}");
                assert.equal(result.osm.way[0].tag[0].$.k, "power");
                assert.equal(result.osm.way[0].tag[0].$.v, "line");
            });
        });

        it('should handle tdstoosm POST of facility area feature', function() {
            var trans2osm = server.handleInputs({
                osm: '<osm version="0.6" upload="true" generator="hootenanny"><way id="-6" version="0"><nd ref="-13"/><nd ref="-14"/><nd ref="-15"/><nd ref="-16"/><nd ref="-13"/><tag k="UID" v="fee4529b-5ecc-4e5c-b06d-1b26a8e830e6"/><tag k="FCODE" v="AL010"/><tag k="SDP" v="D"/></way></osm>',
                method: 'POST',
                translation: 'MGCP',
                path: '/translateFrom'
            });
            var output = xml2js.parseString(trans2osm, function(err, result) {
                if (err) console.error(err);
                assert.equal(result.osm.way[0].tag[0].$.k, "source");
                assert.equal(result.osm.way[0].tag[0].$.v, "D");
                assert.equal(result.osm.way[0].tag[2].$.k, "uuid");
                assert.equal(result.osm.way[0].tag[2].$.v, "{fee4529b-5ecc-4e5c-b06d-1b26a8e830e6}");
                assert.equal(result.osm.way[0].tag[3].$.k, "facility");
                assert.equal(result.osm.way[0].tag[3].$.v, "yes");
                assert.equal(result.osm.way[0].tag[1].$.k, "area");
                assert.equal(result.osm.way[0].tag[1].$.v, "yes");
            });
        });

        it('should return error message for invalid F_CODE/geom combination in translateTo POST', function() {
            var output = server.handleInputs({
                osm: '<osm version="0.6" upload="true" generator="hootenanny"><node id="-24" lon="9.305143094234467" lat="41.65140640721789" version="0"><tag k="leisure" v="park"/><tag k="source" v="DigitalGlobe"/><tag k="source:imagery:sensor" v="WV02"/><tag k="source:imagery:id" v="756b80e1f695fb591caca8e7ce0f9ef5"/><tag k="source:imagery:datetime" v="2017-11-11 10:45:15"/><tag k="security:classification" v="UNCLASSIFIED"/></node></osm>',
                method: 'POST',
                translation: 'MGCP',
                path: '/translateTo'
            });
            var xml = parser.parseFromString(output);
            var gj = osmtogeojson(xml);

            assert.equal(xml.getElementsByTagName("osm")[0].getAttribute("schema"), "MGCP");

            var tags = gj.features[0].properties;
            assert.equal(tags["error"], 'Point geometry is not valid for AK120 in MGCP TRD4');
        });

        it('should handle bad translation schema value in translateTo POST', function() {
            assert.throws(function error() {
                var osm2trans = server.handleInputs({
                    osm: '<osm version="0.6" upload="true" generator="JOSM"><node id="-1" lon="-105.21811763904256" lat="39.35643172777992" version="0"><tag k="building" v="yes"/><tag k="uuid" v="{bfd3f222-8e04-4ddc-b201-476099761302}"/></node></osm>',
                    method: 'POST',
                    translation: 'TDv61',
                    path: '/translateTo'
                });
            }, Error, 'Unsupported translation schema');
        });

        it('should handle translateFrom POST', function() {
            var trans2osm = server.handleInputs({
                osm: '<osm version="0.6" upload="true" generator="JOSM"><node id="-9" lon="-104.907037158172" lat="38.8571566428667" version="0"><tag k="ACC" v="1"/><tag k="BAC" ve="0"/><tag k="CCN" v="UNK"/><tag k="FCODE" v="AL020"/><tag k="FUC" v="999"/><tag k="FUN" v="0"/><tag k="NAM" v="Manitou Springs"/><tag k="NFI" v="UNK"/><tag k="NFN" v="UNK"/><tag k="ORD" v="0"/><tag k="SDP" v="N_A"/><tag k="SDV" v="UNK"/><tag k="SRT" v="0"/><tag k="TXT" v="&lt;OSM&gt;{&quot;poi&quot;:&quot;yes&quot;}&lt;/OSM&gt;"/><tag k="UID" v="c6df0618-ce96-483c-8d6a-afa33541646c"/></node></osm>',
                method: 'POST',
                translation: 'MGCP',
                path: '/translateFrom'
            });

            var output = xml2js.parseString(trans2osm, function(err, result) {
                if (err) console.error(err);
                assert.equal(result.osm.node[0].tag[4].$.k, "use");
                assert.equal(result.osm.node[0].tag[4].$.v, "other");
                assert.equal(result.osm.node[0].tag[6].$.k, "place");
                assert.equal(result.osm.node[0].tag[6].$.v, "yes");
                assert.equal(result.osm.node[0].tag[5].$.k, "poi");
                assert.equal(result.osm.node[0].tag[5].$.v, "yes");
                assert.equal(result.osm.node[0].tag[3].$.k, "uuid");
                assert.equal(result.osm.node[0].tag[3].$.v, "{c6df0618-ce96-483c-8d6a-afa33541646c}");
                assert.equal(result.osm.node[0].tag[2].$.k, "name");
                assert.equal(result.osm.node[0].tag[2].$.v, "Manitou Springs");
                assert.equal(result.osm.node[0].tag[1].$.k, "landuse");
                assert.equal(result.osm.node[0].tag[1].$.v, "built_up_area");
                assert.equal(result.osm.node[0].tag[0].$.k, "source:accuracy:horizontal:category");
                assert.equal(result.osm.node[0].tag[0].$.v, "accurate");
            });
        });

        it('should untangle MGCP tags', function() {
            var trans2osm = server.handleInputs({
                osm: '<osm version="0.6" upload="true" generator="hootenanny"><way id="-1" version="0"><nd ref="-1"/><nd ref="-4"/><nd ref="-7"/><nd ref="-10"/><nd ref="-1"/><tag k="FCODE" v="AL013"/><tag k="levels" v="3"/>/></way></osm>',
                method: 'POST',
                translation: 'MGCP',
                path: '/translateFrom'
            });
            xml2js.parseString(trans2osm, function(err, result) {
                if (err) console.error(err);
                assert.equal(result.osm.way[0].tag[0].$.k, "levels");
                assert.equal(result.osm.way[0].tag[0].$.v, "3");
                assert.equal(result.osm.way[0].tag[1].$.k, "building");
                assert.equal(result.osm.way[0].tag[1].$.v, "yes");
            });
        });

        it('should untangle TDSv61 tags', function() {
            var trans2osm = server.handleInputs({
                osm: '<osm version="0.6" upload="true" generator="hootenanny"><way id="-1" version="0"><nd ref="-1"/><nd ref="-4"/><nd ref="-7"/><nd ref="-10"/><nd ref="-1"/><tag k="AL013" v="building"/><tag k="levels" v="3"/>/></way></osm>',
                method: 'POST',
                translation: 'TDSv61',
                path: '/translateFrom'
            });
            xml2js.parseString(trans2osm, function(err, result) {
                if (err) console.error(err);
                assert.equal(result.osm.way[0].tag[0].$.k, "levels");
                assert.equal(result.osm.way[0].tag[0].$.v, "3");
                assert.equal(result.osm.way[0].tag[1].$.k, "building");
                assert.equal(result.osm.way[0].tag[1].$.v, "yes");
            });
        });

        it('should untangle TDSv40 tags', function() {
            var trans2osm = server.handleInputs({
                osm: '<osm version="0.6" upload="true" generator="hootenanny"><way id="-1" version="0"><nd ref="-1"/><nd ref="-4"/><nd ref="-7"/><nd ref="-10"/><nd ref="-1"/><tag k="AL013" v="building"/><tag k="levels" v="3"/>/></way></osm>',
                method: 'POST',
                translation: 'TDSv40',
                path: '/translateFrom'
            });
            xml2js.parseString(trans2osm, function(err, result) {
                if (err) console.error(err);
                assert.equal(result.osm.way[0].tag[0].$.k, "levels");
                assert.equal(result.osm.way[0].tag[0].$.v, "3");
                assert.equal(result.osm.way[0].tag[1].$.k, "building");
                assert.equal(result.osm.way[0].tag[1].$.v, "yes");
            });
        });

        it('should handle /capabilities GET', function() {

            var capas = server.handleInputs({
                method: 'GET',
                path: '/capabilities'
            });
            assert.equal(capas.TDSv61.isavailable, true);
            assert.equal(capas.TDSv40.isavailable, true);
            assert.equal(capas.MGCP.isavailable, true);
            assert.equal(capas.GGDMv30.isavailable, true);
        });

        it('should handle /translations GET', function() {

            var trans = server.handleInputs({
                method: 'GET',
                path: '/translations'
            });
            assert.equal(4, trans.length);
        });

        it('should handle /schema GET', function() {
            //http://localhost:8094/schema?geometry=point&translation=MGCP&searchstr=Buil&maxlevdst=20&limit=12
            var schm = server.handleInputs({
                geometry: 'line',
                translation: 'TDSv40',
                searchstr: 'river',
                maxLeinDistance: 20,
                limit: 12,
                method: 'GET',
                path: '/schema'
            });
            assert.equal(schm[0].name, 'RIVER_C');
            assert.equal(schm[0].fcode, 'BH140');
            assert.equal(schm[0].desc, 'River');
        });

        //Checking the use of limit param
        it('should handle /schema GET', function() {
            var schm = server.handleInputs({
                geometry: 'line',
                translation: 'TDSv40',
                searchstr: 'river',
                maxleindst: 10,
                limit: 1,
                method: 'GET',
                path: '/schema'
            });
            assert.equal(schm.length, 1);
        });

        it('should handle /schema GET', function() {
            var schm = server.handleInputs({
                geometry: 'Line',
                translation: 'TDSv40',
                searchstr: 'ri',
                maxleindst: 200,
                limit: 33,
                method: 'GET',
                path: '/schema'
            });

            assert.equal(schm.length, 8);
        });

        it('should handle /schema GET', function() {
            var schm = server.handleInputs({
                geometry: 'line',
                translation: 'TDSv40',
                searchstr: 'ri',
                maxleindst: 50,
                limit: 100,
                method: 'GET',
                path: '/schema'
            });
            assert(schm.length <= 100, 'Schema search results greater than requested');
        });

        //Checking the use of limit param with no search string
        it('should handle /schema GET', function() {
            var schm = server.handleInputs({
                geometry: 'line',
                translation: 'TDSv40',
                searchstr: '',
                maxleindst: 10,
                limit: 1,
                method: 'GET',
                path: '/schema'
            });
            assert.equal(schm.length, 1);
        });

        it('should handle /schema GET', function() {
            var schm = server.handleInputs({
                geometry: 'point',
                translation: 'TDSv61',
                searchstr: '',
                limit: 33,
                method: 'GET',
                path: '/schema'
            });
            assert.equal(schm.length, 33);
        });

        it('should handle /schema GET', function() {
            var schm = server.handleInputs({
                geometry: 'line',
                translation: 'MGCP',
                searchstr: '',
                maxlevdst: 0, //This shouldn't be used when searchstr is ''
                limit: 100,
                method: 'GET',
                path: '/schema'
            });
            assert(schm.length <= 100, 'Schema search results greater than requested');
            assert(schm.some(function(d) {
                return d.desc === 'Railway';
            }));
        });

        it('should handle /schema GET', function() {
            var schm = server.handleInputs({
                geometry: 'line',
                translation: 'GGDMv30',
                searchstr: '',
                maxlevdst: 0, //This shouldn't be used when searchstr is ''
                limit: 100,
                method: 'GET',
                path: '/schema'
            });
            assert(schm.length <= 100, 'Schema search results greater than requested');
            assert(schm.some(function(d) {
                return d.desc === 'Railway';
            }));
        });

        it('throws error if url not found', function() {
            assert.throws(function error() {
                server.handleInputs({
                    idval: 'AL015',
                    geom: 'Point',
                    translation: 'TDSv40',
                    idelem: 'fcode',
                    method: 'GET',
                    path: '/foo'
                })
            }, Error, 'Not found');
        });

        it('throws error if unsupported method', function() {
            assert.throws(function error() {
                server.handleInputs({
                    method: 'POST',
                    path: '/schema'
                })
            }, Error, 'Unsupported method');
        });

        it('throws error if unsupported method', function() {
            assert.throws(function error() {
                server.handleInputs({
                    method: 'POST',
                    path: '/capabilities'
                })
            }, Error, 'Unsupported method');
        });
     });

    describe('capabilities', function () {
      it('should return 200', function (done) {
        var request  = httpMocks.createRequest({
            method: 'GET',
            url: '/capabilities'
        });
        var response = httpMocks.createResponse();
        server.TranslationServer(request, response);
        assert.equal(response.statusCode, '200');
        done();
      });
    });

    describe('schema', function () {
      it('should return 200', function (done) {
        var request  = httpMocks.createRequest({
            method: 'GET',
            url: '/schema',
            params: {
                geometry: 'point',
                translation: 'MGCP',
                searchstr: 'Buil',
                maxlevdst: 20,
                limit: 12
            }
        });
        var response = httpMocks.createResponse();
        server.TranslationServer(request, response);
        assert.equal(response.statusCode, '200');
        done();
      });
    });

    describe('translateTo', function () {
      it('should return 200', function (done) {
        var request  = httpMocks.createRequest({
            method: 'GET',
            url: '/translateTo',
            params: {
                value: 'AP030',
                translation: 'MGCP',
                geom: 'Line',
                key: 'fcode'
            }
        });
        var response = httpMocks.createResponse();
        server.TranslationServer(request, response);
        assert.equal(response.statusCode, '200');
        done();
      });
    });

    describe('translateFrom', function () {
      it('should return 200', function (done) {
        var request  = httpMocks.createRequest({
            method: 'GET',
            url: '/translateFrom',
            params: {
                fcode: 'AL013',
                translation: 'TDSv61'
            }
        });
        var response = httpMocks.createResponse();
        server.TranslationServer(request, response);
        assert.equal(response.statusCode, '200');
        done();
      });
      it('should return 200', function (done) {
        var request  = httpMocks.createRequest({
            method: 'GET',
            url: '/translateFrom',
            params: {
                idelem: 'fcode',
                idval: 'AL013',
                geom: 'Area',
                translation: 'MGCP'
            }
        });
        var response = httpMocks.createResponse();
        server.TranslationServer(request, response);
        assert.equal(response.statusCode, '200');
        done();
      });
    });

    describe('supportedGeometries', function() {
        it ('replies supported geometries for provided feature code', function() {
            var baseParams = {
                method: 'GET',
                path: '/supportedGeometries',
            }
            var tds61Building = Object.assign(baseParams, { translation: 'TDSv61', fcode: 'AL013' });
            assert.deepEqual(server.handleInputs(tds61Building), ['Point', 'Area']);
            var tds40Wall = Object.assign(baseParams, { translation: 'TDSv40', fcode: 'AL260' })
            assert.deepEqual(server.handleInputs(tds40Wall), [ 'Line' ]);
            var mgcpFord = Object.assign(baseParams, { translation: 'MGCP', fcode: 'BH070' });
            assert.deepEqual(server.handleInputs(mgcpFord), [ 'Line', 'Point' ]);
            var ggdm30Tower = Object.assign(baseParams, { translation: 'GGDMv30' , fcode: 'AL241'})
            assert.deepEqual(server.handleInputs(ggdm30Tower), [ 'Point', 'Area' ]);
        })
    });

    describe('not found', function () {
      it('should return 404', function (done) {
        var request  = httpMocks.createRequest({
            method: 'GET',
            url: '/foo',
            params: {
                fcode: 'AL013',
                translation: 'TDSv61'
            }
        });
        var response = httpMocks.createResponse();
        server.TranslationServer(request, response);
        assert.equal(response.statusCode, '404');
        done();
      });
    });

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

    describe('getIntendedKeys', function() {
        it('should return ["a", "x", "s"] when provided "z"', function() {
            var intendedKeys = server.getIntendedKeys('z');

            assert.equal(intendedKeys[0], 'a')
            assert.equal(intendedKeys[1], 'x')
            assert.equal(intendedKeys[2], 's')
        });
        it('should return same result when passed "{" or "["', function() {
            var leftSqiglyKeys = server.getIntendedKeys('{'),
                leftStraigthKeys = server.getIntendedKeys('[');

            assert.equal(leftSqiglyKeys[0], leftStraigthKeys[0]);
        });
    });

    describe('getFuzzyString', function() {
        it('should return ["cuilding", "fuilding", "guilding", building"] when passed "vuilding',
            function() {
                var fuzzyStrings = server.getFuzzyStrings('vuilding');

                assert.equal(fuzzyStrings[0], 'cuilding')
                assert.equal(fuzzyStrings[1], 'fuilding')
                assert.equal(fuzzyStrings[2], 'guilding')
                assert.equal(fuzzyStrings[3], 'building')
            }
        )
        it('should return surf and furf when passed durf', function() {
            var fuzzyStrings = server.getFuzzyStrings('durf');

            assert.equal(fuzzyStrings[0], 'surf')
            assert.equal(fuzzyStrings[1], 'furf');
        })
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
            it('includes words including "mine" at reasonable index', function() {
                var options = {
                        geomType: 'area',
                        translation: 'TDSv61',
                        searchStr: 'mine',
                        limitResult: 12,
                        maxLeinDistance: 100
                    },
                alResults = server.searchSchema(options),
                includesMine = alResults.filter(function(d, index) {
                    return /mine/.test(d.desc.toLowerCase()) && index < 99;
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
            });

            it('includes items with installation in description when searchStr is intall, isnall, or insralkl at reasonable index',
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
                            includesInstallation = misTypeResults.filter(function(d, index) {
                                return /installation/.test(d.desc.toLowerCase()) && index < 99;
                            }).length > 0;

                        assert.equal(includesInstallation, true);
                    })
                }
            );
            it('includes items with building in description when searchStr is bugidln, bidng, or buldng at reasonable index',
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
                            includesBuilding = misTypeResults.filter(function(d, index) {
                                return /building/.test(d.desc.toLowerCase()) && index < 99;
                            }).length > 0;

                        assert.equal(includesBuilding, true);
                    });
                }
            )
        });
        describe('fuzzyKeyMatching', function() {
            it('searching "toad" should include "road" in results at reasonable index', function() {
                var options = {
                    searchStr: 'toad',
                    translation: 'TDSv61',
                    maxLeinDistance: 200,
                    geomType: 'Area'
                }

                var includesRoad = server.searchSchema(options).filter(function(d, index) {
                    return d.desc.toLowerCase().indexOf('road') !== -1 && index < 99;
                }).length > 0;

                assert.equal(includesRoad, true);
            })

            it('searching "vilding" or "vuilding" should both include "building" in results at reasonable index', function() {
                var options = {
                    translation: 'TDSv61',
                    maxLeinDistance: 200,
                    geomType: 'area'
                }

                var bothIncludeBuilding = [
                    'vuilding',
                    'vilding'
                ].filter(function(misTyped) {
                    options.searchStr = misTyped;

                    return server.searchSchema(options)
                        .filter(function(d, index) {
                            return d.desc.toLowerCase().indexOf('building') && index < 49;
                    }).length > 0;
                }).length > 0;

                assert.equal(bothIncludeBuilding, true);
            })
        })
    });
});

