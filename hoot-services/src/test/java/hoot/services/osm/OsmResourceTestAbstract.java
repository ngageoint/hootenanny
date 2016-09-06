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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.osm;

import org.glassfish.jersey.test.JerseyTest;
import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.utils.MapUtils;


/*
 * Base class for tests that need to read/write OSM data to the services database
 */
public abstract class OsmResourceTestAbstract extends JerseyTest {
    private static final Logger logger = LoggerFactory.getLogger(OsmResourceTestAbstract.class);

    protected long userId = -1;
    protected long mapId = -1;

    public OsmResourceTestAbstract(String... controllerGroup) {
        super();
    }

    @BeforeClass
    public static void beforeClass() throws Exception {}

    @Before
    public void beforeTest() throws Exception {
        try {
            userId = MapUtils.insertUser();
            mapId = MapUtils.insertMap(userId);
            OsmTestUtils.setUserId(userId);
            OsmTestUtils.setMapId(mapId);
        }
        catch (Exception e) {
            logger.error("{} ", e.getMessage());
            throw e;
        }
    }

    @After
    public void afterTest() throws Exception {
        // no need to clear out each map, if we're clearing the whole db out before each run
        MapUtils.deleteOSMRecord(mapId);
    }

    @AfterClass
    public static void afterClass() throws Exception {
    }
}
