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
        MGCP -> OSM, and
        OSM -> MGCP

    Based on TableExample.js script by Jason S.

    Possible attribute values are taken from the MGCP TRD 3.0 and 4.0 specs  with the addition of
    values found in sample data and the DFDD/NFDD v4
*/


mgcp = {
    getDbSchema: function() {

    mgcp.rawSchema = mgcp.schema.getDbSchema(); // This is <GLOBAL> so we can access it from other areas

    // Build the MGCP fcode/attrs lookup table. Note: This is <GLOBAL>
    mgcpAttrLookup = translate.makeAttrLookup(mgcp.rawSchema);

    // Now build the FCODE/layername lookup table. Note: This is <GLOBAL>
    layerNameLookup = translate.makeLayerNameLookup(mgcp.rawSchema);

    // Now add an o2s[A,L,P] feature to the mgcp.rawSchema
    // We can drop features but this is a nice way to see what we would drop
    mgcp.rawSchema = translate.addEmptyFeature(mgcp.rawSchema);

    // This function dumps the schema to the screen for debugging
    // translate.dumpSchema(mgcp.rawSchema);

    return mgcp.rawSchema;
    }, // End of getDbSchema


    // validateAttrs - Clean up the supplied attr list by dropping anything that should not be part of the
    //                 feature.
    validateAttrs: function(geometryType,attrs) {

        var attrList = mgcpAttrLookup[geometryType.toString().charAt(0) + attrs.F_CODE];

        if (attrList != undefined)
        {
            // The code is duplicated but it is quicker than doing the "if" on each iteration
            if (config.getOgrDebugDumpvalidate() == 'true')
            {
                for (var val in attrs)
                {
                    if (attrList.indexOf(val) == -1)
                    {
                        logWarn('Validate: Dropping ' + val + '  from ' + attrs.F_CODE);
                        delete attrs[val];
                    }
                }
            }
            else
            {
                for (var val in attrs)
                {
                    if (attrList.indexOf(val) == -1) delete attrs[val];
                }
            }
        }
        else
        {
            logWarn('Validate: No attrList for ' + attrs.F_CODE + ' ' + geometryType);
        }

        // No quick and easy way to do this unless we build yet another lookup table
        var feature = {};

        for (var i=0, sLen = mgcp.rawSchema.length; i < sLen; i++)
        {
            if (mgcp.rawSchema[i].fcode == attrs.F_CODE && mgcp.rawSchema[i].geom == geometryType)
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
            if (!(attrs[enumName])) continue;

            var attrValue = attrs[enumName];
            var enumList = feature.columns[i].enumerations;
            var enumValueList = [];

            // Pull all of the values out of the enumerated list to make life easier
            for (var j=0, elen = enumList.length; j < elen; j++) enumValueList.push(enumList[j].value);

            // Check if it is a valid enumerated value
            if (enumValueList.indexOf(attrValue) == -1)
            {
                if (config.getOgrDebugDumpvalidate() == 'true') logWarn('Validate: Enumerated Value: ' + attrValue + ' not found in ' + enumName);

                var othVal = '(' + enumName + ':' + attrValue + ')';

                // Do we have an "Other" value?
                if (enumValueList.indexOf('999') == -1)
                {
                    // No: Set the offending enumerated value to the default value
                    attrs[enumName] = feature.columns[i].defValue;

                    logWarn('Validate: Enumerated Value: ' + attrValue + ' not found in ' + enumName + ' Setting ' + enumName + ' to its default value (' + feature.columns[i].defValue + ')');
                }
                else
                {
                    // Yes: Set the offending enumerated value to the "other" value
                    attrs[enumName] = '999';

                    logWarn('Validate: Enumerated Value: ' + attrValue + ' not found in ' + enumName + ' Setting ' + enumName + ' to Other (999)');
                }
            }
        } // End Validate Enumerations

    }, // End validateAttrs


    // Sort out if we need to return two features or one.
    // This is generally for Roads/Railways & bridges but can also be for other features.
    twoFeatures: function(geometryType, tags, attrs)
    {
        var newAttrs = {};

        if (geometryType == 'Line' && tags.bridge == 'yes' && (tags.highway || tags.railway))
        {
            if (attrs.F_CODE !== 'AQ040')
            {
                newAttrs.F_CODE = 'AQ040';
            }
            else
            {
                if (tags.railway)
                {
                    newAttrs.F_CODE = 'AT010';
                }
                else
                {
                    if (tags.highway == 'track')
                    {
                        newAttrs.F_CODE = 'AP010';
                    }
                    else
                    {   // The default is to make it a road
                        newAttrs.F_CODE = 'AP030';
                    }
                }
            }

            // Remove the uuid from the tag list so we get a new one for the second feature
            delete tags.uuid;
        } // End sort out Road, Railway & Bridge


        // If we are making a second feature, process it.
        if (newAttrs.F_CODE)
        {
            // pre processing
            mgcp.applyToMgcpPreProcessing(tags,newAttrs, geometryType);

            // one 2 one
            translate.applyOne2One(tags, newAttrs, mgcp.lookup, mgcp.fcodeLookup, mgcp.ignoreList);

            // apply the simple number and text biased rules
            translate.applySimpleNumBiased(newAttrs, tags, mgcp.rules.numBiased, 'backward');
            translate.applySimpleTxtBiased(newAttrs, tags,  mgcp.rules.txtBiased,'backward');

            // post processing
            // mgcp.applyToMgcpPostProcessing(attrs, tableName, geometryType);
            mgcp.applyToMgcpPostProcessing(tags, newAttrs, geometryType);
        }

        // Debug:
        // for (var i in newAttrs) print('twoFeatures: New Attrs:' + i + ': :' + newAttrs[i] + ':');

        // Return the new attributes
        return newAttrs;
    },


    // ##### Start of the xxToOsmxx Block #####
    applyToOsmPreProcessing: function(attrs, layerName)
    {
        // The swap list. These are the same attr, just named differently
        // These may get converted back on output.
        var swapList = {
                'CPYRT_NOTE':'CCN',
                'SRC_INFO':'SDP',
                'SRC_DATE':'SDV',
                'SMC':'MCC'
                };

        // List of data values to drop/ignore
        var ignoreList = { '-32765':1, '-32767':1, '-32768':1,
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
            // Do nothing
        }
        else if (attrs.FCODE)
        {
            // fcode = attrs['F_CODE'];
            attrs.F_CODE = attrs.FCODE;
            delete attrs.FCODE;
        }
        else
        {
            // Time to find an FCODE based on teh filename
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

    }, // End of applyToOsmPreProcessing

    // Post Processing: Lots of cleanup
    applyToOsmPostProcessing : function (attrs, tags, layerName)
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

        // If we have a House of Worship type then tag it _before_ the complex rules
        if (attrs.HWT && attrs.HWT !== '0') tags.amenity = 'place_of_worship';

        // Add UUID
        tags.source = 'mgcp';
        if (attrs.UID)
        {
            tags.uuid = '{' + attrs.UID + '}'
        }
        else
        {
            tags.uuid = createUuid();
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

            mgcp.osmPostRules = translate.buildComplexRules(rulesList);
        }

        // translate.applyComplexRules(tags,attrs,rulesList);
        translate.applyComplexRules(tags,attrs,mgcp.osmPostRules);

        translate.fixConstruction(tags, 'highway');
        translate.fixConstruction(tags, 'railway');

        // Cache for easy access
        var fCode = attrs.F_CODE;

        // Fix up the 'surface' values for buildings
        if (fCode == 'AL015' && tags.surface == 'unknown') delete tags['surface'];

        // Add 'building = yes' to amenities if we don't already have one
        if (tags.amenity && !(tags.building))
        {
            // Debug:
            // print('Added building');
            // Don't add building=yes to built up areas!
            if (!(tags.place)) tags.building = 'yes';
        }

        // Add 'building = yes' to military if it isn't a range
        if (tags.military && !(tags.building))
        {
            // Debug:
            // print('Added building to military');
            if (tags.military !== 'range') tags.building = 'yes';
        }

        // Add tags if we have Null attributes.  This happens when a feature has an
        // FCODE and no other attributes.  These FCODES don't have default values
        // in the fcode_common lookup table.
        if (fCode == 'AF030' && !(tags['tower:type'])) tags['tower:type'] = 'cooling';
        if (fCode == 'AL015' && !(tags.building)) tags.building = 'yes';
        if (fCode == 'AP020' && !(tags.junction)) tags.junction = 'yes';
        if (fCode == 'AP030' && !(tags.highway)) tags.highway = 'road';
        if (fCode == 'AQ040' && !(tags.bridge)) tags.bridge = 'yes';
        if (fCode == 'BH140' && !(tags.waterway)) tags.waterway = 'river';

        // if (tags.building == 'train_station' && !(tags.railway)) tags.railway = 'station';

    }, // End of applyToOsmPostProcessing

    // ##### Start of the xxToMgcpxx Block #####
    applyToMgcpPreProcessing: function(tags, attrs, geometryType)
    {
        // initial cleanup
        for (var val in tags)
        {
            // Remove empty tags
            if (tags[val] == '')
            {
                delete tags[val];
                continue;
            }

            // Convert "abandoned:XXX" features
            if (val.indexOf('abandoned:') !== -1)
            {
                // Hopeing there is only one ':' in the tag name...
                var tList = val.split(':');
                tags[tList[1]] = tags[val];
                tags.condition = 'abandoned';
                delete tags[val];
            }

        }

        if (mgcp.mgcpPreRules == undefined)
        {
            // See ToOsmPostProcessing for more details about rulesList.
            var rulesList = [
            ["t.construction && t.railway","t.railway = t.construction; t.condition = 'construction'; delete t.construction"],
            ["t.construction && t.highway","t.highway = t.construction; t.condition = 'construction'; delete t.construction"],
            ["t.waterway == 'riverbank'","t.waterway = 'river'"],
            ["t.landuse == 'allotments'","t.landuse = 'farmland'"],
            ["t.leisure == 'stadium' && t.building","delete t.building"],
            ["t.natural == 'wood'","t.landuse = 'forest'"],
            ["!(t.water) && t.natural == 'water'","t.water = 'lake'"],
            ["t.water == 'pond'","a.F_CODE = 'BH170'; t.natural = 'other_pool_type'"],
            ["t.water == 'river'","t.waterway = 'river'"],
            ["t.rapids == 'yes'","t.waterway = 'rapids'"],
            ["t.route == 'road' && !(t.highway)","t.highway = 'road'; delete t.route"],
            ["t.landuse == 'retail'","t.landuse = 'built_up_area'; t.use = 'commercial'"],
            ["t.amenity == 'marketplace'","t.facility = 'yes'"],
            ["t.power == 'line'","t['cable:type'] = 'power'; t.cable = 'yes'"],
            ["t.landuse == 'construction'","t.landuse = 'built_up_area'; t.condition = 'construction'"],
            ["t.landuse == 'military'","t.military = 'installation'; delete t.landuse"],
            ["t.landuse == 'farm'","t.landuse = 'farmland'"],
            ["t.landuse == 'farmyard'","t.facility = 'yes'; t.use = 'agriculture'; delete t.landuse"],
            ["t.landuse == 'grass' || t.landuse == 'meadow'","t.natural = 'grassland'; t['grassland:type'] = 'grassland'; delete t.landuse"],
            ["t.natural == 'scrub'","t.natural = 'grassland'; t['grassland:type'] = 'grassland_with_trees'"],
            ["t.power == 'generator'","a.F_CODE = 'AL015'; t.use = 'power_generation'"],
            ["t.man_made == 'water_tower'","a.F_CODE = 'AL241'"],
            ["t.resource","t.product = t.resource; delete t.resource"],
            ["(t.shop || t.office) && !(t.building)","a.F_CODE = 'AL015'"]
            ];

            mgcp.mgcpPreRules = translate.buildComplexRules(rulesList);
        }

        // Apply the rulesList.
        // translate.applyComplexRules(tags,attrs,rulesList);
        translate.applyComplexRules(tags,attrs,mgcp.mgcpPreRules);

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

        if (tags.amenity && notBuildingList.indexOf(tags.amenity) == -1 && !(tags.building)) attrs.F_CODE = 'AL015';

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
            if (!(attrs.FFN)) attrs.FFN = facilityList[tags.amenity];
        }

        // Cutlines and Highways.
        // In OSM, a cutline is a cleared way, if it is a polygon then drop the highway info
        if (tags.man_made == 'cutline' && geometryType == 'Area' && tags.highway) delete tags.highway;

    /*
        // Geonames cause problems
        if (tags.waterway && !(tags.intermittent))
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
                    attrs.F_CODE = 'AL020'; // Built Up Area
                    delete tags.place;
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
                    attrs.F_CODE = 'ZD040'; // Named Location
                    delete tags.place;
                    break;

            } // End switch
        }

        // Built-up-areas & Settlements vs Buildings
        // If we have a BUA or a Settlement, change the settlement:type tag to a building so we can
        // go through one2one and get an FFN out
        if (tags['settlement:type'] && !(tags.building))
        {
            tags.building = tags['settlement:type'];
            delete tags['settlement:type'];
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

        // The FCODE for Buildings changed...
        if (attrs.F_CODE == 'AL013') attrs.F_CODE = 'AL015';

    }, // End applyToMgcpPreProcessing


    applyToMgcpPostProcessing : function (tags, attrs, geometryType)
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
            attrs.UID = createUuid().replace('{','').replace('}','');
        }

        // Default railway
        // if (attrs.F_CODE == 'AN010' && tags.railway == 'yes') attrs.RRC = '0';

        // Not a great fit.
        if (tags.public_transportation == 'station' && tags.bus == 'yes') attrs.FFN = '481';

        // Mapping Senior Citizens home to Accomodation. Not great
        if (tags.amenity == 'social_facility' && tags['social_facility:for'] == 'senior') attrs.FFN = 550;

        // Unknown House of Worship
        if (tags.amenity == 'place_of_worship' && tags.building == 'other') attrs.HWT = 999;

        // Cached for faster access
        var fCode = attrs.F_CODE;

        // The folloing bit of code is to account for the specs haveing two different attributes
        // with similar names and roughly the same attributes.
        // smcArray is the list of FCODE's that need to have MCC and SMC swapped
        var smcArray = [ 'BA050', 'DB070', 'DA010', 'AK040' ];

        if (smcArray.indexOf(attrs.F_CODE) !== -1 && attrs.MCC)
        {
            attrs.SMC = attrs.MCC;
            delete attrs.MCC;
        }

        // Apparently, AP050 (Trail) _doesn't_ have WTC=1 (All weather)
        if (fCode == 'AP050' && attrs.WTC == '1') attrs.WTC = '0';

        // Allowed values for FUC in various features
        if (fCode == 'AL020' &&  (['0','1','2','4','19','999'].indexOf(attrs['FUC']) < 0)) attrs.FUC = '999';

        if (fCode == 'AL105' &&  (['0','4','8','19','999'].indexOf(attrs['FUC']) < 0)) attrs.FUC = '999';

        // Workaround until I fix the 'default' values
        // if (fCode == 'AP030' && !('CON' in attrs)) attrs['CON'] = '998';

        // MCC and RST both have mappings to surface=XXX.
        if (fCode == 'AQ040' && attrs.RST)
        {
            // logWarn('Found RST = ' + attrsi.RST + ' in AQ040'); // Should not get this
            var convSurf = { 1:5, 2:46, 5:104, 6:104, 8:104, 999:999 };

            attrs.MCC = convSurf[attrs.RST];
            delete attrs.RST;
        }

        // We have FCODES that are valid NFDD/NAS etc but are not in the MGCP spec
        if (fCode == 'AJ085') attrs.F_CODE = 'AL015'; // Barns are Buildings
        if (fCode == 'AQ140' && geometryType == 'Point') attrs.F_CODE = 'AL015'; // Parking Garage Building

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

        if (noAttrList.indexOf(fCode) !== -1)
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

        // Fix TRD3 Crop Land vs TRD4 Crop Land/Orchard
        if (fCode == 'EA010' && attrs.CSP == '15')
        {
            attrs.F_CODE = 'EA040';
            // logWarn('TRD3 feature EA010 changed to TRD4 EA040 - some data has been dropped');
        }

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
            ["t.sport == 'tennis'","a.F_CODE = 'AK040'"],
            ["t.natural == 'tree'","a.F_CODE = 'EC030'"],
            ["t.amenity == 'ferry_terminal'","a.F_CODE = 'AQ125'; a.FFN = '7'"],
            // ["t.landuse == 'forest'","a.F_CODE = 'EC030'"],
            ["t.amenity == 'fuel'","a.F_CODE = 'AL015'"]
            ];

            mgcp.mgcpPostRules = translate.buildComplexRules(rulesList);
        }

        // translate.applyComplexRules(tags,attrs,rulesList);
        translate.applyComplexRules(tags,attrs,mgcp.mgcpPostRules);

        // Fix up SRT values so we comply with the spec. These values came from data files
        // Format is: orig:new
        srtFix = {
            28:112,  // Ikonos Imagery -> Hires Commercial
            66:112,  // Quickbird Multi 2.44m -> Hires Commercial
            68:110,  // Quickbird Pan 60cm -> Very Hires Commercial
        }; // End srtFix

        if (attrs.SRT in srtFix) attrs.SRT = srtFix[attrs.SRT];

    }, // End of applyToMgcpPostProcessing

    // ##### End of the xxToMgcpxx Block #####

    // toOsm - Translate Attrs to Tags
    toOsm : function(attrs, layerName)
    {
        tags = {};  // This is the output
        // fCode = '';

        // Debug:
        if (config.getOgrDebugDumpattrs() == 'true')
        {
            print('');
            print('#####');
            for (var i in attrs) print('In Attrs:' + i + ': :' + attrs[i] + ':');
        }

        // Set up the fcode translation rules
        if (mgcp.fcodeLookup == undefined)
        {
            // Order is important:
            // First the MGCPv3 & 4 FCODES, then the common ones. This ensures that the common ones don't
            // stomp on the other ones.
            mgcp.rules.fcodeOne2oneV4.push.apply(mgcp.rules.fcodeOne2oneV4,mgcp.rules.fcodeOne2oneInV3);
            mgcp.rules.fcodeOne2oneV4.push.apply(mgcp.rules.fcodeOne2oneV4,fcodeCommon.one2one);

            mgcp.fcodeLookup = translate.createLookup(mgcp.rules.fcodeOne2oneV4);

            // Debug:
            // translate.dumpOne2OneLookup(mgcp.fcodeLookup);
        }

        if (mgcp.lookup == undefined)
        {
            // Setup lookup tables to make translation easier.

            // Add the MGCPv3.0 specific attributes to the v4.0/common attribute table
            mgcp.rules.one2one.push.apply(mgcp.rules.one2one,mgcp.rules.one2oneIn);

            mgcp.lookup = translate.createLookup(mgcp.rules.one2one);

            // Build an Object with both the SimpleText & SimpleNum lists
            mgcp.ignoreList = translate.joinList(mgcp.rules.numBiased, mgcp.rules.txtBiased);

            // Add features to ignore
            mgcp.ignoreList.F_CODE = '';
            mgcp.ignoreList.UID = '';
        }

        // pre processing
        mgcp.applyToOsmPreProcessing(attrs, layerName);

        // Use the FCODE to add some tags.
        if (attrs.F_CODE)
        {
            var ftag = mgcp.fcodeLookup['F_CODE'][attrs.F_CODE];
            tags[ftag[0]] = ftag[1];
            // Debug: Dump out the tags from the FCODE
            // print('FCODE: ' + attrs.F_CODE + ' tag=' + ftag[0] + '  value=' + ftag[1]);
        }

        // one 2 one
        translate.applyOne2One(attrs, tags, mgcp.lookup, {'k':'v'}, mgcp.ignoreList);

        // apply the simple number and text biased rules
        translate.applySimpleNumBiased(attrs, tags, mgcp.rules.numBiased, 'forward');
        translate.applySimpleTxtBiased(attrs, tags,  mgcp.rules.txtBiased,'forward');

        // post processing
        mgcp.applyToOsmPostProcessing(attrs, tags, layerName);

        // Debug: Add the FCODE to the tags
        if (config.getOgrDebugAddfcode() == 'true') tags['raw:debugFcode'] = attrs.F_CODE;

        // Debug:
        if (config.getOgrDebugDumptags() == 'true') for (var i in tags) print('Out Tags: ' + i + ': :' + tags[i] + ':');

        return tags;
    }, // End of ToOsm

    // This gets called by translateToOGR and is where the main work gets done
    // We get Tags and return Attrs and a tableName
    toMgcp : function(tags, elementType, geometryType)
    {
        var tableName = '';
        attrs = {}; // This is the output <GLOBAL>
        attrs.F_CODE = '';

        var tableName2 = ''; // The second table name - will populate if appropriate
        var attrs2 = {}; // The second feature - will populate if appropriate

        // Check if we have a schema. This is a quick way to workout if various lookup tables have been built
        if (mgcp.rawSchema == undefined)
        {
            var tmp_schema = mgcp.getDbSchema();
        }

        // The Nuke Option: If we have a relation, drop the feature and carry on
        if (tags['building:part']) return null;

        // The Nuke Option: "Collections" are groups of different feature types: Point, Area and Line.
        // There is no way we can translate these to a single TDS feature.
        if (geometryType == 'Collection') return null;

        // Debug:
        if (config.getOgrDebugDumptags() == 'true')
        {
            print ('');
            print ('#####');
            for (var i in tags) print('In Tags: ' + i + ': :' + tags[i] + ':');
        }

        // Set up the fcode translation rules
        if (mgcp.fcodeLookup == undefined)
        {
            // Order is important:
            // First the MGCPv4 FCODES, then the common ones. This ensures that the common ones don't
            // stomp on the V4 ones.
            mgcp.rules.fcodeOne2oneV4.push.apply(mgcp.rules.fcodeOne2oneV4,fcodeCommon.one2one);
            mgcp.rules.fcodeOne2oneV4.push.apply(mgcp.rules.fcodeOne2oneV4,mgcp.rules.fcodeOne2oneOut);

            mgcp.fcodeLookup = translate.createBackwardsLookup(mgcp.rules.fcodeOne2oneV4);
        }

        if (mgcp.lookup == undefined)
        {
            // Add the conversion from MGCPv3.0 attributes to the v4.0/common attribute table
            mgcp.rules.one2one.push.apply(mgcp.rules.one2one,mgcp.rules.one2oneOut);

            mgcp.lookup = translate.createBackwardsLookup(mgcp.rules.one2one);

            // Build a list of things to ignore and flip is backwards
            mgcp.ignoreList = translate.flipList(translate.joinList(mgcp.rules.numBiased, mgcp.rules.txtBiased));

            // Add some features to ignore
            mgcp.ignoreList.uuid = '';
            mgcp.ignoreList.source = '';
            mgcp.ignoreList.area = '';
            mgcp.ignoreList['note:extra'] = '';
            mgcp.ignoreList['hoot:status'] = '';
            mgcp.ignoreList['error:circular'] = '';
            mgcp.ignoreList['source:ingest:datetime'] = '';
        }

        // pre processing
        mgcp.applyToMgcpPreProcessing(tags,attrs, geometryType);

        // one 2 one
        translate.applyOne2One(tags, attrs, mgcp.lookup, mgcp.fcodeLookup, mgcp.ignoreList);

        // apply the simple number and text biased rules
        translate.applySimpleNumBiased(attrs, tags, mgcp.rules.numBiased, 'backward');
        translate.applySimpleTxtBiased(attrs, tags,  mgcp.rules.txtBiased,'backward');

        // post processing
        // mgcp.applyToMgcpPostProcessing(attrs, tableName, geometryType);
        mgcp.applyToMgcpPostProcessing(tags, attrs, geometryType);

        // Set the tablename: [P,A,L]<fcode>
        // tableName = geometryType.toString().substring(0,1) + attrs.F_CODE;
        tableName = geometryType.toString().charAt(0) + attrs.F_CODE;

        // Now check for invalid feature geometry
        // E.g. If the spec says a runway is a polygon and we have a line, throw error and
        // push the feature to the o2s layer
        if (!(layerNameLookup[tableName]))
        {
            // tableName = layerNameLookup[tableName];
            logError('FCODE and Geometry: ' + tableName + ' is not in the schema');

            tableName = 'o2s_' + geometryType.toString().charAt(0);

            // Dump out what attributes we have converted before they get wiped out
            if (config.getOgrDebugDumpattrs() == 'true') for (var i in attrs) print('Converted Attrs:' + i + ': :' + attrs[i] + ':');

            for (var i in tags)
            {
                // Clean out all of the "source:XXX" tags to save space
                // if (i.indexOf('source:') !== -1) delete tags[i];
                if (i.indexOf('error:') !== -1) delete tags[i];
                if (i.indexOf('hoot:') !== -1) delete tags[i];
            }

            var str = JSON.stringify(tags);

            // If the tags are > 254 char, split into pieces. Not pretty but stops errors.
            // A nicer thing would be to arrange the tags until they fit neatly
            // Apparently Shape & FGDB can't handle fields > 254 chars.
            if (str.length < 255 || config.getOgrSplitO2s() == 'false')
            {
                // return {attrs:{tag1:str}, tableName: tableName};
                attrs = {tag1:str};
            }
            else
            {
                // Not good. Will fix with the rewrite of the tag splitting code
                if (str.length > 1012)
                {
                    logError('o2s tags truncated to fit in available space.');
                    str = str.substring(0,1012);
                }

                // Now split the text across the available tags
                attrs = {tag1:str.substring(0,253),
                         tag2:str.substring(253,506),
                         tag3:str.substring(506,759),
                         tag4:str.substring(759,1012)};
             }
        }
        else
        {
            // Check if we need to make a second feature
            attrs2 = mgcp.twoFeatures(geometryType,tags,attrs);

            if (attrs2.F_CODE)
            {
                // Set the tablename: [P,A,L]<fcode>
                // tableName = geometryType.toString().substring(0,1) + attrs.F_CODE;
                tableName2 = geometryType.toString().charAt(0) + attrs2.F_CODE;

                // Repeat the feature validation and adding attributes
                mgcp.validateAttrs(geometryType,attrs2);
            }

            // Validate attrs: remove all that are not supposed to be part of a feature
            mgcp.validateAttrs(geometryType,attrs);

        } // End else

        // Debug:
        if (config.getOgrDebugDumpattrs() == 'true' || config.getOgrDebugDumptags() == 'true')
        {
            print('TableName: ' + tableName + '  FCode: ' + attrs.F_CODE + '  Geom: ' + geometryType);
            if (tableName2 !== '') print('TableName2: ' + tableName2 + '  FCode: ' + attrs2.F_CODE + '  Geom: ' + geometryType);
        }

        // Debug:
        if (config.getOgrDebugDumpattrs() == 'true')
        {
            for (var i in attrs) print('Out Attrs:' + i + ': :' + attrs[i] + ':');
            if (attrs2.F_CODE) for (var i in attrs2) print('2Out Attrs:' + i + ': :' + attrs2[i] + ':');
            print('');
        }

        // Send back the feature
        if (attrs2.F_CODE)
        {
            return([{attrs: attrs, tableName: tableName},{attrs: attrs2, tableName: tableName2}]);
        }
        else
        {
            return {attrs: attrs, tableName: tableName};
        }
    } // End of toMgcp

} // End of mgcp
