var assert = require('assert');
var translator = require('../hoot-translate.js');

describe('hoot mgcp translations', function () {

    it('should translate OSM to MGCP raw', function() {
        var data = translator.mgcp.OSMtoRaw({
            'error:circular': '5',
            'highway': 'road',
            'hoot:status': '1',
            'name': '23RD ST NW',
            'hoot': 'DcGisRoadsCucumber'
        }, '', 'Line');
        assert.equal(data[0].attrs.NAM, '23RD ST NW');
        assert.equal(data[0].attrs.FCODE, 'AP030');
        assert.equal(data[0].attrs.HCT, '0');
    });

   it('should translate MGCP raw to OSM', function() {
        var data = translator.mgcp.RawtoOSM({
            //'error:circular': '5',
            'FCODE': 'AP030',
            //'hoot:status': '1',
            'NAM': '23RD ST NW',
            'HCT': '0'
            //'hoot': 'DcGisRoadsCucumber'
        }, '', 'Line');
        assert.equal(data.highway, 'road');
        assert.equal(data.name, '23RD ST NW');
    });

    it('should translate OSM to MGCP english', function() {
        var data = translator.mgcp.OSMtoEnglish({
            'error:circular': '5',
            'highway': 'road',
            'hoot:status': '1',
            'name': '23RD ST NW',
            'hoot': 'DcGisRoadsCucumber'
        }, '', 'Line');
        assert.equal(data.attrs['Name'], '23RD ST NW');
        assert.equal(data.attrs['Feature Code'], 'AP030:Road');
        assert.equal(data.attrs['Thoroughfare Class'], 'Unknown');
    });

    it('should translate MGCP english to OSM', function() {
        var data = translator.mgcp.EnglishtoOSM({
            //'error:circular': '5',
            'Feature Code': 'AP030:Road',
            //'hoot:status': '1',
            'Name': '23RD ST NW',
            'Thoroughfare Class': 'Unknown'
            //'hoot': 'DcGisRoadsCucumber'
        }, '', 'Line');
        assert.equal(data.attrs.highway, 'road');
        assert.equal(data.attrs.name, '23RD ST NW');
    });

    it('should translate OSM to MGCP english', function() {
        var data = translator.mgcp.OSMtoEnglish({
            use: 'other',
            place: 'yes',
            poi: 'yes',
            uuid: '{c6df0618-ce96-483c-8d6a-afa33541646c}',
            name: 'Manitou Springs',
            landuse: 'built_up_area',
            'source:accuracy:horizontal:category': 'accurate'
        }, '', 'Point');
        assert.equal(data.attrs['Name'], 'Manitou Springs');
        assert.equal(data.attrs['Feature Code'], 'AL020:Built-Up Area');
        assert.equal(data.attrs['Associated Text'], '<OSM>{"poi":"yes"}</OSM>');
        assert.equal(data.attrs['Functional Use'], 'Other');
        assert.equal(data.attrs['Horizontal Accuracy Category'], 'Accurate');
    });

});
