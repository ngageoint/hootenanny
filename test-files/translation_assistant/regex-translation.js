hoot.require('translation_assistant')

var attributeMapping = {
    "RegexTranslation": {
        "ID": "IGNORED",
        "NAME": {
            "name": "NAME"
        },
        "TYPE": {
            "building": {
                "/.*[Cc]ommercial.*/": "commercial",
                "/.*store.*/": "yes"
            },
            "shop": {
                "/.*general.*/": "general",
                "/.*grocery.*/": "supermarket"
            }
        }
    }
};

var fcode;
var schema;
var replace = true;
//translateToOsm - takes 'attrs' and returns OSM 'tags'
var translateToOsm = function(attrs, layerName) {
    return translation_assistant.translateAttributes(attrs, layerName, attributeMapping, fcode, schema, replace);
};
