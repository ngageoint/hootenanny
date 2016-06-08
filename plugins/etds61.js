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
        var tdsData = [];

        tdsData = tds61.toNfdd(tags, elementType, geometryType)

//         // Debug:
//         if (config.getOgrDebugDumptags() == 'true')
//         {
//             for (var i = 0, fLen = tdsData.length; i < fLen; i++)
//             {
//                 print('rTableName ' + i + ': ' + tdsData[i]['tableName'] + '  FCode: ' + tdsData[i]['attrs']['F_CODE'] + '  Geom: ' + geometryType);
//                 var kList = Object.keys(tdsData[i]['attrs']).sort()
//                 for (var j = 0, kLen = kList.length; j < kLen; j++) print('rOut Attrs:' + kList[j] + ': :' + tdsData[i]['attrs'][kList[j]] + ':');
//             }
//             print('');
//         }

        var eAttrs = {}; // The final, English output

        if (tdsData.length > 0)
        {
            for (var fNum = 0, fLen = tdsData.length; fNum < fLen; fNum++)
            {
                var tFCODE = tdsData[fNum]['attrs']['F_CODE'];

                // Go through the list of possible attributes and add the missing ones
                var tmpList = etds61.rules.fcodeLookup[tFCODE]['enum'];

                for (var i=0, elen = tmpList.length; i < elen; i++)
                {
                    // If we don't find one, add it with it's default value
                    if (!(tdsData[fNum]['attrs'][tmpList[i]]))
                    {
                        tdsData[fNum]['attrs'][tmpList[i]] = etds61.rules.engDefault[tmpList[i]];
                    }
                }

                // Translate the single values
                for (var val in tdsData[fNum]['attrs'])
                {
                    if (val in etds61.rules.engSingle)
                    {
                        if (tdsData[fNum]['attrs'][val] == undefined)
                        {
                            eAttrs[etds61.rules.engSingle[val]] = etds61.rules.engDefault[val];
                        }
                        else
                        {
                            eAttrs[etds61.rules.engSingle[val]] = tdsData[fNum]['attrs'][val];
                        }

                        // Cleanup used attrs so we don't translate them again
                        delete tdsData[fNum]['attrs'][val];
                    }
                }

                // Apply the English one2one rules
                translate.applyOne2One(tdsData[fNum]['attrs'], eAttrs, etds61.rules.engEnum, {'k':'v'});

                // Add a default value for the FCODE
                eAttrs['Feature Code'] = 'Not found';

                // Find an FCODE
                if (tFCODE in etds61.rules.fcodeLookup)
                {
                    eAttrs['Feature Code'] = tFCODE + ': ' + etds61.rules.fcodeLookup[tFCODE]['desc'];
                }
            } // End for tdsData


        // ######################
        // The now convert the NFDD to English

        // If we _dont_ have an FCODE, no need to translate the feature
//         if (attrs.F_CODE)
//         {
//
//             // Go through the list of possible attributes and add the missing ones
//             var tmpList = etds61.rules.fcodeLookup[attrs.F_CODE]['enum'];
//
//             if (attrs2.F_CODE) // Second FCode
//             {
//                 // Debug: print ('Second FCODE: ' + attrs2.F_CODE)
//                 tmpList.push.apply(tmpList,etds61.rules.fcodeLookup[attrs2.F_CODE]['enum']);
//             }
//
//             for (var i=0, elen = tmpList.length; i < elen; i++)
//             {
//                 // If we don't find one, add it with it's default value
//                 if (!(attrs[tmpList[i]])) attrs[tmpList[i]] = etds61.rules.engDefault[tmpList[i]];
//             }
//
//             // Translate the single values
//             for (var val in attrs)
//             {
//                 if (val in etds61.rules.engSingle)
//                 {
//                     eAttrs[etds61.rules.engSingle[val]] = attrs[val];
//
//                     // Cleanup used attrs so we don't translate them again
//                     delete attrs[val];
//                 }
//             }
//
//             // Apply the English one2one rules
//             translate.applyOne2One(attrs, eAttrs, etds61.rules.engEnum, {'k':'v'}, tds61.ignoreList);
//
//             // Add a default value for the FCODE
//             eAttrs['Feature Code'] = 'Not found';
//
//             // Find an FCODE
//             if (attrs.F_CODE in etds61.rules.fcodeLookup)
//             {
//                 eAttrs['Feature Code'] = attrs.F_CODE + ': ' + etds61.rules.fcodeLookup[attrs.F_CODE]['desc'];
//             }
//
//             // Find a second FCODE
//             // if (attrs2 && (attrs2.F_CODE in etds61.rules.fcodeLookup))
//             if (attrs2.F_CODE in etds61.rules.fcodeLookup)
//             {
//                 eAttrs['Feature Code'] = eAttrs['Feature Code'] + ';' + attrs2.F_CODE + ': ' + etds61.rules.fcodeLookup[attrs2.F_CODE]['desc'];
//             }

            if (config.getOgrDebugDumptags() == 'true')
            {
                var kList = Object.keys(eAttrs).sort()
                for (var j = 0, kLen = kList.length; j < kLen; j++) print('eOut Attrs:' + kList[j] + ': :' + eAttrs[kList[j]] + ':');
                print('');
            }

            // Return the English results. The "tableName" is not set
            return {attrs: eAttrs, tableName: ''};
            //return tdsData;
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
