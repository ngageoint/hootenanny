#!/usr/bin/node
/**
 * This prepares data for validation via imagery by applying review tags.
 */
//var hoot = require('../lib/HootJs');
var HOOT_HOME = process.env.HOOT_HOME
var hoot = require(HOOT_HOME + '/lib/HootJs');

hoot.Settings.set({"api.db.email":"test@test.com"});

var input = process.argv[2]
var output = process.argv[3]
var poiCount = 0;
var isRandom = true;

if (process.argv.length == 5) {
    isRandom = process.argv[4] == "true" ? true : false
}

// visitor for counting the number of POIs in a map
function countPois(e) {
    if (hoot.OsmSchema.isPoi(e)) {
        poiCount++;
    }
}

// creates a sub sample ordered array of indexes, if isRandom is true, the indexes are random.
function createSubSampleArray(count, subsamplePercent, isRandom) {
    // create an array of ascending numbers of count size
    var result = new Array(count);
    for (i = 0; i < count; i++) {
        result[i] = i;
    }

    if (isRandom == true) {
        // randomize the array
        for (var i = result.length - 1; i > 0; i--) {
            var j = Math.floor(Math.random() * (i + 1));
            var temp = result[i];
            result[i] = result[j];
            result[j] = temp;
        }
    }

    // crop the array to subsample size
    return result.slice(0, Math.ceil(count * subsamplePercent));
}

var getListChoices = function() {
    var list = [];
    list.push(JSON.stringify({
        "label": "Confirmed",
        "description": "You can look at the point and tell what it is (e.g. mosque or airport)",
        "changes": {
            "replaceTags": {
                "hgis:imagery_confirmed": "confirmed",
                "hgis:accuracy": "high"
            },
            "appendTags": {
                // e.g. digitalglobe
                "source:geometry": "${BASEMAP_IMAGE_SOURCE}",
                // use name of sensor E.g. WV02
                "source:geometry:sensor": "${BASEMAP_IMAGE_SENSOR}",
                // use ISO date E.g. 2012-03-28 11:22:29
                "source:geometry:date": "${BASEMAP_IMAGE_DATETIME}",
                // The associated image identifier e.g. 32905903099a73faec6d7de72b9a2bdb
                "source:geometry:id": "${BASEMAP_IMAGE_ID}"
            }
        }
    }));
    list.push(JSON.stringify({
        "label": "Assessed",
        "description": "The point is on a building, but you can't verify its type (e.g. hair salon).",
        "changes": {
            "replaceTags": {
                "hgis:imagery_confirmed": "assessed",
                "hgis:accuracy": "high"
            },
            "appendTags": {
                // e.g. digitalglobe
                "source:geometry": "${BASEMAP_IMAGE_SOURCE}",
                // use name of sensor E.g. WV02
                "source:geometry:sensor": "${BASEMAP_IMAGE_SENSOR}",
                // use ISO date E.g. 2012-03-28 11:22:29
                "source:geometry:date": "${BASEMAP_IMAGE_DATETIME}",
                // The associated image identifier e.g. 32905903099a73faec6d7de72b9a2bdb
                "source:geometry:id": "${BASEMAP_IMAGE_ID}"
            }
        }
    }));
    list.push(JSON.stringify({
        "label": "Reported",
        "description": "Imagery is pixelated or cloudy -- can not be assessed.",
        "changes": {
            "replaceTags": {
                "hgis:imagery_confirmed": "reported"
            },
            "appendTags": {
                // e.g. digitalglobe
                "source:geometry": "${BASEMAP_IMAGE_SOURCE}",
                // use name of sensor E.g. WV02
                "source:geometry:sensor": "${BASEMAP_IMAGE_SENSOR}",
                // use ISO date E.g. 2012-03-28 11:22:29
                "source:geometry:date": "${BASEMAP_IMAGE_DATETIME}",
                // The associated image identifier e.g. 32905903099a73faec6d7de72b9a2bdb
                "source:geometry:id": "${BASEMAP_IMAGE_ID}"
            }
        }
    }));
    return list;
}

// create a new map and populate it with a input file
var map = new hoot.OsmMap();
hoot.loadMap(map, input, false, 1);

// count the number of POIs in the map
map.visit(countPois);

// Create an array of indexes for the random 30% sample
var randomIndexes = createSubSampleArray(poiCount, 0.30, isRandom).sort(function(a,b) { return a - b; });
randomIndexes[randomIndexes.length] = poiCount;

// go through all the elements
var index = 0;
var i = 0;
var choices = getListChoices();
map.visit(function(e) {
    if (hoot.OsmSchema.isPoi(e)) {
        // if this is an element flagged for validation
        if (index == randomIndexes[i]) {
            i++;
            hoot.ReviewMarker.mark(map, e, "Flagged for imagery validation", "Validation", 1, choices);
        } else if (index > randomIndexes[i]) {
            throw "Logic error: Expected the index to be <= randomIndexes[i]";
        }
        index++;
    }
});

hoot.saveMap(map, output);
