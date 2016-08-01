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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

//
// Simple script to add tags to an OSM file
//

hoot.require('translate');

function translateAttributes(attrs, layerName, geometryType)
{ 
    // Add UUID
    if (!(attrs.uuid)) attrs.uuid = createUuid();

    // Say where the data came from
    // NOTE: layerName will be empty if using a translationOp to convert the OSM file.
    // This is due to not having metadata pass from OSM reader -> map -> translation
    if (layerName !== '')
    {
        attrs.source = translate.appendValue(attrs.source,'osm:' + layerName.toLowerCase(),';');
    }
    else
    {
        attrs.source = translate.appendValue(attrs.source,'osm',';');
    }

    if (!(attrs.attribution))
    {
        attrs.attribution = 'osm';
    }
    else
    {
        attrs.attribution = attrs.attribution + ';osm';
    }

    // Add the Open Database Licence
    if (!(attrs.license)) attrs.license =  'This data is made available under the Open Database License: http://opendatacommons.org/licenses/odbl/1.0/.'

    return attrs;
}

