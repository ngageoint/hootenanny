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
 * @copyright Copyright (C) 2020 Maxar (http://www.maxar.com/)
 */

//
// Convert OSM+ to ENCv311
//

hoot.require('config');
hoot.require('translate');

function initialize()
{
  // Set the schema type for the export
  hoot.Settings.set({'osm.map.writer.schema':'ENCv311'});
}


// Create the output Schema
function getDbSchema()
{
  if (enc311.schema == undefined) hoot.require('encv311_schema');

  // Warning: This is <GLOBAL> so we can get access to it from other functions
  enc311.rawSchema = enc311.schema.getDbSchema();

  // Add empty "extra" feature layers if needed
  enc311.rawSchema = translate.addExtraFeature(enc311.rawSchema);

  // Now add an o2s[A,L,P] feature to the enc311.rawSchema
  // We can drop features but this is a nice way to see what we would drop
  if (config.getOgrOutputFormat() == 'shp')
  {
    // Add tag1, tag2, tag3 and tag4
    enc311.rawSchema = translate.addEmptyFeature(enc311.rawSchema);
  }
  else
  {
    // Just add tag1
    enc311.rawSchema = translate.addSingleO2sFeature(enc311.rawSchema);
  }

  // Debug:
  // print('##### Start Schema #####');
  // translate.dumpSchema(enc311.rawSchema);
  // print('##### End Schema #####');

  return enc311.rawSchema;
}


// Start the translation here
enc311 = {
  validateAttrs: function (tableName,geometryType,attrs,notUsed,transMap)
  {
    var attrList = enc311.attrLookup[tableName];

    if (attrList == undefined)
    {
      hoot.logError('Validate: No attrList for ' + tableName + ' ' + geometryType);
    }
    else
    {
      for (var val in attrs)
      {
        if (attrList.indexOf(val) == -1)
        {
          // Debug
          // print('Validate: Dropping ' + val + '  from ' + tableName);
          if (val in transMap)
          {
            notUsed[transMap[val][1]] = transMap[val][2];
            // Debug:
            // print('Validate: Re-Adding ' + transMap[val][1] + ' = ' + transMap[val][2] + ' to notUsed');
          }
          delete attrs[val];

          // Since we deleted the attribute, Skip the text check
          continue;
        }

        // Now check the length of the text fields
        // We need more info from the customer about this: What to do if it is too long
        if (val in enc311.rules.txtLength)
        {
          if (attrs[val].length > enc311.rules.txtLength[val])
          {
            // First try splitting the attribute and grabbing the first value
            var tStr = attrs[val].split(';');
            if (tStr[0].length <= enc311.rules.txtLength[val])
            {
              attrs[val] = tStr[0];
            }
            else
            {
              hoot.logWarn('Validate: Attribute ' + val + ' is ' + attrs[val].length + ' characters long. Truncating to ' + enc311.rules.txtLength[val] + ' characters.');
              // Still too long. Chop to the maximum length.
              attrs[val] = tStr[0].substring(0,enc311.rules.txtLength[val]);
            }
          } // End text attr length > max length

          // It's text fo skip the next test
          continue;
        } // End in txtLength
      } // End attrs loop
    } // End else

  }, // End validateAttrs


  // findLayerName: Figure out what the layer could/should be
  findLayerName: function(tags,attrs,geometryType)
  {
    // Unique CAT to layer list
    var cList = {
    'CALSGN':'RDOSTA', 'CATAIR':'AIRARE', 'CATACH':'ACHARE', 'CATBRG':'BRIDGE', 'CATBUA':'BUAARE',
    'CATFNC':'FNCLNE', 'CATFOR':'FORSTC', 'CATGAT':'GATCON', 'CATHAF':'HRBFAC', 'CATLMK':'LNDMRK',
    'CATLND':'LNDRGN', 'CATPYL':'PYLONS', 'CATREA':'RESARE', 'CATROD':'ROADWY', 'CATRUN':'RUNWAY',
    'CATSEA':'SEAARE', 'CATSIL':'SILTNK', 'CATSLC':'SLCONS', 'CATVEG':'VEGATN', 'JRSDTN':'ADMARE'
    } // End cList

    for (var cat in cList)
    {
      if (attrs[cat]) return cList[cat];
    }

    if (tags.building || tags.office || tags.shop) return 'BUISGL'

    // Some tags imply that they are buildings but don't actually say so.
    // Most of these are taken from raw OSM and the OSM Wiki
    // Not sure if the list of amenities that ARE buildings is shorter than the list of ones that
    // are not buildings.
    var notBuildingList = [
      'artwork','atm','bbq','bench','bicycle_parking','bicycle_rental','biergarten','boat_sharing','car_sharing',
      'charging_station','clock','compressed_air','dog_bin','dog_waste_bin','drinking_water','emergency_phone',
      'ferry_terminal','fire_hydrant','fountain','game_feeding','grass_strip','grit_bin','hunting_stand','hydrant',
      'life_ring','loading_dock','nameplate','park','parking','parking_entrance','parking_space','picnic_table',
      'post_box','recycling','street_light','swimming_pool','taxi','trailer_park','tricycle_station','vending_machine',
      'waste_basket','waste_disposal','water','water_point','watering_place','yes',
      'fuel'
    ]; // End notBuildingList

    if (tags.amenity && (notBuildingList.indexOf(tags.amenity) == -1)) return 'BUISGL';

    // tourism tags that ARE buildings
    var tourismNotBuildingList = [
      'art_work','attraction','camp_pitch','camp_site','caravan_site','monument','park','picnic_site',
      'picnic_table','theme_park','viewpoint','yes','zoo'
    ]; // End tourismBuildingList

    if (tags.tourism && (tourismNotBuildingList.indexOf(tags.tourism) == -1)) return 'BUISGL';

    if (tags.natural == 'cliff') return (geometryType == 'Line') ? 'SLOTOP' : 'SLOGRD';

    if (tags.natural == 'water') return (geometryType == 'Line') ? 'RIVERS' : 'LAKARE';

    if (tags.natural == 'beach' || tags.natural == 'sand') return 'LNDRGN';

    // If nothing, send back undefined so it stays unset
    return undefined;
  }, // End findLayerName


  applyToOgrPreProcessing: function(tags, attrs, geometryType)
  {
    // Remove Hoot assigned tags for the source of the data
    delete tags['source:ingest:datetime'];
    // delete tags.area;
    delete tags['error:circular'];
    delete tags['hoot:status'];

    // Make the source a bit cleaner
    if (tags['source:geometry'] && (!tags.source))
    {
      tags.source = tags['source:geometry'];
      delete tags['source:geometry'];
    }

    // Initial cleanup
    for (var i in tags)
    {
      // Remove empty tags
      if (tags[i] == '')
      {
        delete tags[i];
        continue;
      }

      // There should be a cleaner way to do this.
      // Convert "abandoned:XXX" and "disused:XXX"features
      if ((i.indexOf('abandoned:') == 0) || (i.indexOf('disused:') == 0))
      {
        var tTag = i.replace('abandoned:','').replace('disused:','');
        tags[tTag] = tags[i];
        tags.STATUS = 'not_in_use';
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

      // Convert "demolished:XXX" features
      if (i.indexOf('demolished:') !== -1)
      {
        var tTag = i.replace('demolished:','');
        tags[tTag] = tags[i];
        tags.condition = 'destroyed';
        tags.STATUS = 'destroyed'
        delete tags[i];
        continue;
      }
    } // End Cleanup loop

    // Cleanup before Life Cycle fixes
    // Old Railway that is now a road/cycleway/bridleway etc
    if (tags.highway && (tags.railway == 'disused' || tags.railway == 'abandoned')) delete tags.railway;

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
          tags.STATUS = 'not_in_use';
          break;

        case 'destroyed':
        case 'demolished':
          tags[typ] = cycleList[typ];
          tags.condition = 'destroyed';
          tags.STATUS = 'destroyed';
          break;
      }
    } // End cycleList

    // Names
    if (tags['seamark:name'])
    {
      if (tags['seamark:name'] == tags.name)
      {
        delete tags['seamark:name'];
      }
      else if (!tags.name)
      {
        tags.name = tags['seamark:name'];
        delete tags['seamark:name'];
      }
    }

    // Countries
    if (tags['is_in:country_code'])
    {
      tags['addr:country'] = tags['is_in:country_code'];
      delete tags['is_in:country_code'];
    }
    else if (tags['is_in:country'])
    {
      tags['addr:country'] = translate.findCountryCode('c2',tags['is_in:country']);
      if (tags['addr:country'] == '') delete tags['addr:country'];
    }

    // Make sure the country code is in the right format - 2ch
    if (tags['addr:country'])
    {
      tags['addr:country'] = translate.findCountryCode('c2',tags['addr:country']);
      if (tags['addr:country'] == '') delete tags['addr:country'];
    }

    // Power plants & generation
    switch (tags.power)
    {
      case undefined:
        break;

      case 'generator':
        if (tags['generator:source'] !== 'wind' && tags['generator:source'] !== 'solar') tags.FUNCTN = 'power_station';
        break;

      case 'plant':
        if (tags.landuse == 'industrial') delete tags.landuse; // Avoid making it into a BUA
        if (tags['plant:source'] == 'wind') break;  // Windfarms are LNDMRK features
        if (tags.barrier == 'fence') delete tags.barrier; // Remove the fence from the facility
        attrs.CATPRA = '4'; // power_station
        break;

      case 'cable':
      case 'minor_line':
        tags['cable:type'] = 'power'
        break;

      case 'line':
        attrs.encLayerName = 'CBLOHD'; // Overhead cable
        tags['cable:type'] = 'transmission'
        break;
    } // End switch power

    // Fairways
    if (tags.waterway == 'fairway' && !tags['seamark:type'])
      {
        tags['seamark:type'] = 'fairway';
        delete tags.waterway;
      }


    // Uuid vs FIDN
    if (tags.uuid && tags['s57:feature_identification_number']) delete tags.uuid;

    // Remove extra/addded tags
    if (tags.natural)
    {
      if (tags.wetland || tags.water || tags.leaf_cycle) delete tags.natural;
    }

    if (tags.historic)
    {
      if (tags.memorial || tags.monument) delete tags.historic;
    }

    // Source Dates
    if (tags['source:datetime'] && !tags['source:date'])
    {
      tags['source:date'] = tags['source:datetime'];
      delete tags['source:datetime'];
    }

    if (tags['source:date']) tags['source:date'] = tags['source:date'].split('T')[0].replace(/-/g,'');

    // Start looking for a output layer
    if (tags['seamark:type'] && enc311.seaMarkToLayer[tags['seamark:type']])
    {
      attrs.encLayerName = enc311.seaMarkToLayer[tags['seamark:type']];
      // Debug
      print('Got encLayerName: ' + attrs.encLayerName);
    }

    // Now break up the seamark:XXX and seamark:XXX:YYY tags
    // NOTE: The next step is to split and translate these
    for (tag in tags)
    {
      if (tag.indexOf('seamark:') == -1) continue;

      // print('## Start: ' + tag);

      if (tag in enc311.rules.categoryTagToSeamarkType)
      {
        // print('tag ' + tag + ' in categoryTagToSeamarkType' + 'New:' + enc311.rules.categoryTagToSeamarkType[tag]);
        tags[enc311.rules.categoryTagToSeamarkType[tag]] = tags[tag];
        delete tags[tag];
        continue;
      }

      var tList = tag.split(':');

      // These get handled later
      if (tList[1] == 'type' || tList[1] == 'name') continue;

      // This looks at the last tag in seamark:XXX:YYY or seamark:YYY or seamark:XXX:1:YYYY
      var endElement = tList[tList.length -1];
      if (endElement in enc311.rules.tagToSeamarkType)
      {
        // print('End element: ' + endElement + ' value: ' + enc311.rules.tagToSeamarkType[endElement]);
        tags[enc311.rules.tagToSeamarkType[endElement]] = tags[tag];
        delete tags[tag];
        continue;
      }
      // Debug
      // print('## Got to the end:' + tag + ':');
    } // End breakup seamark

    // CATWED vs CATVEG. Sigh....
    if (tags['seamark:type'] == 'weed' && tags['seamark:vegetation:category'])
    {
      tags['seamark:weed:category'] = tags['seamark:vegetation:category'];
      delete tags['seamark:vegetation:category']
    }


    // Now split and translate the List attributes
    for (var col in tags)
    {
      // Debug
      // print('Loop: ' + col + ' ' + tags[col]);
      if (col in enc311.stringListLookup)
      {
        // Debug
      print('stringList: ' + col + ' ' + tags[col]);
        var valList = tags[col].split(';');
        var used = false;
        for (var value in valList)
        {
          // Debug
          print('Checking: '+ valList[value]);

          // Skip empty values
          if (valList[value] == '') continue;

          if (valList[value] in enc311.stringListLookup[col])
          {
            var row = enc311.stringListLookup[col][valList[value]];

            var nAttr = row[0];
            if (!attrs[nAttr])
            {
              attrs[nAttr] = row[1];
            }
            else
            {
              attrs[nAttr] = attrs[nAttr] + ',' + row[1];
            }
            // Debug
            // print('nAttr:' + nAttr + ' value: ' + attrs[nAttr]);
            used = true;
          }
          else
          {
            // Making this a Warning not an Error. Exporting has lots of funky values
            hoot.logWarn('StringList missing value ' + tags[col] + ' for ' + col);
          }
        } // End for valList

        if (used) delete tags[col];
      } // End col in Lookup
    } // End tags



  }, // End applyToOgrPreProcessing

  // #####################################################################################################

  applyToOgrPostProcessing : function (tags, attrs, geometryType, notUsedTags)
  {
    // Add a uuid/FIDN value. NOTE: it is 10 char long
    // if (!attrs.FIDN) attrs.FIDN = createUuid().replace('-','').replace('{','').replace('}','').substring(0,10);
    if (!attrs.FIDN)
    {
      attrs.FIDN = enc311.FIDN;
      enc311.FIDN = enc311.FIDN + 1;
    }

    // Primitives
    if (!attrs.PRIM)
    {
      if (tags.area || geometryType == 'Area')
      {
        attrs.PRIM = '3';
      }
      else if (geometryType == 'Line')
      {
        attrs.PRIM = '2';
      }
      else if (geometryType == 'Point')
      {
        attrs.PRIM = '1';
      }
    }

    // Cleanup
    delete notUsedTags.area;

    // Admin Boundary levels
    if (tags.admin_level)
    {
      var aLevel = parseInt(tags.admin_level,10);
      attrs.JRSDTN = (aLevel < 3) ? '2' : '3';
    }

    // Shrines & Temples
    switch (tags.religion)
    {
      case undefined:
        break;

      case 'buddhist':
        attrs.FUNCTN = '25';
        break;

      case 'shinto':
        attrs.FUNCTN = '24';
        break;
    }

    // Keep looking for an output layer
    if (!attrs.encLayerName) attrs.encLayerName = enc311.findLayerName(tags,attrs,geometryType);
    // print('Post: encLayerName: ' + attrs.encLayerName);
  }, // End applyToOgrPostProcessing


  // Convert Tags to Attributes
  toOgr: function (tags, elementType, geometryType)
  {
    var returnData = []; // The array of features to return
    var transMap = {}; // A map of translated attributes
    attrs = {}; // The output attributes

    // Try to use this as a form of UUID
    if (enc311.FIDN == undefined) enc311.FIDN = 1;

    if (enc311.rules == undefined) hoot.require('encv311_rules');

    // Setup config variables. We could do this in initialize() but some things don't call it :-(
    // Doing this so we don't have to keep calling into Hoot core
    if (enc311.configOut == undefined)
    {
      enc311.configOut = {};
      enc311.configOut.OgrDebugDumptags = config.getOgrDebugDumptags();
      enc311.configOut.OgrDebugDumpvalidate = config.getOgrDebugDumpvalidate();
      enc311.configOut.OgrNoteExtra = config.getOgrNoteExtra();
      enc311.configOut.OgrFormat = config.getOgrOutputFormat();
      enc311.configOut.OgrThrowError = config.getOgrThrowError();

      // Get any changes to OSM tags
      // NOTE: the rest of the config variables will change to this style of assignment soon
      enc311.toChange = hoot.Settings.get('schema.translation.override');
    }

    // Start processing here
    // Debug:
    if (enc311.configOut.OgrDebugDumptags == 'true') translate.debugOutput(tags,'',geometryType,elementType,'In tags: ');

    // The Nuke Option: If we have a relation, drop the feature and carry on
    if (tags['building:part']) return null;

    // The Nuke Option: "Collections" are groups of different feature types: Point, Area and Line.
    // There is no way we can translate these to a single TDS feature.
    if (geometryType == 'Collection') return null;

    // Override values if appropriate
    translate.overrideValues(tags,enc311.toChange);

    // Make a copy of the input tags so we can remove them as they get translated. What is left is the not used tags.
    // not in v8 yet: // var tTags = Object.assign({},tags);
    var notUsedTags = (JSON.parse(JSON.stringify(tags)));

    delete notUsedTags.hoot; // Added by the UI
    // Debug info. We use this in postprocessing via "tags"
    delete notUsedTags['hoot:id'];


    // Make lookup tables.
    // If the main lookup is undefined then it a good chance that the rest are
    if (enc311.lookup == undefined)
    {
      // Seamark Type -> Layer name lookup
      // In the rules it is <layer>:<tag> swap it to be <tag>:<layer>
      enc311.seaMarkToLayer = {};
      for (var i in enc311.rules.layerToSeaMark) enc311.seaMarkToLayer[enc311.rules.layerToSeaMark[i]] = i;

      enc311.stringListLookup = translate.createBackwardsLookup(enc311.rules.one2oneStringList);

      // Add "other" rules to the one2one
      enc311.rules.one2one.push.apply(enc311.rules.one2one,enc311.rules.one2oneOut);
      enc311.rules.one2one.push.apply(enc311.rules.one2one,enc311.rules.one2oneStringList);

      // Add in the extra tags that import could add
      enc311.rules.one2one.push.apply(enc311.rules.one2one,enc311.rules.extraTagList);
      enc311.rules.one2one.push.apply(enc311.rules.one2one,enc311.rules.extraStringListTagList);

      enc311.lookup = translate.createBackwardsLookup(enc311.rules.one2one);
      // Debug
      // print('#####  Lookup #####');
      // translate.dumpOne2OneLookup(enc311.lookup);
      // print('#####  Lookup #####');
    } // End enc311.lookup Undefined

    // Lookup table for vlaidation
    if (enc311.attrLookup == undefined)
    {
      var rawSchema = [];

      if (enc311.rawSchema == undefined)
      {
        // Debug
        // print('## Loading Schema');
        enc311.rawSchema = enc311.schema.getDbSchema();
      }

      enc311.attrLookup = translate.makeTdsAttrLookup(enc311.rawSchema);
      delete enc311.rawSchema; // We don't need this any more
    }

    // Testing/Debug
    // if (tags['s57:primitive'])
    // {
    //   print('Tag: ' + tags['s57:primitive'] + '  Geom: ' + geometryType);
    //   if (geometryType !== tags['s57:primitive'])
    //   {
    //     print('### Geom Different');
    //     geometryType = tags['s57:primitive'].toString();
    //   }
    // }

    // Pre Processing
    enc311.applyToOgrPreProcessing(notUsedTags, attrs, geometryType);

    // Apply the simple number and text biased rules
    // NOTE: These are BACKWARD, not forward!
    // NOTE: These delete tags as they are used
    translate.numToOgr(attrs,notUsedTags,enc311.rules.numBiased,enc311.rules.intList,transMap);
    translate.txtToOgr(attrs,notUsedTags,enc311.rules.txtBiased,transMap);

    // one 2 one
    // NOTE: This deletes tags as they are used
    // if (enc311.configOut.OgrDebugDumptags == 'true') translate.debugOutput(notUsedTags,'',geometryType,elementType,'Not before: ');
    translate.applyOne2OneQuiet(notUsedTags, attrs, enc311.lookup,transMap);
    // if (enc311.configOut.OgrDebugDumptags == 'true') translate.debugOutput(notUsedTags,'',geometryType,elementType,'Not after: ');

    // Post Processing.
    // We send the original list of tags and the list of tags we haven't used yet.
    // enc311.applyToOgrPostProcessing(tags, attrs, geometryType);
    enc311.applyToOgrPostProcessing(tags, attrs, geometryType, notUsedTags);


    // Output layer name is <ENC Object>_<P,L,A>
    var encLayerName = '';
    // Debug
    // print('attrs:encLayerName: ' + attrs.encLayerName);

    if (attrs.encLayerName)
    {
      encLayerName = attrs.encLayerName + '_' + geometryType.toString().charAt(0);

      delete attrs.encLayerName; // Don't need this anymore

      // Debug
      // Now, check that it is a valid layername & geometry
      if (!enc311.attrLookup[encLayerName])
      {
        // Debug
        // print('After: ' + encLayerName + ' is dropped');
        encLayerName = '';
      }
    }

    // If we do not have an ENC layer name then just dump the feature to O2S_X
    // if (!attrs.encLayerName)
    if (encLayerName == '')
    {
      // We don't have a feature.
      // For the UI: Throw an error and die if we don't have a valid feature
      if (enc311.configOut.getOgrThrowError == 'true')
      {
        returnData.push({attrs:{'error':'No valid layer'}, tableName: ''});
        return returnData;
      }

      // hoot.logError('Unable to assign the feature to a layer');

      var o2sLayerName = 'o2s_' + geometryType.toString().charAt(0);

      // Debug:
      // Dump out what attributes we have converted before they get wiped out
      if (enc311.configOut.OgrDebugDumptags == 'true') translate.debugOutput(attrs,'',geometryType,elementType,'Converted attrs: ');

      // We want to keep the hoot:id if present
      if (tags['hoot:id'])
      {
        tags.raw_id = tags['hoot:id'];
        delete tags['hoot:id'];
      }

      // Convert all of the Tags to a string so we can jam it into an attribute
      // var str = JSON.stringify(tags);
      var str = JSON.stringify(tags,Object.keys(tags).sort());

      // Shapefiles can't handle fields > 254 chars.
      // If the tags are > 254 char, split into pieces. Not pretty but stops errors.
      // A nicer thing would be to arrange the tags until they fit neatly
      if (enc311.configOut.OgrFormat == 'shp')
      {
        // Throw a warning that text will get truncated.
        if (str.length > 1012) hoot.logWarn('o2s tags truncated to fit in available space.');

        // NOTE: if the start & end of the substring are grater than the length of the string, they get assigned to the length of the string
        // which means that it returns an empty string.
        attrs = {tag1:str.substring(0,253),
          tag2:str.substring(253,506),
          tag3:str.substring(506,759),
          tag4:str.substring(759,1012)};
      }
      else
      {
        attrs = {tag1:str};
      }

      returnData.push({'attrs': attrs, 'tableName': o2sLayerName});

      // Debug:
      if (enc311.configOut.OgrDebugDumptags == 'true')
      {
        print('TableName : ' + returnData[0]['tableName'] + '  Geom: ' + geometryType);
        translate.debugOutput(returnData[0]['attrs'],'',geometryType,elementType,'Out attrs: ');
        print('');
      }

      return returnData;
    } // End Push to O2S

    // ##### If we get this far then we have a valid feature #####

    // Debug
    // if (enc311.configOut.OgrDebugDumptags == 'true') translate.debugOutput(notUsedTags,'',geometryType,elementType,'Not used2: ');

    // The follwing bit of ugly code is to account for the specs haveing two different attributes
    // with similar names and roughly the same attributes. Bleah!
    if (enc311.rules.swapListOut[attrs.encLayerName])
    {
      for (var i in enc311.rules.swapListOut[attrs.encLayerName])
      {
        if (i in attrs)
        {
          attrs[enc311.rules.swapListOut[attrs.encLayerName][i]] = attrs[i];
          // Debug
          // print('SwapList: swapped ' + attrs[i]);
          delete attrs[i];
        }
      }
    }

    returnData.push({'attrs': attrs, 'tableName': encLayerName});

    enc311.validateAttrs(encLayerName,geometryType,returnData[0]['attrs'],notUsedTags,transMap);

    if (Object.keys(notUsedTags).length > 0)
    {
      // if (enc311.configOut.OgrNoteExtra == 'file')
      // {
        // If we have unused tags, throw them into the "extra" layer
        var extraFeature = {};
        extraFeature.tags = JSON.stringify(notUsedTags);
        extraFeature.uuid = attrs.FIDN;

        var extraName = 'extra_' + geometryType.toString().charAt(0);

        returnData.push({'attrs': extraFeature, 'tableName': extraName});
      // }
      // else if (enc311.configOut.OgrNoteExtra == 'attribute')
      // {
      //   // If we have unused tags, add them to the memo field.
      //   var tStr = '<OSM>' + JSON.stringify(notUsedTags) + '</OSM>';
      //   attrs.MemoOther = translate.appendValue(attrs.MemoOther,tStr,';');
      // }
    }

    // Look for Review tags and push them to a review layer if found
    if (tags['hoot:review:needs'] == 'yes')
    {
      var reviewAttrs = {};

      // Note: Some of these may be "undefined"
      reviewAttrs.note = tags['hoot:review:note'];
      reviewAttrs.score = tags['hoot:review:score'];
      reviewAttrs.uuid = attrs.FIDN;
      reviewAttrs.source = tags['hoot:review:source'];

      var reviewTable = 'review_' + geometryType.toString().charAt(0);
      returnData.push({'attrs': reviewAttrs, 'tableName': reviewTable});
    } // End ReviewTags

    // Debug:
    if (enc311.configOut.OgrDebugDumptags == 'true')
    {
      for (var i = 0, fLen = returnData.length; i < fLen; i++)
      {
        print('TableName ' + i + ': ' + returnData[i]['tableName'] + '  Geom: ' + geometryType);
        translate.debugOutput(returnData[i]['attrs'],'',geometryType,elementType,'Out attrs: ');
      }
      print('');
    }

    return returnData;
  } // End of toOgr

}; // End of enc311

// ####################

// Should not need this for export!
// function translateToOsm(attrs, layerName, geometryType) { return {}; }

// EXPORT
// translateToOgr - takes 'tags' + geometry and returns 'attrs' + tableName
function translateToOgr(tags, elementType, geometryType)
{
  return enc311.toOgr(tags, elementType, geometryType);
} // End of translateToOgr


