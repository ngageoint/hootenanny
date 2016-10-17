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
import static hoot.services.controllers.osm.OSMTestUtils.insertNew;
import static hoot.services.models.db.QChangesets.changesets;
import static hoot.services.models.db.QCurrentNodes.currentNodes;
import static hoot.services.models.db.QCurrentRelationMembers.currentRelationMembers;
import static hoot.services.models.db.QCurrentRelations.currentRelations;
import static hoot.services.models.db.QCurrentWayNodes.currentWayNodes;
import static hoot.services.models.db.QCurrentWays.currentWays;
import static hoot.services.utils.DbUtils.createQuery;
import static org.junit.Assert.*;

import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.HashMap;
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
import hoot.services.models.db.CurrentRelationMembers;
import hoot.services.models.db.CurrentRelations;
import hoot.services.models.db.CurrentWayNodes;
import hoot.services.models.db.CurrentWays;
import hoot.services.models.osm.Changeset;
import hoot.services.models.osm.Element.ElementType;
import hoot.services.models.osm.RelationMember;
import hoot.services.utils.DbUtils;
import hoot.services.utils.PostgresUtils;
import hoot.services.utils.QuadTileCalculator;
import hoot.services.utils.XmlUtils;


public class ChangesetResourceUploadDeleteTest extends OSMResourceTestAbstract {

    private void testUploadDelete(String request, BoundingBox originalBounds, long changesetId, Long[] nodeIdsArr,
            Long[] wayIdsArr, Long[] relationIdsArr) throws Exception {
        Document responseData = target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(request, MediaType.TEXT_XML_TYPE), Document.class);

        assertNotNull(responseData);

        XPath xpath = XmlUtils.createXPath();
        NodeList returnedNodes = XPathAPI.selectNodeList(responseData, "//osm/diffResult/node");
        assertEquals(1, returnedNodes.getLength());

        // check that the returned IDs are correct and the correct
        // attributes were returned
        assertEquals((long) nodeIdsArr[3],
                Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@old_id", responseData)));
        org.w3c.dom.Node xml = XPathAPI.selectSingleNode(responseData, "//osm/diffResult/node[1]");
        assertNull(xml.getAttributes().getNamedItem("new_id"));
        assertNull(xml.getAttributes().getNamedItem("new_version"));

        NodeList returnedWays = XPathAPI.selectNodeList(responseData, "//osm/diffResult/way");
        assertEquals(1, returnedWays.getLength());

        assertEquals((long) wayIdsArr[2],
                Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@old_id", responseData)));
        xml = XPathAPI.selectSingleNode(responseData, "//osm/diffResult/way[1]");
        assertNull(xml.getAttributes().getNamedItem("new_id"));
        assertNull(xml.getAttributes().getNamedItem("new_version"));

        NodeList returnedRelations = XPathAPI.selectNodeList(responseData, "//osm/diffResult/relation");
        assertEquals(1, returnedRelations.getLength());

        assertEquals((long) relationIdsArr[2],
                Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@old_id", responseData)));
        xml = XPathAPI.selectSingleNode(responseData, "//osm/diffResult/relation[1]");
        assertNull(xml.getAttributes().getNamedItem("new_id"));
        assertNull(xml.getAttributes().getNamedItem("new_version"));

        Timestamp now = super.getCurrentDBTime();

        /*
        Map<Long, CurrentNodes> nodes = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                    .from(currentNodesTbl)
                    .map(currentNodesTbl.id, currentNodesTbl);
         */
        Map<Long, CurrentNodes> nodes = createQuery(mapId)
                .from(currentNodes)
                .transform(groupBy(currentNodes.id).as(currentNodes));

        assertEquals(4, nodes.size());

        CurrentNodes nodeRecord = nodes.get(nodeIdsArr[0]);
        assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
        assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
        assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
        assertEquals(nodeIdsArr[0], nodeRecord.getId());
        assertEquals(
                new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                nodeRecord.getTile());
        assertTrue(nodeRecord.getTimestamp().before(now));
        assertEquals(new Long(1), nodeRecord.getVersion());
        assertTrue(nodeRecord.getVisible());
        Map<String, String> tags = PostgresUtils.postgresObjToHStore(nodeRecord.getTags());
        assertNotNull(tags);
        assertEquals(2, tags.size());
        assertEquals("val 1", tags.get("key 1"));
        assertEquals("val 2", tags.get("key 2"));

        nodeRecord = nodes.get(nodeIdsArr[1]);
        assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
        assertEquals(new Double(originalBounds.getMaxLat()), nodeRecord.getLatitude());
        assertEquals(new Double(originalBounds.getMaxLon()), nodeRecord.getLongitude());
        assertEquals(nodeIdsArr[1], nodeRecord.getId());
        assertEquals(
                new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                nodeRecord.getTile());
        assertTrue(nodeRecord.getTimestamp().before(now));
        assertEquals(new Long(1), nodeRecord.getVersion());
        assertTrue(nodeRecord.getVisible());
        assertTrue((nodeRecord.getTags() == null)
                || PostgresUtils.postgresObjToHStore(nodeRecord.getTags()).isEmpty());

        nodeRecord = nodes.get(nodeIdsArr[2]);
        assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
        assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
        assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
        assertEquals(nodeIdsArr[2], nodeRecord.getId());
        assertEquals(
                new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                nodeRecord.getTile());
        assertTrue(nodeRecord.getTimestamp().before(now));
        assertEquals(new Long(1), nodeRecord.getVersion());
        assertTrue(nodeRecord.getVisible());
        assertTrue((nodeRecord.getTags() == null)
                || PostgresUtils.postgresObjToHStore(nodeRecord.getTags()).isEmpty());

        nodeRecord = nodes.get(nodeIdsArr[4]);
        assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
        assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
        assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
        assertEquals(nodeIdsArr[4], nodeRecord.getId());
        assertEquals(
                new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                nodeRecord.getTile());
        assertTrue(nodeRecord.getTimestamp().before(now));
        assertEquals(new Long(1), nodeRecord.getVersion());
        assertTrue(nodeRecord.getVisible());
        tags = PostgresUtils.postgresObjToHStore(nodeRecord.getTags());
        assertNotNull(tags);
        assertEquals(1, tags.size());
        assertEquals("val 4", tags.get("key 4"));

        // verify the deleted node
        nodeRecord = createQuery(mapId)
                .select(currentNodes)
                .from(currentNodes)
                .where(currentNodes.id.eq(nodeIdsArr[3]))
                .fetchOne();

        assertNull(nodeRecord);

        Map<Long, CurrentWays> ways = createQuery(mapId)
                .from(currentWays)
                .transform(groupBy(currentWays.id).as(currentWays));

        assertEquals(2, ways.size());

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

        assertEquals(3, wayNodes.size());
        CurrentWayNodes wayNode = wayNodes.get(0);
        assertEquals(nodeIdsArr[0], wayNode.getNodeId());
        assertEquals(new Long(1), wayNode.getSequenceId());
        assertEquals(wayRecord.getId(), wayNode.getWayId());
        wayNode = wayNodes.get(1);
        assertEquals(nodeIdsArr[1], wayNode.getNodeId());
        assertEquals(new Long(2), wayNode.getSequenceId());
        assertEquals(wayRecord.getId(), wayNode.getWayId());
        wayNode = wayNodes.get(2);
        assertEquals(nodeIdsArr[4], wayNode.getNodeId());
        assertEquals(new Long(3), wayNode.getSequenceId());
        assertEquals(wayRecord.getId(), wayNode.getWayId());

        // verify the previously existing tags
        tags = PostgresUtils.postgresObjToHStore(wayRecord.getTags());
        assertNotNull(tags);
        assertEquals(2, tags.size());
        assertEquals("val 1", tags.get("key 1"));
        assertEquals("val 2", tags.get("key 2"));

        wayRecord = ways.get(wayIdsArr[1]);
        assertEquals(new Long(changesetId), wayRecord.getChangesetId());
        assertEquals(wayIdsArr[1], wayRecord.getId());
        assertTrue(wayRecord.getTimestamp().before(now));
        assertEquals(new Long(1), wayRecord.getVersion());
        assertTrue(wayRecord.getVisible());

        wayNodes = createQuery(mapId)
                .from(currentWayNodes)
                .select(currentWayNodes)
                .where(currentWayNodes.wayId.eq(wayIdsArr[1]))
                .orderBy(currentWayNodes.sequenceId.asc())
                .fetch();

        assertEquals(2, wayNodes.size());
        wayNode = wayNodes.get(0);
        assertEquals(nodeIdsArr[2], wayNode.getNodeId());
        assertEquals(new Long(1), wayNode.getSequenceId());
        assertEquals(wayRecord.getId(), wayNode.getWayId());
        wayNode = wayNodes.get(1);
        assertEquals(nodeIdsArr[1], wayNode.getNodeId());
        assertEquals(new Long(2), wayNode.getSequenceId());
        assertEquals(wayRecord.getId(), wayNode.getWayId());

        // verify the way with no tags
        assertTrue((wayRecord.getTags() == null)
                || PostgresUtils.postgresObjToHStore(wayRecord.getTags()).isEmpty());

        // verify the deleted way
        assertEquals(0L, createQuery(mapId)
                .from(currentWays)
                .where(currentWays.id.eq(wayIdsArr[2]))
                .fetchCount());

        Map<Long, CurrentRelations> relations =
                createQuery(mapId)
                        .from(currentRelations)
                        .transform(groupBy(currentRelations.id).as(currentRelations));

        assertEquals(3, relations.size());

        // verify the previously existing relations
        CurrentRelations relationRecord = relations.get(relationIdsArr[0]);
        assertEquals(new Long(changesetId), relationRecord.getChangesetId());
        assertEquals(relationIdsArr[0], relationRecord.getId());
        assertTrue(relationRecord.getTimestamp().before(now));
        assertEquals(new Long(1), relationRecord.getVersion());
        assertTrue(relationRecord.getVisible());

        List<CurrentRelationMembers> members =
                createQuery(mapId)
                        .select(currentRelationMembers)
                        .from(currentRelationMembers)
                        .where(currentRelationMembers.relationId.eq(relationIdsArr[0]))
                        .orderBy(currentRelationMembers.sequenceId.asc())
                        .fetch();

        assertEquals(4, members.size());
        CurrentRelationMembers member = members.get(0);
        assertEquals(relationRecord.getId(), member.getRelationId());
        assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
        assertEquals("role1", member.getMemberRole());
        assertEquals(new Integer(1), member.getSequenceId());

        assertEquals(nodeIdsArr[0], member.getMemberId());
        member = members.get(1);
        assertEquals(relationRecord.getId(), member.getRelationId());
        assertEquals(DbUtils.nwr_enum.way, member.getMemberType());
        assertEquals("role3", member.getMemberRole());
        assertEquals(new Integer(2), member.getSequenceId());

        assertEquals(wayIdsArr[1], member.getMemberId());
        member = members.get(2);
        assertEquals(relationRecord.getId(), member.getRelationId());
        assertEquals(DbUtils.nwr_enum.way, member.getMemberType());
        assertEquals("role2", member.getMemberRole());
        assertEquals(new Integer(3), member.getSequenceId());

        assertEquals(wayIdsArr[0], member.getMemberId());
        member = members.get(3);
        assertEquals(relationRecord.getId(), member.getRelationId());
        assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
        assertEquals("", member.getMemberRole());
        assertEquals(new Integer(4), member.getSequenceId());

        assertEquals(nodeIdsArr[2], member.getMemberId());

        // verify the previously existing tags
        tags = PostgresUtils.postgresObjToHStore(relationRecord.getTags());
        assertNotNull(tags);
        assertEquals(1, tags.size());
        assertEquals("val 1", tags.get("key 1"));

        relationRecord = relations.get(relationIdsArr[1]);
        assertEquals(new Long(changesetId), relationRecord.getChangesetId());
        assertEquals(relationIdsArr[1], relationRecord.getId());

        assertTrue(relationRecord.getTimestamp().before(now));
        assertEquals(new Long(1), relationRecord.getVersion());
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
        assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
        assertEquals("role1", member.getMemberRole());
        assertEquals(new Integer(1), member.getSequenceId());

        assertEquals(nodeIdsArr[4], member.getMemberId());
        member = members.get(1);
        assertEquals(relationRecord.getId(), member.getRelationId());
        assertEquals(DbUtils.nwr_enum.relation, member.getMemberType());
        assertEquals("role1", member.getMemberRole());
        assertEquals(new Integer(2), member.getSequenceId());

        assertEquals(relationIdsArr[0], member.getMemberId());
        tags = PostgresUtils.postgresObjToHStore(relationRecord.getTags());
        assertNotNull(tags);
        assertEquals(2, tags.size());
        assertEquals("val 2", tags.get("key 2"));
        assertEquals("val 3", tags.get("key 3"));

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

        // verify the deleted relation
        assertNull(
                createQuery(mapId)
                        .select(currentRelations)
                        .from(currentRelations)
                        .where(currentRelations.id.eq(relationIdsArr[2]))
                        .fetchOne());

        // verify tags that should no longer exist
        assertTrue((relationRecord.getTags() == null)
                || PostgresUtils.postgresObjToHStore(relationRecord.getTags()).isEmpty());

        Changesets changeset = createQuery(mapId)
                .select(changesets)
                .from(changesets)
                .where(changesets.id.eq(changesetId))
                .fetchOne();

        assertNotNull(changeset);
        assertTrue(changeset.getCreatedAt().before(now));
        assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));
        assertEquals(new Integer(15), changeset.getNumChanges());
        assertEquals(new Long(userId), changeset.getUserId());

        BoundingBox expandedBounds = new BoundingBox(originalBounds);
        expandedBounds.expand(originalBounds, Double.parseDouble(CHANGESET_BOUNDS_EXPANSION_FACTOR_DEEGREES));
        Changeset hootChangeset = new Changeset(mapId, changesetId);
        BoundingBox changesetBounds = hootChangeset.getBounds();
        assertEquals(changesetBounds, expandedBounds);
    }

    @Test
    @Category(UnitTest.class)
    public void testUploadDelete() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // Now, delete one of the nodes, one of the ways, and one of the relations.
        testUploadDelete(
            "<osmChange version=\"0.3\" generator=\"iD\">" +
                "<create/>" +
                "<modify/>" +
                "<delete if-unused=\"true\">" +
                    "<node id=\"" + nodeIdsArr[3] + "\" lon=\"" + originalBounds.getMinLon() +
                        "\" lat=\"" + originalBounds.getMinLat() + "\" version=\"1\" " + "changeset=\"" + changesetId + "\">" +
                        "<tag k=\"key 1\" v=\"val 1\"></tag>" +
                    "</node>" +
                    "<way id=\"" + wayIdsArr[2] + "\" version=\"1\" " + "changeset=\"" + changesetId + "\">" +
                        "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                        "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                        "<tag k=\"key 1\" v=\"val 1\"></tag>" +
                    "</way>" +
                    "<relation id=\"" + relationIdsArr[2] + "\" version=\"1\" " + "changeset=\"" + changesetId + "\">" +
                        "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[2] + "\"></member>" +
                        "<tag k=\"key 4\" v=\"val 4\"></tag>" +
                    "</relation>" +
                "</delete>" +
            "</osmChange>",
            originalBounds, changesetId, nodeIdsArr, wayIdsArr, relationIdsArr);
    }

    // This test comes from direct experience calling the services from the iD editor.
    @Test
    @Category(UnitTest.class)
    public void testUploadDelete2() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds, 8);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);

        // Now delete two of the nodes, where ways are specified in the
        // modify portion of the request before the nodes.
        Document responseData = target("api/0.6/changeset/" + changesetId + "/upload")
            .queryParam("mapId", String.valueOf(mapId))
            .request(MediaType.TEXT_XML)
            .post(Entity.entity(
                "<osmChange version=\"0.3\" generator=\"iD\">" +
                    "<create/>" +
                    "<modify/>" +
                    "<delete if-unused=\"true\">" +
                        "<way id=\"" + wayIdsArr[0] + "\" version=\"1\" " + "changeset=\"" + changesetId + "\">" +
                            "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                            "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                            "<nd ref=\"" + nodeIdsArr[4] + "\"></nd>" +
                        "</way>" +
                        "<node id=\"" + nodeIdsArr[4] + "\" lon=\"" + originalBounds.getMinLon() + "\" lat=\"" +
                            originalBounds.getMinLat() + "\" version=\"1\" " + "changeset=\"" + changesetId + "\">" +
                        "</node>" +
                    "</delete>" +
                "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);

        assertNotNull(responseData);

        XPath xpath = XmlUtils.createXPath();
        NodeList returnedNodes = XPathAPI.selectNodeList(responseData, "//osm/diffResult/node");
        assertEquals(1, returnedNodes.getLength());

        // check that the returned IDs are correct and the correct attributes were returned
        assertEquals((long) nodeIdsArr[4],
                Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@old_id", responseData)));
        org.w3c.dom.Node xml = XPathAPI.selectSingleNode(responseData, "//osm/diffResult/node[1]");
        assertNull(xml.getAttributes().getNamedItem("new_id"));
        assertNull(xml.getAttributes().getNamedItem("new_version"));

        Timestamp now = super.getCurrentDBTime();

        Map<Long, CurrentNodes> nodes =
                createQuery(mapId)
                        .from(currentNodes)
                        .transform(groupBy(currentNodes.id).as(currentNodes));

        assertEquals(4, nodes.size());

        CurrentNodes nodeRecord = nodes.get(nodeIdsArr[0]);

        assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
        assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
        assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
        assertEquals(nodeIdsArr[0], nodeRecord.getId());
        assertEquals(
                new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                nodeRecord.getTile());
        assertTrue(nodeRecord.getTimestamp().before(now));
        assertEquals(new Long(1), nodeRecord.getVersion());
        assertTrue(nodeRecord.getVisible());

        Map<String, String> tags = PostgresUtils.postgresObjToHStore(nodeRecord.getTags());

        assertNotNull(tags);
        assertEquals(2, tags.size());
        assertEquals("val 1", tags.get("key 1"));
        assertEquals("val 2", tags.get("key 2"));

        nodeRecord = nodes.get(nodeIdsArr[1]);

        assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
        assertEquals(new Double((originalBounds.getMaxLat())), nodeRecord.getLatitude());
        assertEquals(new Double((originalBounds.getMaxLon())), nodeRecord.getLongitude());
        assertEquals(nodeIdsArr[1], nodeRecord.getId());
        assertEquals(
                new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                nodeRecord.getTile());
        assertTrue(nodeRecord.getTimestamp().before(now));
        assertEquals(new Long(1), nodeRecord.getVersion());
        assertTrue(nodeRecord.getVisible());
        assertTrue((nodeRecord.getTags() == null)
                || PostgresUtils.postgresObjToHStore(nodeRecord.getTags()).isEmpty());

        nodeRecord = nodes.get(nodeIdsArr[2]);

        assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
        assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
        assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
        assertEquals(nodeIdsArr[2], nodeRecord.getId());
        assertEquals(
                new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                nodeRecord.getTile());
        assertTrue(nodeRecord.getTimestamp().before(now));
        assertEquals(new Long(1), nodeRecord.getVersion());
        assertEquals(true, nodeRecord.getVisible());
        assertTrue((nodeRecord.getTags() == null)
                || PostgresUtils.postgresObjToHStore(nodeRecord.getTags()).isEmpty());

        nodeRecord = nodes.get(nodeIdsArr[2]);

        assertTrue((nodeRecord.getTags() == null)
                || PostgresUtils.postgresObjToHStore(nodeRecord.getTags()).isEmpty());

        nodeRecord = nodes.get(nodeIdsArr[3]);

        assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
        assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
        assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
        assertEquals(nodeIdsArr[3], nodeRecord.getId());
        assertEquals(
                new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                nodeRecord.getTile());
        assertTrue(nodeRecord.getTimestamp().before(now));
        assertEquals(new Long(1), nodeRecord.getVersion());
        assertTrue(nodeRecord.getVisible());

        tags = PostgresUtils.postgresObjToHStore(nodeRecord.getTags());

        assertNotNull(tags);
        assertEquals(1, tags.size());
        assertEquals("val 3", tags.get("key 3"));

        // verify the deleted node
        nodeRecord = createQuery(mapId)
                .select(currentNodes)
                .from(currentNodes)
                .where(currentNodes.id.eq(nodeIdsArr[4]))
                .fetchOne();

        assertNull(nodeRecord);

        Map<Long, CurrentWays> ways = createQuery(mapId)
                .from(currentWays)
                .transform(groupBy(currentWays.id).as(currentWays));

        assertEquals(2, ways.size());

        CurrentWays wayRecord = ways.get(wayIdsArr[1]);

        assertEquals(new Long(changesetId), wayRecord.getChangesetId());
        assertEquals(wayIdsArr[1], wayRecord.getId());
        assertTrue(wayRecord.getTimestamp().before(now));
        assertEquals(new Long(1), wayRecord.getVersion());
        assertTrue(wayRecord.getVisible());

        List<CurrentWayNodes> wayNodes = createQuery(mapId)
                .select(currentWayNodes)
                .from(currentWayNodes)
                .where(currentWayNodes.wayId.eq(wayIdsArr[1]))
                .orderBy(currentWayNodes.sequenceId.asc())
                .fetch();

        assertEquals(2, wayNodes.size());

        CurrentWayNodes wayNode = wayNodes.get(0);

        assertEquals(nodeIdsArr[2], wayNode.getNodeId());
        assertEquals(new Long(1), wayNode.getSequenceId());
        assertEquals(wayRecord.getId(), wayNode.getWayId());

        wayNode = wayNodes.get(1);

        assertEquals(nodeIdsArr[1], wayNode.getNodeId());
        assertEquals(new Long(2), wayNode.getSequenceId());
        assertEquals(wayRecord.getId(), wayNode.getWayId());

        // verify the way with no tags
        assertTrue((wayRecord.getTags() == null)
                || PostgresUtils.postgresObjToHStore(wayRecord.getTags()).isEmpty());

        wayRecord = ways.get(wayIdsArr[2]);

        assertEquals(new Long(changesetId), wayRecord.getChangesetId());
        assertEquals(wayIdsArr[2], wayRecord.getId());
        assertTrue(wayRecord.getTimestamp().before(now));
        assertEquals(new Long(1), wayRecord.getVersion());
        assertTrue(wayRecord.getVisible());

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

        // verify the previously existing tags
        tags = PostgresUtils.postgresObjToHStore(wayRecord.getTags());

        assertNotNull(tags);
        assertEquals(1, tags.size());
        assertEquals("val 3", tags.get("key 3"));

        // verify the deleted way
        assertNull(createQuery(mapId)
                .select(currentWays)
                .from(currentWays)
                .where(currentWays.id.eq(wayIdsArr[0]))
                .fetchOne());

        Changesets changeset = createQuery(mapId)
                .select(changesets)
                .from(changesets)
                .where(changesets.id.eq(changesetId))
                .fetchOne();

        assertNotNull(changeset);
        assertTrue(changeset.getCreatedAt().before(now));
        assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));
        assertEquals(new Integer(10), changeset.getNumChanges());
        assertEquals(new Long(userId), changeset.getUserId());

        BoundingBox expandedBounds = new BoundingBox(originalBounds);
        expandedBounds.expand(originalBounds, Double.parseDouble(CHANGESET_BOUNDS_EXPANSION_FACTOR_DEEGREES));
        Changeset hootChangeset = new Changeset(mapId, changesetId);
        BoundingBox changesetBounds = hootChangeset.getBounds();
        assertEquals(changesetBounds, expandedBounds);
    }

    @Test(expected = BadRequestException.class)
    @Category(UnitTest.class)
    public void testUploadDeleteNoNodeCoords() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        try {
            // Now, delete one of the nodes, one of the ways, and one of the
            // relations. An error should be returned and no data in the system deleted, since we require
            // passing in nodes with their coords...even for a delete.
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create/>" +
                        "<modify/>" +
                        "<delete if-unused=\"true\">" +
                            "<node id=\"" + nodeIdsArr[3] + "\" version=\"1\" " + "changeset=\"" + changesetId + "\">" +
                                "<tag k=\"key 1\" v=\"val 1\"></tag>" +
                            "</node>" +
                            "<way id=\"" + wayIdsArr[2] + "\" version=\"1\" " + "changeset=\"" + changesetId + "\">" +
                                "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                                "<tag k=\"key 1\" v=\"val 1\"></tag>" +
                            "</way>" +
                            "<relation id=\"" + relationIdsArr[2] + "\" version=\"1\" " + "changeset=\"" + changesetId + "\">" +
                                "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[2] + "\"></member>" +
                                "<tag k=\"key 4\" v=\"val 4\"></tag>" +
                            "</relation>" +
                        "</delete>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (BadRequestException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.BAD_REQUEST, Response.Status.fromStatusCode(r.getStatus()));
            //OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testUploadDeleteNoWayNodes() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // Now, delete one of the nodes, one of the ways, and one of the relations.
        testUploadDelete(
            "<osmChange version=\"0.3\" generator=\"iD\">" +
                "<create/>" +
                "<modify/>" +
                "<delete if-unused=\"true\">" +
                    "<node id=\"" + nodeIdsArr[3] + "\" lon=\"" + originalBounds.getMinLon() +
                         "\" lat=\"" + originalBounds.getMinLat() + "\" version=\"1\" " + "changeset=\"" +
                            changesetId + "\">" +
                        "<tag k=\"key 1\" v=\"val 1\"></tag>" +
                    "</node>" +
                    "<way id=\"" + wayIdsArr[2] + "\" version=\"1\" " + "changeset=\"" + changesetId + "\">" +
                        "<tag k=\"key 1\" v=\"val 1\"></tag>" + "</way>" + "<relation id=\"" + relationIdsArr[2] +
                               "\" version=\"1\" " + "changeset=\"" + changesetId + "\">" +
                        "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[2] + "\"></member>" +
                        "<tag k=\"key 4\" v=\"val 4\"></tag>" +
                    "</relation>" +
                "</delete>" +
            "</osmChange>",
            originalBounds, changesetId, nodeIdsArr, wayIdsArr, relationIdsArr);
    }

    @Test
    @Category(UnitTest.class)
    public void testUploadDeleteNoRelationMembers() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // Now, delete one of the nodes, one of the ways, and one of the
        // relations.
        testUploadDelete(
            "<osmChange version=\"0.3\" generator=\"iD\">" +
                "<create/>" +
                "<modify/>" +
                "<delete if-unused=\"true\">" +
                    "<node id=\"" + nodeIdsArr[3] + "\" lon=\"" + originalBounds.getMinLon() + "\" lat=\"" +
                        originalBounds.getMinLat() + "\" version=\"1\" " + "changeset=\"" + changesetId + "\">" +
                        "<tag k=\"key 1\" v=\"val 1\"></tag>" +
                    "</node>" +
                    "<way id=\"" + wayIdsArr[2] + "\" version=\"1\" " + "changeset=\"" + changesetId + "\">" +
                        "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                        "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                        "<tag k=\"key 1\" v=\"val 1\"></tag>" +
                    "</way>" +
                    "<relation id=\"" + relationIdsArr[2] + "\" version=\"1\" " + "changeset=\"" + changesetId + "\">" +
                        "<tag k=\"key 4\" v=\"val 4\"></tag>" +
                    "</relation>" +
                "</delete>" +
            "</osmChange>",
            originalBounds, changesetId, nodeIdsArr, wayIdsArr, relationIdsArr);
    }

    @Test
    @Category(UnitTest.class)
    public void testUploadDeleteRelationContainsAnotherRelation() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // now delete one of the nodes, one of the ways, and one of the relations
        Document responseData = target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create/>" +
                        "<modify/>" +
                        "<delete if-unused=\"true\">" +
                            "<node id=\"" + nodeIdsArr[3] + "\" lon=\"" + originalBounds.getMinLon() +
                                 "\" lat=\"" + originalBounds.getMinLat() + "\" version=\"1\" " +
                                 // i don't understand why you would ever put these tags here at all
                                 // if you're going to delete the nodes, which will automatically cause the
                                 // tags to be deleted too (according to the rails port code)
                                "changeset=\"" + changesetId + "\">" +
                                "<tag k=\"key 1\" v=\"val 1\"></tag>" +
                            "</node>" +
                            "<way id=\"" + wayIdsArr[2] + "\" version=\"1\" " + "changeset=\"" + changesetId + "\">" +
                                "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                                "<tag k=\"key 1\" v=\"val 1\"></tag>" +
                            "</way>" +
                            "<relation id=\"" + relationIdsArr[1] + "\" version=\"1\" " + "changeset=\""
                                + changesetId + "\">" +
                            "</relation>" +
                        "</delete>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);

        assertNotNull(responseData);

        XPath xpath = XmlUtils.createXPath();
        NodeList returnedNodes = XPathAPI.selectNodeList(responseData, "//osm/diffResult/node");
        assertEquals(1, returnedNodes.getLength());

        // check that the returned IDs are correct and the correct attributes were returned
        assertEquals((long) nodeIdsArr[3],
                Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@old_id", responseData)));
        org.w3c.dom.Node xml = XPathAPI.selectSingleNode(responseData, "//osm/diffResult/node[1]");
        assertNull(xml.getAttributes().getNamedItem("new_id"));
        assertNull(xml.getAttributes().getNamedItem("new_version"));

        NodeList returnedWays = XPathAPI.selectNodeList(responseData, "//osm/diffResult/way");
        assertEquals(1, returnedWays.getLength());

        assertEquals((long) wayIdsArr[2],
                Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@old_id", responseData)));
        xml = XPathAPI.selectSingleNode(responseData, "//osm/diffResult/way[1]");
        assertNull(xml.getAttributes().getNamedItem("new_id"));
        assertNull(xml.getAttributes().getNamedItem("new_version"));

        NodeList returnedRelations = XPathAPI.selectNodeList(responseData, "//osm/diffResult/relation");
        assertEquals(1, returnedRelations.getLength());

        assertEquals((long) relationIdsArr[1],
                Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@old_id", responseData)));
        xml = XPathAPI.selectSingleNode(responseData, "//osm/diffResult/relation[1]");
        assertNull(xml.getAttributes().getNamedItem("new_id"));
        assertNull(xml.getAttributes().getNamedItem("new_version"));

        Timestamp now = super.getCurrentDBTime();

        Map<Long, CurrentNodes> nodes = createQuery(mapId)
                .from(currentNodes)
                .transform(groupBy(currentNodes.id).as(currentNodes));

        assertEquals(4, nodes.size());

        CurrentNodes nodeRecord = nodes.get(nodeIdsArr[0]);
        assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
        assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
        assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
        assertEquals(nodeIdsArr[0], nodeRecord.getId());
        assertEquals(new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                nodeRecord.getTile());
        assertTrue(nodeRecord.getTimestamp().before(now));
        assertEquals(new Long(1), nodeRecord.getVersion());
        assertTrue(nodeRecord.getVisible());
        Map<String, String> tags = PostgresUtils.postgresObjToHStore(nodeRecord.getTags());
        assertNotNull(tags);
        assertEquals(2, tags.size());
        assertEquals("val 1", tags.get("key 1"));
        assertEquals("val 2", tags.get("key 2"));

        nodeRecord = nodes.get(nodeIdsArr[1]);
        assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
        assertEquals(new Double(originalBounds.getMaxLat()), nodeRecord.getLatitude());
        assertEquals(new Double(originalBounds.getMaxLon()), nodeRecord.getLongitude());
        assertEquals(nodeIdsArr[1], nodeRecord.getId());
        assertEquals(new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                nodeRecord.getTile());
        assertTrue(nodeRecord.getTimestamp().before(now));
        assertEquals(new Long(1), nodeRecord.getVersion());
        assertTrue(nodeRecord.getVisible());
        assertTrue((nodeRecord.getTags() == null) || PostgresUtils.postgresObjToHStore(nodeRecord.getTags()).isEmpty());

        nodeRecord = nodes.get(nodeIdsArr[2]);
        assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
        assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
        assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
        assertEquals(nodeIdsArr[2], nodeRecord.getId());
        assertEquals(new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                nodeRecord.getTile());
        assertTrue(nodeRecord.getTimestamp().before(now));
        assertEquals(new Long(1), nodeRecord.getVersion());
        assertTrue(nodeRecord.getVisible());
        assertTrue((nodeRecord.getTags() == null) || PostgresUtils.postgresObjToHStore(nodeRecord.getTags()).isEmpty());

        nodeRecord = nodes.get(nodeIdsArr[4]);
        assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
        assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
        assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
        assertEquals(nodeIdsArr[4], nodeRecord.getId());
        assertEquals(new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                nodeRecord.getTile());
        assertTrue(nodeRecord.getTimestamp().before(now));
        assertEquals(new Long(1), nodeRecord.getVersion());
        assertTrue(nodeRecord.getVisible());
        tags = PostgresUtils.postgresObjToHStore(nodeRecord.getTags());
        assertNotNull(tags);
        assertEquals(1, tags.size());
        assertEquals("val 4", tags.get("key 4"));

        // verify the deleted node
        nodeRecord = createQuery(mapId)
                .select(currentNodes)
                .from(currentNodes)
                .where(currentNodes.id.eq(nodeIdsArr[3]))
                .fetchOne();

        assertNull(nodeRecord);

        Map<Long, CurrentWays> ways = createQuery(mapId)
                .from(currentWays)
                .transform(groupBy(currentWays.id).as(currentWays));

        assertEquals(2, ways.size());

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

        assertEquals(3, wayNodes.size());
        CurrentWayNodes wayNode = wayNodes.get(0);
        assertEquals(nodeIdsArr[0], wayNode.getNodeId());
        assertEquals(new Long(1), wayNode.getSequenceId());
        assertEquals(wayRecord.getId(), wayNode.getWayId());
        wayNode = wayNodes.get(1);
        assertEquals(nodeIdsArr[1], wayNode.getNodeId());
        assertEquals(new Long(2), wayNode.getSequenceId());
        assertEquals(wayRecord.getId(), wayNode.getWayId());
        wayNode = wayNodes.get(2);
        assertEquals(nodeIdsArr[4], wayNode.getNodeId());
        assertEquals(new Long(3), wayNode.getSequenceId());
        assertEquals(wayRecord.getId(), wayNode.getWayId());
        // verify the previously existing tags

        tags = PostgresUtils.postgresObjToHStore(wayRecord.getTags());
        assertNotNull(tags);
        assertEquals(2, tags.size());
        assertEquals("val 1", tags.get("key 1"));
        assertEquals("val 2", tags.get("key 2"));

        wayRecord = ways.get(wayIdsArr[1]);
        assertEquals(new Long(changesetId), wayRecord.getChangesetId());
        assertEquals(wayIdsArr[1], wayRecord.getId());
        assertTrue(wayRecord.getTimestamp().before(now));
        assertEquals(new Long(1), wayRecord.getVersion());
        assertTrue(wayRecord.getVisible());

        wayNodes = createQuery(mapId)
                .select(currentWayNodes)
                .from(currentWayNodes)
                .where(currentWayNodes.wayId.eq(wayIdsArr[1]))
                .orderBy(currentWayNodes.sequenceId.asc())
                .fetch();

        assertEquals(2, wayNodes.size());
        wayNode = wayNodes.get(0);
        assertEquals(nodeIdsArr[2], wayNode.getNodeId());
        assertEquals(new Long(1), wayNode.getSequenceId());
        assertEquals(wayRecord.getId(), wayNode.getWayId());
        wayNode = wayNodes.get(1);
        assertEquals(nodeIdsArr[1], wayNode.getNodeId());
        assertEquals(new Long(2), wayNode.getSequenceId());
        assertEquals(wayRecord.getId(), wayNode.getWayId());

        // verify the way with no tags
        assertTrue((wayRecord.getTags() == null)
                || PostgresUtils.postgresObjToHStore(wayRecord.getTags()).isEmpty());

        // verify the deleted way
        assertNull(createQuery(mapId)
                .select(currentWays)
                .from(currentWays)
                .where(currentWays.id.eq(wayIdsArr[2]))
                .fetchOne());

        Map<Long, CurrentRelations> relations = createQuery(mapId)
                .from(currentRelations)
                .transform(groupBy(currentRelations.id).as(currentRelations));

        assertEquals(3, relations.size());

        // verify the previously existing relations
        CurrentRelations relationRecord = relations.get(relationIdsArr[0]);
        assertEquals(new Long(changesetId), relationRecord.getChangesetId());
        assertEquals(relationIdsArr[0], relationRecord.getId());
        assertTrue(relationRecord.getTimestamp().before(now));
        assertEquals(new Long(1), relationRecord.getVersion());
        assertTrue(relationRecord.getVisible());

        List<CurrentRelationMembers> members =
                createQuery(mapId)
                        .select(currentRelationMembers)
                        .from(currentRelationMembers)
                        .where(currentRelationMembers.relationId.eq(relationIdsArr[0]))
                        .orderBy(currentRelationMembers.sequenceId.asc())
                        .fetch();

        assertEquals(4, members.size());
        CurrentRelationMembers member = members.get(0);
        assertEquals(relationRecord.getId(), member.getRelationId());
        assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
        assertEquals("role1", member.getMemberRole());
        assertEquals(new Integer(1), member.getSequenceId());

        assertEquals(nodeIdsArr[0], member.getMemberId());
        member = members.get(1);
        assertEquals(relationRecord.getId(), member.getRelationId());
        assertEquals(DbUtils.nwr_enum.way, member.getMemberType());
        assertEquals("role3", member.getMemberRole());
        assertEquals(new Integer(2), member.getSequenceId());

        assertEquals(wayIdsArr[1], member.getMemberId());
        member = members.get(2);
        assertEquals(relationRecord.getId(), member.getRelationId());
        assertEquals(DbUtils.nwr_enum.way, member.getMemberType());
        assertEquals("role2", member.getMemberRole());
        assertEquals(new Integer(3), member.getSequenceId());

        assertEquals(wayIdsArr[0], member.getMemberId());
        member = members.get(3);
        assertEquals(relationRecord.getId(), member.getRelationId());
        assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
        assertEquals("", member.getMemberRole());
        assertEquals(new Integer(4), member.getSequenceId());

        assertEquals(nodeIdsArr[2], member.getMemberId());
        // verify the previously existing tags
        tags = PostgresUtils.postgresObjToHStore(relationRecord.getTags());
        assertNotNull(tags);
        assertEquals(1, tags.size());
        assertEquals("val 1", tags.get("key 1"));

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

        // verify the deleted relation
        assertNull(createQuery(mapId)
                .select(currentRelations)
                .from(currentRelations)
                .where(currentRelations.id.eq(relationIdsArr[1]))
                .fetchOne());

        Changesets changeset = createQuery(mapId)
                .select(changesets)
                .from(changesets)
                .where(changesets.id.eq(changesetId))
                .fetchOne();

        assertNotNull(changeset);
        assertTrue(changeset.getCreatedAt().before(now));
        assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));

        assertEquals(new Integer(15), changeset.getNumChanges());
        assertEquals(new Long(userId), changeset.getUserId());

        BoundingBox expandedBounds = new BoundingBox(originalBounds);
        expandedBounds.expand(originalBounds, Double.parseDouble(CHANGESET_BOUNDS_EXPANSION_FACTOR_DEEGREES));
        Changeset hootChangeset = new Changeset(mapId, changesetId);
        BoundingBox changesetBounds = hootChangeset.getBounds();
        assertEquals(changesetBounds, expandedBounds);
    }

    /**
     * //TODO: this method is nearly identical to testUploadDelete
     */
    @Test
    @Category(UnitTest.class)
    public void testUploadDeleteNoTags() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // now delete some elements
        Document responseData = target("api/0.6/changeset/" + changesetId + "/upload")
            .queryParam("mapId", String.valueOf(mapId))
            .request(MediaType.TEXT_XML).accept(MediaType.TEXT_XML)
            .post(Entity.entity(
                "<osmChange version=\"0.3\" generator=\"iD\">" +
                    "<create/>" +
                    "<modify/>" +
                    "<delete if-unused=\"true\">" +
                        "<node id=\"" + nodeIdsArr[3] + "\" lon=\"" + originalBounds.getMinLon() +
                               "\" lat=\"" + originalBounds.getMinLat() + "\" version=\"1\" " +
                             // i don't understand why you would ever put these tags here at all
                             // if you're going to delete the nodes, which will automatically cause the
                             // tags to be deleted too (according to the rails port code)
                             "changeset=\"" + changesetId + "\">" +
                        "</node>" +
                        "<way id=\"" + wayIdsArr[2] + "\" version=\"1\" " + "changeset=\"" +
                             changesetId + "\">" +
                            "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                            "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                        "</way>" +
                        "<relation id=\"" + relationIdsArr[2] + "\" version=\"1\" " +
                             "changeset=\"" + changesetId + "\">" + "<member type=\"way\" " +
                             "role=\"role1\" ref=\"" + wayIdsArr[2] + "\"></member>" +
                        "</relation>" +
                    "</delete>" +
                "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);

        assertNotNull(responseData);

        XPath xpath = XmlUtils.createXPath();
        NodeList returnedNodes = XPathAPI.selectNodeList(responseData, "//osm/diffResult/node");
        assertEquals(1, returnedNodes.getLength());

        // check that the returned IDs are correct and the correct attributes were returned
        assertEquals((long) nodeIdsArr[3],
                Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@old_id", responseData)));
        org.w3c.dom.Node xml = XPathAPI.selectSingleNode(responseData, "//osm/diffResult/node[1]");
        assertNull(xml.getAttributes().getNamedItem("new_id"));
        assertNull(xml.getAttributes().getNamedItem("new_version"));

        NodeList returnedWays = XPathAPI.selectNodeList(responseData, "//osm/diffResult/way");
        assertEquals(1, returnedWays.getLength());

        assertEquals((long) wayIdsArr[2],
                Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@old_id", responseData)));
        xml = XPathAPI.selectSingleNode(responseData, "//osm/diffResult/way[1]");
        assertNull(xml.getAttributes().getNamedItem("new_id"));
        assertNull(xml.getAttributes().getNamedItem("new_version"));

        NodeList returnedRelations = XPathAPI.selectNodeList(responseData, "//osm/diffResult/relation");
        assertEquals(1, returnedRelations.getLength());

        assertEquals((long) relationIdsArr[2],
                Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@old_id", responseData)));
        xml = XPathAPI.selectSingleNode(responseData, "//osm/diffResult/relation[1]");
        assertNull(xml.getAttributes().getNamedItem("new_id"));
        assertNull(xml.getAttributes().getNamedItem("new_version"));

        Timestamp now = super.getCurrentDBTime();

        Map<Long, CurrentNodes> nodes = createQuery(mapId)
                .from(currentNodes)
                .transform(groupBy(currentNodes.id).as(currentNodes));

        assertEquals(4, nodes.size());

        CurrentNodes nodeRecord = nodes.get(nodeIdsArr[0]);
        assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
        assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
        assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
        assertEquals(nodeIdsArr[0], nodeRecord.getId());
        assertEquals(new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                nodeRecord.getTile());
        assertTrue(nodeRecord.getTimestamp().before(now));
        assertEquals(new Long(1), nodeRecord.getVersion());
        assertTrue(nodeRecord.getVisible());

        Map<String, String> tags = PostgresUtils.postgresObjToHStore(nodeRecord.getTags());
        assertNotNull(tags);
        assertEquals(2, tags.size());
        assertEquals("val 1", tags.get("key 1"));
        assertEquals("val 2", tags.get("key 2"));

        nodeRecord = nodes.get(nodeIdsArr[1]);
        assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
        assertEquals(new Double(originalBounds.getMaxLat()), nodeRecord.getLatitude());
        assertEquals(new Double(originalBounds.getMaxLon()), nodeRecord.getLongitude());
        assertEquals(nodeIdsArr[1], nodeRecord.getId());
        assertEquals(new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                nodeRecord.getTile());
        assertTrue(nodeRecord.getTimestamp().before(now));
        assertEquals(new Long(1), nodeRecord.getVersion());
        assertTrue(nodeRecord.getVisible());
        assertTrue((nodeRecord.getTags() == null) || PostgresUtils.postgresObjToHStore(nodeRecord.getTags()).isEmpty());

        nodeRecord = nodes.get(nodeIdsArr[2]);
        assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
        assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
        assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
        assertEquals(nodeIdsArr[2], nodeRecord.getId());
        assertEquals(new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                nodeRecord.getTile());
        assertTrue(nodeRecord.getTimestamp().before(now));
        assertEquals(new Long(1), nodeRecord.getVersion());
        assertTrue(nodeRecord.getVisible());
        assertTrue((nodeRecord.getTags() == null) || PostgresUtils.postgresObjToHStore(nodeRecord.getTags()).isEmpty());

        nodeRecord = nodes.get(nodeIdsArr[4]);
        assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
        assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
        assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
        assertEquals(nodeIdsArr[4], nodeRecord.getId());
        assertEquals(new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                nodeRecord.getTile());
        assertTrue(nodeRecord.getTimestamp().before(now));
        assertEquals(new Long(1), nodeRecord.getVersion());
        assertTrue(nodeRecord.getVisible());
        tags = PostgresUtils.postgresObjToHStore(nodeRecord.getTags());
        assertNotNull(tags);
        assertEquals(1, tags.size());
        assertEquals("val 4", tags.get("key 4"));

        // verify the deleted node
        nodeRecord = createQuery(mapId)
                .select(currentNodes)
                .from(currentNodes)
                .where(currentNodes.id.eq(nodeIdsArr[3]))
                .fetchOne();

        assertNull(nodeRecord);

        Map<Long, CurrentWays> ways = createQuery(mapId)
                .from(currentWays)
                .transform(groupBy(currentWays.id).as(currentWays));

        assertEquals(2, ways.size());

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

        assertEquals(3, wayNodes.size());
        CurrentWayNodes wayNode = wayNodes.get(0);
        assertEquals(nodeIdsArr[0], wayNode.getNodeId());
        assertEquals(new Long(1), wayNode.getSequenceId());
        assertEquals(wayRecord.getId(), wayNode.getWayId());
        wayNode = wayNodes.get(1);
        assertEquals(nodeIdsArr[1], wayNode.getNodeId());
        assertEquals(new Long(2), wayNode.getSequenceId());
        assertEquals(wayRecord.getId(), wayNode.getWayId());
        wayNode = wayNodes.get(2);
        assertEquals(nodeIdsArr[4], wayNode.getNodeId());
        assertEquals(new Long(3), wayNode.getSequenceId());
        assertEquals(wayRecord.getId(), wayNode.getWayId());
        // verify the previously existing tags
        tags = PostgresUtils.postgresObjToHStore(wayRecord.getTags());
        assertNotNull(tags);
        assertEquals(2, tags.size());
        assertEquals("val 1", tags.get("key 1"));
        assertEquals("val 2", tags.get("key 2"));

        wayRecord = ways.get(wayIdsArr[1]);
        assertEquals(new Long(changesetId), wayRecord.getChangesetId());
        assertEquals(wayIdsArr[1], wayRecord.getId());
        assertTrue(wayRecord.getTimestamp().before(now));
        assertEquals(new Long(1), wayRecord.getVersion());
        assertTrue(wayRecord.getVisible());

        wayNodes = createQuery(mapId)
                .select(currentWayNodes)
                .from(currentWayNodes)
                .where(currentWayNodes.wayId.eq(wayIdsArr[1]))
                .orderBy(currentWayNodes.sequenceId.asc())
                .fetch();

        assertEquals(2, wayNodes.size());
        wayNode = wayNodes.get(0);
        assertEquals(nodeIdsArr[2], wayNode.getNodeId());
        assertEquals(new Long(1), wayNode.getSequenceId());
        assertEquals(wayRecord.getId(), wayNode.getWayId());
        wayNode = wayNodes.get(1);
        assertEquals(nodeIdsArr[1], wayNode.getNodeId());
        assertEquals(new Long(2), wayNode.getSequenceId());
        assertEquals(wayRecord.getId(), wayNode.getWayId());
        // verify the way with no tags
        assertTrue((wayRecord.getTags() == null)
                || PostgresUtils.postgresObjToHStore(wayRecord.getTags()).isEmpty());

        // verify the deleted way
        assertNull(
                createQuery(mapId)
                        .select(currentWays)
                        .from(currentWays)
                        .where(currentWays.id.eq(wayIdsArr[2]))
                        .fetchOne());

        Map<Long, CurrentRelations> relations = createQuery(mapId)
                .from(currentRelations)
                .transform(groupBy(currentRelations.id).as(currentRelations));

        assertEquals(3, relations.size());

        // verify the previously existing relations
        CurrentRelations relationRecord = relations.get(relationIdsArr[0]);
        assertEquals(new Long(changesetId), relationRecord.getChangesetId());
        assertEquals(relationIdsArr[0], relationRecord.getId());
        assertTrue(relationRecord.getTimestamp().before(now));
        assertEquals(new Long(1), relationRecord.getVersion());
        assertTrue(relationRecord.getVisible());

        List<CurrentRelationMembers> members =
                createQuery(mapId)
                        .select(currentRelationMembers)
                        .from(currentRelationMembers)
                        .where(currentRelationMembers.relationId.eq(relationIdsArr[0]))
                        .orderBy(currentRelationMembers.sequenceId.asc())
                        .fetch();

        assertEquals(4, members.size());
        CurrentRelationMembers member = members.get(0);
        assertEquals(relationRecord.getId(), member.getRelationId());
        assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
        assertEquals("role1", member.getMemberRole());
        assertEquals(new Integer(1), member.getSequenceId());

        assertEquals(nodeIdsArr[0], member.getMemberId());
        member = members.get(1);
        assertEquals(relationRecord.getId(), member.getRelationId());
        assertEquals(DbUtils.nwr_enum.way, member.getMemberType());
        assertEquals("role3", member.getMemberRole());
        assertEquals(new Integer(2), member.getSequenceId());

        assertEquals(wayIdsArr[1], member.getMemberId());
        member = members.get(2);
        assertEquals(relationRecord.getId(), member.getRelationId());
        assertEquals(DbUtils.nwr_enum.way, member.getMemberType());
        assertEquals("role2", member.getMemberRole());
        assertEquals(new Integer(3), member.getSequenceId());

        assertEquals(wayIdsArr[0], member.getMemberId());
        member = members.get(3);
        assertEquals(relationRecord.getId(), member.getRelationId());
        assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
        assertEquals("", member.getMemberRole());
        assertEquals(new Integer(4), member.getSequenceId());

        assertEquals(nodeIdsArr[2], member.getMemberId());
        // verify the previously existing tags
        tags = PostgresUtils.postgresObjToHStore(relationRecord.getTags());
        assertNotNull(tags);
        assertEquals(1, tags.size());
        assertEquals("val 1", tags.get("key 1"));

        relationRecord = relations.get(relationIdsArr[1]);
        assertEquals(new Long(changesetId), relationRecord.getChangesetId());
        assertEquals(relationIdsArr[1], relationRecord.getId());

        assertTrue(relationRecord.getTimestamp().before(now));
        assertEquals(new Long(1), relationRecord.getVersion());
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
        assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
        assertEquals("role1", member.getMemberRole());
        assertEquals(new Integer(1), member.getSequenceId());

        assertEquals(nodeIdsArr[4], member.getMemberId());
        member = members.get(1);
        assertEquals(relationRecord.getId(), member.getRelationId());
        assertEquals(DbUtils.nwr_enum.relation, member.getMemberType());
        assertEquals("role1", member.getMemberRole());
        assertEquals(new Integer(2), member.getSequenceId());

        assertEquals(relationIdsArr[0], member.getMemberId());
        tags = PostgresUtils.postgresObjToHStore(relationRecord.getTags());
        assertNotNull(tags);
        assertEquals(2, tags.size());
        assertEquals("val 2", tags.get("key 2"));
        assertEquals("val 3", tags.get("key 3"));

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

        // verify the deleted relation
        assertNull(createQuery(mapId)
                .select(currentRelations)
                .from(currentRelations)
                .where(currentRelations.id.eq(relationIdsArr[2]))
                .fetchOne());

        Changesets changeset = createQuery(mapId)
                .select(changesets)
                .from(changesets)
                .where(changesets.id.eq(changesetId))
                .fetchOne();

        assertNotNull(changeset);
        assertTrue(changeset.getCreatedAt().before(now));
        assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));
        assertEquals(new Integer(15), changeset.getNumChanges());
        assertEquals(new Long(userId), changeset.getUserId());

        BoundingBox expandedBounds = new BoundingBox(originalBounds);
        expandedBounds.expand(originalBounds, Double.parseDouble(CHANGESET_BOUNDS_EXPANSION_FACTOR_DEEGREES));
        Changeset hootChangeset = new Changeset(mapId, changesetId);
        BoundingBox changesetBounds = hootChangeset.getBounds();
        assertEquals(changesetBounds, expandedBounds);
    }

    @Test(expected = NotFoundException.class)
    @Category(UnitTest.class)
    public void testUploadDeleteNonExistingNode() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        BoundingBox updateBounds = OSMTestUtils.createAfterModifiedTestChangesetBounds();
        // Try to delete a node in the changeset that doesn't exist. A failure
        // should occur and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create/>" +
                        "<modify/>" +
                        "<delete if-unused=\"true\">" +
                            "<node id=\"" + nodeIdsArr[3] + "\" lon=\"" + originalBounds.getMinLon() +
                                   "\" " + "lat=\"" + originalBounds.getMinLat() + "\" version=\"1\" changeset=\""
                                   + changesetId + "\">" + "</node>" + "<node id=\"" + (nodeIdsArr[4] + 1) +
                                   "\" lon=\"" + updateBounds.getMinLon() + "\" " + "lat=\"" +
                                  updateBounds.getMinLat() + "\" version=\"1\" changeset=\"" + changesetId + "\">" +
                                "<tag k=\"key 3\" v=\"val 3\"></tag>" +
                            "</node>" +
                        "</delete>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (NotFoundException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.NOT_FOUND, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("Element(s) being referenced don't exist."));

            // make sure that any of the existing nodes weren't deleted
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
    }

    @Test(expected = NotFoundException.class)
    @Category(UnitTest.class)
    public void testUploadDeleteNonExistingWay() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Try to delete a way in the changeset that doesn't exist. A failure
        // should occur and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                     "<osmChange version=\"0.3\" generator=\"iD\">" +
                         "<create/>" +
                         "<modify/>" +
                         "<delete if-unused=\"true\">" +
                             "<way id=\"" + wayIdsArr[2] + "\" version=\"1\" changeset=\"" + changesetId + "\">" +
                                 "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                 "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                             "</way>" +
                             "<way id=\"" + (wayIdsArr[2] + 1) + "\" version=\"1\" changeset=\"" + changesetId + "\">" + "</way>" +
                         "</delete>" +
                     "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (NotFoundException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.NOT_FOUND, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("Element(s) being referenced don't exist."));

            // make sure that any of the existing nodes weren't deleted
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test(expected = NotFoundException.class)
    @Category(UnitTest.class)
    public void testUploadDeleteNonExistingRelation() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // Try to delete a relation in the changeset that doesn't exist. A
        // failure should occur and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create/>" +
                        "<modify/>" +
                        "<delete if-unused=\"true\">" +
                            "<relation id=\"" + relationIdsArr[3] + "\" version=\"1\" changeset=\"" + changesetId + "\">" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[1] + "\"></member>" +
                            "</relation>" +
                            "<relation id=\"" + (relationIdsArr[3] + 1) + "\" version=\"1\" changeset=\""
                                + changesetId + "\">" +
                            "</relation>" +
                        "</delete>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (NotFoundException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.NOT_FOUND, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("Element(s) being referenced don't exist."));

            // make sure that any of the existing nodes weren't deleted
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test(expected = BadRequestException.class)
    @Category(UnitTest.class)
    public void testUploadDeleteDuplicateNodeIds() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Try to delete two nodes with the same ID. A failure should occur and
        // no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create/>" +
                        "<modify/>" +
                        "<delete if-unused=\"true\">" +
                            "<node id=\"" + nodeIdsArr[3] + "\" lon=\"" + originalBounds.getMinLon() + "\" " +
                                   "lat=\"" + originalBounds.getMinLat() + "\" version=\"1\" changeset=\"" + changesetId + "\">" +
                                 "<tag k=\"key 1\" v=\"val 1\"></tag>" + "<tag k=\"key 2\" v=\"val 2\"></tag>" +
                            "</node>" +
                            "<node id=\"" + nodeIdsArr[3] + "\" lon=\"" + originalBounds.getMinLon() + "\" " +
                                    "lat=\"" + originalBounds.getMinLat() + "\" version=\"1\" changeset=\"" + changesetId + "\">" +
                                 "<tag k=\"key 1\" v=\"val 1\"></tag>" + "<tag k=\"key 2\" v=\"val 2\"></tag>" +
                            "</node>" +
                        "</delete>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (BadRequestException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.BAD_REQUEST, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("Duplicate OSM element ID"));

            // make sure that no nodes were deleted
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test(expected = BadRequestException.class)
    @Category(UnitTest.class)
    public void testUploadDeleteDuplicateWayIds() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Try to delete two ways with the same ID. A failure should occur and
        // no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create/>" +
                        "<modify/>" +
                        "<delete if-unused=\"true\">" +
                            "<way id=\"" + wayIdsArr[2] + "\" version=\"1\" changeset=\"" + changesetId + "\">" +
                                "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                                "<tag k=\"key 3\" v=\"val 3\"></tag>" +
                            "</way>" +
                            "<way id=\"" + wayIdsArr[2] + "\" version=\"1\" changeset=\"" + changesetId + "\">" + "</way>" +
                        "</delete>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (BadRequestException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.BAD_REQUEST, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("Duplicate OSM element ID"));

            // make sure that no nodes were deleted
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test(expected = BadRequestException.class)
    @Category(UnitTest.class)
    public void testUploadDeleteDuplicateRelationIds() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // Try to delete two relations with the same ID. A failure should occur
        // and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create/>" +
                        "<modify/>" +
                        "<delete if-unused=\"true\">" +
                            "<relation id=\"" + relationIdsArr[3] + "\" version=\"1\" changeset=\"" + changesetId + "\">" +
                                "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[2] + "\"></member>" +
                                "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[1] + "\"></member>" +
                                "<tag k=\"key 1\" v=\"val 1\"></tag>" +
                                "<tag k=\"key 2\" v=\"val 2\"></tag>" +
                            "</relation>" +
                            "<relation id=\"" + relationIdsArr[3] + "\" version=\"1\" changeset=\"" + changesetId + "\">" +
                                "<tag k=\"key 1\" v=\"val 1\"></tag>" +
                                "<tag k=\"key 2\" v=\"val 2\"></tag>" +
                            "</relation>" +
                        "</delete>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (BadRequestException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.BAD_REQUEST, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("Duplicate OSM element ID"));

            // make sure that no nodes were deleted
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test(expected = ClientErrorException.class)
    @Category(UnitTest.class)
    public void testUploadDeleteNodeInvalidVersion() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Update the changeset where one of the nodes has version that doesn't
        // match the version on he server. A failure should occur and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create/>" +
                        "<modify/>" +
                        "<delete if-unused=\"true\">" +
                            "<node id=\"" + nodeIdsArr[3] + "\" lon=\"" + originalBounds.getMinLon() + "\" " +
                                "lat=\"" + originalBounds.getMinLat() + "\" version=\"1\" changeset=\"" + changesetId + "\">" +
                                "<tag k=\"key 1\" v=\"val 1\"></tag>" + "<tag k=\"key 2\" v=\"val 2\"></tag>" +
                            "</node>" +
                            "<node id=\"" + nodeIdsArr[4] + "\" lon=\"" + originalBounds.getMinLon() + "\" " +
                                "lat=\"" + originalBounds.getMinLat() + "\" version=\"2\" changeset=\"" + changesetId + "\">" +
                                "<tag k=\"key 3\" v=\"val 3\"></tag>" +
                            "</node>" +
                        "</delete>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (ClientErrorException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.CONFLICT, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("Invalid version"));

            // make sure that no nodes were deleted
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testUploadDeleteNegativeElementId() throws Exception {
        // We allow elements with a negative ID to exist, which allows for
        // using the hoot --convert command as a source of test data for the services.

        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        BoundingBox updateBounds = OSMTestUtils.createAfterModifiedTestChangesetBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);

        // explicitly create a node with a negative ID
        Map<String, String> tags = new HashMap<>();
        tags.put("key 3", "val 3");

        long negativeNodeId = -1;
        insertNew(negativeNodeId, changesetId, mapId, originalBounds.getMaxLat(), originalBounds.getMaxLon(), tags);
        tags.clear();

        CurrentNodes insertedNodeRecord = createQuery(mapId)
                .select(currentNodes)
                .from(currentNodes)
                .where(currentNodes.id.eq(negativeNodeId))
                .fetchOne();

        assertNotNull(insertedNodeRecord);
        assertEquals(new Long(negativeNodeId), insertedNodeRecord.getId());

        // link some nodes to the changeset
        Set<Long> nodeIds = new LinkedHashSet<>();
        nodeIds.add(negativeNodeId);

        tags.put("key 1", "val 1");
        tags.put("key 2", "val 2");

        nodeIds.add(insertNew(changesetId, mapId, originalBounds.getMinLat(), originalBounds.getMinLon(), tags));

        tags.clear();

        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);

        // link some ways to the changeset
        List<Long> wayNodeIds = new ArrayList<>();

        // explicitly create a way with a negative ID
        wayNodeIds.add(nodeIdsArr[0]);
        wayNodeIds.add(nodeIdsArr[1]);

        tags.put("key 1", "val 1");
        tags.put("key 2", "val 2");

        long negativeWayId = -1;
        OSMTestUtils.insertNewWay(negativeWayId, changesetId, mapId, wayNodeIds, tags);
        tags.clear();

        CurrentWays insertedWayRecord = createQuery(mapId)
                .select(currentWays)
                .from(currentWays)
                .where(currentWays.id.eq(negativeWayId))
                .fetchOne();

        assertNotNull(insertedWayRecord);
        assertEquals(new Long(negativeWayId), insertedWayRecord.getId());

        Set<Long> wayIds = new LinkedHashSet<>();
        wayIds.add(negativeWayId);

        wayNodeIds.clear();

        wayNodeIds.add(nodeIdsArr[0]);
        wayNodeIds.add(nodeIdsArr[1]);

        wayIds.add(OSMTestUtils.insertNewWay(changesetId, mapId, wayNodeIds, null));

        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);

        // link some relations to the changeset

        // explicitly create a relation with a negative ID
        List<RelationMember> members = new ArrayList<>();
        members.add(new RelationMember(nodeIdsArr[0], ElementType.Node, "role1"));
        members.add(new RelationMember(wayIdsArr[0], ElementType.Way, "role1"));

        tags.put("key 1", "val 1");
        tags.put("key 2", "val 2");

        long negativeRelationId = -1;
        OSMTestUtils.insertNewRelation(negativeRelationId, changesetId, mapId, members, tags);
        tags.clear();

        CurrentRelations insertedRelationRecord = createQuery(mapId)
                .select(currentRelations)
                .from(currentRelations)
                .where(currentRelations.id.eq(negativeRelationId))
                .fetchOne();

        assertNotNull(insertedRelationRecord);
        assertEquals(new Long(negativeRelationId), insertedRelationRecord.getId());

        Set<Long> relationIds = new LinkedHashSet<>();
        relationIds.add(negativeRelationId);

        members.clear();
        members.add(new RelationMember(nodeIdsArr[1], ElementType.Node, "role1"));
        members.add(new RelationMember(wayIdsArr[1], ElementType.Way, "role1"));
        relationIds.add(OSMTestUtils.insertNewRelation(changesetId, mapId, members, null));

        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // Update the changeset via the service. This update is valid,
        // because the property was changed above.
        Document responseData = target("api/0.6/changeset/" + changesetId + "/upload")
            .queryParam("mapId", String.valueOf(mapId))
            .request(MediaType.TEXT_XML)
            .post(Entity.entity(
                "<osmChange version=\"0.3\" generator=\"iD\">" +
                    "<create/>" +
                    "<modify/>" +
                    "<delete if-unused=\"true\">" +
                        "<node id=\"" + nodeIdsArr[1] + "\" lon=\"" + updateBounds.getMinLon() + "\" " +
                            "lat=\"" + updateBounds.getMinLat() + "\" version=\"1\" changeset=\"" +
                             changesetId + "\">" +
                        "</node>" +
                        "<node id=\"" + nodeIdsArr[0] + "\" lon=\"" + originalBounds.getMaxLon() + "\" " +
                            "lat=\"" + updateBounds.getMinLat() + "\" version=\"1\" changeset=\"" +
                            changesetId + "\">" +
                        "</node>" +
                        "<way id=\"" + wayIdsArr[1] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                            "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                            "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                        "</way>" +
                        "<way id=\"" + wayIdsArr[0] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                            "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                            "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                        "</way>" +
                        "<relation id=\"" + relationIdsArr[1] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                            "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                            "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[1] + "\"></member>" +
                        "</relation>" +
                        "<relation id=\"" + relationIdsArr[0] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                            "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                            "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[0] + "\"></member>" +
                        "</relation>" +
                    "</delete>" +
                "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);

        assertNotNull(responseData);

        XPath xpath = XmlUtils.createXPath();
        NodeList returnedNodes = XPathAPI.selectNodeList(responseData, "//osm/diffResult/node");
        assertEquals(2, returnedNodes.getLength());

        // ID output ordering matches input ordering

        assertEquals((long) nodeIdsArr[1],
                Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@old_id", responseData)));
        org.w3c.dom.Node xml = XPathAPI.selectSingleNode(responseData, "//osm/diffResult/node[1]");
        assertNull(xml.getAttributes().getNamedItem("new_id"));
        assertNull(xml.getAttributes().getNamedItem("new_version"));

        assertEquals(negativeNodeId,
                Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@old_id", responseData)));
        xml = XPathAPI.selectSingleNode(responseData, "//osm/diffResult/node[2]");
        assertNull(xml.getAttributes().getNamedItem("new_id"));
        assertNull(xml.getAttributes().getNamedItem("new_version"));

        NodeList returnedWays = XPathAPI.selectNodeList(responseData, "//osm/diffResult/way");
        assertEquals(2, returnedWays.getLength());

        assertEquals((long) wayIdsArr[1],
                Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@old_id", responseData)));
        xml = XPathAPI.selectSingleNode(responseData, "//osm/diffResult/way[1]");
        assertNull(xml.getAttributes().getNamedItem("new_id"));
        assertNull(xml.getAttributes().getNamedItem("new_version"));

        assertEquals(negativeWayId,
                Long.parseLong(xpath.evaluate("//osm/diffResult/way[2]/@old_id", responseData)));
        xml = XPathAPI.selectSingleNode(responseData, "//osm/diffResult/way[2]");
        assertNull(xml.getAttributes().getNamedItem("new_id"));
        assertNull(xml.getAttributes().getNamedItem("new_version"));

        NodeList returnedRelations = XPathAPI.selectNodeList(responseData, "//osm/diffResult/relation");
        assertEquals(2, returnedRelations.getLength());

        assertEquals((long) relationIdsArr[1],
                Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@old_id", responseData)));
        xml = XPathAPI.selectSingleNode(responseData, "//osm/diffResult/relation[1]");
        assertNull(xml.getAttributes().getNamedItem("new_id"));
        assertNull(xml.getAttributes().getNamedItem("new_version"));

        assertEquals(negativeRelationId,
                Long.parseLong(xpath.evaluate("//osm/diffResult/relation[2]/@old_id", responseData)));
        xml = XPathAPI.selectSingleNode(responseData, "//osm/diffResult/relation[2]");
        assertNull(xml.getAttributes().getNamedItem("new_id"));
        assertNull(xml.getAttributes().getNamedItem("new_version"));

        assertEquals(0, createQuery(mapId).from(currentNodes).fetchCount());
        assertEquals(0, createQuery(mapId).from(currentWays).fetchCount());
        assertEquals(0, createQuery(mapId).from(currentRelations).fetchCount());

        Changesets changeset = createQuery(mapId)
                .select(changesets)
                .from(changesets)
                .where(changesets.id.eq(changesetId))
                .fetchOne();

        assertNotNull(changeset);

        Timestamp now = super.getCurrentDBTime();
        assertTrue(changeset.getCreatedAt().before(now));
        assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));
        assertEquals(new Integer(18), changeset.getNumChanges());
        assertEquals(new Long(userId), changeset.getUserId());

//            BoundingBox expandedBounds = new BoundingBox(originalBounds);
//            expandedBounds.expand(updateBounds, Double.parseDouble(CHANGESET_BOUNDS_EXPANSION_FACTOR_DEEGREES));
//            Changeset hootChangeset = new Changeset(mapId, changesetId);
//            BoundingBox changesetBounds = hootChangeset.getBounds();
//            Assert.assertEquals(changesetBounds, expandedBounds);
    }

    @Test
    @Category(UnitTest.class)
    public void testUploadDeleteMissingNodeTagValue() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds, 5);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);

        // Now delete some nodes with an update to the changeset. One of the
        // nodes being deleted does not have its tag value specified. The test should pass b/c
        // node tags are optional for a delete and not parsed.
        Document responseData = target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create/>" +
                        "<modify/>" +
                        "<delete if-unused=\"true\">" +
                            "<node id=\"" + nodeIdsArr[3] + "\" lon=\"" +
                                originalBounds.getMinLon() + "\" lat=\"" + originalBounds.getMinLat() +
                                "\" version=\"1\" " + "changeset=\"" + changesetId + "\">" +
                            "</node>" +
                            "<node id=\"" + nodeIdsArr[4] + "\" lon=\"" + originalBounds.getMinLon() +
                                "\" lat=\"" + originalBounds.getMinLat() + "\" version=\"1\" " +
                               "changeset=\"" + changesetId + "\">" +
                               "<tag k=\"key 3\"></tag>" +
                            "</node>" +
                        "</delete>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);

        assertNotNull(responseData);

        Timestamp now = super.getCurrentDBTime();
        XPath xpath = XmlUtils.createXPath();
        NodeList returnedNodes = XPathAPI.selectNodeList(responseData, "//osm/diffResult/node");
        assertEquals(2, returnedNodes.getLength());

        assertEquals((long) nodeIdsArr[3],
                Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@old_id", responseData)));

        assertEquals((long) nodeIdsArr[4],
                Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@old_id", responseData)));

        Map<Long, CurrentNodes> nodes = createQuery(mapId)
                .from(currentNodes)
                .transform(groupBy(currentNodes.id).as(currentNodes));

        assertEquals(3, nodes.size());

        CurrentNodes nodeRecord = nodes.get(nodeIdsArr[0]);
        Map<String, String> tags = PostgresUtils.postgresObjToHStore(nodeRecord.getTags());
        assertNotNull(tags);
        assertEquals(2, tags.size());
        assertEquals("val 1", tags.get("key 1"));
        assertEquals("val 2", tags.get("key 2"));

        nodeRecord = nodes.get(nodeIdsArr[1]);
        assertTrue((nodeRecord.getTags() == null)
                || PostgresUtils.postgresObjToHStore(nodeRecord.getTags()).isEmpty());

        nodeRecord = nodes.get(nodeIdsArr[2]);
        assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
        assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
        assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
        assertEquals(nodeIdsArr[2], nodeRecord.getId());
        assertEquals(
                new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                nodeRecord.getTile());
        assertTrue(nodeRecord.getTimestamp().before(now));
        assertEquals(new Long(1), nodeRecord.getVersion());
        assertTrue(nodeRecord.getVisible());
        assertTrue((nodeRecord.getTags() == null)
                || PostgresUtils.postgresObjToHStore(nodeRecord.getTags()).isEmpty());

        assertNull(nodes.get(nodeIdsArr[3]));
        assertNull(nodes.get(nodeIdsArr[4]));

        Changesets changeset = createQuery(mapId)
                .select(changesets)
                .from(changesets)
                .where(changesets.id.eq(changesetId))
                .fetchOne();

        assertNotNull(changeset);
        assertTrue(changeset.getCreatedAt().before(now));
        assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));
        assertEquals(new Integer(7), changeset.getNumChanges());
        assertEquals(new Long(userId), changeset.getUserId());

        BoundingBox expandedBounds = new BoundingBox(originalBounds);
        expandedBounds.expand(originalBounds, Double.parseDouble(CHANGESET_BOUNDS_EXPANSION_FACTOR_DEEGREES));
        Changeset hootChangeset = new Changeset(mapId, changesetId);
        BoundingBox changesetBounds = hootChangeset.getBounds();
        assertEquals(changesetBounds, expandedBounds);
    }

    @Test
    @Category(UnitTest.class)
    public void testUploadDeleteMissingWayTagValue() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds, 8);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);

        // Now delete some ways with an update to the changeset. One of the
        // ways being deleted does not have its tag value specified. The test should pass b/c
        // way tags are optional for a delete and not parsed.
        Document responseData = target("api/0.6/changeset/" + changesetId + "/upload")
            .queryParam("mapId", String.valueOf(mapId))
            .request(MediaType.TEXT_XML)
            .post(Entity.entity(
                "<osmChange version=\"0.3\" generator=\"iD\">" +
                    "<create/>" +
                    "<modify/>" +
                    "<delete if-unused=\"true\">" +
                        "<way id=\"" + wayIdsArr[0] + "\" version=\"1\" changeset=\"" + changesetId + "\">" + "</way>" +
                        "<way id=\"" + wayIdsArr[2] + "\" version=\"1\" changeset=\"" + changesetId + "\">" + "</way>" +
                    "</delete>" +
                "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);

        assertNotNull(responseData);

        XPath xpath = XmlUtils.createXPath();
        NodeList returnedWays = XPathAPI.selectNodeList(responseData, "//osm/diffResult/way");
        assertEquals(2, returnedWays.getLength());

        assertEquals((long) wayIdsArr[0],
                Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@old_id", responseData)));

        assertEquals((long) wayIdsArr[2],
                Long.parseLong(xpath.evaluate("//osm/diffResult/way[2]/@old_id", responseData)));

        Timestamp now = super.getCurrentDBTime();

        Map<Long, CurrentWays> ways = createQuery(mapId)
                .from(currentWays)
                .transform(groupBy(currentWays.id).as(currentWays));

        assertEquals(1, ways.size());
        assertNull(ways.get(wayIdsArr[0]));

        CurrentWays wayRecord = ways.get(wayIdsArr[1]);
        assertEquals(new Long(changesetId), wayRecord.getChangesetId());
        assertEquals(wayIdsArr[1], wayRecord.getId());
        assertTrue(wayRecord.getTimestamp().before(now));
        assertEquals(new Long(1), wayRecord.getVersion());
        assertTrue(wayRecord.getVisible());

        List<CurrentWayNodes> wayNodes = createQuery(mapId)
                .select(currentWayNodes)
                .from(currentWayNodes)
                .where(currentWayNodes.wayId.eq(wayIdsArr[1]))
                .orderBy(currentWayNodes.sequenceId.asc())
                .fetch();

        assertEquals(2, wayNodes.size());
        CurrentWayNodes wayNode = wayNodes.get(0);
        assertEquals(nodeIdsArr[2], wayNode.getNodeId());
        assertEquals(new Long(1), wayNode.getSequenceId());
        assertEquals(wayRecord.getId(), wayNode.getWayId());
        wayNode = wayNodes.get(1);
        assertEquals(nodeIdsArr[1], wayNode.getNodeId());
        assertEquals(new Long(2), wayNode.getSequenceId());
        assertEquals(wayRecord.getId(), wayNode.getWayId());
        assertTrue((wayRecord.getTags() == null)
                || PostgresUtils.postgresObjToHStore(wayRecord.getTags()).isEmpty());

        assertNull(ways.get(wayIdsArr[2]));

        Changesets changeset = createQuery(mapId)
                .select(changesets)
                .from(changesets)
                .where(changesets.id.eq(changesetId))
                .fetchOne();

        assertNotNull(changeset);
        assertTrue(changeset.getCreatedAt().before(now));
        assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));
        assertEquals(new Integer(10), changeset.getNumChanges());
        assertEquals(new Long(userId), changeset.getUserId());

        BoundingBox expandedBounds = new BoundingBox(originalBounds);
        expandedBounds.expand(originalBounds, Double.parseDouble(CHANGESET_BOUNDS_EXPANSION_FACTOR_DEEGREES));
        Changeset hootChangeset = new Changeset(mapId, changesetId);
        BoundingBox changesetBounds = hootChangeset.getBounds();
        assertEquals(changesetBounds, expandedBounds);
    }

    @Test
    @Category(UnitTest.class)
    public void testUploadDeleteMissingRelationTagValue() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // Now delete some relations with an update to the changeset. One of
        // the relations being deleted does not have its tag value specified. The test should
        // pass b/c tags are optional for a delete and not parsed.
        Document responseData = target("api/0.6/changeset/" + changesetId + "/upload")
            .queryParam("mapId", String.valueOf(mapId))
            .request(MediaType.TEXT_XML)
            .post(Entity.entity(
                "<osmChange version=\"0.3\" generator=\"iD\">" +
                    "<create/>" +
                    "<modify/>" +
                    "<delete if-unused=\"true\">" +
                        "<relation id=\"" + relationIdsArr[2] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                            "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                        "</relation>" +
                        "<relation id=\"" + relationIdsArr[3] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                            "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                "<tag k=\"name 1\"/>" + // missing tag value
                        "</relation>" +
                    "</delete>" +
                "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);

        assertNotNull(responseData);

        XPath xpath = XmlUtils.createXPath();
        NodeList returnedNodes = XPathAPI.selectNodeList(responseData, "//osm/diffResult/node");
        assertEquals(0, returnedNodes.getLength());

        NodeList returnedWays = XPathAPI.selectNodeList(responseData, "//osm/diffResult/way");
        assertEquals(0, returnedWays.getLength());

        NodeList returnedRelations = XPathAPI.selectNodeList(responseData, "//osm/diffResult/relation");
        assertEquals(2, returnedRelations.getLength());

        assertEquals((long) relationIdsArr[2],
                Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@old_id", responseData)));
        org.w3c.dom.Node xml = XPathAPI.selectSingleNode(responseData, "//osm/diffResult/relation[1]");
        assertNull(xml.getAttributes().getNamedItem("new_id"));
        assertNull(xml.getAttributes().getNamedItem("new_version"));
        assertEquals((long) relationIdsArr[3],
                Long.parseLong(xpath.evaluate("//osm/diffResult/relation[2]/@old_id", responseData)));
        xml = XPathAPI.selectSingleNode(responseData, "//osm/diffResult/relation[2]");
        assertNull(xml.getAttributes().getNamedItem("new_id"));
        assertNull(xml.getAttributes().getNamedItem("new_version"));

        OSMTestUtils.verifyTestNodesUnmodified(nodeIds, changesetId, originalBounds);
        OSMTestUtils.verifyTestWaysUnmodified(wayIds, nodeIds, changesetId);

        Timestamp now = super.getCurrentDBTime();
        Map<Long, CurrentRelations> relations = createQuery(mapId)
                .from(currentRelations)
                .transform(groupBy(currentRelations.id).as(currentRelations));

        assertEquals(2, relations.size());

        CurrentRelations relationRecord = relations.get(relationIdsArr[0]);
        assertEquals(new Long(changesetId), relationRecord.getChangesetId());
        assertEquals(relationIdsArr[0], relationRecord.getId());
        assertEquals(new Long(1), relationRecord.getVersion());

        List<CurrentRelationMembers> relationMembers =
                createQuery(mapId)
                        .select(currentRelationMembers)
                        .from(currentRelationMembers)
                        .where(currentRelationMembers.relationId.eq(relationIdsArr[0]))
                        .orderBy(currentRelationMembers.sequenceId.asc())
                        .fetch();

        assertEquals(4, relationMembers.size());
        CurrentRelationMembers member = relationMembers.get(0);
        assertEquals(relationRecord.getId(), member.getRelationId());
        assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
        assertEquals("role1", member.getMemberRole());
        assertEquals(new Integer(1), member.getSequenceId());

        assertEquals(nodeIdsArr[0], member.getMemberId());
        member = relationMembers.get(1);
        assertEquals(relationRecord.getId(), member.getRelationId());
        assertEquals(DbUtils.nwr_enum.way, member.getMemberType());
        assertEquals("role3", member.getMemberRole());
        assertEquals(new Integer(2), member.getSequenceId());

        assertEquals(wayIdsArr[1], member.getMemberId());
        member = relationMembers.get(2);
        assertEquals(relationRecord.getId(), member.getRelationId());
        assertEquals(DbUtils.nwr_enum.way, member.getMemberType());
        assertEquals("role2", member.getMemberRole());
        assertEquals(new Integer(3), member.getSequenceId());

        assertEquals(wayIdsArr[0], member.getMemberId());
        member = relationMembers.get(3);
        assertEquals(relationRecord.getId(), member.getRelationId());
        assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
        assertEquals("", member.getMemberRole());
        assertEquals(new Integer(4), member.getSequenceId());

        assertEquals(nodeIdsArr[2], member.getMemberId());
        Map<String, String> tags = PostgresUtils.postgresObjToHStore(relationRecord.getTags());
        assertNotNull(tags);
        assertEquals(1, tags.size());
        assertEquals("val 1", tags.get("key 1"));

        relationRecord = relations.get(relationIdsArr[1]);
        assertEquals(new Long(changesetId), relationRecord.getChangesetId());
        assertEquals(relationIdsArr[1], relationRecord.getId());
        assertEquals(new Long(1), relationRecord.getVersion());

        relationMembers = createQuery(mapId)
                .select(currentRelationMembers)
                .from(currentRelationMembers)
                .where(currentRelationMembers.relationId.eq(relationIdsArr[1]))
                .orderBy(currentRelationMembers.sequenceId.asc())
                .fetch();

        assertEquals(2, relationMembers.size());
        member = relationMembers.get(0);
        assertEquals(relationRecord.getId(), member.getRelationId());
        assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
        assertEquals("role1", member.getMemberRole());
        assertEquals(new Integer(1), member.getSequenceId());

        assertEquals(nodeIdsArr[4], member.getMemberId());
        member = relationMembers.get(1);
        assertEquals(relationRecord.getId(), member.getRelationId());
        assertEquals(DbUtils.nwr_enum.relation, member.getMemberType());
        assertEquals("role1", member.getMemberRole());
        assertEquals(new Integer(2), member.getSequenceId());

        assertEquals(relationIdsArr[0], member.getMemberId());
        tags = PostgresUtils.postgresObjToHStore(relationRecord.getTags());
        assertNotNull(tags);
        assertEquals(2, tags.size());
        assertEquals("val 2", tags.get("key 2"));
        assertEquals("val 3", tags.get("key 3"));
        assertNull(relations.get(relationIdsArr[2]));
        assertNull(relations.get(relationIdsArr[3]));

        Changesets changeset = createQuery(mapId)
                .select(changesets)
                .from(changesets)
                .where(changesets.id.eq(changesetId))
                .fetchOne();

        assertNotNull(changeset);
        assertTrue(changeset.getCreatedAt().before(now));
        assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));
        assertEquals(new Integer(14), changeset.getNumChanges());
        assertEquals(new Long(userId), changeset.getUserId());

        BoundingBox expandedBounds = new BoundingBox(originalBounds);
        expandedBounds.expand(originalBounds, Double.parseDouble(CHANGESET_BOUNDS_EXPANSION_FACTOR_DEEGREES));
        Changeset hootChangeset = new Changeset(mapId, changesetId);
        BoundingBox changesetBounds = hootChangeset.getBounds();
        assertEquals(changesetBounds, expandedBounds);
    }

    @Test(expected = ClientErrorException.class)
    @Category(UnitTest.class)
    public void testUploadDeleteWayInvalidVersion() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Send a changeset delete request where one of the ways has a version
        // not equal to what's on the server. A failure should occur and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create/>" +
                        "<modify/>" + "<delete if-unused=\"true\">" +
                            "<way id=\"" + wayIdsArr[0] + "\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                            "</way>" +
                            "<way id=\"" + wayIdsArr[1] + "\" version=\"2\" changeset=\"" + changesetId + "\" >" +
                                "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" + "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                            "</way>" +
                        "</delete>" +
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
    public void testUploadDeleteRelationInvalidVersion() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // Send a changeset delete request where one of the relations has a
        // version not equal to what's on the server. A failure should occur and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create/>" +
                        "<modify/>" +
                        "<delete if-unused=\"true\">" +
                            "<relation id=\"" + relationIdsArr[0] + "\" version=\"0\" changeset=\"" +
                                changesetId + "\" >" +
                            "</relation>" +
                            "<relation id=\"" + relationIdsArr[1] + "\" version=\"2\" changeset=\"" +
                                changesetId + "\" >" +
                            "</relation>" +
                        "</delete>" +
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

    @Test
    @Category(UnitTest.class)
    public void testUploadDeleteNodeStillBelongsToWayWithIfUnused() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Try to delete some elements with the "if-unused" tag is passed
        // where one of them is a node that still belongs to a way. No failure should occur, the node
        // should not be deleted, and the other element should be deleted.
        Document responseData = target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create/>" +
                        "<modify/>" +
                        "<delete if-unused=\"true\">" +
                            "<node id=\"" + nodeIdsArr[0] + "\" lon=\"" + originalBounds.getMinLon() +
                                "\" lat=\"" + originalBounds.getMinLat() + "\" version=\"1\" " +
                                "changeset=\"" + changesetId + "\">" +
                            "</node>" +
                            "<way id=\"" + wayIdsArr[2] + "\" version=\"1\" " + "changeset=\"" + changesetId + "\">" + "</way>" +
                        "</delete>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);

        assertNotNull(responseData);

        XPath xpath = XmlUtils.createXPath();
        NodeList returnedNodes = XPathAPI.selectNodeList(responseData, "//osm/diffResult/node");

        assertEquals(0, returnedNodes.getLength());

        NodeList returnedWays = XPathAPI.selectNodeList(responseData, "//osm/diffResult/way");

        assertEquals(1, returnedWays.getLength());

        assertEquals((long) wayIdsArr[2],
                Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@old_id", responseData)));

        org.w3c.dom.Node xml = XPathAPI.selectSingleNode(responseData, "//osm/diffResult/way[1]");

        assertNull(xml.getAttributes().getNamedItem("new_id"));
        assertNull(xml.getAttributes().getNamedItem("new_version"));

        Timestamp now = super.getCurrentDBTime();

        Map<Long, CurrentNodes> nodes = createQuery(mapId)
                .from(currentNodes)
                .transform(groupBy(currentNodes.id).as(currentNodes));

        assertEquals(5, nodes.size());

        CurrentNodes nodeRecord = nodes.get(nodeIdsArr[0]);

        assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
        assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
        assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
        assertEquals(nodeIdsArr[0], nodeRecord.getId());
        assertEquals(
                new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                nodeRecord.getTile());
        assertTrue(nodeRecord.getTimestamp().before(now));
        assertEquals(new Long(1), nodeRecord.getVersion());
        assertTrue(nodeRecord.getVisible());

        Map<String, String> tags = PostgresUtils.postgresObjToHStore(nodeRecord.getTags());

        assertNotNull(tags);
        assertEquals(2, tags.size());
        assertEquals("val 1", tags.get("key 1"));
        assertEquals("val 2", tags.get("key 2"));

        nodeRecord = nodes.get(nodeIdsArr[1]);

        assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
        assertEquals(new Double(originalBounds.getMaxLat()), nodeRecord.getLatitude());
        assertEquals(new Double(originalBounds.getMaxLon()), nodeRecord.getLongitude());
        assertEquals(nodeIdsArr[1], nodeRecord.getId());
        assertEquals(
                new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                nodeRecord.getTile());
        assertTrue(nodeRecord.getTimestamp().before(now));
        assertEquals(new Long(1), nodeRecord.getVersion());
        assertTrue(nodeRecord.getVisible());
        assertTrue((nodeRecord.getTags() == null)
                || PostgresUtils.postgresObjToHStore(nodeRecord.getTags()).isEmpty());

        nodeRecord = nodes.get(nodeIdsArr[2]);

        assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
        assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
        assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
        assertEquals(nodeIdsArr[2], nodeRecord.getId());
        assertEquals(
                new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                nodeRecord.getTile());
        assertTrue(nodeRecord.getTimestamp().before(now));
        assertEquals(new Long(1), nodeRecord.getVersion());
        assertTrue(nodeRecord.getVisible());
        assertTrue((nodeRecord.getTags() == null)
                || PostgresUtils.postgresObjToHStore(nodeRecord.getTags()).isEmpty());

        nodeRecord = nodes.get(nodeIdsArr[3]);

        assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
        assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
        assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
        assertEquals(nodeIdsArr[3], nodeRecord.getId());
        assertEquals(
                new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                nodeRecord.getTile());
        assertTrue(nodeRecord.getTimestamp().before(now));
        assertEquals(new Long(1), nodeRecord.getVersion());
        assertTrue(nodeRecord.getVisible());

        tags = PostgresUtils.postgresObjToHStore(nodeRecord.getTags());

        assertNotNull(tags);
        assertEquals(1, tags.size());
        assertEquals("val 3", tags.get("key 3"));

        nodeRecord = nodes.get(nodeIdsArr[4]);
        assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
        assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
        assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
        assertEquals(nodeIdsArr[4], nodeRecord.getId());
        assertEquals(
                new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                nodeRecord.getTile());
        assertTrue(nodeRecord.getTimestamp().before(now));
        assertEquals(new Long(1), nodeRecord.getVersion());
        assertTrue(nodeRecord.getVisible());

        tags = PostgresUtils.postgresObjToHStore(nodeRecord.getTags());

        assertNotNull(tags);
        assertEquals(1, tags.size());
        assertEquals("val 4", tags.get("key 4"));

            Map<Long, CurrentWays> ways = createQuery(mapId)
                    .from(currentWays)
                    .transform(groupBy(currentWays.id).as(currentWays));

            assertEquals(2, ways.size());

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

            assertEquals(3, wayNodes.size());

            CurrentWayNodes wayNode = wayNodes.get(0);

            assertEquals(nodeIdsArr[0], wayNode.getNodeId());
            assertEquals(new Long(1), wayNode.getSequenceId());
            assertEquals(wayRecord.getId(), wayNode.getWayId());

            wayNode = wayNodes.get(1);

            assertEquals(nodeIdsArr[1], wayNode.getNodeId());
            assertEquals(new Long(2), wayNode.getSequenceId());
            assertEquals(wayRecord.getId(), wayNode.getWayId());

            wayNode = wayNodes.get(2);

            assertEquals(nodeIdsArr[4], wayNode.getNodeId());
            assertEquals(new Long(3), wayNode.getSequenceId());
            assertEquals(wayRecord.getId(), wayNode.getWayId());

            // verify the previously existing tags
            tags = PostgresUtils.postgresObjToHStore(wayRecord.getTags());

            assertNotNull(tags);
            assertEquals(2, tags.size());
            assertEquals("val 1", tags.get("key 1"));
            assertEquals("val 2", tags.get("key 2"));

            wayRecord = ways.get(wayIdsArr[1]);

            assertEquals(new Long(changesetId), wayRecord.getChangesetId());
            assertEquals(wayIdsArr[1], wayRecord.getId());
            assertTrue(wayRecord.getTimestamp().before(now));
            assertEquals(new Long(1), wayRecord.getVersion());
            assertTrue(wayRecord.getVisible());

            wayNodes = createQuery(mapId)
                    .select(currentWayNodes)
                    .from(currentWayNodes)
                    .where(currentWayNodes.wayId.eq(wayIdsArr[1]))
                    .orderBy(currentWayNodes.sequenceId.asc())
                    .fetch();

            assertEquals(2, wayNodes.size());

            wayNode = wayNodes.get(0);

            assertEquals(nodeIdsArr[2], wayNode.getNodeId());
            assertEquals(new Long(1), wayNode.getSequenceId());
            assertEquals(wayRecord.getId(), wayNode.getWayId());

            wayNode = wayNodes.get(1);

            assertEquals(nodeIdsArr[1], wayNode.getNodeId());
            assertEquals(new Long(2), wayNode.getSequenceId());
            assertEquals(wayRecord.getId(), wayNode.getWayId());

            // verify the way with no tags
            assertTrue((wayRecord.getTags() == null)
                    || PostgresUtils.postgresObjToHStore(wayRecord.getTags()).isEmpty());

            // verify the deleted way
            assertNull(createQuery(mapId)
                    .select(currentWays)
                    .from(currentWays)
                    .where(currentWays.id.eq(wayIdsArr[2]))
                    .fetchOne());

        OSMTestUtils.verifyTestRelationsUnmodified(relationIds, wayIds, nodeIds, changesetId);

        Changesets changeset = createQuery(mapId)
                .select(changesets)
                .from(changesets)
                .where(changesets.id.eq(changesetId))
                .fetchOne();

        assertNotNull(changeset);
        assertTrue(changeset.getCreatedAt().before(now));
        assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));
        assertEquals(new Integer(13), changeset.getNumChanges());
        assertEquals(new Long(userId), changeset.getUserId());

        BoundingBox expandedBounds = new BoundingBox(originalBounds);
        expandedBounds.expand(originalBounds, Double.parseDouble(CHANGESET_BOUNDS_EXPANSION_FACTOR_DEEGREES));
        Changeset hootChangeset = new Changeset(mapId, changesetId);
        BoundingBox changesetBounds = hootChangeset.getBounds();

        assertEquals(changesetBounds, expandedBounds);
    }

    @Test
    @Category(UnitTest.class)
    public void testUploadDeleteNodeStillBelongsToRelationWithIfUnused() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> relationIds = OSMTestUtils.createTestRelationsNoWays(changesetId, nodeIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // Try to delete some elements with the "if-unused" tag is passed
        // where one of them is a node that still belongs to a relation. No failure should occur, the
        // node should not be deleted, and the other element should be deleted.
        Document responseData = target("api/0.6/changeset/" + changesetId + "/upload")
            .queryParam("mapId", String.valueOf(mapId))
            .request(MediaType.TEXT_XML)
            .post(Entity.entity(
                "<osmChange version=\"0.3\" generator=\"iD\">" +
                    "<create/>" +
                    "<modify/>" +
                    "<delete if-unused=\"true\">" +
                        "<node id=\"" + nodeIdsArr[3] + "\" lon=\"" + originalBounds.getMinLon() +
                            "\" lat=\"" + originalBounds.getMinLat() + "\" version=\"1\" " + "changeset=\"" +
                            changesetId + "\">" +
                        "</node>" +
                        "<node id=\"" + nodeIdsArr[4] + "\" lon=\"" + originalBounds.getMinLon() +
                            "\" lat=\"" + originalBounds.getMinLat() + "\" version=\"1\" " + "changeset=\"" +
                            changesetId + "\">" +
                        "</node>" +
                    "</delete>" +
                "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);

        assertNotNull(responseData);

        XPath xpath = XmlUtils.createXPath();
        NodeList returnedNodes = XPathAPI.selectNodeList(responseData, "//osm/diffResult/node");

        assertEquals(1, returnedNodes.getLength());
        assertEquals((long) nodeIdsArr[3],
                Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@old_id", responseData)));
        org.w3c.dom.Node xml = XPathAPI.selectSingleNode(responseData, "//osm/diffResult/node[1]");

        assertNull(xml.getAttributes().getNamedItem("new_id"));
        assertNull(xml.getAttributes().getNamedItem("new_version"));

        NodeList returnedWays = XPathAPI.selectNodeList(responseData, "//osm/diffResult/way");
        assertEquals(0, returnedWays.getLength());

        NodeList returnedRelations = XPathAPI.selectNodeList(responseData, "//osm/diffResult/relation");
        assertEquals(0, returnedRelations.getLength());

        Timestamp now = super.getCurrentDBTime();

        Map<Long, CurrentNodes> nodes = createQuery(mapId)
                .from(currentNodes)
                .transform(groupBy(currentNodes.id).as(currentNodes));

        assertEquals(4, nodes.size());

        CurrentNodes nodeRecord = nodes.get(nodeIdsArr[0]);
        assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
        assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
        assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
        assertEquals(nodeIdsArr[0], nodeRecord.getId());
        assertEquals(
                new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                nodeRecord.getTile());
        assertTrue(nodeRecord.getTimestamp().before(now));
        assertEquals(new Long(1), nodeRecord.getVersion());
        assertTrue(nodeRecord.getVisible());

        Map<String, String> tags = PostgresUtils.postgresObjToHStore(nodeRecord.getTags());

        assertNotNull(tags);
        assertEquals(2, tags.size());
        assertEquals("val 1", tags.get("key 1"));
        assertEquals("val 2", tags.get("key 2"));

        nodeRecord = nodes.get(nodeIdsArr[1]);

        assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
        assertEquals(new Double(originalBounds.getMaxLat()), nodeRecord.getLatitude());
        assertEquals(new Double(originalBounds.getMaxLon()), nodeRecord.getLongitude());
        assertEquals(nodeIdsArr[1], nodeRecord.getId());
        assertEquals(
                new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                nodeRecord.getTile());
        assertTrue(nodeRecord.getTimestamp().before(now));
        assertEquals(new Long(1), nodeRecord.getVersion());
        assertTrue(nodeRecord.getVisible());
        assertTrue((nodeRecord.getTags() == null)
                || PostgresUtils.postgresObjToHStore(nodeRecord.getTags()).isEmpty());

        nodeRecord = nodes.get(nodeIdsArr[2]);

        assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
        assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
        assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
        assertEquals(nodeIdsArr[2], nodeRecord.getId());
        assertEquals(
                new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                nodeRecord.getTile());
        assertTrue(nodeRecord.getTimestamp().before(now));
        assertEquals(new Long(1), nodeRecord.getVersion());
        assertTrue(nodeRecord.getVisible());
        assertTrue((nodeRecord.getTags() == null)
                || PostgresUtils.postgresObjToHStore(nodeRecord.getTags()).isEmpty());

        assertNull(nodes.get(nodeIdsArr[3]));

        nodeRecord = nodes.get(nodeIdsArr[4]);

        assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
        assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
        assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
        assertEquals(nodeIdsArr[4], nodeRecord.getId());
        assertEquals(
                new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                nodeRecord.getTile());
        assertTrue(nodeRecord.getTimestamp().before(now));
        assertEquals(new Long(1), nodeRecord.getVersion());
        assertTrue(nodeRecord.getVisible());

        tags = PostgresUtils.postgresObjToHStore(nodeRecord.getTags());

        assertNotNull(tags);
        assertEquals(1, tags.size());
        assertEquals("val 4", tags.get("key 4"));

        List<CurrentRelations> relations = createQuery(mapId)
                .select(currentRelations)
                .from(currentRelations)
                .fetch();

        assertEquals(3, relations.size());

        CurrentRelations relationRecord = relations.get(0);

        assertEquals(new Long(changesetId), relationRecord.getChangesetId());
        assertEquals(relationIdsArr[0], relationRecord.getId());
        assertTrue(relationRecord.getTimestamp().before(now));
        assertEquals(new Long(1), relationRecord.getVersion());
        assertTrue(relationRecord.getVisible());

        List<CurrentRelationMembers> members = createQuery(mapId)
                .select(currentRelationMembers)
                .from(currentRelationMembers)
                .where(currentRelationMembers.relationId.eq(relationIdsArr[0]))
                .orderBy(currentRelationMembers.sequenceId.asc())
                .fetch();

        assertEquals(2, members.size());

        CurrentRelationMembers member = members.get(0);

        assertEquals(relationRecord.getId(), member.getRelationId());
        assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
        assertEquals("role1", member.getMemberRole());
        assertEquals(new Integer(1), member.getSequenceId());
        assertEquals(nodeIdsArr[0], member.getMemberId());

        member = members.get(1);

        assertEquals(relationRecord.getId(), member.getRelationId());
        assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
        assertEquals("", member.getMemberRole());
        assertEquals(new Integer(2), member.getSequenceId());

        assertEquals(nodeIdsArr[2], member.getMemberId());

        relationRecord = relations.get(1);

        assertEquals(new Long(changesetId), relationRecord.getChangesetId());
        assertEquals(relationIdsArr[1], relationRecord.getId());
        assertTrue(relationRecord.getTimestamp().before(now));
        assertEquals(new Long(1), relationRecord.getVersion());
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
        assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
        assertEquals("role1", member.getMemberRole());
        assertEquals(new Integer(1), member.getSequenceId());

        assertEquals(nodeIdsArr[4], member.getMemberId());

        member = members.get(1);

        assertEquals(relationRecord.getId(), member.getRelationId());
        assertEquals(DbUtils.nwr_enum.relation, member.getMemberType());
        assertEquals("role1", member.getMemberRole());
        assertEquals(new Integer(2), member.getSequenceId());
        assertEquals(relationIdsArr[0], member.getMemberId());

        relationRecord = relations.get(2);

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
        assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
        assertEquals("role1", member.getMemberRole());
        assertEquals(new Integer(1), member.getSequenceId());
        assertEquals(nodeIdsArr[2], member.getMemberId());

        OSMTestUtils.verifyTestRelationsNoWaysUnmodified(relationIds, nodeIds, changesetId, true);

        Changesets changeset = createQuery(mapId)
                .select(changesets)
                .from(changesets)
                .where(changesets.id.eq(changesetId))
                .fetchOne();

        assertNotNull(changeset);
        assertTrue(changeset.getCreatedAt().before(now));
        assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));
        assertEquals(new Integer(13), changeset.getNumChanges());
        assertEquals(new Long(userId), changeset.getUserId());

        BoundingBox expandedBounds = new BoundingBox(originalBounds);
        expandedBounds.expand(originalBounds, Double.parseDouble(CHANGESET_BOUNDS_EXPANSION_FACTOR_DEEGREES));
        Changeset hootChangeset = new Changeset(mapId, changesetId);
        BoundingBox changesetBounds = hootChangeset.getBounds();

        assertEquals(changesetBounds, expandedBounds);
    }

    @Test
    @Category(UnitTest.class)
    public void testUploadDeleteWayStillBelongsToRelationWithIfUnused() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Try to delete some elements with the "if-unused" tag is passed
        // where one of them is a way that still belongs to a relation. No failure should occur, the
        // way should not be deleted, and the other element should be deleted.
        Document responseData = target("api/0.6/changeset/" + changesetId + "/upload")
            .queryParam("mapId", String.valueOf(mapId))
            .request(MediaType.TEXT_XML)
            .post(Entity.entity(
                "<osmChange version=\"0.3\" generator=\"iD\">" +
                    "<create/>" +
                    "<modify/>" +
                    "<delete if-unused=\"true\">" +
                        "<node id=\"" + nodeIdsArr[3] +
                            "\" lon=\"" + originalBounds.getMinLon() + "\" lat=\"" + originalBounds.getMinLat() +
                            "\" version=\"1\" " + "changeset=\"" + changesetId + "\">" +
                        "</node>" +
                        "<way id=\"" + wayIdsArr[1] + "\" version=\"1\" " + "changeset=\"" + changesetId + "\">" + "</way>" +
                    "</delete>" +
                "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);

        assertNotNull(responseData);

        XPath xpath = XmlUtils.createXPath();
        NodeList returnedNodes = XPathAPI.selectNodeList(responseData, "//osm/diffResult/node");
        assertEquals(1, returnedNodes.getLength());

        assertEquals((long) nodeIdsArr[3],
                Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@old_id", responseData)));

        org.w3c.dom.Node xml = XPathAPI.selectSingleNode(responseData, "//osm/diffResult/node[1]");

        assertNull(xml.getAttributes().getNamedItem("new_id"));
        assertNull(xml.getAttributes().getNamedItem("new_version"));

        NodeList returnedWays = XPathAPI.selectNodeList(responseData, "//osm/diffResult/way");

        assertEquals(0, returnedWays.getLength());

        Timestamp now = super.getCurrentDBTime();

        Map<Long, CurrentNodes> nodes = createQuery(mapId)
                .from(currentNodes)
                .transform(groupBy(currentNodes.id).as(currentNodes));

        assertEquals(4, nodes.size());

        CurrentNodes nodeRecord = nodes.get(nodeIdsArr[0]);

        assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
        assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
        assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
        assertEquals(nodeIdsArr[0], nodeRecord.getId());
        assertEquals(
                new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                nodeRecord.getTile());
        assertTrue(nodeRecord.getTimestamp().before(now));
        assertEquals(new Long(1), nodeRecord.getVersion());
        assertTrue(nodeRecord.getVisible());

        Map<String, String> tags = PostgresUtils.postgresObjToHStore(nodeRecord.getTags());

        assertNotNull(tags);
        assertEquals(2, tags.size());
        assertEquals("val 1", tags.get("key 1"));
        assertEquals("val 2", tags.get("key 2"));

        nodeRecord = nodes.get(nodeIdsArr[1]);
        assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
        assertEquals(new Double(originalBounds.getMaxLat()), nodeRecord.getLatitude());
        assertEquals(new Double(originalBounds.getMaxLon()), nodeRecord.getLongitude());
        assertEquals(nodeIdsArr[1], nodeRecord.getId());
        assertEquals(
                new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                nodeRecord.getTile());
        assertTrue(nodeRecord.getTimestamp().before(now));
        assertEquals(new Long(1), nodeRecord.getVersion());
        assertTrue(nodeRecord.getVisible());
        assertTrue((nodeRecord.getTags() == null)
                || PostgresUtils.postgresObjToHStore(nodeRecord.getTags()).isEmpty());

        nodeRecord = nodes.get(nodeIdsArr[2]);
        assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
        assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
        assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
        assertEquals(nodeIdsArr[2], nodeRecord.getId());
        assertEquals(
                new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                nodeRecord.getTile());
        assertTrue(nodeRecord.getTimestamp().before(now));
        assertEquals(new Long(1), nodeRecord.getVersion());
        assertTrue(nodeRecord.getVisible());
        assertTrue((nodeRecord.getTags() == null)
                || PostgresUtils.postgresObjToHStore(nodeRecord.getTags()).isEmpty());

        assertNull(nodes.get(nodeIdsArr[3]));

        nodeRecord = nodes.get(nodeIdsArr[4]);

        assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
        assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
        assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
        assertEquals(nodeIdsArr[4], nodeRecord.getId());
        assertEquals(
                new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                nodeRecord.getTile());
        assertTrue(nodeRecord.getTimestamp().before(now));
        assertEquals(new Long(1), nodeRecord.getVersion());
        assertTrue(nodeRecord.getVisible());

        tags = PostgresUtils.postgresObjToHStore(nodeRecord.getTags());

        assertNotNull(tags);
        assertEquals(1, tags.size());
        assertEquals("val 4", tags.get("key 4"));

        OSMTestUtils.verifyTestWaysUnmodified(wayIds, nodeIds, changesetId);
        OSMTestUtils.verifyTestRelationsUnmodified(relationIds, wayIds, nodeIds, changesetId);

        Changesets changeset = createQuery(mapId)
                .select(changesets)
                .from(changesets)
                .where(changesets.id.eq(changesetId))
                .fetchOne();

        assertNotNull(changeset);
        assertTrue(changeset.getCreatedAt().before(now));
        assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));
        assertEquals(new Integer(13), changeset.getNumChanges());
        assertEquals(new Long(userId), changeset.getUserId());

        BoundingBox expandedBounds = new BoundingBox(originalBounds);
        expandedBounds.expand(originalBounds, Double.parseDouble(CHANGESET_BOUNDS_EXPANSION_FACTOR_DEEGREES));
        Changeset hootChangeset = new Changeset(mapId, changesetId);
        BoundingBox changesetBounds = hootChangeset.getBounds();

        assertEquals(changesetBounds, expandedBounds);
    }

    @Test
    @Category(UnitTest.class)
    public void testUploadDeleteRelationStillBelongsToRelationWithIfUnused() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // Try to delete some elements with the "if-unused" tag is passed
        // where one of them is a relation that still belongs to a relation. No failure should
        // occur, the relation should not be deleted, and the other element should be deleted.
        Document responseData = target("api/0.6/changeset/" + changesetId + "/upload")
            .queryParam("mapId", String.valueOf(mapId))
            .request(MediaType.TEXT_XML)
            .post(Entity.entity(
                "<osmChange version=\"0.3\" generator=\"iD\">" +
                    "<create/>" +
                    "<modify/>" +
                    "<delete if-unused=\"true\">" +
                        "<relation id=\"" + relationIdsArr[0] +
                            "\" version=\"1\" " + "changeset=\"" + changesetId + "\">" +
                        "</relation>" +
                        "<relation id=\"" + relationIdsArr[2] + "\" version=\"1\" " + "changeset=\"" +
                            changesetId + "\">" +
                        "</relation>" +
                    "</delete>" +
                "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);

        assertNotNull(responseData);

        XPath xpath = XmlUtils.createXPath();
        NodeList returnedNodes = XPathAPI.selectNodeList(responseData, "//osm/diffResult/node");
        assertEquals(0, returnedNodes.getLength());

        NodeList returnedWays = XPathAPI.selectNodeList(responseData, "//osm/diffResult/way");
        assertEquals(0, returnedWays.getLength());

        NodeList returnedRelations = XPathAPI.selectNodeList(responseData, "//osm/diffResult/relation");

        assertEquals(1, returnedRelations.getLength());
        assertEquals((long) relationIdsArr[2],
                Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@old_id", responseData)));
        org.w3c.dom.Node xml = XPathAPI.selectSingleNode(responseData, "//osm/diffResult/relation[1]");

        assertNull(xml.getAttributes().getNamedItem("new_id"));
        assertNull(xml.getAttributes().getNamedItem("new_version"));

        OSMTestUtils.verifyTestNodesUnmodified(nodeIds, changesetId, originalBounds);
        OSMTestUtils.verifyTestWaysUnmodified(wayIds, nodeIds, changesetId);

        Timestamp now = super.getCurrentDBTime();
        Map<Long, CurrentRelations> relations = createQuery(mapId)
                .from(currentRelations)
                .transform(groupBy(currentRelations.id).as(currentRelations));

        assertEquals(3, relations.size());

        // verify the previously existing relations
        CurrentRelations relationRecord = relations.get(relationIdsArr[0]);

        assertEquals(new Long(changesetId), relationRecord.getChangesetId());
        assertEquals(relationIdsArr[0], relationRecord.getId());
        assertTrue(relationRecord.getTimestamp().before(now));
        assertEquals(new Long(1), relationRecord.getVersion());
        assertTrue(relationRecord.getVisible());

        List<CurrentRelationMembers> members = createQuery(mapId)
                .select(currentRelationMembers)
                .from(currentRelationMembers)
                .where(currentRelationMembers.relationId.eq(relationIdsArr[0]))
                .orderBy(currentRelationMembers.sequenceId.asc())
                .fetch();

        assertEquals(4, members.size());

        CurrentRelationMembers member = members.get(0);

        assertEquals(relationRecord.getId(), member.getRelationId());
        assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
        assertEquals("role1", member.getMemberRole());
        assertEquals(new Integer(1), member.getSequenceId());
        assertEquals(nodeIdsArr[0], member.getMemberId());

        member = members.get(1);

        assertEquals(relationRecord.getId(), member.getRelationId());
        assertEquals(DbUtils.nwr_enum.way, member.getMemberType());
        assertEquals("role3", member.getMemberRole());
        assertEquals(new Integer(2), member.getSequenceId());
        assertEquals(wayIdsArr[1], member.getMemberId());

        member = members.get(2);

        assertEquals(relationRecord.getId(), member.getRelationId());
        assertEquals(DbUtils.nwr_enum.way, member.getMemberType());
        assertEquals("role2", member.getMemberRole());
        assertEquals(new Integer(3), member.getSequenceId());
        assertEquals(wayIdsArr[0], member.getMemberId());

        member = members.get(3);

        assertEquals(relationRecord.getId(), member.getRelationId());
        assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
        assertEquals("", member.getMemberRole());
        assertEquals(new Integer(4), member.getSequenceId());
        assertEquals(nodeIdsArr[2], member.getMemberId());

        Map<String, String> tags = PostgresUtils.postgresObjToHStore(relationRecord.getTags());

        assertNotNull(tags);
        assertEquals(1, tags.size());
        assertEquals("val 1", tags.get("key 1"));

        relationRecord = relations.get(relationIdsArr[1]);

        assertEquals(new Long(changesetId), relationRecord.getChangesetId());
        assertEquals(relationIdsArr[1], relationRecord.getId());
        assertTrue(relationRecord.getTimestamp().before(now));
        assertEquals(new Long(1), relationRecord.getVersion());
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
        assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
        assertEquals("role1", member.getMemberRole());
        assertEquals(new Integer(1), member.getSequenceId());
        assertEquals(nodeIdsArr[4], member.getMemberId());

        member = members.get(1);

        assertEquals(relationRecord.getId(), member.getRelationId());
        assertEquals(DbUtils.nwr_enum.relation, member.getMemberType());
        assertEquals("role1", member.getMemberRole());
        assertEquals(new Integer(2), member.getSequenceId());
        assertEquals(relationIdsArr[0], member.getMemberId());

        tags = PostgresUtils.postgresObjToHStore(relationRecord.getTags());

        assertNotNull(tags);
        assertEquals(2, tags.size());
        assertEquals("val 2", tags.get("key 2"));
        assertEquals("val 3", tags.get("key 3"));
        assertNull(relations.get(relationIdsArr[2]));

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

        Changesets changeset = createQuery(mapId)
                .select(changesets)
                .from(changesets)
                .where(changesets.id.eq(changesetId))
                .fetchOne();

        assertNotNull(changeset);
        assertTrue(changeset.getCreatedAt().before(now));
        assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));

        assertEquals(new Integer(13), changeset.getNumChanges());
        assertEquals(new Long(userId), changeset.getUserId());

        BoundingBox expandedBounds = new BoundingBox(originalBounds);
        expandedBounds.expand(originalBounds, Double.parseDouble(CHANGESET_BOUNDS_EXPANSION_FACTOR_DEEGREES));
        Changeset hootChangeset = new Changeset(mapId, changesetId);
        BoundingBox changesetBounds = hootChangeset.getBounds();
        assertEquals(changesetBounds, expandedBounds);
    }

    @Test(expected = ClientErrorException.class)
    @Category(UnitTest.class)
    public void testUploadDeleteNodeStillBelongsToWayWithoutIfUnused() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Try to delete a node that still belongs to a way when the "if-unused"
        // tag is not passed. A failure should occur and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create/>" +
                        "<modify/>" +
                        "<delete>" +
                            "<node id=\"" + nodeIdsArr[1] + "\" lon=\"" + originalBounds.getMinLon() + "\" " +
                                 "lat=\"" + originalBounds.getMinLat() + "\" version=\"1\" changeset=\"" + changesetId + "\">" +
                            "</node>" +
                        "</delete>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (ClientErrorException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.PRECONDITION_FAILED, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("still used by other way(s)"));
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test(expected = ClientErrorException.class)
    @Category(UnitTest.class)
    public void testUploadDeleteNodeStillBelongsToRelationWithoutIfUnused() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> relationIds = OSMTestUtils.createTestRelationsNoWays(changesetId, nodeIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // Try to delete a node that still belongs to a relation when the
        // "if-unused" tag is not passed.  A failure should occur and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create/>" +
                        "<modify/>" +
                        "<delete>" +
                            "<node id=\"" + nodeIdsArr[0] + "\" lon=\"" + originalBounds.getMinLon() + "\" " +
                                "lat=\"" + originalBounds.getMinLat() + "\" version=\"1\" changeset=\"" + changesetId + "\">" +
                            "</node>" +
                        "</delete>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (ClientErrorException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.PRECONDITION_FAILED, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("still used by other relation(s)"));
            OSMTestUtils.verifyTestChangesetUnmodified(changesetId, originalBounds);
            OSMTestUtils.verifyTestNodesUnmodified(nodeIds, changesetId, originalBounds);

            List<CurrentRelations> relations = createQuery(mapId)
                    .select(currentRelations)
                    .from(currentRelations)
                    .orderBy(currentRelations.id.asc())
                    .fetch();

            assertEquals(3, relations.size());

            CurrentRelations relationRecord = relations.get(0);
            assertEquals(new Long(changesetId), relationRecord.getChangesetId());
            assertEquals(relationIdsArr[0], relationRecord.getId());

            Timestamp now = super.getCurrentDBTime();

            assertTrue(relationRecord.getTimestamp().before(now));
            assertEquals(new Long(1), relationRecord.getVersion());
            assertTrue(relationRecord.getVisible());

            List<CurrentRelationMembers> members =
                    createQuery(mapId)
                            .select(currentRelationMembers)
                            .from(currentRelationMembers)
                            .where(currentRelationMembers.relationId.eq(relationIdsArr[0]))
                            .orderBy(currentRelationMembers.sequenceId.asc())
                            .fetch();

            assertEquals(2, members.size());

            CurrentRelationMembers member = members.get(0);

            assertEquals(relationRecord.getId(), member.getRelationId());
            assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
            assertEquals("role1", member.getMemberRole());
            assertEquals(new Integer(1), member.getSequenceId());

            assertEquals(nodeIdsArr[0], member.getMemberId());

            member = members.get(1);

            assertEquals(relationRecord.getId(), member.getRelationId());
            assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
            assertEquals("", member.getMemberRole());
            assertEquals(new Integer(2), member.getSequenceId());
            assertEquals(nodeIdsArr[2], member.getMemberId());

            relationRecord = relations.get(1);

            assertEquals(new Long(changesetId), relationRecord.getChangesetId());
            assertEquals(relationIdsArr[1], relationRecord.getId());

            assertTrue(relationRecord.getTimestamp().before(now));
            assertEquals(new Long(1), relationRecord.getVersion());
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
            assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
            assertEquals("role1", member.getMemberRole());
            assertEquals(new Integer(1), member.getSequenceId());
            assertEquals(nodeIdsArr[4], member.getMemberId());

            member = members.get(1);

            assertEquals(relationRecord.getId(), member.getRelationId());
            assertEquals(DbUtils.nwr_enum.relation, member.getMemberType());
            assertEquals("role1", member.getMemberRole());
            assertEquals(new Integer(2), member.getSequenceId());
            assertEquals(relationIdsArr[0], member.getMemberId());

            relationRecord = relations.get(2);

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
            assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
            assertEquals("role1", member.getMemberRole());
            assertEquals(new Integer(1), member.getSequenceId());
            assertEquals(nodeIdsArr[2], member.getMemberId());

            OSMTestUtils.verifyTestRelationsNoWaysUnmodified(relationIds, nodeIds, changesetId, true);

            throw e;
        }
    }

    @Test(expected = ClientErrorException.class)
    @Category(UnitTest.class)
    public void testUploadDeleteWayStillBelongsToRelationWithoutIfUnused() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Try to delete a way that still belongs to a relation when the
        // "if-unused" tag is not passed. A failure should occur and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create/>" +
                        "<modify/>" +
                        "<delete>" +
                            "<way id=\"" + wayIdsArr[1] + "\" version=\"1\" changeset=\"" + changesetId + "\">" + "</way>" +
                        "</delete>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (ClientErrorException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.PRECONDITION_FAILED, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("still used by other relation(s)"));
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test(expected = ClientErrorException.class)
    @Category(UnitTest.class)
    public void testUploadDeleteRelationStillBelongsToRelationWithoutIfUnused() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // Try to delete a relation that still belongs to a relation when the
        // "if-unused" tag is not passed. A failure should occur and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create/>" +
                        "<modify/>" +
                        "<delete>" +
                            "<relation id=\"" + relationIdsArr[0] + "\" version=\"1\" changeset=\"" +
                                changesetId + "\">" +
                            "</relation>" +
                        "</delete>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (ClientErrorException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.PRECONDITION_FAILED, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("still used by other relation(s)"));
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test(expected = BadRequestException.class)
    @Category(UnitTest.class)
    public void testUploadDeleteRelationEmptyMemberId() throws Exception {
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
                    "<create/>" +
                    "<modify/>" +
                    "<delete if-unused=\"true\"/>" +
                        "<relation id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                            "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                            "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                        "</relation>" +
                        "<relation id=\"-2\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                            "<member type=\"node\" role=\"role1\" ref=\"\"></member>" + // empty member ID
                            "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                        "</relation>" +
                    "</delete>" +
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
    public void testUploadDeleteRelationMissingMemberId() throws Exception {
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
                        "<create/>" +
                        "<modify/>" +
                        "<delete if-unused=\"true\"/>" +
                            "<relation id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                "<member type=\"node\" role=\"role1\"\"></member>" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                            "</relation>" +
                            "<relation id=\"-2\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                "<member type=\"node\" role=\"role1\"></member>" + // mising member ID
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                            "</relation>" +
                        "</delete>" +
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
