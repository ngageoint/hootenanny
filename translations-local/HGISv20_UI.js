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

//
// Convert HGISv20 to/from OSM+ and then add the Layername as an attribute
// This is a copy of the standard HGISv20 translator specially for the UI
//

hoot.require('hgis20');
hoot.require('hgis20_schema');
hoot.require('hgis20_rules');
hoot.require('config');
hoot.require('translate');
hoot.require('fcode_common');

// Layer name filter - Filter out all layers that match this regexp
function layerNameFilter()
{
    // Drop all of the "SRC_*" and "o2s_*" layers
    return "^(?!_ATTACH)";
}


// Create the output Schema
function getDbSchema()
{
    return hgis20.getDbSchema();
}


// IMPORT
// translateAttributes - takes 'attrs' and returns OSM 'tags'
// function translateAttributes(attrs, layerName)
function translateToOsm(attrs, layerName, geometryType)
{
    return hgis20.toOsm(attrs, layerName, geometryType);

} // End of Translate Attributes


// EXPORT
// translateToOgr - takes 'tags' + geometry and returns 'attrs' + tableName
function translateToOgr(tags, elementType, geometryType)
{
    var output = hgis20.toHgis(tags, elementType, geometryType);

    for (var i in output)
    {

        output[i]['attrs']['HGIS_Layer'] = output[i]['tableName'];

        // Debug
//         if (config.getOgrDebugDumptags() == 'true')
//         {
//             print('Tablename: ' + output[i]['tableName'])
//             for (var t in output[i]['attrs'])
//             {
//                 print('Out Attrs: ' + t + ' = ' + output[i]['attrs'][t]);
//             }
//         } // End Debug
    }

    return output;

} // End of translateToOgr

