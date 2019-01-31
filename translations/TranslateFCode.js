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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

//
// Convert an FCode to OSM+
//

fcTools = {
    translateFCode : function(fcode,schema)
    {
        var hootHome = process.env.HOOT_HOME;

        // Debug:
        console.log('Hoot Home = ' + hootHome);
        console.log('FCode: ' + fcode + '  Schema: ' + schema);

        // Grab the common FCode translations
        if (typeof fcodeCommon === 'undefined') require(hootHome+'/plugins/fcode_common.js');

        var rawLookup = [];

        // Make a raw lookup table
        switch (schema)
        {
            case undefined:
            case '':
                break;

            case 'TDSv40':
                if (typeof tds === 'undefined') tds = {};
                if (typeof tds.rules === 'undefined') require(hootHome + '/plugins/tds40_rules.js');          
                rawLookup = fcodeCommon.one2one.concat(tds.rules.fcodeOne2oneIn);
                break;

            case 'TDSv61':
                if (typeof tds61 === 'undefined') tds61 = {};
                if (typeof tds61.rules === 'undefined') require(hootHome + '/plugins/tds61_rules.js');          
                rawLookup = fcodeCommon.one2one.concat(tds61.rules.fcodeOne2oneIn);
                break;

            case 'MGCP':
                if (typeof mgcp === 'undefined') mgcp = {};
                if (typeof mgcp.rules === 'undefined') require(hootHome + '/plugins/mgcp_rules.js');
                rawLookup = fcodeCommon.one2one.concat(mgcp.rules.fcodeOne2oneInV3, mgcp.rules.fcodeOne2oneV4);
                break;

            case 'DNC':
                if (typeof dnc === 'undefined') dnc = {};
                if (typeof dnc.rules === 'undefined') require(hootHome + '/plugins/dnc_rules.js');
                rawLookup = fcodeCommon.one2one.concat(dnc.rules.fcodeOne2oneIn);
                break;

            case 'GGDM30':
                if (typeof ggdm30 === 'undefined') ggdm30 = {};
                if (typeof ggdm30.rules === 'undefined') require(hootHome + '/plugins/ggdm30_rules.js');
                rawLookup = fcodeCommon.one2one.concat(ggdm30.rules.fcodeOne2oneIn);
                break;
        } // End make lookup

        // Convert it to something useable
        var lookup = {};

        for (var r in rawLookup)
        {
            var row = rawLookup[r];

            if (row[2]) // Make sure it isn't 'undefined'
            {
                lookup[row[1]] = [row[2],row[3]];
            }
        }

        // Debug:
        // console.log('###');
        // for (var i in lookup) console.log('key:' + i + '  Value:' + lookup[i][0] + ' = ' + lookup[i][1]);
        // console.log('###');

        if (fcode in lookup)
        {
            // Debug:
            // console.log('Found: ' + fcode + ' in lookup');
            // console.log('Key: ' + lookup[fcode][0]);
            // console.log('Value: ' + lookup[fcode][1]);

            // There has got to be a better way of doing this.
            var returnObj = {};
            returnObj[lookup[fcode][0]] = lookup[fcode][1] ;

            return {attrs:returnObj, tableName: ''};
        }
        else
        {
            return {attrs:{'error':'Feature Code ' + fcode + ' is not valid for ' + schema}, tableName: ''};
        }
    }
}


if (typeof exports !== 'undefined') {
    exports.translateFCode = fcTools.translateFCode;

    // Debug:
    console.log('Final Exports: ', exports);
}
