

if (!exports)
{
    exports = {};
}
schemaTools = exports;
st = schemaTools;

if (typeof hoot === 'undefined')
{
    var HOOT_HOME = process.env.HOOT_HOME;
    schemaTools.hoot = require(HOOT_HOME + '/lib/HootJs');
} else {
    schemaTools.hoot = hoot;
}

schemaTools.expandAliases = function(tags) {
    var result = [];

    for (var i in tags) {
        var t = tags[i];

        for (var j in t.aliases) {
            var newT = JSON.parse(JSON.stringify(t));
            newT.name = t.aliases[j];
            newT.key = st.splitKvp(newT.name)[0];
            newT.value = st.splitKvp(newT.name)[1];
            newT.aliases = [];

            result.push(newT);
        }

        t.aliases = [];

        result.push(t);
    }

    return result;
}

schemaTools.isSimilar = function(name, threshold, minScore, maxScore) {
    if (threshold === undefined) {
        threshold = 0.8;
    }
    if (minScore === undefined) {
        minScore = 1;
    }
    if (maxScore === undefined) {
        maxScore = minScore;
    }

    if (!st.hoot.OsmSchema.getTagVertex(name)) {
        throw new Error("Invalid tag specified in isSimilar: " + JSON.stringify(name));
    }

    return {
        "ruleType": "similarTo",
        "threshold": threshold,
        "minScore": minScore,
        "maxScore": maxScore,
        "name": name,
        "toOsmKvp": name
    };
}

/**
 * The input table is expected to be in the form:
 * [ [key1, value1, rule1], [key2, value2, rule2] ]
 * Where rules is one of the above (isA or isSimilar).
 *
 * The rules are prioritized so higher scoring rules override lower scoring rules. Duplicate records
 * will not be generated and warnings will only be produced if ogr.debug.lookupclass == true.
 *
 *
 */
schemaTools.generateRuleTags = function(rule) {
    var result = [];

    if (rule.ruleType === 'similarTo') {
        var tags = st.hoot.OsmSchema.getSimilarTags(rule.name, rule.threshold);

        tags = st.expandAliases(tags);

        for (i in tags) {
            if (tags[i].value !== '' && tags[i].value !== '*')
            {
                var schemaScore = st.hoot.OsmSchema.scoreOneWay(tags[i].name, rule.name);
                if (schemaScore >= rule.threshold)
                {
                    // scale the score so that rule.threshold -> 0, and 1 -> 1. Linearly.
                    var score = (schemaScore - rule.threshold) / (1 - rule.threshold);
                    // scale the final score so that 0 -> minScore and 1 -> maxScore
                    tags[i].score = (rule.maxScore - rule.minScore) * score + rule.minScore;
                    result.push(tags[i]);
                }
            }
        }
    } else if (rule.ruleType === 'isA') {
        var tags = schemaTools.hoot.OsmSchema.getChildTags(rule.name);
        tags = st.expandAliases(tags);

        for (i in tags) {
            if (tags[i].value !== '' && tags[i].value !== '*')
            {
                tags[i].score = rule.score;
                result.push(tags[i]);
            }
        }
    } else if (rule.ruleType === 'simple') {
        var tags = [schemaTools.hoot.OsmSchema.getTagVertex(rule.name)];
        tags = st.expandAliases(tags);

        for (i in tags) {
            tags[i].score = rule.score;
            result.push(tags[i]);
        }
    } else if (rule.ruleType === 'wildcard') {
        result = result.concat(schemaTools.getWildcardTags(rule.name, rule.score));
    } else {
        throw new Error("Unexpected rule type: " + rule.ruleType);
    }


    return result;
}

/**
 * The input table is expected to be in the form:
 * [ [key1, value1, rule1], [key2, value2, rule2] ]
 * Where rules is one of the above (isA or isSimilar).
 */
schemaTools.generateToOsmTable = function(rules) {

    // build a more efficient lookup
    var lookup = {}

    for (var r in rules)
    {
        var row = rules[r];

        if (row[2]) // Make sure it isn't 'undefined'
        {
            var toOsmKvp = schemaTools.getToOsmKvp(row);

            var key = schemaTools.splitKvp(toOsmKvp)[0];
            var value = schemaTools.splitKvp(toOsmKvp)[1];

            if (!(row[0] in lookup))
            {
                lookup[row[0]] = {}
            }

            if (lookup[row[0]][row[1]])
            {
                throw new Error('Export Table Clash: ' + row[2] + ' ' + row[3] + '  is ' +
                    lookup[row[2]][row[3]] + '  tried to change to ' + [row[0], row[1]]);
            }

            lookup[row[0]][row[1]] = [key, value];
        }
    }

    return lookup;
}

schemaTools.generateToOgrTable = function(rules) {

    // build a more efficient lookup
    var lookup = {}

    for (var r in rules)
    {
        var row = rules[r];

        if (row[2]) // Make sure it isn't 'undefined'
        {
            var osmValues = [];
            for (var i = 2; i < row.length; i++) {
                if (typeof row[i] != 'string') {
                    osmValues = osmValues.concat(schemaTools.generateRuleTags(row[i]));
                }
            }

            if (osmValues.length == 0) {
                throw new Error("Unable to create any rules from a row: " + JSON.stringify(row));
            }

            // go through all the specific rules that were generated.
            for (var i in osmValues) {
                var tag = osmValues[i];

                if (!(tag.key in lookup))
                {
                    lookup[tag.key] = {}
                }

                if (!(tag.value in lookup[tag.key]) ||
                    tag.score > lookup[tag.key][tag.value][2])
                {
                    lookup[tag.key][tag.value] = [row[0], row[1], tag.score];
                }
            }
        }
    }

    return lookup;
}

schemaTools.getToOsmKvp = function(row) {
    var result;

    if (typeof row[2] == 'string') {
        result = row[2];
        return result;
    }

    for (var i = 2; i < row.length; i++) {
        if (row[i].toOsmKvp) {
            if (!result) {
                result = row[i].toOsmKvp;
            } else if (result !== row[i].toOsmKvp) {
                throw new Error("Inconsistent implicit toOsm KVP in row: " + JSON.stringify(row));
            }
        }
    }

    if (!result) {
        throw new Error("No OSM KVP is specified. Try specifying it explicitly.");
    }

    return result;
}

schemaTools.getWildcardTags = function(kvp, score) {
    var result = [];
    var split = schemaTools.splitKvp(kvp);

    var keyRx = new RegExp(split[0]);
    var valueRx = new RegExp(split[1]);

    var allTags = schemaTools.hoot.OsmSchema.getAllTags();
    allTags = st.expandAliases(allTags);

    for (var i in allTags) {
        var tag = allTags[i];
        if (tag.key.search(keyRx) !== -1 &&
            tag.value.search(valueRx) !== -1)
        {
            if (score) {
                tag.score = score;
            }
            result.push(tag);
        }
    }

    return result;
}

schemaTools.isA = function(name, score) {
    if (score === undefined) {
        score = 0.001;
    }

    if (!st.hoot.OsmSchema.getTagVertex(name)) {
        throw new Error("Invalid tag specified in isA: " + JSON.stringify(name));
    }

    return {
        "ruleType": "isA",
        "score": score,
        "name": name,
        "toOsmKvp": name
    };
}

schemaTools.simple = function(name, score) {
    if (!score) {
        score = 2;
    }

    if (!st.hoot.OsmSchema.getTagVertex(name)) {
        throw new Error("Invalid tag specified in simple: " + JSON.stringify(name));
    }

    return {
        "ruleType": "simple",
        "score": score,
        "name": name,
        "toOsmKvp": name
    };
}

schemaTools.splitKvp = function(kvp) {
    var i = kvp.indexOf('=');

    return [kvp.substring(0, i), kvp.substring(i + 1)];
}

/**
 * @param name is a key=[regex] where regex uses the JavaScript syntax.
 */
schemaTools.wildcard = function(name, score) {
    if (score === undefined) {
        score = 1;
    }

    if (st.getWildcardTags(name).length === 0) {
        throw new Error("wildcard didn't match any tags: " + JSON.stringify(name));
    }

    return {
        "ruleType": "wildcard",
        "score": score,
        "name": name
    };
}

