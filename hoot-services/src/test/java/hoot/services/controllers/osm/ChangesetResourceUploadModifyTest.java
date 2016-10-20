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
import java.util.Calendar;
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
import hoot.services.testsupport.HootCustomPropertiesSetter;
import hoot.services.utils.DbUtils;
import hoot.services.utils.PostgresUtils;
import hoot.services.utils.QuadTileCalculator;
import hoot.services.utils.RandomNumberGenerator;
import hoot.services.utils.XmlUtils;


public class ChangesetResourceUploadModifyTest extends OSMResourceTestAbstract {

    @Test
    @Category(UnitTest.class)
    public void testUploadModify() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // Now, update some of the elements and their tags.
        BoundingBox updatedBounds = OSMTestUtils.createAfterModifiedTestChangesetBounds();
        Document responseData = target("api/0.6/changeset/" + changesetId + "/upload")
            .queryParam("mapId", String.valueOf(mapId))
            .request(MediaType.TEXT_XML)
            .post(Entity.entity(
                "<osmChange version=\"0.3\" generator=\"iD\">" +
                    "<create/>" +
                    "<modify>" +
                        "<node id=\"" + nodeIdsArr[0] + "\" lon=\"" + updatedBounds.getMinLon() + "\" " + "lat=\"" +
                                 updatedBounds.getMinLat() + "\" version=\"1\" changeset=\"" + changesetId + "\">" +
                            "<tag k=\"key 1b\" v=\"val 1b\"></tag>" +
                            "<tag k=\"key 2b\" v=\"val 2b\"></tag>" +
                        "</node>" +
                        "<node id=\"" + nodeIdsArr[1] + "\" lon=\"" + originalBounds.getMaxLon() + "\" " +
                               "lat=\"" + updatedBounds.getMinLat() + "\" version=\"1\" changeset=\"" + changesetId + "\">" +
                            "<tag k=\"key 3b\" v=\"val 3b\"></tag>" +
                        "</node>" +
                        "<way id=\"" + wayIdsArr[0] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                            "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" + "<nd ref=\"" + nodeIdsArr[4] + "\"></nd>" +
                            "<tag k=\"key 2\" v=\"val 2\"></tag>" + "</way>" + "<way id=\"" + wayIdsArr[1] +
                                "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                            "<nd ref=\"" + nodeIdsArr[4] + "\"></nd>" +
                            "<nd ref=\"" + nodeIdsArr[2] + "\"></nd>" +
                        "</way>" +
                        "<relation id=\"" + relationIdsArr[0] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                            "<member type=\"way\" role=\"role4\" ref=\"" + wayIdsArr[1] + "\"></member>" +
                            "<member type=\"way\" role=\"role2\" ref=\"" + wayIdsArr[0] + "\"></member>" +
                            "<member type=\"node\" ref=\"" + nodeIdsArr[2] + "\"></member>" +
                        "</relation>" +
                        "<relation id=\"" + relationIdsArr[1] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                            "<member type=\"relation\" role=\"role1\" ref=\"" + relationIdsArr[0] + "\">" + "</member>" +
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
        assertEquals(
                Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@old_id", responseData)),
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
        assertEquals(new Double(updatedBounds.getMinLat()), nodeRecord.getLatitude());
        assertEquals(new Double(updatedBounds.getMinLon()), nodeRecord.getLongitude());
        assertEquals(nodeIdsArr[0], nodeRecord.getId());
        assertEquals(
                new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
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
        assertEquals(new Double(originalBounds.getMaxLon()), nodeRecord.getLongitude());
        assertEquals(nodeIdsArr[1], nodeRecord.getId());
        assertEquals(
                new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                nodeRecord.getTile());
        assertTrue(nodeRecord.getTimestamp().before(now));
        assertEquals(new Long(2), nodeRecord.getVersion());
        assertTrue(nodeRecord.getVisible());
        tags = PostgresUtils.postgresObjToHStore(nodeRecord.getTags());
        assertNotNull(tags);
        assertEquals(1, tags.size());
        assertEquals("val 3b", tags.get("key 3b"));

        nodeRecord = nodes.get(nodeIdsArr[2]);
        assertTrue((nodeRecord.getTags() == null)
                || PostgresUtils.postgresObjToHStore(nodeRecord.getTags()).isEmpty());

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

        // verify the created tags
        tags = PostgresUtils.postgresObjToHStore(wayRecord.getTags());

        assertNotNull(tags);
        assertEquals(1, tags.size());
        assertEquals("val 3", tags.get("key 3"));

        Map<Long, CurrentRelations> relations = createQuery(mapId)
                        .from(currentRelations)
                        .transform(groupBy(currentRelations.id).as(currentRelations));

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

        // removed tag
        assertTrue((relationRecord.getTags() == null)
                || PostgresUtils.postgresObjToHStore(relationRecord.getTags()).isEmpty());

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
        assertEquals(new Integer(18), changeset.getNumChanges());
        assertEquals(new Long(userId), changeset.getUserId());

        BoundingBox expandedBounds = new BoundingBox(originalBounds);
        expandedBounds.expand(updatedBounds, Double.parseDouble(CHANGESET_BOUNDS_EXPANSION_FACTOR_DEEGREES));
        Changeset hootChangeset = new Changeset(mapId, changesetId);
        BoundingBox changesetBounds = hootChangeset.getBounds();
        assertEquals(changesetBounds, expandedBounds);
    }

    @Test
    @Category(UnitTest.class)
    public void testUploadModifyClosedPolygon() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);

        // Now, update some of the elements and their tags.
        // TODO: why was this disabled?
        // final BoundingBox updatedBounds =
        // OSMTestUtils.createAfterModifiedTestChangesetBounds();
        Document responseData = target("api/0.6/changeset/" + changesetId + "/upload")
            .queryParam("mapId", String.valueOf(mapId))
            .request(MediaType.TEXT_XML)
            .post(Entity.entity(
                "<osmChange version=\"0.3\" generator=\"iD\">" +
                    "<create/>" +
                    "<modify>" +
                        "<way id=\"" + wayIdsArr[0] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                            "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                            "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                            "<nd ref=\"" + nodeIdsArr[2] + "\"></nd>" +
                            "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                        "</way>" +
                    "</modify>" +
                    "<delete if-unused=\"true\"/>" +
                "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);

        assertNotNull(responseData);

        XPath xpath = XmlUtils.createXPath();
        NodeList returnedWays = XPathAPI.selectNodeList(responseData, "//osm/diffResult/way");
        assertEquals(1, returnedWays.getLength());

        assertEquals((long) wayIdsArr[0],
                     Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@old_id", responseData)));

        assertEquals(Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@old_id", responseData)),
                     Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@new_id", responseData)));

        assertEquals(2, Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@new_version", responseData)));

        Timestamp now = super.getCurrentDBTime();

        Changesets changeset = createQuery(mapId)
                .select(changesets)
                .from(changesets)
                .where(changesets.id.eq(changesetId))
                .fetchOne();

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

        assertEquals(new Integer(13), changeset.getNumChanges());
        assertEquals(new Long(userId), changeset.getUserId());

        BoundingBox expandedBounds = new BoundingBox(originalBounds);
        Changeset hootChangeset = new Changeset(mapId, changesetId);
        BoundingBox changesetBounds = hootChangeset.getBounds();
        assertEquals(changesetBounds, expandedBounds);
    }

    @Test(expected = BadRequestException.class)
    @Category(UnitTest.class)
    public void testUploadModifyDuplicateNodeIds() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Try to create two nodes with the same ID. A failure should occur and
        // no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
            .queryParam("mapId", String.valueOf(mapId))
            .request(MediaType.TEXT_XML)
            .post(Entity.entity(
                "<osmChange version=\"0.3\" generator=\"iD\">" +
                    "<create/>" +
                    "<modify>" +
                        "<node id=\"" + nodeIdsArr[0] + "\" lon=\"" + originalBounds.getMinLon() + "\" " + "lat=\"" +
                            originalBounds.getMinLat() + "\" version=\"1\" changeset=\"" + changesetId + "\">" +
                           "<tag k=\"key 1b\" v=\"val 1b\"></tag>" +
                           "<tag k=\"key 2b\" v=\"val 2b\"></tag>" +
                        "</node>" +
                        "<node id=\"" + nodeIdsArr[0] + "\" lon=\"" + originalBounds.getMinLon() + "\" " +
                            "lat=\"" + originalBounds.getMinLat() + "\" version=\"1\" changeset=\"" + changesetId + "\">" +
                           "<tag k=\"key 3\" v=\"val 3\"></tag>" +
                        "</node>" +
                    "</modify>" +
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

    @Test(expected = NotFoundException.class)
    @Category(UnitTest.class)
    public void testUploadModifyInvalidChangesetSpecifiedInUrl() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(null);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);

        // Upload a changeset where the changeset specified in the URL doesn't
        // exist. A failure should occur and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + (changesetId + 1) + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                     "<osmChange version=\"0.3\" generator=\"iD\">" +
                         "<modify>" +
                             "<node id=\"" + nodeIdsArr[0] + "\" lon=\"" + originalBounds.getMinLon() +
                                   "\" lat=\"" + originalBounds.getMinLat() + "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                                 "<tag k=\"name 1\" v=\"val 1\"/>" +
                                 "<tag k=\"name 2\" v=\"val 2\"/>" +
                             "</node>" +
                             "<node id=\"" + nodeIdsArr[1] + "\" lon=\"" + originalBounds.getMinLon() +
                                   "\" lat=\"" + originalBounds.getMinLat() + "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                                 "<tag k=\"name 1\" v=\"val 1\"/>" +
                             "</node>" +
                             "</modify>" +
                         "<create/>" +
                         "<delete if-unused=\"true\"/>" +
                     "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (NotFoundException e) {
            Response r = e.getResponse();
            assertEquals(404, r.getStatus());
            assertTrue(r.readEntity(String.class).contains("Changeset to be updated does not exist"));
            OSMTestUtils.verifyTestChangesetUnmodified(changesetId);
            throw e;
        }
    }

    @Test(expected = BadRequestException.class)
    @Category(UnitTest.class)
    public void testUploadModifyEmptyNodeCoords() throws Exception {
        // Try to update a changeset with nodes that have empty coordinate
        // strings. A failure should occur and no data in system should be modified.
        uploadModifyNodeCoords("", "");
    }

    @Test(expected = BadRequestException.class)
    @Category(UnitTest.class)
    public void testUploadModifyNodeCoordsOutOfBounds() throws Exception {
        // Try to update a changeset with nodes that have out of world bounds
        // coordinate values.  A failure should occur and no data in system should be modified.
        uploadModifyNodeCoords("91", "-181");
    }

    private void uploadModifyNodeCoords(String lat, String lon) throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Now update the changeset via the service where one of the nodes has a
        // empty coords. A ailure should occur and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create/>" +
                        "<modify>" +
                        "<node id=\"" + nodeIdsArr[0] + "\" lon=\"" + originalBounds.getMinLon() + "\" " +
                               "lat=\"" + originalBounds.getMinLat() + "\" version=\"1\" changeset=\"" + changesetId + "\">" +
                            "<tag k=\"key 1\" v=\"val 1\"></tag>" +
                            "<tag k=\"key 2\" v=\"val 2\"></tag>" +
                        "</node>" +
                        "<node id=\"" + nodeIdsArr[1] + "\" lon=\"" + lon + "\" " + "lat=\"" + lat +
                               "\" version=\"1\" changeset=\"" + changesetId + "\">" +
                            "<tag k=\"key 3\" v=\"val 3\"></tag>" +
                        "</node>" +
                        "</modify>" +
                        "<delete if-unused=\"true\"/>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (BadRequestException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.BAD_REQUEST, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("Error uploading changeset with ID"));
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testUploadModifyNoTags() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // Now update the changeset with elements that have no tags in them,
        // which is allowed. The existing tags for the specified elements will be deleted, since
        // every request must specify all the tags it wants retained for each element. The elements not
        // referenced in this modify request should retain their tags.
        BoundingBox updatedBounds = OSMTestUtils.createAfterModifiedTestChangesetBounds();
        Document responseData = target("api/0.6/changeset/" + changesetId + "/upload")
            .queryParam("mapId", String.valueOf(mapId))
            .request(MediaType.TEXT_XML)
            .post(Entity.entity(
                "<osmChange version=\"0.3\" generator=\"iD\">" +
                     "<create/>" +
                     "<modify>" +
                         "<node id=\"" + nodeIdsArr[0] + "\" lon=\"" + updatedBounds.getMinLon() + "\" lat=\"" +
                               updatedBounds.getMinLat() + "\" version=\"1\" changeset=\"" + changesetId + "\">" +
                         "</node>" +
                         "<node id=\"" + nodeIdsArr[1] + "\" lon=\"" + originalBounds.getMaxLon() + "\" lat=\"" +
                               updatedBounds.getMinLat() + "\" version=\"1\" changeset=\"" + changesetId + "\">" +
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
                            "<member type=\"relation\" role=\"role1\" ref=\"" + relationIdsArr[0] + "\">" +
                            "</member>" + "<member type=\"node\" ref=\"" + nodeIdsArr[4] + "\"></member>" +
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
        assertEquals(2,
                Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@new_version", responseData)));

        assertEquals((long) wayIdsArr[1],
                Long.parseLong(xpath.evaluate("//osm/diffResult/way[2]/@old_id", responseData)));
        assertEquals(Long.parseLong(xpath.evaluate("//osm/diffResult/way[2]/@old_id", responseData)),
                Long.parseLong(xpath.evaluate("//osm/diffResult/way[2]/@new_id", responseData)));
        assertEquals(2,
                Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@new_version", responseData)));

        NodeList returnedRelations = XPathAPI.selectNodeList(responseData, "//osm/diffResult/relation");
        assertEquals(2, returnedRelations.getLength());

        assertEquals((long) relationIdsArr[0],
                Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@old_id", responseData)));
        assertEquals(
                Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@old_id", responseData)),
                Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@new_id", responseData)));
        assertEquals(2,
                Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@new_version", responseData)));

        assertEquals((long) relationIdsArr[1],
                Long.parseLong(xpath.evaluate("//osm/diffResult/relation[2]/@old_id", responseData)));
        assertEquals(
                Long.parseLong(xpath.evaluate("//osm/diffResult/relation[2]/@old_id", responseData)),
                Long.parseLong(xpath.evaluate("//osm/diffResult/relation[2]/@new_id", responseData)));
        assertEquals(2,
                Long.parseLong(xpath.evaluate("//osm/diffResult/relation[2]/@new_version", responseData)));

        Timestamp now = super.getCurrentDBTime();

        Map<Long, CurrentNodes> nodes = createQuery(mapId)
                .from(currentNodes)
                .transform(groupBy(currentNodes.id).as(currentNodes));

        assertEquals(5, nodes.size());

        CurrentNodes nodeRecord = nodes.get(nodeIdsArr[0]);
        assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
        assertEquals(new Double(updatedBounds.getMinLat()), nodeRecord.getLatitude());
        assertEquals(new Double(updatedBounds.getMinLon()), nodeRecord.getLongitude());
        assertEquals(nodeIdsArr[0], nodeRecord.getId());
        assertEquals(
                new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                nodeRecord.getTile());
        assertTrue(nodeRecord.getTimestamp().before(now));
        assertEquals(new Long(2), nodeRecord.getVersion());
        assertTrue(nodeRecord.getVisible());
        assertTrue((nodeRecord.getTags() == null)
                || PostgresUtils.postgresObjToHStore(nodeRecord.getTags()).isEmpty());

        nodeRecord = nodes.get(nodeIdsArr[1]);
        assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
        assertEquals(new Double(updatedBounds.getMinLat()), nodeRecord.getLatitude());
        assertEquals(new Double(originalBounds.getMaxLon()), nodeRecord.getLongitude());
        assertEquals(nodeIdsArr[1], nodeRecord.getId());
        assertEquals(
                new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                nodeRecord.getTile());
        assertTrue(nodeRecord.getTimestamp().before(now));
        assertEquals(new Long(2), nodeRecord.getVersion());
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
        Map<String, String> tags = PostgresUtils.postgresObjToHStore(nodeRecord.getTags());
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

        assertEquals(nodeIdsArr[1], wayNode.getNodeId());
        assertEquals(new Long(2), wayNode.getSequenceId());
        assertEquals(wayRecord.getId(), wayNode.getWayId());
        assertTrue((wayRecord.getTags() == null)
                || PostgresUtils.postgresObjToHStore(wayRecord.getTags()).isEmpty());

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

        assertEquals(nodeIdsArr[0], wayNode.getNodeId());
        assertEquals(new Long(1), wayNode.getSequenceId());
        assertEquals(wayRecord.getId(), wayNode.getWayId());
        wayNode = wayNodes.get(1);
        assertEquals(nodeIdsArr[1], wayNode.getNodeId());
        assertEquals(new Long(2), wayNode.getSequenceId());
        assertEquals(wayRecord.getId(), wayNode.getWayId());
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

        tags = PostgresUtils.postgresObjToHStore(wayRecord.getTags());

        assertNotNull(tags);
        assertEquals(1, tags.size());
        assertEquals("val 3", tags.get("key 3"));

        Map<Long, CurrentRelations> relations = createQuery(mapId)
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
        assertTrue((relationRecord.getTags() == null)
                || PostgresUtils.postgresObjToHStore(relationRecord.getTags()).isEmpty());

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
        assertTrue((relationRecord.getTags() == null)
                || PostgresUtils.postgresObjToHStore(relationRecord.getTags()).isEmpty());

        relationRecord = relations.get(relationIdsArr[2]);
        assertEquals(new Long(changesetId), relationRecord.getChangesetId());
        assertEquals(relationIdsArr[2], relationRecord.getId());

        assertTrue(relationRecord.getTimestamp().before(now));
        assertEquals(new Long(1), relationRecord.getVersion());
        assertEquals(true, relationRecord.getVisible());

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

        Changesets changeset = createQuery(mapId)
                .select(changesets)
                .from(changesets)
                .where(changesets.id.eq(changesetId))
                .fetchOne();

        assertNotNull(changeset);
        assertTrue(changeset.getCreatedAt().before(now));
        assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));
        assertEquals(new Integer(18), changeset.getNumChanges());
        assertEquals(new Long(userId), changeset.getUserId());

        BoundingBox expandedBounds = new BoundingBox(originalBounds);
        expandedBounds.expand(updatedBounds, Double.parseDouble(CHANGESET_BOUNDS_EXPANSION_FACTOR_DEEGREES));
        Changeset hootChangeset = new Changeset(mapId, changesetId);
        BoundingBox changesetBounds = hootChangeset.getBounds();

        assertEquals(changesetBounds, expandedBounds);
    }

    @Test(expected = ClientErrorException.class)
    @Category(UnitTest.class)
    public void testUploadModifyInvalidChangesetIdInNode() throws Exception {
        BoundingBox changeset1Bounds = OSMTestUtils.createStartingTestBounds();
        long changesetId1 = OSMTestUtils.createTestChangeset(changeset1Bounds);
        BoundingBox changeset2Bounds = OSMTestUtils.createStartingTestBounds();
        long changesetId2 = OSMTestUtils.createTestChangeset(changeset2Bounds);

        // link some nodes to the changesets
        Set<Long> nodeIds1 = OSMTestUtils.createTestNodes(changesetId1, changeset1Bounds);
        Long[] nodeIds1Arr = nodeIds1.toArray(new Long[nodeIds1.size()]);
        Set<Long> nodeIds2 = OSMTestUtils.createTestNodes(changesetId2, changeset2Bounds);
        Long[] nodeIds2Arr = nodeIds2.toArray(new Long[nodeIds2.size()]);

        // Upload a changeset where one of the elements in the changeset lists a
        // changeset ID that doesn't match the changeset ID specified in the URL. A failure should
        // occur and no data in the system should be modified.
        BoundingBox updatedBounds = OSMTestUtils.createAfterModifiedTestChangesetBounds();
        try {
            target("api/0.6/changeset/" + changesetId1 + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create/>" +
                        "<modify>" +
                            "<node id=\"" + nodeIds1Arr[0] + "\" lon=\"" + updatedBounds.getMinLon() + "\" " +
                                  "lat=\"" + updatedBounds.getMinLat() + "\" version=\"1\" changeset=\"" + changesetId1 + "\">" +
                                "<tag k=\"key 1b\" v=\"val 1b\"></tag>" +
                                "<tag k=\"key 2b\" v=\"val 2b\"></tag>" +
                            "</node>" +
                            "<node id=\"" + nodeIds2Arr[2] + "\" lon=\"" + updatedBounds.getMinLon() + "\" " +
                                "lat=\"" + updatedBounds.getMinLat() + "\" version=\"1\" changeset=\"" + changesetId2 + "\">" +
                                "<tag k=\"key 3b\" v=\"val 3b\"></tag>" +
                            "</node>" +
                        "</modify>" +
                        "<delete if-unused=\"true\"/>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (ClientErrorException e) {
            Response r = e.getResponse();

            assertEquals(Response.Status.CONFLICT, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("Invalid changeset ID"));

            OSMTestUtils.verifyTestNodesUnmodified(nodeIds1, changesetId1, changeset1Bounds);
            OSMTestUtils.verifyTestNodesUnmodified(nodeIds2, changesetId2, changeset2Bounds);

            Changesets changeset = createQuery(mapId)
                    .select(changesets)
                    .from(changesets)
                    .where(changesets.id.eq(changesetId1))
                    .fetchOne();

            assertNotNull(changeset);
            Timestamp now = super.getCurrentDBTime();
            assertTrue(changeset.getCreatedAt().before(now));
            assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));
            assertEquals(new Integer(12), changeset.getNumChanges());
            assertEquals(new Long(userId), changeset.getUserId());

            changeset = createQuery(mapId)
                    .select(changesets)
                    .from(changesets)
                    .where(changesets.id.eq(changesetId2))
                    .fetchOne();

            assertNotNull(changeset);
            assertTrue(changeset.getCreatedAt().before(now));
            assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));
            assertEquals(new Integer(12), changeset.getNumChanges());
            assertEquals(new Long(userId), changeset.getUserId());

            // make sure the changeset bounds weren't updated
            Changeset hootChangeset = new Changeset(mapId, changesetId1);
            BoundingBox changesetBounds = hootChangeset.getBounds();
            BoundingBox expandedBounds = new BoundingBox();
            double boundsExpansionFactor = Double.parseDouble(CHANGESET_BOUNDS_EXPANSION_FACTOR_DEEGREES);
            expandedBounds.expand(changeset1Bounds, boundsExpansionFactor);
            assertEquals(expandedBounds, changesetBounds);

            hootChangeset = new Changeset(mapId, changesetId2);
            changesetBounds = hootChangeset.getBounds();
            expandedBounds = new BoundingBox();
            expandedBounds.expand(changeset2Bounds, boundsExpansionFactor);
            assertEquals(expandedBounds, changesetBounds);

            throw e;
        }
    }

    @Test(expected = NotFoundException.class)
    @Category(UnitTest.class)
    public void testUploadModifyNonExistingNode() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        BoundingBox updateBounds = OSMTestUtils.createAfterModifiedTestChangesetBounds();

        // Update the changeset with a node that has an ID that doesn't exist. A
        // failure should occur and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create/>" +
                        "<modify>" +
                            "<node id=\"" + nodeIdsArr[0] + "\" lon=\"" + originalBounds.getMinLon() + "\" " + "lat=\"" +
                                   originalBounds.getMinLat() + "\" version=\"1\" changeset=\"" + changesetId + "\">" +
                                "<tag k=\"key 1\" v=\"val 1\"></tag>" + "<tag k=\"key 2\" v=\"val 2\"></tag>" +
                            "</node>" +
                            "<node id=\"" + ((int) RandomNumberGenerator.nextDouble(nodeIdsArr[0] + 10 ^ 4, Integer.MAX_VALUE)) +
                                    "\" lon=\"" + updateBounds.getMinLon() + "\" " + "lat=\"" + updateBounds.getMinLat() +
                                    "\" version=\"1\" changeset=\"" + changesetId + "\">" + "" +
                                "<tag k=\"key 3\" v=\"val 3\"></tag>" +
                            "</node>" +
                        "</modify>" +
                        "<delete if-unused=\"true\"/>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (NotFoundException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.NOT_FOUND, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("Element(s) being referenced don't exist."));

            // make sure the bad node wasn't created and the other node wasn't updated
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testUploadModifyNegativeElementId() throws Exception {
        // We allow elements with a negative ID to exist, which allows for
        // using the hootProps.hoot --convert command as a source of test data for the services.

        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        BoundingBox updateBounds = OSMTestUtils.createAfterModifiedTestChangesetBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);

        // link some nodes to the changeset

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

        CurrentNodes insertedWayRecord = createQuery(mapId)
                .select(currentNodes)
                .from(currentNodes)
                .where(currentNodes.id.eq(negativeWayId))
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

        long negativeRelationId = -1;
        OSMTestUtils.insertNewRelation(negativeRelationId, changesetId, mapId, members, tags);
        tags.clear();
        CurrentRelations insertedRelationRecord =
                createQuery(mapId)
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
        tags.put("key 2", "val 2");
        relationIds.add(OSMTestUtils.insertNewRelation(changesetId, mapId, members, tags));
        tags.clear();

        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // Update the changeset via the service. This update is valid, because the property was changed above.
        Document responseData = target("api/0.6/changeset/" + changesetId + "/upload")
            .queryParam("mapId", String.valueOf(mapId))
            .request(MediaType.TEXT_XML)
            .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                            "<create/>" +
                            "<modify>" +
                                "<node id=\"" + nodeIdsArr[1] + "\" lon=\"" + updateBounds.getMinLon() + "\" " +
                                        "lat=\"" + updateBounds.getMinLat() + "\" version=\"1\" changeset=\"" + changesetId + "\">" +
                                    "<tag k=\"key 1b\" v=\"val 1b\"></tag>" +
                                    "<tag k=\"key 2b\" v=\"val 2b\"></tag>" +
                                "</node>" +
                                "<node id=\"" + negativeNodeId + "\" lon=\"" + originalBounds.getMaxLon() + "\" " +
                                        "lat=\"" + updateBounds.getMinLat() + "\" version=\"1\" changeset=\"" + changesetId + "\">" +
                                    "<tag k=\"key 3b\" v=\"val 3b\"></tag>" +
                                "</node>" +
                                "<way id=\"" + wayIdsArr[1] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                    "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                    "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                                    "<tag k=\"key 1b\" v=\"val 1b\"></tag>" +
                                    "<tag k=\"key 2b\" v=\"val 2b\"></tag>" +
                                "</way>" +
                                "<way id=\"" + negativeWayId + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                    "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                    "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                                    "<tag k=\"key 1\" v=\"val 1\"></tag>" +
                                "</way>" +
                                "<relation id=\"" + relationIdsArr[1] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                    "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                    "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[0] + "\"></member>" +
                                    "<tag k=\"key 2b\" v=\"val 2b\"></tag>" +
                                "</relation>" +
                                "<relation id=\"" + negativeRelationId + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                    "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                                    "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[1] + "\"></member>" +
                                "</relation>" +
                            "</modify>" +
                            "<delete if-unused=\"true\"/>" +
                            "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);

        assertNotNull(responseData);

        XPath xpath = XmlUtils.createXPath();
        NodeList returnedNodes = XPathAPI.selectNodeList(responseData, "//osm/diffResult/node");

        assertEquals(2, returnedNodes.getLength());
        assertEquals((long) nodeIdsArr[1],
                Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@old_id", responseData)));
        assertEquals(Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@old_id", responseData)),
                Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@new_id", responseData)));
        assertEquals(2, Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@new_version", responseData)));
        assertEquals(negativeNodeId,
                Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@old_id", responseData)));
        assertEquals(Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@old_id", responseData)),
                Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@new_id", responseData)));
        assertEquals(2, Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@new_version", responseData)));

        NodeList returnedWays = XPathAPI.selectNodeList(responseData, "//osm/diffResult/way");

        assertEquals(2, returnedWays.getLength());

        assertEquals((long) wayIdsArr[1],
                Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@old_id", responseData)));
        assertEquals(Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@old_id", responseData)),
                Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@new_id", responseData)));
        assertEquals(2, Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@new_version", responseData)));
        assertEquals(negativeWayId,
                Long.parseLong(xpath.evaluate("//osm/diffResult/way[2]/@old_id", responseData)));

        assertEquals(Long.parseLong(xpath.evaluate("//osm/diffResult/way[2]/@old_id", responseData)),
                Long.parseLong(xpath.evaluate("//osm/diffResult/way[2]/@new_id", responseData)));

        assertEquals(2, Long.parseLong(xpath.evaluate("//osm/diffResult/way[2]/@new_version", responseData)));

        NodeList returnedRelations = XPathAPI.selectNodeList(responseData, "//osm/diffResult/relation");

        assertEquals(2, returnedRelations.getLength());
        assertEquals((long) relationIdsArr[1],
                Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@old_id", responseData)));
        assertEquals(Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@old_id", responseData)),
                Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@new_id", responseData)));
        assertEquals(2, Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@new_version", responseData)));
        assertEquals(negativeRelationId,
                Long.parseLong(xpath.evaluate("//osm/diffResult/relation[2]/@old_id", responseData)));
        assertEquals(Long.parseLong(xpath.evaluate("//osm/diffResult/relation[2]/@old_id", responseData)),
                Long.parseLong(xpath.evaluate("//osm/diffResult/relation[2]/@new_id", responseData)));
        assertEquals(2, Long.parseLong(xpath.evaluate("//osm/diffResult/relation[2]/@new_version", responseData)));

        Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());

        Map<Long, CurrentNodes> nodes = createQuery(mapId)
                .from(currentNodes)
                .transform(groupBy(currentNodes.id).as(currentNodes));

        assertEquals(2, nodes.size());
        assertEquals(negativeNodeId, (long) nodeIdsArr[0]);

        CurrentNodes nodeRecord = nodes.get(nodeIdsArr[0]);

        assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
        assertEquals(new Double(updateBounds.getMinLat()), nodeRecord.getLatitude());
        assertEquals(new Double(originalBounds.getMaxLon()), nodeRecord.getLongitude());
        assertEquals(new Long(negativeNodeId), nodeRecord.getId());
        assertEquals(new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                nodeRecord.getTile());
        assertTrue(nodeRecord.getTimestamp().before(now));
        assertEquals(new Long(2), nodeRecord.getVersion());
        assertTrue(nodeRecord.getVisible());

        Map<String, String> parsedTags = PostgresUtils.postgresObjToHStore(nodeRecord.getTags());

        assertNotNull(parsedTags);
        assertEquals(1, parsedTags.size());
        assertEquals("val 3b", parsedTags.get("key 3b"));

        nodeRecord = nodes.get(nodeIdsArr[1]);

        assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
        assertEquals(new Double(updateBounds.getMinLat()), nodeRecord.getLatitude());
        assertEquals(new Double(updateBounds.getMinLon()), nodeRecord.getLongitude());
        assertEquals(nodeIdsArr[1], nodeRecord.getId());
        assertEquals(new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                nodeRecord.getTile());
        assertTrue(nodeRecord.getTimestamp().before(now));
        assertEquals(new Long(2), nodeRecord.getVersion());
        assertTrue(nodeRecord.getVisible());
        parsedTags = PostgresUtils.postgresObjToHStore(nodeRecord.getTags());
        assertNotNull(parsedTags);
        assertEquals(2, parsedTags.size());
        assertEquals("val 1b", parsedTags.get("key 1b"));
        assertEquals("val 2b", parsedTags.get("key 2b"));

        Map<Long, CurrentWays> ways = createQuery(mapId)
                .from(currentWays)
                .transform(groupBy(currentWays.id).as(currentWays));

        assertEquals(2, ways.size());

        // verify the updated ways
        assertEquals(negativeWayId, (long) wayIdsArr[0]);

        CurrentWays wayRecord = ways.get(wayIdsArr[0]);

        assertEquals(new Long(changesetId), wayRecord.getChangesetId());
        assertEquals(new Long(negativeWayId), wayRecord.getId());
        assertTrue(wayRecord.getTimestamp().before(now));
        assertEquals(new Long(2), wayRecord.getVersion());
        assertTrue(wayRecord.getVisible());

        List<CurrentWayNodes> wayNodes = createQuery(mapId)
                .from(currentWayNodes)
                .select(currentWayNodes)
                .where(currentWayNodes.wayId.eq(wayIdsArr[1]))
                .orderBy(currentWayNodes.sequenceId.asc())
                .fetch();

        assertEquals(2, wayNodes.size());
/*
        CurrentWayNodes wayNode = wayNodes.get(0);

        assertEquals(nodeIdsArr[0], wayNode.getNodeId());
        assertEquals(new Long(1), wayNode.getSequenceId());
        assertEquals(wayRecord.getId(), wayNode.getWayId());

        wayNode = wayNodes.get(1);

        assertEquals(nodeIdsArr[1], wayNode.getNodeId());
        assertEquals(new Long(2), wayNode.getSequenceId());
        assertEquals(wayRecord.getId(), wayNode.getWayId());

        parsedTags = PostgresUtils.postgresObjToHStore(wayRecord.getTags());

        assertNotNull(parsedTags);
        assertEquals(1, parsedTags.size());
        assertEquals("val 1", parsedTags.get("key 1"));

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

        assertEquals(nodeIdsArr[1], wayNode.getNodeId());
        assertEquals(new Long(1), wayNode.getSequenceId());
        assertEquals(wayRecord.getId(), wayNode.getWayId());

        wayNode = wayNodes.get(1);

        assertEquals(nodeIdsArr[2], wayNode.getNodeId());
        assertEquals(new Long(2), wayNode.getSequenceId());
        assertEquals(wayRecord.getId(), wayNode.getWayId());

        parsedTags = PostgresUtils.postgresObjToHStore(wayRecord.getTags());

        assertNotNull(parsedTags);
        assertEquals(2, parsedTags.size());
        assertEquals("val 1b", parsedTags.get("key 1b"));
        assertEquals("val 2b", parsedTags.get("key 2b"));

        Map<Long, CurrentRelations> relations = createQuery(mapId)
                .from(currentRelations)
                .transform(groupBy(currentRelations.id).as(currentRelations));

        assertEquals(2, relations.size());
        assertEquals(negativeRelationId, (long) relationIdsArr[0]);

        CurrentRelations relationRecord = relations.get(relationIdsArr[0]);

        assertEquals(new Long(changesetId), relationRecord.getChangesetId());
        assertEquals(relationIdsArr[0], relationRecord.getId());
        assertTrue(relationRecord.getTimestamp().before(now));
        assertEquals(new Long(2), relationRecord.getVersion());
        assertTrue(relationRecord.getVisible());

        List<CurrentRelationMembers> memberRecords = createQuery(mapId)
                .select(currentRelationMembers)
                .from(currentRelationMembers)
                .where(currentRelationMembers.relationId.eq(relationIdsArr[0]))
                .orderBy(currentRelationMembers.sequenceId.asc())
                .fetch();

        assertEquals(2, members.size());

        CurrentRelationMembers member = memberRecords.get(0);

        assertEquals(relationRecord.getId(), member.getRelationId());
        assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
        assertEquals("role1", member.getMemberRole());
        assertEquals(new Integer(1), member.getSequenceId());
        assertEquals(nodeIdsArr[1], member.getMemberId());

        member = memberRecords.get(1);

        assertEquals(relationRecord.getId(), member.getRelationId());
        assertEquals(DbUtils.nwr_enum.way, member.getMemberType());
        assertEquals("role1", member.getMemberRole());
        assertEquals(new Integer(2), member.getSequenceId());
        assertEquals(wayIdsArr[1], member.getMemberId());

        parsedTags = PostgresUtils.postgresObjToHStore(relationRecord.getTags());

        assertNotNull(parsedTags);
        assertEquals(1, parsedTags.size());
        assertEquals("val 1", parsedTags.get("key 1"));

        relationRecord = relations.get(relationIdsArr[1]);

        assertEquals(new Long(changesetId), relationRecord.getChangesetId());
        assertEquals(relationIdsArr[1], relationRecord.getId());
        assertTrue(relationRecord.getTimestamp().before(now));
        assertEquals(new Long(2), relationRecord.getVersion());
        assertTrue(relationRecord.getVisible());

        memberRecords = createQuery(mapId)
                .select(currentRelationMembers)
                .from(currentRelationMembers)
                .where(currentRelationMembers.relationId.eq(relationIdsArr[1]))
                .orderBy(currentRelationMembers.sequenceId.asc())
                .fetch();

        assertEquals(2, members.size());

        member = memberRecords.get(0);

        assertEquals(relationRecord.getId(), member.getRelationId());
        assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
        assertEquals("role1", member.getMemberRole());
        assertEquals(new Integer(1), member.getSequenceId());
        assertEquals(nodeIdsArr[0], member.getMemberId());

        member = memberRecords.get(1);

        assertEquals(relationRecord.getId(), member.getRelationId());
        assertEquals(DbUtils.nwr_enum.way, member.getMemberType());
        assertEquals("role1", member.getMemberRole());
        assertEquals(new Integer(2), member.getSequenceId());
        assertEquals(wayIdsArr[0], member.getMemberId());

        parsedTags = PostgresUtils.postgresObjToHStore(relationRecord.getTags());

        assertNotNull(parsedTags);
        assertEquals(1, parsedTags.size());
        assertEquals("val 2b", parsedTags.get("key 2b"));

        Changesets changeset = createQuery(mapId)
                .select(changesets)
                .from(changesets)
                .where(changesets.id.eq(changesetId))
                .fetchOne();

        assertNotNull(changeset);
        assertTrue(changeset.getCreatedAt().before(now));
        assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));
        assertEquals(new Integer(6), changeset.getNumChanges());
        assertEquals(new Long(userId), changeset.getUserId());

        BoundingBox expandedBounds = new BoundingBox(originalBounds);
        expandedBounds.expand(updateBounds, Double.parseDouble(CHANGESET_BOUNDS_EXPANSION_FACTOR_DEEGREES));

        Changeset hootChangeset = new Changeset(mapId, changesetId);
        BoundingBox changesetBounds = hootChangeset.getBounds();

        assertEquals(changesetBounds, expandedBounds);
*/
    }

    @Test(expected = ClientErrorException.class)
    @Category(UnitTest.class)
    public void testUploadModifyNodeInvalidVersion() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        BoundingBox updateBounds = OSMTestUtils.createAfterModifiedTestChangesetBounds();

        // Update the changeset where one of the nodes has version that doesn't
        // match the version on the server. A failure should occur and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create/>" +
                        "<modify>" +
                            "<node id=\"" + nodeIdsArr[0] + "\" lon=\"" + originalBounds.getMinLon() + "\" " + "lat=\"" +
                                  originalBounds.getMinLat() + "\" version=\"1\" changeset=\"" + changesetId + "\">" +
                                "<tag k=\"key 1\" v=\"val 1\"></tag>" +
                                "<tag k=\"key 2\" v=\"val 2\"></tag>" +
                            "</node>" +
                            "<node id=\"" + nodeIdsArr[1] + "\" lon=\"" + updateBounds.getMinLon() + "\" " +
                                 "lat=\"" + updateBounds.getMinLat() + "\" version=\"2\" changeset=\"" + changesetId + "\">" +
                                "<tag k=\"key 3\" v=\"val 3\"></tag>" +
                            "</node>" +
                        "</modify>" +
                        "<delete if-unused=\"true\"/>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (ClientErrorException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.CONFLICT, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("Invalid version"));

            // make sure neither node was updated
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testUploadModifyNoMembersInRelation() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // relations with no members are allowed
        Document responseData = target("api/0.6/changeset/" + changesetId + "/upload")
            .queryParam("mapId", String.valueOf(mapId))
            .request(MediaType.TEXT_XML)
            .post(Entity.entity(
                "<osmChange version=\"0.3\" generator=\"iD\">" +
                    "<create/>" +
                    "<modify>" +
                       "<relation id=\"" + relationIdsArr[0] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" + "</relation>" +
                    "</modify>" +
                    "<delete if-unused=\"true\"/>" +
                "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);

        assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm/diffResult/relation").getLength());
    }

    @Test(expected = ClientErrorException.class)
    @Category(UnitTest.class)
    public void testUploadModifyWayInvalidVersion() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Update the changeset where one of the nodes has a version greater
        // than zero, which is what the version should be for all new elements. A failure should occur
        // and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create/>" +
                        "<modify>" +
                            "<way id=\"" + wayIdsArr[0] + "\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                            "</way>" +
                            "<way id=\"" + wayIdsArr[1] + "\" version=\"2\" changeset=\"" + changesetId + "\" >" +
                                "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                            "</way>" +
                        "</modify>" +
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
    public void testUploadModifyRelationInvalidVersion() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // Update the changeset where one of the relations has an invalid
        // version. A failure should occur and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create/>" +
                        "<modify>" +
                            "<relation id=\"" + relationIdsArr[0] + "\" version=\"1\" changeset=\"" +
                                    changesetId + "\" >" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                            "</relation>" +
                            "<relation id=\"" + relationIdsArr[1] + "\" version=\"2\" changeset=\"" +
                                    changesetId + "\" >" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                            "</relation>" +
                        "</modify>" +
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
    public void testUploadModifyChangesetClosed() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // close the changeset
        Timestamp now = new Timestamp(System.currentTimeMillis());

        createQuery(mapId)
                .update(changesets)
                .where(changesets.id.eq(changesetId))
                .set(changesets.closedAt, now)
                .execute();

        Changesets changeset = createQuery(mapId)
                .select(changesets)
                .from(changesets)
                .where(changesets.id.eq(changesetId))
                .fetchOne();

        assertTrue(changeset.getClosedAt().equals(now));

        BoundingBox updateBounds = OSMTestUtils.createAfterModifiedTestChangesetBounds();

        // Try to update the closed changeset. A failure should occur and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create/>" +
                        "<modify>" +
                            "<node id=\"" + nodeIdsArr[0] + "\" lon=\"" + updateBounds.getMinLon() + "\" " +
                                 "lat=\"" + updateBounds.getMinLat() + "\" version=\"1\" changeset=\"" + changesetId + "\">" +
                                 "<tag k=\"key 1\" v=\"val 1\"></tag>" +
                                 "<tag k=\"key 2\" v=\"val 2\"></tag>" +
                            "</node>" +
                        "</modify>" +
                        "<delete if-unused=\"true\"/>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (ClientErrorException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.CONFLICT, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("The changeset with ID: " + changesetId + " was closed at"));

            // make sure nothing was updated
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            OSMTestUtils.verifyTestChangesetClosed(changesetId);
            throw e;
        }
    }

    @Test(expected = BadRequestException.class)
    @Category(UnitTest.class)
    public void testUploadModifyMissingNodeTagValue() throws Exception {
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
                        "<modify>" +
                            "<node id=\"" + nodeIdsArr[0] + "\" lon=\"" + originalBounds.getMinLon() + "\" lat=\"" +
                                  originalBounds.getMinLat() + "\" version=\"1\" changeset=\"" + changesetId + "\">" +
                                "<tag k=\"name 1\"/>" + // missing tag value
                                "<tag k=\"name 2\" v=\"val 2\"/>" +
                            "</node>" +
                            "<node id=\"" + nodeIdsArr[1] + "\" lon=\"" + originalBounds.getMinLon() + "\" lat=\"" +
                                  originalBounds.getMinLat() + "\" version=\"1\" changeset=\"" + changesetId + "\">" +
                                "<tag k=\"name 1\" v=\"val 1\"/>" +
                            "</node>" +
                        "</modify>" +
                        "<create/>" +
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
    public void testUploadModifyTooFewNodesForWay() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // A way has to have two or more nodes. Try to upload a way with one
        // node. The request should fail and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create/>" +
                        "<modify>" +
                            "<way id=\"" + wayIdsArr[0] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                "<tag k=\"key 1\" v=\"val 1\"/>" +
                            "</way>" +
                        "</modify>" +
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
    public void testUploadModifyTooManyNodesForWay() throws Exception {
        String originalMaximumWayNodes = MAXIMUM_WAY_NODES;

        BoundingBox originalBounds = null;
        Long changesetId = null;
        Set<Long> nodeIds = null;
        Set<Long> wayIds = null;
        Set<Long> relationIds = null;

        // A way has to have two or more nodes. Try to upload a way more nodes
        // than is allowed by the
        // system. The request should fail and no data in the system should be
        // modified.
        try {
            originalBounds = OSMTestUtils.createStartingTestBounds();
            changesetId = OSMTestUtils.createTestChangeset(originalBounds);
            nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
            Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
            wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
            Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
            relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

            // remember the original value to restore it later
            originalMaximumWayNodes = MAXIMUM_WAY_NODES;

            // use a lower number of max way nodes then default for efficiency
            HootCustomPropertiesSetter.setProperty("MAXIMUM_WAY_NODES", "2");

            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create/>" +
                        "<modify>" +
                            "<way id=\"" + wayIdsArr[0] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                                "<nd ref=\"" + nodeIdsArr[2] + "\"></nd>" +
                            "</way>" +
                        "</modify>" +
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

    @Test(expected = NotFoundException.class)
    @Category(UnitTest.class)
    public void testUploadModifyWayWithNonExistentNode() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Try to create a way referencing a node that doesn't exist. The
        // request should fail and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create/>" +
                        "<modify>" +
                            "<way id=\"" + wayIdsArr[0] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                "<nd ref=\"" + (nodeIdsArr[4] + 2) + "\"></nd>" +
                            "</way>" +
                        "</modify>" +
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
    public void testUploadModifyRelationWithNonExistentNode() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // Try to update a relation referencing a node that doesn't exist. The
        // request should fail and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create/>" +
                        "<modify>" +
                            "<relation id=\"" + relationIdsArr[0] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + (nodeIdsArr[4] + 2) + "\"></member>" +
                            "</relation>" +
                        "</modify>" +
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
    public void testUploadModifyRelationWithNonExistentWay() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // Try to update a relation referencing a way that doesn't exist. The
        // request should fail and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create/>" +
                        "<modify>" +
                        "<relation id=\"" + relationIdsArr[0] + "\" version=\"1\" changeset=\"" +
                               changesetId + "\" >" +
                            "<member type=\"way\" role=\"role1\" ref=\"" + (wayIdsArr[2] + 1) + "\"></member>" +
                            "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                        "</relation>" +
                        "</modify>" +
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
    public void testUploadModifyRelationWithNonExistentRelation() throws Exception {
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
                        "<create/>" +
                        "<modify>" +
                            "<relation id=\"" + relationIdsArr[0] + "\" version=\"1\" changeset=\"" +
                                    changesetId + "\" >" +
                                "<member type=\"relation\" role=\"role1\" ref=\"" + (relationIdsArr[3] + 1) + "\"></member>" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                            "</relation>" +
                        "</modify>" +
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

    @Test(expected = BadRequestException.class)
    @Category(UnitTest.class)
    public void testUploadModifyWayWithInvisibleNode() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // make one of way nodes invisible
        CurrentNodes invisibleNode = createQuery(mapId)
                .select(currentNodes)
                .from(currentNodes)
                .where(currentNodes.id.eq(nodeIdsArr[0]))
                .fetchOne();

        assertNotNull(invisibleNode);

        invisibleNode.setVisible(false);

        long success = createQuery(mapId).update(currentNodes)
                .where(currentNodes.id.eq(invisibleNode.getId()))
                .set(currentNodes.visible, false)
                .execute();

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
                        "<create/>" +
                        "<modify>" +
                            "<way id=\"" + wayIdsArr[0] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                            "</way>" +
                        "</modify>" +
                        "<delete if-unused=\"true\"/>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (BadRequestException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.BAD_REQUEST, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("aren't visible for way"));
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test(expected = BadRequestException.class)
    @Category(UnitTest.class)
    public void testUploadModifyRelationWithInvisibleNode() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // make one of relation node members invisible
        CurrentNodes invisibleNode = createQuery(mapId)
                .select(currentNodes)
                .from(currentNodes)
                .where(currentNodes.id.eq(nodeIdsArr[0]))
                .fetchOne();

        assertNotNull(invisibleNode);

        invisibleNode.setVisible(false);

        long success = createQuery(mapId).update(currentNodes)
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
                        "<create/>" +
                        "<modify>" +
                            "<relation id=\"" + relationIdsArr[0] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                            "</relation>" +
                        "</modify>" +
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
    public void testUploadModifyRelationWithInvisibleWay() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // make one of relation way members invisible
        CurrentWays invisibleWay = createQuery(mapId)
                .select(currentWays)
                .from(currentWays)
                .where(currentWays.id.eq(wayIdsArr[0]))
                .fetchOne();

        assertNotNull(invisibleWay);

        invisibleWay.setVisible(false);

        long success = createQuery(mapId).update(currentWays)
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
                        "<create/>" +
                        "<modify>" +
                            "<relation id=\"" + relationIdsArr[0] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[0] + "\"></member>" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                            "</relation>" +
                        "</modify>" +
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
    public void testUploadModifyRelationWithInvisibleRelation() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // make one of relation's members invisible
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

        // Try to upload a relation which references an invisible relation. The
        // request should fail and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create/>" +
                        "<modify>" +
                            "<relation id=\"" + relationIdsArr[1] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[0] + "\"></member>" +
                                "<member type=\"relation\" role=\"role1\" ref=\"" + relationIdsArr[0] + "\"></member>" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                            "</relation>" +
                        "</modify>" +
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
    public void testUploadModifyDuplicateWayIds() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Try to upload two ways with the same ID. The request should fail and
        // no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create/>" +
                        "<modify>" +
                            "<way id=\"" + wayIdsArr[0] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                            "</way>" +
                            "<way id=\"" + wayIdsArr[0] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                "<nd ref=\"" + nodeIdsArr[2] + "\"></nd>" +
                            "</way>" +
                        "</modify>" +
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
    public void testUploadModifyInvalidChangesetIdInWay() throws Exception {
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
                        "<create/>" +
                        "<modify>" +
                            "<way id=\"" + wayIdsArr[0] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" + "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                            "</way>" +
                            "<way id=\"" + wayIdsArr[1] + "\" version=\"1\" changeset=\"" + (changesetId + 1) + "\" >" +
                                "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                "<nd ref=\"" + nodeIdsArr[2] + "\"></nd>" +
                            "</way>" +
                        "</modify>" +
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
    public void testUploadModifyInvalidChangesetIdInRelation() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // Upload a changeset where one of the elements in the changeset lists a
        // changeset ID that doesn't match the changeset ID specified in the URL. A failure should
        // occur and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create/>" +
                        "<modify>" +
                            "<relation id=\"" + relationIdsArr[0] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                            "</relation>" +
                            "<relation id=\"" + relationIdsArr[1] + "\" version=\"1\" changeset=\"" + (changesetId + 1) + "\" >" +
                                "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[0] + "\"></member>" +
                                "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[1] + "\"></member>" +
                            "</relation>" +
                        "</modify>" +
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
    public void testUploadModifyMissingWayTagValue() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Upload a changeset where one of the tags specified has no value
        // attribute. A failure should occur and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create/>" +
                        "<modify>" +
                            "<way id=\"" + wayIdsArr[0] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                            "</way>" +
                            "<way id=\"" + wayIdsArr[1] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                "<nd ref=\"" + nodeIdsArr[2] + "\"></nd>" +
                                "<tag k=\"name 1\"/>" + // missing tag value
                            "</way>" +
                        "</modify>" +
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
    public void testUploadModifyMissingRelationTagValue() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // Upload a changeset where one of the element tags specified has no
        // value attribute. A failure should occur and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create/>" +
                        "<modify>" +
                            "<relation id=\"" + relationIdsArr[0] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                            "</relation>" +
                            "<relation id=\"" + relationIdsArr[1] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                "<tag k=\"name 1\"/>" + // missing tag value
                            "</relation>" +
                        "</modify>" +
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
    public void testUploadModifyDuplicateWayNodeIds() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);

        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Try to upload a way which lists the same node twice. The request
        // should fail and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create/>" +
                        "<modify>" +
                            "<way id=\"" + wayIdsArr[0] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                            "</way>" +
                            "<way id=\"" + wayIdsArr[0] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                            "</way>" +
                        "</modify>" +
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
    public void testUploadModifyWayEmptyNodeId() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Update the changeset where one of the ways has a node with an empty
        // string for its ID. A failure should occur and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create/>" +
                        "<modify>" +
                            "<way id=\"" + wayIdsArr[0] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" + "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                            "</way>" +
                            "<way id=\"" + wayIdsArr[1] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                "<nd ref=\"\"></nd>" +
                                "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                            "</way>" +
                        "</modify>" +
                        "<delete if-unused=\"true\"/>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (BadRequestException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.BAD_REQUEST, Response.Status.fromStatusCode(r.getStatus()));
            // TODO: needed?
            // Assert.assertTrue(r.getEntity(String.class).contains("Invalid version"));

            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test(expected = BadRequestException.class)
    @Category(UnitTest.class)
    public void testUploadModifyWayMissingNodeId() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);

        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Update the changeset where one of the ways has a node with no ID
        // attribute. A failure should occur and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create/>" +
                        "<modify>" +
                            "<way id=\"" + wayIdsArr[0] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                "<nd></nd>" + // missing node ID
                                "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                            "</way>" +
                            "<way id=\"" + wayIdsArr[1] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                "<nd ref=\"\"></nd>" +
                                "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                            "</way>" +
                        "</modify>" +
                        "<delete if-unused=\"true\"/>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (BadRequestException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.BAD_REQUEST, Response.Status.fromStatusCode(r.getStatus()));
            // TODO: needed?
            // Assert.assertTrue(r.getEntity(String.class).contains("Invalid version"));
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test(expected = BadRequestException.class)
    @Category(UnitTest.class)
    public void testUploadModifyDuplicateRelationIds() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // Try to upload two relations with the same ID. The request should fail
        // and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create/>" +
                        "<modify>" +
                            "<relation id=\"" + relationIdsArr[0] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[0] + "\"></member>" +
                            "</relation>" +
                            "<relation id=\"" + relationIdsArr[0] + "\" version=\"1\" changeset=\"" +
                                changesetId + "\" >" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[2] + "\"></member>" +
                            "</relation>" +
                        "</modify>" +
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
    public void testUploadModifyRelationReferencingItself() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // Update the changeset with a relation that references itself as a
        // member. A failure should occur and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create/>" +
                        "<modify>" +
                            "<relation id=\"" + relationIdsArr[0] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                "<member type=\"relation\" role=\"role1\" ref=\"" + relationIdsArr[0] + "\"></member>" +
                                "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[1] + "\"></member>" +
                            "</relation>" +
                            "<relation id=\"" + relationIdsArr[1] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[2] + "\"></member>" +
                                "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[1] + "\"></member>" +
                            "</relation>" +
                        "</modify>" +
                        "<delete if-unused=\"true\"/>" +
                    "</osmChange>", MediaType.TEXT_XML_TYPE), Document.class);
        }
        catch (ClientErrorException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.CONFLICT, Response.Status.fromStatusCode(r.getStatus()));
            assertTrue(r.readEntity(String.class).contains("contains a relation member that references itself"));
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test(expected = BadRequestException.class)
    @Category(UnitTest.class)
    public void testUploadModifyRelationEmptyMemberId() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // Update the changeset where one of the relations has a member with an
        // empty string for an ID. A failure should occur and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create/>" +
                        "<modify>" +
                        "<relation id=\"" + relationIdsArr[0] + "\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                            "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                            "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                        "</relation>" +
                        "<relation id=\"" + relationIdsArr[1] + "\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                            "<member type=\"node\" role=\"role1\" ref=\"\"></member>" + // empty member id
                            "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                        "</relation>" +
                        "</modify>" +
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
    public void testUploadModifyRelationMissingMemberId() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // Update the changeset where one of the relations has a member with an
        // empty string for an ID.  A failure should occur and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create/>" +
                        "<modify>" +
                            "<relation id=\"" + relationIdsArr[0] + "\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                            "</relation>" +
                            "<relation id=\"" + relationIdsArr[1] + "\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                "<member type=\"node\" role=\"role1\"></member>" + // missing member ID
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                            "</relation>" +
                        "</modify>" +
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
    public void testUploadModifyRelationInvalidMemberType() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // Update the changeset where one of the relations has a member with an
        // empty string for one of its member's type. A failure should occur and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create/>" +
                        "<modify>" +
                            "<relation id=\"" + relationIdsArr[0] + "\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                 "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                 // invalid member type
                                 "<member type=\"blah\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                            "</relation>" +
                            "<relation id=\"" + relationIdsArr[1] + "\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                            "</relation>" +
                        "</modify>" +
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
    public void testUploadModifyRelationMissingMemberType() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // Update the changeset where one of the relations has a member with no
        // type. A failure should occur and no data in the system should be modified.
        try {
            target("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_XML)
                .post(Entity.entity(
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create/>" +
                        "<modify>" +
                            "<relation id=\"" + relationIdsArr[0] + "\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                // missing member type
                                "<member role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                            "</relation>" +
                        "<relation id=\"" + relationIdsArr[1] + "\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                            "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                            "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                        "</relation>" +
                        "</modify>" +
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
}
