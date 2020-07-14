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
 * @copyright Copyright (C) 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

/*
    DNC conversion script
        DNC <-> OSM

    Based on tds/__init__.js script
*/

dnc = {
  // getDbSchema - Load the standard schema or modify it into the Thematic structure
  getDbSchema: function() {

    // Warning: This is <GLOBAL> so we can get access to it from other functions
    dnc.rawSchema = dnc.schema.getDbSchema();

    // Now add some fields to hold OSM specific information on export
    for (var i = 0, schemaLen = dnc.rawSchema.length; i < schemaLen; i++)
    {
      dnc.rawSchema[i].columns.push( { name:'OSM_TAGS',
        desc:'Unused OSM tags',
        type:'String',
        defValue:''
      });
      dnc.rawSchema[i].columns.push( { name:'OSM_UUID',
        desc:'OSM UUID',
        type:'String',
        defValue:''
      });
    }

    // Now add an o2s[A,L,P] feature to the dnc.rawSchema
    // We can drop features but this is a nice way to see what we would drop
    dnc.rawSchema = translate.addEmptyFeature(dnc.rawSchema);

    // Add the empty Review layers
    dnc.rawSchema = translate.addReviewFeature(dnc.rawSchema);

    // Debug:
    // translate.dumpSchema(dnc.rawSchema);

    return dnc.rawSchema;
  }, // End getDbSchema

  // Sort out if we need to return more than one feature
  // This is generally for Roads, Railways, bridges, tunnels etc
  manyFeatures: function(geometryType, tags, attrs, transMap)
  {
    // Add the first feature to the structure that we return
    var returnData = [{attrs:attrs, tableName:''}];

    // Quit early if we don't need to check anything. We are only looking at linework from now on
    if (geometryType !== 'Line') return returnData;

    // Admin Line vs Coastline - FA000
    // I have no idea why this F_CODE is in two layers.
    if (attrs.F_CODE == 'FA000')
    {
      // returnData.push({attrs:attrs, tableName:'COALINE'});
      returnData.push({attrs:{'F_CODE':'FA000X'}, tableName:'COALINE'});
      return returnData;
    }


    // Only looking at roads & railways with something else tacked on
    if (!(tags.highway || tags.railway)) return returnData;

    // Check the list of secondary/tertiary etc features
    if (!(tags.bridge || tags.tunnel || tags.embankment || tags.cutting || tags.ford)) return returnData;

    // We are going to make another feature so copy tags and trash the UUID so it gets a new one
    var newFeatures = [];
    var newAttributes = {};
    var nTags = JSON.parse(JSON.stringify(tags));
    delete nTags.uuid;
    delete nTags['hoot:id'];

    // Now drop the tags that made the FCODE
    switch(attrs.F_CODE)
    {
    case 'AN010': // Railway
    case 'AN050': // Railway Sidetrack
      delete nTags.railway;
      newAttributes.TRS = '12'; // Transport Type = Railway
      break;

    case 'AP010': // Cart Track
    case 'AP030': // Road
    case 'AP050': // Trail
      switch (nTags.highway)
      {
      case 'pedestrian':
      case 'footway':
      case 'steps':
      case 'path':
      case 'bridleway':
      case 'cycleway':
        newAttributes.TRS = '9'; // Transport Type = Pedestrian
        break;

      default:
        newAttributes.TRS = '13'; // Transport Type = Road
      }
      delete nTags.highway;
      break;

    case 'AQ040': // Bridge
      delete nTags.bridge;
      break;

    case 'AQ130': // Tunnel
      delete nTags.tunnel;
      break;

    case 'BH070': // Ford
      delete nTags.ford;
      break;

    case 'DB070': // Cutting
      delete nTags.cutting;
      break;

    case 'DB090': // Embankment
      delete nTags.embankment;
      break;

    default:
      // Debug
      hoot.logWarn('ManyFeatures: Should get to here');
    } // end switch

    // Now make new features based on what tags are left
    if (nTags.railway)
    {
      newFeatures.push({attrs: JSON.parse(JSON.stringify(newAttributes)), tags: JSON.parse(JSON.stringify(nTags))});
      delete nTags.railway;
    }

    if (nTags.highway)
    {
      newFeatures.push({attrs: JSON.parse(JSON.stringify(newAttributes)), tags: JSON.parse(JSON.stringify(nTags))});
      delete nTags.highway;
    }

    if (nTags.cutting)
    {
      newAttributes.F_CODE = 'DB070';
      newFeatures.push({attrs: JSON.parse(JSON.stringify(newAttributes)), tags: JSON.parse(JSON.stringify(nTags))});
      delete nTags.cutting;
    }

    if (nTags.bridge && nTags.bridge !== 'no')
    {
      newAttributes.F_CODE = 'AQ040';
      newFeatures.push({attrs: JSON.parse(JSON.stringify(newAttributes)), tags: JSON.parse(JSON.stringify(nTags))});
      delete nTags.bridge;
    }

    if (nTags.tunnel && nTags.tunnel !== 'no')
    {
      newAttributes.F_CODE = 'AQ130';
      newFeatures.push({attrs: JSON.parse(JSON.stringify(newAttributes)), tags: JSON.parse(JSON.stringify(nTags))});
      delete nTags.tunnel;
    }

    if (nTags.ford && nTags.ford !== 'no')
    {
      newAttributes.F_CODE = 'BH070';
      newFeatures.push({attrs: JSON.parse(JSON.stringify(newAttributes)), tags: JSON.parse(JSON.stringify(nTags))});
      delete nTags.ford;
    }

    if (nTags.embankment)
    {
      newAttributes.F_CODE = 'DB090';
      newFeatures.push({attrs: JSON.parse(JSON.stringify(newAttributes)), tags: JSON.parse(JSON.stringify(nTags))});
      delete nTags.embankment;
    }

    // Loop through the new features and process them
    for (var i = 0, nFeat = newFeatures.length; i < nFeat; i++)
    {
      // pre processing
      dnc.applyToOgrPreProcessing(newFeatures[i]['tags'], newFeatures[i]['attrs'], geometryType);

      var notUsedTags = (JSON.parse(JSON.stringify(tags)));

      // apply the simple number and text biased rules
      // Note: These are BACKWARD, not forward!
      translate.numToOgr(newFeatures[i]['attrs'], notUsedTags, dnc.rules.numBiased,dnc.rules.intList,transMap);
      translate.txtToOgr(newFeatures[i]['attrs'], notUsedTags, dnc.rules.txtBiased,transMap);

      // one 2 one
    print('manyFeatures applyOne2One');
      translate.applyOne2One(notUsedTags, newFeatures[i]['attrs'], dnc.lookup, dnc.fcodeLookup,transMap);

      // post processing
      dnc.applyToOgrPostProcessing(newFeatures[i]['tags'], newFeatures[i]['attrs'], geometryType,notUsedTags);

      returnData.push({attrs: newFeatures[i]['attrs'],tableName: ''});
    }

    return returnData;
  }, // End manyFeatures


  // validateAttrs - Clean up the supplied attr list by dropping anything that should not be part of the
  //                 feature. Also, set per feature defaults where appropriate.
  validateAttrs: function(layerName,geometryType,attrs,notUsed,transMap) {

    // Debug:
    // print('Validate: ' + attrs.F_CODE + ' Geom:' + geometryType + ' LayerName:' + layerName);

    // No quick and easy way to do this unless we build yet another lookup table
    var feature = {};
    for (var i=0, sLen = dnc.rawSchema.length; i < sLen; i++)
    {
      if (dnc.rawSchema[i].name == layerName)
      {
        feature = dnc.rawSchema[i];
        break;
      }
    }

    // Sanity checkoing
    if (feature.name === undefined)
    {
      // If we can't find an entry in attrArray then we have a problem,
      // Throw an error message and return.
      hoot.logError('Validate: No feature for ' + layerName);
      return;
    }
    // else
    // {
    // Debug:
    // print('FeatureName: ' + feature.name);
    // }


    var attrArray = [];
    for (var j=0, cLen = feature.columns.length; j < cLen; j++)
    {
      attrArray.push(feature.columns[j].name);
    }

    // First: Go through the translated attributes and drop those that are not valid for the feature
    for (var val in attrs)
    {
      if (attrArray.indexOf(val) == -1)
      {
        // Debug:
        // print('Validate: Dropping ' + val + ' from ' + layerName);
        if (val in transMap)
        {
          notUsed[transMap[val][1]] = transMap[val][2];
          // Debug:
          // print('Validate: Re-Adding ' + transMap[val][1] + ' = ' + transMap[val][2] + ' to notUsed');
        }
        // else
        // {
        // Debug:
        //     hoot.logError('Validate: ' + val + ' missing from transMap');
        // }

        delete attrs[val];

        // Since we deleted the attribute, Skip the text check
        continue;
      }

      // Now check the length of the text fields
      // We need more info from the customer about this: What to do if it is too long
      if (val in dnc.rules.txtLength)
      {
        if (attrs[val].length > dnc.rules.txtLength[val])
        {
          // First try splitting the attribute and grabbing the first value
          var tStr = attrs[val].split(';');
          if (tStr[0].length <= dnc.rules.txtLength[val])
          {
            attrs[val] = tStr[0];
          }
          else
          {
            hoot.logWarn('Validate: Attribute ' + val + ' is ' + attrs[val].length + ' characters long. Truncating to ' + dnc.rules.txtLength[val] + ' characters.');
            // Still too long. Chop to the maximum length
            attrs[val] = tStr[0].substring(0,dnc.rules.txtLength[val]);
          }
        } // End text attr length > max length
      } // End in txtLength
    } // End attr loop

    // Grab the F_CODE and the geometry for use in lookup tables
    var gFcode = attrs.F_CODE + geometryType.toString().charAt(0);

    // Second: Now go through the feature, check the values and look at enumerations
    // for (var i=0, cLen = feature['columns'].length; i < cLen; i++)
    for (var i=0, cLen = feature.columns.length; i < cLen; i++)
    {
      var colName = feature.columns[i].name;

      // See if we have a default/mandatory value
      if (dnc.rules.defaultList[gFcode])
      {
        // Debug:
        // print('Found: ' + gFcode + ' in the default list');
        // if (dnc.rules.defaultList[gFcode][attrs[colName]])
        if (dnc.rules.defaultList[gFcode][colName])
        {
          // Debug:
          // print('Setting: ' + colName + ' to ' + dnc.rules.defaultList[gFcode][colName]);
          attrs[colName] = dnc.rules.defaultList[gFcode][colName];

          // We wiped the value so try to restore the tags for it
          // NOTE: If this was an empty default, there is nothing to restore
          if (colName in transMap)
          {
            notUsed[transMap[colName][1]] = transMap[colName][2];
            // Debug:
            // print('Validate: re-adding enumeration ' + transMap[colName][1] + ' = ' + transMap[colName][2] + ' to notUsed');
          }

          // Since we eddited the attribute, no point checking if it is enumerated.
          continue;
        }
      } // End F_CODE in defaultList

      // Now check if having a value for the attribute is actually valid
      if (feature.columns[i].type == 'enumeration')
      {
        var attrValue = attrs[colName];
        var enumList = feature.columns[i].enumerations;
        var enumValueList = [];

        // Pull all of the values out of the enumerated list to make life easier
        for (var j=0, elen = enumList.length; j < elen; j++) enumValueList.push(enumList[j].value);

        // Check if it is a valid enumerated value
        if (enumValueList.indexOf(attrValue) == -1)
        {
          // Do we have an "Other" value?
          if (enumValueList.indexOf('999') == -1 || attrValue == undefined)
          {
            // No: Set the offending enumerated value to the default value
            attrs[colName] = feature.columns[i].defValue;

            // Debug:
            // print('Validate: 2: Enumerated Value: ' + attrValue + ' not found in ' + colName + ' Setting ' + colName + ' to its default value (' + feature.columns[i].defValue + ')');
          }
          else
          {
            // Yes: Set the offending enumerated value to the "other" value
            attrs[colName] = '999';

            // Debug:
            // print('Validate: 2: Enumerated Value: ' + attrValue + ' not found in ' + colName + ' Setting ' + colName + ' to Other (999)');
          }

          // Since we either wiped the value or set it to '999', try to restore the tags for it
          if (colName in transMap)
          {
            notUsed[transMap[colName][1]] = transMap[colName][2];
            // Debug:
            // print('Validate: 2: re-adding enumeration ' + transMap[colName][1] + ' = ' + transMap[colName][2] + ' to notUsed');
          }
        }

        continue;
      } // End validate Enumerations

      // Since we did the enumerations, Text and Numbers are next


    } // End feature attributes
  }, // End validateAttrs


  // Apply one to one translations and don't report errors: missing columns etc
  // This does keep track of what has been used so we can undo it later if needed
  applyOne2OneModified : function(inList, outList, lookup, transMap)
  {
    var row = [];

    for (var col in inList)
    {
      var value = inList[col];
      if (col in lookup)
      {
        if (value in lookup[col])
        {
          row = lookup[col][value];

          // Drop all of the undefined values
          if (row[0])
          {
            outList[row[0]] = row[1];
            transMap[row[0]] = [row[1],col,value];

            // Debug:
            // print('Adding: ' + row[0] + ' as ' + transMap[row[0]]);

            delete inList[col];
          }
        }
      }
    } // End for col in inList
  }, // End applyOne2OneModified


  // applyTxtToOgr - Apply 0ne2one rules for Text Attributes
  // The "direction is either "forward" or "backward" - convert to or from
  applyTxtToOgr : function(attrs, tags, rules, transMap)
  {
    // convert Tags to Attrs
    for (var i in rules)
    {
      if (rules[i] in tags)
      {
        attrs[i] = tags[rules[i]];
        transMap[i] = ['',rules[i],tags[rules[i]]];
        // Debug:
        // print('Txt Adding: ' + i + ' as ' + transMap[attrs[i]]);

        delete tags[rules[i]];
      }
    }
  }, // End applyTxtToOgr

  // applyNumToOgr - Apply 0ne2one rules for Number Attributes
  // NOTE: This is the toOgr direction
  applyNumToOgr : function(attrs, tags, rules, intList, transMap)
  {
    // convert Tags to Attrs
    for (var i in rules)
    {
      if (rules[i] in tags)
      {
        // Strip out anything that is not a number. Get rid of 125m etc
        var tNum = tags[rules[i]].replace(/-[^0-9\\.]+/g, '');

        if (translate.isNumber(tNum))
        {
          // Now check the Integer attributes
          if (intList.indexOf(i) > -1)
          {
            // Quick bitwise or to strip off anything after the decimal
            var tInt = tNum | 0;

            // Back to a string for a comparison
            if ((tInt + '') !== tNum)
            {
              hoot.logTrace('Converting ' + i + ' from ' + tNum + ' to ' + tInt);
            }

            tNum = tInt;

          } // End in intList

          attrs[i] = tNum;
          transMap[i] = ['',rules[i],tags[rules[i]]];
          // Debug:
          // print('Num Adding: ' + i + ' as ' + transMap[attrs[i]]);

          delete tags[rules[i]];
        }
        else
        {
          hoot.logTrace('Expected a number for:: ' + rules[i] + '. Got ' + tags[rules[i]] + ' instead. Skipping ' + i);
        }
      }
    }
  }, // End applyNumToOgr


  // appendToOsmTags: Unpacks the OSM_TAGS text string, appends a value to it and then re-packs it
  appendToOsmTags : function (rawTagObj, tag, value)
  {
    // Debug:
    // print('appendToOsmTags: in: ' + rawTagObj);

    var tTags = {};

    // Make sure we have something to work with
    if (rawTagObj)
    {
      var tObj = translate.unpackMemo(rawTagObj);

      if (tObj.tags !== '')
      {
        tTags = JSON.parse(tObj.tags);
      }
    }

    tTags[tag] = value;
    // Debug:
    // print('appendToOsmTags: out: ' + JSON.stringify(tTags));

    var tStr = '<OSM>' + JSON.stringify(tTags) + '</OSM>';
    return tStr;
  },


  // ##### Start of the xxToOsmxx Block #####
  applyToOsmPreProcessing: function(attrs, layerName, geometryType)
  {
    // List of data values to drop/ignore
    var ignoreList = { 'UNK':1, 'N/A':1, 'noinformation':1, '-32768':1, '-2147483648':1 };

    // This is a handy loop. We use it to:
    // 1) Remove all of the "No Information" and -999999 fields
    // 2) Convert all of the Attrs to uppercase - if needed
    // 3) Swap some of the funky named attrs around.
    // 4) Move the appropriate XXX2 and XXX3 attributes to a 'Note'
    for (var col in attrs)
    {
      // Trim off the whitespace
      attrs[col] = attrs[col].trim();

      // slightly ugly but we would like to account for: 'No Information', 'noInformation' etc
      // First, push to lowercase
      var attrValue = attrs[col].toString().toLowerCase();

      // Get rid of the spaces in the text
      attrValue = attrValue.replace(/\s/g, '');

      // Wipe out the useless values
      if (attrs[col] == '' || attrValue in ignoreList || attrs[col] in ignoreList)
      {
        delete attrs[col]; // debug: Comment this out to leave all of the No Info stuff in for testing
        continue;
      }

      // Push the attribute to upper case - if needed
      var c = col.toUpperCase();
      if (c !== col)
      {
        attrs[c] = attrs[col];
        delete attrs[col];
        col = c; // For the rest of this loop iteration
      }
    } // End in attrs loop

  }, // End of applyToOsmPreProcessing


  applyToOsmPostProcessing : function (attrs, tags, layerName, geometryType)
  {
    // Unpack the OSM_TAGS attribute if it exists
    if (attrs.OSM_TAGS)
    {
      var tObj = translate.unpackMemo(attrs.OSM_TAGS);

      if (tObj.tags !== '')
      {
        var tTags = JSON.parse(tObj.tags);
        for (i in tTags)
        {
          // Debug
          // print('Memo: Add: ' + i + ' = ' + tTags[i]);
          if (tags[tTags[i]]) hoot.logWarn('Unpacking OSM_TAGS, overwriting ' + i + ' = ' + tags[i] + '  with ' + tTags[i]);
          tags[i] = tTags[i];
        }
      }

      if (tObj.text !== '')
      {
        tags.note = tObj.text;
      }
      else
      {
        delete tags.note;
      }
    } // End process attrs.OSM_TAGS

    // DNC doesn't have a lot of info for land features
    if (attrs.F_CODE == 'AP020') tags.junction = 'yes';
    if (attrs.F_CODE == 'AP030') tags.highway = 'road';
    if (attrs.F_CODE == 'BH140') tags.waterway = 'river';

    // The Data Quality layer doesn't have an F_CODE
    if (layerName.toLowerCase() == 'dqyarea_dqy') tags['source:metadata'] = 'dataset';

    // If we have a UFI, store it.
    tags.source = 'dnc:' + layerName.toLowerCase();
    if (attrs.OSM_UUID)
    {
      tags.uuid = '{' + attrs['OSM_UUID'].toString().toLowerCase() + '}';
    }
    else
    {
      tags.uuid = createUuid();
    }

    if (dnc.osmPostRules == undefined)
    {
      // ##############
      // A "new" way of specifying rules. Jason came up with this while playing around with NodeJs
      //
      // Rules format:  ["test expression","output result"];
      // Note: t = tags, a = attrs and attrs can only be on the RHS
      var rulesList = [
        ['t["radar:use"] == "early_warning" && !(t.man_made == "radar_station")','t.man_made = "radar_station"'],
        ['t["cable:type"] && !(t.cable)','t.cable = "yes";'],
        ['t["tower:type"] && !(t.man_made)','t.man_made = "tower"'],
        ['t.foreshore && !(t.tidal)','t.tidal = "yes"; t.natural = "water"'],
        ['t.tidal == "yes" && !(t.natural)','t.natural = "water"']
        // ["t.tidal && !(t.water)","t.natural = 'water'"]
      ];

      dnc.osmPostRules = translate.buildComplexRules(rulesList);
    }
    translate.applyComplexRules(tags,attrs,dnc.osmPostRules);

    // Fix lifecycle tags
    switch (tags.condition)
    {
    case undefined: // Break early if no value
      break;

    case 'construction':
      for (var typ of ['highway','bridge','railway','building'])
      {
        if (tags[typ])
        {
          tags.construction = tags[typ];
          tags[typ] = 'construction';
          delete tags.condition;
          break;
        }
      }
      break;

    case 'abandoned':
      for (var typ of ['highway','bridge','railway','building'])
      {
        if (tags[typ])
        {
          tags['abandoned:' + typ] = tags[typ];
          delete tags[typ];
          delete tags.condition;
          break;
        }
      }
      break;

    case 'dismantled':
      for (var typ of ['highway','bridge','railway','building'])
      {
        if (tags[typ])
        {
          tags['demolished:' + typ] = tags[typ];
          delete tags[typ];
          delete tags.condition;
          break;
        }
      }
      break;
    } // End switch condifion

  }, // End of applyToOsmPostProcessing

  // ##### End of the xxToOsmxx Block #####
  // #####################################################################################################


  // ##### Start of the xxToOgrxx Block #####
  applyToOgrPreProcessing: function(tags, attrs, geometryType)
  {
    // Remove Hoot assigned tags for the source of the data
    if (tags['source:ingest:datetime']) delete tags['source:ingest:datetime'];
    if (tags.area) delete tags.area;
    if (tags['error:circular']) delete tags['error:circular'];
    if (tags['hoot:status']) delete tags['hoot:status'];

    // Initial cleanup
    for (var i in tags)
    {
      // Remove empty tags
      if (tags[i] == '')
      {
        delete tags[i];
        continue;
      }

      // Convert "abandoned:XXX" and "disused:XXX"features
      if ((i.indexOf('abandoned:') !== -1) || (i.indexOf('disused:') !== -1))
      {
        // Hopeing there is only one ':' in the tag name...
        var tList = i.split(':');
        tags[tList[1]] = tags[i];
        tags.condition = 'abandoned';
        delete tags[i];
        continue;
      }

      // Convert "demolished:XXX" features
      if (i.indexOf('demolished:') !== -1)
      {
        // Hopeing there is only one ':' in the tag name...
        var tList = i.split(':');
        tags[tList[1]] = tags[i];
        tags.condition = 'dismantled';
        delete tags[i];
        continue;
      }
    } // End Cleanup loop

    // Lifecycle: This is a bit funky and should probably be done with a fancy function instead of
    // repeating the code
    switch (tags.highway)
    {
    case undefined: // Break early if no value
      break;

    case 'abandoned':
    case 'disused':
      tags.highway = 'road';
      tags.condition = 'abandoned';
      break;

    case 'demolished':
      tags.highway = 'road';
      tags.condition = 'dismantled';
      break;

    case 'construction':
      if (tags.construction)
      {
        tags.highway = tags.construction;
        delete tags.construction;
      }
      else
      {
        tags.highway = 'road';
      }
      tags.condition = 'construction';
      break;
    }

    switch (tags.railway)
    {
    case undefined: // Break early if no value
      break;

    case 'abandoned':
    case 'disused':
      tags.railway = 'rail';
      tags.condition = 'abandoned';
      break;

    case 'demolished':
      tags.railway = 'yes';
      tags.condition = 'dismantled';
      break;

    case 'construction':
      if (tags.construction)
      {
        tags.railway = tags.construction;
        delete tags.construction;
      }
      else
      {
        tags.railway = 'rail';
      }
      tags.condition = 'construction';
      break;
    }

    switch (tags.building)
    {
    case undefined: // Break early if no value
      break;

    case 'abandoned':
    case 'disused':
      tags.building = 'yes';
      tags.condition = 'abandoned';
      break;

    case 'destroyed':
      tags.building = 'yes';
      tags.condition = 'destroyed';
      break;

    case 'demolished':
      tags.building = 'yes';
      tags.condition = 'dismantled';
      break;

    case 'construction':
      if (tags.construction)
      {
        tags.building = tags.construction;
        delete tags.construction;
      }
      else
      {
        tags.building = 'yes';
      }
      tags.condition = 'construction';
      break;
    }

    switch (tags.bridge)
    {
    case undefined: // Break early if no value
      break;

    case 'abandoned':
    case 'disused':
      tags.bridge = 'yes';
      tags.condition = 'abandoned';
      break;

    case 'destroyed':
      tags.bridge = 'yes';
      tags.condition = 'destroyed';
      break;

    case 'demolished':
      tags.bridge = 'yes';
      tags.condition = 'dismantled';
      break;

    case 'construction':
      if (tags.construction)
      {
        tags.bridge = tags.construction;
        delete tags.construction;
      }
      else
      {
        tags.bridge = 'yes';
      }
      tags.condition = 'construction';
      break;
    }


    if (dnc.dncPreRules == undefined)
    {
      // ##############
      // A "new" way of specifying rules. Jason came up with this while playing around with NodeJs
      //
      // Rules format:  ["test expression","output result"];
      // Note: t = tags, a = attrs and attrs can only be on the RHS
      var rulesList = [
        ['t["radar:use"] == "early_warning" && t.man_made == "radar_station"','delete t.man_made'],
        ['t.water == "tidal"','t.tidal = "yes";delete t.water']
      ];

      dnc.dncPreRules = translate.buildComplexRules(rulesList);
    }

    // Apply the rulesList
    // translate.applyComplexRules(tags,attrs,dnc.dncPreRules);
    // Pulling this out of translate
    for (var i = 0, rLen = dnc.dncPreRules.length; i < rLen; i++)
    {
      if (dnc.dncPreRules[i][0](tags)) dnc.dncPreRules[i][1](tags,attrs);
    }


    // natural water usually has a subtype. If it does, just keep the subtype
    if (tags.natural == 'water' && tags.water) delete tags.natural;

    // // Fix up OSM 'walls' around facilities
    // if (tags.barrier == 'wall' && geometryType == 'Area')
    // {
    //     attrs.F_CODE = 'AL010'; // Facility
    //     delete tags.barrier; // Take away the walls...
    // }

    // // An "amenity" can be a building or a thing
    // // If appropriate, make the "amenity" into a building
    // // This list is taken from the OSM Wiki and Taginfo
    // var notBuildingList = [
    //     'bbq','biergarten','drinking_water','bicycle_parking','bicycle_rental','boat_sharing',
    //     'car_sharing','charging_station','grit_bin','parking','parking_entrance','parking_space',
    //     'taxi','atm','fountain','bench','clock','hunting_stand','post_box',
    //     'recycling', 'vending_machine','waste_disposal','watering_place','water_point',
    //     'waste_basket','drinking_water','swimming_pool','fire_hydrant','emergency_phone','yes',
    //     'compressed_air','water','nameplate','picnic_table','life_ring','grass_strip','dog_bin',
    //     'artwork','dog_waste_bin','street_light','park','hydrant','tricycle_station','loading_dock',
    //     'trailer_park','game_feeding','ferry_terminal'
    //     ]; // End bldArray

    // if (tags.amenity && notBuildingList.indexOf(tags.amenity) == -1 && !tags.building) attrs.F_CODE = 'AL015';

    // Places, localities and regions
    switch (tags.place)
    {
    case undefined: // Break early if no value
      break;

    case 'city':
    case 'town':
    case 'suburb':
    case 'neighbourhood':
    case 'quarter':
    case 'village':
    case 'hamlet':
    case 'yes':  // We set this as a default when going to OSM
      attrs.F_CODE = 'AL020'; // Built Up Area
      delete tags.place;
      break;

    case 'isolated_dwelling':
      attrs.F_CODE = 'AL105'; // Settlement
      delete tags.place;
      break;

    case 'populated':
    case 'state':
    case 'county':
    case 'region':
    case 'locality':
    case 'municipality':
    case 'borough':
    case 'unknown':
      attrs.F_CODE = 'ZD040'; // Named Location
      delete tags.place;
      break;

    case 'island':
    case 'islet':
      // If we have a coastline around an Island, decide if we are going make an Island
      // or a Coastline
      if (tags.natural == 'coastline')
      {
        if (geometryType == 'Line')
        {
          attrs.F_CODE = 'BA010'; // Land/Water Boundary - Line
          delete tags.place;
        }
        else
        {
          // NOTE: If we have a Point, this will goto the O2S layer
          attrs.F_CODE = 'BA030'; // Island - Polygon
          delete tags.natural;
        }
      }
      break;

    } // End switch

    // Capitals are important
    if (tags.capital)
    {
      if (!(tags['place:importance'])) tags['place:importance'] = 'first';
      delete tags.capital;
    }

    // Built-up-areas & Settlements vs Buildings
    // If we have a BUA or a Settlement, change the settlement:type tag to a building so we can
    // go through one2one and get an FFN out
    if (tags['settlement:type'] && !tags.building)
    {
      tags.building = tags['settlement:type'];
      delete tags['settlement:type'];
    }

    // Movable Bridges
    if (tags.bridge == 'movable')
    {
      if (! tags['bridge:movable'])
      {
        tags['bridge:movable'] = 'yes';
      }
      tags.bridge = 'yes';
      attrs.F_CODE = 'AQ040';
    }

    // Viaducts
    if (tags.bridge == 'viaduct')
    {
      tags.bridge = 'yes';
      tags.note = translate.appendValue(tags.note,'Viaduct',';');
    }

    // Exceptions to the rules. Values from TagInfo
    switch (tags.highway)
    {
    case 'bus_stop':
    case 'corridor':
    case 'crossing':
    case 'elevator':
    case 'footway':
    case 'give_way':
    case 'milestone':
    case 'no':
    case 'path':
    case 'priority':
    case 'rest_area':
    case 'speed_camera':
    case 'speed_display':
    case 'steps':
    case 'stop':
    case 'street_lamp':
    case 'street_light':
    case 'toll_gantry':
    case 'traffic_mirror':
    case 'traffic_sign':
    case 'traffic_signals':
      attrs.OSM_TAGS = dnc.appendToOsmTags(attrs.OSM_TAGS,'highway',tags.highway);
      delete tags.highway;
      break;
    } // End highway switch

    // Exceptions to the rules. Values from TagInfo
    switch (tags.railway)
    {
    case 'buffer_stop':
    case 'crossing':
    case 'halt':
    case 'junction':
    case 'level_crossing':
    case 'milestone':
    case 'phone':
    case 'platform':
    case 'platform_edge':
    case 'radio':
    case 'railway_crossing':
    case 'signal_box':
    case 'station':
    case 'stop':
    case 'subway_entrance':
    case 'switch':
    case 'vacancy_detection':
    case 'ventilation_shaft': // ???
      attrs.OSM_TAGS = dnc.appendToOsmTags(attrs.OSM_TAGS,'railway',tags.railway);
      delete tags.railway;
      break;
    } // End railway switch


    // Bays and beaches can become named locations if they are points
    if (geometryType == 'Point' && tags.natural)
    {
      if (tags.natural == 'bay' || tags.natural == 'beach') attrs.F_CODE = 'ZD040'; // Named Area
    }

    // Keep looking for an FCODE
    // This uses the fcodeLookup tables that are defined earlier
    // var fcodeLookup = translate.createLookup(fcodeList);
    if (!attrs.F_CODE)
    {
      for (var col in tags)
      {
        var value = tags[col];
        if (col in dnc.fcodeLookup)
        {
          if (value in dnc.fcodeLookup[col])
          {
            var row = dnc.fcodeLookup[col][value];
            attrs.F_CODE = row[1];

            // Debug:
            // print('F_CODE: ' + col + ' = ' + value + ' makes ' + row[1]);
          }
        }
      }
    }

    // An "amenitiy" can be a building or a thing
    // If appropriate, make the "amenity" into a building
    // This list is taken from the OSM Wiki and Taginfo
    var notBuildingList = [
      'bbq','biergarten','drinking_water','bicycle_parking','bicycle_rental','boat_sharing',
      'car_sharing','charging_station','grit_bin','parking','parking_entrance','parking_space',
      'taxi','atm','fountain','bench','clock','hunting_stand','post_box',
      'recycling', 'vending_machine','waste_disposal','watering_place','water_point',
      'waste_basket','drinking_water','swimming_pool','fire_hydrant','emergency_phone','yes',
      'compressed_air','water','nameplate','picnic_table','life_ring','grass_strip','dog_bin',
      'artwork','dog_waste_bin','street_light','park','hydrant','tricycle_station','loading_dock',
      'trailer_park','game_feeding','ferry_terminal'
    ]; // End bldArray

    if (!attrs.F_CODE && tags.amenity && notBuildingList.indexOf(tags.amenity) == -1 && !tags.building) attrs.F_CODE = 'AL015';

    // The FCODE for Buildings changed...
    if (attrs.F_CODE == 'AL013') attrs.F_CODE = 'AL015';

    // Now, re-translate some of the NAS building F_CODES to DNC
    var buildingList = ['AD041','AH060','AJ080','AJ085','AJ110','AK110','AL014','AL019','AL099','AL250','AL371','GB230'];
    if (buildingList.indexOf(attrs.F_CODE) > -1)
    {
      attrs.F_CODE = 'AL015'; // General Building
    }

    // Some more custom F_CODE fixes
    if (!attrs.F_CODE)
    {
      if (tags.aeroway == 'heliport') attrs.F_CODE = 'GB005'; // Airport
      if (tags.landcover == 'snowfield' || tags.landcover == 'ice-field') attrs.F_CODE = 'BJ100'; // Snowfield/Icefield
      if (tags.aeroway == 'heliport') attrs.F_CODE = 'GB005'; // Airport
    }

  }, // End applyToOgrPreProcessing

  // #####################################################################################################

  applyToOgrPostProcessing : function (tags, attrs, geometryType, notUsedTags)
  {
    // BFC generally == Building
    if (attrs.BFC && !attrs.F_CODE) attrs.F_CODE = 'AL015';

    // Tourism: These should be buildings but don't appear in the BFC list
    switch (tags.tourism)
    {
    case undefined: // Break early if no value
      break;

    case 'alpine_hut':
    case 'apartment':
    case 'aquarium':
    case 'bed_and_breakfast':
    case 'cabin':
    case 'chalet':
    case 'guest_house':
    case 'hostel':
    case 'informaton':
    case 'motel':
    case 'resort':
    case 'wilderness_hut':
      attrs.F_CODE = 'AL015';
      attrs.BFC = '95'; // Hotel
      break;

    case 'attraction':
    case 'gallery':
      attrs.F_CODE = 'AL015';
      attrs.BFC = '999'; // Other
      break;

    case 'view_point':
      attrs.F_CODE = 'ZD040'; // Named location
      break;

    } // End tourism switch

    // Landuse. A couple of options for these
    switch (tags.landuse)
    {
    case undefined: // Break early if no value
      break;

    case 'basin':
    case 'brownfield':
    case 'conservation':
    case 'farm':
    case 'farmland':
    case 'field':
    case 'grass':
    case 'greenfield':
    case 'landfill':
    case 'meadow':
      attrs.F_CODE = 'DA010'; // Ground Surface Element
      break;

    case 'village_green':
    case 'recreation_ground':
      attrs.F_CODE = 'AK120'; // Park
      break;
    } // End landuse switch


    // If we still don't have an FCODE, try a bit of brute force
    if (!attrs.F_CODE)
    {
      var fcodeMap = {
        'highway':'AP030', 'railway':'AN010', 'building':'AL015',
        'ford':'BH070', 'waterway':'BH140', 'bridge':'AQ040',
        'shop':'AL015'
      };

      for (var i in fcodeMap)
      {
        if (i in tags)
        {
          // Debug
          // print('Added FCODE from Map: ' + fcodeMap[i]);
          attrs.F_CODE = fcodeMap[i];
          break;
        }
      }

    } // End !fcode

    // Sort out the UUID
    if (attrs.OSM_UUID)
    {
      var str = attrs['OSM_UUID'].split(';');
      attrs.OSM_UUID = str[0].replace('{','').replace('}','');
    }
    else if (tags['hoot:id'])
    {
      attrs.OSM_UUID = 'raw_id:' + tags['hoot:id'];
    }
    else
    {
      if (dnc.configOut.OgrAddUuid == 'true') attrs.OSM_UUID = createUuid().replace('{','').replace('}','');
    }

    // Fix some of the default values
    switch (attrs.F_CODE + geometryType.toString().charAt(0))
    {
    case undefined: // Break early if no value
      break;

    case 'AL015P':
      if (attrs.BFC == '81')
      {
        if (!attrs.COL) attrs.COL = 'N/A';
        if (!attrs.EXS) attrs.EXS = '-32768';
        if (!attrs.SSR) attrs.SSR = '-32768';
      }
      else
      {
        if (!attrs.SST) attrs.SST = '-32768';
        if (!attrs.STA) attrs.STA = '-32768';

      }
      break;

    case 'BB010A': // Anchorage
    case 'BB010L': // Anchorage
      if (!attrs.MAC) attrs.MAC = '11';
      break;

    case 'FC021A': // Maritime Limit Boundary
      if (!attrs.PRO) attrs.PRO = '130';
      break;

    case 'FC021L': // Maritime Limit Boundary
      switch (attrs.MBL)
      {
      case undefined:
        break;

      case '-32768':
        if (!attrs.COD) attrs.COD = '1';
        if (!attrs.MAC) attrs.MAC = '0';
        if (!attrs.MBL) attrs.MBL = '0';
        if (!attrs.OPS) attrs.OPS = '1';
        break;

      case '13':
        if (!attrs.DRP) attrs.DRP = 'UNK';
        if (!attrs.LAF) attrs.LAF = '0';
        // Fall through to default

      default:
        if (!attrs.DRP) attrs.DRP = 'N/A';
        if (!attrs.COD) attrs.COD = '-32768';
        if (!attrs.MAC) attrs.MAC = '-32768';
        if (!attrs.NAM) attrs.NAM = 'N/A';
        if (!attrs.OPS) attrs.OPS = '-32768';
        if (!attrs.PRO) attrs.PRO = '-32768';
        break;
      } // End MBL

      break;

    case 'FC031A': // Maritime Area
      if (!attrs.DAN && attrs.ATN == '2') attrs.DAN = 'N/A';
      break;

    case 'FC021P': // Maritime Limit Boundary
    case 'FC036P': // Restricted Area
      if (!attrs.MAC) attrs.MAC = '0';
      break;
    } // End default fixes

  }, // End applyToOgrPostProcessing

  // #####################################################################################################
  // ##### End of the xxToOgrxx Block #####


  // toOsm - Translate Attrs to Tags
  // This is the main routine to convert _TO_ OSM
  toOsm : function(attrs, layerName, geometryType)
  {
    tags = {};  // The final output Tag list

    // Setup config variables. We could do this in initialize() but some things don't call it :-(
    // Doing this so we don't have to keep calling into Hoot core
    if (dnc.configIn == undefined)
    {
      dnc.configIn = {};
      dnc.configIn.OgrDebugAddfcode = config.getOgrDebugAddfcode();
      dnc.configIn.OgrDebugDumptags = config.getOgrDebugDumptags();
      dnc.configIn.OgrAddUuid = config.getOgrAddUuid();

      // Get any changes to OSM tags
      // NOTE: the rest of the config variables will change to this style of assignment soon
      dnc.toChange = hoot.Settings.get('schema.translation.override');
    }
    // Debug:
    if (dnc.configIn.OgrDebugDumptags == 'true') translate.debugOutput(attrs,layerName,geometryType,'','In attrs: ');

    // See if we have an o2s_X layer and try to unpack it
    if (layerName.indexOf('o2s_') > -1)
    {
      tags = translate.parseO2S(attrs);

      // Add some metadata
      if (! tags.uuid)
      {
        if (dnc.configIn.OgrAddUuid == 'true') tags.uuid = createUuid();
      }

      if (! tags.source) tags.source = 'dnc:' + layerName.toLowerCase();

      // Debug:
      if (dnc.configIn.OgrDebugDumptags == 'true')
      {
        translate.debugOutput(tags,layerName,geometryType,'','Out tags: ');
        print('');
      }

      return tags;
    } // End layername = o2s_X

    // Set up the fcode translation rules. We need this due to clashes between the one2one and
    // the fcode one2one rules
    if (dnc.fcodeLookup == undefined)
    {
      dnc.fcodeLookup = translate.createLookup(dnc.rules.fcodeOne2oneIn);
      // translate.dumpOne2OneLookup(dnc.fcodeLookup);
    }

    if (dnc.lookup == undefined)
    {
      // Setup lookup tables to make translation easier. I'm assumeing that since this is not set, the
      // other tables are not set either.

      dnc.lookup = translate.createLookup(dnc.rules.one2one);

      // Build an Object with both the SimpleText & SimpleNum lists
      dnc.ignoreList = translate.joinList(dnc.rules.numBiased, dnc.rules.txtBiased);

      // Add features to ignore
      dnc.ignoreList.F_CODE = '';
      dnc.ignoreList.FAC_ID = '';
      dnc.ignoreList.ID = '';
      dnc.ignoreList.TILE_ID = '';
      dnc.ignoreList.EDG_ID = '';
      dnc.ignoreList.END_ID = '';
    }

    // pre processing
    dnc.applyToOsmPreProcessing(attrs, layerName, geometryType);

    // Use the FCODE to add some tags.
    if (attrs.F_CODE)
    {
      var ftag = dnc.fcodeLookup['F_CODE'][attrs.F_CODE];
      if (ftag)
      {
        tags[ftag[0]] = ftag[1];
        // Debug: Dump out the tags from the FCODE
        // print('FCODE: ' + attrs.F_CODE + ' tag=' + ftag[0] + '  value=' + ftag[1]);
      }
      else
      {
        hoot.logError('Translation for FCODE ' + attrs.F_CODE + ' not found');
      }
    }

    // Make a copy of the input attributes so we can remove them as they get translated. Looking at what
    // isn't used in the translation - this should end up empty.
    // not in v8 yet: // var tTags = Object.assign({},tags);
    var notUsedAttrs = (JSON.parse(JSON.stringify(attrs)));

    // apply the simple number and text biased rules
    // NOTE: We are not using the intList paramater for applySimpleNumBiased when going to OSM.
    translate.numToOSM(notUsedAttrs, tags, dnc.rules.numBiased);
    translate.txtToOSM(notUsedAttrs, tags, dnc.rules.txtBiased);

    // one 2 one
    // translate.applyOne2One(notUsedAttrs, tags, dnc.lookup, {'k':'v'}, dnc.ignoreList,{'k':'v'});
    translate.applyOne2One(notUsedAttrs, tags, dnc.lookup,{'k':'v'},{'k':'v'});

    // post processing
    dnc.applyToOsmPostProcessing(attrs, tags, layerName, geometryType);

    // Debug
    // for (var i in notUsedAttrs) print('NotUsed: ' + i + ': :' + notUsedAttrs[i] + ':');

    // Debug: Add the FCODE to the tags
    if (dnc.configIn.OgrDebugAddfcode == 'true') tags['raw:debugFcode'] = attrs.F_CODE;

    // Debug:
    if (dnc.configIn.OgrDebugDumptags == 'true')
    {
      translate.debugOutput(notUsedAttrs,layerName,geometryType,'','Not used: ');
      translate.debugOutput(tags,layerName,geometryType,'','Out tags: ');
      print('');
    }

    return tags;

  }, // End of toOsm

  // This gets called by translateToOGR and is where the main work gets done
  // We get Tags and return Attrs and a tableName
  // This is the main routine to convert _TO_ DNC
  toOgr : function(tags, elementType, geometryType)
  {
    var tableName = ''; // The final table name
    var returnData = []; // The array of features to return
    var transMap = {}; // A map of translated attributes
    attrs = {}; // The output attributes
    attrs.F_CODE = ''; // Initial setup

    // Setup config variables. We could do this in initialize() but some things don't call it :-(
    // Doing this so we don't have to keep calling into Hoot core
    if (dnc.configOut == undefined)
    {
      dnc.configOut = {};
      dnc.configOut.OgrDebugDumptags = config.getOgrDebugDumptags();
      dnc.configOut.OgrSplitO2s = config.getOgrSplitO2s();
      dnc.configOut.OgrThrowError = config.getOgrThrowError();
      dnc.configOut.OgrAddUuid = config.getOgrAddUuid();

      // Get any changes to OSM tags
      // NOTE: the rest of the config variables will change to this style of assignment soon
      dnc.toChange = hoot.Settings.get('schema.translation.override');
    }

    // Check if we have a schema. This is a quick way to workout if various lookup tables have been built
    if (dnc.rawSchema == undefined)
    {
      var tmp_schema = dnc.getDbSchema();
    }

    // The Nuke Option: If we have a relation, drop the feature and carry on
    if (tags['building:part']) return null;

    // The Nuke Option: "Collections" are groups of different geometry types: Point, Area and Line
    // There is no way we can translate these to a single GGDM30 feature
    if (geometryType == 'Collection') return null;

    // Start processing here
    // Debug:
    if (dnc.configOut.OgrDebugDumptags == 'true') translate.debugOutput(tags,'',geometryType,elementType,'In tags: ');

    // Set up the fcode translation rules. We need this due to clashes between the one2one and
    // the fcode one2one rules
    if (dnc.fcodeLookup == undefined)
    {
      // Add the FCODE rules for Export
      // We use the input rules and add the output ones on to the list
      dnc.rules.fcodeOne2oneIn.push.apply(dnc.rules.fcodeOne2oneOut,dnc.rules.fcodeOne2oneIn);
      dnc.fcodeLookup = translate.createBackwardsLookup(dnc.rules.fcodeOne2oneOut);
      // Debug
      // translate.dumpOne2OneLookup(dnc.fcodeLookup);
    }

    if (dnc.lookup == undefined)
    {
      // Add "other" rules to the one2one
      dnc.rules.one2one.push.apply(dnc.rules.one2one,dnc.rules.one2oneOut);

      dnc.lookup = translate.createBackwardsLookup(dnc.rules.one2one);
      // Debug
      // translate.dumpOne2OneLookup(dnc.lookup);
    } // End dnc.lookup Undefined

    // Override values if appropriate
    translate.overrideValues(tags,dnc.toChange);

    // Pre Processing
    dnc.applyToOgrPreProcessing(tags, attrs, geometryType);

    // Make a copy of the input tags so we can remove them as they get translated. What is left is
    // the not used tags
    // not in v8 yet: // var tTags = Object.assign({},tags);
    var notUsedTags = (JSON.parse(JSON.stringify(tags)));

    if (notUsedTags.hoot) delete notUsedTags.hoot; // Added by the UI
    // Debug info. We use this in postprocessing via "tags"
    if (notUsedTags['hoot:id']) delete notUsedTags['hoot:id'];


    // Apply the simple number and text biased rules
    // NOTE: These use the transMap option. This will migrate to the other translations soon
    // NOTE: These delete tags as they are used

    translate.numToOgr(attrs, notUsedTags, dnc.rules.numBiased, dnc.rules.intList, transMap);
    translate.txtToOgr(attrs, notUsedTags, dnc.rules.txtBiased, transMap);

    // Apply one2one rules
    // NOTE: This is a local function updates a structure of what got translated so we can undo it if needed
    // dnc.applyOne2OneModified(notUsedTags, attrs, dnc.lookup, transMap);
    translate.applyOne2One(notUsedTags, attrs, dnc.lookup, dnc.fcodeLookup,transMap);

    // Translate the XXX:2, XXX2, XXX:3 etc attributes
    // Note: This deletes tags as they are used
    translate.fix23Tags(notUsedTags, attrs, dnc.lookup);

    // Post Processing
    // We send the original list of tags and the list of tags we haven't used yet
    // dnc.applyToOgrPostProcessing(tags, attrs, geometryType);
    dnc.applyToOgrPostProcessing(tags, attrs, geometryType, notUsedTags);

    // Now figure out of the tags we used to find the F_CODE are still in the notUsedTags list
    // If so, delete them
    for (var col in notUsedTags)
    {
      var value = notUsedTags[col];
      if (col in dnc.fcodeLookup)
      {
        if (value in dnc.fcodeLookup[col])
        {
          var row = dnc.fcodeLookup[col][value];
          if (attrs.F_CODE == row[1])
          {
            // Debug:
            // print('F_CODE: Dropping ' + col + '=' + value + ' from notUsedTags');
            delete notUsedTags[col];
            break;
          }
        }
      }
    } // End clean notUsedTags

    // Debug
    if (dnc.configOut.OgrDebugDumptags == 'true') translate.debugOutput(notUsedTags,'',geometryType,elementType,'Not used: ');

    // Now check for invalid feature geometry
    // E.g. If the spec says a runway is a polygon and we have a line, throw error and
    // push the feature to o2s layer
    var gFcode = attrs.F_CODE + geometryType.toString().charAt(0);

    // Debug:
    // print('gFcode: ' + gFcode + '  layerName: ' + dnc.rules.layerList[gFcode]);

    if (gFcode in dnc.rules.layerList)
    {
      // Check if we need to make more features
      // NOTE: This returns structure we are going to send back to Hoot:  {attrs: attrs, tableName: 'Name'}
      returnData = dnc.manyFeatures(geometryType,tags,attrs,transMap);

      // Debug: Add the first feature
      //returnData.push({attrs: attrs, tableName: ''});
      // print('ReturnData.length = ' + returnData.length);

      // Now go through the features and clean them up
      var gType = geometryType.toString().charAt(0);
      for (var i = 0, fLen = returnData.length; i < fLen; i++)
      {
        // var gFcode = returnData[i]['attrs']['F_CODE'] + gType;

        var tableName = '';
        tableName = dnc.rules.layerList[returnData[i]['attrs']['F_CODE'] + gType];

        // if (dncAttrLookup[gFcode.toUpperCase()])
        if (tableName !== '')
        {
          // If we have already set the tablename, don't stomp on it
          if (returnData[i]['tableName'] == '') returnData[i]['tableName'] = tableName;

          // This is UGLY
          // FA000 appears in two layers. Both with different attribute lists
          if (returnData[i]['attrs']['F_CODE'] == 'FA000X') returnData[i]['attrs']['F_CODE'] = 'FA000';

          // Validate attrs: remove all that are not supposed to be part of a feature
          dnc.validateAttrs(tableName,geometryType,returnData[i]['attrs'],notUsedTags,transMap);

          // If we have unused tags, add them to the extra field we added to the schema field
          if (Object.keys(notUsedTags).length > 0)
          {
            // var tStr = '<OSM>' + JSON.stringify(notUsedTags) + '</OSM>';
            var tStr = '<OSM>' + JSON.stringify(notUsedTags) + '</OSM>';
            returnData[i]['attrs']['OSM_TAGS'] = tStr;
          }
        }
        else
        {
          // If the feature is not valid, just drop it
          // Debug
          // print('## Skipping: ' + gFcode);
          returnData.splice(i,1);
        }
      } // End returnData loop

      // Look for Review tags and push them to a review layer if found
      if (tags['hoot:review:needs'] == 'yes')
      {
        var reviewAttrs = {};

        // Note: Some of these may be "undefined"
        reviewAttrs.note = tags['hoot:review:note'];
        reviewAttrs.score = tags['hoot:review:score'];
        reviewAttrs.uuid = tags.uuid;
        reviewAttrs.source = tags['hoot:review:source'];

        var reviewTable = 'review_' + geometryType.toString().charAt(0);
        returnData.push({attrs: reviewAttrs, tableName: reviewTable});
      } // End ReviewTags
    } // End We have a feature
    else // We DON'T have a feature
    {
      // For the UI: Throw an error and die if we don't have a valid feature
      if (dnc.configOut.OgrThrowError == 'true')
      {
        if (! attrs.F_CODE)
        {
          returnData.push({attrs:{'error':'No Valid Feature Code'}, tableName: ''});
          return returnData;
        }
        else
        {
          //throw new Error(geometryType.toString() + ' geometry is not valid for F_CODE ' + attrs.F_CODE);
          returnData.push({attrs:{'error':geometryType + ' geometry is not valid for ' + attrs.F_CODE + ' in DNC'}, tableName: ''});
          return returnData;
        }
      }

      // hoot.logTrace('FCODE and Geometry: ' + gFcode + ' is not in the schema');
      hoot.logError('FCODE and Geometry: ' + gFcode + ' is not in the schema');

      tableName = 'o2s_' + geometryType.toString().charAt(0);

      // Debug:
      // Dump out what attributes we have converted before they get wiped out
      if (dnc.configOut.OgrDebugDumptags == 'true') translate.debugOutput(attrs,'',geometryType,elementType,'Converted attrs: ');

      // We want to keep the hoot:id if present
      if (tags['hoot:id'])
      {
        tags.raw_id = tags['hoot:id'];
        delete tags['hoot:id'];
      }

      // If we have stashed anything in the OSM_TAGS, add it back to tags
      if (attrs.OSM_TAGS)
      {
        var tObj = translate.unpackMemo(attrs.OSM_TAGS);
        if (tObj.tags !== '')
        {
          var tTags = JSON.parse(tObj.tags);
          for (var tg in tTags)
          {
            // Debug:
            // if (tags[tg]) print('## Overwriteing tag: ' + tg + ' = ' + tags[tg] + '  with: ' + tTags[tg]);

            tags[tg] = tTags[tg];
          }
        }
      }

      // Convert all of the Tags to a string so we can jam it into an attribute
      // var str = JSON.stringify(tags);
      var str = JSON.stringify(tags,Object.keys(tags).sort());

      // Shapefiles can't handle fields > 254 chars
      // If the tags are > 254 char, split into pieces. Not pretty but stops errors
      // A nicer thing would be to arrange the tags until they fit neatly
      if (str.length < 255 || dnc.configOut.OgrSplitO2s == 'false')
      {
        // return {attrs:{tag1:str}, tableName: tableName};
        attrs = {tag1:str};
      }
      else
      {
        // Not good. Will fix with the rewrite of the tag splitting code
        if (str.length > 1012)
        {
          hoot.logTrace('o2s tags truncated to fit in available space.');
          str = str.substring(0,1012);
        }

        // return {attrs:{tag1:str.substring(0,253), tag2:str.substring(253)}, tableName: tableName};
        attrs = {tag1:str.substring(0,253),
          tag2:str.substring(253,506),
          tag3:str.substring(506,759),
          tag4:str.substring(759,1012)};
      }

      returnData.push({attrs: attrs, tableName: tableName});
    } // End Don't have a feature

    // Debug:
    if (dnc.configOut.OgrDebugDumptags == 'true')
    {
      for (var i = 0, fLen = returnData.length; i < fLen; i++)
      {
        print('TableName ' + i + ': ' + returnData[i]['tableName'] + '  FCode: ' + returnData[i]['attrs']['F_CODE'] + '  Geom: ' + geometryType);
        translate.debugOutput(returnData[i]['attrs'],'',geometryType,elementType,'Out attrs: ');
      }
      print('');
    }

    return returnData;
  } // End of toOgr
}; // End of dnc
