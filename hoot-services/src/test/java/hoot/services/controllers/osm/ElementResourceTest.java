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

import java.net.URLDecoder;
import java.util.ArrayList;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Set;

import javax.ws.rs.NotFoundException;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.xml.xpath.XPath;

import org.apache.xpath.XPathAPI;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.w3c.dom.Document;

import hoot.services.UnitTest;
import hoot.services.geo.BoundingBox;
import hoot.services.models.osm.Element.ElementType;
import hoot.services.models.osm.RelationMember;
import hoot.services.utils.RandomNumberGenerator;
import hoot.services.utils.XmlDocumentBuilder;
import hoot.services.utils.XmlUtils;


public class ElementResourceTest extends OSMResourceTestAbstract {

    private static void verifyFirstNodeWasReturned(Document responseData, String id, long changesetId,
            BoundingBox bounds) throws Exception {
        XPath xpath = XmlUtils.createXPath();

        OSMTestUtils.verifyOsmHeader(responseData);

        String data = XmlDocumentBuilder.toString(responseData);

        assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm/node").getLength());
        assertEquals(0, XPathAPI.selectNodeList(responseData, "//osm/way").getLength());
        assertEquals(0, XPathAPI.selectNodeList(responseData, "//osm/relation").getLength());

        OSMTestUtils.verifyNode(responseData, 1, id, changesetId, bounds.getMinLat(), bounds.getMinLon(),
                id.contains("_"));

        assertEquals(2, XPathAPI.selectNodeList(responseData, "//osm/node[1]/tag").getLength());
        assertEquals("key 1", xpath.evaluate("//osm/node[1]/tag[1]/@k", responseData));
        assertEquals("val 1", URLDecoder.decode(xpath.evaluate("//osm/node[1]/tag[1]/@v", responseData), "UTF-8"));
        assertEquals("key 2", xpath.evaluate("//osm/node[1]/tag[2]/@k", responseData));
        assertEquals("val 2", URLDecoder.decode(xpath.evaluate("//osm/node[1]/tag[2]/@v", responseData), "UTF-8"));
    }

    private static void verifyFirstWayWasReturned(Document responseData, String id, long changesetId,
            Set<Long> wayNodeIds) throws Exception {
        XPath xpath = XmlUtils.createXPath();

        OSMTestUtils.verifyOsmHeader(responseData);

        // TODO: needed?
        // Assert.assertEquals(0, XPathAPI.selectNodeList(responseData,
        // "//osm/node").getLength());
        assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm/way").getLength());
        assertEquals(0, XPathAPI.selectNodeList(responseData, "//osm/relation").getLength());

        OSMTestUtils.verifyWay(responseData, 1, id, changesetId, wayNodeIds, id.contains("_"));
        assertEquals(2, XPathAPI.selectNodeList(responseData, "//osm/way[1]/tag").getLength());
        assertEquals("key 1", xpath.evaluate("//osm/way[1]/tag[1]/@k", responseData));
        assertEquals("val 1", URLDecoder.decode(xpath.evaluate("//osm/way[1]/tag[1]/@v", responseData), "UTF-8"));
        assertEquals("key 2", xpath.evaluate("//osm/way[1]/tag[2]/@k", responseData));
        assertEquals("val 2", URLDecoder.decode(xpath.evaluate("//osm/way[1]/tag[2]/@v", responseData), "UTF-8"));

        if (wayNodeIds != null) {
            assertEquals(3, XPathAPI.selectNodeList(responseData, "//osm/way[1]/nd").getLength());
            Long[] wayNodeIdsArr = wayNodeIds.toArray(new Long[wayNodeIds.size()]);
            assertEquals(String.valueOf(wayNodeIdsArr[0]), xpath.evaluate("//osm/way[1]/nd[1]/@ref", responseData));
            assertEquals(String.valueOf(wayNodeIdsArr[1]), xpath.evaluate("//osm/way[1]/nd[2]/@ref", responseData));
            assertEquals(String.valueOf(wayNodeIdsArr[2]), xpath.evaluate("//osm/way[1]/nd[3]/@ref", responseData));
        }
    }

    private static void verifyFirstRelationWasReturned(Document responseData, String id, long changesetId,
            List<RelationMember> relationMembers) throws Exception {
        XPath xpath = XmlUtils.createXPath();

        OSMTestUtils.verifyOsmHeader(responseData);

        assertEquals(0, XPathAPI.selectNodeList(responseData, "//osm/node").getLength());
        assertEquals(0, XPathAPI.selectNodeList(responseData, "//osm/way").getLength());
        assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm/relation").getLength());

        OSMTestUtils.verifyRelation(responseData, 1, id, changesetId, relationMembers, id.contains("_"));

        assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm/relation[1]/tag").getLength());
        assertEquals("key 1", xpath.evaluate("//osm/relation[1]/tag[1]/@k", responseData));
        assertEquals("val 1", URLDecoder.decode(xpath.evaluate("//osm/relation[1]/tag[1]/@v", responseData), "UTF-8"));

        if (relationMembers != null) {
            assertEquals(4, XPathAPI.selectNodeList(responseData, "//osm/relation[1]/member").getLength());
            assertEquals(String.valueOf(relationMembers.get(0).getId()),
                    xpath.evaluate("//osm/relation[1]/member[1]/@ref", responseData));
            assertEquals(String.valueOf(relationMembers.get(1).getId()),
                    xpath.evaluate("//osm/relation[1]/member[2]/@ref", responseData));
            assertEquals(String.valueOf(relationMembers.get(2).getId()),
                    xpath.evaluate("//osm/relation[1]/member[3]/@ref", responseData));
            assertEquals(String.valueOf(relationMembers.get(3).getId()),
                    xpath.evaluate("//osm/relation[1]/member[4]/@ref", responseData));
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testGetNode() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        Document responseData = target("api/0.6/node/" + nodeIdsArr[0])
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .get(Document.class);

        assertNotNull(responseData);

        verifyFirstNodeWasReturned(responseData, String.valueOf(nodeIdsArr[0]), changesetId, originalBounds);
        OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
    }

    @Test
    @Category(UnitTest.class)
    public void testGetWay() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        Document responseData = target("api/0.6/way/" + wayIdsArr[0])
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .get(Document.class);

        assertNotNull(responseData);

        verifyFirstWayWasReturned(responseData, String.valueOf(wayIdsArr[0]), changesetId, null);
        OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
    }

    @Test
    @Category(UnitTest.class)
    public void testGetRelation() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        Document responseData = target("api/0.6/relation/" + relationIdsArr[0])
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .get(Document.class);

        assertNotNull(responseData);
        verifyFirstRelationWasReturned(responseData, String.valueOf(relationIdsArr[0]), changesetId, null);
        OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
    }

    @Test
    @Category(UnitTest.class)
    public void testGetWayFull() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        Document responseData = target("api/0.6/way/" + wayIdsArr[0] + "/full")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .get(Document.class);

        assertNotNull(responseData);

        Set<Long> wayNodes = new LinkedHashSet<>();
        wayNodes.add(nodeIdsArr[0]);
        wayNodes.add(nodeIdsArr[1]);
        wayNodes.add(nodeIdsArr[4]);

        verifyFirstWayWasReturned(responseData, String.valueOf(wayIdsArr[0]), changesetId, wayNodes);
        OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
    }

    @Test
    @Category(UnitTest.class)
    public void testGetRelationFull() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        Document responseData = target("api/0.6/relation/" + relationIdsArr[0] + "/full")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .get(Document.class);

        assertNotNull(responseData);

        List<RelationMember> members = new ArrayList<>();
        members.add(new RelationMember(nodeIdsArr[0], ElementType.Node, "role1"));
        members.add(new RelationMember(wayIdsArr[1], ElementType.Way, "role3"));
        members.add(new RelationMember(wayIdsArr[0], ElementType.Way, "role2"));
        members.add(new RelationMember(nodeIdsArr[2], ElementType.Node));

        verifyFirstRelationWasReturned(responseData, String.valueOf(relationIdsArr[0]), changesetId, members);
        OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
    }

    @Test(expected = NotFoundException.class)
    @Category(UnitTest.class)
    public void testGetNodeDoesntExist() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        try {
            target("api/0.6/node/" + nodeIdsArr[nodeIdsArr.length - 1] + 1)
                    .queryParam("mapId", String.valueOf(mapId))
                    .request(MediaType.TEXT_XML)
                    .get(Document.class);
        }
        catch (NotFoundException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.NOT_FOUND, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("does not exist"));
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test(expected = NotFoundException.class)
    @Category(UnitTest.class)
    public void testGetWayDoesntExist() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        try {
            target("api/0.6/way/" + wayIdsArr[wayIdsArr.length - 1] + 1)
                    .queryParam("mapId", String.valueOf(mapId))
                    .request(MediaType.TEXT_XML)
                    .get(Document.class);
        }
        catch (NotFoundException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.NOT_FOUND, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("does not exist"));
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test(expected = NotFoundException.class)
    @Category(UnitTest.class)
    public void testGetWayFullDoesntExist() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        try {
            target("api/0.6/way/" + wayIdsArr[wayIdsArr.length - 1] + 1 + "/full")
                    .queryParam("mapId", String.valueOf(mapId))
                    .request(MediaType.TEXT_XML)
                    .get(Document.class);
        }
        catch (NotFoundException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.NOT_FOUND, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("does not exist"));
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test(expected = NotFoundException.class)
    @Category(UnitTest.class)
    public void testGetRelationDoesntExist() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        try {
            target("api/0.6/relation/" + relationIdsArr[relationIdsArr.length - 1] + 1)
                    .queryParam("mapId", String.valueOf(mapId))
                    .request(MediaType.TEXT_XML)
                    .get(Document.class);
        }
        catch (NotFoundException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.NOT_FOUND, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("does not exist"));
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test(expected = NotFoundException.class)
    @Category(UnitTest.class)
    public void testGetRelationFullDoesntExist() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        try {
            target("api/0.6/relation/" + relationIdsArr[relationIdsArr.length - 1] + 1 + "/full")
                    .queryParam("mapId", String.valueOf(mapId))
                    .request(MediaType.TEXT_XML)
                    .get(Document.class);
        }
        catch (NotFoundException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.NOT_FOUND, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("does not exist"));
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    // Technically, could test these map params for each element type, but the
    // map ID checking code should always be the same for all element types.
    @Test(expected = NotFoundException.class)
    @Category(UnitTest.class)
    public void testGetElementMapDoesntExist() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        try {
            target("api/0.6/node/" + nodeIdsArr[0])
                    .queryParam("mapId", String.valueOf((int) RandomNumberGenerator.nextDouble(mapId + 10 ^ 4, Integer.MAX_VALUE)))
                    .request(MediaType.TEXT_XML)
                    .get(Document.class);
        }
        catch (NotFoundException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.NOT_FOUND, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("No map exists"));
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test(expected = NotFoundException.class)
    @Category(UnitTest.class)
    public void testGetFullElementMapDoesntExist() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        try {
            target("api/0.6/way/" + wayIdsArr[0] + "/full")
                    .queryParam("mapId", String.valueOf((int) RandomNumberGenerator.nextDouble(mapId + 10 ^ 4, Integer.MAX_VALUE)))
                    .request(MediaType.TEXT_XML)
                    .get(Document.class);
        }
        catch (NotFoundException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.NOT_FOUND, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("No map exists"));
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test(expected = NotFoundException.class)
    @Category(UnitTest.class)
    public void testGetInvalidElementType() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        try {
            // invalid element type
            target("api/0.6/blah/" + nodeIdsArr[0]).request(MediaType.TEXT_XML).get(Document.class);
        }
        catch (NotFoundException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.NOT_FOUND, Response.Status.fromStatusCode(r.getStatus()));
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test(expected = NotFoundException.class)
    @Category(UnitTest.class)
    public void testGetFullInvalidElementType() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        try {
            // invalid element type
            target("api/0.6/blah/" + nodeIdsArr[0] + "/full").request(MediaType.TEXT_XML).get(Document.class);
        }
        catch (NotFoundException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.NOT_FOUND, Response.Status.fromStatusCode(r.getStatus()));
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    /*
     * A call to get a full node is not allowed, since nodes have no children
     * (like ways and relations do) and, therefore, the call makes no sense.
     */
    @Test(expected = NotFoundException.class)
    @Category(UnitTest.class)
    public void testGetNodeFull() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        try {
            target("api/0.6/node/" + nodeIdsArr[0] + "/full")
                    .queryParam("mapId", String.valueOf(mapId))
                    .request(MediaType.TEXT_XML)
                    .get(Document.class);
        }
        catch (NotFoundException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.NOT_FOUND, Response.Status.fromStatusCode(r.getStatus()));
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }
}
