//Used for two-step translation
//from custom->TDSvXX->OSM
if (typeof hoot !== 'undefined') {
    hoot.require('etds40_osm')
    hoot.require('etds61_osm')
}


translation_assistant = {
    difference: function(a, b) {
            var diff = [];
            for (var k in a) {
                if (b.indexOf(a[k]) === -1) {
                    diff.push(a[k]);
                }
            }
            return diff;
        },

    //Takes 'attrs' and returns OSM 'tags'
    translateAttributes: function(attrs, layerName, attributeMapping, fcode, schema)
    {
        var tags = {};
        var extras = []
        var l = attributeMapping[layerName];

        //If layerName has no match in the mapping
        //compare columns for a match
        if (!l) {
            for (var lyr in attributeMapping) {
                if (translation_assistant.difference(Object.keys(attrs), Object.keys(attributeMapping[lyr])).length === 0) {
                    l = attributeMapping[lyr];
                    break;
                }
            }
        }

        //Don't translate feature if no matching attribute mapping could be found
        if (!l) {
            return null;
        }

        for (var key in attrs)
        {
            var k = key;
            var m = l[k];
            if (m && m != "IGNORED") { //attribute does map to a tag
                for (var tagKey in m) {
                    var tagValue = m[tagKey];
                    if (tagKey == "note" || tagKey == "Note : Memorandum") { //tag is list of extra attributes to be concatenated, att1=val1;att2=val2;att3=val3
                        if (tags[tagKey] && (tags[tagKey] !== attrs[key])) { //append values to existing tags
                            tags[tagKey] = tags[tagKey] + ";" + k + "=" + attrs[key];
                        } else {
                            tags[tagKey] = k + "=" + attrs[key];
                        }
                    } else if (tagValue == k) { //tag is set to attribute value
                        if (tags[tagKey] && (tags[tagKey] !== attrs[key])) { //append values to existing tags
                            tags[tagKey] = tags[tagKey] + ";" + attrs[key];
                        } else {
                            tags[tagKey] = attrs[key];
                        }
                    } else if (typeof tagValue === 'string') { //attribute is mapped to a static tag
                        tags[tagKey] = tagValue;
                    } else { //attribute value is mapped to a tag value (if tag key and value is not empty string)
                        if (attrs[key].length > 0 && tagValue[attrs[key]].length > 0) {
                            if (tags[tagKey] && (tags[tagKey] !== tagValue[attrs[key]])) { //append values to existing tags
                                tags[tagKey] = tags[tagKey] + ";" + tagValue[attrs[key]];
                            } else {
                                tags[tagKey] = tagValue[attrs[key]];
                            }
                        }
                    }
                }
            }
        }


        //If necessary, convert from English TDS to OSM+
        if (tags['Feature Code'] || fcode) {
            if (!tags['Feature Code'] && fcode) {
                tags['Feature Code'] = fcode;
            }
            var osmplus;
            switch(schema) {
            case 'TDSv40':
            default:
                osmplus = etds40_osm.toOSM(tags, '', '');
                break;
            case 'TDSv61':
                osmplus = etds61_osm.toOSM(tags, '', '');
                break;
            }
            if (osmplus) {
                return osmplus.attrs;
            } else {
                return null;
            }
        }

        //Don't translate feature if no attrs were translated to tags
        if (Object.keys(tags).length === 0) {
            return null;
        }

        return tags;
    }
}

if (typeof exports !== 'undefined') {
    exports.translateAttributes = translation_assistant.translateAttributes;
    exports.difference = translation_assistant.difference;
}
