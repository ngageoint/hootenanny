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
    Wikimapia conversion script
        Wikimapia -> OSM+

    Based on translations/GeoNamesOrg.js script
*/


wikimapia = {
    // toOsm - Translate Attrs to Tags
    // This is the main routine to convert TO OSM+
    toOsm : function(attrs, layerName)
    { 
        tags = {};  // The final output Tag list

        // List of data values to drop/ignore
//         var ignoreList = { '-999999.0':1, '-999999':1, 'noinformation':1 };

        // Cleanup the input data
        for (var col in attrs)
        {
            // Wipe out the useless/empty values
//             if (attrs[col] == '' || attrValue in ignoreList || attrs[col] in ignoreList)
            if (attrs[col] == '')
            {
                delete attrs[col];
                continue;
            }

            // Push the attribute to upper case - if needed
            var c = col.toUpperCase();
            if (c !== col)
            {
                attrs[c] = attrs[col];
                delete attrs[col];
                // col = c; // For the rest of this loop iteration
            }
        } // End in Attrs loop

        // Debug
        if (config.getOgrDebugDumpattrs() == 'true') for (var i in attrs) print('In Attrs:' + i + ': :' + attrs[i] + ':');

        // Metadata
        tags.source = 'wikimapia';
        if (attrs.PLACE_ID)
        {
            tags.uuid = "wikimapia:" + attrs.PLACE_ID;
        }
        else
        {
            tags.uuid = createUuid();
        }

        // Start translating attributes
        if (attrs.NAME) tags.name = attrs.NAME;
        if (attrs.DESC) tags.note = attrs.DESC;
        if (attrs.URL) tags['source:url'] = attrs.URL;

        // Feature Code
        if (attrs.CATEGORY)
        {
            var catList = attrs.CATEGORY.split(', ');

            for (var i = 0, len = catList.length; i < len; i++)
            {
                if (wikimapia.rules.transRules[catList[i]])
                {
                    for (var j in wikimapia.rules.transRules[catList[i]]) tags[j] = wikimapia.rules.transRules[catList[i]][j];
                }
                else // Unknown category
                {
                    tags.wm_Unk_Category = translate.appendValue(attrs.wm_Unk_Category, catList[i], ';');
                }
            } // End catList
        } // End category
        else // No Category, try somthing else
        {
            tags.wm_No_Category = 'yes';
        } // End No Category

        if (config.getOgrDebugDumptags() == 'true')
        {
            for (var i in tags) print('Out Tags: ' + i + ': :' + tags[i] + ':');
            print('');
        }

        return tags;
    }, // End of toOsm

} // End of wikimapia
