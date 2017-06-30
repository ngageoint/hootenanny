
poi = hoot.require("PoiGeneric");

// poor man's inheritance. Is there a better practice in JS?
// use all the functions/parameters as defined in PoiGeneric, then we will override later.
Object.keys(poi).forEach(function(key) {
    exports[key] = poi[key];
});

exports.description = "Multiary POI Generic";

// this matcher will never cause a conflict. However, another matcher may conflict with us.
exports.neverCausesConflict = function() { return true; }

delete exports.mergePair;

exports.mergeSets = function(map, pairs, replaced)
{
    var elements = {};

    // create a dictionary that only contains each element once.
    for (var i = 0; i < pairs.length; i++)
    {
        elements[pairs[i][0].toString()] = pairs[i][0];
        elements[pairs[i][1].toString()] = pairs[i][1];
    }

    var first = null;
    Object.keys(elements).forEach(function(key) {
        if (first == null)
        {
            first = map.getElement(elements[key]);
        }
        else
        {
            mergeElements(map, first, map.getElement(elements[key]));
        }
    });
};

