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

    // Build the MGCP fcode/attrs lookup table. Note: This is <GLOBAL>
    mgcp.AttrLookup = translate.makeAttrLookup(mgcp.rawSchema);

    // Now build the FCODE/layername lookup table. Note: This is <GLOBAL>
    mgcp.layerNameLookup = translate.makeLayerNameLookup(mgcp.rawSchema);

    // Now add an o2s[A,L,P] feature to the mgcp.rawSchema
    // We can drop features but this is a nice way to see what we would drop
    mgcp.rawSchema = translate.addEmptyFeature(mgcp.rawSchema);

    // Add empty Review layers
    mgcp.rawSchema = translate.addReviewFeature(mgcp.rawSchema);

    // Add empty "extra" feature layers if needed
    if (config.getOgrNoteExtra() == 'file') mgcp.rawSchema = translate.addExtraFeature(mgcp.rawSchema);

    // This function dumps the schema to the screen for debugging
    // translate.dumpSchema(mgcp.rawSchema);

    return mgcp.rawSchema;
    }, // End of getDbSchema


    // validateAttrs: Clean up the supplied attr list by dropping anything that should not be part of the
    //                feature
    validateAttrs: function(geometryType,attrs) {

        var attrList = mgcp.AttrLookup[geometryType.toString().charAt(0) + attrs.FCODE];

        if (attrList != undefined)
        {
            for (var val in attrs)
            {
                if (attrList.indexOf(val) == -1)
                {
                    hoot.logDebug('Validate: Dropping ' + val + ' from ' + attrs.FCODE);
                    delete attrs[val];

                    // Since we deleted the attribute, Skip the text check
                    continue;
                }

                // Now check the length of the text fields
                // We need more info from the customer about this: What to do if it is too long
                if (val in mgcp.rules.txtLength)
                {
                    if (attrs[val].length > mgcp.rules.txtLength[val])
                    {
                        // First try splitting the attribute and grabbing the first value
                        var tStr = attrs[val].split(';');
                        if (tStr[0].length <= mgcp.rules.txtLength[val])
                        {
                            attrs[val] = tStr[0];
                        }
                        else
                        {
                            hoot.logDebug('Validate: Attribute ' + val + ' is ' + attrs[val].length + ' characters long. Truncating to ' + mgcp.rules.txtLength[val] + ' characters.');
                            // Still too long. Chop to the maximum length
                            attrs[val] = tStr[0].substring(0,mgcp.rules.txtLength[val]);
                        }
                    } // End text attr length > max length
                } // End in txtLength
            }
        }
        else
        {
            hoot.logDebug('Validate: No attrList for ' + attrs.FCODE + ' ' + geometryType);
        }

        // No quick and easy way to do this unless we build yet another lookup table
        var feature = {};

        for (var i=0, sLen = mgcp.rawSchema.length; i < sLen; i++)
        {
            if (mgcp.rawSchema[i].fcode == attrs.FCODE && mgcp.rawSchema[i].geom == geometryType)
            {
                feature = mgcp.rawSchema[i];
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
            if (!attrs[enumName]) continue;

            var attrValue = attrs[enumName];
            var enumList = feature.columns[i].enumerations;
            var enumValueList = [];

            // Pull all of the values out of the enumerated list to make life easier
            for (var j=0, elen = enumList.length; j < elen; j++) enumValueList.push(enumList[j].value);

            // Check if it is a valid enumerated value
            if (enumValueList.indexOf(attrValue) == -1)
            {
                hoot.logDebug('Validate: Enumerated Value: ' + attrValue + ' not found in ' + enumName);

                // Do we have an "Other" value?
                if (enumValueList.indexOf('999') == -1)
                {
                    // No: Set the offending enumerated value to the default value
                    attrs[enumName] = feature.columns[i].defValue;

                    hoot.logDebug('Validate: Enumerated Value: ' + attrValue + ' not found in ' + enumName + ' Setting ' + enumName + ' to its default value (' + feature.columns[i].defValue + ')');
                }
                else
                {
                    // Yes: Set the offending enumerated value to the "other" value
                    attrs[enumName] = '999';

                    hoot.logDebug('Validate: Enumerated Value: ' + attrValue + ' not found in ' + enumName + ' Setting ' + enumName + ' to Other (999)');
                }
            }
        } // End Validate Enumerations
    }, // End validateAttrs


    // Sort out if we need to return more than one feature
    // This is generally for Roads, Railways, bridges, tunnels etc
    manyFeatures: function(geometryType, tags, attrs)
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
            mgcp.applyToMgcpPreProcessing(newFeatures[i]['tags'], newFeatures[i]['attrs'], geometryType);

            var notUsedTags = (JSON.parse(JSON.stringify(tags)));

            // apply the simple number and text biased rules
            // Note: These are BACKWARD, not forward!
            translate.applySimpleNumBiased(newFeatures[i]['attrs'], notUsedTags, mgcp.rules.numBiased,'backward',mgcp.rules.intList);
            translate.applySimpleTxtBiased(newFeatures[i]['attrs'], notUsedTags, mgcp.rules.txtBiased,'backward');

            // one 2 one - we call the version that knows about OTH fields
            translate.applyOne2One(notUsedTags, newFeatures[i]['attrs'], mgcp.lookup, mgcp.fcodeLookup);

            // post processing
            mgcp.applyToMgcpPostProcessing(newFeatures[i]['tags'], newFeatures[i]['attrs'], geometryType,notUsedTags);

            returnData.push({attrs: newFeatures[i]['attrs'],tableName: ''});
        }

        return returnData;
    }, // End manyFeatures


    // ##### Start of the xxToOsmxx Block #####

    // Untangle MGCP attributes & OSM tags
    // Some people have been editing OSM files and inserting MGCP attributes
    untangleAttributes: function (attrs, tags)
    {
        // If we use ogr2osm, the GDAL driver jams any tag it doesn't know about into an "other_tags" tag
        // We need to unpack this before we can do anything
        if (attrs.other_tags)
        {
            var tList = attrs.other_tags.split('","');

            delete attrs.other_tags;

            for (var val in tList)
            {
                vList = tList[val].split('"=>"');

                attrs[vList[0].replace('"','')] = vList[1].replace('"','');

                // Debug
                //print('val: ' + tList[val] + '  vList[0] = ' + vList[0] + '  vList[1] = ' + vList[1]);
            }
        }

        for (var col in attrs)
        {
            // Sort out FCODE funkyness:  f_CODE, F_Code etc
            var tKey = col.toLowerCase();
            tKey = tKey.replace(/\s/g, '').replace(/_/g, '');;

            if (tKey == 'fcode' && col !== 'F_CODE')
            {
                attrs.F_CODE = attrs[col];
                delete attrs[col];
                continue;
            }

            // Check for an FCODE as a tag
            if (col in mgcp.fcodeLookup['F_CODE'])
            {
                attrs.F_CODE = col;
                delete attrs[col];
                continue;
            }

            // Stuff to be ignored or that gets swapped later - See applyToOsmPreProcessing
            if (['FCODE','error_circ','CPYRT_NOTE','SRC_INFO','SRC_DATE','SMC'].indexOf(col) > -1) continue;

            // Look for Attributes
            if (col in mgcp.numLookup) continue;

            if (col in mgcp.txtLookup) continue;

            if (col in mgcp.lookup) continue;

            // Drop the "GEOM" attribute
            if (col == 'GEOM')
            {
                delete attrs[col];
                continue;
            }

            // Not an Attribute so push it to the tags object
            tags[col] = attrs[col];
            delete attrs[col];
        }

    }, // End attribute attributeUntangle


    applyToOsmPreProcessing: function(attrs, layerName, geometryType)
    {
        // Drop the FCSUBTYPE since we don't use it
        if (attrs.FCSUBTYPE) delete attrs.FCSUBTYPE;

        // The swap list. These are the same attr, just named differently
        // These may get converted back on output
        var swapList = {
                'CPYRT_NOTE':'CCN',
                'SRC_INFO':'SDP',
                'SRC_DATE':'SDV',
                'SMC':'MCC'
                };

        // List of data values to drop/ignore
        var ignoreList = { '-32765':1,'-32767':1,'-32768':1,
                           '998':1,'-999999':1,
                           'n_a':1,'noinformation':1,'unknown':1,'unk':1 };

        // Unit conversion. Some attributes are in centimetres, others in decimetres
        // var unitList = { 'GAW':100,'HCA':10,'WD1':10,'WD2':10,'WD3':10,'WT2':10 };

        for (var col in attrs)
        {
            // slightly ugly but we would like to account for: 'No Information','noInformation' etc
            // First, push to lowercase
            var attrValue = attrs[col].toString().toLowerCase();

            // Get rid of the spaces in the text
            attrValue = attrValue.replace(/\s/g, '');

            // Wipe out the useless values
            // if (attrs[col] == '-32768' || attrValue == 'unk' || attrValue == 'n_a' || attrValue == 'noinformation' || attrs[col] == '')
            if (attrs[col] == '' || attrValue in ignoreList || attrs[col] in ignoreList)
            {
                // debug: Comment this out to leave all of the No Info stuff in for testing
                delete attrs[col];
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

            // Sort out units - if needed
            // if (col in unitList) attrs[col] = attrs[col] / unitList[col];

            // Now see if we need to swap attr names
            if (col in swapList)
            {
                // print('Swapped: ' + swapList[i]); // debug
                attrs[swapList[col]] = attrs[col];
                delete attrs[col];
                continue
            }
        } // End col in attrs loop

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
                ['ZD040', ['zd040']], // Annotated Location
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

        // The FCODE for Buildings is different. TDS uses AL013
        if (attrs.F_CODE == 'AL015') attrs.F_CODE = 'AL013';

        // Swap the F_CODE for a Ferry Terminal
        if (attrs.F_CODE == 'AQ125' && attrs.TRS == '7')
        {
            attrs.F_CODE = 'AQ080'; // TDS/GGDM fcode for Ferry Terminal
        }

    }, // End of applyToOsmPreProcessing

    // Post Processing: Lots of cleanup
    applyToOsmPostProcessing : function (attrs, tags, layerName, geometryType)
    {
        // Unpack the TXT field
        if (tags.note)
        {
            var tObj = translate.unpackMemo(tags.note);

            if (tObj.tags !== '')
            {
                var tTags = JSON.parse(tObj.tags)
                for (i in tTags)
                {
                    // Debug
                    // print('Memo: Add: ' + i + ' = ' + tTags[i]);
                    if (tags[tTags[i]]) hoot.logWarn('Unpacking TXT, overwriting ' + i + ' = ' + tags[i] + '  with ' + tTags[i]);
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
        if (attrs.HWT && attrs.HWT !== '0')
        {
            tags.amenity = 'place_of_worship';

            if (tags.building)
            {
                switch (tags.building)
                {
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
            }

            if (tags['tower:type'] == 'minaret')
            {
                tags.religion = 'muslim';
            }
        } // End HWT

        // Add the LayerName to the source
        if ((! tags.source) && layerName !== '') tags.source = 'mgcp:' + layerName.toLowerCase();

        // If we have a UID, store it
        if (tags.uuid)
        {
            tags.uuid = '{' + tags['uuid'].toString().toLowerCase()  + '}'
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
            ["t['cable:type'] && !(t.cable)","t.cable = 'yes'"],
            ["t.control_tower == 'yes'","t['tower:type'] = 'observation'; t.use = 'air_traffic_control'"],
            ["t['generator:source'] == 'wind'","t.power = 'generator'"],
            ["(t.landuse == 'built_up_area' || t.place == 'settlement') && t.building","t['settlement:type'] = t.building; delete t.building"],
            ["t.leisure == 'stadium'","t.building = 'yes'"],
            ["t['monitoring:weather'] == 'yes'","t.man_made = 'monitoring_station'"],
            ["t.natural =='spring' && t['spring:type'] == 'spring'","delete t['spring:type']"],
            ["t.public_transport == 'station'","t.bus = 'yes'"],
            ["t.pylon =='yes' && t['cable:type'] == 'power'"," t.power = 'tower'"],
            ["t['social_facility:for'] == 'senior'","t.amenity = 'social_facility'; t.social_facility = 'group_home'"],
            ["t['tower:type'] && !(t.man_made)","t.man_made = 'tower'"],
            ["t.water && !(t.natural)","t.natural = 'water'"],
            ["t.waterway == 'flow_control'","t.flow_control = 'sluice_gate'"]
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

        // Add 'building = yes' to amenities if we don't already have one
        // if (tags.amenity && !tags.building)
        // {
        //     // Debug:
        //     // print('Added building');
        //     // Don't add building=yes to built up areas!
        //     if (!tags.place) tags.building = 'yes';
        // }

        // if (tags.building == 'train_station' && !tags.railway) tags.railway = 'station';
        // if ('ford' in tags && !tags.highway) tags.highway = 'road';

        // Some FCODE specific rules
        switch (attrs.F_CODE)
        {
            case undefined: // Break early if no value
                break;

            case 'AA052': // Hydrocarbons Field
                tags.landuse = 'industrial';
                break;

            case 'AF030': // Cooling Tower
                if (!tags['tower:type']) tags['tower:type'] = 'cooling';
                break;

            case 'AL013': // Building  NOTE this is the TDS F_CODE for Building. This was swapped during pre-processing
                if (tags.surface == 'unknown') delete tags.surface;
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

            case 'AQ125': // Transportation Station
                if (tags.amenity == 'ferry_terminal')
                {
                    attrs.TRS = '7';
                    if (tags.bus) delete tags.bus;
                }
                break;

            case 'BH070': // Ford
                // Fords are also supposed to be roads
                if (geometryType == 'Line' && !tags.highway) tags.highway = 'road';
                break;

            case 'GB485': // Approach Lighting System
                tags.navigationaid = 'als';
                break;

            case 'AH050': // Fortification
                // Castles are not Bunkers but they get stored in the same layer
                if (tags.military == 'bunker' && tags.historic == 'castle') delete tags.military;
                break;
        } // End switch FCODE

        // Sort out TRS (Transport Type)
        switch (attrs.TRS)
        {
            case undefined:
                break;

            case '9': // Pedestrian
                // NOTE: This _might_ be a path: AP050 (Trail)
                if (!tags.highway) tags.highway = 'track';
                break;

            case '12': // Railway
                if (!tags.railway) tags.railway = 'rail';
                break;

            case '3': // TRD3 'Automotive'
            case '4': // Bus
            case '13': // Road
                if (!tags.highway) tags.highway = 'road';
                break;

            case '14': // Road and Railway. This might be ugly....
                if (!tags.highway) tags.highway = 'road';
                if (!tags.railway) tags.railway = 'rail';
                break;
        } // End switch TRS

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
            tags.facility = 'yes';
        }

        // Sort out the WID, WD1 etc attributes
        // Roads etc have a WD1 attribute but this doesn't get translated to "width"
        if (attrs.WD1)
        {
            if (! tags.width) tags.width = attrs.WD1;
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

    // ##### Start of the xxToMgcpxx Block #####
    applyToMgcpPreProcessing: function(tags, attrs, geometryType)
    {
        // Remove Hoot assigned tags for the source of the data
        if (tags['source:ingest:datetime']) delete tags['source:ingest:datetime'];
        if (tags.area) delete tags.area;
        if (tags['error:circular']) delete tags['error:circular'];
        if (tags['hoot:status']) delete tags['hoot:status'];

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

        // Lifecycle and general cleaning
        // This is a bit funky and should probably be done with a fancy function instead of repeating the code
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

            // Doing this here so we preserve the "highway" tag
            case 'bridleway':
            case 'cycleway':
            case 'footway':
            case 'steps':
                attrs.F_CODE = 'AP050'; // Trail
                break;

            case 'service':
            case 'living_street':
            case 'pedestrian':
                attrs.F_CODE = 'AP030'; // Road
                break;
        } // End highway

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

        if (mgcp.mgcpPreRules == undefined)
        {
            // See ToOsmPostProcessing for more details about rulesList
            var rulesList = [
            // ["t.amenity == 'marketplace'","t.facility = 'yes'"],
            ["t.amenity == 'ferry_terminal'","t['transport:type'] = 'maritime'"],
            ["t.aeroway == 'navigationaid' && t.navigationaid","delete t.navigationaid"],
            ["t.barrier == 'tank_trap' && t.tank_trap == 'dragons_teeth'","t.barrier = 'dragons_teeth'; delete t.tank_trap"],
            ["t.communication == 'line'","t['cable:type'] = 'communication'"],
            // ["t.construction && t.railway","t.railway = t.construction; t.condition = 'construction'; delete t.construction"],
            // ["t.construction && t.highway","t.highway = t.construction; t.condition = 'construction'; delete t.construction"],
            ["t.content && !(t.product)","t.product = t.content; delete t.content"],
            ["t.landuse == 'railway' && t['railway:yard'] == 'marshalling_yard'","a.F_CODE = 'AN060'"],
            ["t.leisure == 'stadium' && t.building","delete t.building"],
            ["t.man_made && t.building == 'yes'","delete t.building"],
            ["t.man_made == 'water_tower'","a.F_CODE = 'AL241'"],
            ["t.natural == 'sinkhole'","a.F_CODE = 'BH145'; t['water:sink:type'] = 'disappearing'; delete t.natural"],
            ["t.natural == 'spring' && !(t['spring:type'])","t['spring:type'] = 'spring'"],
            ["t.natural == 'wood'","t.landuse = 'forest'"],
            ["t.power == 'generator'","a.F_CODE = 'AL015'; t.use = 'power_generation'"],
            //["t.power == 'line'","t['cable:type'] = 'power'; t.cable = 'yes'"],
            ["t.power == 'tower'","t['cable:type'] = 'power'; t.pylon = 'yes'; delete t.power"],
            ["t.rapids == 'yes'","t.waterway = 'rapids'"],
            ["t.resource","t.product = t.resource; delete t.resource"],
            ["t.route == 'road' && !(t.highway)","t.highway = 'road'; delete t.route"],
            // ["(t.shop || t.office) && !(t.building)","a.F_CODE = 'AL015'"],
            ["t.tourism == 'information' && t.information","delete t.tourism"],
            ["t.tunnel == 'building_passage'","t.tunnel = 'yes'"],
            ["!(t.water) && t.natural == 'water'","t.water = 'lake'"],
            ["t.water == 'pond'","a.F_CODE = 'BH170'; t.natural = 'other_pool_type'"],
            ["t.water == 'river'","t.waterway = 'river'"],
            ["t.waterway == 'riverbank'","t.waterway = 'river'"],
            ["t.wetland && t.natural == 'wetland'","delete t.natural"]
            ];

            mgcp.mgcpPreRules = translate.buildComplexRules(rulesList);
        }

        // Apply the rulesList
        //translate.applyComplexRules(tags,attrs,mgcp.mgcpPreRules);
        for (var i = 0, rLen = mgcp.mgcpPreRules.length; i < rLen; i++)
        {
            if (mgcp.mgcpPreRules[i][0](tags)) mgcp.mgcpPreRules[i][1](tags,attrs);
        }

        // Sort out landuse
        switch (tags.landuse)
        {
            case undefined: // Break early if no value
                break;

            case 'brownfield':
                tags.landuse = 'built_up_area';
                tags.condition = 'destroyed';
                break

            case 'construction':
                tags.condition = 'construction';
                tags.landuse = 'built_up_area';
                break;

            case 'commercial':
            case 'retail':
                tags.use = 'commercial';
                tags.landuse = 'built_up_area';
                break;

            case 'farm':
            case 'allotments':
                tags.landuse = 'farmland';
                break;

            case 'farmyard': // NOTE: This is different to farm
                tags.facility = 'yes';
                tags.use = 'agriculture';
                delete tags.landuse;
                break;

            case 'grass':
            case 'meadow':
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
                }
                break;

            case 'military':
                tags.military = 'installation';
                delete tags.landuse;
                break;

            case 'residential':
                tags.use = 'residential';
                tags.landuse = 'built_up_area';
                break;
        } // End switch landuse


        // Fix up OSM 'walls' around facilities
        if (tags.barrier == 'wall' && geometryType == 'Area')
        {
            attrs.F_CODE = 'AL010'; // Facility
            delete tags.barrier; // Take away the walls...
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
                if (tags.building) delete tags.building;
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

        // Keep looking for an FCODE
        // This uses the fcodeLookup tables that are defined earlier
        // var fcodeLookup = translate.createLookup(fcodeList);
        if (!attrs.F_CODE)
        {
            for (var col in tags)
            {
                var value = tags[col];
                if (col in mgcp.fcodeLookup)
                {
                    if (value in mgcp.fcodeLookup[col])
                    {
                        var row = mgcp.fcodeLookup[col][value];
                        attrs.F_CODE = row[1];
                    }
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

        // The FCODE for Buildings changed...
        if (attrs.F_CODE == 'AL013') attrs.F_CODE = 'AL015';

        // Tag changed
        if (tags.vertical_obstruction_identifier)
        {
            tags['aeroway:obstruction'] = tags.vertical_obstruction_identifier;
            delete tags.vertical_obstruction_identifier;
        }

        // Soil Surface Regions
        if (! attrs.F_CODE)
        {
            if (tags.surface)
            {
                attrs.F_CODE = 'DA010'; // Soil Surface Region
                if (! tags.material)
                {
                    tags.material = tags.surface;
                    delete tags.surface;
                }
            }

            switch (tags.natural)
            {
                case 'mud':
                    attrs.F_CODE = 'DA010'; // Soil Surface Region
                    tags.material = tags.surface;
                    break;

                case 'sand':
                    attrs.F_CODE = 'DA010'; // Soil Surface Region
                    tags.material = tags.surface;
                 break;

                case 'bare_rock':
                    attrs.F_CODE = 'DA010'; // Soil Surface Region
                    tags.material = tags.surface;
                    break;

                case 'rock':
                    attrs.F_CODE = 'DA010'; // Soil Surface Region
                    tags.material = tags.surface;
                    break;

                case 'stone':
                    attrs.F_CODE = 'DA010'; // Soil Surface Region
                    tags.material = tags.surface;
                    break;

                case 'scree':
                    attrs.F_CODE = 'DA010'; // Soil Surface Region
                    tags.material = tags.surface;
                    break;

                case 'shingle':
                    attrs.F_CODE = 'DA010'; // Soil Surface Region
                    tags.material = tags.surface;
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
        }

        // Names. Sometimes we don't have a name but we do have language ones
        if (!tags.name) translate.swapName(tags);

    }, // End applyToMgcpPreProcessing


    applyToMgcpPostProcessing : function (tags, attrs, geometryType, notUsedTags)
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
                'railway:in_road':'AN010','tourism':'AL013','mine:access':'AA010',
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
            attrs.UID = str[0].replace('{','').replace('}','');
        }
        else if (tags['hoot:id'])
        {
            attrs.UID = 'raw_id:' + tags['hoot:id'];
        }
        else
        {
            if (mgcp.configOut.OgrAddUuid == 'true') attrs.UID = createUuid().replace('{','').replace('}','');
        }

        // Default railway
        // if (attrs.F_CODE == 'AN010' && tags.railway == 'yes') attrs.RRC = '0';

        // Not a great fit
        if (tags.public_transportation == 'station' && tags.bus == 'yes') attrs.FFN = '481';

        // Mapping Senior Citizens home to Accomodation. Not great
        if (tags.amenity == 'social_facility' && tags['social_facility:for'] == 'senior') attrs.FFN = 550;

        // These FCODES have "No prescribed attributes" in TRDv40
        // Therefore:
        // - clean up the the attrs
        // - JSON the tags and stick them in a text field
        var noAttrList = [ 'AJ010','AJ030','AK170','AL019','AL070','AL099','AN076',
                           'BD100','BD180','BH165','BI010','BJ020','BJ031','BJ110',
                           'DB061','DB071','DB100',
                           'EA020','EA055','EC010','EC040','EC060',
                           'FA090',
                           'GB050',
                           'ZD020',
                         ];

        if (noAttrList.indexOf(attrs.F_CODE) !== -1)
        {
            // The metadata tags to skip
            var skipTags = ['F_CODE','ACC','TXT','UID','SDV','SDP','CCN','SRT'];
            var tmpAttrs = {};

            for (var i in attrs)
            {
                // Skip the metadata tags, save the others, then delete them
                // from the main list of attrs
                if (skipTags.indexOf(i) == -1)
                {
                    tmpAttrs[i] = attrs[i];
                    delete attrs[i];
                }
            }
            attrs.TEXT = JSON.stringify(tmpAttrs);
        }

        // Add Weather Restrictions to transportation features
        if (['AP010','AP030','AP050'].indexOf(attrs.FCODE > -1) && !attrs.WTC )
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

            case 'AJ085': // Barn: Valid NFDD/NAS FCODE but not in the MGCP spec
                attrs.F_CODE = 'AL015'; // Barns are Buildings
                break;

            case 'AK040': // Athletic Field, Sports Ground
                if (attrs.MCC)
                {
                    attrs.SMC = attrs.MCC;
                    delete attrs.MCC;
                }
                break;

            case 'AL015': // General Building
                // Unknown House of Worship
                if (tags.amenity == 'place_of_worship' && tags.building == 'other') attrs.HWT = 999;

                if (attrs.HWT && ! tags.amenity && ! attrs.FFN)
                {
                    attrs.FFN = '931';
                }

                if (attrs.FFN && (attrs.FFN !== '930' && attrs.FFN !== '931'))
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
                }
                break;

            case 'AL020': // Built-up Area
                // Allowed values for FUC
                if (['0','1','2','4','19','999'].indexOf(attrs['FUC']) < 0) attrs.FUC = '999';
                break;

            case 'AL105': // Settlement
                // Allowed values for FUC
                if (['0','4','8','19','999'].indexOf(attrs['FUC']) < 0) attrs.FUC = '999';
                break;

            case 'AN010': // Railway
                if (tags.bridge) attrs.LOC = '45'; // Above Surface
                if (tags.tunnel) attrs.LOC = '40'; // Below Surface
                if (tags.embankment || tags.man_made == 'causeway') attrs.LOC = '44'; // On Surface

                // Single lane roads dont have a median and are not separated
                // NOTE: This could cause problems
                if (attrs.LTN == '1')
                {
                    attrs.SEP = '1000';
                    attrs.MES = '1000';
                }
                break;

            case 'AP010': // Cart Track
                if (attrs.WID && ! attrs.WD1)
                {
                    attrs.WD1 = attrs.WID;
                    delete attrs.WID;
                }
                break;

            case 'AP030': // Road
                if (tags.bridge) attrs.LOC = '45'; // Above Surface
                if (tags.tunnel) attrs.LOC = '40'; // Below Surface
                if (tags.embankment || tags.man_made == 'causeway') attrs.LOC = '44'; // On Surface
                if (attrs.RST == '6') attrs.RST = '2'; // Move 'ground' to 'unpaved'
                if (attrs.WID && ! attrs.WD1)
                {
                    attrs.WD1 = attrs.WID;
                    delete attrs.WID;
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
                break;

            case 'AQ140': // Vehicle Lot/Vehicle Storage area: Valid NFDD/NAS FCODE but not in the MGCP spec
                if (geometryType == 'Point') attrs.F_CODE = 'AL015'; // Parking Garage Building
                break;

            case 'BA050': // Beach
                if (attrs.MCC)
                {
                    attrs.SMC = attrs.MCC;
                    delete attrs.MCC;
                }
                break;

            case 'DA010': // Soil Surface Area
                if (attrs.MCC)
                {
                    attrs.SMC = attrs.MCC;
                    delete attrs.MCC;
                }
                break;

            case 'DB010': // Bluff/Cliff/Escarpment
                if (attrs.MCC)
                {
                    attrs.SMC = attrs.MCC;
                    delete attrs.MCC;
                }
                break;

            case 'DB070': // Cut
                if (attrs.MCC)
                {
                    attrs.SMC = attrs.MCC;
                    delete attrs.MCC;
                }
                break;

            case 'DB090': // Embankment
                // If the embankment supports a transportation feature
                if (tags.highway || tags.railway)
                {
                    attrs.FIC = '2'; // Fill
                }
                else
                {
                    attrs.FIC = '1'; // Mound
                }
                break;

            case 'EA010': // Crop Land
                if (attrs.CSP == '15') attrs.F_CODE = 'EA040';
                // hoot.logTrace('TRD3 feature EA010 changed to TRD4 EA040 - some data has been dropped');
                break;

            case 'ED030': // Mangrove Swamp
                if (! attrs.TID) attrs.TID = '1001'; // Tidal
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
            ["t.man_made == 'water_well'","a.F_CODE = 'AA050'"],
            ["t.man_made == 'well'","a.F_CODE = 'AA050'"],
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

        // Fix up SRT values so we comply with the spec. These values came from data files
        // Format is: orig:new
        srtFix = {
            28:112,  // Ikonos Imagery -> Hires Commercial
            66:112,  // Quickbird Multi 2.44m -> Hires Commercial
            68:110,  // Quickbird Pan 60cm -> Very Hires Commercial
        }; // End srtFix

        if (attrs.SRT in srtFix) attrs.SRT = srtFix[attrs.SRT];

        //Map alternate source date tags to SDV in order of precedence
        //default in mgcp_rules is 'source:datetime'
        if (! attrs.SDV)
            attrs.SDV = tags['source:imagery:datetime']
                || tags['source:date']
                || tags['source:geometry:date']
                || '';

        //Map alternate source tags to ZI001_SDP in order of precedence
        //default in mgcp_rules is 'source'
        if (! attrs.SDP)
            attrs.SDP = tags['source:imagery']
                || tags['source:description']
                || '';

        // Chop the milliseconds off the "source:datetime"
        if (attrs.SDV)
        {
            attrs.SDV = translate.chopDateTime(attrs.SDV);
        }

    }, // End of applyToMgcpPostProcessing

    // ##### End of the xxToMgcpxx Block #####

    // toOsm - Translate Attrs to Tags
    toOsm : function(attrs, layerName, geometryType)
    {
        tags = {};  // This is the output

        // Setup config variables. We could do this in initialize() but some things don't call it :-(
        // Doing this so we don't have to keep calling into Hoot core
        if (mgcp.configIn == undefined)
        {
            mgcp.configIn = {};
            mgcp.configIn.OgrDebugAddfcode = config.getOgrDebugAddfcode();
            mgcp.configIn.OgrDebugDumptags = config.getOgrDebugDumptags();
            mgcp.configIn.OgrAddUuid = config.getOgrAddUuid();

            // Get any changes
            mgcp.toChange = hoot.Settings.get("schema.translation.override");
        }

        // Debug:
        if (mgcp.configIn.OgrDebugDumptags == 'true') translate.debugOutput(attrs,layerName,geometryType,'','In Attrs: ');

        // See if we have an o2s_X layer and try to unpack it
        if (layerName.indexOf('o2s_') > -1)
        {
            tags = translate.parseO2S(attrs);

            // Add some metadata
            if (! tags.uuid)
            {
                if (mgcp.configIn.OgrAddUuid == 'true') tags.uuid = createUuid();
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
            mgcp.rules.fcodeOne2oneV4.push.apply(mgcp.rules.fcodeOne2oneV4,fcodeCommon.one2one);

            mgcp.fcodeLookup = translate.createLookup(mgcp.rules.fcodeOne2oneV4);

            // Debug:
            // translate.dumpOne2OneLookup(mgcp.fcodeLookup);
        }

        if (mgcp.lookup == undefined)
        {
            // Setup lookup tables to make translation easier

            // Add the MGCPv3.0 specific attributes to the v4.0/common attribute table
            mgcp.rules.one2one.push.apply(mgcp.rules.one2one,mgcp.rules.one2oneIn);

            mgcp.lookup = translate.createLookup(mgcp.rules.one2one);
        }

        if (mgcp.txtLookup == undefined)
        {
            mgcp.txtLookup = {};
            // Add the MGCPv3.0 specific attributes to the v4.0/common attribute table
            for (var i in mgcp.rules.txtBiased) mgcp.txtLookup[i] = mgcp.rules.txtBiased[i];
            for (var i in mgcp.rules.txtBiasedV3) mgcp.txtLookup[i] = mgcp.rules.txtBiasedV3[i];
        }

        if (mgcp.numLookup == undefined)
        {
            mgcp.numLookup = {};
            for (var i in mgcp.rules.numBiased) mgcp.numLookup[i] = mgcp.rules.numBiased[i];
            for (var i in mgcp.rules.numBiasedV3) mgcp.numLookup[i] = mgcp.rules.numBiasedV3[i];
        }


        // A little cleaning before we try to untangle stuff
        delete attrs.SHAPE_Length;
        delete attrs.SHAPE_Area;

        // Untangle MGCP attributes & OSM tags
        // NOTE: This could get wrapped with an ENV variable so it only gets called during import
        mgcp.untangleAttributes(attrs, tags);

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
                tags[ftag[0]] = ftag[1];
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
        translate.applySimpleNumBiased(notUsedAttrs, tags, mgcp.numLookup,'forward',[]);
        translate.applySimpleTxtBiased(notUsedAttrs, tags,  mgcp.txtLookup,'forward');

        // one 2 one
        translate.applyOne2One(notUsedAttrs, tags, mgcp.lookup, {'k':'v'});

        // post processing
        mgcp.applyToOsmPostProcessing(attrs, tags, layerName, geometryType);

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
    toMgcp : function(tags, elementType, geometryType)
    {
        var tableName = '';
        var returnData = []; // The array of features to return
        attrs = {}; // This is the output <GLOBAL>
        attrs.F_CODE = '';

        // Setup config variables. We could do this in initialize() but some things don't call it :-(
        // Doing this so we don't have to keep calling into Hoot core
        if (mgcp.configOut == undefined)
        {
            mgcp.configOut = {};
            mgcp.configOut.OgrDebugDumptags = config.getOgrDebugDumptags();
            mgcp.configOut.OgrNoteExtra = config.getOgrNoteExtra();
            mgcp.configOut.OgrSplitO2s = config.getOgrSplitO2s();
            mgcp.configOut.OgrThrowError = config.getOgrThrowError();
            mgcp.configOut.OgrAddUuid = config.getOgrAddUuid();

            // Get any changes to OSM tags
            // NOTE: the rest of the config variables will change to this style of assignment soon
            mgcp.toChange = hoot.Settings.get("schema.translation.override");
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
            // First the MGCPv4 FCODES, then the common ones. This ensures that the common ones don't
            // stomp on the V4 ones
            mgcp.rules.fcodeOne2oneV4.push.apply(mgcp.rules.fcodeOne2oneV4,mgcp.rules.fcodeOne2oneOut);
            mgcp.rules.fcodeOne2oneV4.push.apply(mgcp.rules.fcodeOne2oneV4,fcodeCommon.one2one);

            mgcp.fcodeLookup = translate.createBackwardsLookup(mgcp.rules.fcodeOne2oneV4);

            // Debug
            // translate.dumpOne2OneLookup(mgcp.fcodeLookup);
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
        mgcp.applyToMgcpPreProcessing(tags, attrs, geometryType);

        // Make a copy of the input tags so we can remove them as they get translated. What is left is
        // the not used tags
        // not in v8 yet: // var tTags = Object.assign({},tags);
        var notUsedTags = (JSON.parse(JSON.stringify(tags)));

        if (notUsedTags.hoot) delete notUsedTags.hoot; // Added by the UI
        // Debug info. We use this in postprocessing via "tags"
        if (notUsedTags['hoot:id']) delete notUsedTags['hoot:id'];

        // apply the simple number and text biased rules
        translate.applySimpleNumBiased(attrs, notUsedTags, mgcp.rules.numBiased,'backward',mgcp.rules.intList);
        translate.applySimpleTxtBiased(attrs, notUsedTags,  mgcp.rules.txtBiased,'backward');

        // one 2 one
        translate.applyOne2One(notUsedTags, attrs, mgcp.lookup, mgcp.fcodeLookup);

        // post processing
        // mgcp.applyToMgcpPostProcessing(attrs, tableName, geometryType);
        mgcp.applyToMgcpPostProcessing(tags, attrs, geometryType, notUsedTags);

        // Debug
        if (mgcp.configOut.OgrDebugDumptags == 'true') translate.debugOutput(notUsedTags,'',geometryType,elementType,'Not used: ');

        // If we have unused tags, add them to the TXT field
        // NOTE: We are not checking if this is longer than 255 characters
        if (Object.keys(notUsedTags).length > 0 && mgcp.configOut.OgrNoteExtra == 'attribute')
        {
            var tStr = '<OSM>' + JSON.stringify(notUsedTags) + '</OSM>';
            attrs.TXT = translate.appendValue(attrs.TXT,tStr,';');
        }

        // Set the tablename: [P,A,L]<fcode>
        // tableName = geometryType.toString().substring(0,1) + attrs.F_CODE;
        tableName = geometryType.toString().charAt(0) + attrs.F_CODE;

        // Now check for invalid feature geometry
        // E.g. If the spec says a runway is a polygon and we have a line, throw error and
        // push the feature to the o2s layer
        if (mgcp.layerNameLookup[tableName])
        {
            // Check if we need to return more than one feature
            // NOTE: This returns structure we are going to send back to Hoot:  {attrs: attrs, tableName: 'Name'}
            returnData = mgcp.manyFeatures(geometryType,tags,attrs);

            // Now go through the features and clean them up
            var gType = geometryType.toString().charAt(0);

            for (var i = 0, fLen = returnData.length; i < fLen; i++)
            {
                returnData[i]['attrs']['FCODE'] = returnData[i]['attrs']['F_CODE'];
                delete returnData[i]['attrs']['F_CODE'];

                // Now make sure that we have a valid feature _before_ trying to validate and jam it into the list of
                // features to return
                var gFcode = gType + returnData[i]['attrs']['FCODE'];
                if (mgcp.layerNameLookup[gFcode.toUpperCase()])
                {
                    // Validate attrs: remove all that are not supposed to be part of a feature
                    mgcp.validateAttrs(geometryType,returnData[i]['attrs']);

                    returnData[i]['tableName'] = mgcp.layerNameLookup[gFcode.toUpperCase()];
                }
                else
                {
                    // Debug
                    // print('## Skipping: ' + gFcode);
                    returnData.splice(i,1);
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
                    return returnData;
                }
                else
                {
                    //throw new Error(geometryType.toString() + ' geometry is not valid for F_CODE ' + attrs.F_CODE);
                    returnData.push({attrs:{'error':geometryType + ' geometry is not valid for ' + attrs.F_CODE + ' in MGCP TRD4'}, tableName: ''});
                    return returnData;
                }
            }

            hoot.logTrace('FCODE and Geometry: ' + tableName + ' is not in the schema');

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

            // var str = JSON.stringify(tags);
            var str = JSON.stringify(tags,Object.keys(tags).sort());

            // Shapefiles can't handle fields > 254 chars
            // If the tags are > 254 char, split into pieces. Not pretty but stops errors
            // A nicer thing would be to arrange the tags until they fit neatly
            if (str.length < 255 || mgcp.configOut.OgrSplitO2s == 'false')
            {
                //return {attrs:{tag1:str}, tableName: tableName};
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

                // Now split the text across the available tags
                attrs = {tag1:str.substring(0,253),
                        tag2:str.substring(253,506),
                        tag3:str.substring(506,759),
                        tag4:str.substring(759,1012)};
            }

            returnData.push({attrs: attrs, tableName: tableName});
        } // End We DON'T have a feature

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

    } // End of toMgcp

} // End of mgcp
