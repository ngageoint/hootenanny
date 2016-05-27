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

import org.junit.Assert;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.mysema.query.sql.SQLQuery;

import hoot.services.db.DbUtils;
import hoot.services.db2.QCurrentNodes;
import hoot.services.geo.BoundingBox;
import hoot.services.models.osm.Changeset;
import hoot.services.models.osm.Element.ElementType;
import hoot.services.writers.osm.ChangesetDbWriter;


/**
 * This is simply in place for doing a rough performance check on changeset
 * writing. Its not meant to be run as part of the test suite.
 */
@SuppressWarnings("unused")
public class ChangesetDbWriterTest {
    private static final Logger log = LoggerFactory.getLogger(ChangesetDbWriterTest.class);

    private static final QCurrentNodes nodes = QCurrentNodes.currentNodes;

    private static final int NUM_NODES = 1000;
    private static final int NUM_TAGS_PER_NODE = 10;

    /*
     * @Test
     * 
     * @Category(IntegrationTest.class)
     */
    public void testLargeWrite() throws Exception {
        Connection conn = DbUtils.createConnection();
        final long userId = DbUtils.insertUser(conn);
        final long mapId = DbUtils.insertMap(userId, conn);
        final long changesetId = Changeset.insertNew(mapId, userId, conn);
        final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        try {
            String changeset = "<osmChange version=\"0.3\" generator=\"iD\">" + "<create>";
            for (int i = 0; i < NUM_NODES; i++) {
                changeset += "<node id=\"" + String.valueOf((i + 1) * -1) + "\" lon=\"" + originalBounds.getMinLon()
                        + "\" lat=\"" + originalBounds.getMinLat() + "\" version=\"0\" changeset=\"" + changesetId
                        + "\">";
                for (int j = 0; j < NUM_TAGS_PER_NODE; j++) {
                    changeset += "<tag k=\"" + "tag " + String.valueOf(j + 1) + "\" v=\"" + String.valueOf(j + 1)
                            + "\"/>";
                }
                changeset += "</node>";
            }
            changeset += "</create>" + "<modify/>" + "<delete if-unused=\"true\"/>" + "</osmChange>";

            log.info("Create elements test start.");
            /* final Document response = */new ChangesetDbWriter(conn).write(mapId, changesetId, changeset);
            log.info("Create elements test end.");

            Assert.assertEquals(NUM_NODES,
                    (int) new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(nodes).count());
            Assert.assertEquals(NUM_NODES * NUM_TAGS_PER_NODE,
                    OsmTestUtils.getTagCountForElementType(mapId, ElementType.Node, conn));
        }
        finally {
            // DbUtils.deleteOSMRecord(conn, mapId);
            conn.close();
        }
    }
}
