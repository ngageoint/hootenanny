/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2013 DigitalGlobe (http://www.digitalglobe.com/)
 */

// if (!exports)
if (typeof exports === 'undefined')
{
    exports = {};
}
schemaTools = exports;

if (typeof hoot === 'undefined')
{
    var HOOT_HOME = process.env.HOOT_HOME;
    var hoot = require(HOOT_HOME + '/lib/HootJs');

}

schemaTools.expandAliases = function(tags) {
    var result = [];

    for (var i in tags) {
        var t = tags[i];

        for (var j in t.aliases) {
            var newT = JSON.parse(JSON.stringify(t));
            newT.name = t.aliases[j];
            newT.key = schemaTools.splitKvp(newT.name)[0];
            newT.value = schemaTools.splitKvp(newT.name)[1];
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

    if (!hoot.OsmSchema.getTagVertex(name)) {
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
        var tags = hoot.OsmSchema.getSimilarTags(rule.name, rule.threshold);

        tags = schemaTools.expandAliases(tags);

        for (i in tags) {
            if (tags[i].value !== '' && tags[i].value !== '*')
            {
                var schemaScore = hoot.OsmSchema.scoreOneWay(tags[i].name, rule.name);
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
        var tags = hoot.OsmSchema.getChildTags(rule.name);

        // Add the parents
        tags.push.apply(tags,[hoot.OsmSchema.getTagVertex(rule.name)]);

        tags = schemaTools.expandAliases(tags);

        for (i in tags) {
            if (tags[i].value !== '' && tags[i].value !== '*')
            {
                tags[i].score = rule.score;
                result.push(tags[i]);
            }
        }
    } else if (rule.ruleType === 'simple') {
        var tags = [hoot.OsmSchema.getTagVertex(rule.name)];
        tags = schemaTools.expandAliases(tags);

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
                throw new Error('Export Table Clash: ' + row[0] + ' ' + row[1] + '  is ' +
                    lookup[row[0]][row[1]] + '  tried to change to ' + [key, value]);
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

    var allTags = hoot.OsmSchema.getAllTags();
    allTags = schemaTools.expandAliases(allTags);

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

    if (!hoot.OsmSchema.getTagVertex(name)) {
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

    if (!hoot.OsmSchema.getTagVertex(name)) {
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

    if (schemaTools.getWildcardTags(name).length === 0) {
        throw new Error("wildcard didn't match any tags: " + JSON.stringify(name));
    }

    return {
        "ruleType": "wildcard",
        "score": score,
        "name": name
    };
}

