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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.validation;

import java.io.InputStream;

import org.openstreetmap.josm.data.osm.AbstractPrimitive;
import org.openstreetmap.josm.data.osm.OsmPrimitive;
import org.openstreetmap.josm.tools.Logging;
import org.openstreetmap.josm.io.OsmReader;
import org.openstreetmap.josm.data.osm.PrimitiveData;
import org.openstreetmap.josm.data.osm.DataSet;
import org.openstreetmap.josm.io.IllegalDataException;

/**
 * TODO
 */
public class HootOsmReader extends OsmReader
{
  protected OsmPrimitive buildPrimitive(PrimitiveData pd)
  {
    OsmPrimitive p;

    Logging.trace("primitive data unique id: " + pd.getUniqueId());
    Logging.trace("primitive data primitive id: " + pd.getPrimitiveId());

    p = pd.getType().newInstance(pd.getUniqueId(), true);
    Logging.trace("primitive id: " + p.getId());
    AbstractPrimitive.advanceUniqueId(pd.getUniqueId());
    p.setVisible(pd.isVisible());
    p.load(pd);

    externalIdMap.put(pd.getPrimitiveId(), p);

    return p;
  }

  /**
   * TODO
   */
  public static DataSet parseDataSet(InputStream source) throws IllegalDataException
  {
    Logging.debug("Converting input elements from xml with hoot reader...");
    return new HootOsmReader().doParseDataSet(source, null);
  }
}
