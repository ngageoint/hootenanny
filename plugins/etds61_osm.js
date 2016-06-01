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

    This script is the same as the standard "etds_osm" script but uses "tds61" instead of "tds"

    MattJ, Sept 14
*/

// For the new fuzy rules
hoot.require('SchemaTools')

// For the OSM+ to NFDD translation
hoot.require('tds61')
hoot.require('tds61_rules')
hoot.require('fcode_common')

// For the NFDD to NFDD "English" translation
hoot.require('etds61')
hoot.require('etds61_rules')

// NOTE: This include has "etds_osm_rules" NOT "etds_osm.rules"
// This was renamed so the include will work.
hoot.require('etds61_osm_rules')

// Common translation scripts
hoot.require('translate');
hoot.require('config');


etds61_osm = {
    // This function converts the OSM+ to NFDD and then translated the NFDD into "English"
    toOSM : function(attrs, elementType, geometryType)
    {
        // Build the NFDD lookup table
        if (tds61.lookup == undefined)
        {
            // Add "other" rules to the NFDD one2one rules
            tds61.rules.one2one.push.apply(tds61.rules.one2one,tds61.rules.one2oneIn);

            // Add the FCODE rules for Export to the FCODE lookup
            fcodeCommon.one2one.push.apply(fcodeCommon.one2one,tds61.rules.fcodeOne2oneIn);

            // Add the common FCODE rules to the NFDD rules - to get rid of warnings
            tds61.rules.one2one.push.apply(tds61.rules.one2one,fcodeCommon.one2one);

            // Create the Lookup tables
            tds61.lookup = translate.createLookup(tds61.rules.one2one);

            // translate.dumpOne2OneLookup(tds61.lookup);
            
            // Build an Object with both the SimpleText & SimpleNum lists
            tds61.ignoreList = translate.joinList(tds61.rules.numBiased, tds61.rules.txtBiased);
            
            // Add features to ignore
            tds61.ignoreList.F_CODE = '';
            tds61.ignoreList.FCSUBTYPE = '';
            tds61.ignoreList.UFI = '';
        }

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
            if (val in etds61_osm_rules.singleValues)
            {
                nAttrs[etds61_osm_rules.singleValues[val]] = attrs[val];

                // Cleanup used attrs
                delete attrs[val];
            }
              // Debug:
//             else
//             {
//                 print('Single: Didnt find: ' + val);
//             }

        }

        // Debug:
        // if (config.getOgrDebugDumptags() == 'true') for (var i in nAttrs) print('Single nAttrs:' + i + ': :' + nAttrs[i] + ':');

        translate.applyOne2One(attrs, nAttrs, etds61_osm_rules.enumValues, {'k':'v'}, tds61.ignoreList);

        // Convert the NFDD to OSM+
        tags = {}; // The final OSM+ tags

        // pre processing
        tds61.applyToOsmPreProcessing(nAttrs, '', geometryType);
        
        // Debug:
        // if (config.getOgrDebugDumptags() == 'true') for (var i in tags) print('After PreProc:' + i + ': :' + tags[i] + ':');

        // one 2 one
        translate.applyOne2One(nAttrs, tags, tds61.lookup, {'k':'v'}, tds61.ignoreList);

        // Debug:
        // if (config.getOgrDebugDumptags() == 'true') for (var i in tags) print('After one2one:' + i + ': :' + tags[i] + ':');

        // apply the simple number and text biased rules
        // NOTE: We are not using the intList paramater for applySimpleNumBiased when going to OSM.
        translate.applySimpleNumBiased(nAttrs, tags, tds61.rules.numBiased, 'forward',[]);
        translate.applySimpleTxtBiased(nAttrs, tags, tds61.rules.txtBiased, 'forward');

        // Debug:
        // if (config.getOgrDebugDumptags() == 'true') for (var i in tags) print('After Simple:' + i + ': :' + tags[i] + ':');

        // Crack open the OTH field and populate the appropriate attributes
        // if (nAttrs.OTH) translate.processOTH(nAttrs, tags, tds61.lookup);

        // post processing
        tds61.applyToOsmPostProcessing(nAttrs, tags, '', geometryType);

        // If we have a second FCODE, re run the translation with it
        if (fCode2 !== '')
        {
            nAttrs.F_CODE = fCode2;

            // pre processing
            tds61.applyToOsmPreProcessing(nAttrs, '');

            // if (config.getOgrDebugDumptags() == 'true') for (var i in tags) print('After 2nd PreProc:' + i + ': :' + tags[i] + ':');

            // one 2 one
            translate.applyOne2One(nAttrs, tags, tds61.lookup, {'k':'v'}, tds61.ignoreList);

            // if (config.getOgrDebugDumptags() == 'true') for (var i in tags) print('After 2nd one2one:' + i + ': :' + tags[i] + ':');

            // apply the simple number and text biased rules
            // NOTE: We are not using the intList paramater for applySimpleNumBiased when going to OSM.
            translate.applySimpleNumBiased(nAttrs, tags, tds61.rules.numBiased, 'forward',[]);
            translate.applySimpleTxtBiased(nAttrs, tags, tds61.rules.txtBiased, 'forward');

            // if (config.getOgrDebugDumptags() == 'true') for (var i in tags) print('After 2nd Simple:' + i + ': :' + tags[i] + ':');

            // post processing
            tds61.applyToOsmPostProcessing(nAttrs, tags, '');
        }

        // Debug:
//         if (config.getOgrDebugDumptags() == 'true')
//         {
//             for (var i in tags) print('Out Tags: ' + i + ': :' + tags[i] + ':');
//             print('');
//         }

        return {attrs: tags, tableName: ''};

    } // End of toOSM

} // End of etds61_osm

exports.toOSM = etds61_osm.toOSM;
