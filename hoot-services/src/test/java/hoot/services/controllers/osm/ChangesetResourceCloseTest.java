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

import static com.querydsl.core.group.GroupBy.groupBy;
import static hoot.services.HootProperties.*;
import static hoot.services.models.db.QChangesets.changesets;
import static hoot.services.models.db.QCurrentNodes.currentNodes;
import static hoot.services.models.db.QCurrentRelationMembers.currentRelationMembers;
import static hoot.services.models.db.QCurrentRelations.currentRelations;
import static hoot.services.models.db.QCurrentWayNodes.currentWayNodes;
import static hoot.services.models.db.QCurrentWays.currentWays;
import static hoot.services.utils.DbUtils.createQuery;
import static org.junit.Assert.*;

import java.sql.Timestamp;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import javax.ws.rs.ClientErrorException;
import javax.ws.rs.NotFoundException;
import javax.ws.rs.client.Entity;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status;
import javax.xml.xpath.XPath;

import org.apache.xpath.XPathAPI;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.w3c.dom.Document;
import org.w3c.dom.NodeList;

import hoot.services.UnitTest;
import hoot.services.geo.BoundingBox;
import hoot.services.models.db.Changesets;
import hoot.services.models.db.CurrentNodes;
import hoot.services.models.db.CurrentRelationMembers;
import hoot.services.models.db.CurrentRelations;
import hoot.services.models.db.CurrentWayNodes;
import hoot.services.models.db.CurrentWays;
import hoot.services.models.osm.Changeset;
import hoot.services.testsupport.HootCustomPropertiesSetter;
import hoot.services.utils.DbUtils;
import hoot.services.utils.PostgresUtils;
import hoot.services.utils.QuadTileCalculator;
import hoot.services.utils.XmlUtils;


public class ChangesetResourceCloseTest extends OSMResourceTestAbstract {

    @Test
    @Category(UnitTest.class)
    public void testCloseChangeset() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);

        // close the changeset via the service
        Response response = target("api/0.6/changeset/" + changesetId + "/close")
                .queryParam("mapId", mapId)
                .request(MediaType.APPLICATION_JSON)
                .put(Entity.text(""), Response.class);

        assertEquals(200, response.getStatus());

        OSMTestUtils.verifyTestChangesetClosed(changesetId);
    }

    @Test(expected = ClientErrorException.class)
    @Category(UnitTest.class)
    public void testCloseClosedChangeset() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);

        // close the changeset
        OSMTestUtils.closeChangeset(mapId, changesetId);

        try {
            // Try to close an already closed changeset. A failure should occur
            // and no data in the system should be modified.
            target("api/0.6/changeset/" + changesetId + "/close")
                    .queryParam("mapId", String.valueOf(mapId))
                    .request(MediaType.APPLICATION_JSON)
                    .put(Entity.text(""), String.class);
        }
        catch (ClientErrorException e) {
            Response r = e.getResponse();
            assertEquals(Status.CONFLICT, Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("The changeset with ID: " + changesetId + " was closed at"));
            OSMTestUtils.verifyTestChangesetUnmodified(changesetId, originalBounds);
            OSMTestUtils.verifyTestChangesetClosed(changesetId);
            throw e;
        }
    }

    @Test(expected = NotFoundException.class)
    @Category(UnitTest.class)
    public void testCloseNonExistingChangeset() throws Exception {
        // Try to close a changeset that doesn't exist. A failure should occur
        // and no data in system should be modified.
        try {
            // close a changeset that doesn't exist
            target("api/0.6/changeset/1/close")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.APPLICATION_JSON)
                .put(Entity.text(""), String.class);
        }
        catch (NotFoundException e) {
            Response r = e.getResponse();
            assertEquals(Status.NOT_FOUND, Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("Changeset to be updated does not exist"));
            throw e;
        }
    }

    @Test(expected = ClientErrorException.class)
    @Category(UnitTest.class)
    public void testChangesetMaxElementsExceededUploadedToEmptyChangeset() throws Exception {
        String original_MAXIMUM_CHANGESET_ELEMENTS = MAXIMUM_CHANGESET_ELEMENTS;
        try {
            // lower the max allowed elements per changeset from the default
            int maximumChangesetElements = 2;
            HootCustomPropertiesSetter.setProperty("MAXIMUM_CHANGESET_ELEMENTS", String.valueOf(maximumChangesetElements));

            BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
            long changesetId = OSMTestUtils.createTestChangeset(originalBounds, 0);

            // Now create a new changeset with a number of elements larger than
            // the max allowed. A failure should occur and no data in the system should be modified.
            try {
                target("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", mapId)
                    .request(MediaType.TEXT_XML)
                    .post(Entity.entity(
                        "<osmChange version=\"0.3\" generator=\"iD\">" +
                            "<create>" +
                                "<node id=\"-1\" lon=\"" + originalBounds.getMinLon() +
                                      "\" lat=\"" + originalBounds.getMinLat() +
                                      "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                                    "<tag k=\"key 1\" v=\"val 1\"/>" + "<tag k=\"key 2\" v=\"val 2\"/>" +
                                "</node>" +
                                "<node id=\"-2\" lon=\"" + originalBounds.getMaxLon() +
                                      "\" lat=\"" + originalBounds.getMaxLat() + "\" version=\"0\" changeset=\"" +
                                      changesetId + "\">" + "</node>" + "<node id=\"-3\" lon=\"" +
                                      originalBounds.getMinLon() + "\" lat=\"" + originalBounds.getMinLat() +
                                      "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                                "</node>" +
                            "</create>" +
                            "<modify/>" +
                            "<delete if-unused=\"true\"/>" +
                        "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
            }
            catch (ClientErrorException e) {
                Response r = e.getResponse();
                assertEquals(Status.CONFLICT, Status.fromStatusCode(r.getStatus()));
                assertTrue(r.readEntity(String.class).contains("Changeset maximum element threshold exceeded"));

                Changesets changeset = createQuery(mapId)
                        .select(changesets)
                        .from(changesets)
                        .where(changesets.id.eq(changesetId))
                        .fetchOne();

                assertNotNull(changeset);

                Timestamp now = new Timestamp(System.currentTimeMillis());

                assertTrue(changeset.getCreatedAt().before(now));
                assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));
                assertEquals(new Integer(0), changeset.getNumChanges());
                assertEquals(new Long(userId), changeset.getUserId());

                // make sure the changeset bounds wasn't updated
                Changeset hootChangeset = new Changeset(mapId, changesetId);
                BoundingBox changesetBounds = hootChangeset.getBounds();
                BoundingBox defaultBounds = new BoundingBox();

                // a change the size of the expansion factor is made
                // automatically, so the changeset's bounds should be no larger than that
                defaultBounds.expand(originalBounds, Double.parseDouble(CHANGESET_BOUNDS_EXPANSION_FACTOR_DEEGREES));
                assertEquals(defaultBounds, changesetBounds);
                throw e;
            }
        }
        finally {
            // restore original properties since they are at defined at the class level
            HootCustomPropertiesSetter.setProperty("MAXIMUM_CHANGESET_ELEMENTS", original_MAXIMUM_CHANGESET_ELEMENTS);
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testChangesetAutoCloseWhenMaxElementsUploadedToEmptyChangeset() throws Exception {
        String original_MAXIMUM_CHANGESET_ELEMENTS = MAXIMUM_CHANGESET_ELEMENTS;

        try {
            // lower the max allowed elements per changeset from the default to the
            // number of elements we're uploading
            int maximumChangesetElements = 12;
            HootCustomPropertiesSetter.setProperty("MAXIMUM_CHANGESET_ELEMENTS", String.valueOf(maximumChangesetElements));

            BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
            long changesetId = OSMTestUtils.createTestChangeset(originalBounds, 0);

            // Now create a new changeset with a number of elements equal to the
            // max allowed. The elements should be written and the changeset closed.
            Document responseData = target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create>" +
                            "<node id=\"-1\" lon=\"" + originalBounds.getMinLon() + "\" lat=\"" +
                                   originalBounds.getMinLat() + "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                                "<tag k=\"key 1\" v=\"val 1\"/>" + "<tag k=\"key 2\" v=\"val 2\"/>" +
                            "</node>" +
                            "<node id=\"-2\" lon=\"" + originalBounds.getMaxLon() + "\" lat=\"" +
                                   originalBounds.getMaxLat() + "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                            "</node>" +
                            "<node id=\"-3\" lon=\"" + originalBounds.getMinLon() + "\" lat=\"" +
                                   originalBounds.getMinLat() + "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                            "</node>" +
                            "<node id=\"-4\" lon=\"" + originalBounds.getMinLon() + "\" lat=\"" +
                                   originalBounds.getMinLat() + "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                                "<tag k=\"key 3\" v=\"val 3\"/>" + "</node>" + "<node id=\"-5\" lon=\""
                                       + originalBounds.getMinLon() + "\" lat=\"" + originalBounds.getMinLat() +
                                    "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                                "<tag k=\"key 4\" v=\"val 4\"/>" +
                            "</node>" +
                            "<way id=\"-6\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                "<nd ref=\"-1\"></nd>" +
                                "<nd ref=\"-2\"></nd>" +
                                "<nd ref=\"-5\"></nd>" +
                                "<tag k=\"key 1\" v=\"val 1\"/>" +
                                "<tag k=\"key 2\" v=\"val 2\"/>" +
                            "</way>" +
                            "<way id=\"-7\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                "<nd ref=\"-3\"></nd>" + "<nd ref=\"-2\"></nd>" +
                            "</way>" +
                            "<way id=\"-8\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                "<nd ref=\"-1\"></nd>" +
                                "<nd ref=\"-2\"></nd>" +
                                "<tag k=\"key 3\" v=\"val 3\"/>" +
                            "</way>" +
                            "<relation id=\"-9\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                "<member type=\"node\" role=\"role1\" ref=\"-1\"></member>" +
                                "<member type=\"way\" role=\"role3\" ref=\"-7\"></member>" +
                                "<member type=\"way\" role=\"role2\" ref=\"-6\"></member>" +
                                "<member type=\"node\" ref=\"-3\"></member>" + "<tag k=\"key 1\" v=\"val 1\"/>" +
                            "</relation>" +
                            "<relation id=\"-10\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                "<member type=\"node\" role=\"role1\" ref=\"-5\"></member>" +
                                "<member type=\"relation\" role=\"role1\" ref=\"-9\"></member>" +
                                "<tag k=\"key 2\" v=\"val 2\"/>" + "<tag k=\"key 3\" v=\"val 3\"/>" +
                            "</relation>" +
                            "<relation id=\"-11\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                "<member type=\"way\" role=\"\" ref=\"-7\"></member>" +
                                "<tag k=\"key 4\" v=\"val 4\"/>" +
                            "</relation>" +
                            "<relation id=\"-12\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                "<member type=\"node\" role=\"role1\" ref=\"-3\"></member>" +
                            "</relation>" +
                        "</create>" +
                        "<modify/>" +
                        "<delete if-unused=\"true\"/>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);

            assertNotNull(responseData);

            XPath xpath = XmlUtils.createXPath();

            NodeList returnedNodes = XPathAPI.selectNodeList(responseData, "//osm/diffResult/node");
            assertEquals(5, returnedNodes.getLength());

            long oldElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@old_id", responseData));
            assertEquals(-1, oldElementId);

            long newElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@new_id", responseData));
            assertNotSame(-1, newElementId);

            Set<Long> nodeIds = new LinkedHashSet<>();
            nodeIds.add(newElementId);
            assertEquals(1, Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@new_version", responseData)));

            oldElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@old_id", responseData));
            assertEquals(-2, oldElementId);

            newElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@new_id", responseData));
            assertNotSame(-2, newElementId);

            nodeIds.add(newElementId);
            assertEquals(Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@new_id", responseData)) + 1,
                         newElementId);
            assertEquals(1, Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@new_version", responseData)));

            oldElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/node[3]/@old_id", responseData));
            assertEquals(-3, oldElementId);

            newElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/node[3]/@new_id", responseData));
            assertNotSame(-3, newElementId);

            nodeIds.add(newElementId);
            assertEquals(Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@new_id", responseData)) + 1,
                         newElementId);
            assertEquals(1, Long.parseLong(xpath.evaluate("//osm/diffResult/node[3]/@new_version", responseData)));

            oldElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/node[4]/@old_id", responseData));
            assertEquals(-4, oldElementId);

            newElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/node[4]/@new_id", responseData));
            assertNotSame(-4, newElementId);

            nodeIds.add(newElementId);
            assertEquals(Long.parseLong(xpath.evaluate("//osm/diffResult/node[3]/@new_id", responseData)) + 1,
                         newElementId);
            assertEquals(1, Long.parseLong(xpath.evaluate("//osm/diffResult/node[4]/@new_version", responseData)));

            oldElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/node[5]/@old_id", responseData));
            assertEquals(-5, oldElementId);

            newElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/node[5]/@new_id", responseData));
            assertNotSame(-5, newElementId);

            nodeIds.add(newElementId);
            assertEquals(Long.parseLong(xpath.evaluate("//osm/diffResult/node[4]/@new_id", responseData)) + 1,
                         newElementId);
            assertEquals(1, Long.parseLong(xpath.evaluate("//osm/diffResult/node[5]/@new_version", responseData)));

            NodeList returnedWays = XPathAPI.selectNodeList(responseData, "//osm/diffResult/way");
            assertEquals(3, returnedWays.getLength());

            oldElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@old_id", responseData));
            assertEquals(-6, oldElementId);

            newElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@new_id", responseData));
            assertNotSame(-6, newElementId);

            Set<Long> wayIds = new LinkedHashSet<>();
            wayIds.add(newElementId);
            assertEquals(1, Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@new_version", responseData)));

            oldElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/way[2]/@old_id", responseData));
            assertEquals(-7, oldElementId);

            newElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/way[2]/@new_id", responseData));
            assertNotSame(-7, newElementId);

            wayIds.add(newElementId);
            assertEquals(1, Long.parseLong(xpath.evaluate("//osm/diffResult/way[2]/@new_version", responseData)));

            oldElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/way[3]/@old_id", responseData));
            assertEquals(-8, oldElementId);

            newElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/way[3]/@new_id", responseData));
            assertNotSame(-8, newElementId);

            wayIds.add(newElementId);
            assertEquals(1, Long.parseLong(xpath.evaluate("//osm/diffResult/way[3]/@new_version", responseData)));

            NodeList returnedRelations = XPathAPI.selectNodeList(responseData, "//osm/diffResult/relation");
            assertEquals(4, returnedRelations.getLength());

            oldElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@old_id", responseData));
            assertEquals(-9, oldElementId);

            newElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@new_id", responseData));
            assertNotSame(-9, newElementId);

            Set<Long> relationIds = new LinkedHashSet<>();
            relationIds.add(newElementId);
            assertEquals(1, Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@new_version", responseData)));

            oldElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/relation[2]/@old_id", responseData));
            assertEquals(-10, oldElementId);

            newElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/relation[2]/@new_id", responseData));
            assertNotSame(-10, newElementId);

            relationIds.add(newElementId);
            assertEquals(1, Long.parseLong(xpath.evaluate("//osm/diffResult/relation[2]/@new_version", responseData)));

            oldElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/relation[3]/@old_id", responseData));
            assertEquals(-11, oldElementId);

            newElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/relation[3]/@new_id", responseData));
            assertNotSame(-11, newElementId);

            relationIds.add(newElementId);
            assertEquals(1, Long.parseLong(xpath.evaluate("//osm/diffResult/relation[3]/@new_version", responseData)));

            oldElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/relation[4]/@old_id", responseData));
            assertEquals(-12, oldElementId);

            newElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/relation[4]/@new_id", responseData));
            assertNotSame(-12, newElementId);

            relationIds.add(newElementId);
            assertEquals(1, Long.parseLong(xpath.evaluate("//osm/diffResult/relation[4]/@new_version", responseData)));

            // changes have actually occurred with the upload of the
            // changeset...what's actually being done here is to compare the state of the
            // default test data set with the dataset we uploaded here, and they should match each other
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            OSMTestUtils.verifyTestChangesetClosed(changesetId);
        }
        finally {
            // restore original properties since they are at defined at the class level
            HootCustomPropertiesSetter.setProperty("MAXIMUM_CHANGESET_ELEMENTS", original_MAXIMUM_CHANGESET_ELEMENTS);
        }
    }

    @Test(expected = ClientErrorException.class)
    @Category(UnitTest.class)
    public void testChangesetMaxElementsExceededUploadedToExistingChangeset() throws Exception {
        String original_MAXIMUM_CHANGESET_ELEMENTS = MAXIMUM_CHANGESET_ELEMENTS;

        try {
            // lower the max allowed elements per changeset from the default
            int maximumChangesetElements = 16;
            HootCustomPropertiesSetter.setProperty("MAXIMUM_CHANGESET_ELEMENTS", String.valueOf(maximumChangesetElements));

            BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
            long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
            Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
            Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
            Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
            Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
            Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
            Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

            BoundingBox updatedBounds = OSMTestUtils.createAfterModifiedTestChangesetBounds();
            // Now update an existing changeset with a number of elements larger than
            // the max allowed. A failure should occur and no data in the system should be modified.
            try {
                target("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .request(MediaType.TEXT_XML)
                    .post(Entity.entity(
                        "<osmChange version=\"0.3\" generator=\"iD\">" +
                            "<create/>" +
                            "<modify>" +
                                "<node id=\"" + nodeIdsArr[0] + "\" lon=\"" + updatedBounds.getMinLon() + "\" lat=\"" +
                                        updatedBounds.getMinLat() + "\" version=\"1\" changeset=\"" + changesetId + "\">" +
                                "</node>" +
                                "<node id=\"" + nodeIdsArr[1] + "\" lon=\"" + originalBounds.getMaxLon() +
                                        "\" lat=\"" + updatedBounds.getMinLat() + "\" version=\"1\" changeset=\"" +
                                        changesetId + "\">" +
                                "</node>" +
                                "<way id=\"" + wayIdsArr[0] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                    "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                    "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                                "</way>" +
                                "<way id=\"" + wayIdsArr[1] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                    "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                    "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                                "</way>" +
                                "<relation id=\"" + relationIdsArr[0] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                    "<member type=\"way\" role=\"role4\" ref=\"" + wayIdsArr[1] + "\"></member>" +
                                    "<member type=\"way\" role=\"role2\" ref=\"" + wayIdsArr[0] + "\"></member>" +
                                    "<member type=\"node\" ref=\"" + nodeIdsArr[2] + "\"></member>" +
                                "</relation>" +
                                "<relation id=\"" + relationIdsArr[1] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                    "<member type=\"relation\" role=\"role1\" ref=\"" + relationIdsArr[0] + "\"></member>" +
                                    "<member type=\"node\" ref=\"" + nodeIdsArr[4] + "\"></member>" +
                                "</relation>" +
                            "</modify>" +
                            "<delete if-unused=\"true\"/>" +
                        "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
            }
            catch (ClientErrorException e) {
                Response r = e.getResponse();
                assertEquals(Status.CONFLICT, Status.fromStatusCode(r.getStatus()));
                assertTrue(r.readEntity(String.class).contains("Changeset maximum element threshold exceeded"));
                OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
                throw e;
            }
        }
        finally {
            // restore original properties since they are at defined at the class level
            HootCustomPropertiesSetter.setProperty("MAXIMUM_CHANGESET_ELEMENTS", original_MAXIMUM_CHANGESET_ELEMENTS);
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testChangesetAutoCloseWhenMaxElementsUploadedToExistingChangeset() throws Exception {
        String original_MAXIMUM_CHANGESET_ELEMENTS = MAXIMUM_CHANGESET_ELEMENTS;

        try {
            // lower the max allowed elements per changeset from the default to the
            // sum of number of elements existing currently in changeset + what we're adding
            int maximumChangesetElements = 18;
            HootCustomPropertiesSetter.setProperty("MAXIMUM_CHANGESET_ELEMENTS", String.valueOf(maximumChangesetElements));

            BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
            long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
            Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
            Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
            Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
            Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
            Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
            Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

            BoundingBox updatedBounds = OSMTestUtils.createAfterModifiedTestChangesetBounds();

            // Now update an existing changeset with a number of elements that,
            // when combined with the existing elements, are equal to the max allowed. The elements
            // should be written and the changeset closed.
            Document responseData = target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create/>" +
                        "<modify>" +
                            "<node id=\"" + nodeIdsArr[0] + "\" lon=\"" + updatedBounds.getMinLon() + "\" " +
                                "lat=\"" + updatedBounds.getMinLat() + "\" version=\"1\" changeset=\"" + changesetId + "\">" +
                                "<tag k=\"key 1b\" v=\"val 1b\"></tag>" +
                                "<tag k=\"key 2b\" v=\"val 2b\"></tag>" +
                            "</node>" +
                            "<node id=\"" + nodeIdsArr[1] + "\" lon=\"" + originalBounds.getMaxLon() + "\" " + "lat=\"" +
                                 updatedBounds.getMinLat() + "\" version=\"1\" changeset=\"" + changesetId + "\">" +
                                 "<tag k=\"key 3b\" v=\"val 3b\"></tag>" +
                            "</node>" +
                            "<way id=\"" + wayIdsArr[0] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                "<nd ref=\"" + nodeIdsArr[4] + "\"></nd>" +
                                "<tag k=\"key 2\" v=\"val 2\"></tag>" +
                            "</way>" +
                            "<way id=\"" + wayIdsArr[1] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                "<nd ref=\"" + nodeIdsArr[4] + "\"></nd>" +
                                "<nd ref=\"" + nodeIdsArr[2] + "\"></nd>" +
                            "</way>" +
                            "<relation id=\"" + relationIdsArr[0] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                "<member type=\"way\" role=\"role4\" ref=\"" + wayIdsArr[1] + "\"></member>" +
                                "<member type=\"way\" role=\"role2\" ref=\"" + wayIdsArr[0] + "\"></member>" +
                                "<member type=\"node\" ref=\"" + nodeIdsArr[2] + "\"></member>" +
                            "</relation>" +
                            "<relation id=\"" + relationIdsArr[1] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                "<member type=\"relation\" role=\"role1\" ref=\"" + relationIdsArr[0] + "\"></member>" +
                                "<member type=\"node\" ref=\"" + nodeIdsArr[4] + "\"></member>" +
                                "<tag k=\"key 2\" v=\"val 2\"></tag>" +
                                "<tag k=\"key 3\" v=\"val 3\"></tag>" +
                            "</relation>" +
                        "</modify>" +
                        "<delete if-unused=\"true\"/>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);

            assertNotNull(responseData);

            XPath xpath = XmlUtils.createXPath();
            NodeList returnedNodes = XPathAPI.selectNodeList(responseData, "//osm/diffResult/node");
            assertEquals(2, returnedNodes.getLength());

            assertEquals((long) nodeIdsArr[0],
                    Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@old_id", responseData)));

            assertEquals(Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@old_id", responseData)),
                         Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@new_id", responseData)));

            assertEquals(2, Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@new_version", responseData)));

            assertEquals((long) nodeIdsArr[1],
                    Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@old_id", responseData)));

            assertEquals(Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@old_id", responseData)),
                    Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@new_id", responseData)));

            assertEquals(2, Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@new_version", responseData)));

            NodeList returnedWays = XPathAPI.selectNodeList(responseData, "//osm/diffResult/way");
            assertEquals(2, returnedWays.getLength());

            assertEquals((long) wayIdsArr[0],
                    Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@old_id", responseData)));

            assertEquals(Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@old_id", responseData)),
                    Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@new_id", responseData)));

            assertEquals(2, Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@new_version", responseData)));

            assertEquals((long) wayIdsArr[1],
                    Long.parseLong(xpath.evaluate("//osm/diffResult/way[2]/@old_id", responseData)));

            assertEquals(Long.parseLong(xpath.evaluate("//osm/diffResult/way[2]/@old_id", responseData)),
                    Long.parseLong(xpath.evaluate("//osm/diffResult/way[2]/@new_id", responseData)));

            assertEquals(2, Long.parseLong(xpath.evaluate("//osm/diffResult/way[2]/@new_version", responseData)));

            NodeList returnedRelations = XPathAPI.selectNodeList(responseData, "//osm/diffResult/relation");
            assertEquals(2, returnedRelations.getLength());

            // check the modified relations
            assertEquals((long) relationIdsArr[0],
                    Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@old_id", responseData)));

            assertEquals(Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@old_id", responseData)),
                    Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@new_id", responseData)));

            assertEquals(2, Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@new_version", responseData)));

            assertEquals((long) relationIdsArr[1],
                    Long.parseLong(xpath.evaluate("//osm/diffResult/relation[2]/@old_id", responseData)));

            assertEquals(
                    Long.parseLong(xpath.evaluate("//osm/diffResult/relation[2]/@old_id", responseData)),
                    Long.parseLong(xpath.evaluate("//osm/diffResult/relation[2]/@new_id", responseData)));

            assertEquals(2, Long.parseLong(xpath.evaluate("//osm/diffResult/relation[2]/@new_version", responseData)));

            Timestamp now = super.getCurrentDBTime();

            Changesets changeset = createQuery(mapId)
                    .select(changesets)
                    .from(changesets)
                    .where(changesets.id.eq(changesetId))
                    .fetchOne();

            Map<Long, CurrentNodes> nodes = createQuery(mapId)
                    .from(currentNodes)
                    .transform(groupBy(currentNodes.id).as(currentNodes));

            assertEquals(5, nodes.size());

            CurrentNodes nodeRecord = nodes.get(nodeIdsArr[0]);

            assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
            assertEquals(new Double((updatedBounds.getMinLat())), nodeRecord.getLatitude());
            assertEquals(new Double((updatedBounds.getMinLon())), nodeRecord.getLongitude());
            assertEquals(nodeIdsArr[0], nodeRecord.getId());
            assertEquals(new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                    nodeRecord.getTile());
            assertTrue(nodeRecord.getTimestamp().before(now));
            assertEquals(new Long(2), nodeRecord.getVersion());
            assertTrue(nodeRecord.getVisible());

            Map<String, String> tags = PostgresUtils.postgresObjToHStore(nodeRecord.getTags());

            assertNotNull(tags);
            assertEquals(2, tags.size());
            assertEquals("val 1b", tags.get("key 1b"));
            assertEquals("val 2b", tags.get("key 2b"));

            nodeRecord = nodes.get(nodeIdsArr[1]);
            assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
            assertEquals(new Double(updatedBounds.getMinLat()), nodeRecord.getLatitude());
            assertEquals(new Double(updatedBounds.getMaxLon()), nodeRecord.getLongitude());
            assertEquals(nodeIdsArr[1], nodeRecord.getId());
            assertEquals(new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                    nodeRecord.getTile());
            assertTrue(nodeRecord.getTimestamp().before(now));
            assertEquals(new Long(2), nodeRecord.getVersion());
            assertEquals(true, nodeRecord.getVisible());

            tags = PostgresUtils.postgresObjToHStore(nodeRecord.getTags());

            assertNotNull(tags);
            assertEquals(1, tags.size());
            assertEquals("val 3b", tags.get("key 3b"));

            nodeRecord = nodes.get(nodeIdsArr[3]);
            tags = PostgresUtils.postgresObjToHStore(nodeRecord.getTags());

            assertNotNull(tags);
            assertEquals(1, tags.size());
            assertEquals("val 3", tags.get("key 3"));

            nodeRecord = nodes.get(nodeIdsArr[4]);
            tags = PostgresUtils.postgresObjToHStore(nodeRecord.getTags());

            assertNotNull(tags);
            assertEquals(1, tags.size());
            assertEquals("val 4", tags.get("key 4"));

            Map<Long, CurrentWays> ways = createQuery(mapId)
                    .from(currentWays)
                    .transform(groupBy(currentWays.id).as(currentWays));

            assertEquals(3, ways.size());

            CurrentWays wayRecord = ways.get(wayIdsArr[0]);
            assertEquals(new Long(changesetId), wayRecord.getChangesetId());
            assertEquals(wayIdsArr[0], wayRecord.getId());
            assertTrue(wayRecord.getTimestamp().before(now));
            assertEquals(new Long(2), wayRecord.getVersion());
            assertTrue(wayRecord.getVisible());

            List<CurrentWayNodes> wayNodes = createQuery(mapId)
                    .select(currentWayNodes)
                    .from(currentWayNodes)
                    .where(currentWayNodes.wayId.eq(wayIdsArr[0]))
                    .orderBy(currentWayNodes.sequenceId.asc())
                    .fetch();

            assertEquals(2, wayNodes.size());

            CurrentWayNodes wayNode = wayNodes.get(0);
            assertEquals(nodeIdsArr[0], wayNode.getNodeId());
            assertEquals(new Long(1), wayNode.getSequenceId());
            assertEquals(wayRecord.getId(), wayNode.getWayId());

            wayNode = wayNodes.get(1);
            assertEquals(nodeIdsArr[4], wayNode.getNodeId());
            assertEquals(new Long(2), wayNode.getSequenceId());
            assertEquals(wayRecord.getId(), wayNode.getWayId());

            // verify the previously existing tags
            tags = PostgresUtils.postgresObjToHStore(wayRecord.getTags());
            assertNotNull(tags);
            assertEquals(1, tags.size());
            assertEquals("val 2", tags.get("key 2"));

            wayRecord = ways.get(wayIdsArr[1]);
            assertEquals(new Long(changesetId), wayRecord.getChangesetId());
            assertEquals(wayIdsArr[1], wayRecord.getId());
            assertTrue(wayRecord.getTimestamp().before(now));
            assertEquals(new Long(2), wayRecord.getVersion());
            assertTrue(wayRecord.getVisible());

            wayNodes = createQuery(mapId)
                    .select(currentWayNodes)
                    .from(currentWayNodes)
                    .where(currentWayNodes.wayId.eq(wayIdsArr[1]))
                    .orderBy(currentWayNodes.sequenceId.asc())
                    .fetch();

            assertEquals(2, wayNodes.size());

            wayNode = wayNodes.get(0);
            assertEquals(nodeIdsArr[4], wayNode.getNodeId());
            assertEquals(new Long(1), wayNode.getSequenceId());
            assertEquals(wayRecord.getId(), wayNode.getWayId());

            wayNode = wayNodes.get(1);
            assertEquals(nodeIdsArr[2], wayNode.getNodeId());
            assertEquals(new Long(2), wayNode.getSequenceId());
            assertEquals(wayRecord.getId(), wayNode.getWayId());

            // verify the way with no tags
            assertTrue((wayRecord.getTags() == null)
                    || PostgresUtils.postgresObjToHStore(wayRecord.getTags()).isEmpty());

            // verify the created ways
            wayRecord = ways.get(wayIdsArr[2]);
            assertEquals(new Long(changesetId), wayRecord.getChangesetId());
            assertEquals(wayIdsArr[2], wayRecord.getId());
            assertTrue(wayRecord.getTimestamp().before(now));
            assertEquals(new Long(1), wayRecord.getVersion());
            assertEquals(true, wayRecord.getVisible());

            wayNodes = createQuery(mapId)
                    .select(currentWayNodes)
                    .from(currentWayNodes)
                    .where(currentWayNodes.wayId.eq(wayIdsArr[2]))
                    .orderBy(currentWayNodes.sequenceId.asc())
                    .fetch();

            assertEquals(2, wayNodes.size());

            wayNode = wayNodes.get(0);
            assertEquals(nodeIdsArr[0], wayNode.getNodeId());
            assertEquals(new Long(1), wayNode.getSequenceId());
            assertEquals(wayRecord.getId(), wayNode.getWayId());

            wayNode = wayNodes.get(1);
            assertEquals(nodeIdsArr[1], wayNode.getNodeId());
            assertEquals(new Long(2), wayNode.getSequenceId());
            assertEquals(wayRecord.getId(), wayNode.getWayId());

            // verify the created tags
            tags = PostgresUtils.postgresObjToHStore(wayRecord.getTags());

            assertNotNull(tags);
            assertEquals(1, tags.size());
            assertEquals("val 3", tags.get("key 3"));

            Map<Long, CurrentRelations> relations =
                    createQuery(mapId)
                            .from(currentRelations)
                            .transform(groupBy(currentRelations.id).as(currentRelations));

            assertEquals(4, relations.size());

            CurrentRelations relationRecord = relations.get(relationIdsArr[0]);
            assertEquals(new Long(changesetId), relationRecord.getChangesetId());
            assertEquals(relationIdsArr[0], relationRecord.getId());
            assertTrue(relationRecord.getTimestamp().before(now));
            assertEquals(new Long(2), relationRecord.getVersion());
            assertTrue(relationRecord.getVisible());

            List<CurrentRelationMembers> members =
                    createQuery(mapId)
                            .select(currentRelationMembers)
                            .from(currentRelationMembers)
                            .where(currentRelationMembers.relationId.eq(relationIdsArr[0]))
                            .orderBy(currentRelationMembers.sequenceId.asc())
                            .fetch();

            assertEquals(3, members.size());

            CurrentRelationMembers member = members.get(0);
            assertEquals(relationRecord.getId(), member.getRelationId());
            assertEquals(DbUtils.nwr_enum.way, member.getMemberType());
            assertEquals("role4", member.getMemberRole());
            assertEquals(new Integer(1), member.getSequenceId());
            assertEquals(wayIdsArr[1], member.getMemberId());

            member = members.get(1);
            assertEquals(relationRecord.getId(), member.getRelationId());
            assertEquals(DbUtils.nwr_enum.way, member.getMemberType());
            assertEquals("role2", member.getMemberRole());
            assertEquals(new Integer(2), member.getSequenceId());
            assertEquals(wayIdsArr[0], member.getMemberId());

            member = members.get(2);
            assertEquals(relationRecord.getId(), member.getRelationId());
            assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
            assertEquals("", member.getMemberRole());
            assertEquals(new Integer(3), member.getSequenceId());
            assertEquals(nodeIdsArr[2], member.getMemberId());

            tags = PostgresUtils.postgresObjToHStore(relationRecord.getTags());

            assertTrue((tags == null) || tags.isEmpty());

            relationRecord = relations.get(relationIdsArr[1]);
            assertEquals(new Long(changesetId), relationRecord.getChangesetId());
            assertEquals(relationIdsArr[1], relationRecord.getId());
            assertTrue(relationRecord.getTimestamp().before(now));
            assertEquals(new Long(2), relationRecord.getVersion());
            assertTrue(relationRecord.getVisible());

            members = createQuery(mapId)
                    .select(currentRelationMembers)
                    .from(currentRelationMembers)
                    .where(currentRelationMembers.relationId.eq(relationIdsArr[1]))
                    .orderBy(currentRelationMembers.sequenceId.asc())
                    .fetch();

            assertEquals(2, members.size());

            member = members.get(0);
            assertEquals(relationRecord.getId(), member.getRelationId());
            assertEquals(DbUtils.nwr_enum.relation, member.getMemberType());
            assertEquals("role1", member.getMemberRole());
            assertEquals(new Integer(1), member.getSequenceId());
            assertEquals(relationIdsArr[0], member.getMemberId());

            member = members.get(1);
            assertEquals(relationRecord.getId(), member.getRelationId());
            assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
            assertEquals("", member.getMemberRole());
            assertEquals(new Integer(2), member.getSequenceId());
            assertEquals(nodeIdsArr[4], member.getMemberId());

            tags = PostgresUtils.postgresObjToHStore(relationRecord.getTags());
            assertNotNull(tags);
            assertEquals(2, tags.size());
            assertEquals("val 2", tags.get("key 2"));
            assertEquals("val 3", tags.get("key 3"));

            relationRecord = relations.get(relationIdsArr[2]);
            assertEquals(new Long(changesetId), relationRecord.getChangesetId());
            assertEquals(relationIdsArr[2], relationRecord.getId());
            assertTrue(relationRecord.getTimestamp().before(now));
            assertEquals(new Long(1), relationRecord.getVersion());
            assertTrue(relationRecord.getVisible());

            members = createQuery(mapId)
                    .select(currentRelationMembers)
                    .from(currentRelationMembers)
                    .where(currentRelationMembers.relationId.eq(relationIdsArr[2]))
                    .orderBy(currentRelationMembers.sequenceId.asc())
                    .fetch();

            assertEquals(1, members.size());

            member = members.get(0);
            assertEquals(relationRecord.getId(), member.getRelationId());
            assertEquals(DbUtils.nwr_enum.way, member.getMemberType());
            assertEquals("", member.getMemberRole());
            assertEquals(new Integer(1), member.getSequenceId());
            assertEquals(wayIdsArr[1], member.getMemberId());

            tags = PostgresUtils.postgresObjToHStore(relationRecord.getTags());

            assertNotNull(tags);
            assertEquals(1, tags.size());
            assertEquals("val 4", tags.get("key 4"));

            relationRecord = relations.get(relationIdsArr[3]);

            assertEquals(new Long(changesetId), relationRecord.getChangesetId());
            assertEquals(relationIdsArr[3], relationRecord.getId());
            assertTrue(relationRecord.getTimestamp().before(now));
            assertEquals(new Long(1), relationRecord.getVersion());
            assertTrue(relationRecord.getVisible());

            members = createQuery(mapId)
                    .select(currentRelationMembers)
                    .from(currentRelationMembers)
                    .where(currentRelationMembers.relationId.eq(relationIdsArr[3]))
                    .orderBy(currentRelationMembers.sequenceId.asc())
                    .fetch();

            assertEquals(1, members.size());

            member = members.get(0);
            assertEquals(relationRecord.getId(), member.getRelationId());
            assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
            assertEquals("role1", member.getMemberRole());
            assertEquals(new Integer(1), member.getSequenceId());
            assertEquals(nodeIdsArr[2], member.getMemberId());
            assertTrue((relationRecord.getTags() == null)
                    || PostgresUtils.postgresObjToHStore(relationRecord.getTags()).isEmpty());

            assertNotNull(changeset);
            assertTrue(changeset.getCreatedAt().before(now));
            assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));
            assertTrue(changeset.getClosedAt().before(now));
            assertEquals(new Integer(18), changeset.getNumChanges());
            assertEquals(new Long(userId), changeset.getUserId());

            BoundingBox expandedBounds = new BoundingBox(originalBounds);
            expandedBounds.expand(updatedBounds, Double.parseDouble(CHANGESET_BOUNDS_EXPANSION_FACTOR_DEEGREES));
            Changeset hootChangeset = new Changeset(mapId, changesetId);
            BoundingBox changesetBounds = hootChangeset.getBounds();
            assertEquals(changesetBounds, expandedBounds);
        }
        finally {
            // restore original hoot properties since they are shared at the class level
            HootCustomPropertiesSetter.setProperty("MAXIMUM_CHANGESET_ELEMENTS", original_MAXIMUM_CHANGESET_ELEMENTS);
        }
    }

    @Test(expected = ClientErrorException.class)
    @Category(UnitTest.class)
    public void testChangesetAutoCloseWhenNoElementsAddedToItBeforeExpiration() throws Exception {
        String original_TEST_CHANGESET_AUTO_CLOSE = TEST_CHANGESET_AUTO_CLOSE;
        String original_CHANGESET_IDLE_TIMEOUT_MINUTES = CHANGESET_IDLE_TIMEOUT_MINUTES;

        try {
            // A changeset is created, but no elements are added to it. It
            // should auto-close once changesetIdleTimeoutMinutes time has passed, since that's when
            // the changeset is set to auto-close upon its creation.

            // Set these props at the beginning, since they are read by OsmResourceTestUtils.createTestChangeset

            // Toggle the var that allows for testing changeset auto-closing.
            // This will change the service to temporarily interpret changesetIdleTimeoutMinutes as a value
            // in seconds instead of minutes to enable a faster runtime for this test.
            HootCustomPropertiesSetter.setProperty("TEST_CHANGESET_AUTO_CLOSE", String.valueOf(true));

            // set the timeout to one second
            int changesetIdleTimeoutSeconds = 1;
            HootCustomPropertiesSetter.setProperty("CHANGESET_IDLE_TIMEOUT_MINUTES",
                    String.valueOf(changesetIdleTimeoutSeconds));

            BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
            long changesetId = OSMTestUtils.createTestChangeset(originalBounds, 0);

            // pause long enough for the changeset to expire
            Thread.sleep(2000);

            // Access the changeset with a create request. This will trigger
            // closing the changeset, and no data in the system should be modified by the create request.
            try {
                target("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .request(MediaType.TEXT_XML)
                    .post(Entity.entity(
                        "<osmChange version=\"0.3\" generator=\"iD\">" +
                            "<create>" +
                                "<node id=\"-1\" lon=\"" + originalBounds.getMinLon() +
                                      "\" lat=\"" + originalBounds.getMinLat() +
                                      "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                                    "<tag k=\"key 1\" v=\"val 1\"/>" + "<tag k=\"key 2\" v=\"val 2\"/>" +
                                "</node>" +
                                "<node id=\"-2\" lon=\"" + originalBounds.getMaxLon() + "\" lat=\"" +
                                     originalBounds.getMaxLat() + "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                                "</node>" +
                            "</create>" +
                            "<modify/>" +
                            "<delete if-unused=\"true\"/>" +
                        "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
            }
            catch (ClientErrorException e) {
                Response r = e.getResponse();
                assertEquals(Status.CONFLICT, Status.fromStatusCode(r.getStatus()));
                assertTrue(r.readEntity(String.class).contains("The changeset with ID: "
                        + changesetId + " was closed at"));
                OSMTestUtils.verifyTestChangesetClosed(changesetId, 0);
                throw e;
            }
        }
        finally {
            // restore original hoot properties since they are shared at the class level
            HootCustomPropertiesSetter.setProperty("CHANGESET_IDLE_TIMEOUT_MINUTES", original_CHANGESET_IDLE_TIMEOUT_MINUTES);
            HootCustomPropertiesSetter.setProperty("TEST_CHANGESET_AUTO_CLOSE", original_TEST_CHANGESET_AUTO_CLOSE);
        }
    }

    @Test(expected = ClientErrorException.class)
    @Category(UnitTest.class)
    public void testChangesetAutoCloseWhenLengthBetweenUpdatesCausesExpiration() throws Exception {
        String original_TEST_CHANGESET_AUTO_CLOSE = TEST_CHANGESET_AUTO_CLOSE;
        String original_CHANGESET_IDLE_TIMEOUT_MINUTES = CHANGESET_IDLE_TIMEOUT_MINUTES;
        String original_CHANGESET_MAX_OPEN_TIME_HOURS = CHANGESET_MAX_OPEN_TIME_HOURS;

        try {
            // A changeset is created and elements are added to it. After that,
            // a time longer than changesetMaxOpenTimeHours passes before attempting to add any
            // more elements to it. A changeset's expiration increases from changesetIdleTimeoutMinutes
            // to changesetMaxOpenTimeHours after a single element is written to it. The changeset should auto-close.

            // Set these props at the beginning, since they are read by OsmResourceTestUtils.createTestChangeset

            // Toggle the var that allows for testing changeset auto-closing.
            // This will change the serviceto temporarily interpret changesetIdleTimeoutMinutes as a value
            // in seconds instead of minutes to enable a faster runtime for this test.
            HootCustomPropertiesSetter.setProperty("TEST_CHANGESET_AUTO_CLOSE", String.valueOf(true));

            // set the timeout to one second
            int changesetIdleTimeoutSeconds = 1;
            HootCustomPropertiesSetter.setProperty("CHANGESET_IDLE_TIMEOUT_MINUTES",
                    String.valueOf(changesetIdleTimeoutSeconds));

            int changesetMaxOpenTimeSeconds = 2;
            HootCustomPropertiesSetter.setProperty("CHANGESET_MAX_OPEN_TIME_HOURS",
                    String.valueOf(changesetMaxOpenTimeSeconds));

            BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
            long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
            Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
            Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
            Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
            Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

            // pause long enough for the changeset to expire
            Thread.sleep(3000);

            BoundingBox updateBounds = OSMTestUtils.createAfterModifiedTestChangesetBounds();

            // Access the changeset with an update request. This will trigger
            // closing the changeset, and No data in the system should be modified by the update request.
            try {
                target("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .request(MediaType.TEXT_XML)
                    .post(Entity.entity(
                        "<osmChange version=\"0.3\" generator=\"iD\">" +
                            "<create/>" +
                            "<modify>" +
                                "<node id=\"" + nodeIdsArr[0] + "\" lon=\"" + updateBounds.getMinLon() + "\" "
                                       + "lat=\"" + updateBounds.getMinLat() + "\" version=\"1\" changeset=\"" +
                                       changesetId + "\">" +
                                    "<tag k=\"key 1\" v=\"val 1\"></tag>" + "<tag k=\"key 2\" v=\"val 2\"></tag>" +
                                "</node>" +
                            "</modify>" +
                            "<delete if-unused=\"true\"/>" +
                        "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
            }
            catch (ClientErrorException e) {
                Response r = e.getResponse();
                assertEquals(Status.CONFLICT, Status.fromStatusCode(r.getStatus()));
                assertTrue(r.readEntity(String.class).contains("The changeset with ID: " + changesetId + " was closed at"));

                // make sure nothing was updated
                OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
                OSMTestUtils.verifyTestChangesetClosed(changesetId);
                throw e;
            }
        }
        finally {
            // restore original hoot properties since they are shared at the class level
            HootCustomPropertiesSetter.setProperty("CHANGESET_IDLE_TIMEOUT_MINUTES", original_CHANGESET_IDLE_TIMEOUT_MINUTES);
            HootCustomPropertiesSetter.setProperty("CHANGESET_MAX_OPEN_TIME_HOURS", original_CHANGESET_MAX_OPEN_TIME_HOURS);
            HootCustomPropertiesSetter.setProperty("TEST_CHANGESET_AUTO_CLOSE", original_TEST_CHANGESET_AUTO_CLOSE);
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testClosePreflight() throws Exception {
        target("api/0.6/changeset/1/close").queryParam("mapId", "1").request().options();
    }
}
