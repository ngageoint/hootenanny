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
 * @copyright Copyright (C) 2014 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

//
// Convert "English" TDSv61 OSM back to standard OSM+
etds61_osm = {
    initialize : function()
    {
        if (etds61_osm.rules == undefined)
            {
                hoot.require('etds61_osm_rules')
            }

        if (typeof tds61 == 'undefined')
        {
            hoot.require('TDSv61_to_OSM')
        }

        // Turn off the TDS structure so we just get the raw feature
        hoot.Settings.set({"ogr.thematic.structure":"false"});
    },


    // The list of values to ignore/drop
    ignoreList : { '-999999':1, '-999999.0':1, 'noinformation':1, 'No Information':1, 'noInformation':1 },

    // This function converts the "English" TDS to TDS and then to OSM+
    toStandardOSM : function(attrs, elementType, geometryType)
    {
        // Being defensive. We might not have had initialise() called
        if (etds61_osm.rules == undefined) 
        {
            etds61_osm.initialize();            
        }

        // Debug: Commenting this out to cut down the number of Hoot core calls
        // if (config.getOgrDebugDumptags() == 'true')
        // {
        //     var kList = Object.keys(attrs).sort()
        //     print('etds61_osm: Initial Attrs');
        //     for (var i = 0, fLen = kList.length; i < fLen; i++) print('In Attrs: ' + kList[i] + ': :' + attrs[kList[i]] + ':');
        // }

        // Go through the attrs and turn them back into TDS
        var nAttrs = {}; // the "new" TDS attrs
        var fCode2 = ''; // The second FCODE - if we have one

        if (attrs['Feature Code'] && attrs['Feature Code'] !== 'Not found')
        {
            if (attrs['Feature Code'].indexOf(' & ') > -1)
            {
                // Two FCODE's
                var tList = attrs['Feature Code'].split(' & ');
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
        else
        {
            // No FCODE, throw error
            // throw new Error('No Valid Feature Code');
            // return null;
            // return {attrs:{'error':'No Valid Feature Code'}, tableName: ''};
            return null;
        }

        // Translate the single values from "English" to TDS
        for (var val in attrs)
        {
            // Dump the No Information values. This is being defensive. We should not get these
            // This loop goes through the set of tags so it's a good place to do this
            if (attrs[val] in etds61_osm.ignoreList) 
                {
                    // Debug
                    // print('Dropping: ' + val + ' = ' + attrs[val]);
                    delete attrs[val];
                    continue;
                }

            if (val in etds61_osm_rules.singleValues)
            {
                // Debug
                // print('Single: ' + etds61_osm_rules.singleValues[val] + ' = ' + attrs[val])
                nAttrs[etds61_osm_rules.singleValues[val]] = attrs[val];

                // Cleanup used attrs
                delete attrs[val];
            }
        }

        // Use a lookup table to convert the remaining attribute names from "English" to TDS
        translate.applyOne2One(attrs, nAttrs, etds61_osm_rules.enumValues, {'k':'v'});
        var tags = {};

        // Now convert the attributes to tags.
        tags = tds61.toOsm(nAttrs,'',geometryType);

        // Go looking for "OSM:XXX" values and copy them to the output
        for (var i in attrs)
        {
            if (i.indexOf('OSM:') > -1) tags[i.replace('OSM:','')] = attrs[i];
        }

        // Check if we have a second FCODE and if it can add any tags
        if (fCode2 !== '')
        {
            var ftag = tds61.fcodeLookup['F_CODE'][fCode2];
            if (ftag)
            {
                if (!(tags[ftag[0]]))
                {
                    tags[ftag[0]] = ftag[1];
                }
                else
                {
                    if (ftag[1] !== tags[ftag[0]])
                    {
                        hoot.logTrace('fCode2: ' + fCode2 + ' tried to replace ' + ftag[0] + ' = ' + tags[ftag[0]] + ' with ' + ftag[1]);
                    }
                }
            }
        }

        // Debug:
        // if (config.getOgrDebugDumptags() == 'true')
        // {
        //     var kList = Object.keys(tags).sort()
        //     for (var j = 0, kLen = kList.length; j < kLen; j++) print('Final Tags:' + kList[j] + ': :' + tags[kList[j]] + ':');
        //     print('');
        // }

        return tags;

    } // End of toOSM

} // End of etds61_osm


// Just a wrapper
function initialize()
{
    etds61_osm.initialize();            
}


// IMPORT
// translateToOsm - Normally takes 'attrs' and returns OSM 'tags'.  This version
//    converts TDSv61 English OSM to standard OSM
//
// This can be called via the following for testing:
// hoot convert -D "convert.ops=hoot::TranslationOp"  \
//      -D translation.script=$HOOT_HOME/translations/englishTDS_to_OSM.js <input>.osm <output>.osm
//
function translateToOsm(attrs, layerName, geometryType)
{
    return etds61_osm.toStandardOSM(attrs,layerName,geometryType);
} // End of translateToOsm


// EXPORT
// translateToOgr - takes 'tags' + geometry and returns 'attrs' + tableName
//    This version converts OSM+ tags to NFDD "English" attributes
function translateToOgr(tags, elementType, geometryType)
{
    return etds61_osm.toStandardOSM(tags, elementType, geometryType)
} // End of translateToOgr

