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
// Translate OSM to TDSv61 with the attributes converted to "English"
etds61 = {
    initialize : function()
    {
        if (etds61.rules == undefined)
        {
            hoot.require('etds61_rules')
        }

        if (typeof tds61 == 'undefined')
        {
            hoot.require('TDSv61_to_OGR')
        }

        // Turn off the TDS structure so we just get the raw feature
        hoot.Settings.set({"ogr.thematic.structure":"false"});

        // Turn off the tds extra function
        hoot.Settings.set({"ogr.note.extra":"none"});

        // Turn off the ESRI FCSUBTYPE
        hoot.Settings.set({"ogr.esri.fcsubtype":"false"});

        // Throw errors instead of returning partial translations/o2s_X features
        hoot.Settings.set({"ogr.throw.error":"true"});

        // Set the schema type for the export
        hoot.Settings.set({"osm.map.writer.schema":"TDSv61"});
    },

    // This function converts the OSM+ to TDS and then translates the TDS into "English"
    toEnglish : function(tags, elementType, geometryType)
    {
        var tdsData = [];

        // Being defensive. We might not have had initialise() called
        if (etds61.rules == undefined) 
        {
            etds61.initialize();            
        }

        tdsData = tds61.toOgr(tags, elementType, geometryType)

        // Debug: Commenting this out to cut down the number of Hoot core calls
        // if (config.getOgrDebugDumptags() == 'true')
        // {
        //     for (var i = 0, fLen = tdsData.length; i < fLen; i++)
        //     {
        //         print('eTableName ' + i + ': ' + tdsData[i]['tableName'] + '  FCode: ' + tdsData[i]['attrs']['F_CODE'] + '  Geom: ' + geometryType);
        //         var kList = Object.keys(tdsData[i]['attrs']).sort()
        //         for (var j = 0, kLen = kList.length; j < kLen; j++) print('eOut Attrs:' + kList[j] + ': :' + tdsData[i]['attrs'][kList[j]] + ':');
        //     }
        //     print('');
        // }

        var eAttrs = {}; // The final English output
        eAttrs['Feature Code'] = 'Not found';

        // Defensive: This will either be populated or we threw an error earlier
        if (tdsData.length > 0)
        {
            for (var fNum = 0, fLen = tdsData.length; fNum < fLen; fNum++)
            {
                var tFCODE = tdsData[fNum]['attrs']['F_CODE'];

                delete tdsData[fNum]['attrs']['F_CODE'];

                // Translate the single values
                for (var val in tdsData[fNum]['attrs'])
                {
                    // Dump the No Information values. This is being defensive. We should not get these
                    // This loop goes through the set of tags so it's a good place to do this
                    if (tdsData[fNum]['attrs'][val] == '-999999') 
                    {
                        // Debug:
                        // print('Skipping: ' + val + ' = ' + tdsData[fNum]['attrs'][val]);
                        delete tdsData[fNum]['attrs'][val];
                        continue;
                    }

                    if (val in etds61.rules.engSingle)
                    {
                        if (tdsData[fNum]['attrs'][val] !== undefined)
                        {
                            eAttrs[etds61.rules.engSingle[val]] = tdsData[fNum]['attrs'][val];
                        }

                        // Cleanup used attrs so we don't translate them again
                        delete tdsData[fNum]['attrs'][val];
                    }
                }

                // Apply the English one2one rules
                translate.applyOne2One(tdsData[fNum]['attrs'], eAttrs, etds61.rules.engEnum, {'k':'v'});

                // Find an FCODE
                if (tFCODE in etds61.rules.fcodeLookup)
                {
                    if (eAttrs['Feature Code'] !== 'Not found')
                    {
                        eAttrs['Feature Code'] = eAttrs['Feature Code'] + ' & ' + tFCODE + ':' + etds61.rules.fcodeLookup[tFCODE]['desc'];
                    }
                    else
                    {
                        eAttrs['Feature Code'] = tFCODE + ':' + etds61.rules.fcodeLookup[tFCODE]['desc'];
                    }
                }
            } // End for tdsData

        }

        if (config.getOgrDebugDumptags() == 'true')
        {
            var kList = Object.keys(eAttrs).sort()
            for (var j = 0, kLen = kList.length; j < kLen; j++) print('Final Attrs:' + kList[j] + ': :' + eAttrs[kList[j]] + ':');
            print('');
        }

        // Return the English results. The "tableName" is not set
        return eAttrs;
    } // End of toEnglish

} // End of etds61


// Just a wrapper
function initialize()
{
    etds61.initialize();            
}


// IMPORT
// translateToOsm - Normally takes 'attrs' and returns OSM 'tags'.  This version
//    converts OSM+ tags to TDSv61 "English" Attributes
//
// This can be called via the following for testing:
// hoot convert -D "convert.ops=hoot::TranslationOp"  \
//      -D translation.script=$HOOT_HOME/translations/OSM_to_englishTDS.js <input>.osm <output>.osm
//
function translateToOsm(attrs, layerName, geometryType)
{
    return etds61.toEnglish(attrs,layerName,geometryType);
} // End of translateToOsm


// EXPORT
// translateToOgr - takes 'tags' + geometry and returns 'attrs' + tableName
//    This version converts OSM+ tags to NFDD "English" attributes
function translateToOgr(tags, elementType, geometryType)
{
    return etds61.toEnglish(tags, elementType, geometryType)
} // End of translateToOgr
