const debug = require('debug')('hoot-translate');
var mgcp = require('./mgcp_translations.js');
var xml2js = require('xml2js');

config = require('./config.js');
// Make sure the MGCP translator exports extra tags to the TXT field
config.OgrMgcpExtra = 'note';
// Throw errors instead of returning partial translations/o2s_X features
config.OgrThrowError = true;

function getTranslator(translation) {
    var translator;
    switch (translation) {
        case 'MGCP':
        default:
            translator = mgcp;
            break;
    }
    return translator;
}

module.exports = {
    mgcp: mgcp,
    translateTo: function(osm, translation, callback) {
        var translator = getTranslator(translation);
        xml2js.parseString(osm, function (err, result) {
            //console.log(JSON.stringify(result));

//{"osm":{"$":{"version":"0.6","upload":"true","generator":"hootenanny"},"bounds":[{"$":{"minlat":"30.6425644","minlon":"34.2201805","maxlat":"30.6558557","maxlon":"34.2422819","origin":"MapEdit server"}}],"way":[{"$":{"id":"23","version":"0"},"nd":[{"$":{"ref":"6"}},{"$":{"ref":"7"}},{"$":{"ref":"8"}},{"$":{"ref":"9"}},{"$":{"ref":"7"}}],"tag":[{"$":{"k":"building","v":"yes"}},{"$":{"k":"uuid","v":"fee4529b-5ecc-4e5c-b06d-1b26a8e830e6"}}]}]}}

            //Loop through nodes, ways, and relations
            //gather tags and translate them
            ['node', 'way', 'relation'].forEach(function(element) {
                if (result.osm[element])
                    result.osm[element].forEach(function(e) {
                        var area = false;
                        //gather tags
                        var tags = e.tag.reduce(function(map, t) {
                            //if (t.$.k === 'area' && t.$.v)
                            map[t.$.k] = t.$.v;
                            return map;
                        }, {});

                        console.log(tags);
                        //translate them
                        var translated = translator.OSMtoRaw(tags, '', 'Area')[0].attrs;

                        console.log(translated);
                        //transform them
                        var transformed = Object.keys(translated).map(function(k) {
                            return {$: {k: k, v: translated[k]}};
                        });

                        console.log(transformed);
                        //update element
                        e.tag = transformed;
                    });
            });

            var builder = new xml2js.Builder();
            var xml = builder.buildObject(result);
            callback(xml);

        });
    },
    translateFrom: function(osm, translation) {

    }
}