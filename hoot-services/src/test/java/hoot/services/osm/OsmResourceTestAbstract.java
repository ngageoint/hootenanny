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

import java.sql.Connection;

import org.joda.time.format.DateTimeFormat;
import org.joda.time.format.DateTimeFormatter;
import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.sun.jersey.api.client.AsyncWebResource;
import com.sun.jersey.test.framework.JerseyTest;

import hoot.services.HootProperties;
import hoot.services.db.DbUtils;
import hoot.services.review.ReviewTestUtils;


/*
 * Base class for tests that need to read/write OSM data to the services database
 */
public abstract class OsmResourceTestAbstract extends JerseyTest {
    private static final Logger log = LoggerFactory.getLogger(OsmResourceTestAbstract.class);

    // For whatever reason, when making Jersey async test calls you have to
    // specify the host and port,
    // whereas you do not with synchronous calls.
    protected static AsyncWebResource asyncTestResource;

    public static final int TEST_JOB_DELAY_MS = /* 125 */0;

    protected static DateTimeFormatter timeFormatter = DateTimeFormat.forPattern(DbUtils.TIMESTAMP_DATE_FORMAT);

    protected static long userId = -1;
    protected long mapId = -1;

    protected static Connection conn = null;

    public OsmResourceTestAbstract(final String... controllerGroup) throws NumberFormatException {
        super(controllerGroup);
        final int grizzlyPort = Integer.parseInt(HootProperties.getPropertyOrDefault("grizzlyPort"));
        asyncTestResource = client().asyncResource("http://localhost:" + String.valueOf(grizzlyPort));
    }

    @BeforeClass
    public static void beforeClass() throws Exception {
        try {
            conn = DbUtils.createConnection();
            OsmTestUtils.conn = conn;
            ReviewTestUtils.conn = conn;
            userId = DbUtils.insertUser(conn);
        }
        catch (Exception e) {
            DbUtils.closeConnection(conn);
            log.error(e.getMessage() + " ");
            throw e;
        }
    }

    @Before
    public void beforeTest() throws Exception {
        try {
            mapId = DbUtils.insertMap(userId, conn);

            OsmTestUtils.userId = userId;

            OsmTestUtils.mapId = mapId;
        }
        catch (Exception e) {
            log.error(e.getMessage() + " ");
            throw e;
        }
    }

    @After
    public void afterTest() throws Exception {
        try {
            // no need to clear out each map, if we're clearing the whole db out
            // before each run
            if (!Boolean.parseBoolean(HootProperties.getPropertyOrDefault("servicesTestClearEntireDb"))) {
                DbUtils.deleteOSMRecord(conn, mapId);
            }
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @AfterClass
    public static void afterClass() throws Exception {
        try {
            // DbUtils.deleteUser(conn, userId);
            OsmTestUtils.conn = null;
            ReviewTestUtils.conn = null;
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
        finally {
            DbUtils.closeConnection(conn);
        }
    }
}
