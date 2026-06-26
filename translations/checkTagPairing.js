#!/usr/bin/env node

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
 */

//
// Quick single tag-pair translation probe.
//
// This intentionally calls the translation module directly instead of going
// through TranslationServer/checkTranslations.js, since those paths can flatten
// a multi-feature translateToOgr result down to the first feature.
//

var HOOT_HOME = process.env.HOOT_HOME;

function usage()
{
  console.log([
    'Usage:',
    '  node translations/checkTagPairing.js <translation> [geometry] <key=value> [key=value ...]',
    '',
    'Examples:',
    '  node translations/checkTagPairing.js TDSv71 highway=primary bridge=yes',
    '  node translations/checkTagPairing.js TDSv71 Line highway=primary bridge=yes',
    '  node translations/checkTagPairing.js MGCP highway=primary bridge=yes',
    '  node translations/checkTagPairing.js TDSv71 natural=water',
    '',
    'If geometry is omitted, Point, Line, and Area are tested.',
    '',
    'Options:',
    '  --element=<Node|Way|Relation>       Override inferred element type',
    '  --thematic=<true|false>             Set writer.thematic.structure',
    '  --coded-values=<true|false>         Set ogr.coded.values',
    '  --round-trip                        Show all features from the final OGR pass'
  ].join('\n'));
}

function setDefaults(options)
{
  hoot.Settings.set({'ogr.add.uuid': 'false'});
  hoot.Settings.set({'ogr.clean.export': 'false'});
  hoot.Settings.set({'ogr.compare.output': 'false'});
  hoot.Settings.set({'ogr.debug.dumptags': 'false'});
  hoot.Settings.set({'ogr.esri.fcsubtype': 'false'});
  hoot.Settings.set({'ogr.note.extra': 'attribute'});
  hoot.Settings.set({'ogr.output.format': 'json'});
  hoot.Settings.set({'ogr.text.field.number': '4'});
  hoot.Settings.set({'ogr.throw.error': 'false'});
  hoot.Settings.set({'reader.drop.defaults': 'true'});
  hoot.Settings.set({'writer.thematic.structure': options.thematic || 'false'});
  hoot.Settings.set({'ogr.coded.values': options.codedValues || 'true'});
}

function loadTranslation(name)
{
  var key = name.toLowerCase();

  if (key === 'mgcp')
  {
    hoot.require('mgcp');
    hoot.require('mgcp_schema');
    hoot.require('mgcp_rules');
    hoot.require('fcode_common');
    hoot.require('translate');
    return {name: 'MGCP', module: mgcp};
  }

  var tdsVersion = key.match(/^tdsv?([0-9]+)$/);
  if (tdsVersion)
  {
    var base = 'tds' + tdsVersion[1];
    hoot.require('SchemaTools');
    hoot.require(base);
    hoot.require(base + '_schema');
    hoot.require(base + '_rules');
    hoot.require('translate');
    hoot.require('fcode_common');
    return {name: 'TDSv' + tdsVersion[1], module: global[base] || eval(base)};
  }

  throw new Error('Unsupported translation for this helper: ' + name);
}

function parseArgs(argv)
{
  var options = {};
  var positional = [];
  var defaultGeometries = ['Point', 'Line', 'Area'];

  for (var i = 0; i < argv.length; i++)
  {
    var arg = argv[i];
    if (arg === '--help' || arg === '-h')
    {
      usage();
      process.exit(0);
    }
    else if (arg === '--round-trip')
    {
      options.roundTrip = true;
    }
    else if (arg.indexOf('--element=') === 0)
    {
      options.element = arg.substring('--element='.length);
    }
    else if (arg.indexOf('--thematic=') === 0)
    {
      options.thematic = arg.substring('--thematic='.length);
    }
    else if (arg.indexOf('--coded-values=') === 0)
    {
      options.codedValues = arg.substring('--coded-values='.length);
    }
    else
    {
      positional.push(arg);
    }
  }

  if (positional.length < 2)
  {
    usage();
    process.exit(1);
  }

  var geometries = defaultGeometries;
  var firstTagIndex = 1;
  var maybeGeometry = positional[1];
  if (maybeGeometry.indexOf('=') === -1)
  {
    geometries = [maybeGeometry];
    firstTagIndex = 2;
  }

  if (positional.length <= firstTagIndex)
  {
    usage();
    process.exit(1);
  }

  var tags = {};
  for (var j = firstTagIndex; j < positional.length; j++)
  {
    var pair = positional[j];
    var split = pair.indexOf('=');
    if (split < 1)
    {
      throw new Error('Expected key=value, got: ' + pair);
    }
    tags[pair.substring(0, split)] = pair.substring(split + 1);
  }

  return {
    translation: positional[0],
    geometries: geometries,
    tags: tags,
    options: options
  };
}

function inferredElementType(geometry)
{
  if (geometry === 'Point' || geometry === 'Vertex') return 'Node';
  if (geometry === 'Line' || geometry === 'Area') return 'Way';
  return 'Way';
}

function clone(value)
{
  return JSON.parse(JSON.stringify(value));
}

function sorted(value)
{
  if (value === null || typeof value !== 'object') return value;
  if (Array.isArray(value))
  {
    return value.map(sorted);
  }

  var result = {};
  Object.keys(value).sort().forEach(function(key) {
    result[key] = sorted(value[key]);
  });
  return result;
}

function stringify(value)
{
  return JSON.stringify(sorted(value));
}

function printJson(label, value)
{
  console.log(label + ': ' + stringify(value));
}

function normalizeFeatures(result)
{
  if (!result) return [];
  return Array.isArray(result) ? result : [result];
}

function resetDirectionLookups(translator)
{
  delete translator.fcodeLookup;
  delete translator.fcodeLookupOut;
  delete translator.lookup;
  delete translator.fuzzy;
}

function runGeometry(translation, translator, geometry, parsed)
{
  var elementType = parsed.options.element || inferredElementType(geometry);

  console.log(translation.name + '  ' + geometry + ':');
  printJson('Raw', parsed.tags);

  resetDirectionLookups(translator);
  var features = normalizeFeatures(translator.toOgr(clone(parsed.tags), elementType, geometry));

  if (features.length === 0)
  {
    console.log('  ## No Ogr tags ##');
  }

  for (var i = 0; i < features.length; i++)
  {
    var feature = features[i];
    var ogrLabel = features.length > 1 ? 'Ogr Multi ' + (i + 1) : 'Ogr';
    if (feature.tableName) ogrLabel += ' (' + feature.tableName + ')';
    printJson(ogrLabel, feature.attrs);

    // Copy the attrs before translating back because toOsm mutates the object.
    resetDirectionLookups(translator);
    var backToOsm = translator.toOsm(clone(feature.attrs), feature.tableName, geometry);
    var osmLabel = features.length > 1 ? 'OSM Multi ' + (i + 1) : 'OSM';
    printJson(osmLabel, backToOsm);

    resetDirectionLookups(translator);
    var roundTripFeatures = normalizeFeatures(translator.toOgr(clone(backToOsm), elementType, geometry));
    if (parsed.options.roundTrip)
    {
      console.log('Round Trip Ogr Count: ' + roundTripFeatures.length);
      for (var j = 0; j < roundTripFeatures.length; j++)
      {
        var roundTripLabel = 'Round Trip Ogr ' + (i + 1) + '.' + (j + 1);
        if (roundTripFeatures[j].tableName) roundTripLabel += ' (' + roundTripFeatures[j].tableName + ')';
        printJson(roundTripLabel, roundTripFeatures[j].attrs);
      }
    }
    else if (roundTripFeatures.length > 0)
    {
      var secondPassLabel = features.length > 1 ? 'Ogr Multi ' + (i + 1) + ' Again' : 'Ogr';
      if (roundTripFeatures[0].tableName) secondPassLabel += ' (' + roundTripFeatures[0].tableName + ')';
      printJson(secondPassLabel, roundTripFeatures[0].attrs);
    }
    else
    {
      console.log('  ## No second pass Ogr tags ##');
    }
  }

  console.log('-----');
}

function ensureHoot()
{
  if (!HOOT_HOME)
  {
    throw new Error('HOOT_HOME must be set. Usually: source ./SetupEnv.sh');
  }

  if (typeof hoot === 'undefined')
  {
    hoot = require(HOOT_HOME + '/lib/HootJs');
  }
}

function runParsed(parsed)
{
  ensureHoot();
  setDefaults(parsed.options || {});

  var translation = loadTranslation(parsed.translation);
  var translator = translation.module;

  console.log('---------------');
  for (var i = 0; i < parsed.geometries.length; i++)
  {
    runGeometry(translation, translator, parsed.geometries[i], parsed);
  }
}

function runCase(testCase)
{
  var geometries = testCase.geometries || testCase.geometry && [testCase.geometry] || ['Point', 'Line', 'Area'];
  runParsed({
    translation: testCase.translation,
    geometries: geometries,
    tags: testCase.tags || {},
    options: testCase.options || {}
  });
}

if (typeof exports !== 'undefined')
{
  exports.runCase = runCase;
  exports.runParsed = runParsed;
  exports.parseArgs = parseArgs;
}

if (require.main === module)
{
  try
  {
    runParsed(parseArgs(process.argv.slice(2)));
  }
  catch (err)
  {
    console.error(err.stack || err);
    process.exit(1);
  }
}
