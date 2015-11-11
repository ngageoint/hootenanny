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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.validators.osm;

import java.sql.Connection;

import hoot.services.db2.QMaps;
import hoot.services.models.osm.ModelDaoUtils;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Verifies map data exists
 */
public class MapValidator
{
  private static final Logger log = LoggerFactory.getLogger(MapValidator.class);
  
  private Connection conn;
  
  public MapValidator(final Connection conn)
  {
    this.conn = conn;
  }
  
  /**
   * Determines whether a maps data has been prepared for review; more or less a wrapper with a
   * more identifiable name around ModelDaoUtils map functionality
   * 
   * @param mapIdStr map ID; may be a map ID or unique map name
   * @return the map's numeric ID
   * @throws Exception if the map doesn't exist
   */
  public long verifyMapExists(final String mapIdStr) throws Exception
  {
    log.debug("Checking maps table for map with ID: " + mapIdStr + " ...");
    //this will throw if it doesn't find the map
    QMaps maps = QMaps.maps;
    return ModelDaoUtils.getRecordIdForInputString(mapIdStr, conn, 
    		maps, maps.id, maps.displayName);
  }
}
