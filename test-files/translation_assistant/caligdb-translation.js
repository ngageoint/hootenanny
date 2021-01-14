hoot.require('translation_assistant')

var attributeMapping = {
    "cali_test": {
        "ID": "IGNORED",
        "NAME": {
            "name": "NAME"
        },
        "NAME_ALT": {
            "alt_name": "NAME_ALT"
        },
        "FCC": {
            "highway": {
                "A20": "primary",
                "A40": "tertiary",
                "A30": "secondary"
            }
        },
        "LEN": "IGNORED",
        "LEN_MILES": "IGNORED",
        "LEN_KM": "IGNORED",
        "SPEED_MPH": {
            "maxspeed_mph": "SPEED_MPH"
        },
        "SPEED_KMH": {
            "maxspeed": "SPEED_KMH"
        },
        "CITY": "IGNORED",
        "COUNTRY": "IGNORED",
        "CNTRY_CODE": "IGNORED",
        "ONE_WAY": {
            "oneway": {
                "": "",
                "FT": "true",
                "TF": "true"
            }
        },
        "F_ZLEV": "IGNORED",
        "T_ZLEV": "IGNORED",
        "RAMP": {
            "highway": {
                "T": "motorway_link",
                "": ""
            }
        },
        "ROUNDABOUT": {
            "highway": {
                "": "",
                "T": "turning_circle"
            }
        },
        "ET_ID": "IGNORED"
    },
    "cali_fake_points": {
        "id": "IGNORED",
        "name": {
            "name": "name"
        },
        "size": {
            "population": "size"
        }
    }
};
var fcode;
var schema;

//translateToOsm - takes 'attrs' and returns OSM 'tags'
var translateToOsm = function(attrs, layerName) {
    return translation_assistant.translateToOsm(attrs, layerName, attributeMapping, fcode, schema);
};
