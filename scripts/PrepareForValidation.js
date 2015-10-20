#!/usr/bin/node
/**
 * This prepares data for validation via imagery by applying review tags.
 */
var hoot = require('../lib/HootJs');

var input = process.argv[2]
var output = process.argv[3]
var poiCount = 0;

// visitor for counting the number of POIs in a map
function countPois(e) {
    if (hoot.OsmSchema.isPoi(e)) {
        poiCount++;
    }
}

// creates a randomly ordered array of indexes
function createRandomArray(count, subsamplePercent) {
    // create an array of ascending numbers of count size
    var result = new Array(count);
    for (i = 0; i < count; i++) {
        result[i] = i;
    }

    // randomize the array
    for (var i = result.length - 1; i > 0; i--) {
        var j = Math.floor(Math.random() * (i + 1));
        var temp = result[i];
        result[i] = result[j];
        result[j] = temp;
    }

    // crop the array to subsample size
    return result.slice(0, Math.ceil(count * subsamplePercent));
}

function addReviewTags(e) {
    // this creates a copy of the tags so we have to set it when we're done
    var tags = e.getTags();

    // make sure there is a UUID
    tags.getCreateUuid();
    tags.set("hoot:review:needs", "yes");
    tags.set("hoot:review:note", "Flagged for imagery validation");
    tags.set("hoot:review:choices:1", JSON.stringify({
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
    tags.set("hoot:review:choices:2", JSON.stringify({
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
    tags.set("hoot:review:choices:3", JSON.stringify({
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

    //console.log(JSON.stringify(tags.toDict(), null, 2));
    //console.log("## hoot:review:choices:1 ##");
    //console.log(JSON.stringify(JSON.parse(tags.toDict()["hoot:review:choices:1"]), null, 2));
    //console.log("## hoot:review:choices:2 ##");
    //console.log(JSON.stringify(JSON.parse(tags.toDict()["hoot:review:choices:2"]), null, 2));
    //console.log("## hoot:review:choices:3 ##");
    //console.log(JSON.stringify(JSON.parse(tags.toDict()["hoot:review:choices:3"]), null, 2));

    e.setTags(tags);
}

// create a new map and populate it with a input file
var map = new hoot.OsmMap();
hoot.loadMap(map, input, false, 1);

// count the number of POIs in the map
map.visit(countPois);

// Create an array of indexes for the random 30% sample
var randomIndexes = createRandomArray(poiCount, 0.30).sort(function(a,b) { return a - b; });
randomIndexes[randomIndexes.length] = poiCount;

// go through all the elements
var index = 0;
var i = 0;
map.visit(function(e) {
    if (hoot.OsmSchema.isPoi(e)) {
        // if this is an element flagged for validation
        if (index == randomIndexes[i]) {
            i++;
            // add validation review tags.
            addReviewTags(e);
        } else if (index > randomIndexes[i]) {
            throw "Logic error: Expected the index to be <= randomIndexes[i]";
        }
        index++;
    }
});

hoot.saveMap(map, output);
