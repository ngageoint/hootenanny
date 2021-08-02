hoot.require('translation_assistant')

var attributeMapping = {
  "jakarta_raya_coastline": {
    "NATURAL": {
      "natural": {
        "water": "coastline",
        "shoreline": "coastline"
      }
    }
  }
};

var fcode;
var schema;
//translateToOsm - takes 'attrs' and returns OSM 'tags'
var translateToOsm = function(attrs, layerName) {return translation_assistant.translateAttributes(attrs, layerName, attributeMapping, fcode, schema);};

