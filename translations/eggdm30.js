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
    OSM+ to "English" GGDM30 conversion script

    This script is the same as the standard "etds" script but uses "ggdm30" instead of "tds"
*/

eggdm30 = {
    // This function converts the OSM+ to TDS and then translates the TDS into "English"
    toEnglish : function(tags, elementType, geometryType)
    {
        var tdsData = [];

        tdsData = ggdm30.toOgr(tags, elementType, geometryType)

        // Debug: Commenting this out to cut down the number of Hoot core calls
//         if (config.getOgrDebugDumptags() == 'true')
//         {
//             for (var i = 0, fLen = tdsData.length; i < fLen; i++)
//             {
//                 print('eTableName ' + i + ': ' + tdsData[i]['tableName'] + '  FCode: ' + tdsData[i]['attrs']['F_CODE'] + '  Geom: ' + geometryType);
//                 var kList = Object.keys(tdsData[i]['attrs']).sort()
//                 for (var j = 0, kLen = kList.length; j < kLen; j++) print('eOut Attrs:' + kList[j] + ': :' + tdsData[i]['attrs'][kList[j]] + ':');
//             }
//             print('');
//         }

        var eAttrs = {}; // The final English output
        eAttrs['Feature Code'] = 'Not found';

        if (tdsData.length > 0)
        {
            for (var fNum = 0, fLen = tdsData.length; fNum < fLen; fNum++)
            {
                var tFCODE = tdsData[fNum]['attrs']['F_CODE'];

                delete tdsData[fNum]['attrs']['F_CODE'];

                // Translate the single values
                for (var val in tdsData[fNum]['attrs'])
                {
                    if (val in eggdm30.rules.engSingle)
                    {
                        if (tdsData[fNum]['attrs'][val] !== undefined)
                        {
                            eAttrs[eggdm30.rules.engSingle[val]] = tdsData[fNum]['attrs'][val];
                        }

                        // Cleanup used attrs so we don't translate them again
                        delete tdsData[fNum]['attrs'][val];
                    }
                }

                // Apply the English one2one rules
                translate.applyOne2One(tdsData[fNum]['attrs'], eAttrs, eggdm30.rules.engEnum, {'k':'v'},[]);

                // Find an FCODE
                if (tFCODE in eggdm30.rules.fcodeLookup)
                {
                    if (eAttrs['Feature Code'] !== 'Not found')
                    {
                        eAttrs['Feature Code'] = eAttrs['Feature Code'] + ' & ' + tFCODE + ':' + eggdm30.rules.fcodeLookup[tFCODE]['desc'];
                    }
                    else
                    {
                        eAttrs['Feature Code'] = tFCODE + ':' + eggdm30.rules.fcodeLookup[tFCODE]['desc'];
                    }
                }
            } // End for tdsData

        }

//         if (config.getOgrDebugDumptags() == 'true')
//         {
//             var kList = Object.keys(eAttrs).sort()
//             for (var j = 0, kLen = kList.length; j < kLen; j++) print('Final Attrs:' + kList[j] + ': :' + eAttrs[kList[j]] + ':');
//             print('');
//         }

        // Return the English results. The "tableName" is not set
        return {attrs: eAttrs, tableName: ''};
    } // End of toEnglish

} // End of eggdm30
