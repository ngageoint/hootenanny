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
var uuid = require('node-uuid');
var translate = require('./translate.js');
var fcodeCommon = require('./fcode_common.js');
var rules = require('./mgcp_rules.js');
var schema = require('./mgcp_schema.js');
var rawSchema;
var fcodeLookup = {};
var lookup = {};
var mgcpPreRules;
var ignoreList;
var mgcpPostRules;
var osmPostRules;

var self = module.exports = {
    fcodeLookup: fcodeLookup,

    getDbSchema: function() {

    rawSchema = schema.getDbSchema(); // This is <GLOBAL> so we can access it from other areas

    // Build the MGCP fcode/attrs lookup table. Note: This is <GLOBAL>
    mgcpAttrLookup = translate.makeAttrLookup(rawSchema);

    // Now build the FCODE/layername lookup table. Note: This is <GLOBAL>
    layerNameLookup = translate.makeLayerNameLookup(rawSchema);

    // Now add an o2s[A,L,P] feature to the rawSchema
    // We can drop features but this is a nice way to see what we would drop
    rawSchema = translate.addEmptyFeature(rawSchema);

    // Add empty Review layers
    rawSchema = translate.addReviewFeature(rawSchema);

    // Add empty "extra" feature layers if needed
    if (config.OgrMgcpExtra == 'file') rawSchema = translate.addExtraFeature(rawSchema);

    // This function dumps the schema to the screen for debugging
    // translate.dumpSchema(rawSchema);

    return rawSchema;
    }, // End of getDbSchema


    // validateAttrs - Clean up the supplied attr list by dropping anything that should not be part of the
    //                 feature.
    validateAttrs: function(geometryType,attrs) {

        var attrList = mgcpAttrLookup[geometryType.toString().charAt(0) + attrs.FCODE];

        if (attrList != undefined)
        {
            // The code is duplicated but it is quicker than doing the "if" on each iteration
            if (config.OgrDebugDumpvalidate)
            {
                for (var val in attrs)
                {
                    if (attrList.indexOf(val) == -1)
                    {
                        console.warn('Validate: Dropping ' + val + ' from ' + attrs.FCODE);
                        delete attrs[val];

                        // Since we deleted the attribute, Skip the text check
                        continue;
                    }

                    // Now check the length of the text fields
                    // We need more info from the customer about this: What to do if it is too long
                    if (val in rules.txtLength)
                    {
                        if (attrs[val].length > rules.txtLength[val])
                        {
                            // First try splitting the attribute and grabbing the first value
                            var tStr = attrs[val].split(';');
                            if (tStr[0].length <= rules.txtLength[val])
                            {
                                attrs[val] = tStr[0];
                            }
                            else
                            {
                                console.warn('Validate: Attribute ' + val + ' is ' + attrs[val].length + ' characters long. Truncating to ' + rules.txtLength[val] + ' characters.');
                                // Still too long. Chop to the maximum length.
                                attrs[val] = tStr[0].substring(0,rules.txtLength[val]);
                            }
                        } // End text attr length > max length
                    } // End in txtLength
                }
            }
            else
            {
                for (var val in attrs)
                {
                    if (attrList.indexOf(val) == -1)
                    {
                        delete attrs[val];

                        // Since we deleted the attribute, Skip the text check
                        continue;
                    }

                    // Now check the length of the text fields
                    // We need more info from the customer about this: What to do if it is too long
                    if (val in rules.txtLength)
                    {
                        if (attrs[val].length > rules.txtLength[val])
                        {
                            // First try splitting the attribute and grabbing the first value
                            var tStr = attrs[val].split(';');
                            if (tStr[0].length <= rules.txtLength[val])
                            {
                                attrs[val] = tStr[0];
                            }
                            else
                            {
                                console.warn('Validate: Attribute ' + val + ' is ' + attrs[val].length + ' characters long. Truncating to ' + rules.txtLength[val] + ' characters.');
                                // Still too long. Chop to the maximum length.
                                attrs[val] = tStr[0].substring(0,rules.txtLength[val]);
                            }
                        } // End text attr length > max length
                    } // End in txtLength
                }
            } // End getOgrDebugDumpvalidate
        }
        else
        {
            console.warn('Validate: No attrList for ' + attrs.FCODE + ' ' + geometryType);
        }

        // No quick and easy way to do this unless we build yet another lookup table
        var feature = {};

        for (var i=0, sLen = rawSchema.length; i < sLen; i++)
        {
            if (rawSchema[i].fcode == attrs.FCODE && rawSchema[i].geom == geometryType)
            {
                feature = rawSchema[i];
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
                if (config.OgrDebugDumpvalidate) console.warn('Validate: Enumerated Value: ' + attrValue + ' not found in ' + enumName);

                var othVal = '(' + enumName + ':' + attrValue + ')';

                // Do we have an "Other" value?
                if (enumValueList.indexOf('999') == -1)
                {
                    // No: Set the offending enumerated value to the default value
                    attrs[enumName] = feature.columns[i].defValue;

                    debug('Validate: Enumerated Value: ' + attrValue + ' not found in ' + enumName + ' Setting ' + enumName + ' to its default value (' + feature.columns[i].defValue + ')');
                }
                else
                {
                    // Yes: Set the offending enumerated value to the "other" value
                    attrs[enumName] = '999';

                    debug('Validate: Enumerated Value: ' + attrValue + ' not found in ' + enumName + ' Setting ' + enumName + ' to Other (999)');
                }
            }
        } // End Validate Enumerations

    }, // End validateAttrs


    // Sort out if we need to return more than one feature.
    // This is generally for Roads, Railways, bridges, tunnels etc.
    manyFeatures: function(geometryType, tags, attrs)
    {
        var newfeatures = [];

        // Add the first feature to the structure that we return
        var returnData = [{attrs:attrs, tableName:''}];

        // Sort out Roads, Railways, Bridges, Tunnels, Embankments and Cuttings.
        if (geometryType == 'Line' && (tags.highway || tags.railway))
        {
            // var tagList = ['bridge','tunnel','embankment','ford','cutting'];
            var tagList = ['bridge','tunnel','embankment','cutting'];

            // 1. Look at the fcodes
            // Bridge, Tunnel, Ford, Embankment, Cut
            if (['AQ040','AQ130','BH070','DB090','DB070'].indexOf(attrs.F_CODE) > -1)
            {
                var nTags = JSON.parse(JSON.stringify(tags));
                delete nTags.uuid;

                // Roads can go over a Ford, Railways can't
                tagList.push('ford');

                for (var i in tagList)
                {
                    if (nTags[tagList[i]] && nTags[tagList[i]] !== 'no')
                    {
                        delete nTags[tagList[i]];
                    }
                } // End for tag list

                newfeatures.push({attrs: {}, tags: nTags});
            }
            // Now look for road type features
            // Road, Cart Track, Trail
            else if (['AP030','AP010','AP050'].indexOf(attrs.F_CODE) > -1)
            {
                // Roads can go over a Ford, Railways can't
                tagList.push('ford');

                for (var i in tagList)
                {
                    if (tags[tagList[i]] && tags[tagList[i]] !== 'no') // We have one of these...
                    {
                        var nTags = JSON.parse(JSON.stringify(tags));
                        delete nTags.uuid;

                        if (nTags.highway) // Paranoid.....
                        {
                            delete nTags.highway;
                        }

                        if (attrs.FCODE == 'AP050') // Trail
                        {
                            newfeatures.push({attrs: {'TRS':'9'}, tags: nTags}); // TRS:9 = Pedestrian
                        }
                        else
                        {
                            newfeatures.push({attrs: {'TRS':'13'}, tags: nTags}); // TRS:13 = Road
                        }

                        break;
                    }
                }
            }
            // Now look for Railways
            else if(['AN010','AN050'].indexOf(attrs.F_CODE) > -1)
            {
                for (var i in tagList)
                {
                    if (tags[tagList[i]] && tags[tagList[i]] !== 'no') // We have one of these...
                    {
                        var nTags = JSON.parse(JSON.stringify(tags));
                        delete nTags.uuid;

                        if (nTags.railway) // Paranoid.....
                        {
                            delete nTags.railway;
                        }
                        newfeatures.push({attrs: {'TRS':'12'}, tags: nTags}); // TRS:12 = Rail
                        break;
                    }
                }

            } // End Railway


        } // End sort out Road, Railway, Bridge and Tunnel

        // Loop through the new features and process them.
        // Note: This is the same as we did for the main feature.
        for (var i = 0, nFeat = newfeatures.length; i < nFeat; i++)
        {
            // pre processing
            self.applyToMgcpPreProcessing(newfeatures[i]['tags'], newfeatures[i]['attrs'], geometryType);

            var notUsedTags = (JSON.parse(JSON.stringify(tags)));

            // apply the simple number and text biased rules
            // Note: These are BACKWARD, not forward!
//             translate.applySimpleNumBiased(newfeatures[i]['attrs'], newfeatures[i]['tags'], rules.numBiased, 'backward',rules.intList);
//             translate.applySimpleTxtBiased(newfeatures[i]['attrs'], newfeatures[i]['tags'], rules.txtBiased, 'backward');
            translate.applySimpleNumBiased(newfeatures[i]['attrs'], notUsedTags, rules.numBiased, 'backward',rules.intList);
            translate.applySimpleTxtBiased(newfeatures[i]['attrs'], notUsedTags, rules.txtBiased, 'backward');

            // one 2 one - we call the version that knows about OTH fields
//             translate.applyOne2One(newfeatures[i]['tags'], newfeatures[i]['attrs'], lookup, fcodeLookup.toMgcp);
            translate.applyOne2One(notUsedTags, newfeatures[i]['attrs'], lookup.toMgcp, fcodeLookup.toMgcp);

            // post processing
            applyToMgcpPostProcessing(newfeatures[i]['tags'], newfeatures[i]['attrs'], geometryType,notUsedTags);

            returnData.push({attrs: newfeatures[i]['attrs'],tableName: ''});
        }

        return returnData;
    }, // End manyFeatures


    // ##### Start of the xxToOsmxx Block #####
    applyToOsmPreProcessing: function(attrs, layerName, geometryType)
    {
        // Drop the FCSUBTYPE since we don't use it
        if (attrs.FCSUBTYPE) delete attrs.FCSUBTYPE;

        // The swap list. These are the same attr, just named differently
        // These may get converted back on output.
        var swapList = {
                'CPYRT_NOTE':'CCN',
                'SRC_INFO':'SDP',
                'SRC_DATE':'SDV',
                'SMC':'MCC'
                };

        // List of data values to drop/ignore
        ignoreList = { '-32765':1, '-32767':1, '-32768':1,
                           '998':1, '-999999':1,
                           'n_a':1, 'noinformation':1, 'unknown':1, 'unk':1 };

        // Unit conversion. Some attributes are in centimetres, others in decimetres
        // var unitList = { 'GAW':100, 'HCA':10, 'WD1':10, 'WD2':10, 'WD3':10, 'WT2':10 };

        // make sure all columns are upper case. This simplifies translation.
        for (var col in attrs)
        {
            // slightly ugly but we would like to account for: 'No Information', 'noInformation' etc
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
                ['AP020', ['ap020','interchange_l', 'interchangel']], // Interchange
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

    }, // End of applyToOsmPreProcessing

    // Post Processing: Lots of cleanup
    applyToOsmPostProcessing : function (attrs, tags, layerName, geometryType)
    {
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
            if (attrs.ACC == '2')
            {
                accuracy = accuracy * 2;
                // Add note: Accuracy: Approximate
            }
            else if (attrs.ACC == '3')
            {
                accuracy = accuracy * 4;
                // Add note: Accuracy: Doubtful
            }
            else if (attrs.ACC == '5')
            {
                accuracy = -1;
                // Add note: Accuracy: Disputed
            }
            else if (attrs.ACC == '6')
            {
                // Add note: Accuracy: Undisputed
            }
            else if (attrs.ACC == '7')
            {
                // Add note: Accuracy: Precise
            }
            else if (attrs.ACC == '8')
            {
                accuracy = -1;
                // Add note: Accuracy: Abrogated
            }
        }

        if (accuracy > -1)
        {
            tags['error:circular'] = accuracy;
        }

        // #####
        // #
        // # Now the funky rules start. These account for common tables that are shared between various
        // # features
        // #
        // #####

        if (attrs.HWT && attrs.HWT !== '0') tags.amenity = 'place_of_worship';

        // Add the LayerName to the source
        if ((! tags.source) && layerName !== '') tags.source = 'mgcp:' + layerName.toLowerCase();

        // If we have a UID, store it
        if (tags.uuid)
        {
            tags.uuid = '{' + tags['uuid'].toString().toLowerCase()  + '}'
        }
        else
        {
            tags.uuid = uuid.v4();
        }

        // Railway Yard
        // if (attrs.F_CODE == 'AN060' && tags.landuse =='railway') tags.service = 'yard';

        // Communications Tower
        // if (attrs.F_CODE == 'AT080') tags['tower:type'] = 'communication';

        // Railway vs Road
        // if (attrs.F_CODE == 'AN010' && attrs.RRC =='0') tags.railway = 'yes';

        if (osmPostRules == undefined)
        {
            // "New" style complex rules
            //
            // Rules format:  ["test expression","output result"];
            // Note: t = tags, a = attrs and attrs can only be on the RHS
            var rulesList = [
            ["t['bridge:movable'] && t['bridge:movable'] !== 'no' && t['bridge:movable'] !== 'unknown'","t.bridge = 'movable'"],
            ["t['cable:type'] && !(t.cable)","t.cable = 'yes'"],
            ["t['generator:source'] == 'wind'","t.power = 'generator'"],
            ["t.waterway == 'flow_control'","t.flow_control = 'sluice_gate'"],
            ["(t.landuse == 'built_up_area' || t.place == 'settlement') && t.building","t['settlement:type'] = t.building; delete t.building"],
            ["t['monitoring:weather'] == 'yes'","t.man_made = 'monitoring_station'"],
            ["t['building:religious'] == 'other'","t.amenity = 'religion'"],
            ["t.religion","t.landuse = 'cemetery'"],
            ["t.public_transport == 'station'","t.bus = 'yes'"],
            ["t.leisure == 'stadium'","t.building = 'yes'"],
            ["t['tower:type'] && !(t.man_made)","t.man_made = 'tower'"],
            ["t['social_facility:for'] == 'senior'","t.amenity = 'social_facility'; t.social_facility = 'group_home'"],
            ["t.control_tower == 'yes'","t['tower:type'] = 'observation'; t.use = 'air_traffic_control'"]
            ];

            osmPostRules = translate.buildComplexRules(rulesList);
        }

        // translate.applyComplexRules(tags,attrs,rulesList);
        translate.applyComplexRules(tags,attrs,osmPostRules);

        translate.fixConstruction(tags, 'highway');
        translate.fixConstruction(tags, 'railway');

        // Add 'building = yes' to amenities if we don't already have one
        if (tags.amenity && !tags.building)
        {
            // Debug:
            // print('Added building');
            // Don't add building=yes to built up areas!
            if (!tags.place) tags.building = 'yes';
        }

        // Add 'building = yes' to military if it isn't a range
        if (tags.military && !tags.building)
        {
            // Debug:
            // print('Added building to military');
            if (tags.military !== 'range') tags.building = 'yes';
        }

        // if (tags.building == 'train_station' && !tags.railway) tags.railway = 'station';
        // if ('ford' in tags && !tags.highway) tags.highway = 'road';

        switch (attrs.F_CODE)
        {
            case undefined: // Break early if no value
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
                break;

            case 'BH070': // Ford
                // Fords are also supposed to be roads.
                if (geometryType == 'Line' && !tags.highway) tags.highway = 'road';
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
                    if (tags[tTags[i]]) console.warn('Unpacking TXT, overwriting ' + i + ' = ' + tags[i] + '  with ' + tTags[i]);
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
            if (i.indexOf('abandoned:') !== -1)
            {
                // Hopeing there is only one ':' in the tag name...
                var tList = i.split(':');
                tags[tList[1]] = tags[i];
                tags.condition = 'abandoned';
                delete tags[i];
                continue;
            }
        }

        if (mgcpPreRules == undefined)
        {
            // See ToOsmPostProcessing for more details about rulesList.
            var rulesList = [
            ["t.amenity == 'marketplace'","t.facility = 'yes'"],
            ["t.construction && t.railway","t.railway = t.construction; t.condition = 'construction'; delete t.construction"],
            ["t.construction && t.highway","t.highway = t.construction; t.condition = 'construction'; delete t.construction"],
            ["t.landuse == 'allotments'","t.landuse = 'farmland'"],
            ["t.landuse == 'construction'","t.landuse = 'built_up_area'; t.condition = 'construction'"],
            ["t.landuse == 'military'","t.military = 'installation'; delete t.landuse"],
            ["t.landuse == 'farm'","t.landuse = 'farmland'"],
            ["t.landuse == 'farmyard'","t.facility = 'yes'; t.use = 'agriculture'; delete t.landuse"],
            ["t.landuse == 'grass' || t.landuse == 'meadow'","t.natural = 'grassland'; t['grassland:type'] = 'grassland'; delete t.landuse"],
            ["t.landuse == 'retail'","t.landuse = 'built_up_area'; t.use = 'commercial'"],
            ["t.leisure == 'stadium' && t.building","delete t.building"],
            ["t.man_made == 'water_tower'","a.F_CODE = 'AL241'"],
            ["t.natural == 'scrub'","t.natural = 'grassland'; t['grassland:type'] = 'grassland_with_trees'"],
            ["t.natural == 'wood'","t.landuse = 'forest'"],
            ["t.power == 'generator'","a.F_CODE = 'AL015'; t.use = 'power_generation'"],
            ["t.power == 'line'","t['cable:type'] = 'power'; t.cable = 'yes'"],
            ["t.rapids == 'yes'","t.waterway = 'rapids'"],
            ["t.resource","t.product = t.resource; delete t.resource"],
            ["t.route == 'road' && !(t.highway)","t.highway = 'road'; delete t.route"],
            ["(t.shop || t.office) && !(t.building)","a.F_CODE = 'AL015'"],
            ["t.tunnel == 'building_passage'","t.tunnel = 'yes'"],
            ["!(t.water) && t.natural == 'water'","t.water = 'lake'"],
            ["t.water == 'pond'","a.F_CODE = 'BH170'; t.natural = 'other_pool_type'"],
            ["t.water == 'river'","t.waterway = 'river'"],
            ["t.waterway == 'riverbank'","t.waterway = 'river'"]
            ];

            mgcpPreRules = translate.buildComplexRules(rulesList);
        }

        // Apply the rulesList.
        // translate.applyComplexRules(tags,attrs,rulesList);
        translate.applyComplexRules(tags,attrs,mgcpPreRules);

        // Fix up OSM 'walls' around facilities
        if (tags.barrier == 'wall' && geometryType == 'Area')
        {
            attrs.F_CODE = 'AL010'; // Facility
            delete tags.barrier; // Take away the walls...
        }


        // An "amenitiy" can be a building or a thing.
        // If appropriate, make the "amenity" into a building.
        // This list is taken from the OSM Wiki and Taginfo
        var notBuildingList = [
            'bbq','biergarten','drinking_water','bicycle_parking','bicycle_rental','boat_sharing',
            'car_sharing','charging_station','grit_bin','parking','parking_entrance','parking_space',
            'taxi','atm','fountain','bench','clock','hunting_stand','marketplace','post_box',
            'recycling', 'vending_machine','waste_disposal','watering_place','water_point',
            'waste_basket','drinking_water','swimming_pool','fire_hydrant','emergency_phone','yes',
            'compressed_air','water','nameplate','picnic_table','life_ring','grass_strip','dog_bin',
            'artwork','dog_waste_bin','street_light','park','hydrant','tricycle_station','loading_dock',
            'trailer_park','game_feeding'
            ]; // End bldArray

        if (tags.amenity && notBuildingList.indexOf(tags.amenity) == -1 && !tags.building) attrs.F_CODE = 'AL015';

        // Going out on a limb and processing OSM specific tags:
        // - Building == a thing,
        // - Amenity == The area around a thing. Except for the ones that are not.....
        // If it is a Point feature, it becomes a building. Polygon features become Facilities
        // Line features become buildings and then get ignored.
        var facilityList = {'school':'850','hospital':'860','university':'850'};
        if (tags.amenity in facilityList)
        {
            if (geometryType == 'Area')
            {
                attrs.F_CODE = 'AL010'; // Facility
            }
            else
            {
                attrs.F_CODE = 'AL015'; // Building
            }

            // If we don't have a Feature Function then assign one.
            if (!attrs.FFN) attrs.FFN = facilityList[tags.amenity];
        }

        // Cutlines and Highways.
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

        // Keep looking for an FCODE
        // This uses the fcodeLookup tables that are defined earlier
        // var fcodeLookup.toMgcp = translate.createLookup(fcodeList);
        if (!attrs.F_CODE)
        {
            for (var col in tags)
            {
                var value = tags[col];
                if (col in fcodeLookup.toMgcp)
                {
                    if (value in fcodeLookup.toMgcp[col])
                    {
                        var row = fcodeLookup.toMgcp[col][value];
                        attrs.F_CODE = row[1];
                    }
                }
            }
        }

        // The FCODE for Buildings changed...
        if (attrs.F_CODE == 'AL013') attrs.F_CODE = 'AL015';

    }, // End applyToMgcpPreProcessing


    applyToMgcpPostProcessing : function (tags, attrs, geometryType, notUsedTags)
    {
        // Gross generalisation. If we don't have an FCODE but we do have an FFN then we either have a
        // Building or a Facility.
        // In the absence of any other info, we are making it a Building
        if (!attrs.F_CODE)
        {
            if (attrs.FFN) attrs.F_CODE = 'AL015';
        }

        // If we still don't have an FCODE, try a bit of brute force
        if (!attrs.F_CODE)
        {
            var fcodeMap = {
                'highway':'AP030', 'railway':'AN010', 'building':'AL015',
                'ford':'BH070', 'waterway':'BH140', 'bridge':'AQ040',
            };

            for (var i in fcodeMap)
            {
                if (i in tags)
                {
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
        else
        {
            attrs.UID = uuid.v4().replace('{','').replace('}','');
        }

        // Default railway
        // if (attrs.F_CODE == 'AN010' && tags.railway == 'yes') attrs.RRC = '0';

        // Not a great fit.
        if (tags.public_transportation == 'station' && tags.bus == 'yes') attrs.FFN = '481';

        // Mapping Senior Citizens home to Accomodation. Not great
        if (tags.amenity == 'social_facility' && tags['social_facility:for'] == 'senior') attrs.FFN = 550;

        // Unknown House of Worship
        if (tags.amenity == 'place_of_worship' && tags.building == 'other') attrs.HWT = 999;

        // These FCODES have "No prescribed attributes" in TRDv40
        // Therefore:
        // - clean up the the attrs.
        // - JSON the tags and stick them in a text field
        var noAttrList = [ 'AJ010', 'AJ030', 'AK170', 'AL019', 'AL070', 'AL099', 'AN076',
                           'BD100', 'BD180', 'BH165', 'BI010', 'BJ020', 'BJ031', 'BJ110',
                           'DB061', 'DB071', 'DB100',
                           'EA020', 'EA055', 'EC010', 'EC040', 'EC060',
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

                // Single lane roads dont have a median and are not separated.
                // NOTE: This could cause problems.
                if (attrs.LTN == '1')
                {
                    attrs.SEP = '1000';
                    attrs.MES = '1000';
                }
                break;

            case 'AP030': // Road
                if (tags.bridge) attrs.LOC = '45'; // Above Surface
                if (tags.tunnel) attrs.LOC = '40'; // Below Surface
                if (tags.embankment || tags.man_made == 'causeway') attrs.LOC = '44'; // On Surface
                if (attrs.RST == '6') attrs.RST = '2'; // Move 'ground' to 'unpaved'
                break;

            case 'AP050': // Trail
                // Apparently, AP050 (Trail) _doesn't_ have WTC=1 (All weather)
                if (attrs.WTC == '1') attrs.WTC = '0';
                break;

            case 'AQ040': // Bridge
                if (attrs.RST)
                {
                    // console.warn('Found RST = ' + attrsi.RST + ' in AQ040'); // Should not get this
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

            case 'DA010': // Bluff/Cliff/Escarpment
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
                // debug('TRD3 feature EA010 changed to TRD4 EA040 - some data has been dropped');
                break;
        } // End switch FCODE

        if (mgcpPostRules == undefined)
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
            ["t.sport == 'tennis'","a.F_CODE = 'AK040'"],
            ["t.natural == 'tree'","a.F_CODE = 'EC030'"],
            ["t.amenity == 'ferry_terminal'","a.F_CODE = 'AQ125'; a.FFN = '7'"],
            // ["t.landuse == 'forest'","a.F_CODE = 'EC030'"],
            ["t.amenity == 'fuel'","a.F_CODE = 'AL015'"]
            ];

            mgcpPostRules = translate.buildComplexRules(rulesList);
        }

        // translate.applyComplexRules(tags,attrs,rulesList);
        translate.applyComplexRules(tags,attrs,mgcpPostRules);

        // Fix up SRT values so we comply with the spec. These values came from data files
        // Format is: orig:new
        srtFix = {
            28:112,  // Ikonos Imagery -> Hires Commercial
            66:112,  // Quickbird Multi 2.44m -> Hires Commercial
            68:110,  // Quickbird Pan 60cm -> Very Hires Commercial
        }; // End srtFix

        if (attrs.SRT in srtFix) attrs.SRT = srtFix[attrs.SRT];

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

        // Debug:
        if (config.OgrDebugDumptags)
        {
            print('In Layername: ' + layerName);
            var kList = Object.keys(attrs).sort()
            for (var i = 0, fLen = kList.length; i < fLen; i++) print('In Attrs: ' + kList[i] + ': :' + attrs[kList[i]] + ':');
        }

        // Set up the fcode translation rules
        if (fcodeLookup.toOsm == undefined)
        {
            // Order is important:
            // First the MGCPv3 & 4 FCODES, then the common ones. This ensures that the common ones don't
            // stomp on the other ones.
            rules.fcodeOne2oneV4.push.apply(rules.fcodeOne2oneV4,rules.fcodeOne2oneInV3);
            rules.fcodeOne2oneV4.push.apply(rules.fcodeOne2oneV4,fcodeCommon.one2one);

            fcodeLookup.toOsm = translate.createLookup(rules.fcodeOne2oneV4);

            // Debug:
            // translate.dumpOne2OneLookup(fcodeLookup.toOsm);
        }
        if (lookup.toOsm == undefined)
        {
            // Setup lookup tables to make translation easier.

            // Add the MGCPv3.0 specific attributes to the v4.0/common attribute table
            rules.one2one.push.apply(rules.one2one,rules.one2oneIn);

            lookup.toOsm = translate.createLookup(rules.one2one);
        }

        // pre processing
        self.applyToOsmPreProcessing(attrs, layerName, geometryType);

        // Use the FCODE to add some tags.
        if (attrs.F_CODE)
        {
            var ftag = fcodeLookup.toOsm['F_CODE'][attrs.F_CODE];
            if (ftag)
            {
                tags[ftag[0]] = ftag[1];
                // Debug: Dump out the tags from the FCODE
                // print('FCODE: ' + attrs.F_CODE + ' tag=' + ftag[0] + '  value=' + ftag[1]);
            }
            else
            {
                debug('Translation for FCODE ' + attrs.F_CODE + ' not found');
            }
        }

        // Make a copy of the input attributes so we can remove them as they get translated. Looking at what
        // isn't used in the translation - this should end up empty.
        // not in v8 yet: // var tTags = Object.assign({},tags);
        var notUsedAttrs = (JSON.parse(JSON.stringify(attrs)));
        delete notUsedAttrs.F_CODE;

        // apply the simple number and text biased rules
        // NOTE: We are not using the intList paramater for applySimpleNumBiased when going to OSM.
        translate.applySimpleNumBiased(notUsedAttrs, tags, rules.numBiased, 'forward',[]);
        translate.applySimpleTxtBiased(notUsedAttrs, tags,  rules.txtBiased,'forward');

        // one 2 one
        translate.applyOne2One(notUsedAttrs, tags, lookup.toOsm, {'k':'v'});

        // post processing
        self.applyToOsmPostProcessing(attrs, tags, layerName, geometryType);

        // Debug
        // for (var i in notUsedAttrs) print('NotUsed: ' + i + ': :' + notUsedAttrs[i] + ':');

        // Debug: Add the FCODE to the tags
        if (config.OgrDebugAddfcode) tags['raw:debugFcode'] = attrs.F_CODE;

        // Debug:
        if (config.OgrDebugDumptags)
        {
            var kList = Object.keys(tags).sort()
            for (var i = 0, fLen = kList.length; i < fLen; i++) print('Out Tags: ' + kList[i] + ': :' + tags[kList[i]] + ':');
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

        // Check if we have a schema. This is a quick way to workout if various lookup tables have been built
        if (rawSchema == undefined)
        {
            var tmp_schema = self.getDbSchema();
        }

        // The Nuke Option: If we have a relation, drop the feature and carry on
        if (tags['building:part']) return null;

        // The Nuke Option: "Collections" are groups of different feature types: Point, Area and Line.
        // There is no way we can translate these to a single TDS feature.
        if (geometryType == 'Collection') return null;

        // Debug:
        if (config.OgrDebugDumptags)
        {
            print('In Geometry: ' + geometryType + '  In Element Type: ' + elementType);
            var kList = Object.keys(tags).sort()
            for (var i = 0, fLen = kList.length; i < fLen; i++) print('In Tags: ' + kList[i] + ': :' + tags[kList[i]] + ':');
        }

        // Set up the fcode translation rules
        if (fcodeLookup.toMgcp == undefined)
        {
            // Order is important:
            // First the MGCPv4 FCODES, then the common ones. This ensures that the common ones don't
            // stomp on the V4 ones.
            rules.fcodeOne2oneV4.push.apply(rules.fcodeOne2oneV4,fcodeCommon.one2one);
            rules.fcodeOne2oneV4.push.apply(rules.fcodeOne2oneV4,rules.fcodeOne2oneOut);

            fcodeLookup.toMgcp = translate.createBackwardsLookup(rules.fcodeOne2oneV4);
        }

        if (lookup.toMgcp == undefined)
        {
            // Add the conversion from MGCPv3.0 attributes to the v4.0/common attribute table
            rules.one2one.push.apply(rules.one2one,rules.one2oneOut);

            lookup.toMgcp = translate.createBackwardsLookup(rules.one2one);

            // Debug
            // translate.dumpOne2OneLookup(lookup.toMgcp);
        }

        // pre processing
        self.applyToMgcpPreProcessing(tags,attrs, geometryType);

        // Make a copy of the input tags so we can remove them as they get translated. What is left is
        // the not used tags.
        // not in v8 yet: // var tTags = Object.assign({},tags);
        var notUsedTags = (JSON.parse(JSON.stringify(tags)));

        if (notUsedTags.hoot) delete notUsedTags.hoot; // Added by the UI

        // apply the simple number and text biased rules
        translate.applySimpleNumBiased(attrs, notUsedTags, rules.numBiased, 'backward',rules.intList);
        translate.applySimpleTxtBiased(attrs, notUsedTags,  rules.txtBiased,'backward');

        // one 2 one
        translate.applyOne2One(notUsedTags, attrs, lookup.toMgcp, fcodeLookup.toMgcp, ignoreList);

        // post processing
        // applyToMgcpPostProcessing(attrs, tableName, geometryType);
        self.applyToMgcpPostProcessing(tags, attrs, geometryType, notUsedTags);

        // Debug
        // for (var i in notUsedTags) print('NotUsed: ' + i + ': :' + notUsedTags[i] + ':');

        // If we have unused tags, add them to the TXT field.
        // NOTE: We are not checking if this is longer than 255 characters
        if (Object.keys(notUsedTags).length > 0 && config.OgrMgcpExtra == 'note')
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
        if (!layerNameLookup[tableName])
        {
            // For the UI: Throw an error and die if we don't have a valid feature
            if (config.OgrThrowError)
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

            debug('FCODE and Geometry: ' + tableName + ' is not in the schema');

            if (config.OgrPartialTranslate)
            {
                tableName = 'Partial';
                attrs.FCODE = 'Partial';
                delete attrs.F_CODE;

                // If we have unused tags, add them to partial feature.
                if (Object.keys(notUsedTags).length > 0)
                {
                    for (var i in notUsedTags)
                    {
                        attrs['OSM:' + i] = notUsedTags[i];
                    }
                }
            }
            else
            {
                tableName = 'o2s_' + geometryType.toString().charAt(0);

                // Dump out what attributes we have converted before they get wiped out
                if (config.OgrDebugDumptags) for (var i in attrs) print('Converted Attrs:' + i + ': :' + attrs[i] + ':');

                for (var i in tags)
                {
                    // Clean out all of the "source:XXX" tags to save space
                    if (i.indexOf('source:') !== -1) delete tags[i];
                    if (i.indexOf('error:') !== -1) delete tags[i];
                    if (i.indexOf('hoot:') !== -1) delete tags[i];
                }

                var str = JSON.stringify(tags);

                // Shapefiles can't handle fields > 254 chars.
                // If the tags are > 254 char, split into pieces. Not pretty but stops errors.
                // A nicer thing would be to arrange the tags until they fit neatly
                if (str.length < 255 || !config.OgrSplitO2s)
                {
                    // return {attrs:{tag1:str}, tableName: tableName};
                    attrs = {tag1:str};
                }
                else
                {
                    // Not good. Will fix with the rewrite of the tag splitting code
                    if (str.length > 1012)
                    {
                        debug('o2s tags truncated to fit in available space.');
                        str = str.substring(0,1012);
                    }

                    // Now split the text across the available tags
                    attrs = {tag1:str.substring(0,253),
                            tag2:str.substring(253,506),
                            tag3:str.substring(506,759),
                            tag4:str.substring(759,1012)};
                }
            }

             returnData.push({attrs: attrs, tableName: tableName});
        }
        else // We have a feature
        {
            // Check if we need to make a second feature
            // NOTE: This returns structure we are going to send back to Hoot:  {attrs: attrs, tableName: 'Name'}
            // attrs2 = twoFeatures(geometryType,tags,attrs);
            returnData = self.manyFeatures(geometryType,tags,attrs);

            // Now go through the features and clean them up.
            var gType = geometryType.toString().charAt(0);

            for (var i = 0, fLen = returnData.length; i < fLen; i++)
            {
                returnData[i]['attrs']['FCODE'] = returnData[i]['attrs']['F_CODE'];
                delete returnData[i]['attrs']['F_CODE'];

                 // Validate attrs: remove all that are not supposed to be part of a feature
                self.validateAttrs(geometryType,returnData[i]['attrs']);

                var gFcode = gType + returnData[i]['attrs']['FCODE'];
                returnData[i]['tableName'] = layerNameLookup[gFcode.toUpperCase()];

            } // End returnData loop

            // If we have unused tags, throw them into the "extra" layer
            if (Object.keys(notUsedTags).length > 0 && config.OgrMgcpExtra == 'file')
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

        // Debug:
        if (config.OgrDebugDumptags)
        {
            for (var i = 0, fLen = returnData.length; i < fLen; i++)
            {
                print('TableName ' + i + ': ' + returnData[i]['tableName'] + '  FCode: ' + returnData[i]['attrs']['FCODE'] + '  Geom: ' + geometryType);
                // for (var j in returnData[i]['attrs']) print('Out Attrs:' + j + ': :' + returnData[i]['attrs'][j] + ':');
                var kList = Object.keys(returnData[i]['attrs']).sort()
                for (var j = 0, kLen = kList.length; j < kLen; j++) print('Out Attrs:' + kList[j] + ': :' + returnData[i]['attrs'][kList[j]] + ':');
            }
            print('');
        }

        return returnData;

    } // End of toMgcp

} // End of mgcp
