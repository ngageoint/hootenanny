var ogrToOsmMapping;

function translateAttributes(attrs, layerName, attributeMapping)
{
    var tags = {};
    var mapping = attributeMapping || ogrToOsmMapping;
    var extras = []

    for (var key in attrs)
    {
        var k = key.toLowerCase();
        var m = mapping[k];
        if (m && m != "ignored") { //attribute does map to a tag
            for (var tagKey in m) {
                var tagValue = m[tagKey];
                if (tagKey == "extra_attributes") { //tag is list of extra attributes att1=val1;att2=val2;att3=val3
                    extras.push(k + "=" + attrs[key]);
                } else if (tagValue == k) { //tag is set to attribute value
                    tags[tagKey] = attrs[key];
                } else { //attribute value is mapped to a tag value (if value is not empty string)
                    if (attrs[key].length > 0) tags[tagKey] = tagValue[attrs[key]];
                }
            }
        }
    }

    if (extras.length) tags.extra_attributes = extras.join(';');

    return tags;
}

function invert(attributeMapping) {
    var inverse = {};
    for (var key in attributeMapping)
    {
        var k = key.toLowerCase();
        var m = attributeMapping[k];
        if (m && m != "ignored") { //attribute does map to a tag
            for (var tagKey in m) {
                var tagValue = m[tagKey];
                if (tagValue == k) { //tag is set to attribute value
                    var map = {};
                    map[tagValue] = tagKey;
                    inverse[tagKey] = map;
                } else { //attribute value is mapped to a tag value (if value is not empty string)
                    if (tagValue !== '') {
                        var valueMap = {};
                        for (var attrKey in tagValue) {
                            valueMap[tagValue[attrKey]] = attrKey;
                        }
                        inverse[tagKey] = {};
                        inverse[tagKey][k] = valueMap;
                    }
                }
            }
        }
    }

    return inverse;

}

//translateToOsm - takes 'attrs' and returns OSM 'tags'
var translateToOsm = translateAttributes;

//translateToOgr - takes OSM 'tags' and returns 'attrs'
var translateToOgr = function(tags, layerName, attributeMapping) {
    var mapping = attributeMapping || ogrToOsmMapping;
    return translateAttributes(tags, layerName, invert(mapping));
};

if (typeof exports !== 'undefined') {
    exports.invert = invert;
    exports.translateToOsm = translateToOsm;
    exports.translateToOgr = translateToOgr;
}
