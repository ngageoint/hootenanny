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
    "English" NFDD to OSM+ conversion script

    Based on tds/__init__.js script

    MattJ, Sept 14
*/

// For the OSM+ to NFDD translation
hoot.require('tds40')
hoot.require('tds40_rules')
hoot.require('fcode_common')

// For the NFDD to NFDD "English" translation
hoot.require('etds')
hoot.require('etds_rules')

// Common translation scripts
hoot.require('translate');
hoot.require('config');

etds_osm = {
    // This function converts the OSM+ to NFDD and then translated the NFDD into "English"
    toOSM : function(attrs, elementType, geometryType)
    {
        // Build the NFDD lookup table
        if (tds.lookup == undefined)
        {
            // Add "other" rules to the NFDD one2one rules
            tds.rules.one2one.push.apply(tds.rules.one2one,tds.rules.one2oneIn);

            // Add the FCODE rules for Export to the FCODE lookup
            fcodeCommon.one2one.push.apply(fcodeCommon.one2one,tds.rules.fcodeOne2oneIn);

            // Add the common FCODE rules to the NFDD rules - to get rid of warnings
            tds.rules.one2one.push.apply(tds.rules.one2one,fcodeCommon.one2one);

            // Create the Lookup tables
            tds.lookup = translate.createLookup(tds.rules.one2one);

            // translate.dumpOne2OneLookup(tds.lookup);
            
            // Build an Object with both the SimpleText & SimpleNum lists
            tds.ignoreList = translate.joinList(tds.rules.numBiased, tds.rules.txtBiased);
            
            // Add features to ignore
            tds.ignoreList.F_CODE = '';
            tds.ignoreList.FCSUBTYPE = '';
            tds.ignoreList.UFI = '';
        }

        // Now flip the NFDD English lookup tables around to make life easier
        if (etds_osm.enumValues == undefined)
        {
            etds_osm.enumValues = {};

            for (var i in etds.rules.engEnum)
            {
                var eVal = etds.rules.engEnum[i];

                for (var j in eVal)
                {
                    var key = eVal[j][0];
                    var value = eVal[j][1];

                    // A bit redundant but I don't have a nice way to do this once
                    if (!(key in etds_osm.enumValues)) etds_osm.enumValues[key] = {};

                    etds_osm.enumValues[key][value] = [i,j];
                }
            }

            // Debug:
            // translate.dumpOne2OneLookup(etds_osm.enumValues);
        }

        if (etds_osm.singleValues == undefined)
        {
            etds_osm.singleValues = {};

            for (var i in etds.rules.engSingle)
            {
                etds_osm.singleValues[etds.rules.engSingle[i]] = i;
            }

            // Add another single value
            etds_osm.singleValues['Feature Code'] = 'F_CODE';

            // translate.dumpLookup(etds_osm.singleValues);
        } // End of build lookup tables


        // ##### Convert the English to NFDD #####
        // Strip out the junk - this is also done in the toOsmPreProcessing but this 
        // means that there is less to convert later
        var ignoreList = { '-999999':1, '-999999.0':1, 'noinformation':1, 'No Information':1, 
              'noInformation':1 };

        for (var col in attrs)
        {
            if (attrs[col] in ignoreList) delete attrs[col];
        }

        // Debug:
        // if (config.getOgrDebugDumptags() == 'true') for (var i in attrs) print('In Attrs:' + i + ': :' + attrs[i] + ':');

        // Go through the attrs and turn them back into NFDD
        var nAttrs = {}; // the "new" NFDD attrs
        var fCode2 = ''; // The second FCODE - if we have one

        if (attrs['Feature Code'])
        {
            if (attrs['Feature Code'].indexOf(';') > -1)
            {
                // Two FCODE's
                var tList = attrs['Feature Code'].split(';');
                var fcode = tList[0].split(':');
                attrs['Feature Code'] = fcode[0];

                fcode = tList[1].split(':');
                fCode2 = fcode[0];
            }
            else
            {
                // One FCODE
                var fcode = attrs['Feature Code'].split(':');
                attrs['Feature Code'] = fcode[0];
            }
        }

        // Translate the single values
        for (var val in attrs)
        {
            if (val in etds_osm.singleValues)
            {
                nAttrs[etds_osm.singleValues[val]] = attrs[val];

                // Cleanup used attrs
                delete attrs[val];
            }
         /*
            // Debug:
            else  // Uncomment to Debug
            {
                print('Single: Didnt find: ' + val);
            }
         */
        }

        // Debug:
        // if (config.getOgrDebugDumptags() == 'true') for (var i in nAttrs) print('Single nAttrs:' + i + ': :' + nAttrs[i] + ':');

        translate.applyOne2One(attrs, nAttrs, etds_osm.enumValues, {'k':'v'}, tds.ignoreList);

        // Convert the NFDD to OSM+
        tags = {}; // The final OSM+ tags

        // pre processing
        tds.applyToOsmPreProcessing(nAttrs, '', geometryType);
        
        // Debug:
        // if (config.getOgrDebugDumptags() == 'true') for (var i in tags) print('After PreProc:' + i + ': :' + tags[i] + ':');

        // one 2 one
        translate.applyOne2One(nAttrs, tags, tds.lookup, {'k':'v'}, tds.ignoreList);

        // Debug:
        // if (config.getOgrDebugDumptags() == 'true') for (var i in tags) print('After one2one:' + i + ': :' + tags[i] + ':');

        // apply the simple number and text biased rules
        // NOTE: We are not using the intList paramater for applySimpleNumBiased when going to OSM.
        translate.applySimpleNumBiased(nAttrs, tags, tds.rules.numBiased, 'forward',[]);
        translate.applySimpleTxtBiased(nAttrs, tags, tds.rules.txtBiased, 'forward');

        // Debug:
        // if (config.getOgrDebugDumptags() == 'true') for (var i in tags) print('After Simple:' + i + ': :' + tags[i] + ':');

        // Crack open the OTH field and populate the appropriate attributes
        if (nAttrs.OTH) translate.processOTH(nAttrs, tags, tds.lookup);

        // post processing
        tds.applyToOsmPostProcessing(nAttrs, tags, '', geometryType);

        // If we have a second FCODE, re run the translation with it
        if (fCode2 !== '')
        {
            nAttrs.F_CODE = fCode2;

            // pre processing
            tds.applyToOsmPreProcessing(nAttrs, '', geometryType);

            // Debug:
            // if (config.getOgrDebugDumptags() == 'true') for (var i in tags) print('After 2nd PreProc:' + i + ': :' + tags[i] + ':');

            // one 2 one
            translate.applyOne2One(nAttrs, tags, tds.lookup, {'k':'v'}, tds.ignoreList);

            // Debug:
            // if (config.getOgrDebugDumptags() == 'true') for (var i in tags) print('After 2nd one2one:' + i + ': :' + tags[i] + ':');

            // apply the simple number and text biased rules
            // NOTE: We are not using the intList paramater for applySimpleNumBiased when going to OSM.
            translate.applySimpleNumBiased(nAttrs, tags, tds.rules.numBiased, 'forward',[]);
            translate.applySimpleTxtBiased(nAttrs, tags, tds.rules.txtBiased, 'forward');

            // Debug:
            // if (config.getOgrDebugDumptags() == 'true') for (var i in tags) print('After 2nd Simple:' + i + ': :' + tags[i] + ':');

            // post processing
            tds.applyToOsmPostProcessing(nAttrs, tags, '', geometryType);
        }

        // Debug:
//         if (config.getOgrDebugDumptags() == 'true')
//         {
//             for (var i in tags) print('Out Tags: ' + i + ': :' + tags[i] + ':');
//             print('');
//         }

        return {attrs: tags, tableName: ''};

    } // End of toOSM

} // End of etds_osm

exports.toOSM = etds_osm.toOSM;
