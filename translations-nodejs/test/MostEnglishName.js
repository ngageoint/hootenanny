
var assert = require('assert');

var HOOT_HOME = process.env.HOOT_HOME;
var hoot = require(HOOT_HOME + '/lib/HootJs');

hoot.Log.setLogLevel('warn');

describe('schema', function() {
    it('Pick the most english name', function() {
        var tags = {
            'name': 'foo',
            'alt_name': "Joe's Bar;бар Джо",
            'amenity': 'bar'
        }

        assert.deepEqual(
            "Joe's Bar",
            new hoot.MostEnglishName().getMostEnglishName(tags));
    }).timeout(5000);

});
