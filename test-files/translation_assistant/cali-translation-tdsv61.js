hoot.require('SchemaTools')
hoot.require('translation_assistant')

var attributeMapping = {
    "cali-test": {
        "ID": "IGNORED",
        "NAME": {
            "Geographic Name Information : Full Name": "NAME"
        },
        "NAME_ALT": "IGNORED",
        "FCC": {
            "Route Identification [route designation type]": {
                "A20": "National",
                "A40": "Local",
                "A30": "Secondary"
            },
            "Roadway Type": {
                "A20": "Road",
                "A40": "Road",
                "A30": "Road"
            }
        },
        "LEN": "IGNORED",
        "LEN_MILES": "IGNORED",
        "LEN_KM": "IGNORED",
        "SPEED_MPH": "IGNORED",
        "SPEED_KMH": "IGNORED",
        "CITY": "IGNORED",
        "COUNTRY": "IGNORED",
        "CNTRY_CODE": "IGNORED",
        "ONE_WAY": {
            "One-way": {
                "": "",
                "FT": "True",
                "TF": "True"
            }
        },
        "F_ZLEV": "IGNORED",
        "T_ZLEV": "IGNORED",
        "RAMP": "IGNORED",
        "ROUNDABOUT": "IGNORED",
        "ET_ID": "IGNORED"
    }
};
var fcode = 'AP030: Road';
var schema = 'TDSv61';

//translateToOsm - takes 'attrs' and returns OSM 'tags'
var translateToOsm = function(attrs, layerName) {
    return translation_assistant.translateAttributes(attrs, layerName, attributeMapping, fcode, schema);
};
