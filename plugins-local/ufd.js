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
    UFD conversion script
        UFD -> OSM

    For UFD 1,2 and 3
*/


ufd = {
    // ##### Start of the xxToOsmxx Block #####
    applyToOsmPreProcessing: function(attrs, layerName)
    {
        // This is a handy loop. We use it to:
        // 1) Remove all of the "No Information" and -999999 fields
        // 2) Fix the renameing of some fields

        // This is the current lost of shortened and screwed up attribute names and what they should be.
        var swapList = {
            'ACE_EVAL_M':'ACE_EVAL_METHOD_CD',
            'ALE_EVAL_M':'ALE_EVAL_METHOD_CD',
            'COMM_CPYRT':'COMM_CPYRT_NOTICE',
            'COMM_LIC_T':'COMM_LIC_TIER_NOTICE',
            'COMPLETENE':'COMPLETENESS_CODE',
            'CREATION_D':'CREATION_DATE',
            'ORIGINATIN':'ORIGINATING_SOURCE',
            'ORIG_SOURC':'ORIG_SOURCE_DATE',
            'ORIG_SOU_1':'ORIG_SOURCE_INFO',
            'RSTRN_DCLS':'RSTRN_DCLS_XMPT_CD',
            'RSTRN_DECL':'RSTRN_DECLASS',
            'RSTRN_DS_1':'RSTRN_DSEM_CTRL_NIC',
            'RSTRN_DSEM':'RSTRN_DSEM_CTRL_IC',
            'RSTRN_FORE':'RSTRN_FOREIGN_GOV',
            'RSTRN_RELE':'RSTRN_RELEASIBILITY',
            'SEC_CD_CNT':'SEC_CD_CNTRL',
            'SEC_CLASS_':'SEC_CLASS_SYS_ID',
            'SEC_CLS_AU':'SEC_CLS_AUTH_DESC',
            'SEC_CLS__1':'SEC_CLS_AUTH_MULT',
            'UPDATE_S_1':'UPDATE_SOURCE_DATE',
            'UPDATE_S_2':'UPDATE_SOURCE_INFO',
            'UPDATE_SOU':'UPDATE_SOURCE',
            'Z_VALUE_TY':'Z_VALUE_TYPE'
        };

        // List of data values to drop/ignore. Love the spelling of some of these
        var ignoreList = { 'unknown':1, 'unk':1, 'n/a':1, '-32768':1 };

        // Unit conversion. Some attributes are in centimetres, others in decimetres
        var unitList = { 'WD1':10 };

        // make sure all columns are upper case. This simplifies translation.
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

            var c = col.toUpperCase();
            if (c !== col)
            {
                attrs[c] = attrs[col];
                delete attrs[col];
                col = c; // For the rest of the loop
            }

            // Sort out units - if needed
            if (col in unitList) attrs[col] = attrs[col] / unitList[col];

            // Now see if we need to swap attr names
            if (col in swapList)
            {
                // print('Swapped: ' + swapList[col]); // debug
                attrs[swapList[col]] = attrs[col];
                delete attrs[col];
                continue;
            }

        }

        // Time to find an FCODE
        if (attrs.F_CODE)
        {
            // Do Nothing :-)
        }
        else if (attrs.FCODE)
        {
            attrs.F_CODE = attrs.FCODE;
            delete attrs.FCODE;
        }
        else
        {
            var fCodeMap = [
                ['AA050', ['aa050','well_p']], // Well
                ['AL015', ['al015','building_a','building_p']], // Building
                ['AL020', ['al020','builtup_area_a']], // Built up area
                ['AL030', ['al030','cemetery_a']], // Cemetary
                ['AL100', ['al100']], // Hut
                ['AL105', ['al105']], // Settlement
                ['AL130', ['al130','monument_a','monument_p']], // Memorial Monument
                ['AL200', ['al200','ruins_a']], // Ruins
                ['AN010', ['an010','railway_l']], // Railway
                ['AN050', ['an050','rr_siding_spur_l']], // Railway Sidetrack
                ['AN060', ['an060','rr_marshal_yard_a']], // Railway Yard
                ['AP010', ['ap010','trackl','track_l','cart_track','carttrack','cart_track_l']], // Cart Track
                ['AP020', ['ap020','interchange_l', 'interchangel']], // Interchange
                ['AP030', ['ap030','roadnet','road_l','roadl']], // Road
                ['AP050', ['ap050','traill','trail_l']], // Trail
                ['AQ040', ['aq040','bridgel','bridge_l','bridge_tunnel_l','bridge_overpass_l']], // Bridge 
                ['AQ045', ['aq045','bridge_span_l', 'bridge_span_p']], // Bridge Span
                ['AQ050', ['aq050','bridge_superstructure_p']], // Bridge Superstructure
                ['AQ055', ['aq055','bridge_pylon_p']], // Bridge Pylon
                ['AQ065', ['aq065','culvert_l']], // Culvert
                ['AQ113', ['aq113','pipeline_pipe_l']], // Pipeline
                ['AQ130', ['aq130','tunnell','tunnel_l']], // Tunnel
                ['AT050', ['at050','comm_building_a']], // Communication Building
                ['BH010', ['bh010','aqueduct_l']], // Aqueduct
                ['BH020', ['bh020','canal_a','canal_l']], // Canal
                ['BH030', ['bh030','ditch_l','ditch_a']], // Ditch 
                ['BH070', ['bh070','ford_l','ford_p']], // Ford 
                ['BH140', ['bh140', 'river_stream_a','river_stream_l']], // River
                ['DB070', ['db070']], // Cut
                ['DB150', ['db150']], // Mountain Pass
                ['ZD040', ['zd040','named_location_p']], // Annotated Location
                ]

                // Funky but it makes life easier
            var llayerName = layerName.toString().toLowerCase();

            for (var row in fCodeMap)
            {
                for (var val in fCodeMap[row][1])
                {
                    if (llayerName.match(fCodeMap[row][1][val])) attrs.F_CODE = fCodeMap[row][0];
                }
            }
        } // End of Find an FCode

    }, // End of applyToOsmPreProcessing

    applyToOsmPostProcessing : function (attrs, tags, layerName)
    {
        /*
        // figure out the names
        names = [];
        translate.addName(attrs, names, 'NAM');
        translate.addName(attrs, names, 'NFI');
        translate.addName(attrs, names, 'NFN');
        if (names.length >= 1)
        {
            tags['name'] = names[0];
            names.splice(0, 1);
        }
        if (names.length >= 1)
        {
            tags['alt_name'] = names.join(';');
        }
        */

        // Calculate accuracy: taken straight from facc.py
        // 1/30 inch * SCALE for standard
        // NGA standard is +/-25m 90% circular error (CE) for vector data
        // Error is only for well defined points, not the line connecting points
        var accuracy = -1
        // if ('ACE_EVAL' in attrs && attrs['ACE_EVAL'] != '')
        if (translate.isOK(attrs['ACE_EVAL']))
        {
            // Convert to 95% circular error (AKA ~2 standard deviations)
            accuracy = attrs['ACE_EVAL'] * 1.91;
        }
        // else if ('ACE' in attrs && attrs['ACE'] != '')
        else if (translate.isOK(attrs['ACE']))
        {
            // Convert to 95% circular error (AKA ~2 standard deviations)
            accuracy = attrs['ACE_EVAL'] * 1.91;
        }
        // else if ('SCALE' in attrs && attrs['SCALE'] != '')
        else if (translate.isOK(attrs['SCALE']))
        {
            var scale = attrs['SCALE'];
            // Convert to accuracy as 90% circular error
            accuracy = scale * 1.0 / 30.0 / 12 * 0.3048;
            // Convert to 95% circular error (AKA ~2 standard deviations)
            accuracy = accuracy * 1.91;
        }

        // if ('ACC' in attrs)
        if (translate.isOK(attrs['ACC']))
        {
            if (attrs['ACC'] == '2')
            {
                accuracy = accuracy * 2;
                // Add note: Accuracy: Approximate
            }
            else if (attrs['ACC'] == '3')
            {
                accuracy = accuracy * 4;
                // Add note: Accuracy: Doubtful
            }
            else if (attrs['ACC'] == '5')
            {
                accuracy = -1;
                // Add note: Accuracy: Disputed
            }
            else if (attrs['ACC'] == '6')
            {
                // Add note: Accuracy: Undisputed
            }
            else if (attrs['ACC'] == '7')
            {
                // Add note: Accuracy: Precise
            }
            else if (attrs['ACC'] == '8')
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

        // Add a UUID
        tags.source = 'ufd'; 
        if (attrs.GFID) 
        {
            tags.uuid = '{' + attrs.GFID + '}';
        }
        else
        {
            tags.uuid = createUuid();
        }

        // If we have a House of Worship type then tag it
        if (attrs.HWT && attrs.HWT !== '0') tags.amenity = 'place_of_worship';

        // Special Case...
        if (attrs.HWT == '999') tags.amenity = 'religion';

        if (attrs.REL) tags.landuse = 'cemetery';

        // End Funky Rules

        // Built-up-areas & Settlements vs Buildings
        // If we have a BUA, change the building tags to settlement:type
        if ((attrs.F_CODE == 'AL020' || attrs.F_CODE == 'AL105') && tags.building)
        {
            tags['settlement:type'] = tags.building;
            delete tags.building;
        }

        // AQ060 Airport Control Tower needs some extra tags
        if (tags.aeroway == 'tower') 
        {
            tags.use = 'air_traffic_control';
            tags['tower:type'] = 'observation';
        }

        // AT030 Powerline -> AT005 in TDS. 
        if (tags['cable:type'] == 'power' && tags.pylon !== 'yes') tags.cable = 'yes';

        // Orchards have a "product" not a "crop" in UFD
        if (tags.landuse == 'orchard' && tags.product)
        {
            tags.crop = tags.product;
            delete tags.product;
        }

        // EW Radar Site
        if (attrs.F_CODE == 'AT020') tags['radar:use'] = 'early_warning';

        // Sluice Gate
        if (attrs.F_CODE == 'BI040') tags.flow_control = 'sluice_gate';

        translate.fixConstruction(tags, 'highway');
        translate.fixConstruction(tags, 'railway');

        // Fix up the 'surface' values for buildings
        if (attrs.F_CODE == 'AL015' && tags.surface == 'unknown') delete tags.surface;

        // Add 'building = yes' to amenities if we don't already have one
        if (tags.amenity && !(tags.building))
        {
            // print('Added building'); // debug
            // Don't add building=yes to built up areas!
            if (!(tags.place)) tags.building = 'yes';
        }

        // Add 'building = yes' to military if it isn't a range
        if (tags.military && !(tags.building))
        {
            // print('Added building to military'); // debug
            if (tags.military !== 'range') tags.building = 'yes';
        }

        // Add tags if we have Null attributes
        if (attrs.F_CODE == 'AL015' && !(tags.building)) tags.building = 'yes';
        if (attrs.F_CODE == 'BH140' && !(tags.waterway)) tags.waterway = 'river';
        if (attrs.F_CODE == 'AQ040' && !(tags.bridge)) tags.bridge = 'yes';
        if (attrs.F_CODE == 'AQ040' && !(tags.highway)) tags.highway = 'yes';

        // If we have a Tower, Add a man_made tag
        if (tags['tower:type']) tags.man_made = 'tower';

        // Sort out security stuff - not pretty
        // I'm pretty sure we are not going to see 99% of these but they are in the spec
        if (attrs.SEC_CLASS)
        {
            switch (attrs.SEC_CLASS)
            {
                case '01': // COSMIC Top Secret-Bohemia
                    tags['security:classification'] = 'TS'; // Top Secret
                    tags['security:classification:sci'] = 'COSMIC';
                    tags['security:classification:sci:non_us'] = 'BOHEMIA';
                    break;

                case '02': // COSMIC Top Secret-Balk
                    tags['security:classification'] = 'TS'; // Top Secret
                    tags['security:classification:sci'] = 'COSMIC';
                    tags['security:classification:sci:non_us'] = 'BALK';
                    break;

                case '03': // NATO Secret
                    tags['security:classification'] = 'S'; // Secret
                    tags['security:classification:nato'] = 'yes';
                    break;

                case '04': // NATO Secret-Savate
                    tags['security:classification'] = 'S'; // Secret
                    tags['security:classification:nato'] = 'yes';
                    tags['security:classification:sci','SAVATE'];
                    break;

                case '05': // NATO Secret-Avicula
                    tags['security:classification'] = 'S'; // Secret
                    tags['security:classification:nato'] = 'yes';
                    tags['security:classification:sci','AVICULA'];
                    break;

                case '06': // NATO Confidential
                    tags['security:classification'] = 'C'; // Confidential
                    tags['security:classification:nato'] = 'yes';
                    break;

                case '07': // NATO Restricted
                    tags['security:classification'] = 'R'; // Restricted
                    tags['security:classification:nato'] = 'yes';
                    break;

                case '08': // NATO Unclassified
                    tags['security:classification'] = 'U'; // Unclassified
                    tags['security:classification:nato'] = 'yes';
                    break;

                case '09': // COSMIC Top Secret Atom
                    tags['security:classification'] = 'TS'; // Top Secret
                    tags['security:classification:sci'] = 'COSMIC;ATOM';
                    break;

                case '10': // Secret Atomal
                    tags['security:classification'] = 'S'; // Secret
                    tags['security:classification:sci:non_us'] = 'ATOMAL';
                    break;

                case '11': // Confidential Atomal
                    tags['security:classification'] = 'C'; // Confidential
                    tags['security:classification:sci:non_us'] = 'ATOMAL';
                    break;

                case 'TS':
                    tags['security:classification'] = 'TS'; // Top Secret
                    break;

                case 'S':
                    tags['security:classification'] = 'S'; // Secret
                    break;

                case 'C':
                    tags['security:classification'] = 'C'; // Confidential
                    break;

                case 'U':
                    tags['security:classification'] = 'U'; // Unclassified
                    break;
            }
        } // End SEC_CLASSG396


    }, // End of applyToOsmPostProcessing

    // toOsm - Translate Attrs to Tags
    toOsm : function(attrs, layerName)
    {
        tags = {};

        // Debug
        if (config.getOgrDebugDumptags() == 'true') for (var i in attrs) print('In Attrs:' + i + ': :' + attrs[i] + ':');

        // Set up the fcode translation rules. We need this due to clashes between the one2one and
        // the fcode one2one rules
        if (ufd.fcodeLookup == undefined)
        {
            // Add the FCODE rules for Import
            fcodeCommon.one2one.push.apply(fcodeCommon.one2one,ufd.rules.fcodeOne2oneIn);

            ufd.fcodeLookup = translate.createLookup(fcodeCommon.one2one);
            // translate.dumpOne2OneLookup(ufd.fcodeLookup);
        }

        if (ufd.lookup == undefined)
        {
            // Setup lookup tables to make translation easier. I'm assumeing that since this is not set, the
            // other tables are not set either.

            // Add the common FCODE rules
            ufd.rules.one2one.push.apply(ufd.rules.one2one,fcodeCommon.one2one);
            ufd.rules.one2one.push.apply(ufd.rules.one2one,ufd.rules.fcodeOne2oneIn);

            ufd.lookup = translate.createLookup(ufd.rules.one2one);

            // Build an Object with both the SimpleText & SimpleNum lists
            ufd.biasedList = translate.joinList(ufd.rules.numBiased, ufd.rules.txtBiased);

            // Add features to ignore
            ufd.biasedList.F_CODE = '';
            ufd.biasedList.GFID = '';
            ufd.biasedList.SEC_CLASS = '';
        }

        // pre processing
        ufd.applyToOsmPreProcessing(attrs, layerName);

        // Use the FCODE to add some tags.
        if (attrs.F_CODE)
        {
            var ftag = ufd.fcodeLookup['F_CODE'][attrs.F_CODE];
            if (ftag)
            {
                tags[ftag[0]] = ftag[1];
                // Debug: Dump out the tags from the FCODE
                // print('FCODE: ' + attrs.F_CODE + ' tag=' + ftag[0] + '  value=' + ftag[1]);
            }
            else
            {
                hoot.logWarn('Translation for FCODE ' + attrs.F_CODE + ' not found');
            }
        }
        // one 2 one
        translate.applyOne2One(attrs, tags, ufd.lookup, {'k':'v'}, ufd.biasedList);

        // apply the simple number and text biased rules
        translate.applySimpleNumBiased(attrs, tags, ufd.rules.numBiased, 'forward');
        translate.applySimpleTxtBiased(attrs, tags, ufd.rules.txtBiased, 'forward');

        // post processing
        ufd.applyToOsmPostProcessing(attrs, tags, layerName);

        if (config.getOgrDebugDumptags() == 'true') for (var i in tags) print('Out Tags: ' + i + ': :' + tags[i] + ':');

        // debug: Add the FCODE to the tags
        if (config.getOgrDebugAddfcode() == 'true') tags['raw:debugFcode'] = attrs.F_CODE;

        return tags;
    } // End of ToOsm
} // End of ufd


