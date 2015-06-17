var assert = require("assert")
var translation  = require(__dirname + "/../js/translation-template.js");

var mapping = {
    "city": "ignored",
    "cntry_code": "ignored",
    "country": "ignored",
    "fcc": {
        "highway": {
            "A20": "primary",
            "A30": "secondary",
            "A40": "residential"
        }
    },
    "f_zlev": "ignored",
    "name": {
        "name": "name"
    },
    "name_alt": "ignored",
    "one_way": "ignored",
    "ramp": "ignored",
    "roundabout": "ignored",
    "speed_kmh": {
        "maxspeed": "speed_kmh"
    },
    "speed_mph": "ignored",
    "t_zlev": "ignored"
};

var mappingWithDuplicateTags = {
                "city": "ignored",
                "cntry_code": "ignored",
                "country": "ignored",
                "fcc": {
                    "highway": {
                        "A20": "primary",
                        "A30": "secondary",
                        "A40": "residential"
                    }
                },
                "f_zlev": "ignored",
                "name": {
                    "name": "name"
                },
                "name_alt": "ignored",
                "one_way": "ignored",
                "ramp": {
                    "highway": {
                        "T": "motorway_link",
                        "": ""
                    }
                },
                "roundabout": "ignored",
                "speed_kmh": {
                    "maxspeed": "speed_kmh"
                },
                "speed_mph": "ignored",
                "t_zlev": "ignored"
            };

var mappingManyToOne = {
                "city": "ignored",
                "cntry_code": "ignored",
                "country": "ignored",
                "fcc": {
                    "highway": {
                        "A20": "primary",
                        "A30": "secondary",
                        "A40": "residential"
                    }
                },
                "f_zlev": "ignored",
                "name": {
                    "name": "name"
                },
                "name_alt": {
                    "extra_attributes": "name_alt"
                },
                "one_way": {
                    "extra_attributes": "one_way"
                },
                "ramp": {
                    "highway": {
                        "T": "motorway_link",
                        "": ""
                    }
                },
                "roundabout": {
                    "extra_attributes": "roundabout"
                },
                "speed_kmh": {
                    "maxspeed": "speed_kmh"
                },
                "speed_mph": "ignored",
                "t_zlev": "ignored"
            };


var attrs = {
    "CITY": "CALI",
    "CNTRY_CODE": "CO",
    "COUNTRY": "COLOMBIA",
    "FCC": "A30",
    "F_ZLEV": "0",
    "NAME": "CL 10A",
    "NAME_ALT": "CL 10A",
    "ONE_WAY": "TF",
    "RAMP": "T",
    "ROUNDABOUT": "T",
    "SPEED_KMH": "56",
    "SPEED_MPH": "35",
    "T_ZLEV": "0"
};

var attrsNoRamp = {
    "CITY": "CALI",
    "CNTRY_CODE": "CO",
    "COUNTRY": "COLOMBIA",
    "FCC": "A20",
    "F_ZLEV": "0",
    "NAME": "CL 10A",
    "NAME_ALT": "CL 10A",
    "ONE_WAY": "TF",
    "RAMP": "",
    "ROUNDABOUT": "",
    "SPEED_KMH": "56",
    "SPEED_MPH": "35",
    "T_ZLEV": "0"
};

var layerName = "test";

var tags = {
    highway: "secondary",
    name: "CL 10A",
    maxspeed: "56"
};

var tagsWithDuplicateKeys = {
    highway: "motorway_link",
    name: "CL 10A",
    maxspeed: "56"
};

var tagsWithEmptyValues = {
    highway: "primary",
    name: "CL 10A",
    maxspeed: "56"
};

var tagsWithManyToOne = {
    highway: "motorway_link",
    name: "CL 10A",
    maxspeed: "56",
    extra_attributes: "name_alt=CL 10A;one_way=TF;roundabout=T"
};

var ogrAttrs = {
    "fcc": "A30",
    "name": "CL 10A",
    "speed_kmh": "56"
};

var invertedMapping = {
    "highway": {
        "fcc": {
            "primary": "A20",
            "secondary": "A30",
            "residential": "A40"
        }
    },
    "name": {
        "name": "name"
    },
    "maxspeed": {
        "speed_kmh": "maxspeed"
    }};

describe('translateToOsm', function(){
    it('should translate shapefile attributes to osm tags', function(){
        assert.equal(JSON.stringify(translation.translateToOsm(attrs, layerName, mapping)), JSON.stringify(tags));
    })
    it('for duplicate tag keys, last one wins', function(){
        assert.equal(JSON.stringify(translation.translateToOsm(attrs, layerName, mappingWithDuplicateTags)), JSON.stringify(tagsWithDuplicateKeys));
    })
    it('tags with empty values are ignored', function(){
        assert.equal(JSON.stringify(translation.translateToOsm(attrsNoRamp, layerName, mappingWithDuplicateTags)), JSON.stringify(tagsWithEmptyValues));
    })
    it('map many-to-one fields to extra_attributes', function(){
        assert.equal(JSON.stringify(translation.translateToOsm(attrs, layerName, mappingManyToOne)), JSON.stringify(tagsWithManyToOne));
    })
})

describe('translateToOgr', function(){
    it('should translate osm tags to shapefile attributes', function(){
        assert.equal(JSON.stringify(translation.translateToOgr(tags, layerName, mapping)), JSON.stringify(ogrAttrs));
    })
})

describe('invert', function(){
    it('should invert the attribute mapping json', function(){
        assert.equal(JSON.stringify(translation.invert(mapping)), JSON.stringify(invertedMapping));
    })
})
