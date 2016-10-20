var assert = require("assert")
var translation  = require("../translation_assistant.js");

var mapping = {"test": {
    "CITY": "IGNORED",
    "CNTRY_CODE": "IGNORED",
    "COUNTRY": "IGNORED",
    "FCC": {
        "highway": {
            "A20": "primary",
            "A30": "secondary",
            "A40": "residential"
        }
    },
    "F_ZLEV": "IGNORED",
    "NAME": {
        "name": "NAME"
    },
    "NAME_ALT": "IGNORED",
    "ONE_WAY": "IGNORED",
    "RAMP": "IGNORED",
    "ROUNDABOUT": "IGNORED",
    "SPEED_KMH": {
        "maxspeed": "SPEED_KMH"
    },
    "SPEED_MPH": "IGNORED",
    "T_ZLEV": "IGNORED"
}};

var mappingWithDuplicateTags = {"test": {
                "CITY": "IGNORED",
                "CNTRY_CODE": "IGNORED",
                "COUNTRY": "IGNORED",
                "FCC": {
                    "highway": {
                        "A20": "primary",
                        "A30": "secondary",
                        "A40": "residential"
                    }
                },
                "F_ZLEV": "IGNORED",
                "NAME": {
                    "name": "NAME"
                },
                "NAME_ALT": {
                    "name": "NAME_ALT"
                },
                "ONE_WAY": "IGNORED",
                "RAMP": {
                    "highway": {
                        "T": "motorway_link",
                        "": ""
                    }
                },
                "ROUNDABOUT": "IGNORED",
                "SPEED_KMH": {
                    "maxspeed": "SPEED_KMH"
                },
                "SPEED_MPH": "IGNORED",
                "T_ZLEV": "IGNORED"
            }};

var mappingManyToOne = {"test": {
    "CITY": "IGNORED",
    "CNTRY_CODE": "IGNORED",
    "COUNTRY": "IGNORED",
    "FCC": {
        "highway": {
            "A20": "primary",
            "A30": "secondary",
            "A40": "residential"
        }
    },
    "F_ZLEV": "IGNORED",
    "NAME": {
        "name": "NAME"
    },
    "NAME_ALT": {
        "note": "NAME_ALT"
    },
    "ONE_WAY": {
        "note": "ONE_WAY"
    },
    "RAMP": {
        "highway": {
            "T": "motorway_link",
            "": ""
        }
    },
    "ROUNDABOUT": {
        "note": "ROUNDABOUT"
    },
    "SPEED_KMH": {
        "maxspeed": "SPEED_KMH"
    },
    "SPEED_MPH": "IGNORED",
    "T_ZLEV": "IGNORED"
}};

var mappingManyToOneTds = {"test": {
    "CITY": "IGNORED",
    "CNTRY_CODE": "IGNORED",
    "COUNTRY": "IGNORED",
    "FCC": {
        "highway": {
            "A20": "primary",
            "A30": "secondary",
            "A40": "residential"
        }
    },
    "F_ZLEV": "IGNORED",
    "NAME": {
        "name": "NAME"
    },
    "NAME_ALT": {
        "Note : Memorandum": "NAME_ALT"
    },
    "ONE_WAY": {
        "Note : Memorandum": "ONE_WAY"
    },
    "RAMP": {
        "highway": {
            "T": "motorway_link",
            "": ""
        }
    },
    "ROUNDABOUT": {
        "Note : Memorandum": "ROUNDABOUT"
    },
    "SPEED_KMH": {
        "maxspeed": "SPEED_KMH"
    },
    "SPEED_MPH": "IGNORED",
    "T_ZLEV": "IGNORED"
}};

var mappingManyToOneMerge = {"test": {
    "id": "IGNORED",
    "paved": {
        "surface": {
            "0": "",
            "1": "asphalt",
            "2": "concrete"
        }
    },
    "dirt": {
        "surface": {
            "0": "",
            "1": "dirt"
        }
    }
}};

var mappingMultipleLayers = {"test": {
    "CITY": "IGNORED",
    "CNTRY_CODE": "IGNORED",
    "COUNTRY": "IGNORED",
    "FCC": {
        "highway": {
            "A20": "primary",
            "A30": "secondary",
            "A40": "residential"
        }
    },
    "F_ZLEV": "IGNORED",
    "NAME": {
        "name": "NAME"
    },
    "NAME_ALT": "IGNORED",
    "ONE_WAY": "IGNORED",
    "RAMP": "IGNORED",
    "ROUNDABOUT": "IGNORED",
    "SPEED_KMH": {
        "maxspeed": "SPEED_KMH"
    },
    "SPEED_MPH": "IGNORED",
    "T_ZLEV": "IGNORED"
},
"layer2": {
    "id": "IGNORED",
    "name": {
        "name": "name"
    },
    "size": {
        "population": "size"
    }
}};

var mappingStatic = {"test": {
    "CITY": "IGNORED",
    "CNTRY_CODE": "IGNORED",
    "COUNTRY": "IGNORED",
    "FCC": {
        "highway": {
            "A20": "primary",
            "A30": "secondary",
            "A40": "residential"
        },
        "source": "DigitalGlobe Imagery"
    },
    "F_ZLEV": "IGNORED",
    "NAME": {
        "name": "NAME"
    },
    "NAME_ALT": "IGNORED",
    "ONE_WAY": "IGNORED",
    "RAMP": "IGNORED",
    "ROUNDABOUT": "IGNORED",
    "SPEED_KMH": {
        "maxspeed": "SPEED_KMH"
    },
    "SPEED_MPH": "IGNORED",
    "T_ZLEV": "IGNORED"
}};

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

var attrsNoMatch = {
    "FOO": "BAR",
    "BAZ": "0"
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

var attrsMissingNull = {
    "CITY": "CALI",
    "CNTRY_CODE": "CO",
    "COUNTRY": "COLOMBIA",
    "FCC": "A30",
    "NAME": "CL 10A",
    "NAME_ALT": "CL 10A",
    "ONE_WAY": "TF",
    "RAMP": "T",
    "ROUNDABOUT": "T",
    "SPEED_KMH": "56",
    "SPEED_MPH": "35",
};

var attrsLayer2 = {
     "id": "1",
     "name": "foo",
     "size": "10"
};

var attrsManyToOneMerge1 = {
                "id": "1",
                "paved": "0",
                "dirt": "1"
               };

var attrsManyToOneMerge2 = {
                "id": "1",
                "paved": "1",
                "dirt": "0"
               };

var attrsManyToOneMerge3 = {
                "id": "1",
                "paved": "2",
                "dirt": "0"
               };

var layerName = "test";

var tags = {
    highway: "secondary",
    name: "CL 10A",
    maxspeed: "56"
};

var tagsWithDuplicateKeys = {
    highway: "secondary;motorway_link",
    name: "CL 10A;CL 10A",
    maxspeed: "56"
};

var tagsWithEmptyValues = {
    highway: "primary",
    name: "CL 10A;CL 10A",
    maxspeed: "56"
};

var tagsWithManyToOne = {
                highway: "secondary;motorway_link",
                name: "CL 10A",
                note: "NAME_ALT=CL 10A;ONE_WAY=TF;ROUNDABOUT=T",
                maxspeed: "56"
            };

var tagsWithManyToOneTds = {
                highway: "secondary;motorway_link",
                name: "CL 10A",
                "Note : Memorandum": "NAME_ALT=CL 10A;ONE_WAY=TF;ROUNDABOUT=T",
                maxspeed: "56"
            };

var tagsLayer2 = {
    name: "foo",
    population: "10"
};

var tagsWithManyToOneMerge1 = {
                surface: "dirt"
            };
var tagsWithManyToOneMerge2 = {
                surface: "asphalt"
            };
var tagsWithManyToOneMerge3 = {
                surface: "concrete"
            };

var tagsStatic = {
                highway: "secondary",
                source: "DigitalGlobe Imagery",
                name: "CL 10A",
                maxspeed: "56"
            };

describe('translateAttributes', function(){
    it('should translate shapefile attributes to osm tags', function(){
        assert.equal(JSON.stringify(translation.translateAttributes(attrs, layerName, mapping)), JSON.stringify(tags));
    })
    it('should return null if no attributes could be translated to tags', function(){
        assert.equal(translation.translateAttributes(attrsNoMatch, layerName, mapping), null);
    })
    it('should return null if no layer name matched and no attributes could be translated to tags', function(){
        assert.equal(translation.translateAttributes(attrsNoMatch, 'foo', mapping), null);
    })
    it('for duplicate tag keys, values should be appended', function(){
        assert.equal(JSON.stringify(translation.translateAttributes(attrs, layerName, mappingWithDuplicateTags)), JSON.stringify(tagsWithDuplicateKeys));
    })
    it('tag keys with empty values are ignored', function(){
        assert.equal(JSON.stringify(translation.translateAttributes(attrsNoRamp, layerName, mappingWithDuplicateTags)), JSON.stringify(tagsWithEmptyValues));
    })
    it('note and Note : Memorandum tags concatenate extra attributes', function(){
        assert.equal(JSON.stringify(translation.translateAttributes(attrs, layerName, mappingManyToOne)), JSON.stringify(tagsWithManyToOne));
        assert.equal(JSON.stringify(translation.translateAttributes(attrs, layerName, mappingManyToOneTds)), JSON.stringify(tagsWithManyToOneTds));
    })
    it('empty string in duplicate tag mapping allows merge', function(){
        assert.equal(JSON.stringify(translation.translateAttributes(attrsManyToOneMerge1, layerName, mappingManyToOneMerge)), JSON.stringify(tagsWithManyToOneMerge1));
        assert.equal(JSON.stringify(translation.translateAttributes(attrsManyToOneMerge2, layerName, mappingManyToOneMerge)), JSON.stringify(tagsWithManyToOneMerge2));
        assert.equal(JSON.stringify(translation.translateAttributes(attrsManyToOneMerge3, layerName, mappingManyToOneMerge)), JSON.stringify(tagsWithManyToOneMerge3));
    })
    it('multiple layer mappings are supported', function(){
        assert.equal(JSON.stringify(translation.translateAttributes(attrs, layerName, mappingMultipleLayers)), JSON.stringify(tags));
        assert.equal(JSON.stringify(translation.translateAttributes(attrsLayer2, "layer2", mappingMultipleLayers)), JSON.stringify(tagsLayer2));
    })
    it('allows mapping to static tags', function(){
        assert.equal(JSON.stringify(translation.translateAttributes(attrs, layerName, mappingStatic)), JSON.stringify(tagsStatic));
    })
    it('compares mapping columns for match if layername missing', function(){
        assert.equal(JSON.stringify(translation.translateAttributes(attrsMissingNull, "foo", mappingMultipleLayers)), JSON.stringify(tags));
        assert.equal(JSON.stringify(translation.translateAttributes(attrsLayer2, "bar", mappingMultipleLayers)), JSON.stringify(tagsLayer2));
    })
})

var a = [1, 2, 'a', 'b'],
    b = [2, 'b', 'c'],
    d = [1, 'a'];

describe('difference', function(){
    it('should return elements in a not in b', function(){
        assert.deepEqual(translation.difference(a, b), d);
    })
})