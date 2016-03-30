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
    OSM+ to "English" NFDD conversion script

    Based on tds/__init__.js script

    MattJ, Mar 14
*/

etds = {
    // This function converts the OSM+ to NFDD and then translated the NFDD into "English"
    toEnglish : function(tags, elementType, geometryType)
    {
        if (tds.fcodeLookup == undefined)
        {
            // Set up the fcode translation rules. We need this due to clashes between the one2one and
            // the fcode one2one rules
            fcodeCommon.one2one.push.apply(fcodeCommon.one2one,tds.rules.fcodeOne2oneOut);

            tds.fcodeLookup = translate.createBackwardsLookup(fcodeCommon.one2one);

            // translate.dumpOne2OneLookup(tds.fcodeLookup);
        }

        if (tds.lookup == undefined)
        {
            // Now build the NFDD one2one rules
            tds.rules.one2one.push.apply(tds.rules.one2one,tds.rules.one2oneOut);

            tds.lookup = translate.createBackwardsLookup(tds.rules.one2one);
            // translate.dumpOne2OneLookup(tds.lookup)
            
            // Build a list of things to ignore and flip is backwards
            tds.ignoreList = translate.joinList(tds.rules.numBiased, tds.rules.txtBiased);
            tds.flipIgnoreList = translate.flipList(tds.ignoreList);
            
            // Add features to ignore
            tds.ignoreList.F_CODE = '';
            tds.ignoreList.FCSUBTYPE = '';
            tds.ignoreList.UFI = '';
            
            // Add Flipped features to ignore
            tds.flipIgnoreList.uuid = '';
            tds.flipIgnoreList.source = '';
            tds.flipIgnoreList.area = '';
            tds.flipIgnoreList['note:extra'] = '';
            tds.flipIgnoreList['hoot:status'] = '';
            tds.flipIgnoreList['error:circular'] = '';
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
        tds.applyToNfddPreProcessing(tags, attrs, geometryType);

        // one 2 one - we call the version that knows about OTH fields
        translate.applyNfddOne2One(tags, attrs, tds.lookup, tds.fcodeLookup, tds.flipIgnoreList);

        // apply the simple number and text biased rules
        // Note: These are BACKWARD, not forward!
        translate.applySimpleNumBiased(attrs, tags, tds.rules.numBiased, 'backward',tds.rules.intList);
        translate.applySimpleTxtBiased(attrs, tags, tds.rules.txtBiased, 'backward');

        // post processing
        tds.applyToNfddPostProcessing(tags, attrs, geometryType);

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
                tds.applyToNfddPreProcessing(tags, attrs2, geometryType);

                // one 2 one - we call the version that knows about OTH fields
                translate.applyNfddOne2One(tags, attrs2, tds.lookup, tds.fcodeLookup, tds.flipIgnoreList);

                // apply the simple number and text biased rules
                // Note: These are BACKWARD, not forward!
                translate.applySimpleNumBiased(attrs2, tags, tds.rules.numBiased, 'backward',tds.rules.intList);
                translate.applySimpleTxtBiased(attrs2, tags, tds.rules.txtBiased, 'backward');

                // post processing
                tds.applyToNfddPostProcessing(tags, attrs2, geometryType);
            } // End sort out Road, Railway & Bridge

            // Go through the list of possible attributes and add the missing ones
            var tmpList = etds.rules.fcodeLookup[attrs.F_CODE]['enum'];

            if (attrs2.F_CODE) // Second FCode
            {
                print ('Second FCODE: ' + attrs2.F_CODE)
                tmpList.push.apply(tmpList,etds.rules.fcodeLookup[attrs2.F_CODE]['enum']);
            }

            for (var i=0, elen = tmpList.length; i < elen; i++)
            {
                // If we don't find one, add it with it's default value
                if (!(attrs[tmpList[i]])) attrs[tmpList[i]] = etds.rules.engDefault[tmpList[i]];
            }

            // Translate the single values
            for (var val in attrs)
            { 
                if (val in etds.rules.engSingle) 
                {
                    eAttrs[etds.rules.engSingle[val]] = attrs[val];

                    // Cleanup used attrs so we don't translate them again
                    delete attrs[val];
                }
            }

            // Apply the English one2one rules
            translate.applyOne2One(attrs, eAttrs, etds.rules.engEnum, {'k':'v'}, tds.ignoreList);

            // Add a default value for the FCODE
            eAttrs['Feature Code'] = 'Not found';

            if (attrs.F_CODE in etds.rules.fcodeLookup)
            {
                eAttrs['Feature Code'] = attrs.F_CODE + ': ' + etds.rules.fcodeLookup[attrs.F_CODE]['desc'];
            }

            // if (attrs2 && (attrs2.F_CODE in etds.rules.fcodeLookup))
            if (attrs2.F_CODE in etds.rules.fcodeLookup)
            {
                eAttrs['Feature Code'] = eAttrs['Feature Code'] + ';' + attrs2.F_CODE + ': ' + etds.rules.fcodeLookup[attrs2.F_CODE]['desc'];
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

} // End of etds
