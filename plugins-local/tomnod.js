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
    Tomnod conversion script

    Based on geonames/__init__.js script

    MattJ, Aug 14
*/


tomnod = {
    // toOsm - Translate Attrs to Tags
    // This is the main routine to convert _TO_ OSM
    toOsm : function(attrs, layerName, geometryType)
    { 
        tags = {};  // The final output Tag list

        // Clean up the attributes
        for (var col in attrs)
        {
            // Push the attribute to upper case - if needed
            var c = col.toUpperCase();
            if (c !== col)
            {
                attrs[c] = attrs[col];
                delete attrs[col];
                // col = c; // For the rest of this loop iteration
            }
        }

        // If we don't have a name, drop the feature
        // This drops the polygon bounding box from the campaigns
        if (!attrs.NAME)
        {
            return null;
        }
        else
        {
            // Split the Name into a Type and an ID
            var values = attrs['NAME'].split('ID');

            attrs.TOMNOD_TYPE = values[0].trim();
            tags['tomnod:id'] = values[1].trim();
        }

        if (config.getOgrDebugDumptags() == 'true') for (var i in attrs) print('In Attrs:' + i + ': :' + attrs[i] + ':');

        // Assign tags based on Feature Code
        if (tomnod.rules.one2one[attrs.TOMNOD_TYPE])
        {
            for (i in tomnod.rules.one2one[attrs.TOMNOD_TYPE]) tags[i] = tomnod.rules.one2one[attrs.TOMNOD_TYPE][i];

            // If it's just a POI, add "poi=yes"
            if (tags['poi:type']) tags.poi = 'yes';
        }
        else
        {
            logVerbose('No rule for Tomnod Type: ' + attrs.TOMNOD_TYPE + '  from: ' + attrs.NAME);
        }

        // Metadata
        tags.source = 'tomnod';
        tags.uuid = createUuid(); 
        if (attrs.DESCRIPTION) tags['tomnod:description'] = attrs.DESCRIPTION;

        if (config.getOgrDebugDumptags() == 'true') 
        {
            for (var i in tags) print('Out Tags: ' + i + ': :' + tags[i] + ':');
            print('');
        }

        return tags;
    }, // End of toOsm

} // End of tomnod

