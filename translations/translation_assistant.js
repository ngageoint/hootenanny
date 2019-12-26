//Used for two-step translation
//from custom->TDSvXX->OSM
if (typeof hoot !== 'undefined') {
    hoot.require('etds40_osm')
    hoot.require('etds61_osm')
    hoot.require('etds70_osm')
    hoot.require('emgcp_osm')
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
    // Set the tag `tagKey=value` or append it unless `replace` is `true`
    setTag: function(tags, tagKey, value, replace)
    {
        if (replace) { //force the tag value replacement, order is not possible to define in translation mapping :(
            tags[tagKey] = value;
        }
        else if (tags[tagKey] && (tags[tagKey] !== value)) { //append values to existing tags
            if (tags[tagKey] === "yes") { //replace a tag=yes with the new value instead of appending
                tags[tagKey] = value;
            } else { //append the value if unique and not yes
                var values = tags[tagKey].split(";");

                if (values.indexOf(value) === -1) {
                    tags[tagKey] = tags[tagKey] + ";" + value;
                }
            }
        } else {
            tags[tagKey] = value;
        }
    },
    // Takes 'attrs' and returns OSM 'tags'
    translateAttributes: function(attrs, layerName, attributeMapping, fcode, schema, replace)
    {
        var tags = {};
        var extras = []
        var l = attributeMapping[layerName];

        //If layerName has no match in the mapping, compare columns for a match
        if (!l) {
            for (var lyr in attributeMapping) {
                if (translation_assistant.difference(Object.keys(attrs), Object.keys(attributeMapping[lyr])).length === 0) {
                    l = attributeMapping[lyr];
                    break;
                }
            }
        }

        // If the layerName does not match and we only have one layer in the translation, use it.
        // Yes, the columns could be different but making the translation useable with other layers is what was asked for.
        if (!l) {
            var amKeys = Object.keys(attributeMapping);
            if (amKeys.length == 1) {
                l = attributeMapping[amKeys[0]];
            }            
        }

        // Don't translate feature if no matching attribute mapping could be found
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
                        translation_assistant.setTag(tags, tagKey, attrs[key], replace);
                    } else if (typeof tagValue === 'string') { //attribute is mapped to a static tag
                        translation_assistant.setTag(tags, tagKey, tagValue, replace);
                    } else { //attribute value is mapped to a tag value (if tag key and value is not empty string)
                        if (attrs[key].length > 0) {
                            if (tagValue[attrs[key]] && tagValue[attrs[key]].length > 0) {
                                translation_assistant.setTag(tags, tagKey, tagValue[attrs[key]], replace);
                            } else { //check if the attribute value is a regular expression and run that
                                for (var attrReg in tagValue) {
                                    if (attrReg.startsWith("/") && attrReg.endsWith("/")) {
                                        var regex = new RegExp(attrReg.substring(1, attrReg.length - 1));
                                        if (regex.test(attrs[key])) {
                                            translation_assistant.setTag(tags, tagKey, tagValue[attrReg], replace);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        // If necessary, convert from English TDS to OSM+
        // Raw tag, TDS/GGDM, MGCP or specified
        if (tags['Feature Code'] || tags['F_CODE::Feature Code'] || tags['FCODE::Feature Code'] || fcode) {

            // First, clean up the English names. To make it easier for the user, the short name for the attribute got
            // added to the attribute name. This needs to be removed prior to translation
            // Structure: <short name>::<long name>
            // E.g.  FSC::Flight Strip Capable
            for (var i in tags) {
                if (i.indexOf('::') > -1) {
                console.log('Replaceing:' + i + '   with:' + i.replace(/.*::/,''));
                    tags[i.replace(/.*::/,'')] = tags[i];
                    delete tags[i]
                }
            }

            // With the F_CODE value, we want to keep the short name not the longer text
            // E.g. AJ030::Holding Pen
            if (tags['Feature Code'] && tags['Feature Code'].indexOf('::') > -1) {
                tags['Feature Code'] = tags['Feature Code'].replace(/::.*/,'');
            }

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
                case 'TDSv70':
                    osmplus = etds70_osm.toOSM(tags, '', '');
                    break;
                case 'MGCP':
                    osmplus = emgcp_osm.toOSM(tags, '', '');
                    break;
            }
            if (osmplus) {
                return osmplus.attrs;
            } else {
                return null;
            }
        }
        // Don't translate feature if no attrs were translated to tags
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
