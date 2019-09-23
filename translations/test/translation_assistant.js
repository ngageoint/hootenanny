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
    name: "CL 10A",
    maxspeed: "56"
};

var tagsWithEmptyValues = {
    highway: "primary",
    name: "CL 10A",
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

var attrsRegex = {
    ID: "12345",
    NAME: "Whole Foods",
    TYPE: "grocery"
};
var tagsRegex = {
    name: "Whole Foods",
    shop: "supermarket"
};
var attrsRegex2 = {
    ID: "789",
    NAME: "Big Lots",
    TYPE: "Commercial"
};
var tagsRegex2 = {
    name: "Big Lots",
    building: "commercial"
};
var mappingRegex = {
    "Regex": {
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


var mappingMultiRule = {
    "Building_Polygons": {
        "COUNTYOFFI": {
            "building": {
                "No": "yes",
                "Yes": "government"
            },
            "foo": "bar"
        },
        "FIRESTATIO": {
            "amenity": {
                "No": "",
                "Yes": "fire_station"
            },
            "foo": "biz"
        },
        "LIBRARY": {
            "amenity": {
                "No": "",
                "Yes": "library"
            },
            "foo": "bar"
        },
        "OBJECTID": {
            "building": "yes"
        },
        "SCHOOL": {
            "building": {
                "No": "yes",
                "Yes": "school"
            }
        }
    }
};
var attrsMultiRule = {
    OBJECTID: "12345",
    COUNTYOFFI: "No",
    FIRESTATIO: "No",
    SCHOOL: "Yes",
    LIBRARY: "No"
};
var tagsMultiRule = {
    building: "school",
    foo: "bar;biz"
};
var attrsMultiRule2 = {
    OBJECTID: "1234",
    COUNTYOFFI: "No",
    FIRESTATIO: "Yes",
    SCHOOL: "No",
    LIBRARY: "No"
};
var tagsMultiRule2 = {
    building: "yes",
    foo: "bar;biz",
    amenity: "fire_station"
};

var attrsMultiRule3 = {
    OBJECTID: "1234",
    COUNTYOFFI: "No",
    FIRESTATIO: "No",
    SCHOOL: "No",
    LIBRARY: "Yes"
};
var tagsMultiRule3 = {
    building: "yes",
    foo: "bar;biz",
    amenity: "library"
};


var mappingAppend = {
    "Trails": {
        "LENGTH": {
            "highway": "footway"
        },
        "IMAGERY": {
            "source": {
                "false": "",
                "true": "Maxar"
            }
        },
        "SURVEY": {
            "source": {
                "false": "",
                "true": "GPS"
            }
        }
    }
};
var attrsAppend = {
    LENGTH: "100",
    IMAGERY: "true",
    SURVEY: "true"
};
var tagsAppend = {
    highway: "footway",
    source: "Maxar;GPS"
};
var tagsAppend2 = {
    highway: "footway",
    source: "GPS"
};

describe('translateAttributes', function() {
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
    it('allows mapping to regex tag keys', function(){
        assert.equal(JSON.stringify(translation.translateAttributes(attrsRegex, layerName, mappingRegex)), JSON.stringify(tagsRegex));
        assert.equal(JSON.stringify(translation.translateAttributes(attrsRegex2, layerName, mappingRegex)), JSON.stringify(tagsRegex2));
    })
    it('overwrites "yes" keys with more specific values', function(){
        assert.equal(JSON.stringify(translation.translateAttributes(attrsMultiRule, layerName, mappingMultiRule)), JSON.stringify(tagsMultiRule));
        assert.equal(JSON.stringify(translation.translateAttributes(attrsMultiRule2, layerName, mappingMultiRule)), JSON.stringify(tagsMultiRule2));
        assert.equal(JSON.stringify(translation.translateAttributes(attrsMultiRule3, layerName, mappingMultiRule)), JSON.stringify(tagsMultiRule3));
    })
    it('will append multiple tag values', function(){
        assert.equal(JSON.stringify(translation.translateAttributes(attrsAppend, layerName, mappingAppend)), JSON.stringify(tagsAppend));
    })
    it('will replace instead of append when told', function(){
        assert.equal(JSON.stringify(translation.translateAttributes(attrsAppend, layerName, mappingAppend, null, null, true)), JSON.stringify(tagsAppend2));
    })
})

var a = [1, 2, 'a', 'b'],
    b = [2, 'b', 'c'],
    d = [1, 'a'];

describe('difference', function() {
    it('should return elements in a not in b', function() {
        assert.deepEqual(translation.difference(a, b), d);
    })
})
