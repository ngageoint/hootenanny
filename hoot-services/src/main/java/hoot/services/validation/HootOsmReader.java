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

import org.openstreetmap.josm.data.osm.OsmPrimitive;
import org.openstreetmap.josm.tools.Logging;
import org.openstreetmap.josm.io.OsmReader;
import org.openstreetmap.josm.data.osm.PrimitiveData;
import org.openstreetmap.josm.data.osm.DataSet;
import org.openstreetmap.josm.io.IllegalDataException;

/**
 * Reads data for use by JOSM validation called by hoot
 *
 * The default behavior of OsmReader is to convert all negative ID elements to positive in
 * preparation for upload to the server. In hoot's case, we want to retain all ID's regardless...
 * positive/negative issues get handled when a changeset is created. Overriding buildPrimitive was
 * the only way found to make this happen...there may be a better way.
 */
public class HootOsmReader extends OsmReader
{
  /*
   * TODO
   */
  protected OsmPrimitive buildPrimitive(PrimitiveData pd)
  {
    OsmPrimitive p;

    //Logging.trace("primitive data unique id: " + pd.getUniqueId());
    //Logging.trace("primitive data primitive id: " + pd.getPrimitiveId());

    p = pd.getType().newInstance(pd.getUniqueId(), true);
    //Logging.trace("primitive id: " + p.getId());
    // This is in the parent method but blows up here.
    //AbstractPrimitive.advanceUniqueId(pd.getUniqueId());
    p.setVisible(pd.isVisible());
    p.load(pd);

    externalIdMap.put(pd.getPrimitiveId(), p);

    return p;
  }

  /**
   * @see OsmReader.parseDataSet
   */
  public static DataSet parseDataSet(InputStream source) throws IllegalDataException
  {
    //Logging.debug("Converting input elements from xml with hoot reader...");
    return new HootOsmReader().doParseDataSet(source, null);
  }
}
