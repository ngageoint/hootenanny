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

module.exports = {

    // Build Lookup tables
    createLookup : function(one2one)
    {
        // build a more efficient lookup
        var lookup = {}

        for (var r in one2one)
        {
            var row = one2one[r];
            if (!(row[0] in lookup))
            {
                lookup[row[0]] = {}
            }

            if (!(lookup[row[0]][row[1]]))
            {
                lookup[row[0]][row[1]] = [row[2], row[3]];
            }
            else
            {
                if (config.OgrDebugLookupclash) print('Fwd Clash: ' + row[0] + ' ' + row[1] + '  is ' + lookup[row[0]][row[1]] + '  tried to change to ' + [row[2], row[3]]);
            }
        }

        return lookup;
    },

    /**
     * Given a list of strings create a dictionary of those strings that point to trues.
     */
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
        if (oldValue == undefined)
        {
            return newValue;
        }
        else
        {
            return oldValue + sepValue + newValue;
        }
    },


    // Concatinate two lists
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


    // This is used by anything that "exports" - toNfdd, toMGCP
    createBackwardsLookup : function(one2one)
    {
        // build a more efficient lookup
        var lookup = {}

        for (var r in one2one)
        {
            var row = one2one[r];
            if (row[2]) // Make sure it isn't 'undefined'
            {
                if (!(row[2] in lookup))
                {
                    lookup[row[2]] = {}
                }

                // This has been swapped around. The FCODE lists can stomp on the One2One values.
                if (!(lookup[row[2]][row[3]]))
                {
                    lookup[row[2]][row[3]] = [row[0], row[1]];
                }
                else
                {
                    if (config.OgrDebugLookupclash) print('Bkwd Clash: ' + row[2] + ' ' + row[3] + '  is ' + lookup[row[2]][row[3]] + '  tried to change to ' + [row[0], row[1]]);
                }
            }
        }

        return lookup;
    },


    // Apply one to one translations - used for import and export
    applyOne2One : function(inList, outList, lookup, fCodeList)
    {
        var endChar = '',
            tAttrib = '',
            row = [];

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
                        // Debug
                        // print('Used:' + col + ' = ' + inList[col] + ' - ' + row[0] + ' = ' + row[1]);
                        delete inList[col];
                    }
                    else
                    {
                        // Debug
                        // print('UsedUndef:' + col + ' = ' + inList[col]);
                        delete inList[col];
                    }
                }
                // there may be situations where this error is inappropriate. Though we haven't run
                // into them yet.
                // Oh, yes we have. -Matt
                else if (value !== '')
                {
                    // If these tags are used to find an FCODE, ignore them
                    if ((col in fCodeList) && (value in fCodeList[col]))
                    {
                        // Debug
                        // print('UsedFCode:' + col + ' = ' + inList[col]);
                        delete inList[col];
                        continue;
                    }

                    debug('Lookup value not found for column:: (' + col + '=' + value + ')');
                }
            } // End col in lookup
            else
            {
                // If these tags are used to find an FCODE, ignore them
                if ((col in fCodeList) && (value in fCodeList[col]))
                {
                    // Debug
                    // print('UsedFCode:' + col + ' = ' + inList[col]);
                    delete inList[col];
                    continue;
                }

                // Debug:
                // print('Col::  :' + col + ':  Value :' + value + ':');

                endChar = col.charAt(col.length - 1) // Going to look for a 2 or a 3
                tAttrib = col.slice(0,-1);

                if (tAttrib in lookup)
                {
                    if (value in lookup[tAttrib])
                    {
                        row = lookup[tAttrib][value];

                        // Drop all of the undefined values
                        if (row[0])
                        {
                            outList[row[0] + endChar] = row[1];
                            delete inList[col];
                            // Debug:
                            // print ('one2one: Setting :' + row[0] + endChar + ': to :' + row[1] + ':');
                        }
                    }
                }
                else
                {
                    if (config.OgrDebugLookupcolumn) debug('Column not found:: (' + col + '=' + value + ')');
                }
            } // End !col in lookup
        } // End for col in inList
    }, // End applyOne2One


    // Apply one to one translations and don't report errors: missing columns etc
    applyOne2OneQuiet : function(inList, outList, lookup)
    {
        var endChar = '',
            tAttrib = '',
            row = [];

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
                        // Debug
                        // print('Fuzzy: ' + col);
                        delete inList[col];
                    }
                }
            }
        } // End for col in inList
    }, // End applyOne2OneQuiet


    // Apply one to one translations - For NFDD export
    // This version populates the OTH field for values that are not in the rules
    applyNfddOne2One : function(inList, outList, lookup, fCodeList)
    {
        var endChar = '',
            tAttrib = '',
            row = [],
            otherVal = '',
            othVal = '',
            value = '';

        for (var col in inList)
        {
            var value = inList[col];

            endChar = col.charAt(col.length - 1) // Going to look for a 2 or a 3

            if (endChar == '2' || endChar == '3')
            {
                tAttrib = col.slice(0,-1);
            }
            else
            {
                tAttrib = col;
                endChar = '';
            }

            if (tAttrib in lookup)
            {
                if (value in lookup[tAttrib])
                {
                    row = lookup[tAttrib][value];

                    // Debug:
                    // print('row[0]=' + row[0] + '  row[0]+endChar=' + row[0] + endChar);

                    // Drop all of the undefined values
                    if (row[0])
                    {
                        outList[row[0] + endChar] = row[1];
                        // Debug
                        // print('Used:' + col + ' = ' + inList[col]);
                        delete inList[col];
                    }
                    else
                    {
                        // Debug
                        // print('UsedUndef:' + col + ' = ' + inList[col]);
                        delete inList[col];
                    }

                }
                // there may be situations where this error is inappropriate. Though we haven't run
                // into them yet.
                // Oh, yes we have. -Matt
                else if (value !== '')
                {
                    // If these tags are used to find an FCODE, ignore them
                    if ((tAttrib in fCodeList) && (value in fCodeList[tAttrib]))
                    {
                        // Debug
                        // print('UsedFCode:' + col + ' = ' + inList[col]);
                        delete inList[col];
                        continue;
                    }

                    debug('Lookup value not found for column:: (' + tAttrib + '=' + value + ')');

                    // The following is used for export. If we have an attribute value that can't
                    // find a rule for, we add it to the OTH Field.
                    otherVal = lookup[tAttrib]['other'];

                    if (otherVal)
                    {
                        // Build the OTH value
                        othVal = '(' + otherVal[0] + endChar + ':' + value + ')';
                        outList.OTH = translate.appendValue(outList.OTH,othVal,' ');

                        debug('Adding to OTH field:: ' + othVal);

                        // Set the output attribute to "other"
                        outList[otherVal[0] + endChar] = otherVal[1];

                        // Debug
                        // print('UsedOTH:' + col + ' = ' + inList[col]);
                        delete inList[col];

                    } // End if otherVal
                    else
                    {
                        debug('Could not add ::' + tAttrib + '=' + value + ':: to the OTH field');
                    }
                } // End value != ''
            } // End tAttrib in lookup
            else
            {
                // If we didn't find the tag, check if it is used to find an FCODE. If so, mark it as used
                if ((tAttrib in fCodeList) && (value in fCodeList[tAttrib]))
                {
                    // Debug
                    // print('UsedX:' + col + ' = ' + inList[col]);
                    delete inList[col];
                }
                else
                {
                    if (config.OgrDebugLookupcolumn) debug('Column not found:: (' + col + '=' + value + ')');
                }
            } // End !col in lookup
        } // End for col in inList
    }, // End applyNfddOne2One


    // Parse the note:extra tag and return an associative array of key/value pairs
    parseNoteExtra : function(rawNote)
    {
        var noteVal = [],
            outList = {},
            tmpList = rawNote.split(';');

        for (var i = 0, len = tmpList.length; i < len; i++)
        {
            noteVal = tmpList[i].split(':'); // Split into Key/Value

            // If we have a Key then add it to the output
            if (noteVal[0]) outList[noteVal[0]] = noteVal[1];
        }

        return outList;
    },


    // Parse the OTH value and return an associative array of key/value pairs
    // According to the spec, the format of the OTH field is:
    //      (<Attr>:<Value>) (<Attr>:<Value>.....)
    parseOTH : function(rawOTH)
    {
        var othVal = [],
            outList = {},
            tVal = '',
            tVal = rawOTH.replace(/\) \(/g,'#'); // Swap ') (' for #)

            tVal = tVal.replace(/[\(\)]/g,''); // Get rid of ( and )

            tmpList = tVal.split('#'); // Split on #

        for (var i = 0, len = tmpList.length; i < len; i++)
        {
            othVal = tmpList[i].split(':'); // Split into Key/Value

            // If we have a Key then add it to the output
            if (othVal[0]) outList[othVal[0]] = othVal[1];
        }

        // Debug:
        // for (var j in outList) print('parseOTH: k=  :' + j + ':  v= :' + outList[j] + ':');

        return outList;
    },


    packOTH : function(rawList)
    {
        var othVal = '',
            othField = '';

        for (var i in rawList)
        {
            othVal = '(' + i + ':' + rawList[i] + ')';
            othField = translate.appendValue(othField, othVal, ' ');
        }

        return othField;
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
            // print('i: ' + i + '  atr= :' + attrs[i] + ':');

            if (attrs[i])
            {
                endChar = i.charAt(i.length - 1) // Going to look for a 2 or a 3
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
                debug('OTH:: Attribute :' + i + ': is supposed to be 999/Other. It is not set. Skipping it.');
                continue
            } // End !attrsi[]


            // Debug:
            // print('OTH::  tAttrib:' + tAttrib + ':  tValue:' + tValue + ':  endChar:' + endChar + ':');

            if (tValue !== '999')
            {
                debug('OTH:: Attribute :' + i + ': is supposed to be 999/Other. It is :' + tValue + ':. Skipping it.');
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
                    logError('OTH:: Did not find an "Other" value for ' + i);
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
                    tags[key[0] + endChar] = othList[i];
                    // Debug:
                    // print('OTH:: adding :' + othList[i] + ': to tag :' + key[0] + endChar + ': (From ' + i + ')');
                    delete othList[i];
                }
                else
                {
                    logError('OTH:: Did not find an "Other" value for ' + i);
                }
            }
            else // !tAttrib in lookup
            {
                logError('OTH:: ' + i + ' does not exist in lookup');
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
                console.warn('Missing OSM end tag in: ' + rawMemo);
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
            n = attrs[col].replace("\\", "\\\\");
            n = attrs[col].replace(";", "\\;");
            names.push(n)
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



    // applySimpleTxtBiased - Apply 0ne2one rules for Text Attributes
    // The "direction is either "forward" or "backward" - convert to or from
    applySimpleTxtBiased : function(attrs, tags, rules, direction)
    {
        if (direction == 'forward')
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
        }
        else
        {
            // convert Tags to Attrs
            for (var i in rules)
            {
                if (rules[i] in tags)
                {
                    attrs[i] = tags[rules[i]];
                    // Debug
                    // print('UsedTxt: ' + rules[i]);
                    delete tags[rules[i]];
                }
            }
        }
    }, // End applySimpleTxtBiased

    // applySimpleNumBiased - Apply 0ne2one rules for Number Attributes
    // The "direction is either "forward" or "backward" - convert to or from
    applySimpleNumBiased : function(attrs, tags, rules, direction, intList)
    {
        if (direction == 'forward')
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
                        debug('Expected a number for:: ' + i + '. Got ' + attrs[i] + ' instead. Skipping ' + i);
                    }
                }
            }
        }
        else
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
                                    debug('Converting ' + i + ' from ' + tNum + ' to ' + tInt);
                                }
                                tNum = tInt;
                        } // End in intList

                        attrs[i] = tNum;
                        // Debug
                        // print ('UsedNum: ' + rules[i]);
                        delete tags[rules[i]];
                    }
                    else
                    {
                        debug('Expected a number for:: ' + rules[i] + '. Got ' + tags[rules[i]] + ' instead. Skipping ' + i);
                    }
                }
            }
        }
    }, // End applySimpleTxtBiased

    // buildComplexRules
    // Build the lookup table for the applyComplexRules function
    buildComplexRules : function (rawList)
    {
        var rulesFunction = [];
        for (var i = 0, rLen = rawList.length; i < rLen; i++)
        {
            rulesFunction.push([new Function('t', 'return ' + rawList[i][0]), new Function('t','a', rawList[i][1])]);
            // print('Rules: Added ' + rawList[i][0]);
        }

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
        function applyRules(t,a)
        {
            for (var i = 0, rLen = rulesList.length; i < rLen; i++)
            {
                if (rulesList[i][0](t)) rulesList[i][1](t,a);
            }
        }

        applyRules(tgs,atrs);

    }, // End applyComplexRules


    // makeAttrLookup - build a lookup table for FCODEs and Attrs
    makeTdsAttrLookup : function(schema)
    {
        var lookup = {};

        for (var i=0, sLen = schema.length; i < sLen; i++)
        {
            var attrArray = [];
            for (var j=0, cLen = schema[i].columns.length; j < cLen; j++)
            {
                attrArray.push(schema[i].columns[j].name);
            }
            // Add the attrArray to the list as <tablename>:[array]
            lookup[schema[i].name] = attrArray;
        }

        return lookup;
    },

    // makeAttrLookup - build a lookup table for FCODEs and Attrs
    makeAttrLookup : function(schema)
    {
        var lookup = {};

        for (var i=0, sLen = schema.length; i < sLen; i++)
        {
            var attrArray = [];
            for (var j=0, cLen = schema[i].columns.length; j < cLen; j++)
            {
                attrArray.push(schema[i].columns[j].name);
            }
            // Add the attrArray to the list as <geom><FCODE>:[array]
            // Eg[L,A,P]AP030:[array]
            lookup[schema[i].geom.charAt(0) + schema[i].fcode] = attrArray;
        }

        return lookup;
    },

    // makeLayerNameLookup - build a lookup table for FCODE to LayerName
    makeLayerNameLookup : function(schema)
    {
        var lookup = {};

        for (var i=0, sLen = schema.length; i < sLen; i++)
        {
            lookup[schema[i].geom.charAt(0) + schema[i].fcode] = schema[i].name;
        }

        return lookup;
    },

    // addReviewFeature - Add Review features to a schema
    addReviewFeature: function(schema)
    {
        schema.push({ name: "review_A",
                      desc: "Review features",
                      geom: "Area",
                      columns:[ { name:'score',
                                  desc:'Review Score',
                                  type:'String',
                                  // length:'254'
                                },
                                { name:'note',
                                  desc:'Review note',
                                  type:'String',
                                  // length:'254'
                                },
                                { name:'source',
                                  desc:'Review source',
                                  type:'String',
                                  // length:'254'
                                },
                                { name:'uuid',
                                  desc:'Review uuid',
                                  type:'String',
                                  defValue: '',
                                  // length:'254'
                                }
                              ]
                    });
        schema.push({ name: "review_L",
                      desc: "Review features",
                      geom: "Line",
                      columns:[ { name:'score',
                                  desc:'Review Score',
                                  type:'String',
                                  // length:'254'
                                },
                                { name:'note',
                                  desc:'Review note',
                                  type:'String',
                                  // length:'254'
                                },
                                { name:'source',
                                  desc:'Review source',
                                  type:'String',
                                  // length:'254'
                                },
                                { name:'uuid',
                                  desc:'Review uuid',
                                  type:'String',
                                  defValue: '',
                                  // length:'254'
                                }
                              ]
                    });
        schema.push({ name: "review_P",
                      desc: "Review Features",
                      geom: "Point",
                      columns:[ { name:'score',
                                  desc:'Review Score',
                                  type:'String',
                                  // length:'254'
                                },
                                { name:'note',
                                  desc:'Review note',
                                  type:'String',
                                  // length:'254'
                                },
                                { name:'source',
                                  desc:'Review source',
                                  type:'String',
                                  // length:'254'
                                },
                                { name:'uuid',
                                  desc:'Review uuid',
                                  type:'String',
                                  defValue: '',
                                  // length:'254'
                                }
                              ]
                    });

        return schema;

    }, // End addReviewFeature

    // addEmptyFeature - Add o2s features to a schema
    addEmptyFeature: function(schema)
    {
        schema.push({ name: "o2s_A",
                      desc: "o2s",
                      geom: "Area",
                      columns:[ { name:'tag1',
                                  desc:'Tag List',
                                  type:'String',
                                  // length:'254'
                                },
                                { name:'tag2',
                                  desc:'Tag List',
                                  type:'String',
                                  defValue: '',
                                  // length:'254'
                                },
                                { name:'tag3',
                                  desc:'Tag List',
                                  type:'String',
                                  defValue: '',
                                  // length:'254'
                                },
                                { name:'tag4',
                                  desc:'Tag List',
                                  type:'String',
                                  defValue: '',
                                  // length:'254'
                                }
                              ]
                    });
        schema.push({ name: "o2s_L",
                      desc: "o2s",
                      geom: "Line",
                      columns:[ { name:'tag1',
                                  desc:'Tag List',
                                  type:'String',
                                  // length:'254'
                                },
                                { name:'tag2',
                                  desc:'Tag List',
                                  type:'String',
                                  defValue: '',
                                  // length:'254'
                                },
                                { name:'tag3',
                                  desc:'Tag List',
                                  type:'String',
                                  defValue: '',
                                  // length:'254'
                                },
                                { name:'tag4',
                                  desc:'Tag List',
                                  type:'String',
                                  defValue: '',
                                  // length:'254'
                                }
                              ]
                    });
        schema.push({ name: "o2s_P",
                      desc: "o2s",
                      geom: "Point",
                      columns:[ { name:'tag1',
                                  desc:'Tag List',
                                  type:'String',
                                  // length:'254'
                                },
                                { name:'tag2',
                                  desc:'Tag List',
                                  type:'String',
                                  defValue: '',
                                  // length:'254'
                                },
                                { name:'tag3',
                                  desc:'Tag List',
                                  type:'String',
                                  defValue: '',
                                  // length:'254'
                                },
                                { name:'tag4',
                                  desc:'Tag List',
                                  type:'String',
                                  defValue: '',
                                  // length:'254'
                                }
                              ]
                    });

        return schema;

    }, // End addEmptyFeature

    // addExtraFeature - Add features to hold "extra" tag values to a schema
    addExtraFeature: function(schema)
    {
        schema.push({ name: "extra_A",
                      desc: "extra tag values",
                      geom: "Area",
                      columns:[ { name:'tags',
                                  desc:'Tag List',
                                  type:'String',
                                },
                                { name:'uuid',
                                  desc:'Feature uuid',
                                  type:'String',
                                  defValue: '',
                                }
                              ]
                    });
        schema.push({ name: "extra_L",
                      desc: "extra tag values",
                      geom: "Line",
                      columns:[ { name:'tags',
                                  desc:'Tag List',
                                  type:'String',
                                  // length:'254'
                                },
                                { name:'uuid',
                                  desc:'Feature uuid',
                                  type:'String',
                                  defValue: '',
                                }
                              ]
                    });
        schema.push({ name: "extra_P",
                      desc: "extra tag values",
                      geom: "Point",
                      columns:[ { name:'tags',
                                  desc:'Tag List',
                                  type:'String',
                                  // length:'254'
                                },
                                { name:'uuid',
                                  desc:'Feature uuid',
                                  type:'String',
                                  defValue: '',
                                }
                              ]
                    });

        return schema;

    }, // End addExtraFeature

    // addEtds - Add the eLTDS specific fields to each element in the schema
    addEtds: function(schema)
    {
        for (var i = 0, schemaLen = schema.length; i < schemaLen; i++)
        {
            schema[i].columns.push( { name:'SCAMIN',
                                      desc:'Scale - Minimum',
                                      type:'Integer',
                                      optional:'R',
                                      defValue:'-999999'
                                    });
            schema[i].columns.push( { name:'SCAMAX',
                                      desc:'Scale - Maximum',
                                      type:'Integer',
                                      optional:'R',
                                      defValue:'-999999'
                                    });
            schema[i].columns.push( { name:'LINK_ID',
                                      desc:'Link Id',
                                      type:'String',
                                      optional:'R',
                                      defValue:'No Information'
                                    });
        }

        return schema;

    }, // End addEtds


    // addFCSubtype - Add the ESRI specific FCSUBTYPE field to each element in the schema
    addFCSubtype: function(schema)
    {
        for (var i = 0, schemaLen = schema.length; i < schemaLen; i++)
        {
            schema[i].columns.push( { name:'FCSUBTYPE',
                                      desc:'Feature Code Subtype',
                                      type:'Integer',
                                      optional:'R',
                                      defValue:''
                                    });
        }

        return schema;

    }, // End addFCSubtype


    // addFdName - Add the ESRI Feature Dataset name to every element in the schema
    // If we don't add this the layers get put in the wrong place in the FGDB
    addFdName: function(schema,name)
    {
        for (var i = 0, schemaLen = schema.length; i < schemaLen; i++)
        {
            schema[i].fdname = name;
        }

        return schema;

    }, // End addFdName


    // dumpLookup - Dump a Lookup table so we can check it
    dumpLookup : function(lookupTable)
    {
        for (var i in lookupTable)
        {
            print("Key: " + i + "  Value: " + lookupTable[i]);
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
        for (var i = 0, schemaLen = schema.length; i < schemaLen; i++)
        {
            print('Feature: ' + schema[i].name + '  Geom: ' + schema[i].geom + '  FdName: ' + schema[i].fdname);
            for (var j = 0, columnLen = schema[i].columns.length; j < columnLen; j++)
            {
                print('    Attr: ' + schema[i].columns[j].name + '  Desc: ' + schema[i].columns[j].desc + '  Type: ' + schema[i].columns[j].type + '  Default: ' + schema[i].columns[j].defValue);
                if (schema[i].columns[j].type == 'enumeration')
                {
                    for (var k = 0, enumLen = schema[i].columns[j].enumerations.length; k < enumLen; k++)
                        print('        Value: ' + schema[i].columns[j].enumerations[k].value + '  Name: ' + schema[i].columns[j].enumerations[k].name);
                        // print('        Name: ' + schema[i].columns[j].enumerations[k].name + '  Value: ' + schema[i].columns[j].enumerations[k].value);
                }
            } // End for j

            print(''); // just to get one blank line
        } // End for i
    } // End dumpSchema

} // End of translate
