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

    if (oldValue === undefined || oldValue === "" || oldValue === '')
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


  // Take a tag and increment it.
  // E.g tag -> tag1, tag7 -> tag8 etc
  // NOTE: We are assumeing that this will not go past 9....
  incrementTag: function (tag)
  {
    var endChar = tag.charAt(tag.length - 1);

    if (!isNaN(endChar))
    {
      // var newNum = parseInt(endChar) + 1;
      return tag.substring(0,(tag.length - 1)) + (parseInt(endChar) + 1);
    }
    else
    {
      return tag + '1';
    }
  }, // End incrementTag


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
  },


  // Converting country codes to names to URN's
  // possible From/To values: 
  //  n = name
  //  sN = shortName
  //  fN = fullName
  //  c3 = Alpha 3 code
  //  c2 = Alpha 2 code
  //  fips = fips code
  //  code = Numeric country number
  //  urn = NGA GENC URN

  countryTable : [
{"n":"ARUBA","sN":"Aruba","fN":"Aruba","lN":{"Aruba":"nld"},"c3":"ABW","c2":"AW","fips":"AA","code":"533","urn":"ge:GENC:3:1-2:ABW"},
{"n":"AFGHANISTAN","sN":"Afghanistan","fN":"Islamic Republic of Afghanistan","lN":{"Afghānistān":"fas","Afghānistān":"pus"},"c3":"AFG","c2":"AF","fips":"","code":"004","urn":"ge:GENC:3:1-2:AFG"},
{"n":"ANGOLA","sN":"Angola","fN":"Republic of Angola","lN":{"Angola":"por"},"c3":"AGO","c2":"AO","fips":"","code":"024","urn":"ge:GENC:3:1-2:AGO"},
{"n":"ANGUILLA","sN":"Anguilla","fN":"Anguilla","lN":{"Anguilla":"eng"},"c3":"AIA","c2":"AI","fips":"AV","code":"660","urn":"ge:ISO1:3:VI-15:AIA"},
{"n":"ÅLAND ISLANDS","sN":"Åland Islands","fN":"Åland Islands","lN":{"Åland":"swe"},"c3":"ALA","c2":"AX","fips":"","code":"248","urn":"ge:GENC:3:1-2:ALA"},
{"n":"ALBANIA","sN":"Albania","fN":"Republic of Albania","lN":{"Shqipëria":"sqi"},"c3":"ALB","c2":"AL","fips":"","code":"008","urn":"ge:GENC:3:1-2:ALB"},
{"n":"ANDORRA","sN":"Andorra","fN":"Principality of Andorra","lN":{"Andorra":"cat"},"c3":"AND","c2":"AD","fips":"AN","code":"020","urn":"ge:GENC:3:1-2:AND"},
{"n":"UNITED ARAB EMIRATES","sN":"United Arab Emirates","fN":"United Arab Emirates","lN":{"Al Imārāt al ‘Arabīyah al Muttaḩidah":"ara"},"c3":"ARE","c2":"AE","fips":"AE","code":"784","urn":"ge:GENC:3:1-2:ARE"},
{"n":"ARGENTINA","sN":"Argentina","fN":"Argentine Republic","lN":{"Argentina":"spa"},"c3":"ARG","c2":"AR","fips":"","code":"032","urn":"ge:GENC:3:1-2:ARG"},
{"n":"ARMENIA","sN":"Armenia","fN":"Republic of Armenia","lN":{"Hayastan":"hye"},"c3":"ARM","c2":"AM","fips":"","code":"051","urn":"ge:GENC:3:1-2:ARM"},
{"n":"AMERICAN SAMOA","sN":"American Samoa","fN":"Territory of American Samoa","lN":{"American Samoa":"eng"},"c3":"ASM","c2":"AS","fips":"AQ","code":"016","urn":"ge:GENC:3:1-2:ASM"},
{"n":"ANTARCTICA","sN":"Antarctica","fN":"Antarctica","lN":{},"c3":"ATA","c2":"AQ","fips":"AY","code":"010","urn":"ge:ISO1:3:VI-15:ATA"},
{"n":"FRENCH SOUTHERN AND ANTARCTIC LANDS","sN":"French Southern and Antarctic Lands","fN":"French Southern and Antarctic Lands","lN":{"Terres Australes et Antarctiques Françaises":"fra"},"c3":"ATF","c2":"TF","fips":"FS","code":"260","urn":"ge:GENC:3:1-2:ATF"},
{"n":"ANTIGUA AND BARBUDA","sN":"Antigua and Barbuda","fN":"Antigua and Barbuda","lN":{"Antigua and Barbuda":"eng"},"c3":"ATG","c2":"AG","fips":"AC","code":"028","urn":"ge:GENC:3:1-2:ATG"},
{"n":"AUSTRALIA","sN":"Australia","fN":"Commonwealth of Australia","lN":{"Australia":"eng"},"c3":"AUS","c2":"AU","fips":"AS","code":"036","urn":"ge:GENC:3:1-2:AUS"},
{"n":"AUSTRIA","sN":"Austria","fN":"Republic of Austria","lN":{"Österreich":"deu"},"c3":"AUT","c2":"AT","fips":"AU","code":"040","urn":"ge:GENC:3:1-2:AUT"},
{"n":"UNKNOWN","sN":"Unknown","fN":"Unknown","lN":{},"c3":"AX1","c2":"A1","fips":"","code":"","urn":"ge:GENC:3:1-2:AX1"},
{"n":"GUANTANAMO BAY NAVAL BASE","sN":"Guantanamo Bay Naval Base","fN":"Guantanamo Bay Naval Base","lN":{},"c3":"AX2","c2":"A2","fips":"","code":"","urn":"ge:GENC:3:1-2:AX2"},
{"n":"AZERBAIJAN","sN":"Azerbaijan","fN":"Republic of Azerbaijan","lN":{"Azərbaycan":"aze"},"c3":"AZE","c2":"AZ","fips":"AJ","code":"031","urn":"ge:GENC:3:1-2:AZE"},
{"n":"BURUNDI","sN":"Burundi","fN":"Republic of Burundi","lN":{"Burundi":"fra","Burundi":"run"},"c3":"BDI","c2":"BI","fips":"BY","code":"108","urn":"ge:GENC:3:1-2:BDI"},
{"n":"BELGIUM","sN":"Belgium","fN":"Kingdom of Belgium","lN":{"Belgien":"deu","Belgique":"fra","België":"nld"},"c3":"BEL","c2":"BE","fips":"","code":"056","urn":"ge:GENC:3:1-2:BEL"},
{"n":"BENIN","sN":"Benin","fN":"Republic of Benin","lN":{"Bénin":"fra"},"c3":"BEN","c2":"BJ","fips":"BN","code":"204","urn":"ge:GENC:3:1-2:BEN"},
{"n":"BONAIRE, SINT EUSTATIUS, AND SABA","sN":"Bonaire, Sint Eustatius, and Saba","fN":"Bonaire, Sint Eustatius, and Saba","lN":{"Bonaire, Sint Eustatius, and Saba":"eng"},"c3":"BES","c2":"BQ","fips":"NL","code":"535","urn":"ge:GENC:3:1-2:BES"},
{"n":"BURKINA FASO","sN":"Burkina Faso","fN":"Burkina Faso","lN":{"Burkina Faso":"fra"},"c3":"BFA","c2":"BF","fips":"UV","code":"854","urn":"ge:GENC:3:1-2:BFA"},
{"n":"BANGLADESH","sN":"Bangladesh","fN":"People’s Republic of Bangladesh","lN":{"Bangladesh":"ben"},"c3":"BGD","c2":"BD","fips":"BG","code":"050","urn":"ge:GENC:3:1-2:BGD"},
{"n":"BULGARIA","sN":"Bulgaria","fN":"Republic of Bulgaria","lN":{"Bŭlgariya":"bul"},"c3":"BGR","c2":"BG","fips":"BU","code":"100","urn":"ge:GENC:3:1-2:BGR"},
{"n":"BAHRAIN","sN":"Bahrain","fN":"Kingdom of Bahrain","lN":{"Al Baḩrayn":"ara"},"c3":"BHR","c2":"BH","fips":"BA","code":"048","urn":"ge:GENC:3:1-2:BHR"},
{"n":"BAHAMAS, THE","sN":"Bahamas, The","fN":"Commonwealth of The Bahamas","lN":{"Bahamas, The":"eng"},"c3":"BHS","c2":"BS","fips":"BF","code":"044","urn":"ge:GENC:3:1-2:BHS"},
{"n":"BOSNIA AND HERZEGOVINA","sN":"Bosnia and Herzegovina","fN":"Bosnia and Herzegovina","lN":{"Bosna i Hercegovina":"bos","Bosna i Hercegovina":"hrv","Bosna i Hercegovina":"srp"},"c3":"BIH","c2":"BA","fips":"BK","code":"070","urn":"ge:ISO1:3:VI-15:BIH"},
{"n":"SAINT BARTHELEMY","sN":"Saint Barthelemy","fN":"Saint Barthelemy","lN":{"Saint-Barthélemy":"fra"},"c3":"BLM","c2":"BL","fips":"TB","code":"652","urn":"ge:GENC:3:1-2:BLM"},
{"n":"BELARUS","sN":"Belarus","fN":"Republic of Belarus","lN":{"Byelarus’":"bel","Belarus’":"rus"},"c3":"BLR","c2":"BY","fips":"BO","code":"112","urn":"ge:GENC:3:1-2:BLR"},
{"n":"BELIZE","sN":"Belize","fN":"Belize","lN":{"Belize":"eng"},"c3":"BLZ","c2":"BZ","fips":"BH","code":"084","urn":"ge:ISO1:3:VI-15:BLZ"},
{"n":"BERMUDA","sN":"Bermuda","fN":"Bermuda","lN":{"Bermuda":"eng"},"c3":"BMU","c2":"BM","fips":"BD","code":"060","urn":"ge:ISO1:3:VI-15:BMU"},
{"n":"BOLIVIA","sN":"Bolivia","fN":"Plurinational State of Bolivia","lN":{"Bolivia":"spa"},"c3":"BOL","c2":"BO","fips":"BL","code":"068","urn":"ge:GENC:3:1-2:BOL"},
{"n":"BRAZIL","sN":"Brazil","fN":"Federative Republic of Brazil","lN":{"Brasil":"por"},"c3":"BRA","c2":"BR","fips":"","code":"076","urn":"ge:GENC:3:1-2:BRA"},
{"n":"BARBADOS","sN":"Barbados","fN":"Barbados","lN":{"Barbados":"eng"},"c3":"BRB","c2":"BB","fips":"","code":"052","urn":"ge:ISO1:3:VI-15:BRB"},
{"n":"BRUNEI","sN":"Brunei","fN":"Brunei Darussalam","lN":{"Brunei":"eng","Negara Brunei Darussalam":"msa"},"c3":"BRN","c2":"BN","fips":"BX","code":"096","urn":"ge:GENC:3:1-2:BRN"},
{"n":"BHUTAN","sN":"Bhutan","fN":"Kingdom of Bhutan","lN":{},"c3":"BTN","c2":"BT","fips":"","code":"064","urn":"ge:GENC:3:1-2:BTN"},
{"n":"BOUVET ISLAND","sN":"Bouvet Island","fN":"Bouvet Island","lN":{"Bouvetøya":"nno","Bouvetøya":"nob"},"c3":"BVT","c2":"BV","fips":"","code":"074","urn":"ge:ISO1:3:VI-15:BVT"},
{"n":"BOTSWANA","sN":"Botswana","fN":"Republic of Botswana","lN":{"Botswana":"eng"},"c3":"BWA","c2":"BW","fips":"BC","code":"072","urn":"ge:GENC:3:1-2:BWA"},
{"n":"CENTRAL AFRICAN REPUBLIC","sN":"Central African Republic","fN":"Central African Republic","lN":{"République Centrafricaine":"fra"},"c3":"CAF","c2":"CF","fips":"CT","code":"140","urn":"ge:GENC:3:1-2:CAF"},
{"n":"CANADA","sN":"Canada","fN":"Canada","lN":{"Canada":"eng","Canada":"fra"},"c3":"CAN","c2":"CA","fips":"","code":"124","urn":"ge:GENC:3:1-2:CAN"},
{"n":"COCOS (KEELING) ISLANDS","sN":"Cocos (Keeling) Islands","fN":"Territory of Cocos (Keeling) Islands","lN":{"Cocos (Keeling) Islands":"eng"},"c3":"CCK","c2":"CC","fips":"CK","code":"166","urn":"ge:GENC:3:1-2:CCK"},
{"n":"SWITZERLAND","sN":"Switzerland","fN":"Swiss Confederation","lN":{"Schweiz":"deu","Suisse":"fra","Svizzera":"ita"},"c3":"CHE","c2":"CH","fips":"SZ","code":"756","urn":"ge:GENC:3:1-2:CHE"},
{"n":"CHILE","sN":"Chile","fN":"Republic of Chile","lN":{"Chile":"spa"},"c3":"CHL","c2":"CL","fips":"CI","code":"152","urn":"ge:GENC:3:1-2:CHL"},
{"n":"CHINA","sN":"China","fN":"People’s Republic of China","lN":{"Zhonghua Renmin Gongheguo":"zho"},"c3":"CHN","c2":"CN","fips":"CH","code":"156","urn":"ge:GENC:3:1-2:CHN"},
{"n":"CÔTE D’IVOIRE","sN":"Côte d’Ivoire","fN":"Republic of Côte d’Ivoire","lN":{"Côte d’Ivoire":"fra"},"c3":"CIV","c2":"CI","fips":"IV","code":"384","urn":"ge:GENC:3:1-2:CIV"},
{"n":"CAMEROON","sN":"Cameroon","fN":"Republic of Cameroon","lN":{"Cameroon":"eng","Cameroun":"fra"},"c3":"CMR","c2":"CM","fips":"","code":"120","urn":"ge:GENC:3:1-2:CMR"},
{"n":"CONGO (KINSHASA)","sN":"Congo (Kinshasa)","fN":"Democratic Republic of the Congo","lN":{"Congo":"fra"},"c3":"COD","c2":"CD","fips":"CG","code":"180","urn":"ge:GENC:3:1-2:COD"},
{"n":"CONGO (BRAZZAVILLE)","sN":"Congo (Brazzaville)","fN":"Republic of the Congo","lN":{"Congo":"fra"},"c3":"COG","c2":"CG","fips":"CF","code":"178","urn":"ge:GENC:3:1-2:COG"},
{"n":"COOK ISLANDS","sN":"Cook Islands","fN":"Cook Islands","lN":{"Cook Islands":"eng"},"c3":"COK","c2":"CK","fips":"CW","code":"184","urn":"ge:GENC:3:1-2:COK"},
{"n":"COLOMBIA","sN":"Colombia","fN":"Republic of Colombia","lN":{"Colombia":"spa"},"c3":"COL","c2":"CO","fips":"","code":"170","urn":"ge:GENC:3:1-2:COL"},
{"n":"COMOROS","sN":"Comoros","fN":"Union of the Comoros","lN":{"Komori":"---","Juzur al Qamar":"ara","Comores":"fra"},"c3":"COM","c2":"KM","fips":"CN","code":"174","urn":"ge:GENC:3:1-2:COM"},
{"n":"CLIPPERTON ISLAND","sN":"Clipperton Island","fN":"Clipperton Island","lN":{"Île Clipperton":"fra"},"c3":"CPT","c2":"CP","fips":"","code":"","urn":"ge:GENC:3:1-2:CPT"},
{"n":"CAPE VERDE","sN":"Cape Verde","fN":"Republic of Cape Verde","lN":{"Cabo Verde":"por"},"c3":"CPV","c2":"CV","fips":"","code":"132","urn":"ge:GENC:3:1-2:CPV"},
{"n":"COSTA RICA","sN":"Costa Rica","fN":"Republic of Costa Rica","lN":{"Costa Rica":"spa"},"c3":"CRI","c2":"CR","fips":"CS","code":"188","urn":"ge:GENC:3:1-2:CRI"},
{"n":"CUBA","sN":"Cuba","fN":"Republic of Cuba","lN":{},"c3":"CUB","c2":"CU","fips":"","code":"192","urn":"ge:GENC:3:1-2:CUB"},
{"n":"CURAÇAO","sN":"Curaçao","fN":"Curaçao","lN":{"Curaçao":"nld","Kòrsou":"pap"},"c3":"CUW","c2":"CW","fips":"UC","code":"531","urn":"ge:GENC:3:1-2:CUW"},
{"n":"CHRISTMAS ISLAND","sN":"Christmas Island","fN":"Territory of Christmas Island","lN":{"Christmas Island":"eng"},"c3":"CXR","c2":"CX","fips":"KT","code":"162","urn":"ge:GENC:3:1-2:CXR"},
{"n":"CAYMAN ISLANDS","sN":"Cayman Islands","fN":"Cayman Islands","lN":{"Cayman Islands":"eng"},"c3":"CYM","c2":"KY","fips":"CJ","code":"136","urn":"ge:GENC:3:1-2:CYM"},
{"n":"CYPRUS","sN":"Cyprus","fN":"Republic of Cyprus","lN":{"Kýpros":"ell","Kıbrıs":"tur"},"c3":"CYP","c2":"CY","fips":"","code":"196","urn":"ge:GENC:3:1-2:CYP"},
{"n":"CZECH REPUBLIC","sN":"Czech Republic","fN":"Czech Republic","lN":{"Česká Republika":"ces"},"c3":"CZE","c2":"CZ","fips":"EZ","code":"203","urn":"ge:GENC:3:1-2:CZE"},
{"n":"GERMANY","sN":"Germany","fN":"Federal Republic of Germany","lN":{"Deutschland":"deu"},"c3":"DEU","c2":"DE","fips":"GM","code":"276","urn":"ge:GENC:3:1-2:DEU"},
{"n":"DIEGO GARCIA","sN":"Diego Garcia","fN":"Diego Garcia","lN":{},"c3":"DGA","c2":"DG","fips":"","code":"","urn":"ge:GENC:3:1-2:DGA"},
{"n":"DJIBOUTI","sN":"Djibouti","fN":"Republic of Djibouti","lN":{"Jībūtī":"ara"},"c3":"DJI","c2":"DJ","fips":"","code":"262","urn":"ge:GENC:3:1-2:DJI"},
{"n":"DOMINICA","sN":"Dominica","fN":"Commonwealth of Dominica","lN":{"Dominica":"eng"},"c3":"DMA","c2":"DM","fips":"DO","code":"212","urn":"ge:GENC:3:1-2:DMA"},
{"n":"DENMARK","sN":"Denmark","fN":"Kingdom of Denmark","lN":{"Danmark":"dan"},"c3":"DNK","c2":"DK","fips":"DA","code":"208","urn":"ge:GENC:3:1-2:DNK"},
{"n":"DOMINICAN REPUBLIC","sN":"Dominican Republic","fN":"Dominican Republic","lN":{"República Dominicana":"spa"},"c3":"DOM","c2":"DO","fips":"DR","code":"214","urn":"ge:GENC:3:1-2:DOM"},
{"n":"ALGERIA","sN":"Algeria","fN":"People’s Democratic Republic of Algeria","lN":{"Al Jazā’ir":"ara"},"c3":"DZA","c2":"DZ","fips":"AG","code":"012","urn":"ge:GENC:3:1-2:DZA"},
{"n":"ECUADOR","sN":"Ecuador","fN":"Republic of Ecuador","lN":{"Ecuador":"spa"},"c3":"ECU","c2":"EC","fips":"","code":"218","urn":"ge:GENC:3:1-2:ECU"},
{"n":"EGYPT","sN":"Egypt","fN":"Arab Republic of Egypt","lN":{"Mişr":"ara"},"c3":"EGY","c2":"EG","fips":"","code":"818","urn":"ge:GENC:3:1-2:EGY"},
{"n":"ERITREA","sN":"Eritrea","fN":"State of Eritrea","lN":{"Eritrea":"eng","Iertra":"tir"},"c3":"ERI","c2":"ER","fips":"","code":"232","urn":"ge:GENC:3:1-2:ERI"},
{"n":"WESTERN SAHARA","sN":"Western Sahara","fN":"Western Sahara","lN":{"Aş Şaḩrā' al Gharbīyah":"ara"},"c3":"ESH","c2":"EH","fips":"WI","code":"732","urn":"ge:GENC:3:1-2:ESH"},
{"n":"SPAIN","sN":"Spain","fN":"Kingdom of Spain","lN":{"España":"spa"},"c3":"ESP","c2":"ES","fips":"SP","code":"724","urn":"ge:GENC:3:1-2:ESP"},
{"n":"ESTONIA","sN":"Estonia","fN":"Republic of Estonia","lN":{"Eesti":"est"},"c3":"EST","c2":"EE","fips":"","code":"233","urn":"ge:GENC:3:1-2:EST"},
{"n":"ETHIOPIA","sN":"Ethiopia","fN":"Federal Democratic Republic of Ethiopia","lN":{"Ītyop’iya":"amh"},"c3":"ETH","c2":"ET","fips":"","code":"231","urn":"ge:GENC:3:1-2:ETH"},
{"n":"FINLAND","sN":"Finland","fN":"Republic of Finland","lN":{"Suomi":"fin","Finland":"swe"},"c3":"FIN","c2":"FI","fips":"","code":"246","urn":"ge:GENC:3:1-2:FIN"},
{"n":"FIJI","sN":"Fiji","fN":"Republic of Fiji","lN":{"Fiji":"eng"},"c3":"FJI","c2":"FJ","fips":"","code":"242","urn":"ge:GENC:3:1-2:FJI"},
{"n":"FALKLAND ISLANDS (ISLAS MALVINAS)","sN":"Falkland Islands (Islas Malvinas)","fN":"Falkland Islands (Islas Malvinas)","lN":{"Falkland Islands (Islas Malvinas)":"eng"},"c3":"FLK","c2":"FK","fips":"","code":"238","urn":"ge:GENC:3:1-2:FLK"},
{"n":"FRANCE","sN":"France","fN":"French Republic","lN":{"France":"fra"},"c3":"FRA","c2":"FR","fips":"","code":"250","urn":"ge:GENC:3:1-2:FRA"},
{"n":"FAROE ISLANDS","sN":"Faroe Islands","fN":"Faroe Islands","lN":{"Færøerne":"dan","Føroyar":"fao"},"c3":"FRO","c2":"FO","fips":"","code":"234","urn":"ge:GENC:3:1-2:FRO"},
{"n":"MICRONESIA, FEDERATED STATES OF","sN":"Micronesia, Federated States of","fN":"Federated States of Micronesia","lN":{"Federated States of Micronesia":"eng"},"c3":"FSM","c2":"FM","fips":"","code":"583","urn":"ge:GENC:3:1-2:FSM"},
{"n":"GABON","sN":"Gabon","fN":"Gabonese Republic","lN":{"Gabon":"fra"},"c3":"GAB","c2":"GA","fips":"GB","code":"266","urn":"ge:GENC:3:1-2:GAB"},
{"n":"UNITED KINGDOM","sN":"United Kingdom","fN":"United Kingdom of Great Britain and Northern Ireland","lN":{"United Kingdom":"eng"},"c3":"GBR","c2":"GB","fips":"UK","code":"826","urn":"ge:GENC:3:1-2:GBR"},
{"n":"GEORGIA","sN":"Georgia","fN":"Georgia","lN":{"Sakartvelo":"kat"},"c3":"GEO","c2":"GE","fips":"GG","code":"268","urn":"ge:GENC:3:1-2:GEO"},
{"n":"GUERNSEY","sN":"Guernsey","fN":"Bailiwick of Guernsey","lN":{"Guernsey":"eng"},"c3":"GGY","c2":"GG","fips":"GK","code":"831","urn":"ge:GENC:3:1-2:GGY"},
{"n":"GHANA","sN":"Ghana","fN":"Republic of Ghana","lN":{"Ghana":"eng"},"c3":"GHA","c2":"GH","fips":"","code":"288","urn":"ge:GENC:3:1-2:GHA"},
{"n":"GIBRALTAR","sN":"Gibraltar","fN":"Gibraltar","lN":{"Gibraltar":"eng"},"c3":"GIB","c2":"GI","fips":"","code":"292","urn":"ge:ISO1:3:VI-15:GIB"},
{"n":"GUINEA","sN":"Guinea","fN":"Republic of Guinea","lN":{"Guinée":"fra"},"c3":"GIN","c2":"GN","fips":"GV","code":"324","urn":"ge:GENC:3:1-2:GIN"},
{"n":"GUADELOUPE","sN":"Guadeloupe","fN":"Department of Guadeloupe","lN":{"Guadeloupe":"fra"},"c3":"GLP","c2":"GP","fips":"","code":"312","urn":"ge:GENC:3:1-2:GLP"},
{"n":"GAMBIA, THE","sN":"Gambia, The","fN":"Republic of The Gambia","lN":{"Gambia, The":"eng"},"c3":"GMB","c2":"GM","fips":"GA","code":"270","urn":"ge:GENC:3:1-2:GMB"},
{"n":"GUINEA-BISSAU","sN":"Guinea-Bissau","fN":"Republic of Guinea-Bissau","lN":{"Guiné-Bissau":"por"},"c3":"GNB","c2":"GW","fips":"PU","code":"624","urn":"ge:GENC:3:1-2:GNB"},
{"n":"EQUATORIAL GUINEA","sN":"Equatorial Guinea","fN":"Republic of Equatorial Guinea","lN":{"Guinea Ecuatorial":"spa"},"c3":"GNQ","c2":"GQ","fips":"EK","code":"226","urn":"ge:GENC:3:1-2:GNQ"},
{"n":"GREECE","sN":"Greece","fN":"Hellenic Republic","lN":{"Ellás/Elláda":"ell"},"c3":"GRC","c2":"GR","fips":"","code":"300","urn":"ge:GENC:3:1-2:GRC"},
{"n":"GRENADA","sN":"Grenada","fN":"Grenada","lN":{"Grenada":"eng"},"c3":"GRD","c2":"GD","fips":"GJ","code":"308","urn":"ge:ISO1:3:VI-15:GRD"},
{"n":"GREENLAND","sN":"Greenland","fN":"Greenland","lN":{"Grønland":"dan","Kalaallit Nunaat":"kal"},"c3":"GRL","c2":"GL","fips":"","code":"304","urn":"ge:ISO1:3:VI-15:GRL"},
{"n":"GUATEMALA","sN":"Guatemala","fN":"Republic of Guatemala","lN":{"Guatemala":"spa"},"c3":"GTM","c2":"GT","fips":"","code":"320","urn":"ge:GENC:3:1-2:GTM"},
{"n":"FRENCH GUIANA","sN":"French Guiana","fN":"Department of Guiana","lN":{"Guyane":"fra"},"c3":"GUF","c2":"GF","fips":"FG","code":"254","urn":"ge:GENC:3:1-2:GUF"},
{"n":"GUAM","sN":"Guam","fN":"Territory of Guam","lN":{"Guam":"eng"},"c3":"GUM","c2":"GU","fips":"GQ","code":"316","urn":"ge:GENC:3:1-2:GUM"},
{"n":"GUYANA","sN":"Guyana","fN":"Co-operative Republic of Guyana","lN":{"Guyana":"eng"},"c3":"GUY","c2":"GY","fips":"GY","code":"328","urn":"ge:GENC:3:1-2:GUY"},
{"n":"HONG KONG","sN":"Hong Kong","fN":"Hong Kong Special Administrative Region","lN":{"Hong Kong":"eng","Xianggang":"zho"},"c3":"HKG","c2":"HK","fips":"","code":"344","urn":"ge:GENC:3:1-2:HKG"},
{"n":"HEARD ISLAND AND MCDONALD ISLANDS","sN":"Heard Island and McDonald Islands","fN":"Territory of Heard Island and McDonald Islands","lN":{"Heard Island and McDonald Islands":"eng"},"c3":"HMD","c2":"HM","fips":"","code":"334","urn":"ge:GENC:3:1-2:HMD"},
{"n":"HONDURAS","sN":"Honduras","fN":"Republic of Honduras","lN":{"Honduras":"spa"},"c3":"HND","c2":"HN","fips":"HO","code":"340","urn":"ge:GENC:3:1-2:HND"},
{"n":"CROATIA","sN":"Croatia","fN":"Republic of Croatia","lN":{"Hrvatska":"hrv"},"c3":"HRV","c2":"HR","fips":"","code":"191","urn":"ge:GENC:3:1-2:HRV"},
{"n":"HAITI","sN":"Haiti","fN":"Republic of Haiti","lN":{"Haïti":"fra"},"c3":"HTI","c2":"HT","fips":"HA","code":"332","urn":"ge:GENC:3:1-2:HTI"},
{"n":"HUNGARY","sN":"Hungary","fN":"Hungary","lN":{"Magyarország":"hun"},"c3":"HUN","c2":"HU","fips":"","code":"348","urn":"ge:ISO1:3:VI-15:HUN"},
{"n":"INDONESIA","sN":"Indonesia","fN":"Republic of Indonesia","lN":{"Indonesia":"ind"},"c3":"IDN","c2":"ID","fips":"","code":"360","urn":"ge:GENC:3:1-2:IDN"},
{"n":"ISLE OF MAN","sN":"Isle of Man","fN":"Isle of Man","lN":{"Isle of Man":"eng"},"c3":"IMN","c2":"IM","fips":"","code":"833","urn":"ge:ISO1:3:VI-15:IMN"},
{"n":"INDIA","sN":"India","fN":"Republic of India","lN":{"India":"eng"},"c3":"IND","c2":"IN","fips":"","code":"356","urn":"ge:GENC:3:1-2:IND"},
{"n":"BRITISH INDIAN OCEAN TERRITORY","sN":"British Indian Ocean Territory","fN":"British Indian Ocean Territory","lN":{"British Indian Ocean Territory":"eng"},"c3":"IOT","c2":"IO","fips":"","code":"086","urn":"ge:GENC:3:1-2:IOT"},
{"n":"IRELAND","sN":"Ireland","fN":"Ireland","lN":{"Ireland":"eng","Éire":"gle"},"c3":"IRL","c2":"IE","fips":"EI","code":"372","urn":"ge:ISO1:3:VI-15:IRL"},
{"n":"IRAN","sN":"Iran","fN":"Islamic Republic of Iran","lN":{"Īrān":"fas"},"c3":"IRN","c2":"IR","fips":"","code":"364","urn":"ge:GENC:3:1-2:IRN"},
{"n":"IRAQ","sN":"Iraq","fN":"Republic of Iraq","lN":{"Al ‘Irāq":"ara"},"c3":"IRQ","c2":"IQ","fips":"IZ","code":"368","urn":"ge:GENC:3:1-2:IRQ"},
{"n":"ICELAND","sN":"Iceland","fN":"Republic of Iceland","lN":{"Ísland":"isl"},"c3":"ISL","c2":"IS","fips":"IC","code":"352","urn":"ge:GENC:3:1-2:ISL"},
{"n":"ISRAEL","sN":"Israel","fN":"State of Israel","lN":{"Isrā’īl":"ara","Yisra’el":"heb"},"c3":"ISR","c2":"IL","fips":"IS","code":"376","urn":"ge:GENC:3:1-2:ISR"},
{"n":"ITALY","sN":"Italy","fN":"Italian Republic","lN":{"Italia":"ita"},"c3":"ITA","c2":"IT","fips":"","code":"380","urn":"ge:GENC:3:1-2:ITA"},
{"n":"JAMAICA","sN":"Jamaica","fN":"Jamaica","lN":{"Jamaica":"eng"},"c3":"JAM","c2":"JM","fips":"","code":"388","urn":"ge:ISO1:3:VI-15:JAM"},
{"n":"JERSEY","sN":"Jersey","fN":"Bailiwick of Jersey","lN":{"Jersey":"eng","Jersey":"fra"},"c3":"JEY","c2":"JE","fips":"","code":"832","urn":"ge:GENC:3:1-2:JEY"},
{"n":"JORDAN","sN":"Jordan","fN":"Hashemite Kingdom of Jordan","lN":{"Al Urdun":"ara"},"c3":"JOR","c2":"JO","fips":"","code":"400","urn":"ge:GENC:3:1-2:JOR"},
{"n":"JAPAN","sN":"Japan","fN":"Japan","lN":{"Nihon/Nippon":"jpn"},"c3":"JPN","c2":"JP","fips":"JA","code":"392","urn":"ge:ISO1:3:VI-15:JPN"},
{"n":"KAZAKHSTAN","sN":"Kazakhstan","fN":"Republic of Kazakhstan","lN":{"Qazaqstan":"kaz","Kazahstan":"rus"},"c3":"KAZ","c2":"KZ","fips":"","code":"398","urn":"ge:GENC:3:1-2:KAZ"},
{"n":"KENYA","sN":"Kenya","fN":"Republic of Kenya","lN":{"Kenya":"eng"},"c3":"KEN","c2":"KE","fips":"","code":"404","urn":"ge:GENC:3:1-2:KEN"},
{"n":"KYRGYZSTAN","sN":"Kyrgyzstan","fN":"Kyrgyz Republic","lN":{"Kyrgyzstan":"kir","Kyrgyzstan":"rus"},"c3":"KGZ","c2":"KG","fips":"KG","code":"417","urn":"ge:GENC:3:1-2:KGZ"},
{"n":"CAMBODIA","sN":"Cambodia","fN":"Kingdom of Cambodia","lN":{"Kâmpŭchéa":"khm"},"c3":"KHM","c2":"KH","fips":"CB","code":"116","urn":"ge:GENC:3:1-2:KHM"},
{"n":"KIRIBATI","sN":"Kiribati","fN":"Republic of Kiribati","lN":{"Kiribati":"eng"},"c3":"KIR","c2":"KI","fips":"KR","code":"296","urn":"ge:GENC:3:1-2:KIR"},
{"n":"SAINT KITTS AND NEVIS","sN":"Saint Kitts and Nevis","fN":"Federation of Saint Kitts and Nevis","lN":{"Saint Kitts and Nevis":"eng"},"c3":"KNA","c2":"KN","fips":"SC","code":"659","urn":"ge:GENC:3:1-2:KNA"},
{"n":"KOREA, SOUTH","sN":"Korea, South","fN":"Republic of Korea","lN":{"Taehan-min’guk":"kor"},"c3":"KOR","c2":"KR","fips":"KS","code":"410","urn":"ge:GENC:3:1-2:KOR"},
{"n":"KUWAIT","sN":"Kuwait","fN":"State of Kuwait","lN":{"Al Kuwayt":"ara"},"c3":"KWT","c2":"KW","fips":"KU","code":"414","urn":"ge:GENC:3:1-2:KWT"},
{"n":"LAOS","sN":"Laos","fN":"Lao People’s Democratic Republic","lN":{"Sathalanalat Paxathipatai Paxaxôn Lao":"lao"},"c3":"LAO","c2":"LA","fips":"","code":"418","urn":"ge:GENC:3:1-2:LAO"},
{"n":"LEBANON","sN":"Lebanon","fN":"Lebanese Republic","lN":{"Lubnān":"ara"},"c3":"LBN","c2":"LB","fips":"LE","code":"422","urn":"ge:GENC:3:1-2:LBN"},
{"n":"LIBERIA","sN":"Liberia","fN":"Republic of Liberia","lN":{"Liberia":"eng"},"c3":"LBR","c2":"LR","fips":"LI","code":"430","urn":"ge:GENC:3:1-2:LBR"},
{"n":"LIBYA","sN":"Libya","fN":"Libya","lN":{"Lībiyā":"ara"},"c3":"LBY","c2":"LY","fips":"","code":"434","urn":"ge:ISO1:3:VI-15:LBY"},
{"n":"SAINT LUCIA","sN":"Saint Lucia","fN":"Saint Lucia","lN":{"Saint Lucia":"eng"},"c3":"LCA","c2":"LC","fips":"ST","code":"662","urn":"ge:ISO1:3:VI-15:LCA"},
{"n":"LIECHTENSTEIN","sN":"Liechtenstein","fN":"Principality of Liechtenstein","lN":{"Liechtenstein":"deu"},"c3":"LIE","c2":"LI","fips":"LS","code":"438","urn":"ge:GENC:3:1-2:LIE"},
{"n":"SRI LANKA","sN":"Sri Lanka","fN":"Democratic Socialist Republic of Sri Lanka","lN":{"Sri Lanka":"eng"},"c3":"LKA","c2":"LK","fips":"CE","code":"144","urn":"ge:GENC:3:1-2:LKA"},
{"n":"LESOTHO","sN":"Lesotho","fN":"Kingdom of Lesotho","lN":{"Lesotho":"eng"},"c3":"LSO","c2":"LS","fips":"LT","code":"426","urn":"ge:GENC:3:1-2:LSO"},
{"n":"LITHUANIA","sN":"Lithuania","fN":"Republic of Lithuania","lN":{"Lietuva":"lit"},"c3":"LTU","c2":"LT","fips":"LH","code":"440","urn":"ge:GENC:3:1-2:LTU"},
{"n":"LUXEMBOURG","sN":"Luxembourg","fN":"Grand Duchy of Luxembourg","lN":{"Luxembourg":"fra"},"c3":"LUX","c2":"LU","fips":"","code":"442","urn":"ge:GENC:3:1-2:LUX"},
{"n":"LATVIA","sN":"Latvia","fN":"Republic of Latvia","lN":{"Latvija":"lav"},"c3":"LVA","c2":"LV","fips":"LG","code":"428","urn":"ge:GENC:3:1-2:LVA"},
{"n":"MACAU","sN":"Macau","fN":"Macau Special Administrative Region","lN":{"Macau":"por","Aomen":"zho"},"c3":"MAC","c2":"MO","fips":"MC","code":"446","urn":"ge:GENC:3:1-2:MAC"},
{"n":"SAINT MARTIN","sN":"Saint Martin","fN":"Saint Martin","lN":{"Saint-Martin":"fra"},"c3":"MAF","c2":"MF","fips":"RN","code":"663","urn":"ge:GENC:3:1-2:MAF"},
{"n":"MOROCCO","sN":"Morocco","fN":"Kingdom of Morocco","lN":{"Al Maghrib":"ara"},"c3":"MAR","c2":"MA","fips":"MO","code":"504","urn":"ge:GENC:3:1-2:MAR"},
{"n":"MONACO","sN":"Monaco","fN":"Principality of Monaco","lN":{"Monaco":"fra"},"c3":"MCO","c2":"MC","fips":"MN","code":"492","urn":"ge:GENC:3:1-2:MCO"},
{"n":"MOLDOVA","sN":"Moldova","fN":"Republic of Moldova","lN":{"Moldova":"---"},"c3":"MDA","c2":"MD","fips":"","code":"498","urn":"ge:GENC:3:1-2:MDA"},
{"n":"MADAGASCAR","sN":"Madagascar","fN":"Republic of Madagascar","lN":{"Madagascar":"fra","Madagasikara":"mlg"},"c3":"MDG","c2":"MG","fips":"MA","code":"450","urn":"ge:GENC:3:1-2:MDG"},
{"n":"MALDIVES","sN":"Maldives","fN":"Republic of Maldives","lN":{"Dhivehi Raajje":"div"},"c3":"MDV","c2":"MV","fips":"","code":"462","urn":"ge:GENC:3:1-2:MDV"},
{"n":"MEXICO","sN":"Mexico","fN":"United Mexican States","lN":{"México":"spa"},"c3":"MEX","c2":"MX","fips":"","code":"484","urn":"ge:GENC:3:1-2:MEX"},
{"n":"MARSHALL ISLANDS","sN":"Marshall Islands","fN":"Republic of the Marshall Islands","lN":{"Marshall Islands":"eng"},"c3":"MHL","c2":"MH","fips":"RM","code":"584","urn":"ge:GENC:3:1-2:MHL"},
{"n":"MACEDONIA","sN":"Macedonia","fN":"Republic of Macedonia","lN":{"Makedonija":"mkd"},"c3":"MKD","c2":"MK","fips":"","code":"807","urn":"ge:GENC:3:1-2:MKD"},
{"n":"MALI","sN":"Mali","fN":"Republic of Mali","lN":{"Mali":"fra"},"c3":"MLI","c2":"ML","fips":"","code":"466","urn":"ge:GENC:3:1-2:MLI"},
{"n":"MALTA","sN":"Malta","fN":"Republic of Malta","lN":{"Malta":"eng","Malta":"mlt"},"c3":"MLT","c2":"MT","fips":"","code":"470","urn":"ge:GENC:3:1-2:MLT"},
{"n":"BURMA","sN":"Burma","fN":"Union of Burma","lN":{"Myanma Naingngandaw":"mya"},"c3":"MMR","c2":"MM","fips":"BM","code":"104","urn":"ge:GENC:3:1-2:MMR"},
{"n":"MONTENEGRO","sN":"Montenegro","fN":"Montenegro","lN":{"Crna Gora":"---"},"c3":"MNE","c2":"ME","fips":"MJ","code":"499","urn":"ge:ISO1:3:VI-15:MNE"},
{"n":"MONGOLIA","sN":"Mongolia","fN":"Mongolia","lN":{"Mongol Uls":"mon"},"c3":"MNG","c2":"MN","fips":"MG","code":"496","urn":"ge:GENC:3:1-2:MNG"},
{"n":"NORTHERN MARIANA ISLANDS","sN":"Northern Mariana Islands","fN":"Commonwealth of the Northern Mariana Islands","lN":{"Northern Mariana Islands":"eng"},"c3":"MNP","c2":"MP","fips":"CQ","code":"580","urn":"ge:GENC:3:1-2:MNP"},
{"n":"MOZAMBIQUE","sN":"Mozambique","fN":"Republic of Mozambique","lN":{"Moçambique":"por"},"c3":"MOZ","c2":"MZ","fips":"","code":"508","urn":"ge:GENC:3:1-2:MOZ"},
{"n":"MAURITANIA","sN":"Mauritania","fN":"Islamic Republic of Mauritania","lN":{"Mūrītāniyā":"ara"},"c3":"MRT","c2":"MR","fips":"","code":"478","urn":"ge:GENC:3:1-2:MRT"},
{"n":"MONTSERRAT","sN":"Montserrat","fN":"Montserrat","lN":{"Montserrat":"eng"},"c3":"MSR","c2":"MS","fips":"MH","code":"500","urn":"ge:ISO1:3:VI-15:MSR"},
{"n":"MARTINIQUE","sN":"Martinique","fN":"Department of Martinique","lN":{"Martinique":"fra"},"c3":"MTQ","c2":"MQ","fips":"MB","code":"474","urn":"ge:GENC:3:1-2:MTQ"},
{"n":"MAURITIUS","sN":"Mauritius","fN":"Republic of Mauritius","lN":{"Mauritius":"eng"},"c3":"MUS","c2":"MU","fips":"MP","code":"480","urn":"ge:GENC:3:1-2:MUS"},
{"n":"MALAWI","sN":"Malawi","fN":"Republic of Malawi","lN":{"Malawi":"eng"},"c3":"MWI","c2":"MW","fips":"MI","code":"454","urn":"ge:GENC:3:1-2:MWI"},
{"n":"MALAYSIA","sN":"Malaysia","fN":"Malaysia","lN":{"Malaysia":"msa"},"c3":"MYS","c2":"MY","fips":"","code":"458","urn":"ge:GENC:3:1-2:MYS"},
{"n":"MAYOTTE","sN":"Mayotte","fN":"Department of Mayotte","lN":{"Mayotte":"fra"},"c3":"MYT","c2":"YT","fips":"MF","code":"175","urn":"ge:GENC:3:1-2:MYT"},
{"n":"NAMIBIA","sN":"Namibia","fN":"Republic of Namibia","lN":{"Namibia":"eng"},"c3":"NAM","c2":"NA","fips":"WA","code":"516","urn":"ge:GENC:3:1-2:NAM"},
{"n":"NEW CALEDONIA","sN":"New Caledonia","fN":"New Caledonia","lN":{"Nouvelle-Calédonie":"fra"},"c3":"NCL","c2":"NC","fips":"","code":"540","urn":"ge:GENC:3:1-2:NCL"},
{"n":"NIGER","sN":"Niger","fN":"Republic of the Niger","lN":{"Niger":"fra"},"c3":"NER","c2":"NE","fips":"NG","code":"562","urn":"ge:GENC:3:1-2:NER"},
{"n":"NORFOLK ISLAND","sN":"Norfolk Island","fN":"Territory of Norfolk Island","lN":{"Norfolk Island":"eng"},"c3":"NFK","c2":"NF","fips":"","code":"574","urn":"ge:GENC:3:1-2:NFK"},
{"n":"NIGERIA","sN":"Nigeria","fN":"Federal Republic of Nigeria","lN":{"Nigeria":"eng"},"c3":"NGA","c2":"NG","fips":"NI","code":"566","urn":"ge:GENC:3:1-2:NGA"},
{"n":"NICARAGUA","sN":"Nicaragua","fN":"Republic of Nicaragua","lN":{"Nicaragua":"spa"},"c3":"NIC","c2":"NI","fips":"NU","code":"558","urn":"ge:GENC:3:1-2:NIC"},
{"n":"NIUE","sN":"Niue","fN":"Niue","lN":{"Niue":"eng"},"c3":"NIU","c2":"NU","fips":"NE","code":"570","urn":"ge:ISO1:3:VI-15:NIU"},
{"n":"NETHERLANDS","sN":"Netherlands","fN":"Kingdom of the Netherlands","lN":{"Nederland":"nld"},"c3":"NLD","c2":"NL","fips":"","code":"528","urn":"ge:GENC:3:1-2:NLD"},
{"n":"NORWAY","sN":"Norway","fN":"Kingdom of Norway","lN":{"Norge":"nob"},"c3":"NOR","c2":"NO","fips":"","code":"578","urn":"ge:GENC:3:1-2:NOR"},
{"n":"NEPAL","sN":"Nepal","fN":"Federal Democratic Republic of Nepal","lN":{"Nepāl":"nep"},"c3":"NPL","c2":"NP","fips":"","code":"524","urn":"ge:GENC:3:1-2:NPL"},
{"n":"NAURU","sN":"Nauru","fN":"Republic of Nauru","lN":{"Nauru":"eng","Naoero":"nau"},"c3":"NRU","c2":"NR","fips":"","code":"520","urn":"ge:GENC:3:1-2:NRU"},
{"n":"NEW ZEALAND","sN":"New Zealand","fN":"New Zealand","lN":{"New Zealand":"eng"},"c3":"NZL","c2":"NZ","fips":"","code":"554","urn":"ge:GENC:3:1-2:NZL"},
{"n":"OMAN","sN":"Oman","fN":"Sultanate of Oman","lN":{"‘Umān":"ara"},"c3":"OMN","c2":"OM","fips":"MU","code":"512","urn":"ge:GENC:3:1-2:OMN"},
{"n":"PAKISTAN","sN":"Pakistan","fN":"Islamic Republic of Pakistan","lN":{"Pakistan":"eng","Pākistān":"urd"},"c3":"PAK","c2":"PK","fips":"","code":"586","urn":"ge:GENC:3:1-2:PAK"},
{"n":"PANAMA","sN":"Panama","fN":"Republic of Panama","lN":{"Panamá":"spa"},"c3":"PAN","c2":"PA","fips":"PM","code":"591","urn":"ge:GENC:3:1-2:PAN"},
{"n":"PITCAIRN ISLANDS","sN":"Pitcairn Islands","fN":"Pitcairn, Henderson, Ducie, and Oeno Islands","lN":{"Pitcairn Islands":"eng"},"c3":"PCN","c2":"PN","fips":"PC","code":"612","urn":"ge:GENC:3:1-2:PCN"},
{"n":"PERU","sN":"Peru","fN":"Republic of Peru","lN":{"Perú":"spa"},"c3":"PER","c2":"PE","fips":"","code":"604","urn":"ge:GENC:3:1-2:PER"},
{"n":"PHILIPPINES","sN":"Philippines","fN":"Republic of the Philippines","lN":{"Philippines":"eng","Pilipinas":"tgl"},"c3":"PHL","c2":"PH","fips":"RP","code":"608","urn":"ge:GENC:3:1-2:PHL"},
{"n":"PALAU","sN":"Palau","fN":"Republic of Palau","lN":{"Palau":"eng","Belau":"pau"},"c3":"PLW","c2":"PW","fips":"PS","code":"585","urn":"ge:GENC:3:1-2:PLW"},
{"n":"PAPUA NEW GUINEA","sN":"Papua New Guinea","fN":"Independent State of Papua New Guinea","lN":{"Papua New Guinea":"eng","Papuaniugini":"hmo","Papuaniugini":"tpi"},"c3":"PNG","c2":"PG","fips":"PP","code":"598","urn":"ge:ISO1:3:VI-15:PNG"},
{"n":"POLAND","sN":"Poland","fN":"Republic of Poland","lN":{"Polska":"pol"},"c3":"POL","c2":"PL","fips":"PL","code":"616","urn":"ge:GENC:3:1-2:POL"},
{"n":"PUERTO RICO","sN":"Puerto Rico","fN":"Commonwealth of Puerto Rico","lN":{"Puerto Rico":"eng","Puerto Rico":"spa"},"c3":"PRI","c2":"PR","fips":"RQ","code":"630","urn":"ge:GENC:3:1-2:PRI"},
{"n":"KOREA, NORTH","sN":"Korea, North","fN":"Democratic People’s Republic of Korea","lN":{"Chosŏn-minjujuŭi-inmin-konghwaguk":"kor"},"c3":"PRK","c2":"KP","fips":"KN","code":"408","urn":"ge:GENC:3:1-2:PRK"},
{"n":"PORTUGAL","sN":"Portugal","fN":"Portuguese Republic","lN":{"Portugal":"por"},"c3":"PRT","c2":"PT","fips":"PO","code":"620","urn":"ge:GENC:3:1-2:PRT"},
{"n":"PARAGUAY","sN":"Paraguay","fN":"Republic of Paraguay","lN":{"Paraguay":"spa"},"c3":"PRY","c2":"PY","fips":"PA","code":"600","urn":"ge:GENC:3:1-2:PRY"},
{"n":"PALESTINIAN TERRITORY","sN":"Palestinian Territory","fN":"Palestinian Territory","lN":{},"c3":"PSE","c2":"PS","fips":"WE","code":"275","urn":"ge:GENC:3:1-2:PSE"},
{"n":"FRENCH POLYNESIA","sN":"French Polynesia","fN":"French Polynesia","lN":{"Polynésie Française":"fra"},"c3":"PYF","c2":"PF","fips":"FP","code":"258","urn":"ge:GENC:3:1-2:PYF"},
{"n":"QATAR","sN":"Qatar","fN":"State of Qatar","lN":{"Qaţar":"ara"},"c3":"QAT","c2":"QA","fips":"","code":"634","urn":"ge:GENC:3:1-2:QAT"},
{"n":"REUNION","sN":"Reunion","fN":"Department of Reunion","lN":{"Réunion":"fra"},"c3":"REU","c2":"RE","fips":"","code":"638","urn":"ge:GENC:3:1-2:REU"},
{"n":"ROMANIA","sN":"Romania","fN":"Romania","lN":{"România":"ron"},"c3":"ROU","c2":"RO","fips":"","code":"642","urn":"ge:ISO1:3:VI-15:ROU"},
{"n":"RUSSIA","sN":"Russia","fN":"Russian Federation","lN":{"Rossiya":"rus"},"c3":"RUS","c2":"RU","fips":"RS","code":"643","urn":"ge:GENC:3:1-2:RUS"},
{"n":"RWANDA","sN":"Rwanda","fN":"Republic of Rwanda","lN":{"Rwanda":"eng","Rwanda":"fra","Rwanda":"kin"},"c3":"RWA","c2":"RW","fips":"","code":"646","urn":"ge:GENC:3:1-2:RWA"},
{"n":"SAUDI ARABIA","sN":"Saudi Arabia","fN":"Kingdom of Saudi Arabia","lN":{"Al ‘Arabīyah as Su‘ūdīyah":"ara"},"c3":"SAU","c2":"SA","fips":"","code":"682","urn":"ge:GENC:3:1-2:SAU"},
{"n":"SUDAN","sN":"Sudan","fN":"Republic of the Sudan","lN":{"As Sūdān":"ara"},"c3":"SDN","c2":"SD","fips":"SU","code":"729","urn":"ge:GENC:3:1-2:SDN"},
{"n":"SENEGAL","sN":"Senegal","fN":"Republic of Senegal","lN":{"Sénégal":"fra"},"c3":"SEN","c2":"SN","fips":"SG","code":"686","urn":"ge:GENC:3:1-2:SEN"},
{"n":"SINGAPORE","sN":"Singapore","fN":"Republic of Singapore","lN":{"Singapore":"eng"},"c3":"SGP","c2":"SG","fips":"SN","code":"702","urn":"ge:GENC:3:1-2:SGP"},
{"n":"SOUTH GEORGIA AND SOUTH SANDWICH ISLANDS","sN":"South Georgia and South Sandwich Islands","fN":"South Georgia and the South Sandwich Islands","lN":{"South Georgia and South Sandwich Islands":"eng"},"c3":"SGS","c2":"GS","fips":"SX","code":"239","urn":"ge:GENC:3:1-2:SGS"},
{"n":"SAINT HELENA, ASCENSION, AND TRISTAN DA CUNHA","sN":"Saint Helena, Ascension, and Tristan da Cunha","fN":"Saint Helena, Ascension, and Tristan da Cunha","lN":{"Saint Helena, Ascension, and Tristan da Cunha":"eng"},"c3":"SHN","c2":"SH","fips":"","code":"654","urn":"ge:GENC:3:1-2:SHN"},
{"n":"SVALBARD AND JAN MAYEN","sN":"Svalbard and Jan Mayen","fN":"Svalbard and Jan Mayen","lN":{"Svalbard og Jan Mayen":"nno","Svalbard og Jan Mayen":"nob"},"c3":"SJM","c2":"SJ","fips":"SV","code":"744","urn":"ge:GENC:3:1-2:SJM"},
{"n":"SOLOMON ISLANDS","sN":"Solomon Islands","fN":"Solomon Islands","lN":{"Solomon Islands":"eng"},"c3":"SLB","c2":"SB","fips":"BP","code":"090","urn":"ge:GENC:3:1-2:SLB"},
{"n":"SIERRA LEONE","sN":"Sierra Leone","fN":"Republic of Sierra Leone","lN":{"Sierra Leone":"eng"},"c3":"SLE","c2":"SL","fips":"SL","code":"694","urn":"ge:GENC:3:1-2:SLE"},
{"n":"EL SALVADOR","sN":"El Salvador","fN":"Republic of El Salvador","lN":{"El Salvador":"spa"},"c3":"SLV","c2":"SV","fips":"ES","code":"222","urn":"ge:GENC:3:1-2:SLV"},
{"n":"SAN MARINO","sN":"San Marino","fN":"Republic of San Marino","lN":{"San Marino":"ita"},"c3":"SMR","c2":"SM","fips":"","code":"674","urn":"ge:GENC:3:1-2:SMR"},
{"n":"SOMALIA","sN":"Somalia","fN":"Somalia, Federal Republic of","lN":{"Aş Şūmāl":"ara","Soomaaliya":"som"},"c3":"SOM","c2":"SO","fips":"SO","code":"706","urn":"ge:GENC:3:1-2:SOM"},
{"n":"SAINT PIERRE AND MIQUELON","sN":"Saint Pierre and Miquelon","fN":"Territorial Collectivity of Saint Pierre and Miquelon","lN":{"Saint-Pierre et Miquelon":"fra"},"c3":"SPM","c2":"PM","fips":"SB","code":"666","urn":"ge:GENC:3:1-2:SPM"},
{"n":"SERBIA","sN":"Serbia","fN":"Republic of Serbia","lN":{"Srbija":"srp"},"c3":"SRB","c2":"RS","fips":"RI","code":"688","urn":"ge:GENC:3:1-2:SRB"},
{"n":"SOUTH SUDAN","sN":"South Sudan","fN":"Republic of South Sudan","lN":{"South Sudan":"eng"},"c3":"SSD","c2":"SS","fips":"OD","code":"728","urn":"ge:GENC:3:1-2:SSD"},
{"n":"SAO TOME AND PRINCIPE","sN":"Sao Tome and Principe","fN":"Democratic Republic of Sao Tome and Principe","lN":{"São Tomé e Príncipe":"por"},"c3":"STP","c2":"ST","fips":"TP","code":"678","urn":"ge:GENC:3:1-2:STP"},
{"n":"SURINAME","sN":"Suriname","fN":"Republic of Suriname","lN":{"Suriname":"nld"},"c3":"SUR","c2":"SR","fips":"NS","code":"740","urn":"ge:GENC:3:1-2:SUR"},
{"n":"SLOVAKIA","sN":"Slovakia","fN":"Slovak Republic","lN":{"Slovensko":"slk"},"c3":"SVK","c2":"SK","fips":"LO","code":"703","urn":"ge:GENC:3:1-2:SVK"},
{"n":"SLOVENIA","sN":"Slovenia","fN":"Republic of Slovenia","lN":{"Slovenija":"slv"},"c3":"SVN","c2":"SI","fips":"","code":"705","urn":"ge:GENC:3:1-2:SVN"},
{"n":"SWEDEN","sN":"Sweden","fN":"Kingdom of Sweden","lN":{"Sverige":"swe"},"c3":"SWE","c2":"SE","fips":"SW","code":"752","urn":"ge:GENC:3:1-2:SWE"},
{"n":"SWAZILAND","sN":"Swaziland","fN":"Kingdom of Swaziland","lN":{"Swaziland":"eng"},"c3":"SWZ","c2":"SZ","fips":"WZ","code":"748","urn":"ge:GENC:3:1-2:SWZ"},
{"n":"SINT MAARTEN","sN":"Sint Maarten","fN":"Sint Maarten","lN":{"Sint Maarten":"eng","Sint Maarten":"nld"},"c3":"SXM","c2":"SX","fips":"NN","code":"534","urn":"ge:GENC:3:1-2:SXM"},
{"n":"SEYCHELLES","sN":"Seychelles","fN":"Republic of Seychelles","lN":{"Seychelles":"eng","Seychelles":"fra"},"c3":"SYC","c2":"SC","fips":"SE","code":"690","urn":"ge:GENC:3:1-2:SYC"},
{"n":"SYRIA","sN":"Syria","fN":"Syrian Arab Republic","lN":{"Sūrīyah":"ara"},"c3":"SYR","c2":"SY","fips":"","code":"760","urn":"ge:GENC:3:1-2:SYR"},
{"n":"TURKS AND CAICOS ISLANDS","sN":"Turks and Caicos Islands","fN":"Turks and Caicos Islands","lN":{"Turks and Caicos Islands":"eng"},"c3":"TCA","c2":"TC","fips":"TK","code":"796","urn":"ge:GENC:3:1-2:TCA"},
{"n":"CHAD","sN":"Chad","fN":"Republic of Chad","lN":{"Tchad":"fra"},"c3":"TCD","c2":"TD","fips":"CD","code":"148","urn":"ge:GENC:3:1-2:TCD"},
{"n":"TOGO","sN":"Togo","fN":"Togolese Republic","lN":{"Togo":"fra"},"c3":"TGO","c2":"TG","fips":"TO","code":"768","urn":"ge:GENC:3:1-2:TGO"},
{"n":"THAILAND","sN":"Thailand","fN":"Kingdom of Thailand","lN":{"Prathet Thai":"tha"},"c3":"THA","c2":"TH","fips":"TH","code":"764","urn":"ge:GENC:3:1-2:THA"},
{"n":"TAJIKISTAN","sN":"Tajikistan","fN":"Republic of Tajikistan","lN":{"Tojikiston":"tgk"},"c3":"TJK","c2":"TJ","fips":"TI","code":"762","urn":"ge:GENC:3:1-2:TJK"},
{"n":"TOKELAU","sN":"Tokelau","fN":"Tokelau","lN":{"Tokelau":"eng"},"c3":"TKL","c2":"TK","fips":"TL","code":"772","urn":"ge:ISO1:3:VI-15:TKL"},
{"n":"TURKMENISTAN","sN":"Turkmenistan","fN":"Turkmenistan","lN":{"Türkmenistan":"tuk"},"c3":"TKM","c2":"TM","fips":"TX","code":"795","urn":"ge:ISO1:3:VI-15:TKM"},
{"n":"TIMOR-LESTE","sN":"Timor-Leste","fN":"Democratic Republic of Timor-Leste","lN":{"Timor-Leste":"por","Timor Lorosa’e":"tet"},"c3":"TLS","c2":"TL","fips":"TT","code":"626","urn":"ge:GENC:3:1-2:TLS"},
{"n":"TONGA","sN":"Tonga","fN":"Kingdom of Tonga","lN":{"Tonga":"eng"},"c3":"TON","c2":"TO","fips":"TN","code":"776","urn":"ge:GENC:3:1-2:TON"},
{"n":"TRINIDAD AND TOBAGO","sN":"Trinidad and Tobago","fN":"Republic of Trinidad and Tobago","lN":{"Trinidad and Tobago":"eng"},"c3":"TTO","c2":"TT","fips":"TD","code":"780","urn":"ge:GENC:3:1-2:TTO"},
{"n":"TUNISIA","sN":"Tunisia","fN":"Tunisian Republic","lN":{"Tūnis":"ara"},"c3":"TUN","c2":"TN","fips":"TS","code":"788","urn":"ge:GENC:3:1-2:TUN"},
{"n":"TURKEY","sN":"Turkey","fN":"Republic of Turkey","lN":{"Türkiye":"tur"},"c3":"TUR","c2":"TR","fips":"TU","code":"792","urn":"ge:GENC:3:1-2:TUR"},
{"n":"TUVALU","sN":"Tuvalu","fN":"Tuvalu","lN":{"Tuvalu":"eng","Tuvalu":"tvl"},"c3":"TUV","c2":"TV","fips":"","code":"798","urn":"ge:ISO1:3:VI-15:TUV"},
{"n":"TAIWAN","sN":"Taiwan","fN":"Taiwan","lN":{"Taiwan":"zho"},"c3":"TWN","c2":"TW","fips":"","code":"158","urn":"ge:GENC:3:1-2:TWN"},
{"n":"TANZANIA","sN":"Tanzania","fN":"United Republic of Tanzania","lN":{"Tanzania":"eng","Tanzania":"swa"},"c3":"TZA","c2":"TZ","fips":"","code":"834","urn":"ge:GENC:3:1-2:TZA"},
{"n":"UGANDA","sN":"Uganda","fN":"Republic of Uganda","lN":{"Uganda":"eng"},"c3":"UGA","c2":"UG","fips":"","code":"800","urn":"ge:GENC:3:1-2:UGA"},
{"n":"UKRAINE","sN":"Ukraine","fN":"Ukraine","lN":{"Ukrayina":"ukr"},"c3":"UKR","c2":"UA","fips":"UP","code":"804","urn":"ge:GENC:3:1-2:UKR"},
{"n":"UNITED STATES MINOR OUTLYING ISLANDS","sN":"United States Minor Outlying Islands (the)","fN":"United States Minor Outlying Islands (the)","lN":{"United States Minor Outlying Islands (the)":"eng"},"c3":"UMI","c2":"UM","fips":"","code":"581","urn":"ge:GENC:3:1-2:UMI"},
{"n":"URUGUAY","sN":"Uruguay","fN":"Oriental Republic of Uruguay","lN":{"Uruguay":"spa"},"c3":"URY","c2":"UY","fips":"","code":"858","urn":"ge:GENC:3:1-2:URY"},
{"n":"UNITED STATES","sN":"United States","fN":"United States of America","lN":{"United States":"eng"},"c3":"USA","c2":"US","fips":"","code":"840","urn":"ge:GENC:3:1-2:USA"},
{"n":"UZBEKISTAN","sN":"Uzbekistan","fN":"Republic of Uzbekistan","lN":{"O’zbekiston":"uzb"},"c3":"UZB","c2":"UZ","fips":"","code":"860","urn":"ge:GENC:3:1-2:UZB"},
{"n":"VATICAN CITY","sN":"Vatican City","fN":"State of the Vatican City","lN":{"Città del Vaticano":"ita","Sancta Sedes":"lat"},"c3":"VAT","c2":"VA","fips":"VT","code":"336","urn":"ge:GENC:3:1-2:VAT"},
{"n":"SAINT VINCENT AND THE GRENADINES","sN":"Saint Vincent and the Grenadines","fN":"Saint Vincent and the Grenadines","lN":{"Saint Vincent and the Grenadines":"eng"},"c3":"VCT","c2":"VC","fips":"VC","code":"670","urn":"ge:GENC:3:1-2:VCT"},
{"n":"VENEZUELA","sN":"Venezuela","fN":"Bolivarian Republic of Venezuela","lN":{"Venezuela":"spa"},"c3":"VEN","c2":"VE","fips":"VE","code":"862","urn":"ge:GENC:3:1-2:VEN"},
{"n":"VIRGIN ISLANDS, BRITISH","sN":"Virgin Islands, British","fN":"British Virgin Islands","lN":{},"c3":"VGB","c2":"VG","fips":"VI","code":"092","urn":"ge:GENC:3:1-2:VGB"},
{"n":"VIRGIN ISLANDS, U.S.","sN":"Virgin Islands, U.S.","fN":"United States Virgin Islands","lN":{"Virgin Islands of the United States":"eng"},"c3":"VIR","c2":"VI","fips":"VQ","code":"850","urn":"ge:GENC:3:1-2:VIR"},
{"n":"VIETNAM","sN":"Vietnam","fN":"Socialist Republic of Vietnam","lN":{"Việt Nam":"vie"},"c3":"VNM","c2":"VN","fips":"VM","code":"704","urn":"ge:GENC:3:1-2:VNM"},
{"n":"VANUATU","sN":"Vanuatu","fN":"Republic of Vanuatu","lN":{"Vanuatu":"bis","Vanuatu":"eng"},"c3":"VUT","c2":"VU","fips":"NH","code":"548","urn":"ge:GENC:3:1-2:VUT"},
{"n":"WALLIS AND FUTUNA","sN":"Wallis and Futuna","fN":"Wallis and Futuna","lN":{"Wallis et Futuna":"fra"},"c3":"WLF","c2":"WF","fips":"WF","code":"876","urn":"ge:GENC:3:1-2:WLF"},
{"n":"SAMOA","sN":"Samoa","fN":"Independent State of Samoa","lN":{"Samoa":"eng","Samoa":"smo"},"c3":"WSM","c2":"WS","fips":"WS","code":"882","urn":"ge:GENC:3:1-2:WSM"},
{"n":"ASHMORE AND CARTIER ISLANDS","sN":"Ashmore and Cartier Islands","fN":"Territory of Ashmore and Cartier Islands","lN":{},"c3":"XAC","c2":"XA","fips":"","code":"","urn":"ge:GENC:3:1-2:XAC"},
{"n":"ENTITY 1","sN":"Entity 1","fN":"Entity 1","lN":{},"c3":"XAZ","c2":"","fips":"","code":"","urn":"ge:GENC:3:1-2:XAZ"},
{"n":"BASSAS DA INDIA","sN":"Bassas da India","fN":"Bassas da India","lN":{},"c3":"XBI","c2":"QS","fips":"","code":"","urn":"ge:GENC:3:1-2:XBI"},
{"n":"BAKER ISLAND","sN":"Baker Island","fN":"Baker Island","lN":{},"c3":"XBK","c2":"XB","fips":"","code":"","urn":"ge:GENC:3:1-2:XBK"},
{"n":"ENTITY 2","sN":"Entity 2","fN":"Entity 2","lN":{},"c3":"XCR","c2":"","fips":"","code":"","urn":"ge:GENC:3:1-2:XCR"},
{"n":"CORAL SEA ISLANDS","sN":"Coral Sea Islands","fN":"Coral Sea Islands Territory","lN":{},"c3":"XCS","c2":"XC","fips":"","code":"","urn":"ge:GENC:3:1-2:XCS"},
{"n":"ENTITY 3","sN":"Entity 3","fN":"Entity 3","lN":{},"c3":"XCY","c2":"","fips":"","code":"","urn":"ge:GENC:3:1-2:XCY"},
{"n":"EUROPA ISLAND","sN":"Europa Island","fN":"Europa Island","lN":{},"c3":"XEU","c2":"XE","fips":"","code":"","urn":"ge:GENC:3:1-2:XEU"},
{"n":"GLORIOSO ISLANDS","sN":"Glorioso Islands","fN":"Glorioso Islands","lN":{},"c3":"XGL","c2":"QX","fips":"","code":"","urn":"ge:GENC:3:1-2:XGL"},
{"n":"GAZA STRIP","sN":"Gaza Strip","fN":"Gaza Strip","lN":{},"c3":"XGZ","c2":"XG","fips":"","code":"","urn":"ge:GENC:3:1-2:XGZ"},
{"n":"HOWLAND ISLAND","sN":"Howland Island","fN":"Howland Island","lN":{},"c3":"XHO","c2":"XH","fips":"","code":"","urn":"ge:GENC:3:1-2:XHO"},
{"n":"JOHNSTON ATOLL","sN":"Johnston Atoll","fN":"Johnston Atoll","lN":{},"c3":"XJA","c2":"XU","fips":"","code":"","urn":"ge:GENC:3:1-2:XJA"},
{"n":"JAN MAYEN","sN":"Jan Mayen","fN":"Jan Mayen","lN":{},"c3":"XJM","c2":"XJ","fips":"","code":"","urn":"ge:GENC:3:1-2:XJM"},
{"n":"JUAN DE NOVA ISLAND","sN":"Juan de Nova Island","fN":"Juan de Nova Island","lN":{"Île Juan de Nova":"fra"},"c3":"XJN","c2":"QU","fips":"","code":"","urn":"ge:GENC:3:1-2:XJN"},
{"n":"JARVIS ISLAND","sN":"Jarvis Island","fN":"Jarvis Island","lN":{},"c3":"XJV","c2":"XQ","fips":"","code":"","urn":"ge:GENC:3:1-2:XJV"},
{"n":"ENTITY 4","sN":"Entity 4","fN":"Entity 4","lN":{},"c3":"XKM","c2":"","fips":"","code":"","urn":"ge:GENC:3:1-2:XKM"},
{"n":"ENTITY 5","sN":"Entity 5","fN":"Entity 5","lN":{},"c3":"XKN","c2":"","fips":"","code":"","urn":"ge:GENC:3:1-2:XKN"},
{"n":"KINGMAN REEF","sN":"Kingman Reef","fN":"Kingman Reef","lN":{},"c3":"XKR","c2":"XM","fips":"","code":"","urn":"ge:GENC:3:1-2:XKR"},
{"n":"KOSOVO","sN":"Kosovo","fN":"Republic of Kosovo","lN":{},"c3":"XKS","c2":"XK","fips":"","code":"","urn":"ge:GENC:3:1-2:XKS"},
{"n":"MIDWAY ISLANDS","sN":"Midway Islands","fN":"Midway Islands","lN":{},"c3":"XMW","c2":"QM","fips":"","code":"","urn":"ge:GENC:3:1-2:XMW"},
{"n":"NAVASSA ISLAND","sN":"Navassa Island","fN":"Navassa Island","lN":{},"c3":"XNV","c2":"XV","fips":"","code":"","urn":"ge:GENC:3:1-2:XNV"},
{"n":"PALMYRA ATOLL","sN":"Palmyra Atoll","fN":"Palmyra Atoll","lN":{},"c3":"XPL","c2":"XL","fips":"","code":"","urn":"ge:GENC:3:1-2:XPL"},
{"n":"PARACEL ISLANDS","sN":"Paracel Islands","fN":"Paracel Islands","lN":{},"c3":"XPR","c2":"XP","fips":"","code":"","urn":"ge:GENC:3:1-2:XPR"},
{"n":"ETOROFU, HABOMAI, KUNASHIRI, AND SHIKOTAN ISLANDS","sN":"Etorofu, Habomai, Kunashiri, and Shikotan Islands","fN":"Etorofu, Habomai, Kunashiri, and Shikotan Islands","lN":{},"c3":"XQP","c2":"QP","fips":"","code":"","urn":"ge:GENC:3:1-2:XQP"},
{"n":"AKROTIRI","sN":"Akrotiri","fN":"Akrotiri","lN":{},"c3":"XQZ","c2":"QZ","fips":"","code":"","urn":"ge:GENC:3:1-2:XQZ"},
{"n":"SPRATLY ISLANDS","sN":"Spratly Islands","fN":"Spratly Islands","lN":{},"c3":"XSP","c2":"XS","fips":"","code":"","urn":"ge:GENC:3:1-2:XSP"},
{"n":"SVALBARD","sN":"Svalbard","fN":"Svalbard","lN":{},"c3":"XSV","c2":"XR","fips":"","code":"","urn":"ge:GENC:3:1-2:XSV"},
{"n":"TROMELIN ISLAND","sN":"Tromelin Island","fN":"Tromelin Island","lN":{"Île Tromelin":"fra"},"c3":"XTR","c2":"XT","fips":"","code":"","urn":"ge:GENC:3:1-2:XTR"},
{"n":"WEST BANK","sN":"West Bank","fN":"West Bank","lN":{"Aş Şaḩrā’ al Gharbīyah":"ara"},"c3":"XWB","c2":"XW","fips":"","code":"","urn":"ge:GENC:3:1-2:XWB"},
{"n":"WAKE ISLAND","sN":"Wake Island","fN":"Wake Island","lN":{},"c3":"XWK","c2":"QW","fips":"","code":"","urn":"ge:GENC:3:1-2:XWK"},
{"n":"DHEKELIA","sN":"Dhekelia","fN":"Dhekelia","lN":{},"c3":"XXD","c2":"XD","fips":"","code":"","urn":"ge:GENC:3:1-2:XXD"},
{"n":"NO MAN’S LAND","sN":"No Man’s Land","fN":"No Man’s Land","lN":{},"c3":"XXX","c2":"","fips":"","code":"","urn":"ge:GENC:3:1-2:XXX"},
{"n":"YEMEN","sN":"Yemen","fN":"Republic of Yemen","lN":{"Al Yaman":"ara"},"c3":"YEM","c2":"YE","fips":"YM","code":"887","urn":"ge:GENC:3:1-2:YEM"},
{"n":"SOUTH AFRICA","sN":"South Africa","fN":"Republic of South Africa","lN":{"Suid-Afrika":"afr","South Africa":"eng","Sewula Afrika":"nbl","Afrika-Borwa":"nso","Afrika-Borwa":"sot","iNingizimu Afrika":"ssw","Aforika Borwa":"tsn","Afrika Dzonga":"tso","Afurika Tshipembe":"ven","umZantsi Afrika":"xho","iNingizimu Afrika":"zul"},"c3":"ZAF","c2":"ZA","fips":"SF","code":"710","urn":"ge:GENC:3:1-2:ZAF"},
{"n":"ZAMBIA","sN":"Zambia","fN":"Republic of Zambia","lN":{"Zambia":"eng"},"c3":"ZMB","c2":"ZM","fips":"ZA","code":"894","urn":"ge:GENC:3:1-2:ZMB"},
{"n":"ZIMBABWE","sN":"Zimbabwe","fN":"Republic of Zimbabwe","lN":{"Zimbabwe":"eng"},"c3":"ZWE","c2":"ZW","fips":"ZI","code":"716","urn":"ge:GENC:3:1-2:ZWE"},
  ],

  // General country name conversion
  // This is grungy but avoids having multiple lookup tables
  convertCountryCode: function(from,to,value) {
    if (!value || value == '') return '';

    var output = '';
    for (var i = 0, iLen = translate.countryTable.length; i < iLen; i++){
      if (translate.countryTable[i][from] == value)
      {
        output = translate.countryTable[i][to];
        break;
      }
    }

    return output
  },


  // This looks through the Local Names to try to find a country code
  convertLocalNameCountryCode: function(to,value) {
    if (!value || value == '') return '';

    var output = '';
    for (var i = 0, iLen = translate.countryTable.length; i < iLen; i++){
      if (translate.countryTable[i]['lN'])
      {
        if (value in translate.countryTable[i]['lN'])
        {
          output = translate.countryTable[i][to];
          break;            
        }
      }
    }

    return output
  },


  // Try to convert the country name by searching all fields
  // Useful if you don't know what you have
  findCountryCode: function(to,value) {
    if (!value || value == '') return '';

    var output = '';
    for (var i = 0, iLen = translate.countryTable.length; i < iLen; i++){
      if (translate.countryTable[i]['sN'] == value ||
          translate.countryTable[i]['n'] == value.toUpperCase() ||
          translate.countryTable[i]['fN'] == value ||
          translate.countryTable[i]['code'] == value ||
          translate.countryTable[i]['fips'] == value ||
          translate.countryTable[i]['c3'] == value ||
          translate.countryTable[i]['c2'] == value ||
          translate.countryTable[i]['urn'] == value
          )
      {
        output = translate.countryTable[i][to];
        break;
      }
    }

    // Try looking in the Local Names
    if (output == '') output = translate.convertLocalNameCountryCode(to,value);

    return output
  },


  // Language Codes: From http://www.loc.gov/standards/iso639-2/php/code_list.php
  // Fields:
  // n   = English Name
  // a2  = 2 char ISO 639-1 Code
  // a3b = 3 char Bibliographic ISO 639-2 Code
  // a3t = 3 char Terminology ISO 639-2 Code
  languageCodes : [
{"n":"Afar","a2":"aa","a3b":"aar","a3t":""},{"n":"Abkhazian","a2":"ab","a3b":"abk","a3t":""},
{"n":"Achinese","a2":"","a3b":"ace","a3t":""},{"n":"Acoli","a2":"","a3b":"ach","a3t":""},
{"n":"Adangme","a2":"","a3b":"ada","a3t":""},{"n":"Adyghe; Adygei","a2":"","a3b":"ady","a3t":""},
{"n":"Afro-Asiatic languages","a2":"","a3b":"afa","a3t":""},{"n":"Afrihili","a2":"","a3b":"afh","a3t":""},
{"n":"Afrikaans","a2":"af","a3b":"afr","a3t":""},{"n":"Ainu","a2":"","a3b":"ain","a3t":""},
{"n":"Akan","a2":"ak","a3b":"aka","a3t":""},{"n":"Akkadian","a2":"","a3b":"akk","a3t":""},
{"n":"Albanian","a2":"sq","a3b":"alb","a3t":"sqi"},{"n":"Aleut","a2":"","a3b":"ale","a3t":""},
{"n":"Algonquian languages","a2":"","a3b":"alg","a3t":""},{"n":"Southern Altai","a2":"","a3b":"alt","a3t":""},
{"n":"Amharic","a2":"am","a3b":"amh","a3t":""},{"n":"name, Old (ca.450-1100)","a2":"","a3b":"ang","a3t":""},
{"n":"Angika","a2":"","a3b":"anp","a3t":""},{"n":"Apache languages","a2":"","a3b":"apa","a3t":""},
{"n":"Arabic","a2":"ar","a3b":"ara","a3t":""},{"n":"Official Aramaic (700-300 BCE); Imperial Aramaic (700-300 BCE)","a2":"","a3b":"arc","a3t":""},
{"n":"Aragonese","a2":"an","a3b":"arg","a3t":""},{"n":"Armenian","a2":"hy","a3b":"arm","a3t":"hye"},
{"n":"Mapudungun; Mapuche","a2":"","a3b":"arn","a3t":""},{"n":"Arapaho","a2":"","a3b":"arp","a3t":""},
{"n":"Artificial languages","a2":"","a3b":"art","a3t":""},{"n":"Arawak","a2":"","a3b":"arw","a3t":""},
{"n":"Assamese","a2":"as","a3b":"asm","a3t":""},{"n":"Asturian; Bable; Leonese; Asturleonese","a2":"","a3b":"ast","a3t":""},
{"n":"Athapascan languages","a2":"","a3b":"ath","a3t":""},{"n":"Australian languages","a2":"","a3b":"aus","a3t":""},
{"n":"Avaric","a2":"av","a3b":"ava","a3t":""},{"n":"Avestan","a2":"ae","a3b":"ave","a3t":""},
{"n":"Awadhi","a2":"","a3b":"awa","a3t":""},{"n":"Aymara","a2":"ay","a3b":"aym","a3t":""},
{"n":"Azerbaijani","a2":"az","a3b":"aze","a3t":""},{"n":"Banda languages","a2":"","a3b":"bad","a3t":""},
{"n":"Bamileke languages","a2":"","a3b":"bai","a3t":""},{"n":"Bashkir","a2":"ba","a3b":"bak","a3t":""},
{"n":"Baluchi","a2":"","a3b":"bal","a3t":""},{"n":"Bambara","a2":"bm","a3b":"bam","a3t":""},
{"n":"Balinese","a2":"","a3b":"ban","a3t":""},{"n":"Basque","a2":"eu","a3b":"baq","a3t":"eus"},
{"n":"Basa","a2":"","a3b":"bas","a3t":""},{"n":"Baltic languages","a2":"","a3b":"bat","a3t":""},
{"n":"Beja; Bedawiyet","a2":"","a3b":"bej","a3t":""},{"n":"Belarusian","a2":"be","a3b":"bel","a3t":""},
{"n":"Bemba","a2":"","a3b":"bem","a3t":""},{"n":"Bengali","a2":"bn","a3b":"ben","a3t":""},
{"n":"Berber languages","a2":"","a3b":"ber","a3t":""},{"n":"Bhojpuri","a2":"","a3b":"bho","a3t":""},
{"n":"Bihari languages","a2":"bh","a3b":"bih","a3t":""},{"n":"Bikol","a2":"","a3b":"bik","a3t":""},
{"n":"Bini; Edo","a2":"","a3b":"bin","a3t":""},{"n":"Bislama","a2":"bi","a3b":"bis","a3t":""},
{"n":"Siksika","a2":"","a3b":"bla","a3t":""},{"n":"Bantu languages","a2":"","a3b":"bnt","a3t":""},
{"n":"Bosnian","a2":"bs","a3b":"bos","a3t":""},{"n":"Braj","a2":"","a3b":"bra","a3t":""},
{"n":"Breton","a2":"br","a3b":"bre","a3t":""},{"n":"Batak languages","a2":"","a3b":"btk","a3t":""},
{"n":"Buriat","a2":"","a3b":"bua","a3t":""},{"n":"Buginese","a2":"","a3b":"bug","a3t":""},
{"n":"Bulgarian","a2":"bg","a3b":"bul","a3t":""},{"n":"Burmese","a2":"my","a3b":"bur","a3t":"mya"},
{"n":"Blin; Bilin","a2":"","a3b":"byn","a3t":""},{"n":"Caddo","a2":"","a3b":"cad","a3t":""},
{"n":"Central American Indian languages","a2":"","a3b":"cai","a3t":""},{"n":"Galibi Carib","a2":"","a3b":"car","a3t":""},
{"n":"Catalan; Valencian","a2":"ca","a3b":"cat","a3t":""},{"n":"Caucasian languages","a2":"","a3b":"cau","a3t":""},
{"n":"Cebuano","a2":"","a3b":"ceb","a3t":""},{"n":"Celtic languages","a2":"","a3b":"cel","a3t":""},
{"n":"Chamorro","a2":"ch","a3b":"cha","a3t":""},{"n":"Chibcha","a2":"","a3b":"chb","a3t":""},
{"n":"Chechen","a2":"ce","a3b":"che","a3t":""},{"n":"Chagatai","a2":"","a3b":"chg","a3t":""},
{"n":"Chinese","a2":"zh","a3b":"chi","a3t":"zho"},{"n":"Chuukese","a2":"","a3b":"chk","a3t":""},
{"n":"Mari","a2":"","a3b":"chm","a3t":""},{"n":"Chinook jargon","a2":"","a3b":"chn","a3t":""},
{"n":"Choctaw","a2":"","a3b":"cho","a3t":""},{"n":"Chipewyan; Dene Suline","a2":"","a3b":"chp","a3t":""},
{"n":"Cherokee","a2":"","a3b":"chr","a3t":""},{"n":"Church Slavic; Old Slavonic; Church Slavonic; Old Bulgarian; Old Church Slavonic","a2":"cu","a3b":"chu","a3t":""},
{"n":"Chuvash","a2":"cv","a3b":"chv","a3t":""},{"n":"Cheyenne","a2":"","a3b":"chy","a3t":""},
{"n":"Chamic languages","a2":"","a3b":"cmc","a3t":""},{"n":"Montenegrin","a2":"","a3b":"cnr","a3t":""},
{"n":"Coptic","a2":"","a3b":"cop","a3t":""},{"n":"Cornish","a2":"kw","a3b":"cor","a3t":""},
{"n":"Corsican","a2":"co","a3b":"cos","a3t":""},{"n":"Creoles and pidgins, name based","a2":"","a3b":"cpe","a3t":""},
{"n":"Creoles and pidgins, French-based","a2":"","a3b":"cpf","a3t":""},{"n":"Creoles and pidgins, Portuguese-based","a2":"","a3b":"cpp","a3t":""},
{"n":"Cree","a2":"cr","a3b":"cre","a3t":""},{"n":"Crimean Tatar; Crimean Turkish","a2":"","a3b":"crh","a3t":""},
{"n":"Creoles and pidgins","a2":"","a3b":"crp","a3t":""},{"n":"Kashubian","a2":"","a3b":"csb","a3t":""},
{"n":"Cushitic languages","a2":"","a3b":"cus","a3t":""},{"n":"Czech","a2":"cs","a3b":"cze","a3t":"ces"},
{"n":"Dakota","a2":"","a3b":"dak","a3t":""},{"n":"Danish","a2":"da","a3b":"dan","a3t":""},
{"n":"Dargwa","a2":"","a3b":"dar","a3t":""},{"n":"Land Dayak languages","a2":"","a3b":"day","a3t":""},
{"n":"Delaware","a2":"","a3b":"del","a3t":""},{"n":"Slave (Athapascan)","a2":"","a3b":"den","a3t":""},
{"n":"Dogrib","a2":"","a3b":"dgr","a3t":""},{"n":"Dinka","a2":"","a3b":"din","a3t":""},
{"n":"Divehi; Dhivehi; Maldivian","a2":"dv","a3b":"div","a3t":""},{"n":"Dogri","a2":"","a3b":"doi","a3t":""},
{"n":"Dravidian languages","a2":"","a3b":"dra","a3t":""},{"n":"Lower Sorbian","a2":"","a3b":"dsb","a3t":""},
{"n":"Duala","a2":"","a3b":"dua","a3t":""},{"n":"Dutch, Middle (ca.1050-1350)","a2":"","a3b":"dum","a3t":""},
{"n":"Dutch; Flemish","a2":"nl","a3b":"dut","a3t":"nld"},{"n":"Dyula","a2":"","a3b":"dyu","a3t":""},
{"n":"Dzongkha","a2":"dz","a3b":"dzo","a3t":""},{"n":"Efik","a2":"","a3b":"efi","a3t":""},
{"n":"Egyptian (Ancient)","a2":"","a3b":"egy","a3t":""},{"n":"Ekajuk","a2":"","a3b":"eka","a3t":""},
{"n":"Elamite","a2":"","a3b":"elx","a3t":""},{"n":"n","a2":"en","a3b":"eng","a3t":""},
{"n":"name, Middle (1100-1500)","a2":"","a3b":"enm","a3t":""},{"n":"Esperanto","a2":"eo","a3b":"epo","a3t":""},
{"n":"Estonian","a2":"et","a3b":"est","a3t":""},{"n":"Ewe","a2":"ee","a3b":"ewe","a3t":""},
{"n":"Ewondo","a2":"","a3b":"ewo","a3t":""},{"n":"Fang","a2":"","a3b":"fan","a3t":""},
{"n":"Faroese","a2":"fo","a3b":"fao","a3t":""},{"n":"Fanti","a2":"","a3b":"fat","a3t":""},
{"n":"Fijian","a2":"fj","a3b":"fij","a3t":""},{"n":"Filipino; Pilipino","a2":"","a3b":"fil","a3t":""},
{"n":"Finnish","a2":"fi","a3b":"fin","a3t":""},{"n":"Finno-Ugrian languages","a2":"","a3b":"fiu","a3t":""},
{"n":"Fon","a2":"","a3b":"fon","a3t":""},{"n":"French","a2":"fr","a3b":"fre","a3t":"fra"},
{"n":"French, Middle (ca.1400-1600)","a2":"","a3b":"frm","a3t":""},{"n":"French, Old (842-ca.1400)","a2":"","a3b":"fro","a3t":""},
{"n":"Northern Frisian","a2":"","a3b":"frr","a3t":""},{"n":"Eastern Frisian","a2":"","a3b":"frs","a3t":""},
{"n":"Western Frisian","a2":"fy","a3b":"fry","a3t":""},{"n":"Fulah","a2":"ff","a3b":"ful","a3t":""},
{"n":"Friulian","a2":"","a3b":"fur","a3t":""},{"n":"Ga","a2":"","a3b":"gaa","a3t":""},
{"n":"Gayo","a2":"","a3b":"gay","a3t":""},{"n":"Gbaya","a2":"","a3b":"gba","a3t":""},
{"n":"Germanic languages","a2":"","a3b":"gem","a3t":""},{"n":"Georgian","a2":"ka","a3b":"geo","a3t":"kat"},
{"n":"German","a2":"de","a3b":"ger","a3t":"deu"},{"n":"Geez","a2":"","a3b":"gez","a3t":""},
{"n":"Gilbertese","a2":"","a3b":"gil","a3t":""},{"n":"Gaelic; Scottish Gaelic","a2":"gd","a3b":"gla","a3t":""},
{"n":"Irish","a2":"ga","a3b":"gle","a3t":""},{"n":"Galician","a2":"gl","a3b":"glg","a3t":""},
{"n":"Manx","a2":"gv","a3b":"glv","a3t":""},{"n":"German, Middle High (ca.1050-1500)","a2":"","a3b":"gmh","a3t":""},
{"n":"German, Old High (ca.750-1050)","a2":"","a3b":"goh","a3t":""},{"n":"Gondi","a2":"","a3b":"gon","a3t":""},
{"n":"Gorontalo","a2":"","a3b":"gor","a3t":""},{"n":"Gothic","a2":"","a3b":"got","a3t":""},
{"n":"Grebo","a2":"","a3b":"grb","a3t":""},{"n":"Greek, Ancient (to 1453)","a2":"","a3b":"grc","a3t":""},
{"n":"Greek, Modern (1453-)","a2":"el","a3b":"gre","a3t":"ell"},{"n":"Guarani","a2":"gn","a3b":"grn","a3t":""},
{"n":"Swiss German; Alemannic; Alsatian","a2":"","a3b":"gsw","a3t":""},{"n":"Gujarati","a2":"gu","a3b":"guj","a3t":""},
{"n":"Gwich'in","a2":"","a3b":"gwi","a3t":""},{"n":"Haida","a2":"","a3b":"hai","a3t":""},
{"n":"Haitian; Haitian Creole","a2":"ht","a3b":"hat","a3t":""},{"n":"Hausa","a2":"ha","a3b":"hau","a3t":""},
{"n":"Hawaiian","a2":"","a3b":"haw","a3t":""},{"n":"Hebrew","a2":"he","a3b":"heb","a3t":""},
{"n":"Herero","a2":"hz","a3b":"her","a3t":""},{"n":"Hiligaynon","a2":"","a3b":"hil","a3t":""},
{"n":"Himachali languages; Western Pahari languages","a2":"","a3b":"him","a3t":""},{"n":"Hindi","a2":"hi","a3b":"hin","a3t":""},
{"n":"Hittite","a2":"","a3b":"hit","a3t":""},{"n":"Hmong; Mong","a2":"","a3b":"hmn","a3t":""},
{"n":"Hiri Motu","a2":"ho","a3b":"hmo","a3t":""},{"n":"Croatian","a2":"hr","a3b":"hrv","a3t":""},
{"n":"Upper Sorbian","a2":"","a3b":"hsb","a3t":""},{"n":"Hungarian","a2":"hu","a3b":"hun","a3t":""},
{"n":"Hupa","a2":"","a3b":"hup","a3t":""},{"n":"Iban","a2":"","a3b":"iba","a3t":""},
{"n":"Igbo","a2":"ig","a3b":"ibo","a3t":""},{"n":"Icelandic","a2":"is","a3b":"ice","a3t":"isl"},
{"n":"Ido","a2":"io","a3b":"ido","a3t":""},{"n":"Sichuan Yi; Nuosu","a2":"ii","a3b":"iii","a3t":""},
{"n":"Ijo languages","a2":"","a3b":"ijo","a3t":""},{"n":"Inuktitut","a2":"iu","a3b":"iku","a3t":""},
{"n":"Interlingue; Occidental","a2":"ie","a3b":"ile","a3t":""},{"n":"Iloko","a2":"","a3b":"ilo","a3t":""},
{"n":"Interlingua (International Auxiliary Language Association)","a2":"ia","a3b":"ina","a3t":""},{"n":"Indic languages","a2":"","a3b":"inc","a3t":""},
{"n":"Indonesian","a2":"id","a3b":"ind","a3t":""},{"n":"Indo-European languages","a2":"","a3b":"ine","a3t":""},
{"n":"Ingush","a2":"","a3b":"inh","a3t":""},{"n":"Inupiaq","a2":"ik","a3b":"ipk","a3t":""},
{"n":"Iranian languages","a2":"","a3b":"ira","a3t":""},{"n":"Iroquoian languages","a2":"","a3b":"iro","a3t":""},
{"n":"Italian","a2":"it","a3b":"ita","a3t":""},{"n":"Javanese","a2":"jv","a3b":"jav","a3t":""},
{"n":"Lojban","a2":"","a3b":"jbo","a3t":""},{"n":"Japanese","a2":"ja","a3b":"jpn","a3t":""},
{"n":"Judeo-Persian","a2":"","a3b":"jpr","a3t":""},{"n":"Judeo-Arabic","a2":"","a3b":"jrb","a3t":""},
{"n":"Kara-Kalpak","a2":"","a3b":"kaa","a3t":""},{"n":"Kabyle","a2":"","a3b":"kab","a3t":""},
{"n":"Kachin; Jingpho","a2":"","a3b":"kac","a3t":""},{"n":"Kalaallisut; Greenlandic","a2":"kl","a3b":"kal","a3t":""},
{"n":"Kamba","a2":"","a3b":"kam","a3t":""},{"n":"Kannada","a2":"kn","a3b":"kan","a3t":""},
{"n":"Karen languages","a2":"","a3b":"kar","a3t":""},{"n":"Kashmiri","a2":"ks","a3b":"kas","a3t":""},
{"n":"Kanuri","a2":"kr","a3b":"kau","a3t":""},{"n":"Kawi","a2":"","a3b":"kaw","a3t":""},
{"n":"Kazakh","a2":"kk","a3b":"kaz","a3t":""},{"n":"Kabardian","a2":"","a3b":"kbd","a3t":""},
{"n":"Khasi","a2":"","a3b":"kha","a3t":""},{"n":"Khoisan languages","a2":"","a3b":"khi","a3t":""},
{"n":"Central Khmer","a2":"km","a3b":"khm","a3t":""},{"n":"Khotanese; Sakan","a2":"","a3b":"kho","a3t":""},
{"n":"Kikuyu; Gikuyu","a2":"ki","a3b":"kik","a3t":""},{"n":"Kinyarwanda","a2":"rw","a3b":"kin","a3t":""},
{"n":"Kirghiz; Kyrgyz","a2":"ky","a3b":"kir","a3t":""},{"n":"Kimbundu","a2":"","a3b":"kmb","a3t":""},
{"n":"Konkani","a2":"","a3b":"kok","a3t":""},{"n":"Komi","a2":"kv","a3b":"kom","a3t":""},
{"n":"Kongo","a2":"kg","a3b":"kon","a3t":""},{"n":"Korean","a2":"ko","a3b":"kor","a3t":""},
{"n":"Kosraean","a2":"","a3b":"kos","a3t":""},{"n":"Kpelle","a2":"","a3b":"kpe","a3t":""},
{"n":"Karachay-Balkar","a2":"","a3b":"krc","a3t":""},{"n":"Karelian","a2":"","a3b":"krl","a3t":""},
{"n":"Kru languages","a2":"","a3b":"kro","a3t":""},{"n":"Kurukh","a2":"","a3b":"kru","a3t":""},
{"n":"Kuanyama; Kwanyama","a2":"kj","a3b":"kua","a3t":""},{"n":"Kumyk","a2":"","a3b":"kum","a3t":""},
{"n":"Kurdish","a2":"ku","a3b":"kur","a3t":""},{"n":"Kutenai","a2":"","a3b":"kut","a3t":""},
{"n":"Ladino","a2":"","a3b":"lad","a3t":""},{"n":"Lahnda","a2":"","a3b":"lah","a3t":""},
{"n":"Lamba","a2":"","a3b":"lam","a3t":""},{"n":"Lao","a2":"lo","a3b":"lao","a3t":""},
{"n":"Latin","a2":"la","a3b":"lat","a3t":""},{"n":"Latvian","a2":"lv","a3b":"lav","a3t":""},
{"n":"Lezghian","a2":"","a3b":"lez","a3t":""},{"n":"Limburgan; Limburger; Limburgish","a2":"li","a3b":"lim","a3t":""},
{"n":"Lingala","a2":"ln","a3b":"lin","a3t":""},{"n":"Lithuanian","a2":"lt","a3b":"lit","a3t":""},
{"n":"Mongo","a2":"","a3b":"lol","a3t":""},{"n":"Lozi","a2":"","a3b":"loz","a3t":""},
{"n":"Luxembourgish; Letzeburgesch","a2":"lb","a3b":"ltz","a3t":""},{"n":"Luba-Lulua","a2":"","a3b":"lua","a3t":""},
{"n":"Luba-Katanga","a2":"lu","a3b":"lub","a3t":""},{"n":"Ganda","a2":"lg","a3b":"lug","a3t":""},
{"n":"Luiseno","a2":"","a3b":"lui","a3t":""},{"n":"Lunda","a2":"","a3b":"lun","a3t":""},
{"n":"Luo (Kenya and Tanzania)","a2":"","a3b":"luo","a3t":""},{"n":"Lushai","a2":"","a3b":"lus","a3t":""},
{"n":"Macedonian","a2":"mk","a3b":"mac","a3t":"mkd"},{"n":"Madurese","a2":"","a3b":"mad","a3t":""},
{"n":"Magahi","a2":"","a3b":"mag","a3t":""},{"n":"Marshallese","a2":"mh","a3b":"mah","a3t":""},
{"n":"Maithili","a2":"","a3b":"mai","a3t":""},{"n":"Makasar","a2":"","a3b":"mak","a3t":""},
{"n":"Malayalam","a2":"ml","a3b":"mal","a3t":""},{"n":"Mandingo","a2":"","a3b":"man","a3t":""},
{"n":"Maori","a2":"mi","a3b":"mao","a3t":"mri"},{"n":"Austronesian languages","a2":"","a3b":"map","a3t":""},
{"n":"Marathi","a2":"mr","a3b":"mar","a3t":""},{"n":"Masai","a2":"","a3b":"mas","a3t":""},
{"n":"Malay","a2":"ms","a3b":"may","a3t":"msa"},{"n":"Moksha","a2":"","a3b":"mdf","a3t":""},
{"n":"Mandar","a2":"","a3b":"mdr","a3t":""},{"n":"Mende","a2":"","a3b":"men","a3t":""},
{"n":"Irish, Middle (900-1200)","a2":"","a3b":"mga","a3t":""},{"n":"Mi'kmaq; Micmac","a2":"","a3b":"mic","a3t":""},
{"n":"Minangkabau","a2":"","a3b":"min","a3t":""},{"n":"Uncoded languages","a2":"","a3b":"mis","a3t":""},
{"n":"Mon-Khmer languages","a2":"","a3b":"mkh","a3t":""},{"n":"Malagasy","a2":"mg","a3b":"mlg","a3t":""},
{"n":"Maltese","a2":"mt","a3b":"mlt","a3t":""},{"n":"Manchu","a2":"","a3b":"mnc","a3t":""},
{"n":"Manipuri","a2":"","a3b":"mni","a3t":""},{"n":"Manobo languages","a2":"","a3b":"mno","a3t":""},
{"n":"Mohawk","a2":"","a3b":"moh","a3t":""},{"n":"Mongolian","a2":"mn","a3b":"mon","a3t":""},
{"n":"Mossi","a2":"","a3b":"mos","a3t":""},{"n":"Multiple languages","a2":"","a3b":"mul","a3t":""},
{"n":"Munda languages","a2":"","a3b":"mun","a3t":""},{"n":"Creek","a2":"","a3b":"mus","a3t":""},
{"n":"Mirandese","a2":"","a3b":"mwl","a3t":""},{"n":"Marwari","a2":"","a3b":"mwr","a3t":""},
{"n":"Mayan languages","a2":"","a3b":"myn","a3t":""},{"n":"Erzya","a2":"","a3b":"myv","a3t":""},
{"n":"Nahuatl languages","a2":"","a3b":"nah","a3t":""},{"n":"North American Indian languages","a2":"","a3b":"nai","a3t":""},
{"n":"Neapolitan","a2":"","a3b":"nap","a3t":""},{"n":"Nauru","a2":"na","a3b":"nau","a3t":""},
{"n":"Navajo; Navaho","a2":"nv","a3b":"nav","a3t":""},{"n":"Ndebele, South; South Ndebele","a2":"nr","a3b":"nbl","a3t":""},
{"n":"Ndebele, North; North Ndebele","a2":"nd","a3b":"nde","a3t":""},{"n":"Ndonga","a2":"ng","a3b":"ndo","a3t":""},
{"n":"Low German; Low Saxon; German, Low; Saxon, Low","a2":"","a3b":"nds","a3t":""},{"n":"Nepali","a2":"ne","a3b":"nep","a3t":""},
{"n":"Nepal Bhasa; Newari","a2":"","a3b":"new","a3t":""},{"n":"Nias","a2":"","a3b":"nia","a3t":""},
{"n":"Niger-Kordofanian languages","a2":"","a3b":"nic","a3t":""},{"n":"Niuean","a2":"","a3b":"niu","a3t":""},
{"n":"Norwegian Nynorsk; Nynorsk, Norwegian","a2":"nn","a3b":"nno","a3t":""},{"n":"Bokmål, Norwegian; Norwegian Bokmål","a2":"nb","a3b":"nob","a3t":""},
{"n":"Nogai","a2":"","a3b":"nog","a3t":""},{"n":"Norse, Old","a2":"","a3b":"non","a3t":""},
{"n":"Norwegian","a2":"no","a3b":"nor","a3t":""},{"n":"N'Ko","a2":"","a3b":"nqo","a3t":""},
{"n":"Pedi; Sepedi; Northern Sotho","a2":"","a3b":"nso","a3t":""},{"n":"Nubian languages","a2":"","a3b":"nub","a3t":""},
{"n":"Classical Newari; Old Newari; Classical Nepal Bhasa","a2":"","a3b":"nwc","a3t":""},{"n":"Chichewa; Chewa; Nyanja","a2":"ny","a3b":"nya","a3t":""},
{"n":"Nyamwezi","a2":"","a3b":"nym","a3t":""},{"n":"Nyankole","a2":"","a3b":"nyn","a3t":""},
{"n":"Nyoro","a2":"","a3b":"nyo","a3t":""},{"n":"Nzima","a2":"","a3b":"nzi","a3t":""},
{"n":"Occitan (post 1500)","a2":"oc","a3b":"oci","a3t":""},{"n":"Ojibwa","a2":"oj","a3b":"oji","a3t":""},
{"n":"Oriya","a2":"or","a3b":"ori","a3t":""},{"n":"Oromo","a2":"om","a3b":"orm","a3t":""},
{"n":"Osage","a2":"","a3b":"osa","a3t":""},{"n":"Ossetian; Ossetic","a2":"os","a3b":"oss","a3t":""},
{"n":"Turkish, Ottoman (1500-1928)","a2":"","a3b":"ota","a3t":""},{"n":"Otomian languages","a2":"","a3b":"oto","a3t":""},
{"n":"Papuan languages","a2":"","a3b":"paa","a3t":""},{"n":"Pangasinan","a2":"","a3b":"pag","a3t":""},
{"n":"Pahlavi","a2":"","a3b":"pal","a3t":""},{"n":"Pampanga; Kapampangan","a2":"","a3b":"pam","a3t":""},
{"n":"Panjabi; Punjabi","a2":"pa","a3b":"pan","a3t":""},{"n":"Papiamento","a2":"","a3b":"pap","a3t":""},
{"n":"Palauan","a2":"","a3b":"pau","a3t":""},{"n":"Persian, Old (ca.600-400 B.C.)","a2":"","a3b":"peo","a3t":""},
{"n":"Persian","a2":"fa","a3b":"per","a3t":"fas"},{"n":"Philippine languages","a2":"","a3b":"phi","a3t":""},
{"n":"Phoenician","a2":"","a3b":"phn","a3t":""},{"n":"Pali","a2":"pi","a3b":"pli","a3t":""},
{"n":"Polish","a2":"pl","a3b":"pol","a3t":""},{"n":"Pohnpeian","a2":"","a3b":"pon","a3t":""},
{"n":"Portuguese","a2":"pt","a3b":"por","a3t":""},{"n":"Prakrit languages","a2":"","a3b":"pra","a3t":""},
{"n":"Provençal, Old (to 1500); Occitan, Old (to 1500)","a2":"","a3b":"pro","a3t":""},{"n":"Pushto; Pashto","a2":"ps","a3b":"pus","a3t":""},
{"n":"Reserved for local use","a2":"","a3b":"qaa-qtz","a3t":""},{"n":"Quechua","a2":"qu","a3b":"que","a3t":""},
{"n":"Rajasthani","a2":"","a3b":"raj","a3t":""},{"n":"Rapanui","a2":"","a3b":"rap","a3t":""},
{"n":"Rarotongan; Cook Islands Maori","a2":"","a3b":"rar","a3t":""},{"n":"Romance languages","a2":"","a3b":"roa","a3t":""},
{"n":"Romansh","a2":"rm","a3b":"roh","a3t":""},{"n":"Romany","a2":"","a3b":"rom","a3t":""},
{"n":"Romanian; Moldavian; Moldovan","a2":"ro","a3b":"rum","a3t":"ron"},{"n":"Rundi","a2":"rn","a3b":"run","a3t":""},
{"n":"Aromanian; Arumanian; Macedo-Romanian","a2":"","a3b":"rup","a3t":""},{"n":"Russian","a2":"ru","a3b":"rus","a3t":""},
{"n":"Sandawe","a2":"","a3b":"sad","a3t":""},{"n":"Sango","a2":"sg","a3b":"sag","a3t":""},
{"n":"Yakut","a2":"","a3b":"sah","a3t":""},{"n":"South American Indian languages","a2":"","a3b":"sai","a3t":""},
{"n":"Salishan languages","a2":"","a3b":"sal","a3t":""},{"n":"Samaritan Aramaic","a2":"","a3b":"sam","a3t":""},
{"n":"Sanskrit","a2":"sa","a3b":"san","a3t":""},{"n":"Sasak","a2":"","a3b":"sas","a3t":""},
{"n":"Santali","a2":"","a3b":"sat","a3t":""},{"n":"Sicilian","a2":"","a3b":"scn","a3t":""},
{"n":"Scots","a2":"","a3b":"sco","a3t":""},{"n":"Selkup","a2":"","a3b":"sel","a3t":""},
{"n":"Semitic languages","a2":"","a3b":"sem","a3t":""},{"n":"Irish, Old (to 900)","a2":"","a3b":"sga","a3t":""},
{"n":"Sign Languages","a2":"","a3b":"sgn","a3t":""},{"n":"Shan","a2":"","a3b":"shn","a3t":""},
{"n":"Sidamo","a2":"","a3b":"sid","a3t":""},{"n":"Sinhala; Sinhalese","a2":"si","a3b":"sin","a3t":""},
{"n":"Siouan languages","a2":"","a3b":"sio","a3t":""},{"n":"Sino-Tibetan languages","a2":"","a3b":"sit","a3t":""},
{"n":"Slavic languages","a2":"","a3b":"sla","a3t":""},{"n":"Slovak","a2":"sk","a3b":"slo","a3t":"slk"},
{"n":"Slovenian","a2":"sl","a3b":"slv","a3t":""},{"n":"Southern Sami","a2":"","a3b":"sma","a3t":""},
{"n":"Northern Sami","a2":"se","a3b":"sme","a3t":""},{"n":"Sami languages","a2":"","a3b":"smi","a3t":""},
{"n":"Lule Sami","a2":"","a3b":"smj","a3t":""},{"n":"Inari Sami","a2":"","a3b":"smn","a3t":""},
{"n":"Samoan","a2":"sm","a3b":"smo","a3t":""},{"n":"Skolt Sami","a2":"","a3b":"sms","a3t":""},
{"n":"Shona","a2":"sn","a3b":"sna","a3t":""},{"n":"Sindhi","a2":"sd","a3b":"snd","a3t":""},
{"n":"Soninke","a2":"","a3b":"snk","a3t":""},{"n":"Sogdian","a2":"","a3b":"sog","a3t":""},
{"n":"Somali","a2":"so","a3b":"som","a3t":""},{"n":"Songhai languages","a2":"","a3b":"son","a3t":""},
{"n":"Sotho, Southern","a2":"st","a3b":"sot","a3t":""},{"n":"Spanish; Castilian","a2":"es","a3b":"spa","a3t":""},
{"n":"Sardinian","a2":"sc","a3b":"srd","a3t":""},{"n":"Sranan Tongo","a2":"","a3b":"srn","a3t":""},
{"n":"Serbian","a2":"sr","a3b":"srp","a3t":""},{"n":"Serer","a2":"","a3b":"srr","a3t":""},
{"n":"Nilo-Saharan languages","a2":"","a3b":"ssa","a3t":""},{"n":"Swati","a2":"ss","a3b":"ssw","a3t":""},
{"n":"Sukuma","a2":"","a3b":"suk","a3t":""},{"n":"Sundanese","a2":"su","a3b":"sun","a3t":""},
{"n":"Susu","a2":"","a3b":"sus","a3t":""},{"n":"Sumerian","a2":"","a3b":"sux","a3t":""},
{"n":"Swahili","a2":"sw","a3b":"swa","a3t":""},{"n":"Swedish","a2":"sv","a3b":"swe","a3t":""},
{"n":"Classical Syriac","a2":"","a3b":"syc","a3t":""},{"n":"Syriac","a2":"","a3b":"syr","a3t":""},
{"n":"Tahitian","a2":"ty","a3b":"tah","a3t":""},{"n":"Tai languages","a2":"","a3b":"tai","a3t":""},
{"n":"Tamil","a2":"ta","a3b":"tam","a3t":""},{"n":"Tatar","a2":"tt","a3b":"tat","a3t":""},
{"n":"Telugu","a2":"te","a3b":"tel","a3t":""},{"n":"Timne","a2":"","a3b":"tem","a3t":""},
{"n":"Tereno","a2":"","a3b":"ter","a3t":""},{"n":"Tetum","a2":"","a3b":"tet","a3t":""},
{"n":"Tajik","a2":"tg","a3b":"tgk","a3t":""},{"n":"Tagalog","a2":"tl","a3b":"tgl","a3t":""},
{"n":"Thai","a2":"th","a3b":"tha","a3t":""},{"n":"Tibetan","a2":"bo","a3b":"tib","a3t":"bod"},
{"n":"Tigre","a2":"","a3b":"tig","a3t":""},{"n":"Tigrinya","a2":"ti","a3b":"tir","a3t":""},
{"n":"Tiv","a2":"","a3b":"tiv","a3t":""},{"n":"Tokelau","a2":"","a3b":"tkl","a3t":""},
{"n":"Klingon; tlhIngan-Hol","a2":"","a3b":"tlh","a3t":""},{"n":"Tlingit","a2":"","a3b":"tli","a3t":""},
{"n":"Tamashek","a2":"","a3b":"tmh","a3t":""},{"n":"Tonga (Nyasa)","a2":"","a3b":"tog","a3t":""},
{"n":"Tonga (Tonga Islands)","a2":"to","a3b":"ton","a3t":""},{"n":"Tok Pisin","a2":"","a3b":"tpi","a3t":""},
{"n":"Tsimshian","a2":"","a3b":"tsi","a3t":""},{"n":"Tswana","a2":"tn","a3b":"tsn","a3t":""},
{"n":"Tsonga","a2":"ts","a3b":"tso","a3t":""},{"n":"Turkmen","a2":"tk","a3b":"tuk","a3t":""},
{"n":"Tumbuka","a2":"","a3b":"tum","a3t":""},{"n":"Tupi languages","a2":"","a3b":"tup","a3t":""},
{"n":"Turkish","a2":"tr","a3b":"tur","a3t":""},{"n":"Altaic languages","a2":"","a3b":"tut","a3t":""},
{"n":"Tuvalu","a2":"","a3b":"tvl","a3t":""},{"n":"Twi","a2":"tw","a3b":"twi","a3t":""},
{"n":"Tuvinian","a2":"","a3b":"tyv","a3t":""},{"n":"Udmurt","a2":"","a3b":"udm","a3t":""},
{"n":"Ugaritic","a2":"","a3b":"uga","a3t":""},{"n":"Uighur; Uyghur","a2":"ug","a3b":"uig","a3t":""},
{"n":"Ukrainian","a2":"uk","a3b":"ukr","a3t":""},{"n":"Umbundu","a2":"","a3b":"umb","a3t":""},
{"n":"Undetermined","a2":"","a3b":"und","a3t":""},{"n":"Urdu","a2":"ur","a3b":"urd","a3t":""},
{"n":"Uzbek","a2":"uz","a3b":"uzb","a3t":""},{"n":"Vai","a2":"","a3b":"vai","a3t":""},
{"n":"Venda","a2":"ve","a3b":"ven","a3t":""},{"n":"Vietnamese","a2":"vi","a3b":"vie","a3t":""},
{"n":"Volapük","a2":"vo","a3b":"vol","a3t":""},{"n":"Votic","a2":"","a3b":"vot","a3t":""},
{"n":"Wakashan languages","a2":"","a3b":"wak","a3t":""},{"n":"Wolaitta; Wolaytta","a2":"","a3b":"wal","a3t":""},
{"n":"Waray","a2":"","a3b":"war","a3t":""},{"n":"Washo","a2":"","a3b":"was","a3t":""},
{"n":"Welsh","a2":"cy","a3b":"wel","a3t":"cym"},{"n":"Sorbian languages","a2":"","a3b":"wen","a3t":""},
{"n":"Walloon","a2":"wa","a3b":"wln","a3t":""},{"n":"Wolof","a2":"wo","a3b":"wol","a3t":""},
{"n":"Kalmyk; Oirat","a2":"","a3b":"xal","a3t":""},{"n":"Xhosa","a2":"xh","a3b":"xho","a3t":""},
{"n":"Yao","a2":"","a3b":"yao","a3t":""},{"n":"Yapese","a2":"","a3b":"yap","a3t":""},
{"n":"Yiddish","a2":"yi","a3b":"yid","a3t":""},{"n":"Yoruba","a2":"yo","a3b":"yor","a3t":""},
{"n":"Yupik languages","a2":"","a3b":"ypk","a3t":""},{"n":"Zapotec","a2":"","a3b":"zap","a3t":""},
{"n":"Blissymbols; Blissymbolics; Bliss","a2":"","a3b":"zbl","a3t":""},{"n":"Zenaga","a2":"","a3b":"zen","a3t":""},
{"n":"Standard Moroccan Tamazight","a2":"","a3b":"zgh","a3t":""},{"n":"Zhuang; Chuang","a2":"za","a3b":"zha","a3t":""},
{"n":"Zande languages","a2":"","a3b":"znd","a3t":""},{"n":"Zulu","a2":"zu","a3b":"zul","a3t":""},
{"n":"Zuni","a2":"","a3b":"zun","a3t":""},{"n":"No linguistic content; Not applicable","a2":"","a3b":"zxx","a3t":""},
{"n":"Zaza; Dimili; Dimli; Kirdki; Kirmanjki; Zazaki","a2":"","a3b":"zza","a3t":""}
    ],

  convertLanguageCode: function(from,to,value) {
    if (!value || value == '') return '';

    var output = '';
    for (var i = 0, iLen = translate.languageCodes.length; i < iLen; i++){
      if (translate.languageCodes[i][from] == value)
      {
        output = translate.languageCodes[i][to];
        break;
      }
    }

    return output
  },


  // Search the Alpha3 "B" (bibliographic) and "T" (terminology) and the Alpha 2 values 
  // If these fail, try looking at the landuage Name IF the length of the value is > 3 char
  findLanguage2Code: function(value) {
    if (!value || value == '') return '';

    var output = '';
    for (var i = 0, iLen = translate.languageCodes.length; i < iLen; i++){
      if (translate.languageCodes[i]['a3b'] == value || 
          translate.languageCodes[i]['a3t'] == value)
      {
        output = translate.languageCodes[i]['a2'];
        break;
      }
    }

    if (value.length > 3)
      for (var i = 0, iLen = translate.languageCodes.length; i < iLen; i++){
        if (translate.languageCodes[i]['n'].indexOf(value) > -1)
        {
          output = translate.languageCodes[i]['a2'];
          break;
        }
      }

    return output
  },


  // Search the Alpha3 "B" (bibliographic) or "T" (terminology) as well as 
  // the Alpha 2 values.
  findLanguageName: function(value) {
    if (!value || value == '') return '';

    var output = '';
    for (var i = 0, iLen = translate.languageCodes.length; i < iLen; i++){
      if (translate.languageCodes[i]['a3b'] == value || 
          translate.languageCodes[i]['a3t'] == value ||
          translate.languageCodes[i]['a2'] == value)
      {
        output = translate.languageCodes[i]['n'];
        break;
      }
    }

    return output
  },


}; // End of translate
