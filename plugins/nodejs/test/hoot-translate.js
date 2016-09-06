var assert = require('assert');
var translator = require('../hoot-translate.js');

describe('mgcp', function () {

    it('should translate OSM to MGCP', function() {
        var data = translator.mgcp.toMgcp({
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

   it('should translate MGCP to OSM', function() {
        var data = translator.mgcp.toOsm({
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

});
