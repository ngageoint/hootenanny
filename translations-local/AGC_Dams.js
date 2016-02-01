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
    AGC Dam translation script
*/

// IMPORT
// translateAttributes - takes 'attrs' and returns OSM 'tags'
function translateAttributes(attrs, layerName, geometryType)
{
    tags = {};

    tags.waterway = 'dam';

    tags['is_in:state'] = attrs.STATE;
    tags['is_in:county'] = attrs.COUNTY;

    if (attrs.NIDID)
    {
        tags.source = 'USACE NID';
        tags.uuid = attrs.NIDID;
        tags.name = attrs.DAM_NAME;
        // tags.alt_name = attrs.OTHER_DAM_NAME;
        tags.alt_name = attrs.OTHER_DAM_;
    }
    else
    {
        tags.source = 'BGN GNIS';
        tags.uuid = attrs.FEATURE_ID;
        // tags.name = attrs.FEATURE_NAME;
        tags.name = attrs.FEATURE_NA;
    }

    return tags;

} // End of Translate Attributes

