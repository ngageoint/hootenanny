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
import static hoot.services.HootProperties.CHANGESET_BOUNDS_EXPANSION_FACTOR_DEEGREES;
import static hoot.services.HootProperties.MAXIMUM_WAY_NODES;
import static hoot.services.models.db.QChangesets.changesets;
import static hoot.services.models.db.QCurrentNodes.currentNodes;
import static hoot.services.models.db.QCurrentRelations.currentRelations;
import static hoot.services.models.db.QCurrentWayNodes.currentWayNodes;
import static hoot.services.models.db.QCurrentWays.currentWays;
import static hoot.services.utils.DbUtils.createQuery;
import static org.junit.Assert.*;

import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import javax.ws.rs.BadRequestException;
import javax.ws.rs.ClientErrorException;
import javax.ws.rs.NotFoundException;
import javax.ws.rs.client.Entity;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
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
import hoot.services.models.db.CurrentRelations;
import hoot.services.models.db.CurrentWayNodes;
import hoot.services.models.db.CurrentWays;
import hoot.services.models.osm.Changeset;
import hoot.services.models.osm.Element.ElementType;
import hoot.services.testsupport.HootCustomPropertiesSetter;
import hoot.services.testsupport.MapUtils;
import hoot.services.utils.QuadTileCalculator;
import hoot.services.utils.XmlUtils;


public class ChangesetResourceUploadCreateTest extends OSMResourceTestAbstract {

    @Test
    @Category(UnitTest.class)
    public void testUploadCreate() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(null, 0);

        // Now update it with new OSM elements; make this create the
        // equivalent of the default dataset which is being created
        // outside of the service call for all other tests, so the
        // default dataset verification methods can be used.
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
                        "<node id=\"-3\" lon=\"" + originalBounds.getMinLon() + "\" lat=\"" + originalBounds.getMinLat() +
                            "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                        "</node>" +
                        "<node id=\"-4\" lon=\"" + originalBounds.getMinLon() + "\" lat=\"" + originalBounds.getMinLat() +
                                "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                            "<tag k=\"key 3\" v=\"val 3\"/>" + "</node>" + "<node id=\"-5\" lon=\"" + originalBounds.getMinLon() +
                               "\" lat=\"" + originalBounds.getMinLat() + "\" version=\"0\" changeset=\"" + changesetId + "\">" +
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
                            "<nd ref=\"-1\"></nd>" + "<nd ref=\"-2\"></nd>" +
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
                            "<tag k=\"key 4\" v=\"val 4\"/>" + "</relation>" +
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
        // changeset...what's actually being done here is to compare the state of the default test data set
        // with the dataset we uploaded here, and they should match each other
        OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
    }

    @Test
    @Category(UnitTest.class)
    public void testUploadCreateClosedPolygon() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(null, 0);

        Document responseData = target("api/0.6/changeset/" + changesetId + "/upload")
            .queryParam("mapId", String.valueOf(mapId))
            .request(MediaType.TEXT_XML)
            .post(Entity.entity(
                "<osmChange version=\"0.3\" generator=\"iD\">" +
                    "<create>" +
                        "<node id=\"-1\" lon=\"" + originalBounds.getMinLon() + "\" lat=\"" +
                            originalBounds.getMinLat() + "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                        "</node>" +
                        "<node id=\"-2\" lon=\"" + originalBounds.getMaxLon() + "\" lat=\"" + originalBounds.getMaxLat() +
                            "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                        "</node>" +
                        "<node id=\"-3\" lon=\"" + (originalBounds.getMinLon() - .001) + "\" lat=\"" +
                            (originalBounds.getMinLat() - .001) + "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                        "</node>" +
                        "<way id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                            "<nd ref=\"-1\"></nd>" + "<nd ref=\"-2\"></nd>" +
                            "<nd ref=\"-3\"></nd>" + "<nd ref=\"-1\"></nd>" +
                        "</way>" +
                    "</create>" +
                    "<modify/>" +
                    "<delete if-unused=\"true\"/>" +
                "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);

        assertNotNull(responseData);

        XPath xpath = XmlUtils.createXPath();

        NodeList returnedNodes = XPathAPI.selectNodeList(responseData, "//osm/diffResult/node");
        assertEquals(3, returnedNodes.getLength());

        long oldElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@old_id", responseData));
        assertEquals(-1, oldElementId);

        long newElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@new_id", responseData));
        assertNotSame(-1, newElementId);

        List<Long> nodeIds = new ArrayList<>();
        nodeIds.add(newElementId);
        assertEquals(1, Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@new_version", responseData)));

        oldElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@old_id", responseData));
        assertEquals(-2, oldElementId);

        newElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@new_id", responseData));
        assertNotSame(-2, newElementId);

        nodeIds.add(newElementId);
        assertEquals(1, Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@new_version", responseData)));

        oldElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/node[3]/@old_id", responseData));
        assertEquals(-3, oldElementId);

        newElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/node[3]/@new_id", responseData));
        assertNotSame(-3, newElementId);

        nodeIds.add(newElementId);
        assertEquals(1, Long.parseLong(xpath.evaluate("//osm/diffResult/node[3]/@new_version", responseData)));

        NodeList returnedWays = XPathAPI.selectNodeList(responseData, "//osm/diffResult/way");
        assertEquals(1, returnedWays.getLength());

        oldElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@old_id", responseData));
        assertEquals(-1, oldElementId);

        newElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@new_id", responseData));
        assertNotSame(-1, newElementId);

        Set<Long> wayIds = new LinkedHashSet<>();
        wayIds.add(newElementId);
        assertEquals(1, Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@new_version", responseData)));

        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);

        Timestamp now = super.getCurrentDBTime();

        Changesets changeset = createQuery(mapId)
                .select(changesets)
                .from(changesets)
                .where(changesets.id.eq(changesetId))
                .fetchOne();

        Map<Long, CurrentNodes> nodes = createQuery(mapId)
                .from(currentNodes)
                .transform(groupBy(currentNodes.id).as(currentNodes));

        assertEquals(3, nodes.size());

        CurrentNodes nodeRecord = nodes.get(nodeIdsArr[0]);
        assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
        assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
        assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
        assertEquals(nodeIdsArr[0], nodeRecord.getId());
        assertEquals(new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                nodeRecord.getTile());
        assertTrue(nodeRecord.getTimestamp().before(now));
        assertEquals(new Long(1), nodeRecord.getVersion());
        assertEquals(true, nodeRecord.getVisible());

        nodeRecord = nodes.get(nodeIdsArr[1]);
        assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
        assertEquals(new Double(originalBounds.getMaxLat()), nodeRecord.getLatitude());
        assertEquals(new Double(originalBounds.getMaxLon()), nodeRecord.getLongitude());
        assertEquals(nodeIdsArr[1], nodeRecord.getId());
        assertEquals(new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                nodeRecord.getTile());
        assertTrue(nodeRecord.getTimestamp().before(now));
        assertEquals(new Long(1), nodeRecord.getVersion());
        assertEquals(true, nodeRecord.getVisible());

        nodeRecord = nodes.get(nodeIdsArr[2]);
        assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
        assertEquals(new Double(originalBounds.getMinLat() - .001), nodeRecord.getLatitude());
        assertEquals(new Double(originalBounds.getMinLon() - .001), nodeRecord.getLongitude());
        assertEquals(nodeIdsArr[2], nodeRecord.getId());
        assertEquals(new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                nodeRecord.getTile());
        assertTrue(nodeRecord.getTimestamp().before(now));
        assertEquals(new Long(1), nodeRecord.getVersion());
        assertEquals(true, nodeRecord.getVisible());

        Map<Long, CurrentWays> ways = createQuery(mapId)
                .from(currentWays)
                .transform(groupBy(currentWays.id).as(currentWays));

        assertEquals(1, ways.size());

        CurrentWays wayRecord = ways.get(wayIdsArr[0]);
        assertEquals(new Long(changesetId), wayRecord.getChangesetId());
        assertEquals(wayIdsArr[0], wayRecord.getId());
        assertTrue(wayRecord.getTimestamp().before(now));
        assertEquals(new Long(1), wayRecord.getVersion());
        assertTrue(wayRecord.getVisible());

        List<CurrentWayNodes> wayNodes = createQuery(mapId)
                .select(currentWayNodes)
                .from(currentWayNodes)
                .where(currentWayNodes.wayId.eq(wayIdsArr[0]))
                .orderBy(currentWayNodes.sequenceId.asc())
                .fetch();

        assertEquals(4, wayNodes.size());

        CurrentWayNodes wayNode = wayNodes.get(0);
        assertEquals(nodeIdsArr[0], wayNode.getNodeId());
        assertEquals(new Long(1), wayNode.getSequenceId());
        assertEquals(wayRecord.getId(), wayNode.getWayId());

        wayNode = wayNodes.get(1);
        assertEquals(nodeIdsArr[1], wayNode.getNodeId());
        assertEquals(new Long(2), wayNode.getSequenceId());
        assertEquals(wayRecord.getId(), wayNode.getWayId());

        wayNode = wayNodes.get(2);
        assertEquals(nodeIdsArr[2], wayNode.getNodeId());
        assertEquals(new Long(3), wayNode.getSequenceId());
        assertEquals(wayRecord.getId(), wayNode.getWayId());

        wayNode = wayNodes.get(3);
        assertEquals(nodeIdsArr[0], wayNode.getNodeId());
        assertEquals(new Long(4), wayNode.getSequenceId());
        assertEquals(wayRecord.getId(), wayNode.getWayId());
        assertNotNull(changeset);
        assertTrue(changeset.getCreatedAt().before(now));
        assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));
        assertEquals(new Integer(4), changeset.getNumChanges());
        assertEquals(new Long(userId), changeset.getUserId());

        BoundingBox expandedBounds = new BoundingBox(originalBounds);
        BoundingBox updatedBounds = new BoundingBox((originalBounds.getMinLon() - .001),
                (originalBounds.getMinLat() - .001), originalBounds.getMaxLon(), originalBounds.getMaxLat());
        expandedBounds.expand(updatedBounds, Double.parseDouble(CHANGESET_BOUNDS_EXPANSION_FACTOR_DEEGREES));

        Changeset hootChangeset = new Changeset(mapId, changesetId);
        BoundingBox changesetBounds = hootChangeset.getBounds();
        assertEquals(changesetBounds, expandedBounds);
    }

    @Test
    @Category(UnitTest.class)
    public void testUploadCreateElementNoTags() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds, 0);

        // now update the changeset with new nodes which have no tags, which is allowed
        Document responseData = target("api/0.6/changeset/" + changesetId + "/upload")
            .queryParam("mapId", String.valueOf(mapId))
            .request(MediaType.TEXT_XML)
            .post(Entity.entity("<osmChange version=\"0.3\" generator=\"iD\">" +
                "<create>" +
                    "<node id=\"-1\" lon=\"" + originalBounds.getMinLon() + "\" lat=\"" +
                        originalBounds.getMinLat() + "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                    "</node>" +
                    "<node id=\"-2\" lon=\"" + originalBounds.getMaxLon() + "\" lat=\"" +
                        originalBounds.getMaxLat() + "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                    "</node>" +
                    "<node id=\"-3\" lon=\"" + originalBounds.getMinLon() + "\" lat=\"" +
                        originalBounds.getMinLat() + "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                    "</node>" +
                    "<node id=\"-4\" lon=\"" + originalBounds.getMinLon() + "\" lat=\"" +
                        originalBounds.getMinLat() + "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                    "</node>" +
                    "<node id=\"-5\" lon=\"" + originalBounds.getMinLon() + "\" lat=\""
                        + originalBounds.getMinLat() + "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                    "</node>" +
                    "<way id=\"-6\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                        "<nd ref=\"-1\"></nd>" +
                        "<nd ref=\"-2\"></nd>" +
                        "<nd ref=\"-5\"></nd>" +
                    "</way>" +
                    "<way id=\"-7\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                        "<nd ref=\"-3\"></nd>" + "<nd ref=\"-2\"></nd>" +
                    "</way>" +
                    "<way id=\"-8\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                        "<nd ref=\"-1\"></nd>" + "<nd ref=\"-2\"></nd>" +
                    "</way>" +
                    "<relation id=\"-9\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                        "<member type=\"node\" role=\"role1\" ref=\"-1\"></member>" +
                        "<member type=\"way\" role=\"role3\" ref=\"-7\"></member>" +
                        "<member type=\"way\" role=\"role2\" ref=\"-6\"></member>" +
                        "<member type=\"node\" ref=\"-3\"></member>" + "</relation>" +
                    "<relation id=\"-10\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                        "<member type=\"node\" role=\"role1\" ref=\"-5\"></member>" +
                        "<member type=\"relation\" role=\"role1\" ref=\"-9\"></member>" + "</relation>" +
                    "<relation id=\"-11\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                        "<member type=\"way\" role=\"\" ref=\"-7\"></member>" + "" +
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

        // see notes by similar call in testUploadCreate
        OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds, false);
    }

    @Test(expected = BadRequestException.class)
    @Category(UnitTest.class)
    public void testUploadCreateMissingNodeTagValue() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(null);

        // Upload a changeset where one of the tags specified has no value
        // attribute. A failure should occur and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create>" +
                            "<node id=\"-1\" lon=\"" + originalBounds.getMinLon() + "\" lat=\"" + originalBounds.getMinLat() +
                                "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                                "<tag k=\"name 1\"/>" + // missing tag value
                                "<tag k=\"name 2\" v=\"val 2\"/>" +
                            "</node>" +
                            "<node id=\"-2\" lon=\"" + originalBounds.getMinLon() + "\" lat=\"" + originalBounds.getMinLat() +
                                "\" version=\"0\" changeset=\"" + changesetId + "\">" +
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
            assertTrue(r.readEntity(String.class).contains("Error parsing tag"));
            OSMTestUtils.verifyTestChangesetUnmodified(changesetId);
            assertFalse(MapUtils.elementDataExistsInServicesDb());
            throw e;
        }
    }

    @Test(expected = NotFoundException.class)
    @Category(UnitTest.class)
    public void testUploadCreateInvalidChangesetSpecifiedInUrl() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(null);

        // Upload a changeset where the changeset specified in the URL doesn't
        // exist. A failure should occur and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + (changesetId + 1) + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create>" +
                            "<node id=\"-1\" lon=\"" + originalBounds.getMinLon() + "\" lat=\"" +
                                   originalBounds.getMinLat() + "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                                "<tag k=\"name 1\" v=\"val 1\"/>" + "<tag k=\"name 2\" v=\"val 2\"/>" +
                            "</node>" +
                            "<node id=\"-2\" lon=\"" + originalBounds.getMinLon() + "\" lat=\"" + originalBounds.getMinLat() +
                                   "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                                "<tag k=\"name 1\" v=\"val 1\"/>" +
                            "</node>" +
                        "</create>" +
                        "<modify/>" +
                        "<delete if-unused=\"true\"/>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (NotFoundException e) {
            Response r = e.getResponse();
            assertEquals(404, r.getStatus());
            assertTrue(r.readEntity(String.class).contains("Changeset to be updated does not exist"));
            OSMTestUtils.verifyTestChangesetUnmodified(changesetId);
            assertFalse(MapUtils.elementDataExistsInServicesDb());
            throw e;
        }
    }

    @Test(expected = ClientErrorException.class)
    @Category(UnitTest.class)
    public void testUploadCreateInvalidChangesetIdInNode() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(null);

        // Upload a changeset where one of the elements in the changeset lists a
        // changeset ID that doesn't match the changeset ID specified in the URL. A failure should
        // occur and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create>" +
                            "<node id=\"-1\" lon=\"" + originalBounds.getMinLon() + "\" lat=\"" + originalBounds.getMinLat() +
                                  "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                                "<tag k=\"name 1\" v=\"val 1\"/>" + "<tag k=\"name 2\" v=\"val 2\"/>" +
                            "</node>" +
                            "<node id=\"-2\" lon=\"" + originalBounds.getMinLon() + "\" lat=\"" +
                                  originalBounds.getMinLat() + "\" version=\"0\" changeset=\"" + (changesetId + 1) + "\">" +
                                "<tag k=\"name 1\" v=\"val 1\"/>" +
                            "</node>" +
                        "</create>" +
                        "<modify/>" +
                        "<delete if-unused=\"true\"/>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (ClientErrorException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.CONFLICT, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("Invalid changeset ID"));
            OSMTestUtils.verifyTestChangesetUnmodified(changesetId);
            assertFalse(MapUtils.elementDataExistsInServicesDb());
            throw e;
        }
    }

    @Test(expected = BadRequestException.class)
    @Category(UnitTest.class)
    public void testUploadCreateEmptyNodeCoords() throws Exception {
        // Try to update a changeset with nodes that have empty coordinate
        // strings. A failure should occur and no data in system should be modified.
        uploadCreateNodeCoords("", "");
    }

    @Test(expected = BadRequestException.class)
    @Category(UnitTest.class)
    public void testUploadCreateNodeCoordsOutOfBounds() throws Exception {
        // Try to update a changeset with nodes that have out of world bounds
        // coordinate values. A failure should occur and no data in system should be modified.
        uploadCreateNodeCoords("91", "-181");
    }

    private void uploadCreateNodeCoords(String lat, String lon) throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(null);

        // Now update the changeset via the service where one of the nodes has a
        // empty coords. A failure should occur and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML).accept(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<modify/>" +
                        "<create>" +
                            "<node id=\"-1\" lon=\"" + originalBounds.getMinLon() + "\" " + "lat=\"" +
                                    originalBounds.getMinLat() + "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                                "<tag k=\"key 1\" v=\"val 1\"></tag>" + "<tag k=\"key 2\" v=\"val 2\"></tag>" +
                            "</node>" +
                            "<node id=\"-2\" lon=\"" + lon + "\" " + "lat=\"" + lat + "\" version=\"0\" changeset=\"" +
                                   changesetId + "\">" + "<tag k=\"key 3\" v=\"val 3\"></tag>" + "</node>" + "</create>" +
                        "<delete if-unused=\"true\"/>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (BadRequestException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.BAD_REQUEST, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("Error uploading changeset with ID"));

            // make sure the nodes weren't created
            assertEquals(0, createQuery(mapId).from(currentNodes).fetchCount());

            Changesets changeset = createQuery(mapId)
                    .select(changesets)
                    .from(changesets)
                    .where(changesets.id.eq(changesetId))
                    .fetchOne();

            assertNotNull(changeset);

            Timestamp now = super.getCurrentDBTime();

            assertTrue(changeset.getCreatedAt().before(now));
            assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));
            assertEquals(new Integer(12), changeset.getNumChanges());
            assertEquals(new Long(userId), changeset.getUserId());

            // make sure the changeset bounds weren't updated
            Changeset hootChangeset = new Changeset(mapId, changesetId);
            BoundingBox changesetBounds = hootChangeset.getBounds();
            BoundingBox defaultBounds = new BoundingBox();
            assertEquals(defaultBounds, changesetBounds);

            throw e;
        }
    }

    @Test(expected = BadRequestException.class)
    @Category(UnitTest.class)
    public void testUploadCreateExistingNode() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        BoundingBox updateBounds = OSMTestUtils.createAfterModifiedTestChangesetBounds();

        // Update the changeset with a node that has an ID equal to that of a
        // previously created node. A failure should occur and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<modify/>" +
                        "<create>" +
                            "<node id=\"" + nodeIdsArr[0] + "\" lon=\"" + originalBounds.getMinLon() + "\" " +
                                    "lat=\"" + originalBounds.getMinLat() + "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                                "<tag k=\"key 1\" v=\"val 1\"></tag>" +
                                "<tag k=\"key 2\" v=\"val 2\"></tag>" +
                            "</node>" +
                            "<node id=\"-1\" lon=\"" + updateBounds.getMinLon() + "\" " + "lat=\"" +
                                    updateBounds.getMinLat() + "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                                "<tag k=\"key 3\" v=\"val 3\"></tag>" +
                            "</node>" +
                        "</create>" +
                        "<delete if-unused=\"true\"/>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (BadRequestException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.BAD_REQUEST, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("Invalid OSM element ID for create"));

            // make sure the new nodes weren't created
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test(expected = BadRequestException.class)
    @Category(UnitTest.class)
    public void testUploadCreateDuplicateNodeIds() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        OSMTestUtils.createTestNodes(changesetId, originalBounds);

        // Try to create two nodes with the same ID. A failure should occur and
        // no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<modify/>" +
                        "<create>" +
                            "<node id=\"-1\" lon=\"" + originalBounds.getMinLon() + "\" " +
                                 "lat=\"" + originalBounds.getMinLat() + "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                                "<tag k=\"key 1\" v=\"val 1\"></tag>" + "<tag k=\"key 2\" v=\"val 2\"></tag>" +
                            "</node>" +
                            "<node id=\"-1\" lon=\"" + originalBounds.getMinLon() + "\" " +
                                "lat=\"" + originalBounds.getMinLat() + "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                                "<tag k=\"key 3\" v=\"val 3\"></tag>" +
                            "</node>" +
                        "</create>" +
                        "<delete if-unused=\"true\"/>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (BadRequestException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.BAD_REQUEST, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("Duplicate OSM element ID"));

            // make sure the new nodes weren't created
            assertEquals(5, createQuery(mapId).from(currentNodes).fetchCount());
            assertEquals(4, OSMTestUtils.getTagCountForElementType(mapId, ElementType.Node));
            OSMTestUtils.verifyTestChangesetUnmodified(changesetId, originalBounds);
            throw e;
        }
    }

    @Test(expected = BadRequestException.class)
    @Category(UnitTest.class)
    public void testUploadCreateTooFewNodesForWay() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // A way has to have two or more nodes. Try to upload a way with one
        // node. The request should fail and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create>" +
                            "<way id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                "<nd ref=\"" + nodeIdsArr[0] + "\">" + "</nd>" +
                                "<tag k=\"key 1\" v=\"val 1\"/>" +
                            "</way>" +
                        "</create>" +
                        "<modify/>" +
                        "<delete if-unused=\"true\"/>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (BadRequestException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.BAD_REQUEST, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("Too few nodes specified for way"));
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test(expected = BadRequestException.class)
    @Category(UnitTest.class)
    public void testUploadCreateTooManyNodesForWay() throws Exception {
        String originalMaximumWayNodes = MAXIMUM_WAY_NODES;

        BoundingBox originalBounds = null;
        Long changesetId = null;
        Set<Long> nodeIds = null;
        Set<Long> wayIds = null;
        Set<Long> relationIds = null;

        // A way has to have two or more nodes. Try to upload a way more nodes
        // than is allowed by the system. The request should fail and no data in the system should be modified.
        try {
            originalBounds = OSMTestUtils.createStartingTestBounds();
            changesetId = OSMTestUtils.createTestChangeset(originalBounds);
            nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
            Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
            wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
            relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

            // remember the original value of "maximumWayNodes"
            originalMaximumWayNodes = MAXIMUM_WAY_NODES;

            // use a lower number of max way nodes then default for efficiency
            HootCustomPropertiesSetter.setProperty("MAXIMUM_WAY_NODES", "2");

            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create>" +
                            "<way id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                                "<nd ref=\"" + nodeIdsArr[2] + "\"></nd>" +
                            "</way>" +
                        "</create>" +
                        "<modify/>" +
                        "<delete if-unused=\"true\"/>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (BadRequestException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.BAD_REQUEST, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("Too many nodes specified for way"));
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
        finally {
            HootCustomPropertiesSetter.setProperty("MAXIMUM_WAY_NODES", originalMaximumWayNodes);
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testUploadCreateNoMembersInRelation() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);

        // relations with no members are allowed
        Document responseData = target("api/0.6/changeset/" + changesetId + "/upload")
            .queryParam("mapId", String.valueOf(mapId))
            .request(MediaType.TEXT_XML)
            .post(Entity.entity(
                "<osmChange version=\"0.3\" generator=\"iD\">" +
                    "<create>" +
                        "<relation id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                            "<tag k=\"key 1\" v=\"val 1\"/>" +
                        "</relation>" +
                    "</create>" +
                    "<modify/>" +
                    "<delete if-unused=\"true\"/>" +
                "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);

        assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm/diffResult/relation").getLength());
        assertEquals(1, createQuery(mapId).from(currentRelations).fetchCount());
    }

    @Test(expected = NotFoundException.class)
    @Category(UnitTest.class)
    public void testUploadCreateWayWithNonExistentNode() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Try to create a way referencing a node that doesn't exist. The
        // request should fail and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create>" +
                            "<way id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                "<nd ref=\"" + (nodeIdsArr[4] + 2) + "\"></nd>" +
                            "</way>" +
                        "</create>" +
                        "<modify/>" +
                        "<delete if-unused=\"true\"/>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (NotFoundException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.NOT_FOUND, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("Element(s) being referenced don't exist."));
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test(expected = NotFoundException.class)
    @Category(UnitTest.class)
    public void testUploadCreateRelationWithNonExistentNode() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Try to create a relation referencing a node that doesn't exist. The
        // request should fail and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
            .queryParam("mapId", String.valueOf(mapId))
            .request(MediaType.TEXT_XML)
            .post(Entity.entity(
                "<osmChange version=\"0.3\" generator=\"iD\">" +
                    "<create>" +
                        "<relation id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                            "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                            "<member type=\"node\" role=\"role1\" ref=\"" + (nodeIdsArr[4] + 2) + "\"></member>" +
                        "</relation>" +
                    "</create>" +
                    "<modify/>" +
                    "<delete if-unused=\"true\"/>" +
                "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (NotFoundException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.NOT_FOUND, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("Element(s) being referenced don't exist."));
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test(expected = NotFoundException.class)
    @Category(UnitTest.class)
    public void testUploadCreateRelationWithNonExistentWay() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Try to create a relation referencing a way that doesn't exist. The
        // request should fail and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create>" +
                            "<relation id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\">" +
                                "<member type=\"way\" role=\"role1\" ref=\"" + (wayIdsArr[2] + 1) + "\"></member>" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                            "</relation>" +
                        "</create>" +
                        "<modify/>" +
                        "<delete if-unused=\"true\"/>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (NotFoundException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.NOT_FOUND, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("Element(s) being referenced don't exist."));
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test(expected = NotFoundException.class)
    @Category(UnitTest.class)
    public void testUploadCreateRelationWithNonExistentRelation() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // Try to create a relation referencing a relation that doesn't exist.
        // The request should fail and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create>" +
                            "<relation id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                "<member type=\"relation\" role=\"role1\" ref=\"" + (relationIdsArr[3] + 2) + "\"></member>" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                            "</relation>" +
                        "</create>" +
                        "<modify/>" +
                        "<delete if-unused=\"true\"/>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (NotFoundException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.NOT_FOUND, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("Element(s) being referenced don't exist."));
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    // haven't been able to find any good way to handle this yet, so I'm
    // choosing not to for now
    @Test(expected = ClientErrorException.class)
    @Category(UnitTest.class)
    public void testUploadCreateRelationWhereRelationReferencesAnotherRelationThatComesAfterIt() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Try to create a relation that references a relation that is
        // referenced after it in the request.  This isn't supported by the server, so the request should fail and no
        // data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                     "<osmChange version=\"0.3\" generator=\"iD\">" +
                         "<create>" +
                             "<relation id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                 "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[0] + "\"></member>" +
                                 "<member type=\"relation\" role=\"role1\" ref=\"-2\"></member>" +
                             "</relation>" +
                             "<relation id=\"-2\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                 "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[0] + "\"></member>" +
                                 "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                             "</relation>" +
                         "</create>" +
                         "<modify/>" +
                         "<delete if-unused=\"true\"/>" +
                     "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (ClientErrorException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.PRECONDITION_FAILED, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("does not exist for relation"));
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test(expected = BadRequestException.class)
    @Category(UnitTest.class)
    public void testUploadCreateWayWithInvisibleNode() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // make one of way nodes invisible
        CurrentNodes invisibleNode = createQuery(mapId)
                .select(currentNodes)
                .from(currentNodes)
                .where(currentNodes.id.eq(nodeIdsArr[0]))
                .fetchOne();

        assertNotNull(invisibleNode);

        invisibleNode.setVisible(false);

        int success = ((int) createQuery(mapId).update(currentNodes)
                .where(currentNodes.id.eq(invisibleNode.getId()))
                .set(currentNodes.visible, false)
                .execute());

        assertEquals(1, success);
        assertEquals(false, createQuery(mapId)
                .select(currentNodes.visible)
                .from(currentNodes)
                .where(currentNodes.id.eq(nodeIdsArr[0]))
                .fetchOne());

        // Try to upload a way which references an invisible node. The request
        // should fail and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create>" +
                            "<way id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                            "</way>" +
                        "</create>" +
                        "<modify/>" +
                        "<delete if-unused=\"true\"/>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (BadRequestException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.BAD_REQUEST, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("visible for way"));
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test(expected = BadRequestException.class)
    @Category(UnitTest.class)
    public void testUploadCreateRelationWithInvisibleNode() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // make one of relation node members invisible
        CurrentNodes invisibleNode = createQuery(mapId)
                .select(currentNodes)
                .from(currentNodes)
                .where(currentNodes.id.eq(nodeIdsArr[0]))
                .fetchOne();

        assertNotNull(invisibleNode);

        invisibleNode.setVisible(false);

        int success = (int) createQuery(mapId).update(currentNodes)
                .where(currentNodes.id.eq(invisibleNode.getId()))
                .set(currentNodes.visible, false)
                .execute();

        assertEquals(1, success);
        assertEquals(false, createQuery(mapId)
                .select(currentNodes.visible)
                .from(currentNodes)
                .where(currentNodes.id.eq(nodeIdsArr[0]))
                .fetchOne());

        // Try to upload a relation which references an invisible node. The
        // request should fail and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create>" +
                            "<relation id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                            "</relation>" +
                        "</create>" +
                        "<modify/>" +
                        "<delete if-unused=\"true\"/>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (BadRequestException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.BAD_REQUEST, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("visible for relation"));
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test(expected = BadRequestException.class)
    @Category(UnitTest.class)
    public void testUploadCreateRelationWithInvisibleWay() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // make one of relation way members invisible
        CurrentWays invisibleWay = createQuery(mapId)
                .select(currentWays)
                .from(currentWays)
                .where(currentWays.id.eq(wayIdsArr[0]))
                .fetchOne();

        assertNotNull(invisibleWay);

        invisibleWay.setVisible(false);

        int success = (int) createQuery(mapId).update(currentWays)
                .where(currentWays.id.eq(invisibleWay.getId()))
                .set(currentWays.visible, false)
                .execute();

        assertEquals(1, success);
        assertEquals(false, createQuery(mapId)
                .select(currentWays.visible)
                .from(currentWays)
                .where(currentWays.id.eq(wayIdsArr[0]))
                .fetchOne());

        // Try to upload a relation which references an invisible way. The
        // request should fail and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create>" +
                            "<relation id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[0] + "\"></member>" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                            "</relation>" +
                        "</create>" +
                        "<modify/>" +
                        "<delete if-unused=\"true\"/>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (BadRequestException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.BAD_REQUEST, Response.Status.fromStatusCode(r.getStatus()));
//            Assert.assertTrue(r.getEntity(String.class).contains("visible for relation"));
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test(expected = BadRequestException.class)
    @Category(UnitTest.class)
    public void testUploadCreateRelationWithInvisibleRelation() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // make one of relation's relation members invisible
        CurrentRelations invisibleRelation = createQuery(mapId)
                .select(currentRelations)
                .from(currentRelations)
                .where(currentRelations.id.eq(relationIdsArr[0]))
                .fetchOne();

        assertNotNull(invisibleRelation);

        invisibleRelation.setVisible(false);

        long success = createQuery(mapId).update(currentRelations)
                .where(currentRelations.id.eq(invisibleRelation.getId()))
                .set(currentRelations.visible, false)
                .execute();

        assertEquals(1, success);
        assertEquals(false, createQuery(mapId)
                .select(currentRelations.visible)
                .from(currentRelations)
                .where(currentRelations.id.eq(relationIdsArr[0]))
                .fetchOne());

        // Try to upload a relation which references an invisible relation. The request should fail
        // and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create>" +
                            "<relation id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[0] + "\"></member>" +
                                "<member type=\"relation\" role=\"role1\" ref=\"" + relationIdsArr[0] + "\"></member>" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                            "</relation>" +
                        "</create>" +
                        "<modify/>" +
                        "<delete if-unused=\"true\"/>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (BadRequestException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.BAD_REQUEST, Response.Status.fromStatusCode(r.getStatus()));
 //           Assert.assertTrue(r.getEntity(String.class).contains("visible for relation"));
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test(expected = BadRequestException.class)
    @Category(UnitTest.class)
    public void testUploadCreateDuplicateWayIds() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Try to upload two ways with the same ID. The request should fail and no
        // data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create>" +
                            "<way id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                            "</way>" +
                            "<way id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                "<nd ref=\"" + nodeIdsArr[2] + "\"></nd>" +
                            "</way>" +
                        "</create>" +
                        "<modify/>" +
                        "<delete if-unused=\"true\"/>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (BadRequestException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.BAD_REQUEST, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("Duplicate OSM element ID"));
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test(expected = BadRequestException.class)
    @Category(UnitTest.class)
    public void testUploadCreateDuplicateRelationIds() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Try to upload two relations with the same ID. The request should fail and no
        // data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create>" +
                            "<relation id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[0] + "\"></member>" +
                            "</relation>" +
                            "<relation id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[2] + "\"></member>" +
                            "</relation>" +
                        "</create>" +
                        "<modify/>" +
                        "<delete if-unused=\"true\"/>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (BadRequestException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.BAD_REQUEST, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("Duplicate OSM element ID"));
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test(expected = BadRequestException.class)
    @Category(UnitTest.class)
    public void testUploadCreateExistingWay() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Update the changeset with a way that has an ID equal to that of a
        // previously created way. A failure should occur and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<modify/>" +
                        "<create>" +
                            "<way id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                            "</way>" +
                            "<way id=\"" + wayIdsArr[0] + "\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" + "<nd ref=\"" + nodeIdsArr[2] + "\"></nd>" +
                            "</way>" +
                        "</create>" +
                        "<delete if-unused=\"true\"/>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (BadRequestException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.BAD_REQUEST, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("Invalid OSM element ID for create"));
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test(expected = BadRequestException.class)
    @Category(UnitTest.class)
    public void testUploadCreateExistingRelation() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // Update the changeset with a relation that has an ID equal to that of
        // a previously created relation. A failure should occur and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<modify/>" +
                        "<create>" +
                            "<relation id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[1] + "\"></member>" +
                            "</relation>" +
                            "<relation id=\"" + relationIdsArr[0] + "\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[2] + "\"></member>" +
                                "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[1] + "\"></member>" +
                            "</relation>" +
                        "</create>" +
                        "<delete if-unused=\"true\"/>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (BadRequestException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.BAD_REQUEST, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("Invalid OSM element ID for create"));
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test(expected = ClientErrorException.class)
    @Category(UnitTest.class)
    public void testUploadCreateRelationReferencingItself() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Update the changeset with a relation that references itself as a
        // member. A failure should occur and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<modify/>" +
                        "<create>" +
                            "<relation id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                "<member type=\"relation\" role=\"role1\" ref=\"-1\"></member>" +
                                "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[1] + "\"></member>" +
                            "</relation>" +
                            "<relation id=\"-2\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[2] + "\"></member>" +
                                "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[1] + "\"></member>" +
                            "</relation>" +
                        "</create>" +
                        "<delete if-unused=\"true\"/>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (ClientErrorException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.CONFLICT, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("references itself"));
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test(expected = ClientErrorException.class)
    @Category(UnitTest.class)
    public void testUploadCreateInvalidChangesetIdInWay() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Upload a changeset where one of the elements in the changeset lists a
        // changeset ID that doesn't match the changeset ID specified in the URL. A failure should
        // occur and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create>" +
                            "<way id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                            "</way>" +
                            "<way id=\"-2\" version=\"0\" changeset=\"" + (changesetId + 1) + "\" >" +
                                "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                "<nd ref=\"" + nodeIdsArr[2] + "\"></nd>" +
                            "</way>" +
                        "</create>" +
                        "<modify/>" +
                        "<delete if-unused=\"true\"/>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (ClientErrorException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.CONFLICT, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("Invalid changeset ID"));
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test(expected = ClientErrorException.class)
    @Category(UnitTest.class)
    public void testUploadCreateInvalidChangesetIdInRelation() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Upload a changeset where one of the elements in the changeset lists a
        // changeset ID that doesn't match the changeset ID specified in the URL. A failure should
        // occur and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create>" +
                            "<relation id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                            "</relation>" +
                            "<relation id=\"-2\" version=\"0\" changeset=\"" + (changesetId + 1) + "\" >" +
                                "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[0] + "\"></member>" +
                                "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[1] + "\"></member>" +
                            "</relation>" +
                        "</create>" +
                        "<modify/>" +
                        "<delete if-unused=\"true\"/>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (ClientErrorException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.CONFLICT, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("Invalid changeset ID"));
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test(expected = BadRequestException.class)
    @Category(UnitTest.class)
    public void testUploadCreateMissingWayTagValue() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Upload a changeset where one of the tags specified has no value
        // attribute. A failure should occur and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create>" +
                            "<way id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                            "</way>" +
                            "<way id=\"-2\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                "<nd ref=\"" + nodeIdsArr[2] + "\"></nd>" +
                                "<tag k=\"name 1\"/>" + // missing tag value
                            "</way>" +
                        "</create>" +
                        "<modify/>" +
                        "<delete if-unused=\"true\"/>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (BadRequestException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.BAD_REQUEST, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("Error parsing tag"));
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test(expected = BadRequestException.class)
    @Category(UnitTest.class)
    public void testUploadCreateMissingRelationTagValue() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Upload a changeset where one of the tags of one of the elements
        // specified has no value attribute. A failure should occur and no data in the system should be
        // modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create>" +
                            "<relation id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[0] + "\"></member>" +
                            "</relation>" +
                            "<relation id=\"-2\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[0] + "\"></member>" +
                                "<tag k=\"name 1\"/>" + // missing tag value
                            "</relation>" +
                        "</create>" +
                        "<modify/>" +
                        "<delete if-unused=\"true\"/>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (BadRequestException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.BAD_REQUEST, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("Error parsing tag"));
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test(expected = BadRequestException.class)
    @Category(UnitTest.class)
    public void testUploadCreateDuplicateWayNodeIds() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Try to upload a way which lists the same node twice. The request
        // should fail and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create>" +
                            "<way id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                            "</way>" +
                            "<way id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                            "</way>" +
                        "</create>" +
                        "<modify/>" +
                        "<delete if-unused=\"true\"/>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (BadRequestException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.BAD_REQUEST, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("Duplicate OSM element ID"));
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test(expected = ClientErrorException.class)
    @Category(UnitTest.class)
    public void testUploadCreateNodeInvalidVersion() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        BoundingBox updateBounds = OSMTestUtils.createAfterModifiedTestChangesetBounds();

        // Update the changeset where one of the nodes has a version greater
        // than zero, which is what the version should be for all new elements. A failure should occur
        // and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create>" +
                            "<node id=\"-1\" lon=\"" + originalBounds.getMinLon() + "\" " +
                                  "lat=\"" + originalBounds.getMinLat() + "\" version=\"1\" changeset=\"" + changesetId + "\">" +
                                "<tag k=\"key 1\" v=\"val 1\"></tag>" + "<tag k=\"key 2\" v=\"val 2\"></tag>" +
                            "</node>" +
                            "<node id=\"-2\" lon=\"" + updateBounds.getMinLon() + "\" " + "lat=\""
                                + updateBounds.getMinLat() + "\" version=\"0\" changeset=\"" + changesetId + "\">"
                                + "<tag k=\"key 3\" v=\"val 3\"></tag>" +
                            "</node>" +
                        "</create>" +
                        "<modify/>" +
                        "<delete if-unused=\"true\"/>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (ClientErrorException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.CONFLICT, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("Invalid version"));
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test(expected = ClientErrorException.class)
    @Category(UnitTest.class)
    public void testUploadCreateWayInvalidVersion() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Update the changeset where one of the ways has a version greater than
        // zero, which is what the version should be for all new elements. A failure should occur
        // and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create>" +
                            "<way id=\"-1\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                            "</way>" +
                            "<way id=\"-2\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                            "</way>" +
                        "</create>" +
                        "<modify/>" +
                        "<delete if-unused=\"true\"/>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (ClientErrorException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.CONFLICT, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("Invalid version"));
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test(expected = ClientErrorException.class)
    @Category(UnitTest.class)
    public void testUploadCreateRelationInvalidVersion() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Update the changeset where one of the relations has a version greater
        // than zero, which is what the version should be for all new elements. A failure should occur
        // and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create>" +
                            "<relation id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                            "</relation>" +
                            "<relation id=\"-2\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                            "</relation>" +
                        "</create>" +
                        "<modify/>" +
                        "<delete if-unused=\"true\"/>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (ClientErrorException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.CONFLICT, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("Invalid version"));
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test(expected = BadRequestException.class)
    @Category(UnitTest.class)
    public void testUploadCreateWayEmptyNodeId() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Update the changeset where one of the ways has a node with an empty
        // string for its ID. A failure should occur and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create>" +
                            "<way id=\"-1\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                            "</way>" +
                            "<way id=\"-2\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                "<nd ref=\"\"></nd>" + "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                            "</way>" +
                        "</create>" +
                        "<modify/>" +
                        "<delete if-unused=\"true\"/>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (BadRequestException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.BAD_REQUEST, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("Element in changeset has empty ID."));
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test(expected = BadRequestException.class)
    @Category(UnitTest.class)
    public void testUploadCreateRelationEmptyMemberId() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Update the changeset where one of the relations has a member with an
        // empty string for an ID.  A failure should occur and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create>" +
                            "<relation id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                            "</relation>" +
                            "<relation id=\"-2\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                // empty member ID
                                "<member type=\"node\" role=\"role1\" ref=\"\"></member>" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                            "</relation>" +
                        "</create>" +
                        "<modify/>" +
                        "<delete if-unused=\"true\"/>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (BadRequestException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.BAD_REQUEST, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("Element in changeset has empty ID."));
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test(expected = ClientErrorException.class)
    @Category(UnitTest.class)
    public void testUploadCreateWayMissingNodeId() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Update the changeset where one of the ways has a node with a missing
        // ID attribute. A failure should occur and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create>" +
                            "<way id=\"-1\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                            "</way>" +
                            "<way id=\"-2\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                "<nd></nd>" + // missing node ID
                                "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                            "</way>" +
                        "</create>" +
                        "<modify/>" +
                        "<delete if-unused=\"true\"/>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (ClientErrorException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.CONFLICT, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("Invalid version"));
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test(expected = BadRequestException.class)
    @Category(UnitTest.class)
    public void testUploadCreateRelationMissingMemberId() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Update the changeset with a relation that has a member with a missing
        // ID attribute. A failure should occur and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create>" +
                            "<relation id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                "<member type=\"node\" role=\"role1\"\"></member>" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                            "</relation>" +
                            "<relation id=\"-2\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                // mising member ID
                                "<member type=\"node\" role=\"role1\"></member>" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                            "</relation>" +
                        "</create>" +
                        "<modify/>" +
                        "<delete if-unused=\"true\"/>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (BadRequestException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.BAD_REQUEST, Response.Status.fromStatusCode(r.getStatus()));
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test(expected = BadRequestException.class)
    @Category(UnitTest.class)
    public void testUploadCreateRelationInvalidMemberType() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Update the changeset where one of the relations has a member with an
        // empty string for an ID. A failure should occur and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create>" +
                            "<relation id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                // invalid member type
                                "<member type=\"blah\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                            "</relation>" +
                            "<relation id=\"-2\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                            "</relation>" +
                        "</create>" +
                        "<modify/>" +
                        "<delete if-unused=\"true\"/>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (BadRequestException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.BAD_REQUEST, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("Invalid relation member type"));
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test(expected = BadRequestException.class)
    @Category(UnitTest.class)
    public void testUploadCreateRelationMissingMemberType() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Update the changeset where one of the relations has a member with an
        // empty string for an ID.  A failure should occur and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create>" +
                            "<relation id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                // missing member type
                                "<member role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                            "</relation>" +
                            "<relation id=\"-2\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                            "</relation>" +
                        "</create>" +
                        "<modify/>" +
                        "<delete if-unused=\"true\"/>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (BadRequestException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.BAD_REQUEST, Response.Status.fromStatusCode(r.getStatus()));
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }
}
