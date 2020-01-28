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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.grail;

import static org.junit.Assert.assertEquals;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.List;

import org.junit.Test;

public class PullConnectedWaysTest {

    @Test
    public void testGetOsmXpath() throws IOException {
        InputStream is;

        //Read the crop.osm file
        is = new FileInputStream(new File("src/test/resources/hoot/services/controllers/grail/crop.osm"));
        List<Long> nodeIds = PullConnectedWaysCommand.getOsmXpath(is, "/osm/node/@id");
        assertEquals(77, nodeIds.size());
        is.close();

        is = new FileInputStream(new File("src/test/resources/hoot/services/controllers/grail/crop.osm"));
        List<Long> wayIds = PullConnectedWaysCommand.getOsmXpath(is, "/osm/way/@id");
        assertEquals(0, wayIds.size());
        is.close();

        //Read the reference.osm file
        is = new FileInputStream(new File("src/test/resources/hoot/services/controllers/grail/reference.osm"));
        nodeIds = PullConnectedWaysCommand.getOsmXpath(is, "/osm/node/@id");
        assertEquals(91, nodeIds.size());
        is.close();

        is = new FileInputStream(new File("src/test/resources/hoot/services/controllers/grail/reference.osm"));
        wayIds = PullConnectedWaysCommand.getOsmXpath(is, "/osm/way/@id");
        assertEquals(6, wayIds.size());
        is.close();
    }

}
