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
 * ' * @copyright Copyright ...'
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

//
// Convert NGA World Port Index shapefiles to OSM
//

hoot.require('config');
hoot.require('translate');
hoot.require('wpi_rules');


// IMPORT
function translateToOsm(attrs, layerName, geometryType)
{
  var tags = {};

  // Debug:
  if (config.getOgrDebugDumptags() == 'true') translate.debugOutput(attrs,layerName,geometryType,'','In attrs: ');

  translate.numToOSM(attrs, tags, wpi.numRules);
  translate.txtToOsm(attrs, tags, wpi.txtRules);

  // NOTE: The one2one rules have been converted to JSON
  translate.applyOne2OneQuiet(attrs, tags, wpi.one2one,{'k':'v'});

  tags.port = 'yes';
  tags.source = 'worldportindex';

  // Add a UUID
  tags.uuid = createUuid();

  // Debug:
  if (config.getOgrDebugDumptags() == 'true')
  {
    translate.debugOutput(tags,layerName,geometryType,'','Out tags: ');
    print('');
  }
  return tags;
} // End of Translate Attributes
