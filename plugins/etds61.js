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
    OSM+ to "English" TDSv61 conversion script

    This script is the same as the standard "etds" script but uses "tds61" instead of "tds"

    MattJ, Feb 15
*/

etds61 = {
    // This function converts the OSM+ to NFDD and then translated the NFDD into "English"
    toEnglish : function(tags, elementType, geometryType)
    {
        if (tds61.fcodeLookup == undefined)
        {
            // Set up the fcode translation rules. We need this due to clashes between the one2one and
            // the fcode one2one rules
            fcodeCommon.one2one.push.apply(fcodeCommon.one2one,tds61.rules.fcodeOne2oneOut);

            tds61.fcodeLookup = translate.createBackwardsLookup(fcodeCommon.one2one);

            // translate.dumpOne2OneLookup(tds61.fcodeLookup);
        }

        if (tds61.lookup == undefined)
        {
            // Now build the NFDD one2one rules
            tds61.rules.one2one.push.apply(tds61.rules.one2one,tds61.rules.one2oneOut);

            tds61.lookup = translate.createBackwardsLookup(tds61.rules.one2one);
            // translate.dumpOne2OneLookup(tds61.lookup)
            
            // Build a list of things to ignore and flip is backwards
            tds61.ignoreList = translate.joinList(tds61.rules.numBiased, tds61.rules.txtBiased);
            tds61.flipIgnoreList = translate.flipList(tds61.ignoreList);
            
            // Add features to ignore
            tds61.ignoreList.F_CODE = '';
            tds61.ignoreList.FCSUBTYPE = '';
            tds61.ignoreList.UFI = '';
            
            // Add Flipped features to ignore
            tds61.flipIgnoreList.uuid = '';
            tds61.flipIgnoreList.source = '';
            tds61.flipIgnoreList.area = '';
            tds61.flipIgnoreList['note:extra'] = '';
            tds61.flipIgnoreList['hoot:status'] = '';
            tds61.flipIgnoreList['error:circular'] = '';
        }


        attrs = {}; // The Standard NFDD output
        attrs2 = {}; // The second feature - will populate if appropriate
        eAttrs = {}; // The final, English output
        //var fcodeLookup = {}; // Lookup table for FCODE descriptions

        attrs.F_CODE = ''; // Initial setup

        // The Nuke Option: If we have a relation, drop the feature and carry on
        if (tags['building:part']) return null;

        // There is no way we can translate these to a single TDS feature.
        if (geometryType == 'Collection') return null;

        if (config.getOgrDebugDumptags() == 'true') for (var i in tags) print('In Tags: ' + i + ': :' + tags[i] + ':');

        // pre processing
        tds61.applyToNfddPreProcessing(tags, attrs, geometryType);

        // one 2 one - we call the version that knows about OTH fields
        translate.applyNfddOne2One(tags, attrs, tds61.lookup, tds61.fcodeLookup, tds61.flipIgnoreList);

        // apply the simple number and text biased rules
        // Note: These are BACKWARD, not forward!
        translate.applySimpleNumBiased(attrs, tags, tds61.rules.numBiased, 'backward',tds61.rules.intList);
        translate.applySimpleTxtBiased(attrs, tags, tds61.rules.txtBiased, 'backward');

        // post processing
        tds61.applyToNfddPostProcessing(tags, attrs, geometryType);

        if (config.getOgrDebugDumptags() == 'true') for (var i in attrs) print('Out NFDD Attrs: ' + i + ': :' + attrs[i] + ':');

        // ######################
        // The now convert the NFDD to English

        // If we _dont_ have an FCODE, no need to translate the feature
        if (attrs.F_CODE)
        {
            // Check if we are sending back two features or just one.
            // Sort out Roads, Railways and Bridges.
            //
            // In the normal translation code we check to make sure that we have a line feature. Here, we don't really
            // care what the geometry is - we are just moving attributes around.
            // if (geometryType == 'Line' && tags.bridge && (tags.highway || tags.railway))
            if (tags.bridge && (tags.highway || tags.railway))
            {
                if (attrs.F_CODE !== 'AQ040')
                {
                    attrs2.F_CODE = 'AQ040';
                }
                else
                {
                    if (tags.railway)
                    {
                        attrs2.F_CODE = 'AT010';
                    }
                    else
                    {
                        if (tags.highway == 'track')
                        {
                            attrs2.F_CODE = 'AP010';
                        }
                        else
                        {   // The default is to make it a road
                            attrs2.F_CODE = 'AP030';
                        }
                    }
                }

                // Build a second feature
                // pre processing
                tds61.applyToNfddPreProcessing(tags, attrs2, geometryType);

                // one 2 one - we call the version that knows about OTH fields
                translate.applyNfddOne2One(tags, attrs2, tds61.lookup, tds61.fcodeLookup, tds61.flipIgnoreList);

                // apply the simple number and text biased rules
                // Note: These are BACKWARD, not forward!
                translate.applySimpleNumBiased(attrs2, tags, tds61.rules.numBiased, 'backward',tds61.rules.intList);
                translate.applySimpleTxtBiased(attrs2, tags, tds61.rules.txtBiased, 'backward');

                // post processing
                tds61.applyToNfddPostProcessing(tags, attrs2, geometryType);
            } // End sort out Road, Railway & Bridge

            // Go through the list of possible attributes and add the missing ones
            var tmpList = etds61.rules.fcodeLookup[attrs.F_CODE]['enum'];

            if (attrs2.F_CODE) // Second FCode
            {
                // Debug: print ('Second FCODE: ' + attrs2.F_CODE)
                tmpList.push.apply(tmpList,etds61.rules.fcodeLookup[attrs2.F_CODE]['enum']);
            }

            for (var i=0, elen = tmpList.length; i < elen; i++)
            {
                // If we don't find one, add it with it's default value
                if (!(attrs[tmpList[i]])) attrs[tmpList[i]] = etds61.rules.engDefault[tmpList[i]];
            }

            // Translate the single values
            for (var val in attrs)
            {
                if (val in etds61.rules.engSingle)
                {
                    eAttrs[etds61.rules.engSingle[val]] = attrs[val];

                    // Cleanup used attrs so we don't translate them again
                    delete attrs[val];
                }
            }

            // Apply the English one2one rules
            translate.applyOne2One(attrs, eAttrs, etds61.rules.engEnum, {'k':'v'}, tds61.ignoreList);

            // Add a default value for the FCODE
            eAttrs['Feature Code'] = 'Not found';

            // Find an FCODE
            if (attrs.F_CODE in etds61.rules.fcodeLookup)
            {
                eAttrs['Feature Code'] = attrs.F_CODE + ': ' + etds61.rules.fcodeLookup[attrs.F_CODE]['desc'];
            }

            // Find a second FCODE
            // if (attrs2 && (attrs2.F_CODE in etds61.rules.fcodeLookup))
            if (attrs2.F_CODE in etds61.rules.fcodeLookup)
            {
                eAttrs['Feature Code'] = eAttrs['Feature Code'] + ';' + attrs2.F_CODE + ': ' + etds61.rules.fcodeLookup[attrs2.F_CODE]['desc'];
            }

            if (config.getOgrDebugDumptags() == 'true')
            {
                for (var i in eAttrs) print('Out Attrs: ' + i + ': :' + eAttrs[i] + ':');
                print('');
            }

            // Return the English results. The "tableName" is not set
            return {attrs: eAttrs, tableName: ''};
        }
        else
        {
            // If we can't find an FCODE, just return the tags.
            tags['Feature Code'] = 'Not found';

            // Add "OSM:" to each of the tags
            for (var i in tags)
            {
                tags['OSM:' + i] = tags[i];
                delete tags[i];
            }
            if (config.getOgrDebugDumptags() == 'true')
            {
                for (var i in tags) print('Out Attrs: ' + i + ': :' + tags[i] + ':');
                print('');
            }

            return {attrs: tags, tableName: ''};
        }
    } // End of toEnglish

} // End of etds61
