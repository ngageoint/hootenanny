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
package hoot.services.controllers.osm;

import java.util.Set;

import javax.ws.rs.core.MediaType;

import org.junit.Assert;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.w3c.dom.Document;

import com.sun.jersey.api.client.ClientResponse;
import com.sun.jersey.api.client.ClientResponse.Status;
import com.sun.jersey.api.client.UniformInterfaceException;

import hoot.services.UnitTest;
import hoot.services.db.DbUtils;
import hoot.services.geo.BoundingBox;
import hoot.services.osm.OsmResourceTestAbstract;
import hoot.services.osm.OsmTestUtils;


public class ChangesetResourceUploadCommonTest extends OsmResourceTestAbstract {
    private static final Logger logger = LoggerFactory.getLogger(ChangesetResourceUploadCommonTest.class);

    public ChangesetResourceUploadCommonTest() {
        super("hoot.services.controllers.osm");
    }

    @Test
    @Category(UnitTest.class)
    public void testUploadPreflight() throws Exception {
        try {
            String responseData = null;
            try {
                String changesetId = "1";
                responseData = resource()
                        .path("api/0.6/changeset/" + changesetId + "/upload")
                        .queryParam("mapId", "1")
                        .type(MediaType.APPLICATION_FORM_URLENCODED)
                        .accept(MediaType.TEXT_PLAIN)
                        .options(String.class);
            }
            catch (UniformInterfaceException e) {
                ClientResponse r = e.getResponse();
                Assert.fail("Unexpected response " + r.getStatus() + " " + r.getEntity(String.class));
            }

            Assert.assertEquals("", responseData);
        }
        catch (Exception e) {
            logger.error(e.getMessage());
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadMultipleChangesets() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Upload more than one changeset in the same request. A failure should
        // occur and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
                        "<osmChanges>" +
                            "<osmChange version=\"0.3\" generator=\"iD\">" +
                                "<create>" +
                                    "<node id=\"-1\" lon=\"" + originalBounds.getMinLon() + "\" lat=\"" +
                                         originalBounds.getMinLat() + "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                                        "<tag k=\"name 1\" v=\"val 1\"/>" +
                                        "<tag k=\"name 2\" v=\"val 2\"/>" +
                                    "</node>" +
                                    "<node id=\"-2\" lon=\"" + originalBounds.getMinLon() + "\" lat=\"" +
                                        originalBounds.getMinLat() + "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                                       "<tag k=\"name 1\" v=\"val 1\"/>" +
                                    "</node>" +
                                "</create>" +
                                "<modify/>" +
                                "<delete if-unused=\"true\"/>" +
                            "</osmChange>" +
                                "<osmChange version=\"0.3\" generator=\"iD\">" +
                                "<create>" +
                                    "<node id=\"-1\" lon=\"" + originalBounds.getMinLon() + "\" lat=\"" +
                                        originalBounds.getMinLat() + "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                                        "<tag k=\"name 1\" v=\"val 1\"/>" +
                                        "<tag k=\"name 2\" v=\"val 2\"/>" +
                                    "</node>" +
                                    "<node id=\"-2\" lon=\"" + originalBounds.getMinLon() + "\" lat=\"" +
                                        originalBounds.getMinLat() + "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                                        "<tag k=\"name 1\" v=\"val 1\"/>" +
                                    "</node>" +
                                "</create>" +
                                "<modify/>" +
                                "<delete if-unused=\"true\"/>" +
                            "</osmChange>" +
                        "</osmChanges>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Only one changeset may be uploaded at a time"));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
        catch (Exception e) {
            logger.error(e.getMessage());
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadBadXml() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(null);

        // Upload a changeset with malformed XML. A failure should occur and no
        // data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
                        "<osmChange version=\"0.3\" generator=\"iD\"" + // missing closing tag
                            "<create>" +
                                "<node id=\"-1\" lon=\"" + originalBounds.getMinLon() + "\" lat=\"" +
                                       originalBounds.getMinLat() + "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                                    "<tag k=\"name 1\" v=\"val 1\"/>" +
                                    "<tag k=\"name 2\" v=\"val 2\"/>" +
                                "</node>" +
                                "<node id=\"-2\" lon=\"" + originalBounds.getMinLon() + "\" lat=\"" +
                                       originalBounds.getMinLat() + "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                                    "<tag k=\"name 1\" v=\"val 1\"/>" +
                                "</node>" +
                            "</create>" +
                            "<modify/>" +
                            "<delete if-unused=\"true\"/>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Error parsing changeset diff data"));

            OsmTestUtils.verifyTestChangesetUnmodified(changesetId);
            Assert.assertFalse(DbUtils.elementDataExistsInServicesDb(conn));

            throw e;
        }
        catch (Exception e) {
            logger.error(e.getMessage());
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadEmptyChangeset() throws Exception {
        long changesetId = OsmTestUtils.createTestChangeset(null);

        // Upload a changeset with no items in it. A failure should occur and no
        // data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
                        "<osmChange version=\"0.3\" generator=\"iD\">" +
                            "<create/>" +
                            "<modify/>" +
                            "<delete if-unused=\"true\"/>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("No items in uploaded changeset"));
            Assert.assertFalse(DbUtils.elementDataExistsInServicesDb(conn));
            throw e;
        }
    }
}
