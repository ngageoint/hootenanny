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
    OSM+ to "English" MGCP conversion script
*/

var translate = require('./translate.js');
var mgcp = require('./mgcp.js');

var emgcp = module.exports = {
    rules: require('./emgcp_rules.js'),

    // This function converts the OSM+ to MGCP and then translates the MGCP into "English"
    toEnglish : function(tags, elementType, geometryType)
    {
        var mgcpData = [];

        mgcpData = mgcp.toMgcp(tags, elementType, geometryType)

        // Debug:
        if (config.OgrDebugDumptags)
        {
            for (var i = 0, fLen = mgcpData.length; i < fLen; i++)
            {
                print('eTableName ' + i + ': ' + mgcpData[i]['tableName'] + '  FCode: ' + mgcpData[i]['attrs']['FCODE'] + '  Geom: ' + geometryType);
                var kList = Object.keys(mgcpData[i]['attrs']).sort()
                for (var j = 0, kLen = kList.length; j < kLen; j++) print('eOut Attrs:' + kList[j] + ': :' + mgcpData[i]['attrs'][kList[j]] + ':');
            }
            print('');
        }

        var eAttrs = {}; // The final English output
        eAttrs['Feature Code'] = '';

        // Defensive: This will either be populated or we threw an error earlier
        if (mgcpData.length > 0)
        {
            for (var fNum = 0, fLen = mgcpData.length; fNum < fLen; fNum++)
            {
                var tFCODE = mgcpData[fNum]['attrs']['FCODE'];
                delete mgcpData[fNum]['attrs']['FCODE'];

                // Translate the single values
                for (var val in mgcpData[fNum]['attrs'])
                {
                    if (val in emgcp.rules.engSingle)
                    {
                        if (mgcpData[fNum]['attrs'][val] !== undefined)
                        {
                            eAttrs[emgcp.rules.engSingle[val]] = mgcpData[fNum]['attrs'][val];
                        }

                        // Cleanup used attrs so we don't translate them again
                        delete mgcpData[fNum]['attrs'][val];
                    }
                }

                // Apply the English one2one rules
                translate.applyOne2One(mgcpData[fNum]['attrs'], eAttrs, emgcp.rules.engEnum, {'k':'v'});

                // Find an FCODE
                if (tFCODE in emgcp.rules.fcodeLookup)
                {
                    if (eAttrs['Feature Code'] !== '')
                    {
                        eAttrs['Feature Code'] = eAttrs['Feature Code'] + ' & ' + tFCODE + ':' + emgcp.rules.fcodeLookup[tFCODE]['desc'];
                    }
                    else
                    {
                        eAttrs['Feature Code'] = tFCODE + ':' + emgcp.rules.fcodeLookup[tFCODE]['desc'];
                    }
                }
            } // End for mgcpData

        }

        if (config.OgrDebugDumptags)
        {
            var kList = Object.keys(eAttrs).sort()
            for (var j = 0, kLen = kList.length; j < kLen; j++) print('Final Attrs:' + kList[j] + ': :' + eAttrs[kList[j]] + ':');
            print('');
        }

        // Return the English results. The "tableName" is not set
        return {attrs: eAttrs, tableName: ''};
    } // End of toEnglish

} // End of emgcp
