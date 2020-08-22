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
// Convert ENCv311 to OSM+
//

hoot.require('config');
hoot.require('translate');

// Layer name filter - Filter out all layers that match this regexp
function layerNameFilter()
{
  // Drop all of the "SRC_*" and "extra_*" layers
  return '^(?!SRC_|extra_)';
}

// Start the translation here
enc311 = {
  // Find and fixup the attributes that are "lists"
  fixListAttributes: function(notUsedAttrs,tags)
  {
    // Parse the field then translate each part
    for (var col in notUsedAttrs)
    {
      // Debug
      // print('stringList: ' + col + ' ' + notUsedAttrs[col]);

      if (col in enc311.stringListLookup)
      {
        var valList = notUsedAttrs[col].split(',');
        var used = false;
        for (var value in valList)
        {
          // print('Checking:'+ valList[value] + ':');

          // Skip empty values
          if (valList[value] == '') continue;

          if (valList[value] in enc311.stringListLookup[col])
          {
            var row = enc311.stringListLookup[col][valList[value]];

            var ntag = row[0];
            if (!tags[ntag])
            {
              tags[ntag] = row[1];
            }
            else
            {
              tags[ntag] = tags[ntag] + ';' + row[1];
            }
            used = true;
          }
          else
          {
            hoot.logError('StringList missing value ' + notUsedAttrs[col] + ' for ' + col);
          }
          // Now add "extra" tags and increment them as required
          if (col in enc311.extraStringListTagLookup && valList[value] in enc311.extraStringListTagLookup[col])
          {
            var row = enc311.extraStringListTagLookup[col][valList[value]];

            var ntag = row[0];
            while (tags[ntag])
            {
              // Debug
              // print('Increment ntag: ' + ntag);
              ntag = translate.incrementTag(ntag);
            }
            tags[ntag] = row[1];
          }
        } // End for valList

        if (used) delete notUsedAttrs[col];
      } // End col in Lookup
    } // End notUsedAttrs
  }, // End fixListAttributes


  applyToOsmPreProcessing: function(attrs, layerName, geometryType)
  {
    // List of data values to drop/ignore
    var ignoreList = { '-999999.0':1,'-999999':1,'noinformation':1 };

    // This is a handy loop. We use it to:
    // 1) Remove all of the "No Information" and -999999 fields
    // 2) Convert all of the Attrs to uppercase - if needed
    for (var col in attrs)
    {
      // slightly ugly but we would like to account for: 'No Information','noInformation' etc
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

      // Now see if we need to swap attr names
      if (col in enc311.rules.swapListIn)
      {
        // Debug
        // print('Swapped: ' + enc311.rules.swapListIn[i]); // debug
        attrs[enc311.rules.swapListIn[col]] = attrs[col];
        delete attrs[col];
        continue;
      }
    } // End in attrs loop

    // Chop depths to two decimal places
    // NOTE: It starts as a string and ends up as a string
    if (attrs.VALSOU) attrs.VALSOU = '' + Number(attrs.VALSOU).toFixed(2);

  }, // End of applyToOsmPreProcessing


  // #####################################################################################################
  applyToOsmPostProcessing : function (attrs, tags, layerName, geometryType)
  {
    // Add the LayerName to the source
    if ((! tags.source) && layerName !== '')
    {
      tags.source = 'encv311:' + layerName.toLowerCase();
    }

    // We could use LNAM for this but it isn't in the UUID structure
    if (!tags.uuid) tags.uuid = createUuid();

    // Repack the StringList attributes from the list
    for (var tag in tags)
    {
      if (tag in enc311.rules.seamarkTypeTag)
      {
        if (tags['seamark:type'])
        {
          tags['seamark:' + tags['seamark:type'] + ':' + enc311.rules.seamarkTypeTag[tag]] = tags[tag];
          delete tags[tag];
          continue;
        }
        else
        {
          hoot.logError('### Missing type for ' + tag);
        }
      }
    } // End repack

    // Unpack  Notes
    if (tags.note)
    {
      var tObj = translate.unpackMemo(tags.note);

      if (tObj.tags !== '')
      {
        var tTags = JSON.parse(tObj.tags);
        for (var i in tTags)
        {
          if (tags[tTags[i]]) hoot.logWarn('Unpacking note, overwriting ' + i + ' = ' + tags[i] + '  with ' + tTags[i]);
          tags[i] = tTags[i];
        }
      }

      if (tObj.text && tObj.text !== '')
      {
        tags.note = tObj.text;
      }
      else
      {
        delete tags.note;
      }
    } // End process tags.note

    // Now add "extra" OSM tags for rendering etc
    // These add to the seamark ones.
    // E.g. seamark:building:function = post_office => amenity=post_office
    // First, add some generic tags based on the layer name. These get refined later
  
    // Save this for use later
    var smType = tags['seamark:type'];

    if (enc311.rules.extraLayerTagList[smType])
    {
      for (var i in enc311.rules.extraLayerTagList[smType])
      {
        // DOn't stomp on existing tags. E.g. bridge=aqueduct vs bridge=yes
        if (!tags[i]) tags[i] = enc311.rules.extraLayerTagList[smType][i];
      }
    }

    // Loop through the attrs and add more tags
    for (var col in attrs)
    {
      // print('stringList: ' + col + ' ' + notUsedAttrs[col]);
      if (col in enc311.extraTagLookup)
      {
        var value = attrs[col];
        // print('Checking: '+ valList[value]);
        if (value in enc311.extraTagLookup[col])
        {
          var row = enc311.extraTagLookup[col][value];

          if (row[0]) // Should not need this
          {
            // Debug
            // print('Adding: ' + row[0] + ' = ' + row[1]);
            tags[row[0]] = row[1];
          }
        }
      } // End col in Lookup
    } // End attrs


    // Add more tags if required.
    if (tags.wetland) tags.natural = 'wetland';
    if (tags.water) tags.natural = 'water';
    if (tags.leaf_cycle) tags.natural = 'wood';
    if (tags.memorial) tags.historic = 'memorial';
    if (tags.monument) tags.historic = 'monument';
    if (tags.bunker_type) tags.military = 'bunker';
    if (tags.defensive) tags['tower:type'] = 'defensive';
    if (tags['bridge:movable']) tags.bridge = 'movable';

    // Land elevations - Can either be a spot height or a contour
    if (smType == 'land_elevation')
    {
      if (tags['seamark:land_elevation:elevation']) tags.ele = tags['seamark:land_elevation:elevation'];

      switch (attrs.PRIM)
      {
        case '1': // Point
          tags.geological = 'spot_elevation';
          break;

        case '2': // Line
          tags.contour = 'elevation';
          break;
      }
    }

    // Landmarks and Towers
    if (tags.man_made == 'tower')
    {
      switch (tags.use) {
        case undefined:
          break;

        case 'radio':
          tags['tower:type'] = 'communication';
          tags['communication:radio'] = 'yes;'
          delete tags.use;
          break;

        case 'microwave':
          tags['tower:type'] = 'communication';
          tags['communication:microwave'] = 'yes;'
          delete tags.use;
          break;

        case 'television':
          tags['tower:type'] = 'communication';
          tags['communication:television'] = 'yes;'
          delete tags.use;
          break;

        case 'radar':
          tags['tower:type'] = 'radar';
          delete tags.use;
          break;

        case 'observation':
          tags['tower:type'] = 'observation';
          delete tags.use;
          break;

        case 'light_support':
          tags['tower:type'] = 'lighting';
          delete tags.use;
          break;

        case 'lookout':
          tags['tower:type'] = 'watchtower';
          delete tags.use;
          break;
      } // End switch

      // Shuffle the use tags if we deleted one
      // NOTE: this assumes we only have two. All of the data so far agrees with this
      if (tags.use1 && !tags.use)
      {
        tags.use = tags.use1;
        delete tags.use1
      }

      // Cooling tower is a bit different as it can also be a building
      if (tags.amenity == 'cooling')
      {
        tags['tower:type'] = 'cooling';
        delete tags.amenity;
      }
    }

    if (tags['tower:type'] && !tags.man_made) tags.man_made = 'tower';

    // Cables
    if (tags['cable:type'] == 'power') tags.power = 'cable';

    // Fix up areas
    // The thought is: If Hoot thinks it's an area but OSM doesn't think it's an area, make it an area.
    if (geometryType == 'Area' && !translate.isOsmArea(tags))
    {
      // Debug
      // print('## Adding area=yes');
      tags.area = 'yes';
    }

  }, // End of applyToOsmPostProcessing


  toOsm : function (attrs, layerName, geometryType)
  {
    tags = {};  // The final output Tag list

    // Load the "rules" and lookup tables for the translation
    if (enc311.rules == undefined) hoot.require('encv311_rules');

    // Setup config variables. We could do this in initialize() but some things don't call it :-(
    // Doing this so we don't have to keep calling into Hoot core
    if (enc311.configIn == undefined)
    {
      enc311.configIn = {};
      enc311.configIn.OgrDebugDumptags = config.getOgrDebugDumptags();

      // Get any changes
      enc311.toChange = hoot.Settings.get('schema.translation.override');
    }

    // Debug:
    if (enc311.configIn.OgrDebugDumptags == 'true') translate.debugOutput(attrs,layerName,geometryType,'','In attrs: ');

    // See if we have an o2s_X layer and try to unpack it.
    if (layerName.indexOf('o2s_') > -1)
    {
      tags = translate.parseO2S(attrs);

      // Add some metadata
      if (! tags.uuid) tags.uuid = createUuid();
      if (! tags.source) tags.source = 'encv311:' + layerName.toLowerCase();

      // Debug:
      if (enc311.configIn.OgrDebugDumptags == 'true')
      {
        translate.debugOutput(tags,layerName,geometryType,'','Out tags: ');
        print('');
      }

      return tags;
    } // End layername = o2s_X

    if (enc311.lookup == undefined)
    {
      // Setup lookup tables to make translation easier. I'm assumeing that since this is not set, the
      // other tables are not set either.

      // Support Import Only attributes
      enc311.rules.one2one.push.apply(enc311.rules.one2one,enc311.rules.one2oneIn);

      enc311.lookup = translate.createLookup(enc311.rules.one2one);

      // Lookup table for the things that can be a list of values
      enc311.stringListLookup = translate.createLookup(enc311.rules.one2oneStringList);

      // This is used to add more tags
      enc311.extraTagLookup = translate.createLookup(enc311.rules.extraTagList);

      // This table is for things with multiple values
      enc311.extraStringListTagLookup = translate.createLookup(enc311.rules.extraStringListTagList);

      // Debug
      // translate.dumpOne2OneLookup(enc311.lookup);
    }

    // Drop stuff we don't use
    delete attrs.SHAPE_Length;
    delete attrs.SHAPE_Area;
    delete attrs.FCSUBTYPE; // ESRI FGDB specific
    // delete attrs.PRIM;  // The Primitive type: Point, Line, Area or None

    // Debug
    // if (attrs.DEPTH && attrs.VALSOU) print('##### Got DEPTH and VALSOU');

    // pre processing
    enc311.applyToOsmPreProcessing(attrs, layerName, geometryType);

    // If we don't have an OBJL (Object Label) attribute then try to use the layerName to set the seamark:type tag
    if (!attrs.OBJL)
    {
      if (enc311.rules.layerToSeaMark[layerName])
      {
        tags['seamark:type'] = enc311.rules.layerToSeaMark[layerName];
      }
      else
      {
        hoot.logError('No type for layerName: ' + layerName);
      }
    }

    // Make a copy of the input attributes so we can remove them as they get translated. Looking at what
    // isn't used in the translation - this should end up empty.
    // not in v8 yet: // var tTags = Object.assign({},tags);
    var notUsedAttrs = (JSON.parse(JSON.stringify(attrs)));

    enc311.fixListAttributes(notUsedAttrs,tags);

    // apply the simple number and text biased rules
    // NOTE: We are not using the intList paramater for applySimpleNumBiased when going to OSM.
    translate.numToOSM(notUsedAttrs, tags, enc311.rules.numBiased);
    translate.txtToOSM(notUsedAttrs, tags, enc311.rules.txtBiased);

    // one 2 one
    //translate.applyOne2One(notUsedAttrs, tags, enc311.lookup, {'k':'v'});
    translate.applyOne2OneQuiet(notUsedAttrs, tags, enc311.lookup,{'k':'v'});

    // post processing
    enc311.applyToOsmPostProcessing(attrs, tags, layerName, geometryType);

    // Debug:
    if (enc311.configIn.OgrDebugDumptags == 'true')
    {
      translate.debugOutput(notUsedAttrs,layerName,geometryType,'','Not used: ');
      translate.debugOutput(tags,layerName,geometryType,'','Out tags: ');
      print('');
    }

    // Override tag values if appropriate
    translate.overrideValues(tags,enc311.toChange);

    return tags;
  } // End of Translate Attributes

}; // End enc311


// #####################################################################################################

// IMPORT
// translateAttributes - takes 'attrs' and returns OSM 'tags'
// NOTE: translateToOSM has a higher priority than translateAttributes
function translateAttributes(attrs, layerName, geometryType)
{
  return enc311.toOsm(attrs, layerName, geometryType);
} // End of Translate Attributes
