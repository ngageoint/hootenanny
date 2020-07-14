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
    Geonames conversion script
        GeoNames -> OSM

    Based on translations/GeoNamesOrg.js script
*/

//__setupPackage__(__extension__);

geonames = {

  // Process the Alt Names
  // In the spec, these are "varchar(8000)". We are just passing them through but we could
  // split them if needed.
  processAltName : function (nameIn)
  {
    var nameOut = '';

    if (nameIn !== '')
    {
      nameOut = nameIn.split(',').join(';');
    }

    return nameOut;
  },


  // toOsm - Translate Attrs to Tags
  // This is the main routine to convert TO OSM
  toOsm : function(attrs, layerName, geometryType)
  {
    tags = {};  // The final output Tag list

    if (config.getOgrDebugDumptags() == 'true') for (var i in attrs) print('In Attrs:' + i + ': :' + attrs[i] + ':');

    // Names
    tags.name = attrs.name;
    if (attrs.alternatenames) tags.alt_name = geonames.processAltName(attrs.alternatenames);

    // Populations
    // var pop = Number(attrs.population);
    // if (pop > 0) tags.population = pop;
    if (Number(attrs.population) > 0) tags.population = attrs.population;

    // Elevations
    if (attrs.elevation && attrs.elevation !== '-9999')
    {
      tags.ele = attrs.elevation;
    }
    else if (attrs.dem && attrs.dem !== '-9999')
    {
      tags.ele = attrs.dem;
    }

    // Where are we?
    if (attrs.country_code) tags['is_in:country_code'] = attrs.country_code;

    // Feature Code
    if (geonames.rules.one2one[attrs.feature_code])
    {
      for (i in geonames.rules.one2one[attrs.feature_code]) tags[i] = geonames.rules.one2one[attrs.feature_code][i];

      // If it's just a POI, add "poi=yes"
      if (tags['poi:type']) tags.poi = 'yes';
    }

    // Metadata
    tags.source = 'geonames';
    // tags.uuid = attrs.geonameid;
    tags.uuid = 'GeoNames.org:' + attrs['geonameid'];

    if (config.getOgrDebugDumptags() == 'true') for (var i in tags) print('Out Tags: ' + i + ': :' + tags[i] + ':');

    return tags;
  }, // End of toOsm

}; // End of geonames
