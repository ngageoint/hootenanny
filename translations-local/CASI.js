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
// CASI Importer
//
// This is very basic and based on the data for the November Hoot Demo
//

hoot.require('config');
hoot.require('translate');

// IMPORT
// translateAttributes - takes 'attrs' and returns OSM 'tags'
// translateToOsm - takes 'attrs' and returns OSM 'tags'
// function translateAttributes(attrsdd, layerName)
function translateToOsm(attrs, layerName)
{
    tags = {};

    if (config.getOgrDebugDumptags() == 'true') for (var i in attrs) print('In Attrs:' + i + ': :' + attrs[i] + ':');

    // Simple one to one rules
    // attr:tag
    var txtBiased = {
    'STANDARDIZED_NAME':'name',
    'F_NAME':'name',
    'ALTERNATE_NAME':'alt_name',
    'ALT_NAME':'alt_name',
    'STANDARDIZED_NAME_ARABIC':'name:ar',
    'F_NAM_NATV':'name:ar',
    'COLLECTION_DATE':'source:date_time',
    'DATE_INFO':'source:date_time',
    'PROD_BY':'attribution',
    'SOURCE':'source',
    'CLASS':'security:classification',
    'CLASSIFICATION':'security:classification',
    'IMAG_VER_SRC':'source:image_id',
    'IMAG_VER_S':'source:image_id',
    'REMARKS':'note',
    };

    translate.applySimpleTxtBiased(attrs, tags, txtBiased, 'forward');

    // Split classification if required
    if (tags['security:classification'].indexOf('//') !== -1)
    {
        var classList = tags['security:classification'].split('//');
        tags['security:classification'] = classList[0];
        tags['security:ic_dissemination_control'] = classList[1];
    }

    // Features
    if (attrs.F_TYPE == 'MSQE')
    {
        tags.amenity = 'place_of_worship';
        tags.building = 'mosque';
    }

    if (attrs.F_TYPE == 'SHRN')
    {
        tags.amenity = 'place_of_worship';
        tags.building = 'shrine';
    }

    if (attrs.F_TYPE == 'CH')
    {
        tags.amenity = 'place_of_worship';
        tags.building = 'church';
    }

/*
    if (attrs.F_TYPE == 'PPLX')
    {
        tags.amenity = 'place_of_worship';
        tags.building = 'church';
    }
*/

    if (attrs.F_TYPE == 'CMTY') tags.landuse = 'cemetery';

    // Ethnic Designation
    if (attrs.SUNNI == 'Yes') 
    {
        tags.religion = 'muslim';
        tags.denomination = 'sunni';
    }

    // Is it a neighborhood?
    if ((attrs.NEIGHBORHOOD_NAME || attrs.URBN_NGHBRHD) && !(tags.POI_ID)) tags.place = 'neighborhood';

    // Damage?
    if (attrs.DAMAGE_VISIBLE == 'Yes') tags.condition = 'damaged';



    if (config.getOgrDebugDumptags() == 'true')
    {
        for (var i in tags) print('Out Tags: ' + i + ': :' + tags[i] + ':');
        print('');
    }

    return tags;
} // End of translateToOsm


