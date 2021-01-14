hoot.require('translation_assistant')

var attributeMapping = {
    "cali-test": {
        "CITY": "IGNORED",
        "CNTRY_CODE": "IGNORED",
        "COUNTRY": "IGNORED",
        "ET_ID": "IGNORED",
        "FCC": {
            "HCT::Thoroughfare Class": {
                "A20": "National Motorway",
                "A30": "Secondary Route",
                "A40": "Local Route"
            },
            "FCODE::Feature Code": "AP030"
        },
        "ID": "IGNORED",
        "LEN": "IGNORED",
        "LEN_KM": "IGNORED",
        "LEN_MILES": "IGNORED",
        "NAME": {
            "NAM::Name": "NAME"
        },
        "NAME_ALT": "IGNORED",
        "ONE_WAY": "IGNORED",
        "RAMP": "IGNORED",
        "ROUNDABOUT": "IGNORED",
        "SPEED_KMH": "IGNORED",
        "SPEED_MPH": "IGNORED"
    }
};
var fcode;
var schema = 'MGCP';

var translateToOsm = function(attrs, layerName) {
    return translation_assistant.translateToOsm(attrs, layerName, attributeMapping, fcode, schema);
};


