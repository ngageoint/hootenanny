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
 * @copyright Copyright (C) 2013, 2014 2023 Maxar (http://www.maxar.com/)
 */

/*
  MGCP conversion script for TRD3 and TRD4
    MGCP [TRD3 | TRD4] -> OSM, and
    OSM -> MGCP TRD4

  Based on TableExample.js script by Jason S.

  Possible attribute values are taken from the MGCP TRD 3.0 and 4.0 specs  with the addition of
  values found in sample data and the DFDD/NFDD v4
*/


mgcp = {
  getDbSchema: function() {
    mgcp.rawSchema = mgcp.schema.getDbSchema(); // This is <GLOBAL> so we can access it from other areas

    // These have been moved to mgcp.rules as static lists
    // FCODE to english layer name lookup that is used for O2S reasons
    // var fcodeNameLookup = translate.makeFcodeNameLookup(mgcp.rawSchema);
    // print("##########");
    // print("fcodeNameLookup");
    // print(JSON.stringify(fcodeNameLookup));
    // print("##########");

    // List of valid MGCP geometry + FCODEs
    // var layerNameLookup = [];
    // mgcp.rawSchema.forEach( function (item) {layerNameLookup.push(item.geom.charAt(0) + item.fcode);});
    // print("##########");
    // print("layerNameLookup");
    // print(JSON.stringify(layerNameLookup));
    // print("##########");

      // Build the FCODE to attr lookup table. Note: This is <GLOBAL>
      mgcp.attrLookup = {};
      mgcp.attrLookup = translate.makeAttrLookup(mgcp.rawSchema);

    // Decide if we are going to use the Thematic structure or 1 FCODE / Layer
    // if we DON't want the thematic structure, just return the mgcp.rawSchema
    if (hoot.Settings.get('writer.thematic.structure') == 'false')
    {
      // Now add an o2s[A,L,P] feature to the mgcp.rawSchema and an attribute to hold OSM tags
      if (hoot.Settings.get('ogr.output.format') == 'shp')
      {
        // Add OSMTAGS1-4 and tag1-4 attributes
        mgcp.rawSchema = translate.addO2sFeatures(translate.addTagFeatures(mgcp.rawSchema));
      }
      else
      {
        // Just add tag1 && OSMTAGS
        mgcp.rawSchema = translate.addSingleO2sFeature(translate.addSingleTagFeature(mgcp.rawSchema));
      }

      // Add empty Review layers
      mgcp.rawSchema = translate.addReviewFeature(mgcp.rawSchema);

      // Add empty "extra" feature layers if needed
      if (hoot.Settings.get('ogr.note.extra') == 'file') mgcp.rawSchema = translate.addExtraFeature(mgcp.rawSchema);

      // Debug
      // translate.dumpSchema(mgcp.rawSchema);
      // print('XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX');

      return mgcp.rawSchema;
    } // End non-thematic

    // Thematic schema
    if (mgcp.thematicSchema == undefined) 
    {
      if (hoot.Settings.get('ogr.coded.values') == 'false')
      {
        hoot.require('mgcp_thematic_enum_schema');
        mgcp.thematicSchema = mgcp.thematicSchema.getDbSchema();
      }
      else
      {
        hoot.require('mgcp_thematic_schema');
      }
    }
    
    // This has been moved to mgcp.rules as a static list
    // Lookup table for F_CODE to Thematic layer name
    // var thematicGroupLookup = {}
    // mgcp.rawSchema.forEach( function (item) {thematicGroupLookup['' + item.geom.charAt(0) + item.fcode] = item.thematic;});
    // print("##########");
    // print("mgcp.thematicGroupLookup");
    // print(JSON.stringify(thematicGroupLookup));
    // print("##########");

    if (hoot.Settings.get('ogr.output.format') == 'shp')
    {
      mgcp.thematicLookup = translate.makeThematicAttrLookup(translate.addTagFeatures(mgcp.thematicSchema));
      mgcp.thematicSchema = translate.addO2sFeatures(mgcp.thematicSchema);
    }
    else
    {
      mgcp.thematicLookup = translate.makeThematicAttrLookup(translate.addSingleTagFeature(mgcp.thematicSchema));
      mgcp.thematicSchema = translate.addSingleO2sFeature(mgcp.thematicSchema);
    }
    // Debug
    // // print("##########");
    // print("thematicLookup");
    // print(JSON.stringify(mgcp.thematicLookup));
    // print("##########");

    // Add empty Review layers
    mgcp.thematicSchema = translate.addReviewFeature(mgcp.thematicSchema);

    // Add empty "extra" feature layers if needed
    if (hoot.Settings.get('ogr.note.extra') == 'file') mgcp.thematicSchema = translate.addExtraFeature(mgcp.thematicSchema);

    // Debug
    // translate.dumpSchema(mgcp.thematicSchema);
    // print('XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX');

    return mgcp.thematicSchema;
  }, // End of getDbSchema


  // validateAttrs: Clean up the supplied attr list by dropping anything that should not be part of the feature
  validateAttrs: function(geometryType,attrs,notUsed,transMap) {
    var attrList = mgcp.attrLookup[geometryType.toString().charAt(0) + attrs.FCODE];

    if (attrList == undefined)
    {
      hoot.logDebug('Validate: No attrList for ' + attrs.FCODE + ' ' + geometryType);
      return
    }

    for (var val in attrs)
    {
      // Is the attribute valid?
      if (attrList.indexOf(val) == -1)
      {
        // Now. Undo the translation if possible
        if (val in transMap)
        {
          notUsed[transMap[val][1]] = transMap[val][2];
          hoot.logDebug('Validate: Re-Adding ' + transMap[val][1] + ' = ' + transMap[val][2] + ' to notUsed');
        }

        hoot.logDebug('Validate: Dropping ' + val + ' from ' + attrs.FCODE);
        delete attrs[val];

        continue;
      }
    } // End val in attrs

    // No quick and easy way to do this. Each feature is stored as an object.
    var feature = {};
    for (var i=0, sLen = mgcp.rawSchema.length; i < sLen; i++)
    {
      if (mgcp.rawSchema[i].name == geometryType.toString().charAt(0) + attrs.FCODE)
      {
        feature = mgcp.rawSchema[i];
        break;
      }
    }

    // If we can't find a feature then print an error and return.
    if (feature == {})
    {
      hoot.logError('Validate: Could not find feature: '+ attrs.FCODE + ' ' + geometryType);
      return;
    }

    // Now look through the attributes and values
    // Yet again, looping through the objects
    for (var i=0, cLen = feature['columns'].length; i < cLen; i++)
    {
      var enumName = feature.columns[i].name;

      // Skip anything we don't have a populated vlaue for
      if (!attrs[enumName]) continue;

      // If applicable, check the length of text fields
      // We need more info from the customer about this: What to do if it is too long
      if (feature.columns[i].type == 'String')
      {
        var maxLength = feature.columns[i].length;
        // if (attrs[enumName].length > mgcp.rules.txtLength[val])
        if (attrs[enumName].length > maxLength)
        {
          // First try splitting the attribute and grabbing the first value
          var tStr = attrs[enumName].split(';');
          // if (tStr[0].length <= mgcp.rules.txtLength[val])
          if (tStr[0].length <= maxLength)
          {
            attrs[enumName] = tStr[0];
          }
          else
          {
            hoot.logDebug('Validate: Attribute ' + val + ' is ' + attrs[val].length + ' characters long. Truncating to ' + maxLength + ' characters.');
            // Still too long. Chop to the maximum length
            attrs[val] = tStr[0].substring(0,maxLength);
          }
        } // End text attr length > max length

        // Skip to the next attribute
        continue;
      } // End String

      // Now check enumerated values
      if (feature.columns[i].type !== 'enumeration') continue;

      // Now make a list of the enumeration values to check what is valid. More loops.
      var attrValue = attrs[enumName];
      var enumValueList = [];
      var enumList = feature.columns[i].enumerations;
      feature.columns[i].enumerations.forEach( function (eValue) { enumValueList.push(eValue.value); });

      for (var j=0, elen = enumList.length; j < elen; j++) {
        // if we don't want coded values, then validate the human-readable corresponding value
        if (mgcp.configOut.OgrCodedValues == 'false')
        {
          if (attrValue == enumList[j].value)
          {
            attrValue = enumList[j].name;
          }
          enumValueList.push(enumList[j].name);
        }
        else
        {
          enumValueList.push(enumList[j].value);
        }
      }

      // Check if it is a valid enumerated value
      if (enumValueList.indexOf(attrValue) == -1)
      {
        hoot.logDebug('Validate: Enumerated Value: ' + attrValue + ' not found in ' + enumName);
        var othVal = '999';

        if (mgcp.configOut.OgrCodedValues == 'false') othVal = 'Other'; // change to human-readable value

        // Do we have an "Other" value?
        if (enumValueList.indexOf(othVal) == -1)
        {
          // No: Set the offending enumerated value to the default value
          attrs[enumName] = feature.columns[i].defValue;
          hoot.logDebug('Validate: Enumerated Value: ' + attrValue + ' not found in ' + enumName + ' Setting ' + enumName + ' to its default value (' + feature.columns[i].defValue + ')');
        }
        else
        {
          // Yes: Set the offending enumerated value to the "other" value
          attrs[enumName] = othVal;
          hoot.logDebug('Validate: Enumerated Value: ' + attrValue + ' not found in ' + enumName + ' Setting ' + enumName + ' to Other (999)');
        }

         // Now. Undo the translation if possible
        if (enumName in transMap)
        {
          notUsed[transMap[enumName][1]] = transMap[enumName][2];
          hoot.logDebug('Validate: Re-Adding ' + transMap[enumName][1] + ' = ' + transMap[enumName][2] + ' to notUsed');
        }
      }
      else
      {
        attrs[enumName] = attrValue;
      }
    } // End enumerations
  }, // End validateAttrs


  // validateThematicAttrs - Clean up the thematic layer attrs.  This sets all of the extra attrs to be "undefined"
  validateThematicAttrs: function(gFcode, attrs) {
    var mgcpAttrList = mgcp.thematicLookup[mgcp.rules.thematicGroupLookup[gFcode]];
    var attrList = mgcp.attrLookup[gFcode];

    for (var i = 0, len = mgcpAttrList.length; i < len; i++)
    {
      if (attrList.indexOf(mgcpAttrList[i]) == -1) attrs[mgcpAttrList[i]] = undefined;
      //if (attrList.indexOf(mgcpAttrList[i]) == -1) attrs[mgcpAttrList[i]] = null;
    }

  }, // End validateThematicAttrs


  // Sort out if we need to return more than one feature
  // This is generally for Roads, Railways, bridges, tunnels etc
  manyFeatures: function(geometryType, tags, attrs, transMap)
  {
    // Add the first feature to the structure that we return
    var returnData = [{attrs:attrs, tableName:''}];

    // Quit early if we don't need to check anything. We are only looking at linework
    if (geometryType !== 'Line') return returnData;
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
        if (mgcp.configOut.OgrCodedValues)
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
          case 'cycleway':
            if (mgcp.configOut.OgrCodedValues)
              {
                newAttributes.TRS = '9'; // Transport Type = Pedestrian
              }
              else 
              {
                newAttributes.TRS = 'Pedestrian';
              }
            break;

          default:
            if (mgcp.configOut.OgrCodedValues)
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
        hoot.logDebug('ManyFeatures: Should not get to here');
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
      mgcp.applyToOgrPreProcessing(newFeatures[i]['tags'], newFeatures[i]['attrs'], geometryType);

      var notUsedTags = (JSON.parse(JSON.stringify(tags)));

      // apply the simple number and text biased rules
      // Note: These are BACKWARD, not forward!
      translate.numToOgr(newFeatures[i]['attrs'], notUsedTags, mgcp.rules.numBiased,mgcp.rules.intList,transMap);
      translate.txtToOgr(newFeatures[i]['attrs'], notUsedTags, mgcp.rules.txtBiased,transMap);

      // one 2 one - we call the version that knows about OTH fields
      translate.applyOne2One(notUsedTags, newFeatures[i]['attrs'], mgcp.lookup, mgcp.fcodeLookup, transMap);

      // post processing
      mgcp.applyToOgrPostProcessing(newFeatures[i]['tags'], newFeatures[i]['attrs'], geometryType,notUsedTags);

      returnData.push({attrs: newFeatures[i]['attrs'],tableName: ''});
    }

    return returnData;
  }, // End manyFeatures


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
      if (feat[col] == '' || feat[col] == ' ' || attrValue in mgcp.rules.dropList || feat[col] in mgcp.rules.dropList)
      {
        // debug: Comment this out to leave all of the No Info stuff in for testing
        // print('Dropping: ' + col + ' = ' + feat[col]);
        delete feat[col];
        continue;
      }
    } // End col in attrs loop

  }, // End dropDefaults

  // Cleanup the attributes
  cleanAttrs: function (attrs)
  {
    // Switch to keep all of the default values. Mainly for the schema switcher
    if (mgcp.configIn.ReaderDropDefaults == 'true')
    {
      mgcp.dropDefaults(attrs);
    }

    // Unit conversion. Some attributes are in centimetres, others in decimetres
    // var unitList = { 'GAW':100,'HCA':10,'WD1':10,'WD2':10,'WD3':10,'WT2':10 };
    for (var col in attrs)
    {
      // Sort out units - if needed
      // if (col in unitList) attrs[col] = attrs[col] / unitList[col];

      // Now see if we need to swap attr names
      if (col in mgcp.rules.swapListIn)
      {
        // print('Swapped: ' + swapList[i]); // debug
        attrs[mgcp.rules.swapListIn[col]] = attrs[col];
        delete attrs[col];
        continue
      }
    } // End col in attrs loop
  }, // End cleanAttrs


  // ##### Start of the xxToOsmxx Block #####
  applyToOsmPreProcessing: function(attrs, layerName, geometryType)
  {
    if (attrs.F_CODE)
    {
      // Drop the the "Not Found" F_CODE. This is from the UI
      // NOTE: We _should_ be getting "FCODE" not "F_CODE" from files/UI
      if (attrs.F_CODE == 'Not found') delete attrs.F_CODE;
    }
    else if (attrs.FCODE)
    {
      // Swap these since the rest of the lookup tables & TDS use F_CODE
      if (attrs.FCODE !== 'Not found') attrs.F_CODE = attrs.FCODE;

      delete attrs.FCODE;
    }
    else
    {
      // Time to find an FCODE based on the filename
      var fCodeMap = [
        ['AA010', ['aa010']], // Extraction Mine
        ['AA012', ['aa012']], // Quarry
        ['AA040', ['aa040']], // Rig
        ['AA050', ['aa050','well_p']], // Well
        ['AC000', ['ac000']], // Processing Facility TRDv3 only
        ['AC020', ['ac020']], // Catalytic Cracker
        ['AD030', ['ad030']], // Power Substation
        ['AF010', ['af010']], // Smokestack
        ['AF030', ['af030']], // Cooling Tower: TRDv30 and TDS but not TRDv4
        ['AF040', ['af040']], // Crane
        ['AF070', ['af070']], // Flare Pipe
        ['AH050', ['ah050']], // Fortification
        ['AH070', ['ah070']], // Check Point
        ['AJ051', ['aj051']], // Wind Turbine
        ['AK040', ['ak040']], // Sports Ground
        ['AK160', ['ak160']], // Stadium
        ['AL015', ['al015','building']], // Building
        ['AL020', ['al020']], // Built up area
        ['AL025', ['al025']], // Cairn
        ['AL030', ['al030']], // Cemetary
        ['AL080', ['al080']], // Gantry
        ['AL100', ['al100']], // Hut
        ['AL105', ['al105']], // Settlement
        ['AL130', ['al130']], // Memorial Monument
        ['AL200', ['al200']], // Ruins
        ['AL240', ['al240']], // Non-communication Tower - TRD30
        ['AL241', ['al241']], // Tower - TRD40
        ['AM020', ['am020']], // Grain Storage Structure
        ['AM030', ['am030']], // Grain Elevator
        ['AM040', ['am040']], // Mineral Pile
        ['AM060', ['am060']], // Surface Bunker
        ['AM070', ['am070']], // Storage Tank
        ['AM070', ['am080']], // Water Tower
        ['AN010', ['an010','railroad_l']], // Railway
        ['AN050', ['an050']], // Railway Sidetrack
        ['AN060', ['an060']], // Railway Yard
        ['AN075', ['an075']], // Railway Turntable
        ['AP010', ['ap010','trackl','track_l','cart_track','carttrack','cart_track_l']], // Cart Track
        ['AP020', ['ap020','interchange_l','interchangel']], // Interchange
        ['AP030', ['ap030','roadnet','road_l','roadl']], // Road
        ['AP050', ['ap050','traill','trail_l']], // Trail
        ['AQ040', ['aq040','bridgel','bridge_l','bridge_tunnel_l','bridge_overpass_l']], // Bridge
        ['AQ060', ['aq060']], // Control Tower
        ['AQ065', ['aq065']], // Culvert
        ['AQ090', ['aq090']], // Entrance and/or Exit
        ['AQ113', ['aq113']], // Pipeline
        ['AQ116', ['aq116']], // Pumping Station
        ['AQ125', ['aq125']], // Transportation Station
        ['AQ130', ['aq130','tunnell','tunnel_l']], // Tunnel
        ['AT010', ['at010']], // Dish Aerial
        ['AT042', ['at042']], // Pylon
        ['AT045', ['at045']], // Radar Station
        // ['AT050', ['at050']], // Communication Station
        ['AT080', ['at080']], // Communication Tower
        ['BH010', ['bh010']], // Aqueduct
        ['BH020', ['bh020']], // Canal
        ['BH030', ['bh030']], // Ditch
        ['BH070', ['bh070']], // Ford
        ['BH140', ['bh140','river_stream_l']], // River
        ['BH145', ['bh145']], // Vanishing Point
        ['BH170', ['bh170']], // Natural Pool
        ['BI010', ['bi010']], // Cistern
        ['BI020', ['bi020']], // Dam
        ['BI040', ['bi040']], // Sluice Gate
        ['BI050', ['bi050']], // Water Intake Tower
        ['DB030', ['db030']], // Cave
        ['DB070', ['db070']], // Cut
        ['DB150', ['db150']], // Mountain Pass
        ['GA035', ['ga035']], // Aeronautical NAVAID - TRDv30
        ['GB010', ['gb010']], // Airport Navigation Lights
        ['GB030', ['gb030']], // Helipad
        ['GB040', ['gb040']], // Launch Pad
        ['SU001', ['su001']], // Military Installation
        ['ZD040', ['zd040']], // Text
        ['ZD045', ['zd045', 'annop', 'anno_p']] // Annotation
        ];
      // Funky but it makes life easier
      var llayerName = layerName.toString().toLowerCase();

      for (var row in fCodeMap)
      {
        for (var val in fCodeMap[row][1])
        {
          if (llayerName == fCodeMap[row][1][val])
          {
            attrs.F_CODE = fCodeMap[row][0];
            break;
          }
        }
      }
    } // End of Find an FCode

    // Tag retired
    if (tags.controlling_authority)
    {
      tags.operator = tags.controlling_authority;
      delete tags.controlling_authority;
    }

    switch (attrs.F_CODE)
    {
      case 'AL013':     // Posible Building F_CODE screwup
        attrs.F_CODE = 'AL015';
        break;

      case 'AQ125': // Swap the F_CODE for a Ferry Terminal
        if (attrs.TRS == '7' || attrs.TRS == 'Maritime') attrs.F_CODE = 'AQ080'; // TDS/GGDM fcode for Ferry Terminal
        break;
    } // End F_CODE

    // Convert Rasilway guage widths
    if (attrs.GAW)
    {
      attrs.GAW = Number(attrs.GAW) * 1000; // Convert M to MM
    }

    // TRD3 vs TRD4.  SUC (Shed Type)
    // TRD3 is a building AL015
    // TRD4 is a Protection Shed AL210
    if (attrs.F_CODE == 'AL015' && attrs.SUC)
    {
      attrs.xSUC = attrs.SUC;
      delete attrs.SUC;
    }

    // Aditional features AFA002 and AFA003 have different values for COD
    if (attrs.F_CODE == 'FA002' || attrs.F_CODE == 'FA003')
    {
      if (attrs.COD == '1')
      {
        attrs.COD = '1001';
      }
      else if (attrs.COD == '2')
      {
        attrs.COD = '1000';
      }
    }

    if (attrs.F_CODE == 'AJ010')
    {
      tags.landuse = 'farmland';
      tags.irrigation = 'pivot';
    }

    if (attrs.F_CODE == 'AA010')
    {
      tags.landuse = 'industrial';
      tags.industrial = 'mine';
    }

    if (attrs.F_CODE == 'AD050')
    {
      tags.landuse = 'industrial';
      tags.industrial = 'heating';
    }


  }, // End of applyToOsmPreProcessing

  // Post Processing: Lots of cleanup
  applyToOsmPostProcessing : function (attrs, tags, layerName, geometryType)
  {
    // Unpack the TXT field or OSMTAGSX if needed
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
        if (i in mgcp.fcodeLookupOut) // tag -> FCODE table
        {
          if (tags[i] in mgcp.fcodeLookupOut[i])
          {
            var row = mgcp.fcodeLookupOut[i][tags[i]];

            // Now find the "real" tag that comes frm the FCode
            if (row[1] in mgcp.fcodeLookup['F_CODE'])
            {
              var row2 = mgcp.fcodeLookup['F_CODE'][row[1]];
              // If the tags match, delete it
              if (tags[row2[0]] && (tags[row2[0]] == row2[1]))
              {
                delete tags[row2[0]];
              }
            }
          }
        }
      } // End nTags

      if (tObj.text !== '')
      {
        tags.note = tObj.text;
      }
      else
      {
        delete tags.note;
      }
    } // End process tags.note

    // Calculate accuracy: taken straight from facc.py
    // 1/30 inch * SCALE for standard
    // NGA standard is +/-25m 90% circular error (CE) for vector data
    // Error is only for well defined points, not the line connecting points
    var accuracy = -1
    // if ('ACE_EVAL' in attrs && attrs['ACE_EVAL'] != '')
    if (translate.isOK(attrs.ACE_EVAL))
    {
      // Convert to 95% circular error (AKA ~2 standard deviations)
      accuracy = attrs.ACE_EVAL * 1.91;
    }
    // else if ('ACE' in attrs && attrs['ACE'] != '')
    else if (translate.isOK(attrs.ACE))
    {
      // Convert to 95% circular error (AKA ~2 standard deviations)
      accuracy = attrs.ACE_EVAL * 1.91;
    }
    // else if ('SCALE' in attrs && attrs['SCALE'] != '')
    else if (translate.isOK(attrs.SCALE))
    {
      var scale = attrs.SCALE;
      // Convert to accuracy as 90% circular error
      accuracy = scale * 1.0 / 30.0 / 12 * 0.3048;
      // Convert to 95% circular error (AKA ~2 standard deviations)
      accuracy = accuracy * 1.91;
    }

    // if ('ACC' in attrs)
    if (translate.isOK(attrs.ACC))
    {
      switch (attrs.ACC)
      {
        case '2':
          accuracy = accuracy * 2; // Add note: Accuracy: Approximate
          break;

        case '3':
          accuracy = accuracy * 4; // Add note: Accuracy: Doubtful
          break;

        case '5':
          accuracy = -1; // Add note: Accuracy: Disputed }
          break;

        case '6': // Add note: Accuracy: Undisputed
        case '7': // Add note: Accuracy: Precise
          break;

        case '8':
          accuracy = -1; // Add note: Accuracy: Abrogated
          break;
      }
    }

    if (accuracy > -1)
    {
      tags['error:circular'] = accuracy;
    }

    // #####
    if (attrs.HWT && attrs.HWT !== '0' && attrs.HWT !== '998')
    {
      if (attrs.HWT !== '7') tags.amenity = 'place_of_worship';

      switch (tags.building)
      {
        case undefined:
          break;

        case 'yes':
          delete tags.building;
          break;

        case 'cathedral':
        case 'chapel':
        case 'church':
          tags.religion = 'christian';
          break;

        case 'marabout':
        case 'mosque':
          tags.religion = 'muslim';
          break;

        case 'synagogue':
          tags.religion = 'jewish';
          break;

        case 'stupa':
          religion = 'buddhist';
          break;

        // In the spec, these don't specify a religion
        // case 'religious_community':
        // case 'pagoda':
        // case 'shrine':
        // case 'tabernacle':
        // case 'temple':
      } // End switch
    } // End HWT

    // A bit crude but it helps the case where we drop the religion tag on export
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

    if (tags['tower:type'] == 'minaret')
    {
      tags.religion = 'muslim';
    }

    if (tags.with_minaret == 'yes')
    {
      tags.religion = 'muslim';
      tags.building = 'mosque';
    }

    // Add the LayerName to the source
    if ((! tags.source) && layerName !== '') tags.source = 'mgcp:' + layerName.toLowerCase();

    // If we have a UID, store it
    if (tags.uuid)
    {
      tags.uuid = tags['uuid'].toString().toLowerCase();
      if (tags['uuid'].indexOf('{') == -1)  tags.uuid = '{' + tags['uuid'] + '}';
    }
    else
    {
      if (mgcp.configIn.OgrAddUuid == 'true') tags.uuid = createUuid();
    }

    // Railway Yard
    // if (attrs.F_CODE == 'AN060' && tags.landuse =='railway') tags.service = 'yard';

    // Communications Tower
    // if (attrs.F_CODE == 'AT080') tags['tower:type'] = 'communication';

    // Railway vs Road
    // if (attrs.F_CODE == 'AN010' && attrs.RRC =='0') tags.railway = 'yes';

    // EC060 = man_made:clearcut
    if (attrs.F_CODE == 'EC060')
    {
      tags.man_made = 'clearcut';
      delete tags.natural;
    }

    if (attrs.F_CODE == 'AL010' && attrs.FFN == '825')
    {
      tags.diplomatic = 'embassy';
      tags.office = 'diplomatic';
      delete tags.landuse;
    }

    if (attrs.F_CODE == 'AJ030')
    {
      tags.landuse = 'farmyard';
      tags.farmyard = 'stockyard';
    }

    if (attrs.F_CODE == 'AM030')
    {
      tags.building = 'silo';
      tags.content = 'grain';
      delete tags.man_made;
    }

    if (attrs.F_CODE == 'AP030' && attrs.WTC == '0')
    {
      tags.highway = 'residential';
    }

    if (attrs.F_CODE == 'BH030' && tags.waterway == 'ditch')
    {
      delete tags.area;
      tags.natural = 'water';
    }

    if (mgcp.osmPostRules == undefined)
    {
      // "New" style complex rules
      //
      // Rules format:  ["test expression","output result"];
      // Note: t = tags, a = attrs and attrs can only be on the RHS
      var rulesList = [
      ["t.barrier == 'dragons_teeth' && !(t.tank_trap)","t.barrier = 'tank_trap'; t.tank_trap = 'dragons_teeth'"],
      ["t['bridge:movable'] && t['bridge:movable'] !== 'no' && t['bridge:movable'] !== 'unknown'","t.bridge = 'movable'"],
      ["t['building:religious'] == 'other'","t.amenity = 'religion'"],
      // ["t['cable:type'] && !(t.cable)","t.cable = 'yes'"],
      ["t.control_tower == 'yes'","t['tower:type'] = 'observation'; t.use = 'air_traffic_control'"],
      ["t.embankment == 'yes' && !(t.highway || t.railway)","delete t.embankment; t.man_made = 'embankment'"],
      ["t['generator:source']","t.power = 'generator'"],
      ["(t.landuse == 'built_up_area' || t.place == 'settlement') && t.building","t['settlement:type'] = t.building; delete t.building"],
      ["t.landuse == 'industrial' && t.industrial == 'heating' && t.amenity","delete t.amenity"],
      ["t.leisure == 'stadium'","t.building = 'yes'"],
      ["t['monitoring:weather'] == 'yes'","t.man_made = 'monitoring_station'"],
      ["t.military == 'revetment'","t.barrier = 'berm'; delete t.military"],
      ["t.natural =='spring' && t['spring:type'] == 'spring'","delete t['spring:type']"],
      // ["t.public_transport == 'station'","t.bus = 'yes'"],
      ["t['social_facility:for'] == 'senior'","t.amenity = 'social_facility'; t.social_facility = 'group_home'"],
      ["t['subject_to_inundation'] == 'yes'","delete t['subject_to_inundation']; t.flood_prone = 'yes'"],
      ["t['tower:type'] && !(t.man_made)","t.man_made = 'tower'"],
      ["t.water && !(t.natural)","t.natural = 'water'"],
      ["t.wetland && !(t.natural)","t.natural = 'wetland'"]
      ];

      mgcp.osmPostRules = translate.buildComplexRules(rulesList);
    }

    // translate.applyComplexRules(tags,attrs,mgcp.osmPostRules);
    // Pulling this out of translate
    for (var i = 0, rLen = mgcp.osmPostRules.length; i < rLen; i++)
    {
      if (mgcp.osmPostRules[i][0](tags)) mgcp.osmPostRules[i][1](tags,attrs);
    }

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

    // if (tags.building == 'train_station' && !tags.railway) tags.railway = 'station';
    // if ('ford' in tags && !tags.highway) tags.highway = 'road';

    if (attrs.F_CODE == 'AM040')
    {
      tags.man_made = 'heap';
      switch (attrs.PPO) { // MGCP doesn't support as many options for ore type as OSM does, so we may not always get
                           // the same result going back.
        case '0':
          break;
        case '18':
          tags.resource = 'coal';
          break;
        case '21':
          tags.resource = 'coke';
          break;
        case '53':
          tags.resource = 'gravel';
          break;
        case '69':
          tags.resource = 'gold';
          break;
        case '95':
          tags.resource = 'salt';
          break;
        case '96':
          tags.resource = 'sand';
          break;
        case '110':
          tags.resource = 'dimension_stone';
          break;
        default: // 999 or 996
          tags.resource = 'other';
          break;
      }
      delete tags.product;
    }

    // Some FCODE specific rules
    switch (attrs.F_CODE)
    {
    case undefined: // Break early if no value
      break;

    case 'AA050': // Well
      if (tags.product)
      {
        tags.substance = tags.product;
        delete tags.product;

        if (tags.substance == 'oil' || tags.substance == 'gas') tags.man_made = 'petroleum_well';
        if (tags.substance == 'water' )
        {
          tags.man_made = 'water_well';
          delete tags.substance;
        }
      }
      break;

    case 'AA052': // Hydrocarbons Field
      tags.landuse = 'industrial';
      tags.industrial = 'oil';
      break;

    case 'AD010': // Electric Power Plant
      if (!tags['plant:output:electricity']) tags['plant:output:electricity'] = 'yes';
      if (!tags.landuse) tags.landuse = 'industrial';

      switch (attrs.PPC)
      {
        case undefined:
          break;

        case '2':
          tags['plant:method'] = 'fission';
          break;

        case '5':
          tags['plant:method'] = 'wind_turbine';
          break;
      }
      break;

    case 'AF030': // Cooling Tower
      if (!tags['tower:type']) tags['tower:type'] = 'cooling';
      break;

    case 'AH050': // Fortification
      // Castles are not Bunkers but they get stored in the same layer
      if (tags.military == 'bunker' && tags.historic == 'castle')
      {
        delete tags.military;
      }
      else if (!tags.building)
      {
        tags.building = 'bunker';
      }
      break;
    
    case 'AQ110': // Mooring airship
      tags.man_made = 'tower';
      tags.mooring = 'yes';
      delete tags['seamark:type']
      break;

    case 'AK040': // Athletic Field, Sports Ground
    case 'BA050': // Beach
    case 'AL010': // Facility
      switch (attrs.FFN)
      {
        case '2':
          tags.landuse = 'farmyard';
          break;
        case '99':
          tags.man_made = 'works';
          delete tags.landuse;
          break;
        case '350':
          tags.landuse = 'industrial';
          tags.utilities = 'yes';
          break;
        case '440':
          tags.landuse = 'commercial';
          break;
        case '480':
          tags.public_transport = 'station';
          delete tags.landuse;
          break;
        case '550':
          tags.tourism = 'hotel';
          delete tags.landuse;
          break;
        case '563':
          tags.landuse = 'residential';
          break;
        case '610':
          tags.office = 'telecommunication';
          delete tags.landuse;
          break;
        case '810':
          tags.landuse = 'civic_admin';
          break;
        case '811':
          tags.office = 'government';
          delete tags.landuse;
          break;
        case '825':
          tags.office = 'diplomatic';
          delete tags.landuse;
          break;
        case '835':
          tags.landuse = 'military';
          break;
        case '843':
          tags.amenity = 'prison';
          delete tags.landuse;
          break;
        case '850':
          tags.amenity = 'school';
          delete tags.landuse;
          break;
        case '860':
          tags.amenity = 'hospital';
          delete tags.landuse;
          break;
        case '907':
          tags.leisure = 'garden';
          delete tags.landuse;
          break;
        case '907':
          tags.leisure = 'sports_centre';
          delete tags.landuse;
          break;
      }
    case 'AL140':
      tags.amenity = 'research_institute';
      tags.research = 'particle_physics';
      if (attrs.NAM) tags.operator = attrs.NAM;
      delete tags.man_made;
      delete tags.name;
      break;
    case 'AM020': // Grain Storage Structure
      tags.content = 'grain';
      break;
    case 'BB005': // Harbour
      tags.harbour = 'yes';
      tags['seamark:type'] = 'harbour';
      tags.industrial = 'port';
      break;
    case 'BJ040': // Ice cliff
      tags.natural = 'cliff';
      tags.surface = 'ice';
      break;
    case 'BJ060': // Ice peak
      tags.natural = 'peak';
      tags.surface = 'ice';
      break;
    case 'DB070': // Cut
      if (tags.material && !tags.surface)
      {
        tags.surface = tags.material;
        delete tags.material;
      }
      break;

    case 'AL015': // Building
      if (tags.surface == 'unknown') delete tags.surface;
      if (tags.industrial == 'petroleum_refining' && (tags.product == 'unknown' || tags.product == undefined))
      {
        tags.product = 'petroleum';
      }
      break;

    case 'AL020': // AL020 (Built-up Area) should become a Place. NOTE: This is a bit vague...
      tags.place = 'yes'; // Catch All

      switch (tags['place:importance'])
      {
        case undefined: // Break early if no value
          break;

        case 'first':
          tags.place = 'city';
          tags.capital = 'yes'
          break;

        case 'second':
          tags.place = 'city';
          break;

        case 'third':
        case 'fourth':
          tags.place = 'town';
          break;

        case 'fifth':
          tags.place = 'village';
          break;

        case 'sixth':
          tags.place = 'hamlet';
          break;
      } // End switch

      switch (tags.use) // Fixup the landuse tags
      {
        case undefined: // Break early if no value
          break;

        case 'industrial':
          tags.landuse = 'industrial';
          delete tags.use;
          break;

        case 'commercial':
          tags.landuse = 'commercial';
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

    case 'AN010':
      if (tags['railway:track'] == 'monorail')
      {
        if (tags.railway !== 'rail') tags['railway:type'] = tags.railway; // Redundant tags
        tags.railway = 'monorail';
        delete tags['railway:track'];
      }
      break;

    // case 'AP010': // Track
    // case 'AP050': // Trail
    //     tags.seasonal = 'fair';
    //     break;

    case 'AP030': // Road
      if (tags.highway == 'yes') tags.highway = 'road';
      break;

    case 'AT042': // Pylon
      // We need to deconflict various tower types
      switch (tags['cable:type'])
      {
        case 'unknown': // Just keep 'pylon=yes'
        case 'other':
          break;

        case 'power':
          delete tags['cable:type']; // fall through
        case 'transmission':
          delete tags.pylon;
          tags.power = 'tower';
          break;

        case 'communication':
          break;
      }
      break; // End AT042

    case 'AQ075': // Ice Route
      if (!tags.highway) tags.highway = 'road';
      break;

    case 'AQ125': // Transportation Station
    //   if (tags.amenity == 'ferry_terminal')
    //   {
    //     tags['transport:type'] = 'maritime';
    //     delete tags.bus;
    //   }
      if (!tags.amenity && tags['transport:type'] == 'bus')
      {
        tags.amenity = 'bus_station';
        delete tags['transport:type'];
      }
      break;

    case 'BA040': // Tidal Water
      tags.natural = 'water';
      break;

    case 'BB041': // Breakwater
      // Differentiate between Line and Area breakwaters
      if (geometryType == 'Area')
      {
        // Debug
        // print('Adding area=yes');
        tags.area = 'yes';
      }
      break;

    case 'BB190': // Berthing Structure
      if (tags.waterway == 'dock' && tags.man_made == 'shoreline_construction')
      {
        delete tags.waterway;
        delete tags.man_made;
        tags['seamark:type'] = 'shoreline_construction';
        tags['seamark:shoreline_construction:category'] = 'wharf';
      }
      
      break;

    case 'BD130': // Hazardous Rock Point
      tags['seamark:type'] = 'rock';
      break;

    case 'BD180': // Wreck
      if (!tags['seamark:type']) tags['seamark:type'] = 'wreck';
      break;

    // case 'BH070': // Ford
    //   // Fords are also supposed to be roads
    //   if (geometryType == 'Line' && !tags.highway) tags.highway = 'road';
    //   break;

    case 'BH135': // Ricefield
      tags.landuse = 'farmland';
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
      if (tags.natural && (tags.natural == tags.material))
      {
        delete tags.geological;  // The natural tag is the better one to use
        delete tags.material; // Implied value: natural=sand -> material=sand
      }

      if (tags.material && !tags.surface)
      {
        tags.surface = tags.material;
        delete tags.material;
      }
      break;

    case 'DB100':
      delete tags.landform;
      tags.natural = 'ridge';
      tags.ridge = 'esker';
      break;

    case 'EA010': // Crop Land
        // Coffee is an orchard according to OSM. Tea is as well but MGCP calls it Crop Land.
      if (tags.trees == 'tea_plants') tags.landuse = 'orchard';
      if (tags.trees == 'coffea_plants') tags.landuse = 'orchard';
      if (tags.crop == 'other' && tags.trees) delete tags.crop;
      break;

    case 'EA040': // Orchard
      // Crop vs Trees
      if (tags.crop == 'other' && tags.trees) delete tags.crop;
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

    case 'ED030': // Mangrove Swamp
      if (!tags.tidal) tags.tidal = 'yes';
      break;

    // EC030 - Wood
    case 'EC030':
      if (geometryType == 'Line')
      {
        delete tags.landuse; // Default EC030 translation
        tags.natural = 'tree_row';
      }
      break;

    case 'FA015': // Firing Range
      if (! tags.landuse) tags.landuse = 'military';
      break;

    case 'GB485': // Approach Lighting System
      tags.navigationaid = 'als';
      break;

    case 'ZD040':
      if (layerName.toLowerCase() == 'annop') {
        // ArcMap supports an edge case where annotation points can have a ZD040 (normally text point) fcode
        delete tags.named_location;
        tags.annotated_location = 'yes';
      }
    
    case 'EC010':
      delete tags.natural;
      tags.crop = 'sugarcane'
      tags.landuse = 'orchard'
    } // End switch FCODE

    // Content vs Product for storage tanks
    if (tags.product && tags.man_made == 'storage_tank')
    {
      tags.content = tags.product;
      delete tags.product;
    }

    // AC000 (Processing Facility) vs AL010 (Facility)
    // In TDS, this is just AL010. Therefore, make it AL010 and use a custom rule if we are exporting
    // We are assumeing that it should produce something
    if (tags.facility == 'processing')
    {
      if (! tags.product) tags.product = 'unknown';
      // tags.facility = 'yes';
    }

    // Sort out the WID, WD1 etc attributes
    // Roads etc have a WD1 attribute but this doesn't get translated to "width"
    if (attrs.WD1)
    {
      if (! tags.width)
      {
        tags.width = attrs.WD1;
        delete tags['width:minimum_traveled_way'];
      }
    }

    // Fix up areas
    // The thought is: If Hoot thinks it's an area but OSM doesn't think it's an area, make it an area
    if (geometryType == 'Area' && ! translate.isOsmArea(tags))
    {
      // Debug
      // print('Adding area=yes');
      tags.area = 'yes';
    }
  }, // End of applyToOsmPostProcessing

  // ##### Start of the xxToOgrxx Block #####
  applyToOgrPreProcessing: function(tags, attrs, geometryType)
  {
    // Remove Hoot assigned tags for the source of the data
    delete tags['source:ingest:datetime'];
    delete tags['error:circular'];
    delete tags['hoot:status'];

    // Unpack othertags if required
    translate.unpackOtherTags(tags);

    // initial cleanup
    for (var i in tags)
    {
      // Remove empty tags
      if (tags[i] == '')
      {
        delete tags[i];
        continue;
      }

      // Convert "abandoned:XXX" features
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

    // Highway cleanup
    switch (tags.highway)
    {
      case undefined: // Break early if no value
        break;

      // Doing this here so we preserve the "highway" tag
      case 'bridleway':
      case 'cycleway':
      case 'footway':
      case 'steps':
        attrs.F_CODE = 'AP050'; // Trail
        break;

      case 'service':
      case 'living_street':
        attrs.F_CODE = 'AP030'; // Road
        break;

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
    } // End highway

    // Ice roads are a special case.
    if (tags.ice_road == 'yes')
    {
      attrs.F_CODE = 'AQ075';
      if (tags.highway == 'road') delete tags.highway;
    }
    if (tags.barrier == 'fence' && tags.area == 'yes' && !(tags.military || tags.landuse))
    {
      delete tags.area;
    }

    if (mgcp.mgcpPreRules == undefined)
    {
      // See ToOsmPostProcessing for more details about rulesList
      var rulesList = [
      // ["t.amenity == 'marketplace'","t.facility = 'yes'"],
      ["t.amenity == 'ferry_terminal'","t['transport:type'] = 'maritime'"],
      ["t.amenity == 'prison' && t.barrier == 'wall'","delete t.barrier"],
      ["t.building == 'silo' && t.content == 'grain'","a.F_CODE = 'AM030'"],
      ["t.tourism == 'attraction' && t.building == 'church'","delete t.tourism"],
      ["t.tourism == 'attraction' && t.landuse == 'commercial'","delete t.tourism"],
      ["t.aeroway == 'navigationaid' && t.navigationaid","delete t.navigationaid"],
      ["t.barrier == 'tank_trap' && t.tank_trap == 'dragons_teeth'","t.barrier = 'dragons_teeth'; delete t.tank_trap"],
      ["t.barrier == 'fence' && t.area == 'yes' && !(t.landuse || t.military)","delete t.area"],
      ["t.bus == 'yes'","t['transport:type'] = 'bus'"],
      ["t.communication == 'line'","t['cable:type'] = 'communication'"],
      // ["t.construction && t.railway","t.railway = t.construction; t.condition = 'construction'; delete t.construction"],
      // ["t.construction && t.highway","t.highway = t.construction; t.condition = 'construction'; delete t.construction"],
      ["t.content && !(t.product)","t.product = t.content; delete t.content"],
      ["t.landuse == 'farmyard' && t.farmyard == 'stockyard'","a.F_CODE = 'AJ030'"],
      ["t.landuse == 'aquaculture' && t.aquaculture == 'fish'","a.F_CODE = 'BH051'"],
      ["t.landuse == 'industrial' && t.industrial == 'mine'","a.F_CODE = 'AA010'"],
      ["t.landuse == 'industrial' && t.industrial == 'heating'","a.F_CODE = 'AD050'"],
      ["t.leisure == 'stadium' && t.building","delete t.building"],
      ["t.man_made && t.building == 'yes'","delete t.building"],
      ["t.man_made == 'cut_edge'","t.cutting = 'yes'; a.F_CODE = 'DB070'"],
      ["t.man_made == 'water_tower'","a.F_CODE = 'AL241'"],
      ["t.military == 'bunker' && t.building == 'bunker'","delete t.building"],
      ["t.military == 'revetment'","t.barrier = 'berm'; delete t.military"],
      ["t.natural == 'sinkhole'","a.F_CODE = 'BH145'; t['water:sink:type'] = 'disappearing'; delete t.natural"],
      ["t.natural == 'spring' && !(t['spring:type'])","t['spring:type'] = 'spring'"],
      // ["t.power == 'generator'","a.F_CODE = 'AL015'; t.use = 'power_generation'"],
      //["t.power == 'line'","t['cable:type'] = 'power'; t.cable = 'yes'"],
      ["t.power == 'minor_line'","t.spower = 'minor_line'"],
      ["t.rapids == 'yes'","t.waterway = 'rapids'"],
      ["t.resource && t.man_made != 'heap'","t.product = t.resource; delete t.resource"],
      ["t.route == 'road' && !(t.highway)","t.highway = 'road'; delete t.route"],
      // ["(t.shop || t.office) && !(t.building)","a.F_CODE = 'AL015'"],
      ["t.tourism == 'information' && t.information","delete t.tourism"],
      ["t.tunnel == 'building_passage'","t.tunnel = 'yes'"],
      ["t.waterway == 'riverbank'","t.waterway = 'river'"],
      ["t.wetland && t.natural == 'wetland'","delete t.natural"],
      ["t.wetland == 'wet_meadow'", "a.F_CODE = 'ED010'"],
      ["t.aeroway == 'hangar' && t.building == 'yes'", "t.building = 'hangar'"],
      ["t.landuse == 'religious'", "t.amenity = 'religious_activities'"],
      ["t.industrial == 'depot' && t.depot == 'bus'", "a.F_CODE = 'AL010'; a.FFN = '480'"],
      ["t.water == 'moat'", "a.F_CODE = 'BH030'"],
      ["t.building == 'hotel'", "a.F_CODE = 'AL015'; a.FFN = '550'"],
      ["t.amenity == 'language_school' && t.barrier == 'wall'", "delete t.barrier"],
      ["t.amenity == 'language_school'", "a.FFN = '850'"],
      ["t.depot == 'bus' && t.landuse == 'brownfield'", "delete t.landuse; a.F_CODE = 'AL010'; a.FFN = '480'"],
      ["t.crop == 'sugarcane' && t.landuse == 'orchard'", "a.F_CODE = 'EC010'"], // override the Orchard FCode with Cane when Cane is the relevant crop
      ["t.natural == 'ridge' && t.ridge == 'esker'", "a.F_CODE = 'DB100'"],
      ["t.landuse == 'industrial' && t.utilities", "a.F_CODE = 'AL010'; a.FFN = '350'"],
      ["t.public_transport == 'station'", "a.F_CODE = 'AL010'; a.FFN = '480'"],
      ["t.tourism == 'hotel'", "a.F_CODE = 'AL010'; a.FFN = '550'"],
      ["t.leisure == 'garden' || t.tourism == 'zoo'", "a.F_CODE = 'AL010'; a.FFN = '907'"],
      ["t.leisure == 'sports_centre'", "a.F_CODE = 'AL010'; a.FFN = '912'"],
      ["t.natural == 'cliff' && t.surface == 'ice'", "a.F_CODE = 'BJ040'"],
      ["t.natural == 'peak' && t.surface == 'ice'", "a.F_CODE = 'BJ060'"],
      ["t['seamark:type'] == 'mooring'", "delete t['seamark:type']"]
      ];

      mgcp.mgcpPreRules = translate.buildComplexRules(rulesList);
    }

    // Apply the rulesList
    //translate.applyComplexRules(tags,attrs,mgcp.mgcpPreRules);
    for (var i = 0, rLen = mgcp.mgcpPreRules.length; i < rLen; i++)
    {
      if (mgcp.mgcpPreRules[i][0](tags)) mgcp.mgcpPreRules[i][1](tags,attrs);
    }

    // Deconflict towers
    if (tags.power == 'tower')
    {
      delete tags.power;
      tags.pylon = 'yes';
      if (!tags['cable:type']) tags['cable:type'] = 'power';
    }

    // Sort out landuse
    switch (tags.landuse)
    {
      case undefined: // Break early if no value
        break;

      case 'brownfield':
        tags.landuse = 'built_up_area';
        tags.condition = 'destroyed';
        break;

      case 'civic_admin':
        attrs.F_CODE = 'AL010';
        attrs.FFN = '810';
        break;

      case 'commercial':
        attrs.F_CODE = 'AL010';
        attrs.FFN = '440';
        break;

      case 'construction':
        tags.condition = 'construction';
        tags.landuse = 'built_up_area';
        break;

      case 'residential':
        attrs.F_CODE = 'AL010';
        attrs.FFN = '563';
        break;

      case 'retail':
        tags.use = 'commercial';
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

        if (tags.irrigation == 'pivot') attrs.F_CODE = 'AJ010'; // Circular Irrigation System

        break;

      case 'farmyard': // NOTE: This is different to farm && farmland
        if (tags.farmyard == 'stockyard')
        {
          attrs.F_CODE = 'AJ030';
          break;
        } else {
          attrs.F_CODE = 'AL010';
          attrs.FFN = '2';
          break;
        }
        

      case 'grass':
      case 'meadow':
        tags.natural = 'grassland';
        tags['grassland:type'] = 'grassland';
        delete tags.landuse;
        break;

      case 'industrial':
        switch (tags.industrial)
        {
          case undefined: // Built up Area
            if (!tags.power)
            {
              tags.use = 'industrial';
              tags.landuse = 'built_up_area';
            }

            break;

          case 'oil':
            tags.product = 'petroleum';
            delete tags.landuse;
            attrs.F_CODE = 'AA052';
            break;

          case 'gas':
            tags.product = 'gas';
            delete tags.landuse;
            attrs.F_CODE = 'AA052';
            break;

          case 'hydrocarbons_field': // Unsupported tag in OSM, included here for legacy reasons
            delete tags.landuse;
            break;
        }
        break;

      case 'military':
        attrs.F_CODE = 'AL010'
        attrs.FFN = '835'
        break;

      case 'orchard':
        if (tags.trees == 'tea_plants')  // Tea is a cropland in MGCP and an Orchard in OSM
        {
          attrs.F_CODE = 'EA010'; // Cropland
          attrs.CSP = '34'; // Tea
          delete tags.landuse; // Stop it making an F_CODE later
        }

        if (tags.trees == 'hop_plants')
        {
          attrs.F_CODE = 'EA055'; // Hop Field
          attrs.CSP = '18'; // Hops
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

      case 'rice_field': // Old translation for BH135 Ricefield
        tags.crop = 'rice';
        delete tags.landuse;
        break;

      case 'scrub':
        tags.natural = 'scrub';
        delete tags.landuse;
        break;
    } // End switch landuse

    // Fix up OSM 'walls' around facilities
    if ((tags.barrier == 'wall' || tags.barrier == 'fence') && geometryType == 'Area')
    {
      if (tags.landuse == 'military' || tags.military) attrs.F_CODE = 'SU001'; // Military Installation
    }

    // Wind Turbines, Solar Panels etc  vs power plants
    if (tags['generator:source'])
    {
      delete tags.power;
    }
    else if (tags.power == 'generator')
    {
      attrs.F_CODE = 'AL015';
      tags.use = 'power_generation';
    }

    switch (tags.office)
    {
      case 'telecommunication':
        attrs.F_CODE = 'AL010';
        attrs.FFN = '610';
        break;
      case 'government':
        attrs.F_CODE = 'AL010';
        attrs.FFN = '811';
        break;
      case 'diplomatic':
        attrs.F_CODE = 'AL010';
        attrs.FFN = '825';
        break;
      case 'educational_institution':
        attrs.F_CODE = 'AL010';
        attrs.FFN = '850';
        break;
    }

    switch (tags.amenity)
    {
      case 'prison':
        attrs.F_CODE = 'AL010';
        attrs.FFN = '843';
        break;
      case 'school':
        attrs.F_CODE = 'AL010';
        attrs.FFN = '850';
        break;
      case 'hospital':
      case 'clinic':
      case 'doctors':
      case 'dentist':
        attrs.F_CODE = 'AL010';
        attrs.FFN = '860';
        break;
    }

    // Going out on a limb and processing OSM specific tags:
    // - Building == a thing,
    // - Amenity == The area around a thing. Except for the ones that are not.....
    // If it is a Point feature, it becomes a building. Polygon features become Facilities
    // Line features become buildings and then get ignored
    var facilityList = {'school':'850','hospital':'860','university':'850','college':'850'};
    if (tags.amenity in facilityList)
    {
      if (geometryType == 'Area')
      {
        attrs.F_CODE = 'AL010'; // Facility

        // If the user has also set a building tag, delete it
        delete tags.building;
      }
      else
      {
        attrs.F_CODE = 'AL015'; // Building
      }

      // If we don't have a Feature Function then assign one
      if (!attrs.FFN) attrs.FFN = facilityList[tags.amenity];
    }

    // AL010 (Facility) vs AC000 (Processing Facility)
    if (tags.facility && tags.product)
    {
      tags.facility = 'processing';

      if (tags.product == 'unknown') delete tags.product;
    }

  // Fix up bus stations
    if (tags.amenity == 'bus_station')
    {
      delete tags.amenity;
      attrs.F_CODE = 'AQ125';
      tags['transport:type'] = 'bus';
    }

    // More facilities
    switch (tags.amenity)
    {
      case undefined: // Break early
        break;

      case 'munitions_storage':
        attrs.F_CODE = 'AM010'; // Storage Depot
        attrs.PPO = '3'; // Ammunition - close to munitions....
        delete tags.amenity;
        break;

      case 'fuel_storage':
        attrs.F_CODE = 'AM010'; // Storage Depot
        attrs.PPO = '46'; // Petrol - close but not great
                  // Other options are: Petroleum:83, Oil:75
        delete tags.amenity;
        break;
    } // End switch Amenity

    if (tags.amenity == 'research_institute' && tags.research == 'particle_physics')
    {
      attrs.F_CODE = 'AL140';
      delete tags.amenity;
      if (tags.operator)
      {
        attrs.NAM = tags.operator;
      }
    }

    // Cutlines and Highways
    // In OSM, a cutline is a cleared way, if it is a polygon then drop the highway info
    if (tags.man_made == 'cutline' && geometryType == 'Area' && tags.highway) delete tags.highway;

  /*
    // Geonames cause problems
    if (tags.waterway && !tags.intermittent)
    {
      if (geometryType == "Point")
      {
        // Becomes a named area fCode = 'ZD040';
        attrs.F_CODE = 'ZD040';
      }
      else
      {
        // It's now a river of some sort fCode = 'BH140';
        attrs.F_CODE = 'BH140';
      }
    }
  */

    saltOrFreshwater = ['pond', 'lake', 'lagoon', 'river']
    if (tags.natural == 'water' && saltOrFreshwater.includes(tags.water)) {
      if (tags.salt == 'yes') {
        attrs.SCC = '10'
      }
      else {
        attrs.SCC = '11'
      }
    }

    // Moved these out of the complex rules to help with BH090 vs other things
    switch (tags.water)
    {
      case undefined:
        break;

      case 'intermittent':
        attrs.F_CODE = 'BH090'; // Old tag for Land Subject to inundation
        delete tags.water;
        break;

      case 'river':
        tags.waterway = 'river';
        delete tags.water;
        break;

      case 'pond':
        if (geometryType == 'Point') {
          attrs.F_CODE = 'BH170'
        }
        else {
          delete tags.water
        }
        break;

      case 'lagoon':
        delete tags.water
        break;
    } // End Water

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
        if (!tags.boundary) {
          attrs.F_CODE = 'AL020'; // Built Up Area
          delete tags.place;
        }
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

    // "service = parking_aisle" is actually the road between rows of car spaces.
    // If this is a line: make it into a road - the default
    // If this is an area: make it into a car park.
    if (tags.service == 'parking_aisle' && geometryType == 'Area')
    {
      delete tags.highway;
      delete tags.service;
      attrs.F_CODE = 'AQ140'; // Vehicle lot / car park
    }

    // Fix the changed Tidal tag
    if (tags.water == 'tidal')
    {
      tags.tidal = 'yes';
      delete tags.water;
    }

    // Sort out tidal features
    if (tags.tidal && (tags.water || tags.waterway || tags.wetland))
    {
      if (tags.tidal == 'yes') attrs.TID = '1001'; // Tidal
      if (tags.tidal == 'no') attrs.TID = '1000'; // non-Tidal

      // Ignore other options
      delete tags.tidal;
    }

    // Military buildings in MGCP TRD3 have a MFC tag that we need to account for
    if (tags.building && tags.military) attrs.F_CODE = 'AL015';

    if (tags.diplomatic == 'embassy' && tags.office == 'diplomatic') 
    {
      attrs.FFN = '825';
    }

    // Embassy buildings should also be coupled with a FFN value of 827 (Embassy)
    if (tags.building && (tags.embassy || tags.diplomatic == 'embassy')) attrs.FFN = '827';

    // runway features should also be assigned a Load Bearing Surface Type = hardpaved (1)
    if (tags.aeroway == 'runway') attrs.RST = '1';

    // Tree rows are a special case for EC030
    if (tags.natural == 'tree_row' && geometryType == 'Line')
    {
      attrs.F_CODE = 'EC030'; // Wood
      delete tags.natural;
    }

    if (tags.annotated_location)
    // We need to get an annotation point out of the schema, then change back to the text point fcode, ZD040, after the attributes are populated
    {
      attrs.F_CODE = 'ZD045';
    }

    // Keep looking for an FCODE
    // This uses the fcodeLookup tables that are defined earlier
    if (!attrs.F_CODE)
    {
      for (var col in tags)
      {
        var value = tags[col];
        if (col in mgcp.fcodeLookup && (value in mgcp.fcodeLookup[col]))
        {
          var row = mgcp.fcodeLookup[col][value];
          attrs.F_CODE = row[1];
        }
        else if (col in mgcp.fcodeLookupOut && (value in mgcp.fcodeLookupOut[col]))
        {
          var row = mgcp.fcodeLookupOut[col][value];
          attrs.F_CODE = row[1];
        }
      }
    }

    // An "amenity" can be a building or a thing
    // If appropriate, make the "amenity" into a building
    // This list is taken from the OSM Wiki and Taginfo
    var notBuildingList = [
      'artwork','atm','bbq','bench','bicycle_parking','bicycle_rental','biergarten','boat_sharing','car_sharing',
      'charging_station','clock','compressed_air','dog_bin','dog_waste_bin','drinking_water','emergency_phone',
      'ferry_terminal','fire_hydrant','fountain','game_feeding','grass_strip','grit_bin','hunting_stand','hydrant',
      'life_ring','loading_dock','nameplate','park','parking','parking_entrance','parking_space','picnic_table',
      'post_box','recycling','street_light','swimming_pool','taxi','trailer_park','tricycle_station','vending_machine',
      'waste_basket','waste_disposal','water','water_point','watering_place','yes',
      ]; // End bldArray

    // if (tags.amenity && notBuildingList.indexOf(tags.amenity) == -1 && !tags.building) attrs.F_CODE = 'AL015';
    if (!(attrs.F_CODE) && !(tags.facility) && tags.amenity && !(tags.building) && (notBuildingList.indexOf(tags.amenity) == -1)) attrs.F_CODE = 'AL015';

    // Tag changed
    if (tags.vertical_obstruction_identifier)
    {
      tags['aeroway:obstruction'] = tags.vertical_obstruction_identifier;
      delete tags.vertical_obstruction_identifier;
    }

    // Surface vs Material
    // Deconflict based on F_CODE for Sports Ground, Beach, Soil Surface Region & Cut
    if (tags.surface && ['AK040','BA050','DA010','DB070'].indexOf(attrs.F_CODE) > -1)
    {
      if (!tags.material) // Defensive
      {
        tags.material = tags.surface;
        delete tags.surface;
      }
    }

    if (tags.flood_prone == 'yes')
    {
      attrs.F_CODE = 'BH090';
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
            tags.material = tags.surface;
            delete tags.surface;
          }
          else
          {
            // Set the SMC type
            tags.material = tags.natural;
          }
          break;
      }
    } // End ! F_CODE

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

    if (tags.man_made == 'heap' || tags.landuse == 'mineral_pile') { // landuse = mineral_pile is not supported by OSM
      switch (tags.resource) {
        case undefined:
          attrs.PPO = '0';
          break;
        case 'coal':
          attrs.PPO = '18';
          break;
        case 'coke':
          attrs.PPO = '21';
          break;
        case 'gravel':
          attrs.PPO = '53';
          break;
        case 'gold':
        case 'silver':
        case 'copper':
        case 'iron_ore':
        case 'aluminum':
        case 'lead':
        case 'nickel':
        case 'tin':
        case 'zinc':
        case 'cobalt':
        case 'zirconium':
        case 'bronze':
        case 'brass':
        case 'titanium':
          attrs.PPO = '69';
          break;
        case 'salt':
          attrs.PPO = '95';
          break;
        case 'sand':
          attrs.PPO = '96';
          break;
        case 'granite':
        case 'slate':
        case 'limestone':
        case 'dimension_stone':
          attrs.PPO = '110';
          break;
        default:
          if (tags.resource && tags.resource.includes(';')) { // multiple ore types
            attrs.PPO = '996'
          }
          else {
            attrs.PPO = '999';
          }
          break;
      }
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

    // We don't have BH220 in MGCP
    switch (tags.man_made)
    {
      case undefined: // Break early if no value
        break;

      case 'water_works':
        delete tags.man_made;
        attrs.F_CODE = 'AL010'; // Facility
        attrs.FFN = '350'; // Utilities
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

      case 'mast':
      case 'tower':
        if (tags.mooring = 'yes') { // This combination of tags is specifically for a mooring airship
          attrs.F_CODE = 'AQ110';
        }
        break;

      case 'offshore_construction': // Legacy support for a non-OSM tag
        attrs.F_CODE = 'BD110';
        break;

      case 'water_well': // Fixing these since it is convenient
        if (!tags.product) tags.product = 'water';
        break;

      case 'petroleum_well':
        if (!tags.product) tags.product = 'oil'; // Not great
        break;
      
      case 'works':
        attrs.F_CODE = 'AL010';
        attrs.FFN = '99';
        break;
    }

    // Fix up water features from OSM
    if (tags.natural == 'water' && !(tags.water || tags.waterway))
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
        tags.water = 'lake';
        attrs.F_CODE = 'BH080';
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

    // Names. Sometimes we don't have a name but we do have language ones
    if (!tags.name) translate.swapName(tags);

  }, // End applyToOgrPreProcessing


  applyToOgrPostProcessing : function (tags, attrs, geometryType, notUsedTags)
  {
    // Gross generalisation. If we don't have an FCODE but we do have an FFN then we either have a
    // Building or a Facility
    // In the absence of any other info, we are making it a Building
    if (!attrs.F_CODE)
    {
      if (attrs.FFN) attrs.F_CODE = 'AL015';
    }

    // If we still don't have an FCODE, try a bit of brute force
    if (!attrs.F_CODE)
    {
      var fcodeMap = {
        'highway':'AP030','railway':'AN010','building':'AL015',
        'ford':'BH070','waterway':'BH140','bridge':'AQ040','tomb':'AL036',
        'railway:in_road':'AN010','mine:access':'AA010',
        'cutting':'DB070','shop':'AL015','office':'AL015'
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

    // Sort out the UID
    if (tags.uuid)
    {
      var str = tags['uuid'].split(';');
      attrs.UID = str[0].replace('{','').replace('}','').toUpperCase();
    }
    else if (tags['hoot:id'])
    {
      attrs.UID = 'raw_id:' + tags['hoot:id'];
    }
    else
    {
      // Moving to upper case as a test
      if (mgcp.configOut.OgrAddUuid == 'true') attrs.UID = createUuid().replace('{','').replace('}','').toUpperCase();
    }

    // The follwing bit of ugly code is to account for the specs haveing two different attributes
    // with similar names and roughly the same attributes. Bleah!
    if (mgcp.rules.swapListOut[attrs.F_CODE])
    {
      for (var i in mgcp.rules.swapListOut[attrs.F_CODE])
      {
        if (i in attrs)
        {
          attrs[mgcp.rules.swapListOut[attrs.F_CODE][i]] = attrs[i];
          delete attrs[i];
        }
      }
    }

    // Default railway
    // if (attrs.F_CODE == 'AN010' && tags.railway == 'yes') attrs.RRC = '0';

    // Not a great fit
    if (tags.public_transport == 'station' && tags.bus == 'yes') attrs.FFN = '481';

    // Mapping Senior Citizens home to Accomodation. Not great
    if (tags.amenity == 'social_facility' && tags['social_facility:for'] == 'senior') attrs.FFN = 550;

    // Add Weather Restrictions to transportation features
    if (['AP010','AP030','AP050'].indexOf(attrs.FCODE > -1) && !attrs.WTC )
    {
      switch (tags.highway)
      {
        case 'residential':
          attrs.WTC = '0';
          break;
          
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
        case 'unclassified':
          attrs.WTC = '1'; // All weather
          break;

        case 'path':
        case 'track':
          attrs.WTC = '2'; // Fair weather
          break;
      }

      // Use the road surface to possibly override the classification
      // We are assumeing that unpaved roads are Fair Weather only
      if (attrs.RST == '1') attrs.WTC = '1'; // Hard Paved surface
      if (attrs.RST == '2') attrs.WTC = '2'; // Unpaved surface
    }

    // Additional rules for particular FCODE's
    switch (attrs.F_CODE)
    {
      case undefined: // Break early if no value
        break;

      case 'AA050': // Well
        if (attrs.PPO && attrs.PPO !== '0' && attrs.PPO !== '122')
        {
          if (!attrs.HYP) attrs.HYP = '998'; // Not Applicable for non-water wells
          if (!attrs.SCC) attrs.SCC = '998'; // Not Applicable for non-water wells
        }
        break;

      case 'AD010': // Electric Power Plants
        if (notUsedTags['plant:output:electricity'] == 'yes') delete notUsedTags['plant:output:electricity'];
        if (notUsedTags.landuse == 'industrial') delete notUsedTags.landuse;
        if (notUsedTags.power == 'plant') delete notUsedTags.power;
        if (notUsedTags['plant:method']) delete notUsedTags['plant:method'];
        break;

      case 'AJ085': // Barn: Valid NFDD/NAS FCODE but not in the MGCP spec
        attrs.F_CODE = 'AL015'; // Barns are Buildings
        break;

      // case 'AK040': // Athletic Field, Sports Ground
      //   if (attrs.MCC)
      //   {
      //     attrs.SMC = attrs.MCC;
      //     delete attrs.MCC;
      //   }
      //   break;

      case 'AL015': // General Building
        // Unknown House of Worship
        if (tags.amenity == 'place_of_worship' && tags.building == 'other') attrs.HWT = 999;

        // AL015 doesn't use the religion tag
        delete attrs.REL;

        if (attrs.HWT && attrs.HWT !== '7' && !tags.amenity && !attrs.FFN)
        {
          attrs.FFN = '931';
        }

        if (attrs.FFN && (attrs.FFN !== '930' && attrs.FFN !== '931' && attrs.FFN !== '0'))
        {
          // Debug
          //print('AL015: Setting HWT 998');
          attrs.HWT = '998';
        }

        // This is a catch all and assigns "Commerce" as the function
        // Exceptions to this are in the rules list
        if (!attrs.FFN)
        {
          if (tags.shop || tags.office) attrs.FFN = '440';
          if (tags.industrial == 'petroleum_refining') attrs.FFN = '192';
        }
        break;

      case 'AL020': // Built-up Area
        // Allowed values for FUC
        if (attrs.FUC && ['0','1','2','4','19','999'].indexOf(attrs.FUC) < 0) attrs.FUC = '999';
        break;

      case 'AL105': // Settlement
        // Allowed values for FUC
        if (attrs.FUC && ['0','4','8','19','999'].indexOf(attrs.FUC) < 0) attrs.FUC = '999';
        break;

      case 'AL241': // 
        if (tags.man_made == 'lighthouse') attrs.TTC = '5';
        break;
        
      case 'AN010': // Railway
        if (tags.bridge) attrs.LOC = '45'; // Above Surface
        if (tags.tunnel) attrs.LOC = '40'; // Below Surface
        if (tags.embankment == 'yes')
        {
          attrs.LOC = '45'; // Above Surface
        } else if (tags.embankment || tags.man_made == 'causeway')
        {
          attrs.LOC = '44'; // On Surface
        }
        if (tags.railway == 'rail') delete attrs.RRC; // Avoid sending RRC=0 when it is "unknown"
        break;

      case 'AN076': // Railway Roundhouse
        if (attrs.railway == 'rail') delete attrs.RRC; // Avoid sending RRC=0 when it is "unknown"
        break;

      case 'AP010': // Cart Track
        if (attrs.WID && !attrs.WD1) attrs.WD1 = attrs.WID;
        break;

      case 'AP030': // Road
        if (tags.bridge) attrs.LOC = '45'; // Above Surface
        if (tags.tunnel) attrs.LOC = '40'; // Below Surface
        if (tags.embankment || tags.man_made == 'causeway') attrs.LOC = '44'; // On Surface
        if (attrs.RST == '6') attrs.RST = '2'; // Move 'ground' to 'unpaved'

        // Width not width, minimum traveled way.
        if (attrs.WID && !attrs.WD1) attrs.WD1 = attrs.WID;

        // Single lane roads dont have a median and are not separated
        // NOTE: This could cause problems
        if (attrs.LTN == '1')
        {
          attrs.SEP = '1000';
          attrs.MES = '1000';
        }
        break;

      case 'AP050': // Trail
        // Apparently, AP050 (Trail) _doesn't_ have WTC=1 (All weather)
        if (attrs.WTC == '1') attrs.WTC = '0';
        break;

      case 'AQ040': // Bridge
        if (attrs.RST)
        {
          // hoot.logWarn('Found RST = ' + attrsi.RST + ' in AQ040'); // Should not get this
          var convSurf = { 1:5, 2:46, 5:104, 6:104, 8:104, 999:999 };

          attrs.MCC = convSurf[attrs.RST];
          delete attrs.RST;
        }
        // Width not width, minimum traveled way.
        if (attrs.WID && !attrs.WD1) attrs.WD1 = attrs.WID;
        break;

      case 'AQ140': // Vehicle Lot/Vehicle Storage area: Valid NFDD/NAS FCODE but not in the MGCP spec
        if (geometryType == 'Point') attrs.F_CODE = 'AL015'; // Parking Garage Building
        break;

      case 'AT060': // Communications Cable
        // Drop these since the F_CODE does not have them and validate will try to re-add them
        delete attrs.CAB;
        delete notUsedTags.cable;
        break;

      case 'BA040': // Tidal Water
        // It's tidal so we don't need to store this
        delete attrs.TID;
        break;

      case 'BB190': // Shoreline Construction
        switch (tags.man_made) {
          case 'pier':
            attrs.PWC = '1';
            break;
          
          case 'shoreline_construction':
            attrs.PWC = '2';
            break;

          case 'quay':
            attrs.PWC = '3';
            break;
        }

        if (tags['seamark:type'] == 'shoreline_construction') attrs.PWC = '2';
        
        break;

      case 'BD130': // Hazardous Rock Point
        if (!attrs.WLE) attrs.WLE = '0'; // Unknown (for now)
        break;

      case 'BD180': // BA040 - Tidal Water
        if (notUsedTags['seamark:type'] == 'wreck') delete notUsedTags['seamark:type'];
        break;

      case 'BH140': // River
        if (!attrs.WCC) attrs.WCC = '7'; // Normal Channel
        if (!attrs.TID) attrs.TID = '1000'; // Not tidal
        if (!attrs.WST) attrs.WST = '998'; // Vanishing point - Not Applicable
        break;

      case 'DB090': // Embankment
        // If the embankment supports a transportation feature
        if (tags.highway || tags.railway)
        {
          attrs.FIC = '2'; // Fill
        }
        // else
        // {
        //   attrs.FIC = '1'; // Mound
        // }
        break;

      case 'EA010': // Crop Land
        if (attrs.CSP == '15') attrs.F_CODE = 'EA040';
        // hoot.logTrace('TRD3 feature EA010 changed to TRD4 EA040 - some data has been dropped');
        break;

      case 'EC030': // Trees
        switch (tags.leaf_cycle)
        {
          case undefined: // Break early
            break;

          case 'semi_deciduous':
            attrs.TRE = '1'; // Deciduous
            break;

          case 'semi_evergreen':
            attrs.TRE = '2'; // Evergreen
            break;
        }
        break;

      case 'ED030': // Mangrove Swamp
        if (! attrs.TID) attrs.TID = '1001'; // Tidal
        break;

      case 'FA002': // Geopolitical Entity
      case 'FA003': // Administrative Division
        // Why ????????
        if (attrs.COD == '1001')
        {
          attrs.COD = '1';
        }
        else if (attrs.COD == '1000')
        {
          attrs.COD = '2';
        }
        break;

      case 'AH050': // Fortification
        if (tags['bunker_type'] == 'munitions')
        {
          attrs.F_CODE = 'AM060'; // Surface bunker
          if (! attrs.PPO) attrs.PPO = '3'; // Ammunition
        }

        // If we have a Castle, stop it being turned into a Bunker on import
        if (tags.historic == 'castle') notUsedTags.historic = 'castle';
        break;

    } // End switch FCODE

    if (mgcp.mgcpPostRules == undefined)
    {
      // "New" style complex rules
      //
      // Rules format:  ["test expression","output result"];
      // Note: t = tags, a = attrs and attrs can only be on the RHS

      // Most of these rules are to account for differences between MGCP & TDS/NFDD
      // If we don't change it here, hoot tries to output the wrong FCODE
      var rulesList = [
      ["t.control_tower == 'yes'","a.F_CODE = 'AL241'"],
      ["t.sport == 'tennis'","a.F_CODE = 'AK040'"],
      ["t.natural == 'tree'","a.F_CODE = 'EC030'"],
      ["t.amenity == 'ferry_terminal'","a.F_CODE = 'AQ125'; a.FFN = '7'"],
      // ["t.landuse == 'forest'","a.F_CODE = 'EC030'"],
      ["t.amenity == 'fuel'","a.F_CODE = 'AL015'"]
      ];

      mgcp.mgcpPostRules = translate.buildComplexRules(rulesList);
    }

    //translate.applyComplexRules(tags,attrs,mgcp.mgcpPostRules);
    for (var i = 0, rLen = mgcp.mgcpPostRules.length; i < rLen; i++)
    {
      if (mgcp.mgcpPostRules[i][0](tags)) mgcp.mgcpPostRules[i][1](tags,attrs);
    }

    //Map alternate source date tags to SDV in order of precedence
    //default in mgcp_rules is 'source:datetime'
    if (! attrs.SDV || attrs.SDV == 'UNK')
    {
      attrs.SDV = tags['source:imagery:datetime']
        || tags['source:imagery:earliestDate']
        || tags['source:date']
        || tags['source:geometry:date']
        || tags['hoot:timestamp']
        || 'UNK';

      // UNK is the default value. OGR export will populate it.
      if (attrs.SDV == 'UNK') delete attrs.SDV;
    }

    // Chop the milliseconds off the "source:datetime"
    if (attrs.SDV)
    {
      attrs.SDV = translate.chopDateTime(attrs.SDV);
    }

    //Map alternate source tags to ZI001_SDP in order of precedence
    //default in mgcp_rules is 'source'
    if (! attrs.SDP || attrs.SDP == 'N_A')
    {
      attrs.SDP = tags['source:imagery']
        || tags['source:description']
        || tags['source:name']
        || 'N_A';

      // N_A is the default value. OGR export will populate it.
      if (attrs.SDP == 'N_A') delete attrs.SDP;
    }

    // Fix up SRT values so we comply with the spec. These values came from data files
    // Format is: orig:new
    srtFix = {
      28:112,  // Ikonos Imagery -> Hires Commercial
      66:112,  // Quickbird Multi 2.44m -> Hires Commercial
      68:110,  // Quickbird Pan 60cm -> Very Hires Commercial
    }; // End srtFix

    if (attrs.SRT in srtFix) attrs.SRT = srtFix[attrs.SRT];

    // Now, Add a SRT value if we don't have one
    // NOTE: This will get moved to be a common function later
    if ((! attrs.SRT || attrs.SRT == '0') && tags['source:imagery'])
    {
      // lowercase and wipe out spaces etc
      var tValue = tags['source:imagery'].toString().toLowerCase();
      tValue = tValue.replace(/\s/g, '');

      // Debug
      // print('tValue: ' + tValue);

      switch (tValue)
      {
        case 'unknown':
          break;

        case 'maxar':
        case 'digitalglobe':
        case 'geoeye-1c.0.4mpansharpenedmultispectralimagery':
        case 'ikonos-2c.1mpan-sharpenedmultispectralimagery':
        case 'maxar;digitalglobe':
        case 'unknown;digitalglobe':
        case 'digitalglobe;unknown':
        case 'planet':
          attrs.SRT = '110'; // Very high resolution
          break;

        case 'cnes/spot':
          attrs.SRT = '114'; // Medium Resolution
          break;

        case 'landsat':
        case 'landsat_WMS':
          attrs.SRT = '116'; // Low  Resolution
          break;

        default:
          attrs.SRT = '30';  // Imagery of unspecified type and resolution
          break;
      }
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
          attrs.RGC = '2';
          break;

        case 'broad':
          notUsedTags.gauge = tags.gauge;
          tags.gauge = 0;
          attrs.RGC = '1';
          break;
      }

      // Now work on the numbers
      var gWidth = parseInt(tags.gauge,10);

      if (!isNaN(gWidth) && gWidth > 0)
      {
        if (gWidth == 1435)
        {
          attrs.RGC = '3';
        }
        else if (gWidth < 1435) // Narrow
        {
          attrs.RGC = '2';
        }
        else
        {
          attrs.RGC = '1';
        }
        attrs.GAW = (gWidth / 1000).toFixed(3);  // Convert to Metres
      }
      else  // Not a number, cleanup time
      {
        // Dont use the value, just punt it to the OSMTAGS attribute
        delete attrs.GAW;
        notUsedTags.gauge = tags.gauge;
      }
    }

    // Uglyness needed to save a powerline type
    if (tags.spower)
    {
      notUsedTags.power = tags.spower;
      delete notUsedTags.spower;
    }

    // Crops: Many to One values
    switch (tags.crops)
    {
    case undefined: // Break early
      break;

    case 'cereal':
    case 'corn':
    case 'wheat':
    case 'soy':
    case 'forage':
    case 'field_cropland':
    case 'barley':
    case 'rape':
      attrs.CSP = '13'; // Dry Crop
      break;

    case 'vegetable':
    case 'vegetables':
    case 'market_gardening':
      attrs.CSP = '37'; // Vegetable Crop
      break;

    case 'sugarcane':
      attrs.CSP = '999'; // Other
      break;
    } // End crops


    // Trees in Orchard: Many to One values
    switch (tags.trees)
    {
    case undefined: // Break early
      break;

    case 'apple_trees':
    case 'orange_trees':
    case 'avocado_trees':
    case 'blueberry_planys':
    case 'mango_trees':
    case 'papaya_trees':
    case 'pitaya_plants':
    case 'cherry_trees':
    case 'kiwi_plants':
    case 'peach_trees':
    case 'lemon_trees':
    case 'plum_trees':
      attrs.CSP = '15'; // Fruit Trees
      break;

    case 'almond_trees':
    case 'hazel_plants':
    case 'macadamia_trees':
    case 'pecan_trees':
    case 'walnut_trees':
    case 'pistachio_trees':
      attrs.CSP = '113'; // Nut
      break;

    case 'oil_palms':
    case 'coconut_palms':
    case 'date_palms':
      attrs.CSP = '157'; // Palm
      break;

    case 'olive_trees':
    case 'coffea_plants':
    case 'pineapple_plants':
      attrs.CSP = '999'; // Other
      break;
    } // End trees

  }, // End of applyToOgrPostProcessing

  // ##### End of the xxToOgrxx Block #####

  // toOsm - Translate Attrs to Tags
  toOsm : function(attrs, layerName, geometryType)
  {
    tags = {};  // This is the output

    // Setup config variables. We could do this in initialize() but some things don't call it :-(
    // Doing this so we don't have to keep calling into Hoot core
    if (mgcp.configIn == undefined)
    {
      mgcp.configIn = {};
      mgcp.configIn.OgrAddUuid = hoot.Settings.get('ogr.add.uuid');
      mgcp.configIn.OgrDebugAddfcode = hoot.Settings.get('ogr.debug.addfcode');
      mgcp.configIn.OgrDebugDumptags = hoot.Settings.get('ogr.debug.dumptags');

      // Get any changes
      mgcp.toChange = hoot.Settings.get("schema.translation.override");
    }

    // Moved this so it gets checked for each call
    mgcp.configIn.ReaderDropDefaults = hoot.Settings.get('reader.drop.defaults');

    // Debug:
    if (mgcp.configIn.OgrDebugDumptags == 'true') translate.debugOutput(attrs,layerName,geometryType,'','In Attrs: ');

    // See if we have an o2s_X layer and try to unpack it
    if (layerName.indexOf('o2s_') > -1)
    {
      tags = translate.unpackText(attrs,'tag');

      // Throw out the reason for the o2s if it exists
      delete tags.o2s_reason;

      // Add some metadata
      if (! tags.uuid)
      {
        // Upper case as a test
        if (mgcp.configIn.OgrAddUuid == 'true') tags.uuid = createUuid().toUpperCase();
      }

      if (! tags.source) tags.source = 'mgcp:' + layerName.toLowerCase();

      // Debug:
      if (mgcp.configIn.OgrDebugDumptags == 'true')
      {
        translate.debugOutput(tags,layerName,geometryType,'','Out tags: ');
        print('');
      }

      return tags;
    } // End layername = o2s_X

    // Set up the fcode translation rules
    if (mgcp.fcodeLookup == undefined)
    {
      // Order is important:
      // First the MGCPv3 & 4 FCODES, then the common ones. This ensures that the common ones don't
      // stomp on the other ones
      mgcp.rules.fcodeOne2oneV4.push.apply(mgcp.rules.fcodeOne2oneV4,mgcp.rules.fcodeOne2oneInV3);

      // fcodeCommon.one2one.forEach( function(item) { if (~mgcp.rules.fcodeList.indexOf(item[1])) mgcp.rules.fcodeOne2oneV4.push(item); });
      fcodeCommon.one2one.forEach( function(item) { if (mgcp.rules.fcodeNameLookup[item[1]]) mgcp.rules.fcodeOne2oneV4.push(item); });

      mgcp.fcodeLookup = translate.createLookup(mgcp.rules.fcodeOne2oneV4);

      // Segregate the "Output" list from the common list. We use this to try and preserve the tags that give a many-to-one
      // translation to an FCode
      mgcp.fcodeLookupOut = translate.createBackwardsLookup(mgcp.rules.fcodeOne2oneOut);

      // Debug:
      // translate.dumpOne2OneLookup(mgcp.fcodeLookupOut);
    }

    if (mgcp.lookup == undefined)
    {
      // Setup lookup tables to make translation easier

      // Add the MGCPv3.0 specific attributes to the v4.0/common attribute table
      mgcp.rules.one2one.push.apply(mgcp.rules.one2one,mgcp.rules.one2oneIn);

      mgcp.lookup = translate.createLookup(mgcp.rules.one2one);
    }

    if (mgcp.rules.txtBiased == undefined)
    {
      mgcp.rules.txtBiased = {};
      // Add the MGCPv3.0 specific attributes to the v4.0/common attribute table
      for (var i in mgcp.rules.txtBiasedV4) mgcp.rules.txtBiased[i] = mgcp.rules.txtBiasedV4[i];
      for (var i in mgcp.rules.txtBiasedV3) mgcp.rules.txtBiased[i] = mgcp.rules.txtBiasedV3[i];
    }

    if (mgcp.rules.numBiased == undefined)
    {
      mgcp.rules.numBiased = {};
      for (var i in mgcp.rules.numBiasedV4) mgcp.rules.numBiased[i] = mgcp.rules.numBiasedV4[i];
      for (var i in mgcp.rules.numBiasedV3) mgcp.rules.numBiased[i] = mgcp.rules.numBiasedV3[i];
    }

    // Clean out the usless values
    mgcp.cleanAttrs(attrs);

    // Untangle MGCP attributes & OSM tags
    // NOTE: This could get wrapped with an ENV variable so it only gets called during import
    translate.untangleAttributes(attrs,tags,mgcp);

    // Debug:
    if (mgcp.configIn.OgrDebugDumptags == 'true')
    {
      translate.debugOutput(attrs,layerName,geometryType,'','Untangle attrs: ');
      translate.debugOutput(tags,layerName,geometryType,'','Untangle tags: ');
    }

    // pre processing
    mgcp.applyToOsmPreProcessing(attrs, layerName, geometryType);

    // Use the FCODE to add some tags
    if (attrs.F_CODE)
    {
      var ftag = mgcp.fcodeLookup['F_CODE'][attrs.F_CODE];
      if (ftag)
      {
        if (!tags[ftag[0]])
        {
          tags[ftag[0]] = ftag[1];
        }
        else
        {
          // Debug
          hoot.logWarn('Tried to replace: ' + ftag[0] + '=' + tags[ftag[0]] + '  with ' + ftag[1]);
        }
        // Debug: Dump out the tags from the FCODE
        // print('FCODE: ' + attrs.F_CODE + ' tag=' + ftag[0] + '  value=' + ftag[1]);
      }
      else
      {
        hoot.logTrace('Translation for FCODE ' + attrs.F_CODE + ' not found');
      }
    }

    // Make a copy of the input attributes so we can remove them as they get translated. Looking at what
    // isn't used in the translation - this should end up empty
    // not in v8 yet: // var tTags = Object.assign({},tags);
    var notUsedAttrs = (JSON.parse(JSON.stringify(attrs)));
    delete notUsedAttrs.F_CODE;

    // apply the simple number and text biased rules
    // NOTE: We are not using the intList paramater for applySimpleNumBiased when going to OSM
    translate.numToOSM(notUsedAttrs, tags, mgcp.rules.numBiased);
    translate.txtToOSM(notUsedAttrs, tags,  mgcp.rules.txtBiased);

    // one 2 one
    translate.applyOne2One(notUsedAttrs, tags, mgcp.lookup, {'k':'v'},[]);

    // post processing
    mgcp.applyToOsmPostProcessing(attrs, tags, layerName, geometryType);

    // If we are reading from an OGR source, drop all of the output tags with default values
    // This cleans up after the one2one rules since '0' can be a number or an enumerated attribute value
    if (mgcp.configIn.ReaderDropDefaults == 'true')
    {
      mgcp.dropDefaults(tags);
    }

    // Debug: Add the FCODE to the tags
    if (mgcp.configIn.OgrDebugAddfcode == 'true') tags['raw:debugFcode'] = attrs.F_CODE;

    // Override tag values if appropriate
    translate.overrideValues(tags,mgcp.toChange);

    // Debug:
    if (mgcp.configIn.OgrDebugDumptags == 'true')
    {
      translate.debugOutput(notUsedAttrs,layerName,geometryType,'','Not used: ');
      translate.debugOutput(tags,layerName,geometryType,'','Out tags: ');
      print('');
    }

    return tags;
  }, // End of ToOsm

  // This gets called by translateToOGR and is where the main work gets done
  // We get Tags and return Attrs and a tableName
  toOgr : function(tags, elementType, geometryType)
  {
    var returnData = []; // The array of features to return
    var transMap = {}; // A map of translated attributes
    attrs = {}; // This is the output <GLOBAL>
    attrs.F_CODE = '';

    // Setup config variables. We could do this in initialize() but some things don't call it :-(
    // Doing this so we don't have to keep calling into Hoot core
    if (mgcp.configOut == undefined)
    {
      mgcp.configOut = {};
      mgcp.configOut.OgrAddUuid = hoot.Settings.get('ogr.add.uuid');
      mgcp.configOut.OgrDebugDumptags = hoot.Settings.get('ogr.debug.dumptags');
      mgcp.configOut.OgrFormat = hoot.Settings.get('ogr.output.format');
      mgcp.configOut.OgrNoteExtra = hoot.Settings.get('ogr.note.extra');
      mgcp.configOut.OgrTextFieldNumber = hoot.Settings.get("ogr.text.field.number");
      mgcp.configOut.OgrThematicStructure = hoot.Settings.get('writer.thematic.structure');
      mgcp.configOut.OgrCodedValues = hoot.Settings.get('ogr.coded.values');
      mgcp.configOut.OgrThrowError = hoot.Settings.get('ogr.throw.error');

      // Get any changes to OSM tags
      // NOTE: the rest of the config variables will change to this style of assignment soon
      mgcp.toChange = hoot.Settings.get('schema.translation.override');
    }

    // Check if we have a schema. This is a quick way to workout if various lookup tables have been built
    if (mgcp.rawSchema == undefined)
    {
      var tmp_schema = mgcp.getDbSchema();
    }

    // The Nuke Option: If we have a relation, drop the feature and carry on
    if (tags['building:part']) return null;

    // The Nuke Option: "Collections" are groups of different feature types: Point, Area and Line
    // There is no way we can translate these to a single TDS feature
    if (geometryType == 'Collection') return null;

    // Debug:
    if (mgcp.configOut.OgrDebugDumptags == 'true') translate.debugOutput(tags,'',geometryType,elementType,'In tags: ');

    // Set up the fcode translation rules

    if (mgcp.fcodeLookup == undefined)
    {
      // Order is important:
      // Start with the TRD4 specific FCODES and then add the valid MGCP ones from the common list
      // fcodeCommon.one2one.forEach( function(item) { if (~mgcp.rules.fcodeList.indexOf(item[1])) mgcp.rules.fcodeOne2oneV4.push(item); });
      fcodeCommon.one2one.forEach( function(item) { if (mgcp.rules.fcodeNameLookup[item[1]]) mgcp.rules.fcodeOne2oneV4.push(item); });

      mgcp.fcodeLookup = translate.createBackwardsLookup(mgcp.rules.fcodeOne2oneV4);

      // Segregate the "Output" list from the common list. We use this to try and preserve the tags that give a many-to-one
      // translation to an FCode
      mgcp.fcodeLookupOut = translate.createBackwardsLookup(mgcp.rules.fcodeOne2oneOut);

      // Debug
      // translate.dumpOne2OneLookup(mgcp.fcodeLookupOut);
    }

    if (mgcp.lookup == undefined)
    {
      // Add the conversion from MGCPv3.0 attributes to the v4.0/common attribute table
      mgcp.rules.one2one.push.apply(mgcp.rules.one2one,mgcp.rules.one2oneOut);

      mgcp.lookup = translate.createBackwardsLookup(mgcp.rules.one2one);

      // Debug
      // translate.dumpOne2OneLookup(mgcp.lookup);
    }

    // Override values if appropriate
    translate.overrideValues(tags,mgcp.toChange);

    // pre processing
    mgcp.applyToOgrPreProcessing(tags, attrs, geometryType);

    // Make a copy of the input tags so we can remove them as they get translated. What is left is
    // the not used tags
    // not in v8 yet: // var tTags = Object.assign({},tags);
    var notUsedTags = (JSON.parse(JSON.stringify(tags)));

    delete notUsedTags.hoot; // Added by the UI
    // Debug info. We use this in postprocessing via "tags"
    // delete notUsedTags['hoot:id']; // Keeping this for a while

    // apply the simple number and text biased rules
    translate.numToOgr(attrs, notUsedTags, mgcp.rules.numBiasedV4,mgcp.rules.intList,transMap);
    translate.txtToOgr(attrs, notUsedTags,  mgcp.rules.txtBiasedV4,transMap);

    // one 2 one
    translate.applyOne2One(notUsedTags, attrs, mgcp.lookup, mgcp.fcodeLookup, transMap, mgcp.fcodeLookupOut);

    // post processing
    // mgcp.applyToOgrPostProcessing(attrs, tableName, geometryType);
    mgcp.applyToOgrPostProcessing(tags, attrs, geometryType, notUsedTags);

    // Now check for invalid feature geometry
    // E.g. If the spec says a runway is a polygon and we have a line, throw error and
    // push the feature to the o2s layer
    // var gFcode = geometryType.toString().charAt(0) + attrs.F_CODE;
    if (mgcp.rules.layerNameLookup.includes(geometryType.toString().charAt(0) + attrs.F_CODE))
    {
      // Check if we need to return more than one feature
      // NOTE: This returns structure we are going to send back to Hoot:  {attrs: attrs, tableName: 'Name'}
      returnData = mgcp.manyFeatures(geometryType,tags,attrs,transMap);

      // Now go through the features and clean them up
      var gType = geometryType.toString().charAt(0);

      for (var i = 0, fLen = returnData.length; i < fLen; i++)
      {
        returnData[i]['attrs']['FCODE'] = returnData[i]['attrs']['F_CODE'];
        hoot.logDebug('F_CODE: ' + returnData[i]['attrs']['F_CODE']);
        delete returnData[i]['attrs']['F_CODE'];

        // Now make sure that we have a valid feature _before_ trying to validate and jam it into the list of
        // features to return
        var gFcode = gType + returnData[i]['attrs']['FCODE'];
        gFcode = gFcode.toUpperCase();

        if (mgcp.rules.layerNameLookup.includes(gFcode))
        {
          // Validate attrs: remove all that are not supposed to be part of a feature
          mgcp.validateAttrs(geometryType,returnData[i]['attrs'],notUsedTags,transMap);

          // If we have unused tags, store them
          if (Object.keys(notUsedTags).length > 0 && mgcp.configOut.OgrNoteExtra == 'attribute')
          {
            // var tStr = '<OSM>' + JSON.stringify(notUsedTags) + '</OSM>';
            // returnData[i]['attrs']['TXT'] = translate.appendValue(returnData[i]['attrs']['TXT'],tStr,';');

            var str = JSON.stringify(notUsedTags,Object.keys(notUsedTags).sort());
            if (mgcp.configOut.OgrFormat == 'shp')
            {
              // Split the tags into a maximum of 4 fields, each no greater than 225 char long.
              var tList = translate.packText(notUsedTags,mgcp.configOut.OgrTextFieldNumber,250);
              returnData[i]['attrs']['OSMTAGS'] = tList[1];
              for (var j = 2, tLen = tList.length; j < tLen; j++)
              {
                returnData[i]['attrs']['OSMTAGS' + j] = tList[j];
              }
            }
            else
            {
              returnData[i]['attrs']['OSMTAGS'] = str;
            }
          }

          // If we are using the Thematic structre, fill the rest of the unused attrs in the schema
          if (mgcp.configOut.OgrThematicStructure == 'true')
          {
            returnData[i]['tableName'] = mgcp.rules.thematicGroupLookup[gFcode];
            mgcp.validateThematicAttrs(gFcode, returnData[i]['attrs']);
          }
          else
          {
            // returnData[i]['tableName'] = mgcp.layerNameLookup[gFcode];
            returnData[i]['tableName'] = gFcode;
          }
        }
        else
        {
          // Debug
          // print('## Skipping: ' + gFcode);
          returnData.splice(i,1);
          fLen = returnData.length;
        }
      } // End returnData loop

      // If we have unused tags, throw them into the "extra" layer
      if (Object.keys(notUsedTags).length > 0 && mgcp.configOut.OgrNoteExtra == 'file')
      {
        var extraFeature = {};
        extraFeature.tags = JSON.stringify(notUsedTags);
        extraFeature.uuid = attrs.UID;

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
    } // End else We have a feature
    else // We DON'T have a feature
    {
      // For the UI: Throw an error and die if we don't have a valid feature
      if (mgcp.configOut.OgrThrowError == 'true')
      {
        if (! attrs.F_CODE)
        {
          returnData.push({attrs:{'error':'No Valid Feature Code'}, tableName: ''});
        }
        else
        {
          //throw new Error(geometryType.toString() + ' geometry is not valid for F_CODE ' + attrs.F_CODE);
          returnData.push({attrs:{'error':geometryType + ' geometry is not valid for ' + attrs.F_CODE + ' in MGCP TRD4'}, tableName: ''});
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
        tags.o2s_reason = geometryType + ' geometry is not valid for ' + attrs.F_CODE + ' (' + mgcp.rules.fcodeNameLookup[attrs.F_CODE] + ')';;
      }

      tableName = 'o2s_' + geometryType.toString().charAt(0);

      // Debug:
      // Dump out what attributes we have converted before they get wiped out
      if (mgcp.configOut.OgrDebugDumptags == 'true') translate.debugOutput(attrs,'',geometryType,elementType,'Converted attrs: ');

      // We want to keep the hoot:id if present
      if (tags['hoot:id']) tags.raw_id = tags['hoot:id'];

      for (var i in tags)
      {
        // Clean out all of the "source:XXX" tags to save space
        if (i.indexOf('source:') !== -1) delete tags[i];
        if (i.indexOf('error:') !== -1) delete tags[i];
        if (i.indexOf('hoot:') !== -1) delete tags[i];
      }

      var str = JSON.stringify(tags,Object.keys(tags).sort());

      // Shapefiles can't handle fields > 254 chars
      // If the tags are > 254 char, split into pieces. Not pretty but stops errors
      // A nicer thing would be to arrange the tags until they fit neatly
      if (mgcp.configOut.OgrFormat == 'shp')
      {
        // Throw a warning that text will get truncated.
        if (str.length > (mgcp.configOut.OgrTextFieldNumber * 253)) hoot.logWarn('o2s tags truncated to fit in available space.');

        attrs = {};
        var tList = translate.packText(tags,mgcp.configOut.OgrTextFieldNumber,253);
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
    } // End We DON'T have a feature

    if (tags.annotated_location)
    {
      attrs.FCODE = 'ZD040';
    }

    // Debug:
    if (mgcp.configOut.OgrDebugDumptags == 'true')
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

} // End of mgcp
