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
    "English" TDS to OSM+ conversion script

    Based on tds/__init__.js script
*/
if (typeof hoot === 'undefined') {
    var hoot = require(process.env.HOOT_HOME + '/lib/HootJs');
}

// For the OSM+ to TDS translation
hoot.require('tds40')
hoot.require('tds40_rules')
hoot.require('fcode_common')

// For the TDS to TDS "English" translation
hoot.require('etds40')

// NOTE: This include has "etds_osm_rules" NOT "etds_osm.rules"
// This was renamed so the include will work.
hoot.require('etds40_osm_rules')

// Common translation scripts
hoot.require('translate');
hoot.require('config');


etds40_osm = {
    // This function converts the OSM+ to TDS and then translated the TDS into "English"
    toOSM : function(attrs, elementType, geometryType)
    {
        // Strip out the junk - this is also done in the toOsmPreProcessing but this
        // means that there is less to convert later
        var ignoreList = { '-999999':1, '-999999.0':1, 'noinformation':1, 'No Information':1, 'noInformation':1 };

        for (var col in attrs)
        {
            if (attrs[col] in ignoreList) delete attrs[col];
        }

        // Debug:
        if (config.getOgrDebugDumptags() == 'true')
        {
            var kList = Object.keys(attrs).sort()
            for (var i = 0, fLen = kList.length; i < fLen; i++) print('In Attrs: ' + kList[i] + ': :' + attrs[kList[i]] + ':');
        }

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
            return {attrs:{}, tableName: ''};
        }
        // Translate the single values from "English" to TDS
        for (var val in attrs)
        {
            if (val in etds40_osm_rules.singleValues)
            {
                nAttrs[etds40_osm_rules.singleValues[val]] = attrs[val];
                // Debug
                // print('Single: ' + etds40_osm_rules.singleValues[val] + ' = ' + attrs[val])

                // Cleanup used attrs
                delete attrs[val];
            }
        }

        // Use a lookup table to convert the remaining attribute names from "English" to TDS
        translate.applyOne2One(attrs, nAttrs, etds40_osm_rules.enumValues, {'k':'v'});

        var tags = {};

        // Now convert the attributes to tags.
        tags = tds.toOsm(nAttrs,'',geometryType);

        // Go looking for "OSM:XXX" values and copy them to the output
        for (var i in attrs)
        {
            if (i.indexOf('OSM:') > -1) tags[i.replace('OSM:','')] = attrs[i];
        }

        // Check if we have a second FCODE and if it can add any tags
        if (fCode2 !== '')
        {
            var ftag = tds.fcodeLookup['F_CODE'][fCode2];
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
        if (config.getOgrDebugDumptags() == 'true')
        {
            var kList = Object.keys(tags).sort()
            for (var j = 0, kLen = kList.length; j < kLen; j++) print('eOut Tags:' + kList[j] + ': :' + tags[kList[j]] + ':');
            print('');
        }

        return {attrs: tags, tableName: ''};

    } // End of toOSM

} // End of etds40_osm

if (typeof exports !== 'undefined') {
    exports.toOSM = etds40_osm.toOSM;
    exports.EnglishtoOSM = etds40_osm.toOSM;
    exports.RawtoOSM = tds.toOsm;
    exports.OSMtoEnglish = etds40.toEnglish;
    exports.OSMtoRaw = tds.toNfdd;
}
