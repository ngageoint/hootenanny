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

    This script is the same as the standard "etds_osm" script but uses "tds61" instead of "tds"

    MattJ, Sept 14, Updated May 16
*/

// For the new fuzy rules
hoot.require('SchemaTools')

// For the OSM+ to TDS61 translation
hoot.require('tds61')
hoot.require('tds61_rules')
hoot.require('fcode_common')

// For the TDS to TDS "English" translation
hoot.require('etds61')
hoot.require('etds61_rules')

// NOTE: This include has "etds_osm_rules" NOT "etds_osm.rules"
// This was renamed so the include will work.
hoot.require('etds61_osm_rules')

// Common translation scripts
hoot.require('translate');
hoot.require('config');


etds61_osm = {
    // This function converts the "English" TDS to TDS and then to OSM+
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
        // if (config.getOgrDebugDumptags() == 'true') for (var i in attrs) print('In Attrs:' + i + ': :' + attrs[i] + ':');

        // Go through the attrs and turn them back into TDS
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
                // Debug
                // print('Single: ' + etds61_osm_rules.singleValues[val] + ' = ' + attrs[val])

                // Cleanup used attrs
                delete attrs[val];
            }
        }

        translate.applyOne2One(attrs, nAttrs, etds61_osm_rules.enumValues, {'k':'v'});

        var tags = {};

        tags = tds61.toOsm(nAttrs,'',geometryType);

        // Debug:
        if (config.getOgrDebugDumptags() == 'true')
        {
            for (var i in tags) print('eOut Tags: ' + i + ': :' + tags[i] + ':');
            print('');
        }

        return {attrs: tags, tableName: ''};

    } // End of toOSM

} // End of etds61_osm

exports.toOSM = etds61_osm.toOSM;
