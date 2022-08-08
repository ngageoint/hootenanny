#!/usr/bin/node

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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2021, 2022 Maxar (http://www.maxar.com/)
 */

//
// Script to check translations
// checkAllFeatures & checkSingleFeature are basically the same. Having both makes testing easier
//

var HOOT_HOME = process.env.HOOT_HOME;
hoot = require(HOOT_HOME + '/lib/HootJs');

function dumpOne2OneLookup(lookupTable)
{
  for (var i in lookupTable)
    for (var j in lookupTable[i])
    {
      console.log('I:' + i + '  J:' + j + '  ' + lookupTable[i][j][0] + ' = ' + lookupTable[i][j][1]);
    }
} // End dumpOne2OneLookup


// hoot.require('tds71.js')
require(HOOT_HOME + '/translations/tds71.js')
require(HOOT_HOME + '/translations/tds71_rules.js')
require(HOOT_HOME + '/translations/tds71_full_schema.js')
require(HOOT_HOME + '/translations/translate.js')
require(HOOT_HOME + '/translations/fcode_common.js')


// transTest = require(HOOT_HOME + '/translations/checkTranslations.js');
// Skip the TransportationGroundCrv type layers
// hoot.Settings.set({"ogr.thematic.structure":"false"});
// LOTS of debug output
// hoot.Settings.set({"ogr.debug.dumptags":"true"});
// hoot.Settings.set({"ogr.debug.dumpvalidate":"true"});

// ####################################################################################
// The map of schema to test

// console.log('tds71:');
// for (var i in tds71)
// {
//   console.log(i);
// }

// console.log('\nRules:');
// for (var i in tds71.rules)
// {
//   console.log(i);
// }

// Build some lookup tables to make life easier
fcodeCommon.one2one.forEach( function(item) { if (tds71.rules.fcList.includes(item[1])) tds71.rules.fcodeOne2oneIn.push(item); });
tds71.fcodeLookup = translate.createLookup(tds71.rules.fcodeOne2oneIn);
// dumpOne2OneLookup(tds71.fcodeLookup);

tds71.rules.one2one.push.apply(tds71.rules.one2one,tds71.rules.one2oneIn);
tds71.lookup = translate.createLookup(tds71.rules.one2one);
// console.log('#####');
// dumpOne2OneLookup(tds71.lookup);
// console.log('#####');

// Start the CSV
console.log('"feature","description","attribute","attributeValue","tag","tagValue"');

var schema = tds71.schema.getDbSchema();

schema.forEach(feature => {
  // console.log(JSON.stringify(feature,null,' '));
  var tag = '', tagValue = '';

  if (tds71.fcodeLookup['F_CODE'][feature.fcode])
  {
    tag = tds71.fcodeLookup['F_CODE'][feature.fcode][0];
    tagValue = tds71.fcodeLookup['F_CODE'][feature.fcode][1];
  }
  else
  {
    console.log('### Missing OSM tags for F_CODE: ' + feature.fcode);
  }

  // Things we have added that we want to skip
  var skipList = ['F_CODE','FCSUBTYPE','PFI','image_id','img_layer','img_mosaic','late_date','early_date','legacy_id'];

  console.log('"' + feature.name + ' (' + feature.fcode + ')","' + feature.desc + '","","","' + tag + '","' + tagValue + '"');
  for (var i in feature.columns)
  {
    var attr = feature.columns[i];

    // Skip stuff
    if (~skipList.indexOf(attr.name)) continue;

    var tmpName = attr.name;
    if (tds71.rules.swapListIn[attr.name]) tmpName = tds71.rules.swapListIn[attr.name];

    switch (attr.type)
    {
      case 'String':
        if (tds71.rules.txtBiased[tmpName])
        {
          console.log('"","' + attr.desc +'","' + attr.name + '","","' + tds71.rules.txtBiased[tmpName] + '",""');
        }
        else
        {
          // Why?????
          if (attr.name == 'MAG')
          {
            console.log('"","' + attr.desc +'","' + attr.name + '","","magnetic_variation",""');
            break;
          }

          console.log('### Missing text tags for : ' + attr.name);
        }
        break;

      case 'Real':
      case 'Integer':
        if (tds71.rules.numBiased[tmpName])
        {
          console.log('"","' + attr.desc +'","' + attr.name + '","","' + tds71.rules.numBiased[tmpName] + '",""');
        }
        else
        {
          if (attr.name == 'F_CODE' || attr.name == 'FCSUBTYPE') break;

          console.log('### Missing num tags for : ' + attr.name);
        }
        break;

      case 'enumeration':
        if (tds71.lookup[tmpName])
        {
          for (var j in attr.enumerations)
          {
            var tag = tds71.lookup[tmpName][attr.enumerations[j]['value']]
            console.log('"","' + attr.desc + '","' + attr.name + '","' + attr.enumerations[j]['value'] + ' = ' + attr.enumerations[j]['name'] + '","' + tag[0] + '","' + tag[1] + '"');
          }
          break;
        }
        else
        {
          if (~tmpName.indexOf('ZI020_GE4'))
          {
            var tagName = 'addr:country';
            switch (tmpName)
            {
              case 'ZI020_GE42':
                tagName = 'addr:country:second';
                break;

              case 'ZI020_GE43':
                tagName = 'addr:country:third';
                break;

              case 'ZI020_GE44':
                tagName = 'addr:country:fourth';
                break;
            }
            attr.enumerations.forEach( item => {
              console.log('"","' + attr.desc +'","' + attr.name + '","' + item.value + ' = ' + item.name + '","' + tagName + '","' + tds71.rules.ge4List[item.value] + '"');
            });
            break;
          }

          // Now handle the 2,3,4 attributes
          if (! isNaN(tmpName.slice(-1)))
          {
            // Grab the attribute for matching
            var shortName = tmpName.slice(0,-1);
            for (var i in attr.enumerations)
            {
              var tag = tds71.lookup[shortName][attr.enumerations[j]['value']]
              console.log('"","' + attr.desc + '","' + attr.name + '","' + attr.enumerations[j]['value'] + ' = ' + attr.enumerations[j]['name'] + '","' + tag[0] + '","' + tag[1] + '"');
            }
            break;
          }

          console.log('### Missing enum tags for: ' + attr.name);
        } // End else
        break;

      default:
        console.log('## Unknown type: ' + attr.type);
        break;
    } // End else
    // console.log(',' + attr.name +',',attr.type);
  }
  // console.log(feature.fcode);
});

// End
