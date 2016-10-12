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

import static org.junit.Assert.*;

import java.util.Set;

import javax.ws.rs.BadRequestException;
import javax.ws.rs.client.Entity;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.w3c.dom.Document;

import hoot.services.UnitTest;
import hoot.services.geo.BoundingBox;
import hoot.services.testsupport.MapUtils;


public class ChangesetResourceUploadCommonTest extends OSMResourceTestAbstract {

    @Test
    @Category(UnitTest.class)
    public void testUploadPreflight() throws Exception {
        String responseData = target("api/0.6/changeset/1/upload")
                .queryParam("mapId", "1")
                .request(MediaType.TEXT_PLAIN)
                .options(String.class);

        assertEquals("", responseData);
    }

    @Test(expected = BadRequestException.class)
    @Category(UnitTest.class)
    public void testUploadMultipleChangesets() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Upload more than one changeset in the same request. A failure should
        // occur and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
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
                    "</osmChanges>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (BadRequestException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.BAD_REQUEST, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("Only one changeset may be uploaded at a time"));
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test(expected = BadRequestException.class)
    @Category(UnitTest.class)
    public void testUploadBadXml() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(null);

        // Upload a changeset with malformed XML. A failure should occur and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
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
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (BadRequestException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.BAD_REQUEST, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("Error parsing changeset diff data"));
            OSMTestUtils.verifyTestChangesetUnmodified(changesetId);
            assertFalse(MapUtils.elementDataExistsInServicesDb());
            throw e;
        }
    }

    @Test(expected = BadRequestException.class)
    @Category(UnitTest.class)
    public void testUploadEmptyChangeset() throws Exception {
        long changesetId = OSMTestUtils.createTestChangeset(null);

        // Upload a changeset with no items in it. A failure should occur and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create/>" +
                        "<modify/>" +
                        "<delete if-unused=\"true\"/>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (BadRequestException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.BAD_REQUEST, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("No items in uploaded changeset"));
            assertFalse(MapUtils.elementDataExistsInServicesDb());
            throw e;
        }
    }
}
