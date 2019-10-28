hoot.require('SchemaTools')
hoot.require('translation_assistant')

var attributeMapping = {
    "cali-test": {
        "CITY": "IGNORED",
        "CNTRY_CODE": {
            "ZI020_GE4::(Location Country) Designation : GENC Short URN-based Identifier": "COLOMBIA AS OF 2013-06-30"
        },
        "COUNTRY": "IGNORED",
        "ET_ID": "IGNORED",
        "FCC": {
            "RIN_ROI::Route Identification [route designation type]": {
                "A20": "National",
                "A30": "Secondary",
                "A40": "Local"
            },
            "RTY::Roadway Type": {
                "A20": "Road",
                "A30": "Road",
                "A40": "Road"
            },
            "F_CODE::Feature Code": "AP030::Road"
        },
        "ID": "IGNORED",
        "LEN": "IGNORED",
        "LEN_KM": "IGNORED",
        "LEN_MILES": "IGNORED",
        "NAME": {
            "ZI005_FNA::Geographic Name Information : Full Name": "NAME"
        },
        "NAME_ALT": "IGNORED",
        "ONE_WAY": {
            "ONE::One-way": {
                "FT": "True"
            }
        },
        "RAMP": "IGNORED",
        "ROUNDABOUT": "IGNORED",
        "SPEED_KMH": "IGNORED",
        "SPEED_MPH": "IGNORED"
    }
};
var fcode;
var schema = 'TDSv70';

var translateToOsm = function(attrs, layerName) {
    return translation_assistant.translateAttributes(attrs, layerName, attributeMapping, fcode, schema);
};
