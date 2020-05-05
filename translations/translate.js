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
 * @copyright Copyright (C) 2013, 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

//
// Set of core translation routines
//
translate = {
  // Build Lookup tables
  createLookup : function(one2one)
  {
    // build a more efficient lookup
    var lookup = {};

    one2one.forEach( function(item) {
      // Taking this out to keep the 'undefined' values so that they get found then dropped during the transtion
      // Trying to figure out why they were being dropped.
      // if (item[2]) // Make sure it isn't 'undefined'
      // {
      if (!(item[0] in lookup)) lookup[item[0]] = {};

      if (!(lookup[item[0]][item[1]]))
      {
        lookup[item[0]][item[1]] = [item[2],item[3]];
      }
      else
      {
        if (config.getOgrDebugLookupclash() == 'true')
        {
          if (lookup[item[0]][item[1]] != ('' + item[2] + ',' + item[3]))
          {
            // Debug
            // print('Fwd Clash: ' + item[0] + ' ' + item[1] + '  is ' + lookup[item[0]][item[1]] + ' tried to change to ' + [item[2],item[3]]);
          }
        }
      }
      // }
    } );

    return lookup;
  },


  // Given a list of strings create a dictionary of those strings that point to trues.
  createBoolLookup: function(list)
  {
    var lookup = {};
    for (l in list)
    {
      lookup[l] = true;
    }

    return lookup;
  },


  // Add a value to the end of another value.
  // In the future this might sort the list of values
  appendValue : function(oldValue,newValue,sepValue)
  {
    if (sepValue === undefined) sepValue = ';';

    if (oldValue === undefined || oldValue === null || oldValue === '')
    {
      return newValue;
    }
    else
    {
      return oldValue + sepValue + newValue;
    }
  },


  // Concatinate two lists
  // NOTE: These are not arrays.
  joinList : function(listA,listB)
  {
    // Objects are passed by reference so we need to copy the lot
    // Can't just do: var newList = listA;

    // In QT, this is fractionally slower than a simple loop
    // In v8 this is faster.
    var newList = JSON.parse(JSON.stringify(listA));

    for (var i in listB)
    {
      newList[i] = listB[i];
    }

    return newList;
  },


  // Swap keys and values in a list
  flipList : function(inList)
  {
    var newList = {};

    for (var i in inList)
    {
      newList[inList[i]] = i;
    }

    return newList;
  },


  // This is used by anything that "exports" - toTds, toMGCP
  createBackwardsLookup : function(one2one)
  {
    // build a more efficient lookup
    var lookup = {};

    one2one.forEach( function(item) {
      if (item[2]) // Make sure it isn't 'undefined'
      {
        if (!(item[2] in lookup)) lookup[item[2]] = {};

        // This has been swapped around. The FCODE lists can stomp on the One2One values.
        if (!(lookup[item[2]][item[3]]))
        {
          lookup[item[2]][item[3]] = [item[0],item[1]];
        }
        else
        {
          if (config.getOgrDebugLookupclash() == 'true') print('Bkwd Clash: ' + item[2] + ' ' + item[3] + '  is ' + lookup[item[2]][item[3]] + '  tried to change to ' + [item[0], item[1]]);
        }
      }
    } );

    return lookup;
  },


  // Apply one to one translations - used for import and export
  applyOne2One : function(inList, outList, lookup, fCodeList, transMap = [])
  {
    var endChar = '',
      tAttrib = '',
      row = [];

    var kList = Object.keys(inList).sort();
    for (var i=0, kLen=kList.length; i < kLen; i++)
    {
      var key = kList[i];
      var value = inList[key];
      if (key in lookup)
      {
        if (value in lookup[key])
        {
          row = lookup[key][value];

          // Drop all of the undefined values
          if (row[0])
          {
            outList[row[0]] = row[1];
            transMap[row[0]] = [row[1],key,value];
            // Debug
            // print('One2One: row[0]=' + row[0] + ' Map:' + transMap[row[0] ]);
            // print('Used:' + key + ' = ' + inList[key] + ' - ' + row[0] + ' = ' + row[1]);
            delete inList[key];
          }
          else
          {
            // Debug
            // print('UsedUndef:' + key + ' = ' + inList[key]);
            delete inList[key];
          }
        }
        // there may be situations where this error is inappropriate. Though we haven't run
        // into them yet.
        // Oh, yes we have. -Matt
        else if (value !== '')
        {
          // If these tags are used to find an FCODE, ignore them
          if ((key in fCodeList) && (value in fCodeList[key]))
          {
            // Debug
            // print('UsedFCode:' + key + ' = ' + inList[key]);
            delete inList[key];
            continue;
          }

          hoot.logTrace('Lookup value not found for column:: (' + key + '=' + value + ')');
        }
      } // End key in lookup
      else
      {
        // If these tags are used to find an FCODE, ignore them
        if ((key in fCodeList) && (value in fCodeList[key]))
        {
          // Debug
          // print('UsedFCode:' + key+ ' = ' + inList[col]);
          delete inList[key];
          continue;
        }

        // Debug:
        // print('Col::  :' + key + ':  Value :' + value + ':');

        endChar = key.charAt(key.length - 1); // Going to look for a 2 or a 3
        tAttrib = key.slice(0,-1);

        if (tAttrib in lookup)
        {
          if (value in lookup[tAttrib])
          {
            row = lookup[tAttrib][value];

            // Drop all of the undefined values
            if (row[0])
            {
              outList[row[0] + endChar] = row[1];
              transMap[row[0]] = [row[1],tAttrib,value];
              delete inList[key];
              // Debug:
              // print ('one2one: Setting :' + row[0] + endChar + ': to :' + row[1] + ':');
            }
          }
        }
        else
        {
          // Removing the var test for a while.
          // if (config.getOgrDebugLookupcolumn() == 'true') hoot.logTrace('Column not found:: (' + key + '=' + value + ')');
          hoot.logTrace('Column not found:: (' + key + '=' + value + ')');
        }
      } // End !key in lookup
    } // End for key in inList
  }, // End applyOne2One


  // Apply one to one translations and don't report errors: missing columns etc
  applyOne2OneQuiet : function(inList, outList, lookup, transMap = [])
  {
    var row = [];

    var kList = Object.keys(inList).sort();
    for (var i=0, kLen=kList.length; i < kLen; i++)
    {
      var key = kList[i];
      var value = inList[key];
      if (key in lookup)
      {
        if (value in lookup[key])
        {
          row = lookup[key][value];

          // Drop all of the undefined values
          if (row[0])
          {
            outList[row[0]] = row[1];
            transMap[row[0]] = [row[1],key,value];
            // Debug
            // print('Fuzzy: ' + key);
            delete inList[key];
          }
          else
          {
            // Debug
            // print('UsedUndef:' + key + ' = ' + inList[key]);
            delete inList[key];
          }
        }
      }
    } // End for key in inList
  }, // End applyOne2OneQuiet


  // Apply one to one translations - For TDS export
  // This version populates the OTH field for values that are not in the rules
  applyTdsOne2One : function(inList, outList, lookup, fCodeList,transmap = [])
  {
    var endChar = '',
      tAttrib = '',
      otherVal = '',
      othVal = '',
      value = '';

    var kList = Object.keys(inList).sort();
    for (var i=0, kLen=kList.length; i < kLen; i++)
    {
      var key = kList[i];
      var value = inList[key];

      // Debug
      // print('key: ' + key + '  value: ' + value);

      if (key in lookup)
      {
        //Debug
        // print('key in Lookup');
        if (value in lookup[key])
        {
          var row = lookup[key][value];

          // Debug:
          // print('row[0]=' + row[0] + '  row[0]+endChar=' + row[0] + endChar);

          // Drop all of the undefined values and only continue if we have a value
          if (row[0])
          {
            // Make sure that we don't stomp on already assigned values
            // unless the first n-1 characters are identical then we replace with the higher value
            // (used for cases of where the more specific ffn code is a higher value)
            // NOTE: we could add a verify step to this to make sure that the XXX2, XXX3 values
            // are valid
            if (! outList[row[0]] || outList[row[0]].slice(0, -1) === row[1].slice(0, -1))
            {
              outList[row[0]] = this.getMaxAsString(outList[row[0]], row[1]);
              // Debug
              // print('Used:' + key + ' = ' + inList[key]);
              delete inList[key];
            }
            else if (! outList[row[0] + '2'] || outList[row[0] + '2'].slice(0, -1) === row[1].slice(0, -1))
            {
              outList[row[0] + '2'] = this.getMaxAsString(outList[row[0] + '2'], row[1]);
              // Debug
              // print('Used:' + key + ' = ' + inList[key] + ' as 2nd');
              delete inList[key];
            }
            else if (! outList[row[0] + '3'] || outList[row[0] + '3'].slice(0, -1) === row[1].slice(0, -1))
            {
              outList[row[0] + '3'] = this.getMaxAsString(outList[row[0] + '3'], row[1]);
              // Debug
              // print('Used:' + key + ' = ' + inList[key] + ' as 3rd');
              delete inList[key];
            }
            else
            {
              hoot.logWarn('Could not assign: ' + key + ' = ' + inList[key] + ' to the 4th or greater value of ' + row[0]);
            }
          }
          else
          {
            // Debug
            // print('UsedUndef:' + key + ' = ' + inList[key]);
            delete inList[key];
          }

        }
        // there may be situations where this error is inappropriate. Though we haven't run
        // into them yet.
        // Oh, yes we have. -Matt
        else if (value !== '')
        {
          // If these tags are used to find an FCODE, ignore them
          if ((key in fCodeList) && (value in fCodeList[key]))
          {
            // Debug
            // print('UsedFCode:' + key + ' = ' + inList[key]);
            delete inList[key];
            continue;
          }

          hoot.logDebug('Lookup value not found for column:: (' + key + '=' + value + ')');

          // The following is used for export. If we have an attribute value that can't
          // find a rule for, we add it to the OTH Field.
          otherVal = lookup[key]['other'];

          if (otherVal)
          {
            // Build the OTH value
            othVal = '(' + otherVal[0] + ':' + value + ')';
            outList.OTH = translate.appendValue(outList.OTH,othVal,' ');

            hoot.logDebug('Adding to OTH field:: ' + othVal);

            // Set the output attribute to "other"
            outList[otherVal[0]] = otherVal[1];

            // Debug
            // print('UsedOTH:' + key + ' = ' + inList[key]);
            delete inList[key];

          } // End if otherVal
          else
          {
            hoot.logDebug('Could not add ::' + key + '=' + value + ':: to the OTH field');
          }
        } // End value != ''
      } // End key in lookup
      else
      {
        // If we didn't find the tag, check if it is used to find an FCODE. If so, mark it as used
        if ((key in fCodeList) && (value in fCodeList[key]))
        {
          // Debug
          // print('UsedX:' + key + ' = ' + inList[key]);
          delete inList[key];
        }
        else
        {
          if (config.getOgrDebugLookupcolumn() == 'true') hoot.logTrace('Column not found:: (' + key + '=' + value + ')');
        }
      } // End !key in lookup
    } // End for key in inList

  }, // End applyTdsOne2One


  // Translate XX2, XX3 etc attributes
  fix23Attr: function (inAttrs, outTags, lookup)
  {
    for (var col in inAttrs)
    {
      if (col == 'F_CODE') continue;

      // The OTH unpacker handles these
      if (inAttrs[col] == '999') continue;

      var endChar = col.charAt(col.length - 1);
      var value = inAttrs[col];

      // Yes, there are attributes that go to 5. But, not to 11
      if (['2','3','4','5'].indexOf(endChar) > -1)
      {
        var tAttrib = col.slice(0,-1);

        if (tAttrib in lookup && value in lookup[tAttrib])
        {
          var row = lookup[tAttrib][value];

          if (row[0])
          {
            // If we don't already have one of these tags, assign it.
            if (! outTags[row[0]])
            {
              outTags[row[0]] = row[1];
            }
            else
            {
              // Check for duplicate attributes
              if (outTags[row[0]] !== row[1])
              {
                outTags[row[0] + ':' + endChar] = row[1];
              }
            }
            // Debug
            //print('New Tag:' + row[0] + ':' + endChar+ ' = ' + outTags[row[0] + ':' + endChar] );
            delete inAttrs[col];
          }
        }

      } // End 2,3,4,5
    } // End for inAttrs
  }, // End fix23Attr


  // Translate XXX:2, XXX:3 etc tags and populate the OTH when appropriate
  // NOTE: We also handle XXX2, XXX3 etc since we need to be backwards compatible
  fix23Tags: function (inTags, outAttrs, lookup)
  {
    for (var col in inTags)
    {
      var endChar = col.charAt(col.length - 1);
      var value = inTags[col];

      // Yes, there are attributes that go to 5. But, not to 11
      if (['2','3','4','5'].indexOf(endChar) > -1)
      {
        var tAttrib = '';

        if (col.charAt(col.length - 2) == ':')
        {
          tAttrib = col.slice(0,-2);
        }
        else
        {
          tAttrib = col.slice(0,-1);
        }

        if (tAttrib in lookup)
        {
          if (value in lookup[tAttrib])
          {
            var row = lookup[tAttrib][value];

            if (row[0])
            {
              outAttrs[row[0] + endChar] = row[1];
              delete inTags[col];
              // Debug
              // print('New Attr:' + row[0] + endChar+ ' = ' + outAttrs[row[0] + endChar] );
            }
          } // End value in lookup
          else if (value !== '') // Being defensive...
          {
            var otherVal = lookup[tAttrib]['other'];

            if (otherVal)
            {
              // Build the OTH value
              othVal = '(' + otherVal[0] + endChar + ':' + value + ')';
              outAttrs.OTH = translate.appendValue(outAttrs.OTH,othVal,' ');

              hoot.logTrace('Adding to OTH field:: ' + othVal);

              // Set the output attribute to "other"
              outAttrs[otherVal[0] + endChar] = otherVal[1];

              // Debug
              // print('UsedOTH:' + col + ' = ' + inTags[col]);
              delete inTags[col];
            }
            else
            {
              hoot.logTrace('Could not add ::' + col + '=' + value + ':: to the OTH field');
            }
          } // End value != ''
        }
      } // End 2,3,4,5
    } // End for inTags

  }, // End fix23Tags


  // Parse o2s_X tags
  parseO2S : function(attrs)
  {
    var outTags = {};

    // Check if the tags got split
    var tTags = attrs.tag1;
    if (attrs.tag2) tTags = tTags + attrs.tag2;
    if (attrs.tag3) tTags = tTags + attrs.tag3;
    if (attrs.tag4) tTags = tTags + attrs.tag4;

    // If the JSON looks complete then parse it
    if (tTags.charAt(0) == '{' && tTags.charAt(tTags.length-1) == '}')
    {
      outTags = JSON.parse(tTags);
    }
    else
    {
      // Bad o2s_X. Usual cause is writing > 254 char to a shapefile attribute
      // We are expecting something that got chopped like this:
      //  {"source":"Tdh","building":"yes","statu

      // Wipe out JSON fragments
      if (tTags.charAt(0) == '{') tTags = tTags.slice(1);
      // Just in case...
      if (tTags.charAt(tTags.length-1) == '}') tTags = tTags.slice(-1);

      // Now get rid of begining and/or ending '"' to make the next string split cleaner
      if (tTags.charAt(0) == '"') tTags = tTags.slice(1);
      if (tTags.charAt(tTags.length-1) == '"') tTags = tTags.slice(-1);

      var tArray = tTags.split('","');

      for (var i in tArray)
      {
        // Now split each key:value pair and only keep complete pairs
        // Each pair should look like: building":"yes
        var j = tArray[i].split('":"');
        if (j[1])
        {
          outTags[j[0]] = j[1];
        }
      }
    } // End else Bad o2s

    return outTags;
  },


  // Parse the note:extra tag and return an associative array of key/value pairs
  parseNoteExtra : function(rawNote)
  {
    var outList = {};//,tmpList = rawNote.split(';');

    // for (var i = 0, len = tmpList.length; i < len; i++)
    // {
    //     noteVal = tmpList[i].split(':'); // Split into Key/Value

    //     // If we have a Key then add it to the output
    //     if (noteVal[0]) outList[noteVal[0]] = noteVal[1];
    // }

    rawNote.split(';').forEach( function (item) {
      var noteVal = item.split(':'); // Split into Key/Value
      // If we have a Key then add it to the output
      if (noteVal[0]) outList[noteVal[0]] = noteVal[1];
    });

    return outList;
  },


  // Parse the OTH value and return an associative array of key/value pairs
  // According to the spec, the format of the OTH field is:
  //      (<Attr>:<Value>) (<Attr>:<Value>.....)
  parseOTH : function(rawOTH)
  {
    // var othVal = [],
    //     outList = {},
    //     tVal = '',
    //     tVal = rawOTH.replace(/\) \(/g,'#'); // Swap ') (' for #)

    //     tVal = tVal.replace(/[\(\)]/g,''); // Get rid of ( and )

    //     tmpList = tVal.split('#'); // Split on #

    // for (var i = 0, len = tmpList.length; i < len; i++)
    // {
    //     othVal = tmpList[i].split(':'); // Split into Key/Value

    //     // If we have a Key _and_ a value, then add it to the output
    //     if (othVal[0] && othVal[1]) outList[othVal[0]] = othVal[1];
    // }

    var outList = {};

    // 1) Swap ') (' for #)
    // 2) Get rid of ( and )
    // 3) Split on '#'
    rawOTH.replace(/\) \(/g,'#').replace(/[\(\)]/g,'').split('#').forEach( function (item) {
      var othVal = item.split(':'); // Split into Key/Value
      // If we have a Key _and_ a value, then add it to the output
      if (othVal[0] && othVal[1]) outList[othVal[0]] = othVal[1];
    });

    // Debug:
    // for (var j in outList) print('parseOTH: k=  :' + j + ':  v= :' + outList[j] + ':');

    return outList;
  },


  packOTH : function(rawList)
  {
    var othVal = '',
      othField = '';

    // Now sort the list. Not needed for the spec but it makes life easier when comparing attributes in the output
    var tList = Object.keys(rawList).sort();
    for (var i = 0, fLen = tList.length; i < fLen; i++)
    {
      othVal = '(' + tList[i] + ':' + rawList[tList[i]] + ')';
      othField = translate.appendValue(othField, othVal, ' ');
    }

    return othField.trim();
  }, // End packOTH


  // Process OTH values and convert them to tags
  processOTH : function(attrs, tags, lookup)
  {
    var othList = translate.parseOTH(attrs.OTH),
      key = [],
      tAttrib = '',
      tValue = '',
      endChar = '';

    for (var i in othList)
    {
      // Debug:
      // print('processOTH i: ' + i + '  atr= :' + attrs[i] + ':');

      if (attrs[i])
      {
        endChar = i.charAt(i.length - 1); // Going to look for a 2 or a 3
        tValue = attrs[i];

        if (endChar == '2' || endChar == '3')
        {
          tAttrib = i.slice(0,-1);
        }
        else
        {
          tAttrib = i;
          endChar = '';
        }
      }
      else
      {
        hoot.logTrace('OTH:: Attribute :' + i + ': is supposed to be 999/Other. It is not set. Skipping it.');
        continue;
      } // End !attrsi[]


      // Debug:
      // print('OTH::  tAttrib:' + tAttrib + ':  tValue:' + tValue + ':  endChar:' + endChar + ':');

      if (tValue !== '999')
      {
        hoot.logTrace('OTH:: Attribute :' + i + ': is supposed to be 999/Other. It is :' + tValue + ':. Skipping it.');
        continue;
      }

      if (i in lookup)
      {
        // Find what tag the attribute belongs to
        key = lookup[i]['999'];
        if (!key) key = lookup[i]['other'];

        if (key)
        {
          // Debug:
          // print('OTH:: Std adding :' + othList[i] + ': to tag :' + key[0] + ': (From ' + i + ')');
          tags[key[0]] = othList[i];
          delete othList[i];
        }
        else
        {
          hoot.logError('OTH:: Did not find an "Other" value for ' + i);
        }
      }
      else if (tAttrib in lookup)
      {
        // Find what tag the attribute belongs to
        key = lookup[tAttrib]['999'];
        if (!key) key = lookup[tAttrib]['other'];

        if (key)
        {
          // Add a 2 or a 3 to the "key"
          tags[key[0] + ':' + endChar] = othList[i];
          // Debug:
          // print('OTH:: adding :' + othList[i] + ': to tag :' + key[0] + endChar + ': (From ' + i + ')');
          delete othList[i];
        }
        else
        {
          hoot.logError('OTH:: Did not find an "Other" value for ' + i);
        }
      }
      else // !tAttrib in lookup
      {
        hoot.logError('OTH:: ' + i + ' does not exist in lookup');
      }
    } // End for i

    // Now rebuild the "note:oth" tag with whatever is left in othList
    var othVal = '';
    delete tags['note:oth'];

    for (var i in othList)
    {
      othVal = '(' + i + ':' + othList[i] + ')';
      tags['note:oth'] = translate.appendValue(tags['note:oth'],othVal,' ');
    }
  }, // End processOTH


  // Unpack <OSM>XXX</OSM> from TXT/MEMO fields
  unpackMemo : function(rawMemo)
  {
    var tgs = '';
    var txt = '';

    var sIndex = rawMemo.indexOf('<OSM>');

    if (sIndex > -1)
    {
      var eIndex = rawMemo.indexOf('</OSM>');

      if (eIndex > -1)
      {
        tgs = rawMemo.slice(sIndex + 5, eIndex);

        if (sIndex > 0)
        {
          txt = rawMemo.substring(0,sIndex) + rawMemo.substring(eIndex + 6);
        }

        // If the </OSM> tag was at the end, remove the ';' delimiter from the text
        if (txt.charAt(txt.length - 1) == ';') txt = txt.slice(0,-1);
      }
      else
      {
        hoot.logWarn('Missing OSM end tag in: ' + rawMemo);
      }
    }
    else
    {
      txt = rawMemo;
    }

    return {tags:tgs,text:txt};
  },


  addName : function(attrs, names, col)
  {
    // if (col in attrs && attrs[col] !== '' && attrs[col] !== 'N_A' && attrs[col] !== 'UNK')
    if (!translate.isUnknown(attrs[col]))
    {
      n = attrs[col].replace('\\', '\\\\');
      n = attrs[col].replace(';', '\\;');
      names.push(n);
    }
  },


  swapName : function(tags)
  {
    // If we have an English name, make it the main "name"
    // This is run after a check for the existance of "name"
    if (tags['name:en'])
    {
      tags.name = tags['name:en'];
      delete tags['name:en'];
    }
  },


  fixConstruction : function(tags, key)
  {
    if ('condition' in tags && key in tags && tags.condition == 'construction' && tags[key] != '')
    {
      tags.construction = tags[key];
      tags[key] = 'construction';
      delete tags.condition;
    }
  },


  // Returns true the feature is an OSM area feature
  // This is based on the OSM idea of what is considered an area
  // http://wiki.openstreetmap.org/wiki/Overpass_turbo/Polygon_Features
  isOsmArea : function(tags)
  {
    var result = false;

    if (translate.areaList == undefined)
    {
      translate.areaList = {
        'amenity':undefined,
        'area:highway':undefined,
        'barrier':{'city_wall':1,'ditch':1,'hedge':1,'retaining_wall':1,'wall':1,'spikes':1},
        'boundary':undefined,
        'building':undefined,
        'building:part':undefined,
        'craft':undefined,
        'golf':undefined,
        'highway':{'services':1, 'rest_area':1, 'escape':1, 'elevator':1},
        'historic':undefined,
        'indoor':undefined,
        'landuse':undefined,
        'leisure':undefined,
        'office':undefined,
        'place':undefined,
        'power':{'plant':1,'substation':1,'generator':1,'transformer':1},
        'public_transport':undefined,
        'railway':{'station':1,'turntable':1,'roundhouse':1,'platform':1},
        'ruins':undefined,
        'shop':undefined,
        'tourism':undefined,
        'waterway':{'riverbank':1,'dock':1,'boatyard':1,'dam':1}
      };
    } // End

    if (tags.natural)
    {
      if (['no','coastline','cliff','ridge','arete','tree_row'].indexOf(tags.natural) == -1)
      {
        result = true;
      }
    }
    else if (tags.man_made)
    {
      if (['no','cutline','embankment','pipeline'].indexOf(tags.man_made) == -1)
      {
        result = true;
      }
    }
    else if (tags.aeroway && tags.aeroway !== 'taxiway')
    {
      result = true;
    }
    else
    {
      // Now loop through the tags
      for (var i in tags)
      {
        if (i in translate.areaList)
        {
          if (tags[i] == 'no') continue;

          if (translate.areaList[i] == undefined)
          {
            result = true;
            break;
          }

          if (tags[i] in translate.areaList[i])
          {
            result = true;
            break;
          }
        }
      }
    } // End else

    return result;
  },


  /**
     * Returns true if the col in attr is empty.
     */
  isEmpty : function(v)
  {
    var result = false;
    if (v === '' || v === undefined)
    {
      result = true;
    }
    return result;
  },


  // isOK - A combination of isNumber and IsUnknown to make life easier for value checks
  isOK : function(v)
  {
    var result = true;

    if (v === '' || v === '0' || v == 0.0 || v == -32768.0 || v == -32767.0 || v === '-999999' || v === undefined)
    {
      result = false;
    }
    else if (isNaN(parseFloat(v)) || !isFinite(v))
    {
      result = false;
    }

    // Debug
    // print('### isOK: ' + v + ' : ' + result);
    return result;
  },


  // This is a bit backwards: If it is Unknown it is true...
  isUnknown : function(v)
  {
    var result = false;

    // Exploit the object
    var dropList = {'No Information':1,
      'UNK':1, 'Unk':1,
      'N_A':1, 'N/A':1,'NA':1,
      '-999999':1,
      '0':1,
    };

    // For some reason it is not matching -32768 or -32767.
    // It prints "-32768.00000000000000"
    // After playing, I found the two equals and the .0 helped
    // if (v === '' || v === '0' || v == 0.0 || v == -32768.0 || v == -32767.0 || v === '-999999' || v === undefined)
    if (v === '' || v == 0.0 || v == -32768.0 || v == -32767.0 || v === undefined)
    {
      result = true;
    }
    // Clean out some of the Text Fields
    else if (v in  dropList) result = true;

    // Debug
    // print('## isUnknown: ' + v + ' : ' + result);
    return result;
  },


  isNumber : function(n)
  {
    return !isNaN(parseFloat(n)) && isFinite(n);
  },


  // Chop a datetime field down to a single value and get it to 20 characters long for export to MGCP & TDS
  chopDateTime : function(rawDateTime)
  {
    var finalDateTime = '';

    var tmpList = rawDateTime.split(';');

    // NOTE: This is a workaround untill we can get the "reference dataset" datetime
    // Sort the list so we grab the earliest date.
    tmpList.sort();

    finalDateTime = tmpList[0];

    // Try chopping the milliseconds off the datetime
    if (finalDateTime.length > 20)
    {
      finalDateTime = finalDateTime.replace(/\.\d\d\dZ$/,'Z');
    }

    return finalDateTime;
  },

  // For backwards compatibility
  applySimpleTxtBiased : function(attrs, tags, rules, direction)
  {
    if (direction == 'forward')
    {
      translate.txtToOSM(attrs,tags,rules);
    }
    else
    {
      translate.txtToOgr(attrs,tags,rules,[]);
    }
  }, // End applySimpleNumBiased


  // txtToOSM - convert  text attributes to tags
  txtToOSM : function(attrs, tags, rules)
  {
    // Convert Attrs to Tags
    for (var i in rules)
    {
      if (i in attrs)
      {
        tags[rules[i]] = attrs[i];
        // Debug
        // print('UsedTxt: ' + attrs[i]);
        delete attrs[i];
      }
    }
  }, // End txtToOSM


  // txtToOgr - Convert Text tags to attributes
  txtToOgr : function(attrs, tags, rules, transMap)
  {
    // convert Tags to Attrs
    for (var i in rules)
    {
      if (rules[i] in tags)
      {
        attrs[i] = tags[rules[i]];
        // Debug
        // print('Txt Bwd: i=' + i + ' rules=' + rules[i] + ' attrs=' + tags[rules[i]] + ' tMap:' + transMap + ':');
        // print('UsedTxt: ' + rules[i]);
        transMap[i] = ['',rules[i],tags[rules[i]]];
        delete tags[rules[i]];
      }
    }
  }, // End txtToOgr


  // For backwards compatibility
  applySimpleNumBiased : function(attrs, tags, rules, direction, intList)
  {
    if (direction == 'forward')
    {
      translate.numToOSM(attrs,tags,rules);
    }
    else
    {
      translate.numToOgr(attrs,tags,rules,intList,[]);
    }
  }, // End applySimpleNumBiased


  // numToOSM - Convert Number attributes to tags
  numToOSM : function(attrs, tags, rules)
  {
    // Convert Attrs to Tags
    for (var i in rules)
    {
      if (i in attrs)
      {
        // Just checking it is a number. Dont care if it is an Int or a Real
        if (translate.isNumber(attrs[i]))
        {
          tags[rules[i]] = attrs[i];
          // Debug
          // print('UsedNum: ' + attrs[i]);
          delete attrs[i];
        }
        else
        {
          hoot.logTrace('Expected a number for:: ' + i + '. Got ' + attrs[i] + ' instead. Skipping ' + i);
        }
      }
    }
  },

  // numToOgr - Convert number tags to attributes
  numToOgr : function(attrs, tags, rules, intList, transMap)
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
          // Debug
          // print('Num Ogr: i=' + i + ' rules=' + rules[i] + ' tags=' + tags[rules[i]]);
          // print ('UsedNum: ' + rules[i]);
          delete tags[rules[i]];
        }
        else
        {
          hoot.logTrace('Expected a number for:: ' + rules[i] + '. Got ' + tags[rules[i]] + ' instead. Skipping ' + i);
        }
      }
    }
  }, // End numToOgr


  // buildComplexRules
  // Build the lookup table for the applyComplexRules function
  buildComplexRules : function (rawList)
  {
    var rulesFunction = [];

    rawList.forEach( function (item) {
      rulesFunction.push([new Function('t', 'return ' + item[0]), new Function('t','a', item[1])]);
      // print('Rules: Added ' + item[0]);
    });

    return rulesFunction;
  }, // End buildComplexRules


  // applyComplexRules - Apply rules that are more complex than one to one.
  // A "new" way of specifying rules. Jason came up with this while playing around with NodeJs
  // Rules format:  ["test expression","output result"];
  // Note: t = tags, a = attrs and attrs can only be on the RHS
  applyComplexRules : function(tgs, atrs, rulesList)
  {
    /*
        var rulesFunction = [];

        for (var i = 0, rLen = rulesList.length; i < rLen; i++)
        {
            rulesFunction.push([new Function('t', 'return ' + rulesList[i][0]), new Function('t','a', rulesList[i][1])]);
        }

        function applyRules(t,a)
        {
            for (var i = 0, rLen = rulesFunction.length; i < rLen; i++)
            {
                if (rulesFunction[i][0](t)) rulesFunction[i][1](t,a);
            }
        }
    */
    rulesList.forEach( function (item) {
      if (item[0](tgs)) item[1](tgs,atrs);
    });
  }, // End applyComplexRules


  // makeAttrLookup - build a lookup table for layers and Attrs
  makeTdsAttrLookup : function(schema)
  {
    var lookup = {};

    schema.forEach( function (item) {
      lookup[item.name] = [];
      item.columns.forEach( function (column) {
        lookup[item.name].push(column.name);
      });
    });

    return lookup;
  },


  // makeAttrLookup - build a lookup table for FCODEs and Attrs
  makeAttrLookup : function(schema)
  {
    var lookup = {};

    // Add the attrArray to the list as <geom><FCODE>:[array]
    // Eg[L,A,P]AP030:[array]
    schema.forEach( function (item) {
      var iName = item.geom.charAt(0) + item.fcode;
      lookup[iName] = [];
      item.columns.forEach( function (column) {
        lookup[iName].push(column.name);
      });
    });

    return lookup;
  },


  // makeLayerNameLookup - build a lookup table for FCODE to LayerName
  makeLayerNameLookup : function(schema)
  {
    var lookup = {};

    schema.forEach( function (item) {lookup[item.geom.charAt(0) + item.fcode] = item.name;});

    return lookup;
  },


  // addReviewFeature - Add Review features to a schema
  addReviewFeature: function(schema)
  {
    schema.push({ name:'review_A',desc:'Review features',geom:'Area',
      columns:[ {name:'score',desc:'Review Score',type:'String'/*,length:'254'*/},
        {name:'note',desc:'Review note',type:'String'/*,length:'254'*/},
        {name:'source',desc:'Review source',type:'String'/*,length:'254'*/},
        {name:'uuid',desc:'Review uuid',type:'String',defValue:''/*,length:'254'*/}
      ]
    });
    schema.push({ name:'review_L',desc:'Review features',geom:'Line',
      columns:[ {name:'score',desc:'Review Score',type:'String'/*,length:'254'*/},
        {name:'note',desc:'Review note',type:'String'/*,length:'254'*/},
        {name:'source',desc:'Review source',type:'String'/*,length:'254'*/},
        {name:'uuid',desc:'Review uuid',type:'String',defValue:''/*,length:'254'*/}
      ]
    });
    schema.push({ name:'review_P',desc:'Review Features',geom:'Point',
      columns:[ {name:'score',desc:'Review Score',type:'String'/*,length:'254'*/},
        {name:'note',desc:'Review note',type:'String'/*,length:'254'*/},
        {name:'source',desc:'Review source',type:'String'/*,length:'254'*/},
        {name:'uuid',desc:'Review uuid',type:'String',defValue:''/*,length:'254'*/}
      ]
    });

    return schema;
  }, // End addReviewFeature


  // addEmptyFeature - Add o2s features to a schema
  addEmptyFeature: function(schema)
  {
    schema.push({ name:'o2s_A',desc:'o2s',geom:'Area',
      columns:[ {name:'tag1',desc:'Tag List',type:'String',length:'8192'/*,length:'254'*/},
        {name:'tag2',desc:'Tag List',type:'String',defValue:'',length:'254'},
        {name:'tag3',desc:'Tag List',type:'String',defValue:'',length:'254'},
        {name:'tag4',desc:'Tag List',type:'String',defValue:'',length:'254'}
      ]
    });
    schema.push({ name:'o2s_L',desc:'o2s',geom:'Line',
      columns:[ {name:'tag1',desc:'Tag List',type:'String',length:'8192'/*,length:'254'*/},
        {name:'tag2',desc:'Tag List',type:'String',defValue:'',length:'254'},
        {name:'tag3',desc:'Tag List',type:'String',defValue:'',length:'254'},
        {name:'tag4',desc:'Tag List',type:'String',defValue:'',length:'254'}
      ]
    });
    schema.push({ name:'o2s_P',desc:'o2s',geom:'Point',
      columns:[ {name:'tag1',desc:'Tag List',type:'String',length:'8192'/*,length:'254'*/},
        {name:'tag2',desc:'Tag List',type:'String',defValue:'',length:'254'},
        {name:'tag3',desc:'Tag List',type:'String',defValue:'',length:'254'},
        {name:'tag4',desc:'Tag List',type:'String',defValue:'',length:'254'}
      ]
    });

    return schema;
  }, // End addEmptyFeature


  // addExtraFeature - Add features to hold 'extra' tag values to a schema
  addExtraFeature: function(schema)
  {
    schema.push({name:'extra_A',desc:'extra tag values',geom:'Area',
      columns:[{name:'tags',desc:'Tag List',type:'String'},
        {name:'uuid',desc:'Feature uuid',type:'String',defValue:''}
      ]
    });
    schema.push({ name:'extra_L',desc:'extra tag values',geom:'Line',
      columns:[{name:'tags',desc:'Tag List',type:'String'/*,length:'254'*/},
        {name:'uuid',desc:'Feature uuid',type:'String',defValue:''}
      ]
    });
    schema.push({name:'extra_P',desc:'extra tag values',geom:'Point',
      columns:[{name:'tags',desc:'Tag List',type:'String'/*,length:'254'*/},
        {name:'uuid',desc:'Feature uuid',type:'String',defValue:''}
      ]
    });

    return schema;
  }, // End addExtraFeature


  // addEtds - Add the eLTDS specific fields to each element in the schema
  addEtds: function(schema)
  {
    schema.forEach( function (item) {
      item.columns.push({name:'SCAMIN',desc:'Scale - Minimum',type:'Integer',optional:'R',defValue:'-999999'});
      item.columns.push({name:'SCAMAX',desc:'Scale - Maximum',type:'Integer',optional:'R',defValue:'-999999'});
      item.columns.push({name:'LINK_ID',desc:'Link Id',type:'String',optional:'R',defValue:'No Information'});
    });

    return schema;
  }, // End addEtds


  // addFCSubtype - Add the ESRI specific FCSUBTYPE field to each element in the schema
  addFCSubtype: function(schema)
  {
    schema.forEach( function (item) {
      item.columns.push({name:'FCSUBTYPE',desc:'Feature Code Subtype',type:'Integer',optional:'R',defValue:''});
    });

    return schema;
  }, // End addFCSubtype


  // addFdName - Add the ESRI Feature Dataset name to every element in the schema
  // If we don't add this the layers get put in the wrong place in the FGDB
  addFdName: function(schema,name)
  {
    schema.forEach( function(item) {item.fdname = name; });

    return schema;
  }, // End addFdName


  // debugOutput - Dump out tags or attributes to figure out what is going on
  debugOutput : function(values,layerName,geometryType,elementType,text)
  {
    var kList = Object.keys(values).sort();
    if (kList.length > 0)
    {
      print('LayerName: ' + layerName + '  Geometry: ' + geometryType + '  Element Type: ' + elementType);
      for (var i = 0, fLen = kList.length; i < fLen; i++) if (values[kList[i]]) print(text + kList[i] + ': :' + values[kList[i]] + ':');
    }
  }, // End debugOutput


  // dumpLookup - Dump a Lookup table so we can check it
  dumpLookup : function(lookupTable)
  {
    for (var i in lookupTable)
    {
      print('Key: ' + i + '  Value: ' + lookupTable[i]);
    }
  }, // End dumpLookup


  // dumpOne2OneLookup - Dump a Lookup table so we can check it
  dumpOne2OneLookup : function(lookupTable)
  {
    for (var i in lookupTable)
      for (var j in lookupTable[i])
      {
        print('I:' + i + '  J:' + j + '  ' + lookupTable[i][j][0] + ' = ' + lookupTable[i][j][1]);
      }
  }, // End dumpOne2OneLookup


  // dumpSchema - Dump a schema so we can check it
  dumpSchema : function(schema)
  {
    schema.forEach( function(item) {
      print('Feature: ' + item.name + '  Geom: ' + item.geom + '  FdName: ' + item.fdname);

      item.columns.forEach( function (column) {
        print('    Attr: ' + column.name + '  Desc: ' + column.desc + '  Type: ' + column.type + '  Default: ' + column.defValue);
        if (column.type == 'enumeration')
        {
          column.enumerations.forEach( function (eValue) {print('        Value: ' + eValue.value + '  Name: ' + eValue.name); });
        }
      });

      print(''); // just to get one blank line
    });

  }, // End dumpSchema


  // overrideValues - Add, modify or delete tags/attributes based on a JSON string
  overrideValues: function(values,changeString)
  {
    if (changeString == '') return;

    var override = JSON.parse(changeString);

    for (var i in override)
    {
      // Delete: Remove a tag
      if (override[i] == '')
      {
        if (values[i]) {
          delete values[i];
        }
      }
      // Modify: change a tag value ONLY if the tag already exists
      else if (i.charAt(0) == '#')
      {
        var tag = i.slice(1);
        if (values[tag]) values[tag] = override[i];
      }
      // Add/Modify: change or add a tag
      else
      {
        values[i] = override[i];
      }
    }

    return values;
  }, // End overrideValues

  getMaxAsString: function(value1, value2) {
    var maxVal;
    var val1 = parseInt(value1);
    var val2 = parseInt(value2);

    if (!val1) {
      maxVal = value2;
    } else if (!val2) {
      maxVal = value1;
    } else {
      maxVal = Math.max(val1, val2);
    }

    return maxVal + '';
  }
}; // End of translate
