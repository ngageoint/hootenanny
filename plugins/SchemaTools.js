
schemaTools = {};


schemaTools.isSimilar = function(name, threshold, score) {
    if (threshold === undefined) {
        threshold = 0.8;
    }
    if (score === undefined) {
        score = 1;
    }

    return {
        "ruleType": "similarTo",
        "threshold": threshold,
        "score": score,
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
        var tags = hoot.OsmSchema.getSimilarTags(rule.name, rule.threshold);

        for (i in tags) {
            if (tags[i].value !== '' && tags[i].value !== '*')
            {
                tags[i].score = rule.score;
                result.push(tags[i]);
            }
        }
    } else if (rule.ruleType === 'isA') {
        var tags = hoot.OsmSchema.getChildTags(rule.name);

        for (i in tags) {
            if (tags[i].value != '' && tags[i].value != '*')
            {
                tags[i].score = rule.score;
                result.push(tags[i]);
            }
        }
    } else if (rule.ruleType === 'simple') {
        var tag = hoot.OsmSchema.getTagVertex(rule.name);
        tag.score = rule.score;

        result.push(tag);
    } else if (rule.ruleType === 'wildcard') {
        var split = schemaTools.splitKvp(rule.name);

        var keyRx = new RegExp(split[0]);
        var valueRx = new RegExp(split[1]);

        var allTags = hoot.OsmSchema.getAllTags();

        for (var i in allTags) {
            var tag = allTags[i];
            if (tag.key.search(keyRx) != -1 &&
                tag.value.search(valueRx) != -1)
            {
                tag.score = rule.score;
                result.push(tag);
            }
        }
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

    for (var r in one2one)
    {
        var row = one2one[r];

        if (row[2]) // Make sure it isn't 'undefined'
        {
            var key = schemaTools.splitKvp(row[2].toOsmKvp)[0];
            var value = schemaTools.splitKvp(row[2].toOsmKvp)[1];

            if (!(row[0] in lookup))
            {
                lookup[row[0]] = {}
            }

            // This has been swapped around. The FCODE lists can stomp on the One2One values.
            if (lookup[row[0]][row[1]])
            {
                if (config.getOgrDebugLookupclash() === 'true') {
                    print('Export Table Clash: ' + row[2] + ' ' + row[3] + '  is ' +
                        lookup[row[2]][row[3]] + '  tried to change to ' + [row[0], row[1]]);
                }
            }

            lookup[row[0]][row[1]] = [key, value];
        }
    }

    return lookup;
}

schemaTools.generateToOgrTable = function(rules) {

    // build a more efficient lookup
    var lookup = {}

    for (var r in one2one)
    {
        var row = one2one[r];

        if (row[2]) // Make sure it isn't 'undefined'
        {
            var osmValues = schemaTools.generateRuleTags(row[2]);

            // go through all the specific rules that were generated.
            for (var i in osmValues) {
                var tag = osmValues[i];

                if (!(tag.key in lookup))
                {
                    lookup[tag.key] = {}
                }

                // This has been swapped around. The FCODE lists can stomp on the One2One values.
                if (lookup[tag.key][tag.value])
                {
                    if (config.getOgrDebugLookupclash() === 'true') {
                        print('Ingest Table Clash: ' + row[2] + ' ' + row[3] + '  is ' +
                            lookup[row[2]][row[3]] + '  tried to change to ' + [row[0], row[1]]);
                    }
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

schemaTools.isA = function(name, score) {
    if (score === undefined) {
        score = 0.001;
    }

    return {
        "ruleType": "isA",
        "score": score,
        "name": name,
        "toOsmKvp": name
    };
}

schemaTools.simple = function(name) {
    return {
        "ruleType": "simple",
        "score": 2,
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
schemaTools.wildcard = function(name, toOsmKvp, score) {
    if (score === undefined) {
        score = 1;
    }

    return {
        "ruleType": "wildcard",
        "score": score,
        "name": name,
        "toOsmKvp": toOsmKvp
    };
}

