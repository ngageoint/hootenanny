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
    MUVD conversion script for TRD1
        MUVD -> OSM, and
        OSM -> MUVD
*/

muvd = {
    getDbSchema: function() {
        muvd.rawSchema = muvd.schema.getDbSchema(); // This is <GLOBAL> so we can access it from other areas

        // Build the FCODE to attr lookup table. Note: This is <GLOBAL>
        muvd.attrLookup = {};
        muvd.attrLookup = translate.makeAttrLookup(muvd.rawSchema);

        // Decide if we are going to use the Thematic structure or 1 FCODE / Layer
        // if we don't want the thematic structure, just return the muvd.rawSchema
        if (hoot.Settings.get('writer.thematic.structure') == 'false')
        {
            // Now add an o2s[A,L,P] feature to the muvd.rawSchema and an attribute to hold OSM tags
            if (hoot.Settings.get('ogr.output.format') == 'shp')
            {
                // Add OSMTAGS1-4 and tag1-4 attributes
                muvd.rawSchema = translate.addO2sFeatures(translate.addTagFeatures(muvd.rawSchema));
            }
            else
            {
                // Just add tag1 && OSMTAGS
                muvd.rawSchema = translate.addSingleO2sFeature(translate.addSingleTagFeature(muvd.rawSchema));
            }

            // Add empty Review layers
            muvd.rawSchema = translate.addReviewFeature(muvd.rawSchema);

            // Add empty "extra" feature layers if needed
            if (hoot.Settings.get('ogr.note.extra') == 'file') muvd.rawSchema = translate.addExtraFeature(muvd.rawSchema);

            return muvd.rawSchema;
        } // End non-thematic

        // Thematic schema
        if (muvd.thematicSchema == undefined)
        {
            if (hoot.Settings.get('ogr.coded.values') == 'false')
            {
                hoot.require('muvd_thematic_enum_schema');
                muvd.thematicSchema = muvd.thematicSchema.getDbSchema();
            }
            else
            {
                hoot.require('muvd_thematic_schema');
            }
        }

        if (hoot.Settings.get('ogr.output.format') == 'shp')
        {
            muvd.thematicLookup = translate.makeThematicAttrLookup(translate.addTagFeatures(muvd.thematicSchema));
            muvd.thematicSchema = translate.addO2sFeatures(muvd.thematicSchema);
        }
        else
        {
            muvd.thematicLookup = translate.makeThematicAttrLookup(translate.addSingleTagFeature(muvd.thematicSchema));
            muvd.thematicSchema = translate.addSingleO2sFeature(muvd.thematicSchema);
        }

        // Add empty Review layers
        muvd.thematicSchema = translate.addReviewFeature(muvd.thematicSchema);

        // Add empty "extra" feature layers if needed
        if (hoot.Settings.get('ogr.note.extra') == 'file') muvd.thematicSchema = translate.addExtraFeature(muvd.thematicSchema);

        return muvd.thematicSchema;
    }, // End of getDbSchema


    // validateAttrs: Clean up the supplied attr list by dropping anything that should not be part of the feature
    validateAttrs: function(geometryType,attrs,notUsed,transMap) {

    }, // end validateAttrs

    // validateThematicAttrs - Clean up the thematic layer attrs. This sets all of the extra attrs to be "undefined"
    validateThematicAttrs: function(gFcode, attrs) {

    }, // End validateThematicAttrs

    // sort out if we need to return more than one feature
    // this is generally for roads, railways, bridges, tunnels, etc.
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

    }, // End manyFeatures

    // Function to drop default and useless values
    // NOTE: this is also called to remove translated tag values
    dropDefaults: function (feat)
    {
        for (var col in feat)
        {
            // First push to lowercase
            var attrValue = feat[col].toString().toLowerCase();

            // get rid of the spaces in the text
            attrValue = attrValue.replace(/\s/g, '');

            // Wipe out the useless values
            if (feat[col] == '' || feat[col] == ' ' || attrValue in muvd.rules.dropList || feat[col] in muvd.rules.dropList)
            {
                // debug
                // print('Dropping: ) + col + ' = ' + feat[col];
                delete feat[col];
                continue;
            }
        } // end col in attrs loop
    }, // End dropDefaults

    // Cleanup the attributes
    cleanAttrs: function (attrs)
    {
        // Switch to keep all of the default values. Mainly for the schema switcher
        if (muvd.configIn.ReaderDropDefaults == 'true')
        {
            muvd.dropDefaults(attrs);
        }

        // unit conversion
        for (var col in attrs)
        {
            // see if we need to swap attr names
            if (col in muvd.rules.swapListIn)
            {
                // debug
                // print('Swapped: ) + swapList[i]);
                attrs[muvd.rules.swapListIn[col]] = attrs[col];
                delete attrs[col];
                continue;
            }
        } // End col in attrs loop
    },

    // ##### Start of the toOsm Block
    applyToOsmPreProcessing: function(attrs, layerName, geometryType)
    {
        if (attrs.F_CODE)
        {
            // Drop the "Not Found" F_CODE. This is from the UI
            // NOTE: We should be getting "FCODE" not "F_CODE" from files/UI
            if (attrs.F_CODE == 'Not found') delete attrs.F_CODE;
        }
        else if (attrs.FCODE)
        {
            // Swap these since the rest of the lookup tables use F_CODE
            if (attrs.FCODE != 'Not found') attrs.F_CODE = attrs.FCODE;

            delete attrs.FCODE;
        }
        else
        {
            // TODO: some pre processing after finding an appropriate FCODE
        }
    }, // end of applyToOsmPreProcessing

    // Post processing: lots of cleanup
    applyToOsmPostProcessing : function (attrs, tags, layerName, geometryType)
    {
        // Unpack the TXT field or OSMTAGSX if needed
        if (tags.note || attrs.OSMTAGS)
        {
            var tTags = {};
            var tObj = translate.unpackMemo(tags.note);

            if (tObj.tags != '')
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
                if (tags[tTags[i]]) hoot.logDebug('Unpacking tags, overwriting ' + i + ' = ' + tags[i] + ' with ' + tTags[i]);
                tags[i] = tTags[i];

                // Now check if this is a synonym etc. If so, remove the other tag.
                if (i in muvd.fcodeLookupOut) // tag -> FCODE table
                {
                    if (tags[i] in muvd.fcodeLookupOut[i])
                    {
                        var row = muvd.fcodeLookupOut[i][tags[i]];

                        // Now find the "real" tag that comes from the FCode
                        if (row[1] in muvd.fcodeLookup['F_CODE'])
                        {
                            var row2 = muvd.fcodeLookup['F_CODE'][row[1]];
                            // If the tags match, delete it
                            if (tags[row2[0]] && (tags[row2[0]] == row2[1]))
                            {
                                delete tags[row2[0]];
                            }
                        }
                    }
                }
            } // End nTags

            if (tObj.text != '')
            {
                tags.note = tObj.text;
            }
            else
            {
                delete tags.note;
            }
        } // end process tags.note

        // Add the LayerName to the source
        if ((!tags.source) && layerName != '') tags.source = 'muvd:' + layerName.toLowerCase();

        // If we have a UUID, store it
        if (tags.uuid)
        {
            tags.uuid = tags['uuid'].toString().toLowerCase();
            if (tags['uuid'].indexOf('{') == -1) tags.uuid = '{' + tags['uuid'] + '}';
        }
        else
        {
            if (muvd.configIn.OgrAddUuid == 'true') tags.uuid = createUuid();
        }

        // TODO: there will be a lot more post processing here that may be more MUVD specific

        if (muvd.osmPostRules == undefined)
        {
            // TODO: create post rules here

            // Rules format: ["test expression","output result"];
            // NOTE: t = tags, a = attrs and attrs can only be on the RHS


            // uncomment this line below whenever the rulesList is created
            // muvd.osmPostRules = translate.buildComplexRules(rulesList);
        }

        // applying the post rules here
        // for (var i = 0, rLen = muvd.osmPostRules.length; i< rLen; i++)
        // {
        //   if (muvd.osmPostRules[i][0](tags)) muvd.osmPostRules[i][1](tags,attrs);
        // }
    }, // end of applyToOsmPostProcessing

    // Start of the toOgr Block
    applyToOgrPreProcessing: function(tags, attrs, geometryType)
    {
        // Remove hoot assigned tags for the source of the data
        delete tags['source:ingest:datetime'];
        delete tags['error:circular'];
        delete tags['hoot:status'];

        // Unpack other tags if required
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
        } // End cleanup loop

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

        // Keep looking for an FCODE
        // This uses the fcodeLookup tables that are defined earlier
        if (!attrs.F_CODE)
        {
            for (var col in tags)
            {
                var value = tags[col];
                if (col in muvd.fcodeLookup && (value in muvd.fcodeLookup[col]))
                {
                    var row = muvd.fcodeLookup[col][value];
                    attrs.F_CODE = row[1];
                }
                else if (col in muvd.fcodeLookupOut && (value in muvd.fcodeLookupOut[col]))
                {
                    var row = muvd.fcodeLookupOut[col][value];
                    attrs.F_CODE = row[1];
                }
            }
        }

    }, // End applyToOgrPreProcessing

    applyToOgrPostProcessing: function(tags, attrs, geometryType, notUsedTags)
    {

    }, // End applyToOgrPostProcessing

    // toOsm - Translate Attrs to Tags
    toOsm : function(attrs, layerName, geometryType)
    {
        tags = {}; // This is the output

        // Setup config variables. We could do this in initialize() but some things don't call it
        // Doing this so we don't have to keep calling into Hoot core
        if (muvd.configIn == undefined)
        {
            muvd.configIn = {};
            muvd.configIn.OgrAddUuid = hoot.Settings.get('ogr.add.uuid');
            muvd.configIn.OgrDebugAddfcode = hoot.Settings.get('ogr.debug.addfcode');
            muvd.configIn.OgrDebugDumptags = hoot.Settings.get('ogr.debug.dumptags');

            // Get any changes
            muvd.toChange = hoot.Settings.get("schema.translation.override");
        }

        // Moved this so it gets checked for each call
        muvd.configIn.ReaderDropDefaults = hoot.Settings.get('reader.drop.defaults');

        // Debug:
        if (muvd.configIn.OgrDebugDumptags == 'true') translate.debugOutput(attrs,layerName,geometryType,'','In Attrs: ');

        // See if we have an o2s_X layer and try to unpack it
        if (layerName.indexOf('o2s_') > -1)
        {
            tags = translate.unpackText(attrs,'tag');

            // Throw out the reason for the o2s if it exists
            delete tags.o2s_reason;

            // Add some metadata
            if (!tags.uuid)
            {
                // Upper case as a test
                if (muvd.configIn.OgrAddUuid == 'true') tags.uuid = createUuid().toUpperCase();
            }

            if (!tags.source) tags.source = 'muvd:' + layerName.toLowerCase();

            // Debug:
            if (muvd.configIn.OgrDebugDumptags == 'true')
            {
                translate.debugOutput(tags,layerName,geometryType,'','Out tags: ');
            }

            return tags;
        } // End layername = o2s_X

        // Set up the fcode translation rules
        if (muvd.fcodeLookup == undefined)
        {
            fcodeCommon.one2one.forEach( function(item) { if (muvd.rules.fcodeNameLookup[item[1]]) muvd.rules.fcodeOne2oneOut.push(item); });

            muvd.fcodeLookup = translate.createLookup(muvd.rules.fcodeOne2oneOut);

            muvd.fcodeLookupOut = translate.createBackwardsLookup(muvd.rules.fcodeOne2oneOut);

            // Debug:
            // translate.dumpOne2OneLookup(muvd.fcodeLookupOut);
        }

        if (muvd.lookup == undefined)
        {

            // TODO: whenever the MUVD specific attributes are implemented, come back
            // here and add them to the attribute table

            // Setup lookup tables to make translation easier
            muvd.lookup = translate.createLookup(muvd.rules.one2one);

        }

        if (muvd.rules.txtBiased == undefined)
        {
            // TODO: If there's more than one version of MUVD, then this will come into play

            //muvd.rules.txtBiased = {};
            // Add the MUVD specific attributes to the attribute table
            //for (var i in muvd.rules.txtBasedV2) muvd.rules.txtBiased[i] = muvd.rules.txtBasedV2[i];
        }

        if (muvd.rules.numBiased == undefined)
        {
            // TODO: if there's more than one version of MUVD, then this will come into play

            //muvd.rules.numBiased = {};
            //for (var i in muvd.rules.numBasedV2) muvd.rules.numBiased[i] = muvd.rules.numBasedV2[i];
        }

        // Clean out the useless attrs
        muvd.cleanAttrs(attrs);

        // Untangle MUVD attributes & OSM tags
        // NOTE: this could get wrapped with an ENV variable so it only get called during import
        translate.untangleAttributes(attrs,tags,muvd);

        // Debug:
        if (muvd.configIn.OgrDebugDumptags == 'true')
        {
            translate.debugOutput(attrs,layerName,geometryType,'','Untangle attrs: ');
            translate.debugOutput(tags,layerName,geometryType,'','Untangle tags: ');
        }

        // pre processing
        muvd.applyToOsmPreProcessing(attrs, layerName, geometryType);

        // Use the FCODE to add some tags
        if (attrs.F_CODE)
        {
            var ftag = muvd.fcodeLookup['F_CODE'][attrs.F_CODE];
            if (ftag)
            {
                if (!tags[ftag[0]])
                {
                    tags[ftag[0]] = ftag[1];
                }
                else
                {
                    // Debug
                    hoot.logWarn('Tried to replace: ' + ftag[0] + '=' + tags[ftag[0]] + ' with ' + ftag[1]);
                }
                // Debug: dump out the tags from the FCODE
                // print('FCODE: ' + attrs.F_CODE + ' tag=' + ftag[0] + ' value=' + ftag[1]);
            }
            else
            {
                hoot.logTrace('Translation for FCODE ' + attrs.F_CODE + ' not found');
            }
        }

        // Make a copy of the input attributes so we can remove them as they get translated. Looking at what
        // isn't used in the translation - this should end up empty
        var notUsedAttrs = (JSON.parse(JSON.stringify(attrs)));
        delete notUsedAttrs.F_CODE;

        // apply the simple number and text based rules
        translate.numToOSM(notUsedAttrs, tags, muvd.rules.numBiased);
        translate.txtToOSM(notUsedAttrs, tags, muvd.rules.txtBiased);

        // one 2 one
        translate.applyOne2One(notUsedAttrs, tags, muvd.lookup, {'k':'v'},[]);

        // post processing
        muvd.applyToOsmPostProcessing(attrs, tags, layerName, geometryType);

        // If we are reading from an OGR source, drop all of the output tags with default values
        // This cleans up after the one2one rules since '0' can be a number or an enumerated attribute value
        if (muvd.configIn.ReaderDropDefaults == 'true')
        {
            muvd.dropDefaults(tags);
        }

        // Debug: add the FCODE to the tags
        if (muvd.configIn.OgrDebugAddfcode == 'true') tags['raw:debugFcode'] = attrs.F_CODE;

        // Override tag values if appropriate
        translate.overrideValues(tags,muvd.toChange);

        // Debug:
        if (muvd.configIn.OgrDebugDumptags == 'true')
        {
            translate.debugOutput(notUsedAttrs,layerName,geometryType,'','Not used: ');
            translate.debugOutput(tags,layerName,geometryType,'','Out tags: ');
        }

        return tags;
    }, // End of toOsm

    // We get Tags and return Attrs and a tableName
    toOgr : function(tags, elementType, geometryType)
    {
        var returnData = []; // The array of features to return
        var transMap = {}; // A map of translated attributes
        attrs = {}; // This is the output <GLOBAL>
        attrs.F_CODE = '';

        // Setup config variables. We could do this in initialize() but some things don't call it
        // Doing this so we don't have to keep calling into Hoot core
        if (muvd.configOut == undefined)
        {
            muvd.configOut = {};
            muvd.configOut.OgrAddUuid = hoot.Settings.get('ogr.add.uuid');
            muvd.configOut.OgrDebugDumptags = hoot.Settings.get('ogr.debug.dumptags');
            muvd.configOut.OgrFormat = hoot.Settings.get('ogr.output.format');
            muvd.configOut.OgrNoteExtra = hoot.Settings.get('ogr.note.extra');
            muvd.configOut.OgrTextFieldNumber = hoot.Settings.get("ogr.text.field.number");
            muvd.configOut.OgrThematicStructure = hoot.Settings.get('writer.thematic.structure');
            muvd.configOut.OgrCodedValues = hoot.Settings.get('ogr.coded.values');
            muvd.configOut.OgrThrowError = hoot.Settings.get('ogr.throw.error');
            print(muvd.configOut.OgrThrowError);

            // Get any changes to OSM tags
            // NOTE: the rest of the config variables will change to this style of assignement soon
            muvd.toChange = hoot.Settings.get('schema.translation.override');
        }

        // check if we have a schema. This is a quick way to work out if various lookup tables have been built
        if (muvd.rawSchema == undefined)
        {
            var tmp_schema = muvd.getDbSchema();
        }

        // "Collections" are groups of different feature types: Point, Area and Line
        // There is no way we can translate these to a single MUVD feature
        if (geometryType == 'Collection') return null;

        // Debug:
        if (muvd.configOut.OgrDebugDumptags == 'true') translate.debugOutput(tags,'',geometryType,elementType,'In tags: ');

        // Set up the fcode translation rules

        if (muvd.fcodeLookup == undefined)
        {
            // Order is important:
            // Start with the TRD1 specific FCODEs and then add the valid MUVD ones from the common list
            fcodeCommon.one2one.forEach( function(item) { if (muvd.rules.fcodeNameLookup[item[1]]) muvd.rules.fcodeOne2oneOut.push(item); });

            muvd.fcodeLookup = translate.createBackwardsLookup(muvd.rules.fcodeOne2oneOut);

            muvd.fcodeLookupOut = translate.createBackwardsLookup(muvd.rules.fcodeOne2oneOut);

            // Debug
            // translate.dumpOne2OneLookup(muvd.fcodeLookupOut);
        }

        if (muvd.lookup == undefined)
        {
            muvd.lookup = translate.createBackwardsLookup(muvd.rules.one2one);
        }
        // Override values if appropriate
        translate.overrideValues(tags,muvd.toChange);

        // pre processing
        muvd.applyToOgrPreProcessing(tags, attrs, geometryType);

        // Make a copy of the input tags so we can remove them as they get translated. What is left are
        // the not used tags
        var notUsedTags = (JSON.parse(JSON.stringify(tags)));

        // one 2 one
        translate.applyOne2One(notUsedTags, attrs, muvd.lookup, muvd.fcodeLookup, transMap, muvd.fcodeLookupOut);

        // post processing
        muvd.applyToOgrPostProcessing(tags, attrs, geometryType, notUsedTags);

        // Now check for invalid feature geometry
        // E.g. if the spec says a runway is a polygon and we have a line, throw error and
        // push the feature to the o2s layer
        if (muvd.rules.layerNameLookup.includes(geometryType.toString().charAt(0) + attrs.F_CODE))
        {
            // check if we need to return more than one feature
            // NOTE: this returns structure we are going to send back to hoot: {attrs: attrs, tableName: 'Name'}
            returnData = muvd.manyFeatures(geometryType,tags,attrs,transMap);

            // Now go through the features and clean them up
            var gType = geometryType.toString().charAt(0);

            // for loop to correct the many features returned
            for (var i = 0, fLen = returnData.length; i < fLen; i++)
            {
                returnData[i]['attrs']['FCODE'] = returnData[i]['attrs']['F_CODE'];
                hoot.logDebug('F_CODE: ' + returnData[i]['attrs']['F_CODE']);
                delete returnData[i]['attrs']['F_CODE'];

                // Now make sure that we have a valid feature before trying to validate and jam it into the list of
                // features to return
                var gFcode = gType + returnData[i]['attrs']['FCODE'];
                gFcode = gFcode.toUpperCase();

                if (muvd.rules.layerNameLookup.includes(gFcode))
                {
                    // Validate attrs: remove all that are not supposed to be part of a feature
                    muvd.validateAttrs(geometryType,returnData[i]['attrs'],notUsedTags,transMap);

                    // If we have unused tags, store them
                    if (Object.keys(notUsedTags).length > 0 && muvd.configOut.OgrNoteExtra == 'attribute')
                    {
                        var str = JSON.stringify(notUsedTags,Object.keys(notUsedTags).sort());
                        if (muvd.configOut.OgrFormat == 'shp')
                        {
                            // Split the tags into a maximum of 4 fields, each no greater than 225 char long.
                            var tList = translate.packText(notUsedTags,muvd.configOut.OgrTextFieldNumber,250);
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

                    // If we are using the thematic structure, fill the rest of the unused attrs in the schema
                    if (muvd.configOut.OgrThematicStructure == 'true')
                    {
                        returnData[i]['tableName'] = muvd.rules.thematicGroupLookup[gFcode];
                        muvd.validateThematicAttrs(gFcode, returnData[i]['attrs']);
                    }
                    else
                    {
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
            if (Object.keys(notUsedTags).length > 0 && muvd.configOut.OgrNoteExtra == 'file')
            {
                var extraFeature = {};
                extraFeature.tags = JSON.stringify(notUsedTags);
                extraFeature.uuid = attrs.UID;

                var extraName = 'extra_' + geometryType.toString().charAt(0);

                returnData.push({attrs: extraFeature, tableName: extraName});
            } // End notUsedTags

            // Look for review tags and push them to a review layer if found
            if (tags['hoot:review:needs'] == 'yes')
            {
                var reviewAttrs = {};

                // Note: some of these may be "undefined"
                reviewAttrs.note = tags['hoot:review:note'];
                reviewAttrs.score = tags['hoot:review:score'];
                reviewAttrs.uuid = tags.uuid;
                reviewAttrs.source = tags['hoot:review:source'];

                var reviewTable = 'review_' + geometryType.toString().charAt(0);
                returnData.push({attrs: reviewAttrs, tableName: reviewTable});
            } // End review tags

        } // End else we have a feature
        else // we don't have a feature
        {
            // for the UI: throw an error and die if we don't have a valid feature
            if (muvd.configOut.OgrThrowError == 'true')
            {
                if (!attrs.F_CODE)
                {
                    returnData.push({attrs:{'error':'No Valid Feature Code'}, tableName: ''});
                }
                else
                {
                    returnData.push({attrs:{'error':geometryType + ' geometry is not valid for ' + attrs.F_CODE + ' in MUVD'}, tableName: ''});
                }
                return returnData;
            }

            // Since we are not going to the UI, add the reason for dumping the feature to the list of
            // tags to help other tools.
            if (!attrs.F_CODE)
            {
                tags.o2s_reason = 'Unable to assign an F_CODE';
            }
            else
            {
                tags.o2s_reason = geometryType + ' geometry is not valid for ' + attrs.F_CODE + ' (' + muvd.rules.fcodeNameLookup[attrs.F_CODE] + ')';
            }

            tableName = 'o2s_' + geometryType.toString().charAt(0);

            // Debug:
            // Dump out what attributes we have converted before they get wiped out
            if (muvd.configOut.OgrDebugDumptags == 'true') translate.debugOutput(attrs,'',geometryType,elementType,'Converted attrs: ');

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
            if (muvd.configOut.OgrFormat == 'shp')
            {
                // Throw a warning that text will get truncated
                if (str.length > (muvd.configOut.OgrTextFieldNumber * 253)) hoot.logWarn('o2s tags truncated to fit in available space.');

                attrs = {};
                var tList = translate.packText(tags,muvd.configOut.OgrTextFieldNumber,253);
                for (var i = 1, tLen = tList.length; i < tLen; i++)
                {
                    attrs['tag'+1] = tList[i];
                }
            }
            else
            {
                attrs = {tag1:str};
            }

            returnData.push({attrs: attrs, tableName: tableName});
        } // End we don't have a feature

        // Debug:
        if (muvd.configOut.OgrDebugDumptags == 'true')
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

} // End of muvd