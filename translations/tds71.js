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
 * @copyright Copyright (C) 2022 2023 Maxar (http://www.maxar.com/)
 */

/*
    TDSv71 conversion script
*/

tds71 = {
  // getDbSchema - Load the standard schema or modify it into the TDS structure.
  getDbSchema: function() {
    tds71.layerNameLookup = {}; // <GLOBAL> Lookup table for converting an FCODE to a layername
    tds71.attrLookup = {}; // <GLOBAL> Lookup table for checking what attrs are in an FCODE

    // Warning: This is <GLOBAL> so we can get access to it from other functions
    tds71.rawSchema = tds71.schema.getDbSchema();

    // Add empty "extra" feature layers if needed
    if (hoot.Settings.get('ogr.note.extra') == 'file') tds71.rawSchema = translate.addExtraFeature(tds71.rawSchema);

    // Build the TDS fcode/attrs lookup table. Note: This is <GLOBAL>
    tds71.attrLookup = translate.makeAttrLookup(tds71.rawSchema);
    // Debug:
    // print("tds71.attrLookup");
    // translate.dumpLookup(tds71.attrLookup);
    // print("##########");

    // Code to dump out the F_CODE list
    // var fcList = []
    // tds71.rawSchema.forEach( function (item) {
    //   if (! fcList.includes(item.fcode))
    //   {
    //     fcList.push(item.fcode);
    //   }
    // });
    // print(JSON.stringify(fcList));

    // Code to dump out the FCSUBTYPE list
    // tds71.fcsubList = {}
    // tds71.rawSchema.forEach( function (item) {
    //   if (! tds71.fcsubList[item.fcode])
    //   {
    //     tds71.fcsubList[item.fcode] = item.fcsubtype;
    //   }
    //   else
    //   {
        // if (item.fcsubtype !== tds71.fcsubList[item.fcode]) print('## ' + item.fcode + ' different ' + item.fcsubtype + ' vs ' + tds71.fcsubList[item.fcode]);
    //   }
    // });
    // print(JSON.stringify(tds71.fcsubList));

    // print("##########");

    // Yet Another lookup. FCODE and English name
    // Used for O2S reasons
    tds71.fcodeNameLookup = translate.makeFcodeNameLookup(tds71.rawSchema);
    // print("tds71.fcodeNameLookup");
    // translate.dumpLookup(tds71.fcodeNameLookup);
    // print("##########");

    // Decide if we are going to use TDS structure or 1 FCODE / File
    // if we DON't want the new structure, just return the tds71.rawSchema
    if (hoot.Settings.get('writer.thematic.structure') == 'false')
    {
      // Now build the FCODE/layername lookup table. Note: This is <GLOBAL>
      tds71.layerNameLookup = translate.makeLayerNameLookup(tds71.rawSchema);
      // print("tds71.layerNameLookup");
      // translate.dumpLookup(tds71.layerNameLookup);
      // print("##########");

      // Now add an o2s[A,L,P] feature to the tds71.rawSchema
      // We can drop features but this is a nice way to see what we would drop
      if (hoot.Settings.get('ogr.output.format') == 'shp')
      {
        // Add tag1, tag2, tag3 and tag4
        tds71.rawSchema = translate.addO2sFeatures(translate.addTagFeatures(tds71.rawSchema));
      }
      else
      {
        // Just add tag1 && OSMTAGS
        tds71.rawSchema = translate.addSingleO2sFeature(translate.addSingleTagFeature(tds71.rawSchema));
      }
      // Add the empty Review layers
      tds71.rawSchema = translate.addReviewFeature(tds71.rawSchema);

      // Debugging:
      // translate.dumpSchema(tds71.rawSchema);

      return tds71.rawSchema;
    }


  if (tds71.thematicSchema == undefined) 
  {
    if (hoot.Settings.get('ogr.coded.values') == 'false')
    {
      hoot.require('tds71_thematic_enum_schema');
      tds71.thematicSchema = tds71.thematicSchema.getDbSchema();
    }
    else
    {
      hoot.require('tds71_thematic_schema');
    }
  }
    // Debug:
    // print('New Schema:');
    // translate.dumpSchema(tds71.thematicSchema);
    // print('##########');

    // Until asked to change this, we are only supporting exporting to a "clean" structure with thematic layers.
    if (hoot.Settings.get('ogr.clean.export') !== 'false')
    {
      tds71.thematicSchema.forEach( function(item) {
          newColumns = [];
          for (var i in item.columns)
          {
            // Skip the stuff we don't want
            if (~['image_id','legacy_id','early_date','late_date','img_layer','img_mosaic','PFI'].indexOf(item.columns[i].name)) continue;
            newColumns.push(item.columns[i]);
          }

          delete item.columns;
          item.columns = newColumns;
      });
    }

    // Create a lookup table of thematic attributes. Note this is <GLOBAL>
    if (hoot.Settings.get('ogr.output.format') == 'shp')
    {
      tds71.thematicLookup = translate.makeThematicAttrLookup(translate.addTagFeatures(tds71.thematicSchema));
    }
    else
    {
      tds71.thematicLookup = translate.makeThematicAttrLookup(translate.addSingleTagFeature(tds71.thematicSchema));
    }
    // Debug:
    // print("tds71.thematicLookup");
    // translate.dumpLookup(tds71.thematicLookup);
    // print('##########');

    // Build a lookup table  of F_CODE to Thematic layer names
    tds71.thematicGroupList = {}
    tds71.rawSchema.forEach( function (item) {
      tds71.thematicGroupList['' + item.geom.charAt(0) + item.fcode] = item.thematic;
    });
    // Debug:
    // print("tds71.thematicGroupList");
    // translate.dumpLookup(tds71.thematicGroupList);
    // print("##########");

    // Now add the o2s feature to the tds71.thematicSchema
    // We can drop features but this is a nice way to see what we would drop
    if (hoot.Settings.get('ogr.output.format') == 'shp')
    {
      // Add tag1, tag2, tag3 and tag4
      tds71.thematicSchema = translate.addO2sFeatures(tds71.thematicSchema);
    }
    else
    {
      if (hoot.Settings.get('ogr.clean.export') !== 'false')
      {
        // Just add O2S features
        tds71.thematicSchema = translate.addSingleO2sFeature(tds71.thematicSchema);
      }
      else
      {
        // Add O2S and OSMTAGS
      tds71.thematicSchema = translate.addSingleO2sFeature(tds71.thematicSchema);
      }
    }

    // Add the empty Review layers
    tds71.thematicSchema = translate.addReviewFeature(tds71.thematicSchema);
    // Debug:
    // print('Thematic Schema:');
    // translate.dumpSchema(tds71.thematicSchema);
    // print('##########');

    return tds71.thematicSchema;
  }, // End getDbSchema

  // validateAttrs: Clean up the supplied attr list by dropping anything that should not be part of the
  //                feature, checking values and populateing the OTH field.
  validateAttrs: function(geometryType,attrs,notUsed,transMap) {
    // First, use the lookup table to quickly drop all attributes that are not part of the feature.
    // This is quicker than going through the Schema due to the way the Schema is arranged
    var attrList = tds71.attrLookup[geometryType.toString().charAt(0) + attrs.F_CODE];

    if (attrList == undefined)
    {
      hoot.logDebug('Validate: No attrList for ' + attrs.F_CODE + ' ' + geometryType);
      return
    }

    // Debug: Dump transmap
    // print('\nTransMap: ' + JSON.stringify(transMap));

    // Don't add an FCSUBTYPE attribute if we are not going out via OGR
    if (! attrs.FCSUBTYPE && tds71.configOut.OgrFormat !== '')
    {
      attrs.FCSUBTYPE = tds71.rules.fcSubtypeList[attrs.F_CODE];
    }

    var othList = {};
    if (attrs.OTH)
    {
      othList = translate.parseOTH(attrs.OTH); // Unpack the OTH field
      delete attrs.OTH;
    }

    for (var val in attrs)
    {
      if (attrList.indexOf(val) == -1)
      {
        if (val in othList)
        {
          //Debug:
          // print('Validate: Dropping OTH: ' + val + '  (' + othList[val] + ')');
          delete othList[val];
        }

        hoot.logDebug('Validate: Dropping ' + val + ' = ' + attrs[val] + ' from ' + attrs.F_CODE);
        delete attrs[val];

        if (val in transMap)
        {
          notUsed[transMap[val][1]] = transMap[val][2];
          hoot.logDebug('Validate: Re-Adding ' + transMap[val][1] + ' = ' + transMap[val][2] + ' to notUsed');
        }

        // Since we deleted the attribute, Skip the text check
        continue;
      }

      // Now check the length of the text fields
      // We need more info from the customer about this: What to do if it is too long
      if (val in tds71.rules.txtLength)
      {
        if (attrs[val].length > tds71.rules.txtLength[val])
        {
          // First try splitting the attribute and grabbing the first value
          var tStr = attrs[val].split(';');
          if (tStr[0].length <= tds71.rules.txtLength[val])
          {
            attrs[val] = tStr[0];
          }
          else
          {
            hoot.logDebug('Validate: Attribute ' + val + ' is ' + attrs[val].length + ' characters long. Truncating to ' + tds71.rules.txtLength[val] + ' characters.');
            // Still too long. Chop to the maximum length.
            attrs[val] = tStr[0].substring(0,tds71.rules.txtLength[val]);
          }
        } // End text attr length > max length
      } // End in txtLength
    } // End attrs loop

    // Repack the OTH field
    if (Object.keys(othList).length > 0)
    {
      attrs.OTH = translate.packOTH(othList);
      // Debug:
      // print('New OTH: ' + attrs.OTH);
    }

    // No quick and easy way to do this unless we build yet another lookup table
    var feature = {};

    for (var i=0, sLen = tds71.rawSchema.length; i < sLen; i++)
    {
      if (tds71.rawSchema[i].fcode == attrs.F_CODE && tds71.rawSchema[i].geom == geometryType)
      {
        // print('Got Feature:' + tds71.rawSchema[i].name);
        feature = tds71.rawSchema[i];
        break;
      }
    }

    // Now validate the Enumerated values
    for (var i=0, cLen = feature['columns'].length; i < cLen; i++)
    {
      // Skip non enumeratied attributes
      if (feature.columns[i].type !== 'enumeration') continue;

      var enumName = feature.columns[i].name;

      // Skip stuff that is missing and will end up as a default value
      if (!(attrs[enumName])) continue;

      var attrValue = attrs[enumName];
      var enumList = feature.columns[i].enumerations;
      var enumValueList = [];

      // Pull all of the values out of the enumerated list to make life easier
      for (var j=0, elen = enumList.length; j < elen; j++) {
        // If we don't want coded values, then validate the human-readable corresponding value
        if (tds71.configOut.OgrCodedValues == 'false')
        {
          if (attrValue == enumList[j].value) 
            {
              attrValue = enumList[j].name;
            }
            enumValueList.push(enumList[j].name);
        }
        else
        {
          enumValueList.push(enumList[j].value)
        }
      }

      // If we DONT have the value in the list, add it to the OTH or MEMO field
      if (enumValueList.indexOf(attrValue) == -1)
      {
        var othValStr = '(' + enumName + ':' + attrValue + ')';
        var othVal = '999';

        if (tds71.configOut.OgrCodedValues == 'false') othVal = 'Other'; // change to human-readable value

        // No "Other" value. Push to the Memo field
        if (enumValueList.indexOf(othVal) == -1)
        {
          // Set the offending enumerated value to the default value
          attrs[enumName] = feature.columns[i].defValue;

          hoot.logDebug('Validate: Enumerated Value: ' + attrValue + ' not found in ' + enumName + ' Setting ' + enumName + ' to its default value (' + feature.columns[i].defValue + ')');

          attrs.ZI006_MEM = translate.appendValue(attrs.ZI006_MEM,othValStr,';');
        }
        else
        {
          // Set the offending enumerated value to the "other" value
          attrs[enumName] = othVal;

          hoot.logDebug('Validate: Enumerated Value: ' + attrValue + ' not found in ' + enumName + ' Setting OTH and ' + enumName + ' to Other (999)');

          attrs.OTH = translate.appendValue(attrs.OTH,othValStr,' ');
        }

      } // End attrValue in enumList
      else
      {
        attrs[enumName] = attrValue;
      }

    } // End Validate Enumerations

  }, // End validateAttrs


  // validateThematicAttrs - Clean up the TDS format attrs.  This sets all of the extra attrs to be "undefined"
  validateThematicAttrs: function(gFcode, attrs) {
    var tdsAttrList = tds71.thematicLookup[tds71.thematicGroupList[gFcode]];
    var attrList = tds71.attrLookup[gFcode];

    // Commented this out until we get resolution from QA
    // for (var i = 0, len = tdsAttrList.length; i < len; i++)
    // {
    //   if (attrList.indexOf(tdsAttrList[i]) == -1) attrs[tdsAttrList[i]] = undefined;
    //   //if (attrList.indexOf(tdsAttrList[i]) == -1) attrs[tdsAttrList[i]] = null;
    // }

    // Now fix the Closure attributes
    for (var i in tds71.rules.closureList)
    {
      // If it's in the feature, nothing to do
      if (~attrList.indexOf(i)) continue;

      // If it is in the Thematic feature, just set a default value BUT only if we are not going to the UI
      if (tds71.configOut.OgrFormat !== '' && tds71.configOut.OgrCodedValues && (attrs[tds71.rules.closureList[i][0]] || attrs[tds71.rules.closureList[i][1]]))  
      {
        attrs[i] = '5'; // Closed Interval
      }
      else if (tds71.configOut.OgrFormat !== '' && !tds71.configOut.OgrCodedValues && (attrs[tds71.rules.closureList[i][0]] || attrs[tds71.rules.closureList[i][1]]))
      {
        attrs[i] = 'Closed Interval';
      }
    }

    // Fix default values of closure list attributes
    if (tds71.configOut.OgrCodedValues)
    {
      if (['GB005','GB015','GB045'].includes(attrs['F_CODE']))
      {
        attrs['GSGCHC'] = '-999999';
        attrs['GSGCLC'] = '-999999';
      }
      
      // Custom Physiography rules
      if (['BH150','DB170','DA010'].includes(attrs['F_CODE']))
      {
        attrs['SGCC'] = '-999999';
      }

      // Fix values of attributes that are not associated with subtype
      if (['AP050','AQ040','AP010'].includes(attrs['F_CODE']))
      {
        attrs['SGCC'] = '-999999';
        attrs['RMWC'] = '-999999';
      }
    }
    else 
    {
      if (['GB005','GB015','GB045'].includes(attrs['F_CODE']))
      {
        attrs['GSGCHC'] = 'noInformation';
        attrs['GSGCLC'] = 'noInformation';
      }
        
      // Custom Physiography rules
      if (['BH150','DB170','DA010'].includes(attrs['F_CODE']))
      {
        attrs['SGCC'] = 'noInformation';
      }
  
      // Fix values of attributes that are not associated with subtype
      if (['AP050','AQ040','AP010'].includes(attrs['F_CODE']))
      {
        attrs['SGCC'] = 'noInformation';
        attrs['RMWC'] = 'noInformation';
      }
    }
  }, // End validateThematicAttrs


  // Sort out if we need to return more than one feature.
  // This is generally for Roads, Railways, bridges, tunnels etc.
  manyFeatures: function(geometryType, tags, attrs,transMap)
  {
    // Add the first feature to the structure that we return
    var returnData = [{attrs:attrs, tableName:''}];

    // Quit early if we don't need to check anything. We are only looking at linework
    if (geometryType !== 'Line') return returnData;

    // Only looking at roads & railways with something else tacked on
    if (!(tags.highway || tags.railway)) return returnData;

    // Check the list of secondary/tertiary etc features
    // Need to think about: tags.covered
    if (!(tags.bridge || tags.tunnel || tags.embankment || tags.cutting || tags.ford)) return returnData;

    // We are going to make another feature so copy tags and trash the UUID so it gets a new one
    var newFeatures = [];
    // var newAttributes = {};
    var newAttributes = JSON.parse(JSON.stringify(attrs));
    delete newAttributes.F_CODE;
    var nTags = JSON.parse(JSON.stringify(tags));
    delete nTags.uuid;
    delete nTags['hoot:id'];

    // Now drop the tags that made the FCODE
    switch(attrs.F_CODE)
    {
    case 'AN010': // Railway
    case 'AN050': // Railway Sidetrack
      delete nTags.railway;
      if (tds71.configOut.OgrCodedValues)
      {
        newAttributes.TRS = '12'; // Transport Type = Railway
      }
      else
      {
        newAttributes.TRS = 'Railway';
      }
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
        if (tds71.configOut.OgrCodedValues)
        {
          newAttributes.TRS = '9'; // Transport Type = Pedestrian
        }
        else 
        {
          newAttributes.TRS = 'Pedestrian';
        }
        break;

      default:
        if (tds71.configOut.OgrCodedValues)
        {
          newAttributes.TRS = '13'; // Transport Type = Road
        }
        else 
        {
          newAttributes.TRS = 'Road';
        }
      }
      delete nTags.highway;
      break;

    case 'AQ040': // Bridge
      delete nTags.bridge;
      if (tds71.configOut.OgrCodedValues)
      {
        newAttributes.SBB = '1001'; // Supported By Bridge Span = True
      }
      else 
      {
        newAttributes.SBB = 'True'; // Supported By Bridge Span = True
      }
      break;

    case 'AQ130': // Tunnel
      delete nTags.tunnel;
      if (tds71.configOut.OgrCodedValues)
      {
        newAttributes.CWT = '1001'; // Contained Within Tunnel = True
      }
      else
      {
        newAttributes.CWT = 'True'; // Contained Within Tunnel = True
      }
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
      if (nTags.highway == 'track' && tds71.configOut.OgrCodedValues)
      {
        newAttributes.TRS = '3'; // Cart Track TRS = Automotive
      }
      else if (nTags.highway == 'track' && !tds71.configOut.OgrCodedValues)
      {
        newAttributes.TRS = 'Automotive';
      }
      newFeatures.push({attrs: JSON.parse(JSON.stringify(newAttributes)), tags: JSON.parse(JSON.stringify(nTags))});
      delete nTags.highway;
    }

    if (nTags.cutting)
    {
      newAttributes.F_CODE = 'DB070';
      newFeatures.push({attrs: JSON.parse(JSON.stringify(newAttributes)), tags: JSON.parse(JSON.stringify(nTags))});
      delete nTags.cutting;
    }

    if (nTags.bridge)
    {
      newAttributes.F_CODE = 'AQ040';
      newFeatures.push({attrs: JSON.parse(JSON.stringify(newAttributes)), tags: JSON.parse(JSON.stringify(nTags))});
      delete nTags.bridge;
    }

    if (nTags.tunnel)
    {
      newAttributes.F_CODE = 'AQ130';
      newFeatures.push({attrs: JSON.parse(JSON.stringify(newAttributes)), tags: JSON.parse(JSON.stringify(nTags))});
      delete nTags.tunnel;
    }

    if (nTags.ford)
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

    // Need to think about this
    // if (nTags.covered)
    // {
    //   newAttributes.F_CODE = 'AQ151'; // Arcade
    //   newFeatures.push({attrs: JSON.parse(JSON.stringify(newAttributes)), tags: JSON.parse(JSON.stringify(nTags))});
    //   delete nTags.covered;
    // }

    // Loop through the new features and process them.
    for (var i = 0, nFeat = newFeatures.length; i < nFeat; i++)
    {
      // pre processing
      tds71.applyToOgrPreProcessing(newFeatures[i]['tags'], newFeatures[i]['attrs'], geometryType);

      // apply the simple number and text biased rules
      // Note: These are BACKWARD, not forward!
      translate.numToOgr(newFeatures[i]['attrs'], newFeatures[i]['tags'], tds71.rules.numBiased,tds71.rules.intList,transMap);
      translate.txtToOgr(newFeatures[i]['attrs'], newFeatures[i]['tags'], tds71.rules.txtBiased,transMap);

      // one 2 one - we call the version that knows about OTH fields
      translate.applyTdsOne2One(newFeatures[i]['tags'], newFeatures[i]['attrs'], tds71.lookup, tds71.fcodeLookup,transMap);

      // post processing
      tds71.applyToOgrPostProcessing(newFeatures[i]['tags'], newFeatures[i]['attrs'], geometryType, {});

      returnData.push({attrs: newFeatures[i]['attrs'],tableName: ''});
    }

    return returnData;
  }, // End manyFeatures

  // Doesn't do much but saves typing the same code out a few times in the to TDS Pre Processing
  // NOTE if these are points, we drop the railway/highway tags since we can't make transport features out of these
  fixTransType : function(tags,geometry)
  {
    if (tags.railway)
    {
      tags['transport:type'] = 'railway';
      if (geometry == 'Point') delete tags.railway;
    }
    else if (tags.highway && ['path','pedestrian','steps','trail'].indexOf(tags.highway) > -1)
    {
      tags['transport:type'] = 'pedestrian';
      if (geometry == 'Point') delete tags.highway;
    }
    else if (tags.highway)
    {
      tags['transport:type'] = 'road';
      if (geometry == 'Point') delete tags.highway;
    }
  },

  // Function to drop  default and usless values
  // NOTE: This is also called to remove translated tag values
  dropDefaults: function (feat)
  {
    for (var col in feat)
    {
      // slightly ugly but we would like to account for: 'No Information','noInformation' etc
      // First, push to lowercase
      var attrValue = feat[col].toString().toLowerCase();

      // Get rid of the spaces in the text
      attrValue = attrValue.replace(/\s/g, '');

      // Wipe out the useless values
      if (feat[col] == '' || feat[col] == ' ' || attrValue in tds71.rules.dropList || feat[col] in tds71.rules.dropList)
      {
        // debug: Comment this out to leave all of the No Info stuff in for testing
        // print('Dropping: ' + col + ' = ' + feat[col]);
        delete feat[col];
        continue;
      }
    } // End col in attrs loop

  }, // End dropDefaults

  // Clean up the attributes
  cleanAttrs : function (attrs)
  {
    // Drop the mosaic tag if we don't have an image id
    if (!attrs.image_id || attrs.image_id == 'No Information') delete attrs.img_mosaic;

    // Switch to keep all of the default values. Mainly for the schema switcher
    if (tds71.configIn.ReaderDropDefaults == 'true')
    {
      tds71.dropDefaults(attrs);
    }

    // Backward compatibility
    if (attrs.AEI)
    {
      attrs.image_id = attrs.AEI;
      delete attrs.AEI;
    }

    // This is a handy loop. We use it to:
    // 1) Remove all of the "No Information" and -999999 fields
    // 2) Convert all of the Attrs to uppercase - if needed
    for (var col in attrs)
    {

      // Move noInformation to be Unknown if we are storing it.
      if (attrs[col] == 'noInformation' || attrs[col] == 'noinformation') attrs[col] = 'unknown'

      // Remove attributes with '0' values if they can't be '0'
      if (tds71.rules.noZeroList.indexOf(col) > -1 && attrs[col] == '0')
      {
        delete attrs[col];
        continue;
      }

      // Now see if we need to swap attr names
      if (col in tds71.rules.swapListIn)
      {
        // print('Swapped: ' + tds71.rules.swapListIn[i]); // debug
        attrs[tds71.rules.swapListIn[col]] = attrs[col];
        delete attrs[col];
        continue;
      }

    } // End in attrs loop

    // Drop all of the XXX Closure default values IFF the associated attributes are not set.
    // Doing this after the main cleaning loop so all of the -999999 values are
    // already gone and we can just check for existance.

    for (var i in tds71.rules.closureList)
    {
      if (attrs[i])
      {
        if (attrs[tds71.rules.closureList[i][0]] || attrs[tds71.rules.closureList[i][1]])
        {
          continue;
        }
        else
        {
          delete attrs[i];
        }
      }
    } // End closureList
  }, // End cleanAttrrs

  // #####################################################################################################
  // ##### Start of the xxToOsmxx Block #####
  applyToOsmPreProcessing: function(attrs, layerName, geometryType)
  {
    // Tag retired
    if (tags.controlling_authority)
    {
      tags.operator = tags.controlling_authority;
      delete tags.controlling_authority;
    }

    // Now find an F_CODE
    if (attrs.F_CODE)
    {
      // Drop the the "Not Found" F_CODE. This is from the UI
      if (attrs.F_CODE == 'Not found') delete attrs.F_CODE;
    }
    else if (attrs.FCODE)
    {
      attrs.F_CODE = attrs.FCODE;
      delete attrs.FCODE;
    }
    else
    {
      // Time to find an FCODE based on the filename
      // Funky but it makes life easier
      var llayerName = layerName.toString().toLowerCase();

      for (var row in tds71.rules.fCodeMap)
      {
        for (var val in tds71.rules.fCodeMap[row][1])
        {
          if (llayerName.match(tds71.rules.fCodeMap[row][1][val]))
          {
            attrs.F_CODE = tds71.rules.fCodeMap[row][0];
            break;
          }
        }
      }
    } // End of Find an FCode

    // Convert Rasilway guage widths
    if (attrs.ZI017_GAW)
    {
      attrs.ZI017_GAW = Number(attrs.ZI017_GAW) * 1000; // Convert M to MM
    }
  }, // End of applyToOsmPreProcessing


  // #####################################################################################################
  applyToOsmPostProcessing : function (attrs, tags, layerName, geometryType)
  {
    // Unpack the ZI006_MEM field or OSMTAGS
    if (tags.note || attrs.OSMTAGS)
    {
      var tTags = {};
      var tObj = translate.unpackMemo(tags.note);

      if (tObj.tags !== '')
      {
        try
        {
          tTags = JSON.parse(tObj.tags);
        }
        catch (error)
        {
          hoot.logError('Unable to parse OSM tags in TXT attribute: ' + tObj.tags);
        }
      }

      if (attrs.OSMTAGS)
      {
        var tmp = translate.unpackText(attrs,'OSMTAGS');
        for (var i in tmp)
        {
          if (tTags[i]) hoot.logWarn('Overwriting unpacked tag ' + i + '=' + tTags[i] + ' with ' + tmp[i]);
          tTags[i] = tmp[i];
        }
      }

      // Now add the unpacked tags to the main list
      for (var i in tTags)
      {
        // Debug
        // print('Memo: Add: ' + i + ' = ' + tTags[i]);
        if (tags[tTags[i]]) hoot.logDebug('Unpacking tags, overwriting ' + i + ' = ' + tags[i] + '  with ' + tTags[i]);
        tags[i] = tTags[i];

        // Now check if this is a synonym etc. If so, remove the other tag.
        if (i in tds71.fcodeLookupOut) // tag -> FCODE table
        {
          if (tags[i] in tds71.fcodeLookupOut[i])
          {
            var row = tds71.fcodeLookupOut[i][tags[i]];

            // Now find the "real" tag that comes frm the FCode
            if (row[1] in tds71.fcodeLookup['F_CODE'])
            {
              var row2 = tds71.fcodeLookup['F_CODE'][row[1]];
              // If the tags match, delete it
              if (tags[row2[0]] && (tags[row2[0]] == row2[1]))
              {
                delete tags[row2[0]];
              }
            }
          }
        }
      } // End nTags

      if (tObj.text && tObj.text !== '')
      {
        tags.note = tObj.text;
      }
      else
      {
        delete tags.note;
      }
    } // End process tags.note

    // Ice roads vs highways
    if (attrs.F_CODE == 'AQ075' && !tags.highway) tags.highway = 'road';

    // Roads
    if (attrs.F_CODE == 'AP030' || attrs.F_CODE == 'AQ075') // Road & Ice Road
    {
      // Set a Default: "It is a road but we don't know what it is"
      // tags.highway = 'road';

      // Top level
      if (tags['ref:road:type'] == 'motorway' || tags['ref:road:class'] == 'national_motorway')
      {
        tags.highway = 'motorway';
      }
      else if (tags['ref:road:type'] == 'limited_access_motorway')
      {
        tags.highway = 'trunk';
      }
      else if (tags['ref:road:class'] == 'primary')
      {
        tags.highway = 'primary';
      }
      else if (tags['ref:road:class'] == 'secondary')
      {
        tags.highway = 'secondary';
      }
      else if (tags['ref:road:class'] == 'local')
      {
        if (tags['ref:road:type'] == 'road') // RTY=3
        {
          tags.highway = 'tertiary';
        }
        else
        {
          if (tags.highway !== 'road' && tags.highway !== 'pedestrian') tags.highway = 'unclassified';
        }
      }
      else if (tags['ref:road:type'] == 'pedestrian')
      {
        tags.highway = 'pedestrian';
      }
      else if (tags['ref:road:type'] == 'road') // RTY=3
      {
        tags.highway = 'tertiary';
      }
      else if (tags['ref:road:type'] == 'street') // RTY=4
      {
       if (tags.highway !== 'road') tags.highway = 'unclassified';
      }
      // Other should get picked up by the OTH field
      else if (tags['ref:road:type'] == 'other')
      {
        tags.highway = 'road';
      }
    } // End if AP030

    // Ugly hack to get around a number of conflicts
    if (tags.highway == 'yes') tags.highway = 'road';

    // Remove a default
    if (tags.highway == 'road' && tags['ref:road:class'] == 'local') delete tags['ref:road:class'];

    // New TDSv61 Attribute - ROR (Road Interchange Ramp)
    if (tags.highway && tags.interchange_ramp == 'yes')
    {
      var roadList = ['motorway','trunk','primary','secondary','tertiary'];
      if (roadList.indexOf(tags.highway) !== -1) tags.highway = tags.highway + '_link';
    }

    // Add the LayerName to the source
    if ((! tags.source) && layerName !== '') tags.source = 'tdsv71:' + layerName.toLowerCase();

    if (tds71.osmPostRules == undefined)
    {
      // ##############
      // A "new" way of specifying rules. Jason came up with this while playing around with NodeJs
      //
      // Rules format:  ["test expression","output result"];
      // Note: t = tags, a = attrs and attrs can only be on the RHS
      var rulesList = [
        ['t.barrier == "dragons_teeth" && !(t.tank_trap)','t.barrier = "tank_trap"; t.tank_trap = "dragons_teeth"'],
        ['t["bridge:movable"] && t["bridge:movable"] !== "no" && t["bridge:movable"] !== "unknown"','t.bridge = "movable"'],
        ['t.navigationaid && !(t.aeroway)','t.aeroway = "navigationaid"'],
        ['t.amenity == "stop" && t["transport:type"] == "bus"','t.highway = "bus_stop"'],
        ['t.diplomatic && !(t.amenity)','t.amenity = "embassy"'],
        ['t.boundary == "protected_area" && !(t.protect_class)','t.protect_class = "4"'],
        ['t.bunker_type && !(t.military)','t.military = "bunker"'],
        ['t.defensive','t.man_made = "tower";t["tower:type"] = "defensive"'],
        ['t.cable =="yes" && t["cable:type"] == "power"',' t.power = "minor_line"; delete t.cable; delete t["cable:type"]'],
        ['t.cable =="yes" && t["cable:type"] == "transmission"',' t.power = "line"; delete t.cable; delete t["cable:type"]'],
        ['t.control_tower == "yes" && t.use == "air_traffic_control"','t["tower:type"] = "observation"'],
        ['t.crossing == "tank"','t.highway = "crossing"'],
        //['t.terrain_surface','t.surface = t.terrain_surface; delete t.terrain_surface'],
        ['t.dock && !(t.waterway)','t.waterway = "dock"'],
        ['t.drive_in == "yes"','t.amenity = "cinema"'],
        // ["t['generator:source']","t.power = 'generator'"],
        ['t["glacier:type"] == "icecap" && t.natural == "glacier"','delete t.natural'],
        ['t.golf == "driving_range" && !(t.leisure)','t.leisure = "golf_course"'],
        ['t.historic == "castle" && !(t.ruins) && !(t.building)','t.building = "yes"'],
        //["t.in_tunnel == 'yes' && !(t.tunnel)","t.tunnel = 'yes'; delete t.in_tunnel"],
        ['(t.landuse == "built_up_area" || t.place == "settlement") && t.building','t["settlement:type"] = t.building; delete t.building'],
        ['t.leisure == "stadium"','t.building = "yes"'],
        ['t["material:vertical"]','t.material = t["material:vertical"]; delete t["material:vertical"]'],
        ['t["monitoring:weather"] == "yes"','t.man_made = "monitoring_station"'],
        ['t.natural =="spring" && t["spring:type"] == "spring"','delete t["spring:type"]'],
        //["t.on_bridge == 'yes' && !(t.bridge)","t.bridge = 'yes'; delete t.on_bridge"],
        ['t.public_transport == "station" && t["transport:type"] == "railway"','t.railway = "station"'],
        ['t.public_transport == "station" && t["transport:type"] == "bus"','t.bus = "yes"'],
        ['t.product && t.man_made == "storage_tank"','t.content = t.product; delete t.product'],
        ['t.protect_class && !(t.boundary)','t.boundary = "protected_area"'],
        ['t.pylon =="yes" && t["cable:type"] == "cableway"',' t.aerialway = "pylon"'],
        ['t.pylon =="yes" && t["cable:type"] == "power"',' t.power = "tower"'],
        ['t.sidetrack && !(t.railway)','t.railway = "rail"'],
        ['t.sidetrack && !(t.service)','t.service = "siding"'],
        ['t.social_facility','t.amenity = "social_facility"; t["social_facility:for"] = t.social_facility; t.social_facility = "shelter"'],
        ['t["theatre:type"] && !(t.amenity)','t.amenity = "theatre"'],
        ['t["tower:material"]','t.material = t["tower:material"]; delete t["tower:material"]'],
        ['t["tower:type"] && !(t.man_made)','t.man_made = "tower"'],
        ['t.use == "islamic_prayer_hall" && !(t.amenity)','t.amenity = "place_of_worship"'],
        ['t.water || t.landuse == "basin"','t.natural = "water"'],
        ['t.wetland && !(t.natural)','t.natural = "wetland"'],
        ['t["width:minimum_traveled_way"] && !(t.width)','t.width = t["width:minimum_traveled_way"]']
      ];

      tds71.osmPostRules = translate.buildComplexRules(rulesList);
    }

    // translate.applyComplexRules(tags,attrs,tds71.osmPostRules);
    // Pulling this out of translate
    for (var i = 0, rLen = tds71.osmPostRules.length; i < rLen; i++)
    {
      if (tds71.osmPostRules[i][0](tags)) tds71.osmPostRules[i][1](tags,attrs);
    }

    // ##############

    // Road & Railway Crossings
    // Road/Rail = crossing
    // Road + Rail = level_crossing
    if (tags.crossing_point)
    {
      if (tags['transport:type'] == 'railway')
      {
        tags.railway = 'crossing';

        if (tags['transport:type:2'] == 'road') tags.railway = 'level_crossing';
      }
      else if (tags['transport:type'] == 'road')
      {
        if (tags['transport:type:2'] == 'railway')
        {
          tags.railway = 'level_crossing';
        }
        else
        {
          tags.highway = 'crossing';
        }
      }
    } // End crossing_point


    // Add a building tag to Buildings and Fortified Buildings if we don't have one
    // We can't do this in the funky rules function as it uses "attrs" _and_ "tags"
    if (attrs.F_CODE == 'AH055' && !(tags.building)) tags.building = 'bunker';
    if (attrs.F_CODE == 'AL013' && !(tags.building)) tags.building = 'yes';

    // Fix the building 'use' tag. If the building has a 'use' and no specific building tag. Give it one
    if (tags.building == 'yes' && tags.use)
    {
      if ((tags.use.indexOf('manufacturing') > -1) || (tags.use.indexOf('processing') > -1))
      {
        tags.building = 'industrial';
      }
      // NOTE: Shops have been rempved from "use"
      // Sort out shops
      // else if (hoot.OsmSchema.getTagVertex("shop=" + tags.use).name != "shop=*")
      // {
      //    hoot.warn(hoot.OsmSchema.getTagVertex("shop=" + tags.use));
      //    hoot.warn(hoot.OsmSchema.isAncestor("shop=" + tags.use, "shop"));
      //    hoot.warn(tags.use);
      //    tags.shop = tags.use;
      //    delete tags.use;
      // }

    } // End building & use tags

    // Education:
    if (tags['isced:level'] || tags.use == 'education')
    {
      if (tags.building == 'yes')
      {
        tags.building = 'school';
      }
      else if (tags.facility)
      {
        tags.amenity = 'school';
      }
    }

    if (tags.use == 'vocational_education')
    {
      if (tags.building == 'yes')
      {
        tags.building = 'college';
      }
      else if (tags.facility)
      {
        tags.amenity = 'college';
      }
    }

    // A facility is an area. Therefore "use" becomes "amenity". "Building" becomes "landuse"
    if (tags.facility && tags.use)
    {
      if ((tags.use.indexOf('manufacturing') > -1) || (tags.use.indexOf('processing') > -1))
      {
        tags.man_made = 'works';
      }
    }

    // Additional rules for particular FCODE's
    switch (attrs.F_CODE)
    {
    case undefined: // Break early if no value. Should not get here.....
      break;

    // Fix oil/gas/petroleum fields
    case 'AA052':
      tags.landuse = 'industrial';

      switch (tags.product)
      {
      case undefined:
        break;

      case 'gas':
        tags.industrial = 'gas';
        break;

      case 'petroleum':
        tags.industrial = 'oil';
        break;
      }
      break;

    case 'AA054': // Non-water Well
      if (tags.product)
      {
        tags.substance = tags.product;
        delete tags.product;

        var petroleum = ['gas','liquefied_petroleum_gas_(lpg)','petroleum','coalbed_methane','natural_gas_condensate'];
        if (petroleum.indexOf(tags.substance) > -1) tags.man_made = 'petroleum_well';
      }
      break;

      case 'AF030': // Cooling Tower
        if (!tags['tower:type']) tags['tower:type'] = 'cooling';
        break;

    case 'AF040': // Crane
        if (tags['transport:type'] == 'railway' && !(tags.railway))
        {
          tags.railway = 'rail';
          delete tags['transport:type'];
        }
        if (tags['transport:type'] == 'road' && !(tags.highway))
        {
          tags.highway = 'road';
          delete tags['transport:type'];
        }
      break;

      case 'AK030': // Amusement Park
        // F_CODE translation == tourism but FFN translation could be leisure
        // E.g. water parks
        if (tags.leisure && tags.tourism) delete tags.tourism;
      // Remove a default
      if (tags.use == 'recreation') delete tags.use;        break;

    // Fix up landuse tags
    case 'AL020':
      switch (tags.use) // Fixup the landuse tags
      {
      case undefined: // Break early if no value
        break;

      case 'commercial':
        tags.landuse = 'commercial';
        delete tags.use;
        break;

      case 'industrial':
        tags.landuse = 'industrial';
        delete tags.use;
        break;

      case 'residential':
        tags.landuse = 'residential';
        delete tags.use;
        break;
      } // End switch
      break;

    case 'AL170': // Plaza
      // Pedestrian areas go back to being Highway features.
      if (tags.highway == 'pedestrian') delete tags.landuse;
      break;

    case 'AL270': // Industrial Farm
      if (tags.trees == 'coffea_plants') tags.landuse = 'orchard';
      if (tags.trees == 'tea_plants') tags.landuse = 'orchard';
      break;

    case 'AN010': // Railway
      if (tags['railway:track'] == 'monorail')
      {
        tags.railway = 'monorail';
        delete tags['railway:track'];
      }
      break;

    // case 'AP010': // Track
    // case 'AP050': // Trail
    //     tags.seasonal = 'fair';
    //     break;

      // Add defaults for common features
    case 'AP020':
      if (! tags.junction) tags.junction = 'yes';
      break;

    case 'AQ040':
      if (! tags.bridge) tags.bridge = 'yes';
      break;

    case 'AQ125': // Transportation Station
    //   if (tags.amenity == 'ferry_terminal')
    //   {
    //     tags['transport:type'] = 'maritime';
    //     delete tags.bus;
    //   }
      if (!tags.amenity && (tags['transport:type'] == 'road' && tags.use == 'road_transport'))
      {
        tags.amenity = 'bus_station';
        delete tags['transport:type'];
        delete tags.use;
      }
      break;

    case 'BH135': // Ricefield
      tags.landuse = 'farmland';
      break;

    case 'BA040': // Tidal Water
      tags.natural = 'water';
      break;

    case 'BB005': // Harbour
      if (geometryType == 'Area')
      {
        tags.area = 'yes';
      }
      break;

    case 'BH082': // Inland Water
      // This leaves us with just "natural=water"
      if (tags.water == 'undifferentiated_water_body') delete tags.water;
      break;

    case 'BH140': // River
      if (tags['channel:type'] == 'normal') delete tags['channel:type']; // Default value
      if (tags.tidal == 'no') delete tags.tidal; // Default value

      // Different translation for area rivers
      if (geometryType == 'Area')
      {
        if (!tags.natural) tags.natural = 'water';
        if (!tags.water) tags.water = 'river';
        delete tags.waterway;
        break;
      }
      if (geometryType == 'Line')
      {
        if (tags.natural == 'water') delete tags.natural;
        if (tags.water == 'river') delete tags.water;
      }
      break;

    case 'DA010': // Soil Surface Region
      if (tags.natural && (tags.natural == tags.terrain_surface))
      {
        delete tags.geological;  // The natural tag is the better one to use
        delete tags.terrain_surface; // Implied value: natural=sand -> terrain_surface=sand
      }

      if (tags.terrain_surface && !tags.surface)
      {
        tags.surface = tags.terrain_surface;
        delete tags.terrain_surface;
      }
      break;

    case 'EA010': // Crop Land
      // Coffee is an orchard according to OSM. Tea is as well but GGDM calls it Crop Land
      // Or, Industrial Farm
      if (tags.trees == 'tea_plants') tags.landuse = 'orchard';
      if (tags.trees == 'coffea_plants') tags.landuse = 'orchard';
      if (tags.crop == 'other' && tags.trees) delete tags.crop;
      break;

    case 'EA031': // Botanic Garden
      if (! tags.leisure) tags.leisure = 'garden';
      break;

    case 'EA040': // Orchard
      // Crop vs Trees
      if (tags.crop == 'other' && tags.trees) delete tags.crop;
      break;

    case 'EA050': // Vineyard
      // Landuse = vineyard implies crop=grape
      if (tags.crop == 'grape') delete tags.crop;
      break;

    case 'EA055': // Hop Field
      tags.landuse = 'farmland';

      // Cropland vs Orchard
      if (tags.trees == 'hop_plants' && tags.crop)
      {
        delete tags.crop;
        tags.landuse = 'orchard';
      }
      break;

    case 'EC015': // Forest
      if (geometryType == 'Line')
      {
        delete tags.landuse; // Default EC015 translation
        tags.natural = 'tree_row';
      }
      break;

    case 'ED020': // Swamp
      if (tags['vegetation:type'] == 'mangrove')
      {
        tags.wetland = 'mangrove';
        delete tags['vegetation:type'];
      }
      break;

    case 'FA012': // Contaminated Area
    case 'AL065': // Minefield
      if (! tags.boundary) tags.boundary = 'hazard';
      break;
    } // End switch F_CODE

    // Fix up lifestyle tags.
    // This needs to be expanded to handle all of the options.
    //      ['PCF','1','condition','construction'], // Construction
    //      ['PCF','2','condition','functional'], // Intact in spec, using for MGCP compatibility
    //      ['PCF','3','condition','abandoned'], // Unmaintained in spec
    //      ['PCF','4','condition','damaged'], // Damaged
    //      ['PCF','5','condition','dismantled'], // Dismantled
    //      ['PCF','6','condition','destroyed'], // Destroyed
    if (tags.condition)
    {
      if (tags.condition == 'construction')
      {
        // if (tags.highway && attrs.F_CODE == 'AP030')
        if (tags.highway)
        {
          tags.construction = tags.highway;
          tags.highway = 'construction';
          delete tags.condition;
        }
        else if (tags.railway)
        {
          tags.construction = tags.railway;
          tags.railway = 'construction';
          delete tags.condition;
        }
      } // End Construction

    } // End Condition tags

    // Not sure about adding a Highway tag to this.
    // if (attrs.F_CODE == 'AQ040' && !(tags.highway)) tags.highway = 'yes';

    // Denominations without religions - from ZI037_REL which has some denominations as religions
    switch (tags.denomination)
    {
      case undefined:
        break;

      case 'roman_catholic':
      case 'orthodox':
      case 'protestant':
      case 'chaldean_catholic':
      case 'nestorian': // Not sure about this
        tags.religion = 'christian';
        break;

      case 'shia':
      case 'sunni':
        tags.religion = 'muslim';
        break;
      } // End switch

    // Religious buildings: Church, Pagoda, Temple etc
    if (attrs.ZI037_REL && attrs.ZI037_REL !== '14' && tags.amenity !== 'place_of_worship')
    {
      tags.amenity = 'place_of_worship';
    }

    // Fords and Roads
    // Putting this on hold
    // if (attrs.F_CODE == 'BH070' && !(tags.highway)) tags.highway = 'road';
    // if ('ford' in tags && !(tags.highway)) tags.highway = 'road';

    // Fix up areas
    // The thought is: If Hoot thinks it's an area but OSM doesn't think it's an area, make it an area.
    if (geometryType == 'Area' && ! translate.isOsmArea(tags))
    {
      // Debug
      // print('Adding area=yes');
      tags.area = 'yes';
    }

    // Fix the ZI020_GE4X Values
    var ge4meta = ['addr:country','addr:country:second','addr:country:third','addr:country:fourth'];

    for (var i=0, iLen=ge4meta.length; i < iLen; i++)
    {
      if (tags[ge4meta[i]])
      {
        var country = translate.findCountryCode('c2',tags[ge4meta[i]]);
        if (country !== '')
        {
          tags[ge4meta[i]] = country;
        }
        else
        {
          hoot.logWarn('Dropping invalid ' + ge4meta[i] + ' value: ' + tags[ge4meta[i]]);
          delete tags[ge4meta[i]];
        }
      }
    } // End for GE4 loop

    // Clean up metadata - the many to one issue
    switch(tags['source:datetime'])
    {
      case undefined:
        break;

      // These are all mapped to ZI009_SDV on export.
      case tags['source:imagery:datetime']:
      case tags['source:date']:
      case tags['source:geometry:date']:
        delete tags['source:datetime'];
        break;

    } // End source:datetime

  }, // End of applyToOsmPostProcessing

  // ##### End of the xxToOsmxx Block #####

  // #####################################################################################################

  // ##### Start of the xxToOgrxx Block #####

  applyToOgrPreProcessing: function(tags, attrs, geometryType)
  {
    // Remove Hoot assigned tags for the source of the data
    delete tags['source:ingest:datetime'];
    delete tags['error:circular'];
    delete tags['hoot:status'];

    // Dropping 'source:datetime' if we have an imagery date.
    // Usually, this is added by Hoot on import
    if (tags['source:datetime'] && tags['source:imagery:datetime'])
    {
      delete tags['source:datetime'];
    }

    // Stop this tag getting through the translation
    if (tags['source:imagery'] == 'unknown' || tags['source:imagery'] == 'Unknown') delete tags['source:imagery'];

    // Unpack othertags if required
    translate.unpackOtherTags(tags);

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
      if ((i.indexOf('abandoned:') == 0) || (i.indexOf('disused:') == 0))
      {
        var tTag = i.replace('abandoned:','').replace('disused:','');
        tags[tTag] = tags[i];
        tags.condition = 'abandoned';
        delete tags[i];
        continue;
      }

      // Convert "demolished:XXX" features
      if (i.indexOf('demolished:') == 0)
      {
        var tTag = i.replace('demolished:','');
        tags[tTag] = tags[i];
        tags.condition = 'dismantled';
        delete tags[i];
        continue;
      }

      // Convert "construction:XXX" features
      if (i.indexOf('construction:') == 0)
      {
        var tTag = i.replace('construction:','');
        tags[tTag] = tags[i];
        tags.condition = 'construction';
        delete tags[i];
        continue;
      }
    } // End Cleanup loop

    // Fix Bunkers. Putting this first to skip the building=* rules
    if (tags.building == 'bunker')
    {
      tags.military = 'bunker';
      delete tags.building;
    }

    // Fortified buildings vs Surface Bunkers
    if (tags.military == 'bunker')
    {
      // Making a guess that these are military...
      // if (! tags.operator) tags.operator = 'military';

      if (tags['bunker_type'] == 'munitions')
      {
        attrs.F_CODE = 'AM060'; // Surface Bunker
        attrs.PPO = '3'; // Ammunition
        delete tags.military;
        delete tags['bunker_type'];
      }
    }

    // Lifecycle tags
    var cycleList = {'highway':'road','bridge':'yes','railway':'rail','building':'yes'};
    for (var typ in cycleList)
    {
      switch (tags[typ])
      {
      case undefined: // Break early if no value
        break;

      case 'construction':
        if (tags.construction)
        {
          tags[typ] = tags.construction;
          delete tags.construction;
        }
        else
        {
          tags[typ] = cycleList[typ];
        }
        tags.condition = 'construction';
        break;

      case 'proposed':
        if (tags.proposed)
        {
          tags[typ] = tags.proposed;
          delete tags.proposed;
        }
        else
        {
          tags[typ] = cycleList[typ];
        }
        tags.condition = 'proposed';
        break;

      case 'abandoned':
      case 'disused':
        tags[typ] = cycleList[typ];
        tags.condition = 'abandoned';
        break;

      case 'destroyed':
        tags[typ] = cycleList[typ];
        tags.condition = 'destroyed';
        break;

      case 'demolished':
        tags[typ] = cycleList[typ];
        tags.condition = 'dismantled';
        break;
      }
    } // End cycleList

    // Some highway cleanup
    switch (tags.highway)
    {
      case undefined:
        break;

      case 'road':
        tags.highway = 'yes';
        break;

      case 'bus_stop':
        tags["transport:type"] = 'bus';
        break;

      case 'crossing':
      case 'give-way':
      case 'stop':
        // Special type of crossing
        if (tags.crossing == 'tank')
        {
          attrs.F_CODE = 'AP056';
          break;
        }
        tags['transport:type'] = 'road';
        attrs.F_CODE = 'AQ062';
        delete tags.highway;
        break;

      case 'mini_roundabout':
        tags.junction = 'roundabout';
        break;
        // ['t.highway == "steps"','t.highway = "footway"'],

      case 'pedestrian':
        if (tags.area == 'yes')
        {
          attrs.F_CODE = 'AL170'  // Plaza vs Road
        }
        else
        {
          attrs.F_CODE = 'AP030'
        }
        break;

    } // End Highway cleanup

    // Ice roads are a special case.
    if (tags.ice_road == 'yes')
    {
      attrs.F_CODE = 'AQ075';
      if (tags.highway == 'road') delete tags.highway;
    }

    if (tds71.tdsPreRules == undefined)
    {
      // See ToOsmPostProcessing for more details about rulesList.
      var rulesList = [
        // ['t.amenity == "marketplace"  && !(t.building)','t.facility = "yes"'],
        ['t.barrier == "tank_trap" && t.tank_trap == "dragons_teeth"','t.barrier = "dragons_teeth"; delete t.tank_trap'],
        ['t.boundary == "hazard" && t.hazard','delete t.boundary'],
        ['t.communication == "line"','t["cable:type"] = "communication"'],
        ['t.content && !(t.product)','t.product = t.content; delete t.content'],
        ['t.control_tower && t.man_made == "tower"','delete t.man_made'],
        ['t.crossing == "tank" && t.highway == "crossing"','delete t.highway'],
        ['t.diplomatic && t.amenity == "embassy"','delete t.amenity'],
        ['t.dock && t.waterway == "dock"','delete t.waterway'],
        ['t.golf == "driving_range" && t.leisure == "golf_course"','delete t.leisure'],
        // ['t.highway == "steps"','t.highway = "footway"'],
        ['t.historic == "castle" && t.building','delete t.building'],
        ['t.historic == "castle" && t.ruins == "yes"','t.condition = "destroyed"; delete t.ruins'],
        ['t.landcover == "snowfield" || t.landcover == "ice-field"','a.F_CODE = "BJ100"'],
        ['t.leisure == "recreation_ground"','t.landuse = "recreation_ground"; delete t.leisure'],
        ['t.leisure == "sports_centre"','t.facility = "yes"; t.use = "recreation"; delete t.leisure'],
        ['t.leisure == "stadium" && t.building','delete t.building'],
        ['t.launch_pad','delete t.launch_pad; t.aeroway="launchpad"'],
        ['t.man_made && t.building == "yes"','delete t.building'],
        ['t.man_made == "embankment"','t.embankment = "yes"; delete t.man_made'],
        ['t.man_made == "launch_pad"','delete t.man_made; t.aeroway="launchpad"'],
        ['t.median == "yes"','t.is_divided = "yes"'],
        ['t.military == "barracks"','t.use = "dormitory"'],
        ["t.military == 'bunker' && t.building == 'bunker'","delete t.building"],
        ['t.natural == "desert" && t.surface','t.terrain_surface = t.surface; delete t.surface'],
        ['t.natural == "sinkhole"','a.F_CODE = "BH145"; t["water:sink:type"] = "sinkhole"; delete t.natural'],
        ['t.natural == "spring" && !(t["spring:type"])','t["spring:type"] = "spring"'],
        ['t.natural == "wood"','t.landuse = "forest"; delete t.natural'],
        ['t.power == "pole"','t["cable:type"] = "power"; t["tower:shape"] = "pole"'],
        ['t.power == "tower"','t["cable:type"] = "power"; t.pylon = "yes"; delete t.power'],
        ['t.power == "line"','t["cable:type"] = "transmission"; t.cable = "yes"; delete t.power'],
        ['t.power == "minor_line"','t["cable:type"] = "power"; t.cable = "yes"; delete t.power'],
        ['t.rapids == "yes"','t.waterway = "rapids"; delete t.rapids'],
        ['t.railway == "station"','t.public_transport = "station";  t["transport:type"] = "railway"'],
        ['t.railway == "level_crossing"','t["transport:type"] = "railway";t["transport:type:2"] = "road"; a.F_CODE = "AQ062"; delete t.railway'],
        ['t.railway == "crossing"','t["transport:type"] = "railway"; a.F_CODE = "AQ062"; delete t.railway'],
        ['t.resource','t.raw_material = t.resource; delete t.resource'],
        ['(t.shop || t.office) && !(t.facility) && !(t.building) && !(t.amenity == "fuel")','a.F_CODE = "AL013"'],
        ['t.amenity == "fuel" && t.building == "yes"','a.F_CODE = "AL013"'],
        ['t.social_facility == "shelter"','t.social_facility = t["social_facility:for"]; delete t.amenity; delete t["social_facility:for"]'],
        ['t["tower:type"] == "minaret" && t.man_made == "tower"','delete t.man_made'],
        ['t.tunnel == "building_passage"','t.tunnel = "yes"'],
        ['t.use == "islamic_prayer_hall" && t.amenity == "place_of_worship"','delete t.amenity'],
        ['t.wetland && t.natural == "wetland"','delete t.natural'],
        ['t.water == "river"','t.waterway = "river"'],
        ['t.waterway == "riverbank"','t.waterway = "river"'],
        ['t.waterway == "vanishing_point" && t["water:sink:type"] == "sinkhole"','t.natural = "sinkhole"; delete t.waterway; delete t["water:sink:type"]']
      ];

      tds71.tdsPreRules = translate.buildComplexRules(rulesList);
    }

    // Apply the rulesList.
    // translate.applyComplexRules(tags,attrs,tds71.tdsPreRules);
    // Pulling this out of translate
    for (var i = 0, rLen = tds71.tdsPreRules.length; i < rLen; i++)
    {
      if (tds71.tdsPreRules[i][0](tags)) tds71.tdsPreRules[i][1](tags,attrs);
    }

    // Fix Keeps and Martello Towers
    if (tags.defensive)
    {
      tags.military = 'bunker';
      delete tags['tower:type'];
      delete tags.man_made;
    }

    // Aircraft Shelters
    if (tags.bunker_type == 'hardened_aircraft_shelter') attrs.F_CODE = 'GB250';

    // Deconflict towers
    if (tags.power == 'tower')
    {
      delete tags.power;
      tags.pylon = 'yes';
      if (!tags['cable:type']) tags['cable:type'] = 'power';
    }

    // Towers
    if (tags['tower:type'] && tags.man_made == 'tower')
    {
      switch(tags['tower:type'])
      {
        case 'minaret':
          // Debug
          // print('Got Minaret');
          delete tags.man_made;
          break;

        case 'cooling':
          delete tags.man_made;
          break;

        case 'light':
          delete tags.man_made;
          break;
      }
    }

    // Wind Turbines, Solar Panels etc  vs power plants
    if (tags['generator:source'] && tags.power == 'generator')
    {
      delete tags.power;
    }
    else if (tags.power == 'generator')
    {
      attrs.F_CODE = 'AL013';
      tags.use = 'power_generation';
    }

    // Cranes
    if (tags.man_made == 'crane')
    {
      attrs.F_CODE = 'AF040'; // Crane
      if (tags.railway) tags['transport:type'] = 'railway';
      if (tags.highway) tags['transport:type'] = 'road';
    } // End Cranes

  // Fix up bus stations
    if (tags.amenity == 'bus_station' || tags.bus == 'yes')
    {
      if (!attrs.F_CODE) attrs.F_CODE = 'AQ125';
      delete tags.amenity;
      delete tags.bus;
      tags['transport:type'] = 'road'; // Bus is not valid on AQ125
      tags.use = 'road_transport';
    }

    // Fix up OSM 'walls' around facilities
    if ((tags.barrier == 'wall' || tags.barrier == 'fence') && geometryType == 'Area')
    {
      if (tags.landuse == 'military' || tags.military) attrs.F_CODE = 'SU001'; // Military Installation
    }

    // Railways and other features
    if (tags.building == 'yes' && tags.railway == 'rail')
    {
      delete tags.railway;
      attrs.FFN = '490'; // Railway Transport
    }

    // Area Embankments can't be transportation features as well.
    if (tags.embankment == 'yes' && geometryType == 'Area')
    {
      // Hot sure if we should delete highway features as well. Have not seen any in the data so far.
      delete tags.railway;
    }

    // VERY data specific...
    if (tags.covered == 'arcade' && tags.railway) attrs.F_CODE = 'AN010'; // Railway

    // going out on a limb and processing OSM specific tags:
    // - Building == a thing,
    // - Amenity == The area around a thing
    // Note: amenity=place_of_worship is a special case. It _should_ have an associated building tag
    var facilityList = {
      'school':'850','university':'855','college':'857','hospital':'860'
    };

    if (tags.amenity in facilityList)
    {
      if (geometryType == 'Area')
      {
        attrs.F_CODE = 'AL010'; // Facility
      }
      else
      {
        // Make sure we don't turn point facilities into buildings
        if (!(tags.facility = 'yes'))
        {
          // Debug
          // print('Making a building: ' + tags.facility);
          attrs.F_CODE = 'AL013'; // Building
        }
      }

      // If we don't have a Feature Function then assign one.
      if (!(attrs.FFN))
      {
        attrs.FFN = facilityList[tags.amenity];
        // Debug
        // print('PreDropped: amenity = ' + tags.amenity);
        delete tags.amenity;
      }
    }

    // Adding a custom rule for malls to override the rules above.
    // All of the other "shops" are buildings
    if (tags.shop == 'mall') attrs.F_CODE = 'AG030';

    // Churches etc
    if (tags.building && ! tags.amenity)
    {
      var how = [ 'church','chapel','cathedral','mosque','pagoda','shrine','temple',
        'synagogue','tabernacle','stupa'];
      if (how.indexOf(tags.building) > -1)
      {
        tags.amenity = 'place_of_worship';
      }

      var rc = [ 'mission','religious_community','seminary','convent','monastry',
        'noviciate','hermitage','retrest','marabout'];
      if (rc.indexOf(tags.building) > -1)
      {
        tags.use = 'religious_activities';
      }
    }

    // The existence of tidal = yes in combination with a natural = wetland tag should remain
    // in VegetationSrf (was changing to HydrographySrf)
    if (tags.tidal == 'yes' && (tags.natural == 'wetland'))
    {
      attrs.F_CODE = 'ED010';
    }

    if (tags.water == 'tidal')
    {
      tags.tidal = 'yes';
      delete tags.water;
    }

    // Sort out tidal features
    if (tags.tidal && (tags.water || tags.waterway || tags.landuse == 'port'))
    {
      if (tags.tidal == 'yes') attrs.TID = '1001'; // Tidal
      if (tags.tidal == 'no') attrs.TID = '1000'; // non-Tidal

      // Ignore other options
      delete tags.tidal;
    }

    // Fix up water features from OSM
    if (tags.natural == 'water' && !(tags.water || tags.waterway || tags.wetland))
    {
      if (tags.tidal == 'yes')
      {
        attrs.F_CODE = 'BA040';
      }
      else if (geometryType =='Line')
      {
        tags.waterway = 'river';
        attrs.F_CODE = 'BH140';
      }
      else
      {
        tags.water = 'undifferentiated_water_body';
        attrs.F_CODE = 'BH082';
      }
    }

    // Cutlines/Cleared Ways & Highways
    // This might need a cleanup
    if (tags.man_made == 'cutline' && tags.highway)
    {
      if (geometryType == 'Area')
      {
        // Keep the cutline/cleared way
        attrs.F_CODE = 'EC040';

        if (tags.memo)
        {
          tags.memo = tags.memo + ';highway:' + tags.highway;
        }
        else
        {
          tags.memo = 'highway:' + tags.highway;
        }

        delete tags.highway;
      }
      else
      {
        // Drop the cutline/cleared way
        delete tags.man_made;

        if (tags.memo)
        {
          tags.memo = tags.memo + ';cleared_way:yes';
        }
        else
        {
          tags.memo = 'cleared_way:yes';
        }
      }
    }

    // Fix up landuse & AL020
    switch (tags.landuse)
    {
    case undefined: // Break early if no value
      break;

    case 'brownfield':
      tags.landuse = 'soil_surface_region';
      attrs.F_CODE = 'DA010'
      break;

    case 'construction':
      tags.condition = 'construction';
      tags.landuse = 'built_up_area';
      break;

    case 'farm':
    case 'allotments':
    case 'farmland':
      tags.landuse = 'farmland'; // For Farm and Allotments
      switch (tags.crop)
      {
        case undefined: // Break early
          break;

        case 'rice':
          delete tags.landuse; // Ricefield is a different F_CODE
          break;

        case 'tea': // Depreciated tag
          tags.trees = 'tea_plants';
          attrs.F_CODE = 'AL270'; // Industrial Farm
          // tags.landuse = 'plantation'; // Industrial Farm
          delete tags.crop;
          break;

        case 'coffee': // Depreciated Tag
          tags.trees = 'coffea_plants';
          delete tags.crop;
          break;

        case 'fruit_tree':
          tags.landuse = 'orchard';
          break;

        case 'hop':
          attrs.F_CODE = 'EA055'; // Hop Field
          break;
      }
      break;

    case 'farmyard': // NOTE: This is different to farm.
      tags.facility = 'yes';
      tags.use = 'agriculture';
      delete tags.landuse;
      break;

    case 'grass':
      tags.natural = 'grassland';
      tags['grassland:type'] = 'grassland';
      delete tags.landuse;
      break;

    case 'industrial': // Deconflict with AA052 Hydrocarbons Field
      switch (tags.industrial)
      {
      case undefined: // Built up Area
        tags.use = 'industrial';
        tags.landuse = 'built_up_area';
        break;

      case 'oil':
        tags.product = 'petroleum';
        tags.industrial = 'hydrocarbons_field';
        delete tags.landuse;
        break;

      case 'gas':
        tags.product = 'gas';
        tags.industrial = 'hydrocarbons_field';
        delete tags.landuse;
        break;

      case 'hydrocarbons_field':
        delete tags.landuse;
        break;

      case 'refinery':
        delete tags.landuse;
        break;
      }
      break;

    case 'meadow':
      tags.natural = 'grassland';
      tags['grassland:type'] = 'meadow';
      delete tags.landuse;
      break;

    case 'military':
      if (tags.military !== 'range') tags.military = 'installation';
      delete tags.landuse;
      break;

    case 'orchard':
      switch (tags.crop)
      {
        case undefined: // Break early
          break;

        case 'tea': // Depreciated tag
          tags.trees = 'tea_plants';
          delete tags.crop;
          break;

        case 'coffee': // Depreciated Tag
          tags.trees = 'coffea_plants';
          delete tags.crop;
          break;

        case 'hop':
          attrs.F_CODE = 'EA055'; // Hop Field
          break;
      }

      switch (tags.trees)
      {
        case undefined: // Break early
          break;

        case 'tea_plants':
          attrs.F_CODE = 'AL270'; // Industrial Farm
          break;

        case 'coffea_plants':
          attrs.F_CODE = 'EA010'; // Cropland
          break;

        case 'hop_plants':
          attrs.F_CODE = 'EA055'; // Hop Field
          break;
      }
      break;

    case 'railway':
      if (tags['railway:yard'] == 'marshalling_yard') attrs.F_CODE = 'AN060';
      break;

    case 'reservoir':
      tags.water = 'reservoir';
      delete tags.landuse;
      break;

    case 'residential':
      tags.use = 'residential';
      tags.landuse = 'built_up_area';
      break;

    case 'commercial':
      // Skipping since it has it's own F_CODE
      if (geometryType == 'Area') break;
      // Fall through
    case 'retail':
      tags.use = 'commercial';
      tags.landuse = 'built_up_area';
      break;

    case 'rice_field': // Old translation
      if (! tags.crop) tags.crop = 'rice';
      break;

    case 'scrub':
      tags.natural = 'scrub';
      delete tags.landuse;
      break;
    } // End switch landuse

    // Places, localities and regions
    if (tags.place)
    {
      switch (tags.place)
      {
      case 'city':
      case 'town':
      case 'suburb':
      case 'neighbourhood':
      case 'quarter':
      case 'village':
        if (geometryType == 'Point')
        {
          attrs.F_CODE = 'AL020'; // Built Up Area
          delete tags.place;
        }
        break;

      case 'hamlet':
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
        attrs.F_CODE = 'ZD045'; // Annotated Location
        if (tags.memo)
        {
          tags.memo = tags.memo + ';annotation:' + tags.place;
        }
        else
        {
          tags.memo = 'annotation:' + tags.place;
        }
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
            // NOTE: Islands can be Points or Areas
            attrs.F_CODE = 'BA030'; // Island
            delete tags.natural;
          }
        }
        break;
      } // End switch
    }

    // Bridges & Roads.  If we have an area or a line everything is fine.
    // If we have a point, we need to make sure that it becomes a bridge, not a road.
    if (tags.bridge && geometryType =='Point') attrs.F_CODE = 'AQ040';

    // Movable Bridges
    if (tags.bridge == 'movable')
    {
      if (! tags['bridge:movable'])
      {
        	tags['bridge:movable'] = 'unknown';
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

    // Railway sidetracks
    if (tags.railway == 'monorail')
    {
      // This should not be set differently
      attrs.F_CODE = 'AN010';
      tags['railway:track'] = 'monorail';
      delete tags.railway;
    }

    if (tags.service == 'siding' || tags.service == 'spur' || tags.service == 'passing' || tags.service == 'crossover')
    {
      tags.sidetrack = 'yes';
      delete tags.railway;
    }

    // Fix road junctions.
    // TDS has junctions as points. If we can make the feature into a road, railway or bridge then we will
    // If not, it should go to the o2s layer
    if (tags.junction && geometryType !== 'Point')
    {
      if (tags.highway || tags.bridge || tags.railway)
      {
        delete tags.junction;
      }
    } // End AP020 not Point

    // Railway Crossings.
    if (tags.railway == 'crossing_box')
    {
      // Push this to Crossing but try and keep the tags
      attrs.F_CODE = 'AQ062'; // Crossing
    }

    // Cables
    if (tags.man_made == 'submarine_cable')
    {
      delete tags.man_made;
      tags.cable = 'yes';
      tags.location = 'underwater';
    }

    // "service = parking_aisle" is actually the road between rows of car spaces.
    // If this is a line: make it into a road - the default
    // If this is an area: make it into a car park.
    if (tags.service == 'parking_aisle' && geometryType == 'Area')
    {
      delete tags.highway;
      delete tags.service;
      attrs.F_CODE = 'AQ140'; // Vehicle lot / car park
    }

    // Tree rows are a special case for EC015
    if (tags.natural == 'tree_row' && geometryType == 'Line')
    {
      attrs.F_CODE = 'EC015'; // Forest
      delete tags.natural;
    }

    // Now use the lookup table to find an FCODE. This is here to stop clashes with the
    // standard one2one rules
    if (!(attrs.F_CODE) && tds71.fcodeLookup)
    {
      for (var col in tags)
      {
        var value = tags[col];
        if (col in tds71.fcodeLookup && (value in tds71.fcodeLookup[col]))
        {
          var row = tds71.fcodeLookup[col][value];
          attrs.F_CODE = row[1];
          break;
        }
        else if (col in tds71.fcodeLookupOut && (value in tds71.fcodeLookupOut[col]))
        {
          var row = tds71.fcodeLookupOut[col][value];
          attrs.F_CODE = row[1];
          break;
        }
      }
    } // End find F_CODE

    // Some tags imply that they are buildings but don't actually say so.
    // Most of these are taken from raw OSM and the OSM Wiki
    // Not sure if the list of amenities that ARE buildings is shorter than the list of ones that
    // are not buildings.
    // Taking "place_of_worship" out of this and making it a building
    var notBuildingList = [
      'artwork','atm','bbq','bench','bicycle_parking','bicycle_rental','biergarten','boat_sharing','car_sharing',
      'charging_station','clock','compressed_air','dog_bin','dog_waste_bin','drinking_water','emergency_phone',
      'ferry_terminal','fire_hydrant','fountain','game_feeding','grass_strip','grit_bin','hunting_stand','hydrant',
      'life_ring','loading_dock','nameplate','park','parking','parking_entrance','parking_space','picnic_table',
      'post_box','recycling','street_light','swimming_pool','taxi','trailer_park','tricycle_station','vending_machine',
      'waste_basket','waste_disposal','water','water_point','watering_place','yes',
      'fuel' // NOTE: Fuel goes to a different F_CODE
    ]; // End notBuildingList

    if (!attrs.F_CODE && !tags.facility && tags.amenity && !tags.building && (notBuildingList.indexOf(tags.amenity) == -1)) attrs.F_CODE = 'AL013';

    // If we still don't have an FCODE, try looking for individual elements
    if (!attrs.F_CODE)
    {
      var fcodeMap = {
        'highway':'AP030','railway':'AN010','building':'AL013','ford':'BH070',
        'waterway':'BH140','bridge':'AQ040','railway:in_road':'AN010',
        'barrier':'AP040','tourism':'AL013','junction':'AP020',
        'mine:access':'AA010','cutting':'DB070','tomb':'AL036',
        'shop':'AL013','office':'AL013'
      };

      for (var i in fcodeMap)
      {
        if (i in tags)
        {
          attrs.F_CODE = fcodeMap[i];
          break;
        }
      }
    }

    // Soil Surface Regions
    if (!attrs.F_CODE)
    {
      // if (tags.surface)
      // {
      //   attrs.F_CODE = 'DA010'; // Soil Surface Region
      //   if (!tags.material)
      //   {
      //     tags.material = tags.surface;
      //     delete tags.surface;
      //   }
      // }

      switch (tags.natural)
      {
        case 'mud':
        case 'sand':
        case 'bare_rock':
        case 'rock':
        case 'stone':
        case 'scree':
        case 'shingle':
          attrs.F_CODE = 'DA010'; // Soil Surface Region
          if (tags.surface)
          {
            tags.terrain_surface = tags.surface;
            delete tags.surface;
          }
          else
          {
            // Set the TSM type
            tags.terrain_surface = tags.natural;
          }
          break;
      }
    } // End ! F_CODE

    // EE030 Desert is mapped to DA010 Soil Surface Area
    if (tags.natural == 'desert')
    {
      if (!attrs.SWC) attrs.SWC = '1' // Normally Dry
    }

    // Sort out PYM vs ZI032_PYM vs MCC vs VCM - Ugly
    var pymList = [ 'AL110','AL241','AQ055','AQ110','AT042'];

    var vcmList = [ 'AA040','AC020','AD010','AD025','AD030','AD041','AD050','AF010',
      'AF020','AF021','AF030','AF040','AF070','AH055','AJ050','AJ051',
      'AJ080','AJ085','AL010','AL013','AL019','AL080','AM011','AM020',
      'AM030','AM070','AN076','AQ040','AQ045','AQ060','AQ116','BC050',
      'BD115','BI010','BI050','GB230' ];

    if (tags.material)
    {
      if (pymList.indexOf(attrs.F_CODE) !== -1)
      {
        tags['tower:material'] = tags.material;
        delete tags.material;
      }
      else if (vcmList.indexOf(attrs.F_CODE) !== -1)
      {
        tags['material:vertical'] = tags.material;
        delete tags.material;
      }
    }

    // Protected areas have two attributes that need sorting out
    if (tags.protection_object == 'habitat' || tags.protection_object == 'breeding_ground')
    {
      delete tags.protect_class;
    }

    // Split link roads. TDSv61 now has an attribute for this
    //        if (tags.highway && (tags['highway'].indexOf('_link') !== -1))
    //        {
    //            tags.highway = tags['highway'].replace('_link','');
    //            tags.interchange_ramp = 'yes';
    //        }


    // Sort out "construction" etc
    // This gets ugly due to the mix of construction, highway & railway
    //             ["t.highway == 'construction' && t.construction","t.highway = t.construction; t.condition = 'construction'; delete t.construction"],
    //             ["t.railway == 'construction' && t.construction","t.railway = t.construction; t.condition = 'construction'; delete t.construction"],
    //             ["t.highway == 'construction' && !(t.construction)","t.highway = 'road'; t.condition = 'construction'"],
    //             ["t.railway == 'construction' && !(t.construction)","t.railway = 'rail'; t.condition = 'construction'"],
    if (tags.highway == 'construction' || tags.railway == 'construction')
    {
      if (tags.construction)
      {
        tags.condition = 'construction';
        if (tags.railway)
        {
          tags.railway = tags.construction;
        }
        else
        {
          tags.highway = tags.construction;
        }
        delete tags.construction;
      }
      else
      {
        tags.condition = 'construction';
        if (tags.railway)
        {
          tags.railway = 'rail';
        }
        else
        {
          tags.highway = 'road';
        }
      }
    } // End Highway & Railway construction

    // Embankments & Railways
    if (tags.embankment == 'yes' && geometryType == 'Area')
    {
      delete tags.railway;
    }

    // Now set the relative levels and transportation types for various features
    if (tags.highway || tags.railway)
    {
      if (tags.bridge && tags.bridge !== 'no')
      {
        tds71.fixTransType(tags,geometryType);
        tags.location = 'surface';
        tags.layer = '1';
        tags.on_bridge = 'yes';
      }

      if (tags.tunnel && tags.tunnel !== 'no')
      {
        tds71.fixTransType(tags,geometryType);
        // tags.layer = '-1';
        tags.in_tunnel = 'yes';
      }

      if (tags.embankment && tags.embankment !== 'no')
      {
        tds71.fixTransType(tags,geometryType);
        tags.layer = '1';
      }

      if (tags.cutting && tags.cutting !== 'no')
      {
        tds71.fixTransType(tags,geometryType);
        tags.layer = '-1';
      }

      if (tags.ford && tags.ford !== 'no')
      {
        tds71.fixTransType(tags,geometryType);
        tags.location = 'on_waterbody_bottom';
      }

    } // End if highway || railway

    // Debug
    // for (var i in tags) print('End PreProc Tags: ' + i + ': :' + tags[i] + ':');
    // Tag changed

    if (tags.vertical_obstruction_identifier)
    {
      tags['aeroway:obstruction'] = tags.vertical_obstruction_identifier;
      delete tags.vertical_obstruction_identifier;
    }

    // Railway loading things
    if (tags.railway == 'loading')
    {
      if (tags.facility == 'gantry_crane')
      {
        delete tags.railway;
        delete tags.facility;
        attrs.F_CODE = 'AF040'; // Crane
        tags['crane:type'] = 'bridge';
      }

      if (tags.facility == 'container_terminal')
      {
        delete tags.railway;
        delete tags.facility;
        attrs.F_CODE = 'AL010'; // Facility
        attrs.FFN = '480'; // Transportation
      }
    } // End loading


    switch (tags.man_made)
    {
    case undefined: // Break early if no value
      break;

    case 'reservoir_covered':
      delete tags.man_made;
      attrs.F_CODE = 'AM070'; // Storage Tank
      tags.product = 'water';
      break;

    case 'gasometer':
      delete tags.man_made;
      attrs.F_CODE = 'AM070'; // Storage Tank
      tags.product = 'gas';
      break;
    }

    // Stop some Religion tags from stomping on Denomination tags
    if (tags.religion && tags.denomination)
    {
      if (tags.religion == 'christian' || tags.religion == 'muslim')
      {
        switch (tags.denomination)
        {
        case 'roman_catholic':
        case 'orthodox':
        case 'protestant':
        case 'chaldean_catholic':
        case 'nestorian': // Not sure about this
        case 'shia':
        case 'sunni':
          delete tags.religion;
          break;
        } // End switch
      }
    } // End if religion & denomination

    // Names. Sometimes we don't have a name but we do have language ones
    if (!tags.name) translate.swapName(tags);

    // Handle retired country tags
    var ge4meta = {
      'is_in:country_code':'addr:country',
      'country_code:second':'addr:country:second',
      'country_code:third':'addr:country:third',
      'country_code:fourth':'addr:country:fourth'};

    for (var i in ge4meta)
    {
      if (tags[i])
      {
        tags[ge4meta[i]] = tags[i];
        delete tags[i];
      }
    }

    if (tags['is_in:country'] && !tags['addr:country'])
    {
      tags['addr:country'] = tags['is_in:country'];
      delete tags['is_in:country'];
    }

    // Product vs substance vs resource.  Sigh...
    if (!tags.product)
    {
      if (tags.substance)
      {
        tags.product = tags.substance;
        delete tags.substance;
      }
      else if (tags.resource)
      {
        tags.product = tags.resource;
        delete tags.resource;
      }
    }

    // Monorails are a special case
    if (tags.railway == 'monorail')
    {
      tags['railway:track'] = 'monorail';
      tags.railway = 'rail';

      if (tags['railway:type'])
      {
        tags.railway = tags['railway:type'];
        delete tags['railway:type']
      }
    }

    // Vineyards
    if (tags.landuse == 'vineyard')
    {
      // In the spec, this is the _only_ value for crop so we store orig value
      if (tags.crop)
      {
        tags.tcrop = tags.crop;
        delete tags.crop;
      }

      tags.crop = 'grape';
    }

  }, // End applyToOgrPreProcessing

  // #####################################################################################################

  applyToOgrPostProcessing : function (tags, attrs, geometryType, notUsedTags)
  {
    // Shoreline Construction (BB081) covers a lot of features
    if (attrs.PWC) attrs.F_CODE = 'BB081';

    // Inland Water Body (BH082) also covers a lot of features
    if (attrs.IWT && !(attrs.F_CODE)) attrs.F_CODE = 'BH082';

    // The follwing bit of ugly code is to account for the specs haveing two different attributes
    // with similar names and roughly the same attributes. Bleah!
    if (tds71.rules.swapListOut[attrs.F_CODE])
    {
      for (var i in tds71.rules.swapListOut[attrs.F_CODE])
      {
        // print('swap: i ' + i);
        if (i in attrs)
        {
          if (attrs[tds71.rules.swapListOut[attrs.F_CODE][i]])
          {
            attrs[tds71.rules.swapListOut[attrs.F_CODE][i]] = translate.appendValue(attrs[tds71.rules.swapListOut[attrs.F_CODE][i]],';',attrs[i])
          }
          else
          {
            attrs[tds71.rules.swapListOut[attrs.F_CODE][i]] = attrs[i];
          }
          delete attrs[i];
        }
      }
    }

    // Sort out the UUID
    // if (attrs.UFI)
    // {
    //   var str = attrs['UFI'].split(';');
    //   attrs.UFI = str[0].replace('{','').replace('}','');
    // }
    // else if (tags['hoot:id'])
    // {
    //   attrs.UFI = 'raw_id:' + tags['hoot:id'];
    // }
    // else
    // {
    //   if (tds71.configOut.OgrAddUuid == 'true') attrs.UFI = createUuid().replace('{','').replace('}','');
    // }

    // Add Weather Restrictions to transportation features
    if (['AP010','AP030','AP050'].indexOf(attrs.FCODE > -1) && !attrs.ZI016_WTC )
    {
      switch (tags.highway)
      {
      case 'motorway':
      case 'motorway_link':
      case 'trunk':
      case 'trunk_link':
      case 'primary':
      case 'primary_link':
      case 'secondary':
      case 'secondary_link':
      case 'tertiary':
      case 'tertiary_link':
      case 'residential':
      case 'unclassified':
        attrs.ZI016_WTC = '1'; // All weather
        break;

      case 'track':
        attrs.ZI016_WTC = '4'; // Limited All-weather
        break;

      case 'path':
        attrs.ZI016_WTC = '2'; // Fair Weather
        break;
      }

      // Use the road surface to possibly override the classification.
      // We are assumeing that unpaved roads are Fair Weather only
      switch (attrs.ZI016_ROC)
      {
      case undefined: // Break early if no value
        break;

      case '1': // Unimproved
        attrs.ZI016_WTC = '2'; // Fair Weather
        break;

      case '2': // Stabilized Earth
      case '4': // Gravel
        attrs.ZI016_WTC = '4'; // Limited All-weather
        break;

      case '17': // Ice
      case '18': // Snow
        attrs.ZI016_WTC = '3'; // Winter Only
        break;

      case '999': // Other
        attrs.ZI016_WTC = '-999999'; // No Information
        break;

      default:
        attrs.ZI016_WTC = '1'; // All Weather
      }
    }

    // Custom Aeronautic rules
    if (['GB005','GB015','GB045'].includes(attrs.F_CODE))
    {
      attrs.GSGCHC = '-999999';
      attrs.GSGCLC = '-999999';
    }

    // Custom Physiography rules
    if (['BH150','DB170','DA010'].includes(attrs.F_CODE))
    {
      attrs.SGCC = '-999999';
    }

    // Custom Road rules
    // - Fix the "highway=" stuff that cant be done in the one2one rules
    if (attrs.F_CODE == 'AP030' || attrs.F_CODE == 'AQ075') // Road & Ice Road
    {
      // Tag preservation
      if (tags.highway == 'yes') notUsedTags.highway = 'road';

      // If we havent fixed up the road type/class, have a go with the
      // highway tag.
      if (!attrs.RTY && !attrs.RIN_ROI)
      {
        switch (tags.highway)
        {
        case 'motorway':
        case 'motorway_link':
          attrs.RIN_ROI = '2'; // National Motorway
          attrs.RTY = '1'; // Motorway
          break;

        case 'trunk':
        case 'trunk_link':
          attrs.RIN_ROI = '3'; // National/Primary
          attrs.RTY = '2'; // Limited Access Motorway
          break;

        case 'primary':
        case 'primary_link':
          attrs.RIN_ROI = '3'; // National
          attrs.RTY = '3'; // road: Road outside a BUA
          break;

        case 'secondary':
        case 'secondary_link':
          attrs.RIN_ROI = '4'; // Secondary
          attrs.RTY = '3'; // road: Road outside a BUA
          break;

        case 'tertiary':
        case 'tertiary_link':
          attrs.RIN_ROI = '5'; // Local
          attrs.RTY = '3'; // road: Road outside a BUA
          break;

        case 'residential':
        case 'unclassified':
        case 'pedestrian':
        case 'service':
          attrs.RIN_ROI = '5'; // Local
          attrs.RTY = '4'; // street: Road inside a BUA
          break;

        case 'yes':
        case 'road':
          attrs.RIN_ROI = '5'; // Local. Customer requested this translation value
          attrs.RTY = '-999999'; // No Information
        } // End tags.highway switch

      } // End ROI & RIN_ROI

      // Use the Width to populate the Minimum Travelled Way Width - Customer requested
      if (attrs.WID && !(attrs.ZI016_WD1))
      {
        attrs.ZI016_WD1 = attrs.WID;
        delete attrs.WID;
      }

      // Private Access roads - Customer requested
      if (tags.access == 'private' && !(attrs.CAA))
      {
        attrs.CAA = '3';
        delete notUsedTags.access;
      }

      // Fix up RLE
      // If Vertical Relative Location != Surface && Not on a Bridge, Relative Level == NA
      if ((attrs.LOC && attrs.LOC !== '44') && (attrs.SBB && attrs.SBB == '1000')) attrs.RLE = '998';

      // Road condition
      // if (!attrs.PCF) attrs.PCF = '2'; // Intact
    } // End AP030 || AQ075

    // Fix values of attributes that are not associated with subtype
    if (['AP050','AQ040','AP010'].includes(attrs.F_CODE))
    {
      attrs.SGCC = '-999999';
      attrs.RMWC = '-999999';
    }

    // RLE vs LOC: Need to deconflict this for various features.
    // This is the list of features that can be "Above Surface". Other features use RLE (Relative Level) instead.
    if (attrs.LOC == '45' && (['AT005','AQ113','BH065','BH110'].indexOf(attrs.TRS) == -1))
    {
      attrs.RLE = '2'; // Raised above surface
      attrs.LOC = '44'; // On Surface
    }

    // Additional rules for particular FCODE's
    switch (attrs.F_CODE)
    {
      // Amusement Parks
      case 'AK030':
      if (!attrs.FFN) attrs.FFN = '921'; // Recreation
      break;

      case 'AP010': // Cart Track
        if (attrs.TRS && attrs.TRS == '13') attrs.TRS = '3';

        if (attrs.TRS && (['3','4','6','11','21','22','999'].indexOf(attrs.TRS) == -1))
        {
          var othVal = '(TRS:' + attrs.TRS + ')';
          attrs.OTH = translate.appendValue(attrs.OTH,othVal,' ');
          attrs.TRS = '999';
        }
        break;

      case 'BH140': // River
        if (!attrs.WCC) attrs.WCC = '7'; // Normal Channel
        if (!attrs.TID) attrs.TID = '1000'; // Not tidal
        break;
    } // End switch F_CODE

    // Fix HGT and LMC to keep GAIT happy
    // If things have a height greater than 46m, tags them as being a "Navigation Landmark"
    if (attrs.HGT > 46 && !(attrs.LMC)) attrs.LMC = '1001';

    // Alt_Name:  AL020 Built Up Area & ZD070 Water Measurement Location are the _ONLY_ features in TDS
    // that have a secondary name.
    if (attrs.ZI005_FNA2 && (attrs.F_CODE !== 'AL020' && attrs.F_CODE !== 'ZD070'))
    {
      // We were going to push the Alt Name onto the end of the standard name field - ZI005_FNA
      // but this causes problems so until the customer gives us more direction, we are going to drop it.
      // attrs.ZI005_FNA = translate.appendValue(attrs.ZI005_FNA,attrs.ZI005_FNA2,';');
      delete attrs.ZI005_FNA2;
    }

    // Alt_Name2:  ZD070 Water Measurement Location is the _ONLY_ feature in TDS that has a
    // third name.
    if (attrs.ZI005_FNA3 && attrs.F_CODE !== 'ZD070')
    {
      // We were going to push the Alt Name onto the end of the standard name field - ZI005_FNA
      // but this causes problems so until the customer gives us more direction, we are going to drop it.
      // attrs.ZI005_FNA = translate.appendValue(attrs.ZI005_FNA,attrs.ZI005_FNA2,';');
      delete attrs.ZI005_FNA3;
    }

    // The ZI001_SDV (source date time) field can only be 20 characters long. When we conflate features,
    // we concatenate the tag values for this field.
    // We are getting guidance from the customer on what value they would like in this field:
    // * The earliest date/time,
    // * The first on in the list
    // * etc
    //
    // Until we get an answer, we are going to take the first value in the list.
    if (attrs.ZI001_SDV)
    {
      attrs.ZI001_SDV = translate.chopDateTime(attrs.ZI001_SDV);
    }

    if (!attrs.ZI001_SDV)
      attrs.ZI001_SDV = tags['source:imagery:datetime']
        || tags['source:imagery:earliestDate']
        || tags['source:date']
        || tags['source:geometry:date']
        || tags['hoot:timestamp']
        || 'noInformation';

      // Drop the default value so that it doesn't cause issues in the UI
      if (attrs.ZI001_SDV == 'noInformation') delete attrs.ZI001_SDV;

    // Fix the ZI020_GE4X Values
    // NOTE: This is the opposite to what is done in the toOSM post processing
    var ge4attr = ['ZI020_GE4','ZI020_GE42','ZI020_GE43','ZI020_GE44'];
    for (var i=0, iLen=ge4attr.length; i < iLen; i++)
    {
      if (attrs[ge4attr[i]])
      {
        // First, try the 2char country code
        var urn = translate.convertCountryCode('c2','urn',attrs[ge4attr[i]]);

        // If nothing, try searching all of the fields to get a match
        if (urn == '') urn = translate.findCountryCode('urn',attrs[ge4attr[i]]);

        if (urn !== '')
        {
          attrs[ge4attr[i]] = urn;
        }
        else
        {
          hoot.logWarn('Dropping invalid ' + ge4attr[i] + ' value: ' + attrs[ge4attr[i]]);
          delete attrs[ge4attr[i]];
        }
      }
    } // End for GE4 loop


    // Wetlands
    // Normally, these go to Marsh
    switch(tags.wetland)
    {
    case undefined: // Break early if no value
      break;

    case 'mangrove':
      attrs.F_CODE = 'ED020'; // Swamp
      attrs.VSP = '19'; // Mangrove
      delete notUsedTags.wetland;
      break;
    } // End Wetlands

    // Add imagery tags
    if (!attrs.ZI001_SRT)
    {
      // Not sure if this should be a default
      attrs.ZI001_SRT == '160';

      if (notUsedTags['source:imagery']) attrs.ZI001_SRT = '30';

      // This should have already been removed from notUsedTags
      if (tags['source:imagery:sensor'] == 'IK02') attrs.ZI001_SRT = '28';
    }

    if (!attrs.ZI001_SDP && notUsedTags['source:imagery'])
    {
      attrs.ZI001_SDP = notUsedTags['source:imagery'];
      delete notUsedTags['source:imagery'];
    }

    // Custom Railway fixes
    switch (tags.railway)
    {
      case undefined: // Break early
        break;

      case 'halt':
        notUsedTags.railway = 'halt';
        attrs.TRS = '12'; // Railway
        attrs.FFN = '482'; // Station
        break;

      case 'crossing_box':
      case 'engine_shed':
      case 'workshop':
        notUsedTags.railway = tags.railway; // Preserving thisjavascript ~ operator
        break;

      case 'razed':
        attrs.PCF = '5'; // Dismantled
        break;
    }

    // Fix Railway gauges
    if (tags.gauge)
    {
      // First, see if we have a range
      // make sure the gauge is not already a number
      // the tags get modified in this method and a way with both railway and bridge
      // will get sent through this method twice, so the gauge may have already converted to number
      // alternate to "isNaN()" could be "typeof tags['gauge'] === 'string'"
      if (isNaN(tags['gauge']) && ~tags['gauge'].indexOf(';'))
      {
        notUsedTags.gauge = tags.gauge; // Save the raw value
        tags.gauge = tags['gauge'].split(';')[0]; // Grab the first value
      }

      // Handle "standard" text values
      switch (tags.gauge)
      {
        case 'standard':
          tags.gauge = 1435;
          break;

        case 'narrow':
          notUsedTags.gauge = tags.gauge;
          tags.gauge = 0;
          attrs.ZI017_RGC = '2';
          break;

        case 'broad':
          notUsedTags.gauge = tags.gauge;
          tags.gauge = 0;
          attrs.ZI017_RGC = '1';
          break;
      }

      // Now work on the numbers
      var gWidth = parseInt(tags.gauge,10);

      if (!isNaN(gWidth) && gWidth > 0)
      {
        if (gWidth == 1435)
        {
          attrs.ZI017_RGC = '3';
        }
        else if (gWidth < 1435) // Narrow
        {
          attrs.ZI017_RGC = '2';
        }
        else
        {
          attrs.ZI017_RGC = '1';
        }
        attrs.ZI017_GAW = (gWidth / 1000).toFixed(3);  // Convert to Metres
      }
      else  // Not a number, cleanup time
      {
        // Dont use the value, just punt it to the OSMTAGS attribute
        delete attrs.ZI017_GAW;
        notUsedTags.gauge = tags.gauge;
      }
    }

    // Cleanup crop value if applicable
    if (notUsedTags.tcrop)
    {
      notUsedTags.crop = notUsedTags.tcrop;
      delete notUsedTags.tcrop;
    }

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
    if (tds71.configIn == undefined)
    {
      tds71.configIn = {};
      tds71.configIn.OgrAddUuid = hoot.Settings.get('ogr.add.uuid');
      tds71.configIn.OgrCompareOutput = hoot.Settings.get('ogr.compare.output')
      tds71.configIn.OgrDebugAddfcode = hoot.Settings.get('ogr.debug.addfcode');
      tds71.configIn.OgrDebugDumptags = hoot.Settings.get('ogr.debug.dumptags');

      // Get any changes
      tds71.toChange = hoot.Settings.get('schema.translation.override');
    }

    // Moved this so it gets checked for each call
    tds71.configIn.ReaderDropDefaults = hoot.Settings.get('reader.drop.defaults');

    // Debug:
    if (tds71.configIn.OgrDebugDumptags == 'true') translate.debugOutput(attrs,layerName,geometryType,'','In attrs: ');

    // See if we have an o2s_X layer and try to unpack it.
    if (~layerName.indexOf('o2s_'))
    {
      tags = translate.unpackText(attrs,'tag');

      // Throw out the reason for the o2s if it exists
      delete tags.o2s_reason;

      // Add some metadata
      if (!tags.uuid && tds71.configIn.OgrAddUuid == 'true') tags.uuid = createUuid();
      if (!tags.source) tags.source = 'tdsv71:' + layerName.toLowerCase();

      // Debug:
      if (tds71.configIn.OgrDebugDumptags == 'true')
      {
        translate.debugOutput(tags,layerName,geometryType,'','Out tags: ');
        print('');
      }

      return tags;
    } // End layername = o2s_X

    // Set up the fcode translation rules. We need this due to clashes between the one2one and
    // the fcode one2one rules
    if (tds71.fcodeLookup == undefined)
    {
      // Add the FCODE rules for Import
      fcodeCommon.one2one.forEach( function(item) { if (tds71.rules.fcList.includes(item[1])) tds71.rules.fcodeOne2oneIn.push(item); });
      tds71.fcodeLookup = translate.createLookup(tds71.rules.fcodeOne2oneIn);

      // Segregate the "Output" list from the common list. We use this to try and preserve the tags that give a many-to-one
      // translation to an FCode
      tds71.fcodeLookupOut = translate.createBackwardsLookup(tds71.rules.fcodeOne2oneOut);
      // Debug
      // translate.dumpOne2OneLookup(tds71.fcodeLookup);
    }

    if (tds71.lookup == undefined)
    {
      // Setup lookup tables to make translation easier. I'm assumeing that since this is not set, the
      // other tables are not set either.

      // Support Import Only attributes
      tds71.rules.one2one.push.apply(tds71.rules.one2one,tds71.rules.one2oneIn);

      tds71.lookup = translate.createLookup(tds71.rules.one2one);
      // Debug
      // translate.dumpOne2OneLookup(tds71.lookup);
    }

    // Untangle TDS attributes & OSM tags.
    // NOTE: This could get wrapped with an ENV variable so it only gets called during import
    translate.untangleAttributes(attrs,tags,tds71);

    // Clean out the usless values
    // NOTE: Doing this AFTER the untangle since untangle will push attrs to upper case
    tds71.cleanAttrs(attrs);

    // Doing this so we can get better debug output
    var uuidField = 'UFI';
    if (attrs.F_CODE == 'ZI031') uuidField = 'URI';

    if (attrs[uuidField])
    {
      // UUID cleanup
      attrs[uuidField] = attrs[uuidField].toString().toLowerCase();
      if (attrs[uuidField].indexOf('{') == -1) attrs[uuidField] = '{' + attrs[uuidField] + '}';
    }
    else
    {
      if (tds71.configIn.OgrAddUuid == 'true') attrs[uuidField] = createUuid();
    }

    if (tds71.configIn.OgrCompareOutput == 'true') translate.compareOutput(attrs,uuidField,layerName,'toOSM');

    // Debug:
    // if (tds71.configIn.OgrDebugDumptags == 'true')
    // {
    //   translate.debugOutput(attrs,layerName,geometryType,'','Untangle attrs: ');
    //   translate.debugOutput(tags,layerName,geometryType,'','Untangle tags: ');
    // }

    // pre processing
    tds71.applyToOsmPreProcessing(attrs, layerName, geometryType);

    // Use the FCODE to add some tags
    if (attrs.F_CODE)
    {
      // print('F_CODE: ' + attrs.F_CODE);
      var ftag = tds71.fcodeLookup['F_CODE'][attrs.F_CODE];
      if (ftag)
      {
        tags[ftag[0]] = ftag[1];
        // Debug: Dump out the tags from the FCODE
        // print('FCODE: ' + attrs.F_CODE + ' tag=' + ftag[0] + '  value=' + ftag[1]);
      }
      else
      {
        hoot.logTrace('Translation for F_CODE ' + attrs.F_CODE + ' not found');
      }
    }

    // Make a copy of the input attributes so we can remove them as they get translated. Looking at what
    // isn't used in the translation - this should end up empty.
    // not in v8 yet: // var tTags = Object.assign({},tags);
    var notUsedAttrs = (JSON.parse(JSON.stringify(attrs)));
    delete notUsedAttrs.F_CODE;

    // apply the simple number and text biased rules
    // NOTE: We are not using the intList paramater for applySimpleNumBiased when going to OSM.
    translate.numToOSM(notUsedAttrs, tags, tds71.rules.numBiased);
    translate.txtToOSM(notUsedAttrs, tags, tds71.rules.txtBiased);

    // one 2 one
    //translate.applyOne2One(notUsedAttrs, tags, tds71.lookup, {'k':'v'});
    translate.applyOne2OneQuiet(notUsedAttrs, tags, tds71.lookup,{'k':'v'});

    // Translate the XXX2, XXX3 etc attributes
    translate.fix23Attr(notUsedAttrs, tags, tds71.lookup);

    // Crack open the OTH field and populate the appropriate attributes
    // The OTH format is _supposed_ to be (<attr>:<value>) but anything is possible
    if (attrs.OTH) translate.processOTH(attrs, tags, tds71.lookup);

    // post processing
    tds71.applyToOsmPostProcessing(attrs, tags, layerName, geometryType);

    // If we are reading from an OGR source, drop all of the output tags with default values
    if (tds71.configIn.ReaderDropDefaults == 'true')
    {
      tds71.dropDefaults(tags);
    }

    // Debug: Add the FCODE to the tags
    if (tds71.configIn.OgrDebugAddfcode == 'true') tags['raw:debugFcode'] = attrs.F_CODE;

    // Debug:
    if (tds71.configIn.OgrDebugDumptags == 'true')
    {
      translate.debugOutput(notUsedAttrs,layerName,geometryType,'','Not used: ');
      translate.debugOutput(tags,layerName,geometryType,'','Out tags: ');
      print('');
    }

    // Override tag values if appropriate
    translate.overrideValues(tags,tds71.toChange);

    if (tds71.configIn.OgrCompareOutput == 'true')
    {
      var uuidField = 'uuid';
      if (!tags.uuid && tags['source:ref']) uuidField = 'source:ref'

      translate.compareOutput(tags,uuidField,'outputOSM','toOSM');
    }

    return tags;
  }, // End of toOsm


  // This gets called by translateToOGR and is where the main work gets done
  // We get Tags and return Attrs and a tableName
  // This is the main routine to convert _TO_ TDS
  toOgr : function(tags, elementType, geometryType)
  {
    var tableName = ''; // The final table name
    var returnData = []; // The array of features to return
    var transMap = {}; // A map of translated attributes
    attrs = {}; // The output attributes
    attrs.F_CODE = ''; // Initial setup

    // Setup config variables. We could do this in initialize() but some things don't call it :-(
    // Doing this so we don't have to keep calling into Hoot core
    if (tds71.configOut == undefined)
    {
      tds71.configOut = {};
      tds71.configOut.OgrAddUuid = hoot.Settings.get('ogr.add.uuid');
      tds71.configOut.OgrCleanExport = hoot.Settings.get('ogr.clean.export')
      tds71.configOut.OgrCompareOutput = hoot.Settings.get('ogr.compare.output')
      tds71.configOut.OgrDebugDumptags = hoot.Settings.get('ogr.debug.dumptags');
      tds71.configOut.OgrEsriFcsubtype = hoot.Settings.get('ogr.esri.fcsubtype');
      tds71.configOut.OgrFormat = hoot.Settings.get('ogr.output.format');
      tds71.configOut.OgrNoteExtra = hoot.Settings.get('ogr.note.extra');
      tds71.configOut.OgrTextFieldNumber = hoot.Settings.get("ogr.text.field.number");
      tds71.configOut.OgrThematicStructure = hoot.Settings.get('writer.thematic.structure');
      tds71.configOut.OgrCodedValues = hoot.Settings.get('ogr.coded.values');
      tds71.configOut.OgrThrowError = hoot.Settings.get('ogr.throw.error');

      // Get any changes to OSM tags
      // NOTE: the rest of the config variables will change to this style of assignment soon
      tds71.toChange = hoot.Settings.get('schema.translation.override');
    }

    // Check if we have a schema. This is a quick way to workout if various lookup tables have been built
    if (tds71.rawSchema == undefined)
    {
      var tmp_schema = tds71.getDbSchema();
    }

      // Debug
    if (tds71.configOut.OgrDebugDumptags == 'true') translate.debugOutput(tags,'',geometryType,elementType,'In tags: ');

    // The Nuke Option: If we have a relation, drop the feature and carry on
    if (tags['building:part']) return null;

    // The Nuke Option: "Collections" are groups of different feature types: Point, Area and Line.
    // There is no way we can translate these to a single TDS feature.
    if (geometryType == 'Collection') return null;

    // Set up the fcode translation rules. We need this due to clashes between the one2one and
    // the fcode one2one rules
    if (tds71.fcodeLookup == undefined)
    {
      // Add the FCODE rules for Export
      fcodeCommon.one2one.forEach( function(item) { if (tds71.rules.fcList.includes(item[1])) tds71.rules.fcodeOne2oneIn.push(item); });
      tds71.fcodeLookup = translate.createBackwardsLookup(tds71.rules.fcodeOne2oneIn);

      // Segregate the "Output" list from the common list. We use this to try and preserve the tags that give a many-to-one
      // translation to an FCode
      tds71.fcodeLookupOut = translate.createBackwardsLookup(tds71.rules.fcodeOne2oneOut);

    }

    if (tds71.lookup == undefined)
    {
      // Add "other" rules to the one2one
      tds71.rules.one2one.push.apply(tds71.rules.one2one,tds71.rules.one2oneOut);

      tds71.lookup = translate.createBackwardsLookup(tds71.rules.one2one);
      // Debug
      // translate.dumpOne2OneLookup(tds71.lookup);

      // Make the fuzzy lookup table
      tds71.fuzzy = schemaTools.generateToOgrTable(tds71.rules.fuzzyTable);
      // Debug
      // for (var k1 in tds71.fuzzy)
      // {
      //     for (var v1 in tds71.fuzzy[k1])
      //     {
      //         print(JSON.stringify([k1, v1, tds71.fuzzy[k1][v1][0], tds71.fuzzy[k1][v1][1], tds71.fuzzy[k1][v1][2]]));
      //     }
      // }
    } // End tds71.lookup Undefined

    // Doing this early to help the debug output
    var uuidField = 'uuid';
    if (tags['source:ref'])
    {
      uuidField = 'source:ref'
      tags['source:ref'] = tags['source:ref'].toString().toLowerCase().replace('{','').replace('}','')
    }
    else
    {
      if (tags.uuid)
      {
        tags.uuid = tags['uuid'].toString().toLowerCase().replace('{','').replace('}','')
      }
      else
      {
        if (tds71.configOut.OgrAddUuid == 'true') tags.uuid = createUuid().replace('{','').replace('}','');
      }
    }

    if (tds71.configOut.OgrCompareOutput == 'true') translate.compareOutput(tags,uuidField,'inputOSM','toOgr');

    // Override values if appropriate
    translate.overrideValues(tags,tds71.toChange);

    // Pre Processing
    tds71.applyToOgrPreProcessing(tags, attrs, geometryType);

    // Make a copy of the input tags so we can remove them as they get translated. What is left is
    // the not used tags.
    // not in v8 yet: // var tTags = Object.assign({},tags);
    var notUsedTags = (JSON.parse(JSON.stringify(tags)));

    delete notUsedTags.hoot; // Added by the UI
    // Debug info. We use this in postprocessing via "tags"
    // delete notUsedTags['hoot:id']; // Keeping this for a while

    // Apply the simple number and text biased rules
    // NOTE: These are BACKWARD, not forward!
    // NOTE: These delete tags as they are used
    translate.numToOgr(attrs, notUsedTags, tds71.rules.numBiased,tds71.rules.intList,transMap);
    translate.txtToOgr(attrs, notUsedTags, tds71.rules.txtBiased,transMap);

    // Apply the fuzzy rules
    // NOTE: This deletes tags as they are used
    translate.applyOne2OneQuiet(notUsedTags, attrs, tds71.fuzzy,{'k':'v'});

    // Translate the XXX:2, XXX2, XXX:3 etc attributes
    // Note: This deletes tags as they are used
    translate.fix23Tags(notUsedTags, attrs, tds71.lookup);

    // one 2 one: we call the version that knows about the OTH field
    // NOTE: This deletes tags as they are used
    translate.applyTdsOne2One(notUsedTags, attrs, tds71.lookup, tds71.fcodeLookup,transMap);

    // Post Processing.
    // We send the original list of tags and the list of tags we haven't used yet.
    // tds71.applyToOgrPostProcessing(tags, attrs, geometryType);
    tds71.applyToOgrPostProcessing(tags, attrs, geometryType, notUsedTags);

    // Now check for invalid feature geometry
    // E.g. If the spec says a runway is a polygon and we have a line, throw error and
    // push the feature to o2s layer
    var gFcode = geometryType.toString().charAt(0) + attrs.F_CODE;

    if (tds71.attrLookup[gFcode.toUpperCase()])
    {
      // Check if we need to make more features.
      // NOTE: This returns the structure we are going to send back to Hoot:  {attrs: attrs, tableName: 'Name'}
      returnData = tds71.manyFeatures(geometryType,tags,attrs,transMap);

      // Debug: Add the first feature
      //returnData.push({attrs: attrs, tableName: ''});

      // Now go through the features and clean them up.
      var gType = geometryType.toString().charAt(0);
      for (var i = 0, fLen = returnData.length; i < fLen; i++)
      {
        // Make sure that we have a valid FCODE
        var gFcode = gType + returnData[i]['attrs']['F_CODE'];
        if (tds71.attrLookup[gFcode.toUpperCase()])
        {
          // Validate attrs: remove all that are not supposed to be part of a feature
          tds71.validateAttrs(geometryType,returnData[i]['attrs'], notUsedTags,transMap);

          // If we have unused tags, add them to the memo field.
          if (Object.keys(notUsedTags).length > 0 && tds71.configOut.OgrNoteExtra == 'attribute')
          {
            // var tStr = '<OSM>' + JSON.stringify(notUsedTags) + '</OSM>';
            // returnData[i]['attrs']['ZI006_MEM'] = translate.appendValue(returnData[i]['attrs']['ZI006_MEM'],tStr,';');
            var str = JSON.stringify(notUsedTags,Object.keys(notUsedTags).sort());
            if (tds71.configOut.OgrFormat == 'shp')
            {
              // Split the tags into a maximum of 4 fields, each no greater than 225 char long.
              var tList = translate.packText(notUsedTags,tds71.configOut.OgrTextFieldNumber,250);
              returnData[i]['attrs']['OSMTAGS'] = tList[1];
              for (var j = 2, tLen = tList.length; j < tLen; j++)
              {
                // Drop empty values
                if (tList[j] !== "") returnData[i]['attrs']['OSMTAGS' + j] = tList[j];
              }
            }
            else
            {
              returnData[i]['attrs']['OSMTAGS'] = str;
            }
          }

          // If we are using the TDS structure, fill the rest of the unused attrs in the schema
          if (tds71.configOut.OgrThematicStructure == 'true')
          {
            returnData[i]['tableName'] = tds71.thematicGroupList[gFcode];
            tds71.validateThematicAttrs(gFcode, returnData[i]['attrs']);
          }
          else
          {
            returnData[i]['tableName'] = tds71.layerNameLookup[gFcode.toUpperCase()];
          }
        }
        else
        {
          // If the feature is not valid, just drop it
          // Debug
          // print('## Skipping: ' + gFcode);
          returnData.splice(i,1);
          fLen = returnData.length;
        }
      } // End returnData loop

      // If we have unused tags, throw them into the "extra" layer
      if (Object.keys(notUsedTags).length > 0 && tds71.configOut.OgrNoteExtra == 'file')
      {
        var extraFeature = {};
        extraFeature.tags = JSON.stringify(notUsedTags);
        extraFeature.uuid = attrs.UFI;

        var extraName = 'extra_' + geometryType.toString().charAt(0);

        returnData.push({attrs: extraFeature, tableName: extraName});
      } // End notUsedTags

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
    }
    else // We don't have a feature
    {
      // For the UI: Throw an error and die if we don't have a valid feature
      if (tds71.configOut.OgrThrowError == 'true')
      {
        if (! attrs.F_CODE)
        {
          returnData.push({attrs:{'error':'No Valid Feature Code'}, tableName: ''});
        }
        else
        {
          //throw new Error(geometryType.toString() + ' geometry is not valid for F_CODE ' + attrs.F_CODE);
          returnData.push({attrs:{'error':geometryType + ' geometry is not valid for ' + attrs.F_CODE + ' in TDSv71'}, tableName: ''});
        }
        return returnData;
      }

      // Since we are not going to the UI, add the reason for dumping the feature to the list of 
      // tags to help other tools.
      if (! attrs.F_CODE)
      {
        tags.o2s_reason = 'Unable to assign an F_CODE';
      }
      else
      {
        tags.o2s_reason = geometryType + ' geometry is not valid for ' + attrs.F_CODE + ' (' + tds71.fcodeNameLookup[attrs.F_CODE] + ')';
      }

      tableName = 'o2s_' + geometryType.toString().charAt(0);

      // Debug:
      // Dump out what attributes we have converted before they get wiped out
      if (tds71.configOut.OgrDebugDumptags == 'true') translate.debugOutput(attrs,'',geometryType,elementType,'Converted attrs: ');

      // We want to keep the hoot:id if present
      if (tags['hoot:id'])
      {
        tags.raw_id = tags['hoot:id'];
        delete tags['hoot:id'];
      }

      if (tds71.configOut.OgrCompareOutput == 'true') translate.compareOutput(tags,'uuid',tableName,'toOgr');

      // Convert all of the Tags to a string so we can jam it into an attribute
      // var str = JSON.stringify(tags);
      var str = JSON.stringify(tags,Object.keys(tags).sort());

      // Shapefiles can't handle fields > 254 chars.
      // If the tags are > 254 char, split into pieces. Not pretty but stops errors.
      // A nicer thing would be to arrange the tags until they fit neatly
      if (tds71.configOut.OgrFormat == 'shp')
      {
        // Throw a warning that text will get truncated.
        if (str.length > (tds71.configOut.OgrTextFieldNumber * 250)) hoot.logWarn('o2s tags truncated to fit in available space.');

        attrs = {};
        var tList = translate.packText(tags,tds71.configOut.OgrTextFieldNumber,250);
        for (var i = 1, tLen = tList.length; i < tLen; i++)
        {
          attrs['tag'+i] = tList[i];
        }
      }
      else
      {
        attrs = {tag1:str};
      }

      returnData.push({attrs: attrs, tableName: tableName});

    } // End we don't have a feature

    // Debug:
    if (tds71.configOut.OgrDebugDumptags == 'true')
    {
      for (var i = 0, fLen = returnData.length; i < fLen; i++)
      {
        print('TableName ' + i + ': ' + returnData[i]['tableName'] + '  FCode: ' + returnData[i]['attrs']['F_CODE'] + '  Geom: ' + geometryType);
        translate.debugOutput(returnData[i]['attrs'],'',geometryType,elementType,'Out attrs: ');
      }
      print('');
    }

    // Debug:
    if (tds71.configOut.OgrCompareOutput == 'true')
    {
      for (var i = 0, fLen = returnData.length; i < fLen; i++)
      {
        var uuidField = 'UFI';

        // Some of the ResourceSrf features only have a URI
        if (returnData[i]['attrs']['URI']) uuidField = 'URI';

        if (returnData[i]['tableName'].indexOf('o2s_') == -1) translate.compareOutput(returnData[i]['attrs'],uuidField,returnData[i]['tableName'],'toOgr');
      }
    }

    return returnData;

  } // End of toOgr

}; // End of tds71
