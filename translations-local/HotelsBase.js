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
 * @copyright Copyright (C) 2013, 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

//
// HotelsBase Conversion Script
//
hoot.require('config')
hoot.require('translate')


// IMPORT
// translateAttributes - takes 'attrs' and returns OSM 'tags'
function translateAttributes(attrs, layerName)
{
        tags = {};  // The final output Tag list

        if (config.getOgrDebugDumptags() == 'true') for (var i in attrs) print('In Attrs:' + i + ': :' + attrs[i] + ':');

        tags.tourism = 'hotel';

        // Default accuracy value
        tags['source:accuracy'] = '3';
        
        var txtBiased = {
            'address':'addr:full', // Address
            'countryCod':'is_in:country_code',
            'countryNam':'is_in:country',
            'hotelName':'name', // Name
            'stars':'hotel:star_rating',
            'stateName':'is_in:state', // State Name
            'price':'hotel:price', 
            'url':'website' 
        }
        
        translate.applySimpleTxtBiased(attrs, tags, txtBiased, 'forward');

        // Fix Price
        if (tags['hotel:price'] == '99999') delete tags['hotel:price'];

        // Fix stars
        if (tags['hotel:star_rating'] == '0') delete tags['hotel:star_rating'];

        
        // Sort out City Name Field
        if (attrs.cityName)
        {
            var cList = attrs.cityName.split('.');
            if (cList[0]) tags['is_in:city'] = cList[0];
            if (cList[1]) tags['is_in:state'] = cList[1];
            if (cList[2]) tags['is_in:country'] = cList[2];
        }
        
        // Metadata
        tags.source = 'HotelsBase';
        //tags.uuid = createUuid();
        tags.uuid = "hotelsbase:" + attrs.id;
        
        if (config.getOgrDebugDumptags() == 'true') for (var i in tags) print('Out Tags: ' + i + ': :' + tags[i] + ':');

        return tags;


} // End of Translate Attributes


