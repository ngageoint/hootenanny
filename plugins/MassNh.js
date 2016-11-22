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
 * @copyright Copyright (C) 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

/*
    Convert Mass & NH GIS data to OSM+
*/

MassNh = {
    // ##### Start of the xxToOsmxx Block #####
    applyToOsmPreProcessing: function(attrs, layerName, geometryType)
    {
        // List of data values to drop/ignore
        var ignoreList = { '0':1, '-999999':1, 'noinformation':1 };

        // This is a handy loop. We use it to:
        // 1) Remove all of the "No Information" and -999999 fields
        // 2) Convert all of the Attrs to uppercase - if needed
       for (var col in attrs)
        {
            // slightly ugly but we would like to account for: 'No Information', 'noInformation' etc
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

            // Push the attribute to upper case - if needed
            var c = col.toUpperCase();
            if (c !== col)
            {
                attrs[c] = attrs[col];
                delete attrs[col];
                col = c; // For the rest of this loop iteration
            }

        } // End in attrs loop

        // Median Width
        if (attrs.MEDIANWIDT)
        {
            var mWidth = Number(attrs.MEDIANWIDT);

            if (mWidth !== NaN && mWidth !== 0)
            {
                tags['median:min_width'] = mWidth * 0.3048; // Feet to Meters
                tags['median:max_width'] = mWidth * 0.3048;
                tags['median:interval:closure'] = 'closed_interval'; // Got to love TDS...
            }
        } // End Median Width

        // Road Width
        if (attrs.SURFACEWID)
        {
            var sWidth = Number(attrs.SURFACEWID);

            if (sWidth !== NaN && sWidth !== 0)
            {
                tags['width:minimum_traveled_way'] = sWidth * 0.3048; // Feet to Meters
            }
        } // End Road Width

        // Speed Limit
        if (attrs.SPEEDLIMIT)
        {
            var sLimit = Number(attrs.SPEEDLIMIT);

            if (sLimit !== NaN && sLimit !== 0)
            {
                tags.maxspeed = sLimit * 0.3048; // MPH to KPH
            }
        } // End Speed Limit

        // Bike trails
        if (attrs.TRAIL_STAT) tags.highway = 'cycleway';

        // Mass Building footprints
        if (attrs.STRUCT_ID) tags.building = 'yes';

        // Mass Building footprints
        if (attrs.DR24K_ID) tags.waterway = 'river';

        // Mass Long Trails
        if (layerName == 'LONGDISTTRAILS_ARC') tags.highway  = 'track';


        // POI's in NH - Lazy
        if (attrs.DESID)
        {
            tags.poi = 'yes';
            tags.poi_type = attrs.TYPE;
        }

    }, // End of applyToOsmPreProcessing

    applyToOsmPostProcessing : function (attrs, tags, layerName, geometryType)
    {
        // New TDSv61 Attribute - ROR (Road Interchange Ramp)
        if (tags.highway && tags.link_road == 'yes')
        {
            var roadList = ['motorway','trunk','primary','secondary','tertiary'];
            if (roadList.indexOf(tags.highway) !== -1) tags.highway = tags.highway + '_link';
        }

        // If we have a UFI, store it. Some of the MAAX data has a LINK_ID instead of a UFI
        tags.source = 'MassNh_GIS';
        tags.uuid = createUuid();

        // Medians
        if (attrs.MEDIANTYPE && attrs.MEDIANTYPE !== '0') tags.median = 'yes';

        // Service Roads
        if (attrs.ADDEDROADT == '5' || attrs.ADDEDROADT == '6' || attrs.ADDEDROADT == '7' )
        {
            tags.highway = 'service';
        }

        // Service Roads
        if (attrs.ADDEDROADT == '11')
        {
            tags.highway = 'rest_area';
        }

        // Railway Stuff
        if (attrs.RT_CLASS)
        {
            if (attrs.RT_CLASS == '2')
            {
                tags['railway:in_road'] = 'separated';
            }
            else if (attrs.RT_CLASS == '3')
            {
                tags['railway:in_road'] = 'yes';
            }
            else if (attrs.RT_CLASS == '4')
            {
                tags.layer = '1';
            }
            else if (attrs.RT_CLASS == '6')
            {
                tags.layer = '-1';
            }
        } // End RT_CLASS

        // Cog Railways
        if (attrs.RRA == '2') tags.rack = 'cog';

        // From TRAINS_NODE
        if (attrs.STATION) tags.railway = 'station';

        // Bike trails
        if (attrs.LINENAME && !(attrs.TRAILNAME)) tags.name = attrs.LINENAME;

        // NH Places of Worship
        if (attrs.NAICSCODE == '813110')
        {
            tags.amenity = 'place_of_worship';
            if (attrs.SUBTYPE == 'MUSLIM')
            {
                tags.religion = 'muslim';
                tags.building = 'mosque';
            }
            else if (attrs.SUBTYPE == 'JUDAIC')
            {
                tags.religion = 'jewish';
                tags.building = 'synagogue';
            }
            else if (attrs.SUBTYPE == 'BUDDHIST')
            {
                tags.religion = 'buddhist';
            }
            else if (attrs.SUBTYPE == 'CHRISTIAN')
            {
                tags.religion = 'christian';
                tags.building = 'church';
            }
        } // End NH Places of Worship


        // Hospitals
        if (attrs.NAICSCODE == '622310' || attrs.HOSP_ID) tags.building = 'hospital';

        // Fire Stations
        if (attrs.NAICSCODE == '922160' || layerName == 'FIRESTATIONS_PT_MEMA') tags.amenity = 'fire_station';

        // Police Stations
        if (attrs.NAICSCODE == '922120' || layerName == 'POLICESTATIONS_PT_MEMA') tags.amenity = 'police';


        // Mass Waterways are funky. We are going to just chop them into their main types
        if (attrs.MINOR_TOT)
        {
            var totValue = attrs.MINOR_TOT.slice(0,3);

            switch (totValue)
            {
                case '101':
                    tags.natural = 'water';
                    tags.water = 'reservoir';
                    break;

                case '102':
                    tags.man_made = 'reservoir_covered';
                    break;

                case '105':
                case '710':
                    tags.natural = 'wetland';
                    break;

                case '106':
                    tags.place = 'maritime_farm';
                    break;

                case '107':
                    tags.natural = 'water';
                    tags.water = 'reservoir';
                    tags['reservoir:type'] = 'industrial';
                    break;

                case '109':
                    tags.natural = 'water';
                    tags.water = 'wastewater';
                    break;

                case '111':
                case '612':
                case '711':
                    tags.natural = 'wetland';
                    tags.wetland = 'marsh';
                    break;

                case '114':
                    tags.natural = 'wetland';
                    tags.wetland = 'bog';
                    tags['bog:type'] = 'cranberry';
                    break;

                case '115':
                    tags.natural = 'flats';
                    break;

                case '116':
                    tags.natural = 'water';
                    tags.estuary = 'yes';
                    break;

                case '174':
                    tags['raw:minor_tot'] = attrs.MINOR_TOT;
                    break;

                case '200':
                    tags.shoreline = 'yes';
                    break;

                case '201':
                    tags.shoreline = 'yes';
                    tags['man_made:shoreline'] = 'yes';
                    break;

                case '202':
                    tags.closure_line = 'yes'; // ?????
                    break;

                case '203':
                    tags.shoreline = 'yes';
                    tags['shoreline:delineated'] = 'no';
                    break;

                case '204':
                case '720':
                    tags.apparent_limit = 'yes';
                    break;

                case '400':
                    tags.waterway = 'rapids';
                    break;

                case '401':
                    tags.waterway = 'waterfall';
                    break;

                case '402':
                    tags.gravel_pit = 'yes';
                    tags.natural = 'water';
                    break;

                case '404':
                    tags.man_made = 'pumping_station';
                    break;

                case '406':
                    tags.waterway = 'dam';
                    break;

                case '407':
                    tags.waterway = 'lock_gate';
                    break;

                case '408':
                    tags.waterway = 'spillway';
                    break;

                case '409': // Gate
                    tags.barrier = 'gate';
                    break;

                case '412':
                    tags.waterway = 'stream';
                    break;

                case '414':
                    tags.waterway = 'canal';
                    break;

                case '415':
                    tags.bridge = 'aqueduct';
                    break;

                case '419':
                    tags.natural = 'water';
                    tags.water = 'channel';
                    break;

                case '421':
                    tags.natural = 'water';
                    tags.water = 'lake';
                    break;

                case '605':
                    tags.waterway = 'river';
                    tags['waterway:bank'] = 'right';
                    break;

                case '606':
                    tags.waterway = 'river';
                    tags['waterway:bank'] = 'left';
                    break;

                case '610': // intermittant
                    switch (attrs.MINOR_TOT)
                    {
                        case '610412':
                            tags.waterway = 'stream';
                            break;

                        case '610414':
                            tags.waterway = 'canal';
                            break;

                        case '610421':
                            tags.natural = 'water';
                            tags.water = 'pond';
                            break;
                    }
                    tags.intermittant = 'yes';
                    break;

                case '619': // fix
                    switch (attrs.MINOR_TOT)
                    {
                        case '619421':
                            tags.natural = 'water';
                            tags.water = 'lake';
                            break;

                        case '619625415':
                            tags.bridge = 'aqueduct';
                            break;
                    }
                    break;
            } // End switch
        } // End Mass Waterways

        // Fix up areas
        if (geometryType == 'Area' && translate.isOsmArea(tags))
        {
            // Debug
            // print('Adding area=yes');
            tags.area = 'yes';
        }



    }, // End of applyToOsmPostProcessing

    // ##### End of the xxToOsmxx Block #####


    // toOsm - Translate Attrs to Tags
    // This is the main routine to convert _TO_ OSM
    toOsm : function(attrs, layerName, geometryType)
    {
        tags = {};  // The final output Tag list

        // Debug:
        if (config.getOgrDebugDumptags() == 'true') for (var i in attrs) print('In Attrs:' + i + ': :' + attrs[i] + ':');

        if (MassNh.lookup == undefined)
        {
            // Setup lookup tables to make translation easier. I'm assumeing that since this is not set, the 
            // other tables are not set either.
            
            MassNh.lookup = translate.createLookup(MassNh.rules.one2one);

            // Build an Object with both the SimpleText & SimpleNum lists
            MassNh.ignoreList = translate.joinList(MassNh.rules.numBiased, MassNh.rules.txtBiased);
            
            // Add features to ignore
            MassNh.ignoreList.F_CODE = '';
            MassNh.ignoreList.FCSUBTYPE = '';
            MassNh.ignoreList.UFI = '';
        }

        // pre processing
        MassNh.applyToOsmPreProcessing(attrs, layerName, geometryType);

        // one 2 one
        translate.applyOne2One(attrs, tags, MassNh.lookup, {'k':'v'}, MassNh.ignoreList);

        
        // apply the simple number and text biased rules
        // NOTE: We are not using the intList paramater for applySimpleNumBiased when going to OSM.
        translate.applySimpleNumBiased(attrs, tags, MassNh.rules.numBiased, 'forward',[]);
        translate.applySimpleTxtBiased(attrs, tags, MassNh.rules.txtBiased, 'forward');

        // post processing
        MassNh.applyToOsmPostProcessing(attrs, tags, layerName, geometryType);
        
        // Debug:
        if (config.getOgrDebugDumptags() == 'true') 
        {
            for (var i in tags) print('Out Tags: ' + i + ': :' + tags[i] + ':');
            print('');
        }

        return tags;
    }, // End of toOsm

} // End of MassNh
