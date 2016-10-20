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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.osm;

import static hoot.services.utils.DbUtils.createQuery;

import java.util.HashMap;

import org.junit.Assert;

import hoot.services.geo.BoundingBox;
import hoot.services.models.db.QCurrentNodes;
import hoot.services.models.osm.Changeset;
import hoot.services.models.osm.Element.ElementType;
import hoot.services.testsupport.MapUtils;


/**
 * This is simply in place for doing a rough performance check on changeset
 * writing. Its not meant to be run as part of the test suite.
 */
public class ChangesetDbWriterTest {
    private static final int NUM_NODES = 1000;
    private static final int NUM_TAGS_PER_NODE = 10;

    /*
     * @Test
     * 
     * @Category(IntegrationTest.class)
     */
    public void testLargeWrite() throws Exception {
        long userId = MapUtils.insertUser();
        long mapId = MapUtils.insertMap(userId);
        long changesetId = Changeset.insertNew(mapId, userId, new HashMap<String, String>());
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();

        String changeset = "<osmChange version=\"0.3\" generator=\"iD\">" + "<create>";
        for (int i = 0; i < NUM_NODES; i++) {
            changeset += "<node id=\"" + (i + 1) * -1 + "\" lon=\"" + originalBounds.getMinLon()
                    + "\" lat=\"" + originalBounds.getMinLat() + "\" version=\"0\" changeset=\"" + changesetId
                    + "\">";
            for (int j = 0; j < NUM_TAGS_PER_NODE; j++) {
                changeset += "<tag k=\"" + "tag " + (j + 1) + "\" v=\"" + (j + 1) + "\"/>";
            }
            changeset += "</node>";
        }
        changeset += "</create>" + "<modify/>" + "<delete if-unused=\"true\"/>" + "</osmChange>";

        /* final Document response = */new ChangesetDbWriter().write(mapId, changesetId, changeset);

        Assert.assertEquals(NUM_NODES,
                (int) createQuery(mapId)
                        .from(QCurrentNodes.currentNodes)
                        .fetchCount());

        Assert.assertEquals(NUM_NODES * NUM_TAGS_PER_NODE,
                OSMTestUtils.getTagCountForElementType(mapId, ElementType.Node));
    }
}
