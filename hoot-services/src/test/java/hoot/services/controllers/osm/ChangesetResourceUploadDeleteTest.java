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

import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.HashMap;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import javax.ws.rs.core.MediaType;
import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathExpressionException;

import org.apache.commons.lang3.StringUtils;
import org.apache.xpath.XPathAPI;
import org.junit.Assert;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.postgresql.util.PGobject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.w3c.dom.Document;
import org.w3c.dom.NodeList;

import com.mysema.query.sql.SQLQuery;
import com.sun.jersey.api.client.ClientResponse;
import com.sun.jersey.api.client.ClientResponse.Status;
import com.sun.jersey.api.client.UniformInterfaceException;

import hoot.services.HootProperties;
import hoot.services.UnitTest;
import hoot.services.db.DbUtils;
import hoot.services.db.postgres.PostgresUtils;
import hoot.services.db2.Changesets;
import hoot.services.db2.CurrentNodes;
import hoot.services.db2.CurrentRelationMembers;
import hoot.services.db2.CurrentRelations;
import hoot.services.db2.CurrentWayNodes;
import hoot.services.db2.CurrentWays;
import hoot.services.db2.QChangesets;
import hoot.services.db2.QCurrentNodes;
import hoot.services.db2.QCurrentRelationMembers;
import hoot.services.db2.QCurrentRelations;
import hoot.services.db2.QCurrentWayNodes;
import hoot.services.db2.QCurrentWays;
import hoot.services.geo.BoundingBox;
import hoot.services.geo.QuadTileCalculator;
import hoot.services.models.osm.Changeset;
import hoot.services.models.osm.Element.ElementType;
import hoot.services.models.osm.Node;
import hoot.services.models.osm.RelationMember;
import hoot.services.osm.OsmResourceTestAbstract;
import hoot.services.osm.OsmTestUtils;
import hoot.services.utils.XmlUtils;


public class ChangesetResourceUploadDeleteTest extends OsmResourceTestAbstract {
    private static final Logger log = LoggerFactory.getLogger(ChangesetResourceUploadDeleteTest.class);

    private final QCurrentNodes currentNodesTbl = QCurrentNodes.currentNodes;
    private final QCurrentWays currentWaysTbl = QCurrentWays.currentWays;
    private final QCurrentWayNodes currentWayNodesTbl = QCurrentWayNodes.currentWayNodes;
    private final QCurrentRelations currentRelationsTbl = QCurrentRelations.currentRelations;
    private final QCurrentRelationMembers currentRelationMembersTbl = QCurrentRelationMembers.currentRelationMembers;

    public ChangesetResourceUploadDeleteTest() {
        super("hoot.services.controllers.osm");
    }

    private void testUploadDelete(String request, BoundingBox originalBounds, long changesetId, Long[] nodeIdsArr,
            Long[] wayIdsArr, Long[] relationIdsArr) throws Exception {
        try {
            Document responseData = null;
            try {
                responseData = resource()
                        .path("api/0.6/changeset/" + changesetId + "/upload")
                        .queryParam("mapId", "" + mapId)
                        .type(MediaType.TEXT_XML)
                        .accept(MediaType.TEXT_XML)
                        .post(Document.class, request);
            }
            catch (UniformInterfaceException e) {
                ClientResponse r = e.getResponse();
                Assert.fail("Unexpected response " + r.getStatus() + " " + r.getEntity(String.class));
            }
            Assert.assertNotNull(responseData);

            XPath xpath = XmlUtils.createXPath();
            try {
                NodeList returnedNodes = XPathAPI.selectNodeList(responseData, "//osm/diffResult/node");
                Assert.assertEquals(1, returnedNodes.getLength());

                // check that the returned IDs are correct and the correct
                // attributes were returned
                Assert.assertEquals((long) nodeIdsArr[3],
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@old_id", responseData)));
                org.w3c.dom.Node xml = XPathAPI.selectSingleNode(responseData, "//osm/diffResult/node[1]");
                Assert.assertNull(xml.getAttributes().getNamedItem("new_id"));
                Assert.assertNull(xml.getAttributes().getNamedItem("new_version"));

                NodeList returnedWays = XPathAPI.selectNodeList(responseData, "//osm/diffResult/way");
                Assert.assertEquals(1, returnedWays.getLength());

                Assert.assertEquals((long) wayIdsArr[2],
                        Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@old_id", responseData)));
                xml = XPathAPI.selectSingleNode(responseData, "//osm/diffResult/way[1]");
                Assert.assertNull(xml.getAttributes().getNamedItem("new_id"));
                Assert.assertNull(xml.getAttributes().getNamedItem("new_version"));

                NodeList returnedRelations = XPathAPI.selectNodeList(responseData, "//osm/diffResult/relation");
                Assert.assertEquals(1, returnedRelations.getLength());

                Assert.assertEquals((long) relationIdsArr[2],
                        Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@old_id", responseData)));
                xml = XPathAPI.selectSingleNode(responseData, "//osm/diffResult/relation[1]");
                Assert.assertNull(xml.getAttributes().getNamedItem("new_id"));
                Assert.assertNull(xml.getAttributes().getNamedItem("new_version"));
            }
            catch (XPathExpressionException e) {
                Assert.fail("Error parsing response document: " + e.getMessage());
            }

            Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());

            QChangesets changesets = QChangesets.changesets;
            Changesets changeset = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                    .from(changesets)
                    .where(changesets.id.eq(changesetId))
                    .singleResult(changesets);
            try {
                Map<Long, CurrentNodes> nodes = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentNodesTbl)
                        .map(currentNodesTbl.id, currentNodesTbl);

                Assert.assertEquals(4, nodes.size());

                CurrentNodes nodeRecord = nodes.get(nodeIdsArr[0]);
                Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
                Assert.assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
                Assert.assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
                Assert.assertEquals(nodeIdsArr[0], nodeRecord.getId());
                Assert.assertEquals(
                        new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                        nodeRecord.getTile());
                Assert.assertTrue(nodeRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), nodeRecord.getVersion());
                Assert.assertTrue(nodeRecord.getVisible());
                Map<String, String> tags = PostgresUtils.postgresObjToHStore((PGobject) nodeRecord.getTags());
                Assert.assertNotNull(tags);
                Assert.assertEquals(2, tags.size());
                Assert.assertEquals("val 1", tags.get("key 1"));
                Assert.assertEquals("val 2", tags.get("key 2"));

                nodeRecord = nodes.get(nodeIdsArr[1]);
                Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
                Assert.assertEquals(new Double(originalBounds.getMaxLat()), nodeRecord.getLatitude());
                Assert.assertEquals(new Double(originalBounds.getMaxLon()), nodeRecord.getLongitude());
                Assert.assertEquals(nodeIdsArr[1], nodeRecord.getId());
                Assert.assertEquals(
                        new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                        nodeRecord.getTile());
                Assert.assertTrue(nodeRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), nodeRecord.getVersion());
                Assert.assertTrue(nodeRecord.getVisible());
                Assert.assertTrue((nodeRecord.getTags() == null)
                        || StringUtils.isEmpty(((PGobject) nodeRecord.getTags()).getValue()));

                nodeRecord = nodes.get(nodeIdsArr[2]);
                Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
                Assert.assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
                Assert.assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
                Assert.assertEquals(nodeIdsArr[2], nodeRecord.getId());
                Assert.assertEquals(
                        new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                        nodeRecord.getTile());
                Assert.assertTrue(nodeRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), nodeRecord.getVersion());
                Assert.assertTrue(nodeRecord.getVisible());
                Assert.assertTrue((nodeRecord.getTags() == null)
                        || StringUtils.isEmpty(((PGobject) nodeRecord.getTags()).getValue()));

                nodeRecord = nodes.get(nodeIdsArr[4]);
                Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
                Assert.assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
                Assert.assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
                Assert.assertEquals(nodeIdsArr[4], nodeRecord.getId());
                Assert.assertEquals(
                        new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                        nodeRecord.getTile());
                Assert.assertTrue(nodeRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), nodeRecord.getVersion());
                Assert.assertTrue(nodeRecord.getVisible());
                tags = PostgresUtils.postgresObjToHStore((PGobject) nodeRecord.getTags());
                Assert.assertNotNull(tags);
                Assert.assertEquals(1, tags.size());
                Assert.assertEquals("val 4", tags.get("key 4"));

                // verify the deleted node
                nodeRecord = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentNodesTbl)
                        .where(currentNodesTbl.id.eq(nodeIdsArr[3]))
                        .singleResult(currentNodesTbl);
                Assert.assertNull(nodeRecord);
            }
            catch (Exception e) {
                Assert.fail("Error checking nodes: " + e.getMessage());
            }

            try {
                final Map<Long, CurrentWays> ways = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentWaysTbl).map(currentWaysTbl.id, currentWaysTbl);

                Assert.assertEquals(2, ways.size());

                CurrentWays wayRecord = ways.get(wayIdsArr[0]);
                Assert.assertEquals(new Long(changesetId), wayRecord.getChangesetId());
                Assert.assertEquals(wayIdsArr[0], wayRecord.getId());
                Assert.assertTrue(wayRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), wayRecord.getVersion());
                Assert.assertTrue(wayRecord.getVisible());
                List<CurrentWayNodes> wayNodes = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentWayNodesTbl)
                        .where(currentWayNodesTbl.wayId.eq(wayIdsArr[0]))
                        .orderBy(currentWayNodesTbl.sequenceId.asc())
                        .list(currentWayNodesTbl);
                Assert.assertEquals(3, wayNodes.size());
                CurrentWayNodes wayNode = wayNodes.get(0);
                Assert.assertEquals(nodeIdsArr[0], wayNode.getNodeId());
                Assert.assertEquals(new Long(1), wayNode.getSequenceId());
                Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());
                wayNode = wayNodes.get(1);
                Assert.assertEquals(nodeIdsArr[1], wayNode.getNodeId());
                Assert.assertEquals(new Long(2), wayNode.getSequenceId());
                Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());
                wayNode = wayNodes.get(2);
                Assert.assertEquals(nodeIdsArr[4], wayNode.getNodeId());
                Assert.assertEquals(new Long(3), wayNode.getSequenceId());
                Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());
                // verify the previously existing tags
                Map<String, String> tags = PostgresUtils.postgresObjToHStore((PGobject) wayRecord.getTags());
                Assert.assertNotNull(tags);
                Assert.assertEquals(2, tags.size());
                Assert.assertEquals("val 1", tags.get("key 1"));
                Assert.assertEquals("val 2", tags.get("key 2"));

                wayRecord = ways.get(wayIdsArr[1]);
                Assert.assertEquals(new Long(changesetId), wayRecord.getChangesetId());
                Assert.assertEquals(wayIdsArr[1], wayRecord.getId());
                Assert.assertTrue(wayRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), wayRecord.getVersion());
                Assert.assertTrue(wayRecord.getVisible());
                wayNodes = new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentWayNodesTbl)
                        .where(currentWayNodesTbl.wayId.eq(wayIdsArr[1]))
                        .orderBy(currentWayNodesTbl.sequenceId.asc())
                        .list(currentWayNodesTbl);
                Assert.assertEquals(2, wayNodes.size());
                wayNode = wayNodes.get(0);
                Assert.assertEquals(nodeIdsArr[2], wayNode.getNodeId());
                Assert.assertEquals(new Long(1), wayNode.getSequenceId());
                Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());
                wayNode = wayNodes.get(1);
                Assert.assertEquals(nodeIdsArr[1], wayNode.getNodeId());
                Assert.assertEquals(new Long(2), wayNode.getSequenceId());
                Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());
                // verify the way with no tags
                Assert.assertTrue((wayRecord.getTags() == null)
                        || StringUtils.isEmpty(((PGobject) wayRecord.getTags()).getValue()));

                // verify the deleted way
                Assert.assertNull(new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentWaysTbl)
                        .where(currentWaysTbl.id.eq(wayIdsArr[2]))
                        .singleResult(currentWaysTbl));
            }
            catch (Exception e) {
                Assert.fail("Error checking ways: " + e.getMessage());
            }

            try {
                Map<Long, CurrentRelations> relations = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentRelationsTbl)
                        .map(currentRelationsTbl.id, currentRelationsTbl);

                Assert.assertEquals(3, relations.size());

                // verify the previously existing relations
                CurrentRelations relationRecord = relations.get(relationIdsArr[0]);
                Assert.assertEquals(new Long(changesetId), relationRecord.getChangesetId());
                Assert.assertEquals(relationIdsArr[0], relationRecord.getId());
                Assert.assertTrue(relationRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), relationRecord.getVersion());
                Assert.assertTrue(relationRecord.getVisible());

                List<CurrentRelationMembers> members = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentRelationMembersTbl)
                        .where(currentRelationMembersTbl.relationId.eq(relationIdsArr[0]))
                        .orderBy(currentRelationMembersTbl.sequenceId.asc())
                        .list(currentRelationMembersTbl);

                Assert.assertEquals(4, members.size());
                CurrentRelationMembers member = members.get(0);
                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
                Assert.assertEquals("role1", member.getMemberRole());
                Assert.assertEquals(new Integer(1), member.getSequenceId());

                Assert.assertEquals(nodeIdsArr[0], member.getMemberId());
                member = members.get(1);
                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.way, member.getMemberType());
                Assert.assertEquals("role3", member.getMemberRole());
                Assert.assertEquals(new Integer(2), member.getSequenceId());

                Assert.assertEquals(wayIdsArr[1], member.getMemberId());
                member = members.get(2);
                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.way, member.getMemberType());
                Assert.assertEquals("role2", member.getMemberRole());
                Assert.assertEquals(new Integer(3), member.getSequenceId());

                Assert.assertEquals(wayIdsArr[0], member.getMemberId());
                member = members.get(3);
                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
                Assert.assertEquals("", member.getMemberRole());
                Assert.assertEquals(new Integer(4), member.getSequenceId());

                Assert.assertEquals(nodeIdsArr[2], member.getMemberId());
                // verify the previously existing tags
                Map<String, String> tags = PostgresUtils.postgresObjToHStore((PGobject) relationRecord.getTags());
                Assert.assertNotNull(tags);
                Assert.assertEquals(1, tags.size());
                Assert.assertEquals("val 1", tags.get("key 1"));

                relationRecord = relations.get(relationIdsArr[1]);
                Assert.assertEquals(new Long(changesetId), relationRecord.getChangesetId());
                Assert.assertEquals(relationIdsArr[1], relationRecord.getId());

                Assert.assertTrue(relationRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), relationRecord.getVersion());
                Assert.assertTrue(relationRecord.getVisible());

                members = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentRelationMembersTbl)
                        .where(currentRelationMembersTbl.relationId.eq(relationIdsArr[1]))
                        .orderBy(currentRelationMembersTbl.sequenceId.asc())
                        .list(currentRelationMembersTbl);

                Assert.assertEquals(2, members.size());
                member = members.get(0);
                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
                Assert.assertEquals("role1", member.getMemberRole());
                Assert.assertEquals(new Integer(1), member.getSequenceId());

                Assert.assertEquals(nodeIdsArr[4], member.getMemberId());
                member = members.get(1);
                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.relation, member.getMemberType());
                Assert.assertEquals("role1", member.getMemberRole());
                Assert.assertEquals(new Integer(2), member.getSequenceId());

                Assert.assertEquals(relationIdsArr[0], member.getMemberId());
                tags = PostgresUtils.postgresObjToHStore((PGobject) relationRecord.getTags());
                Assert.assertNotNull(tags);
                Assert.assertEquals(2, tags.size());
                Assert.assertEquals("val 2", tags.get("key 2"));
                Assert.assertEquals("val 3", tags.get("key 3"));

                relationRecord = relations.get(relationIdsArr[3]);
                Assert.assertEquals(new Long(changesetId), relationRecord.getChangesetId());
                Assert.assertEquals(relationIdsArr[3], relationRecord.getId());

                Assert.assertTrue(relationRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), relationRecord.getVersion());
                Assert.assertTrue(relationRecord.getVisible());

                members = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentRelationMembersTbl)
                        .where(currentRelationMembersTbl.relationId.eq(relationIdsArr[3]))
                        .orderBy(currentRelationMembersTbl.sequenceId.asc())
                        .list(currentRelationMembersTbl);

                Assert.assertEquals(1, members.size());
                member = members.get(0);
                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
                Assert.assertEquals("role1", member.getMemberRole());
                Assert.assertEquals(new Integer(1), member.getSequenceId());

                Assert.assertEquals(nodeIdsArr[2], member.getMemberId());

                // verify the deleted relation
                Assert.assertNull(
                        new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                                .from(currentRelationsTbl)
                                .where(currentRelationsTbl.id.eq(relationIdsArr[2]))
                                .singleResult(currentRelationsTbl));

                // verify tags that should no longer exist
                Assert.assertTrue((relationRecord.getTags() == null)
                        || StringUtils.isEmpty(((PGobject) relationRecord.getTags()).getValue()));
            }
            catch (Exception e) {
                Assert.fail("Error checking relations: " + e.getMessage());
            }

            try {
                changeset = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(changesets)
                        .where(changesets.id.eq(changesetId))
                        .singleResult(changesets);

                Assert.assertNotNull(changeset);
                Assert.assertTrue(changeset.getCreatedAt().before(now));
                Assert.assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));
                Assert.assertEquals(new Integer(15), changeset.getNumChanges());
                Assert.assertEquals(new Long(userId), changeset.getUserId());

                BoundingBox expandedBounds = new BoundingBox(originalBounds);
                expandedBounds.expand(originalBounds,
                        Double.parseDouble(HootProperties.getDefault("changesetBoundsExpansionFactorDeegrees")));
                Changeset hootChangeset = new Changeset(mapId, changesetId, conn);
                BoundingBox changesetBounds = hootChangeset.getBounds();
                Assert.assertEquals(changesetBounds, expandedBounds);
            }
            catch (Exception e) {
                Assert.fail("Error checking updated changeset: " + e.getMessage());
            }
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testUploadDelete() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
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
        try {
            BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
            long changesetId = OsmTestUtils.createTestChangeset(originalBounds, 8);
            Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
            Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
            Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
            Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);

            // Now delete two of the nodes, where ways are specified in the
            // modify portion of the request before the nodes.
            Document responseData = null;
            try {
                responseData = resource().path("api/0.6/changeset/" + changesetId + "/upload")
                        .queryParam("mapId", String.valueOf(mapId))
                        .type(MediaType.TEXT_XML)
                        .accept(MediaType.TEXT_XML)
                        .post(Document.class,
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
                            "</osmChange>");
            }
            catch (UniformInterfaceException e) {
                ClientResponse r = e.getResponse();
                Assert.fail("Unexpected response " + r.getStatus() + " " + r.getEntity(String.class));
            }
            Assert.assertNotNull(responseData);

            XPath xpath = XmlUtils.createXPath();
            try {
                NodeList returnedNodes = XPathAPI.selectNodeList(responseData, "//osm/diffResult/node");
                Assert.assertEquals(1, returnedNodes.getLength());

                // check that the returned IDs are correct and the correct attributes were returned
                Assert.assertEquals((long) nodeIdsArr[4],
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@old_id", responseData)));
                org.w3c.dom.Node xml = XPathAPI.selectSingleNode(responseData, "//osm/diffResult/node[1]");
                Assert.assertNull(xml.getAttributes().getNamedItem("new_id"));
                Assert.assertNull(xml.getAttributes().getNamedItem("new_version"));
            }
            catch (XPathExpressionException e) {
                Assert.fail("Error parsing response document: " + e.getMessage());
            }

            Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
            QChangesets changesets = QChangesets.changesets;
            Changesets changeset = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                    .from(changesets)
                    .where(changesets.id.eq(changesetId))
                    .singleResult(changesets);

            try {
                Map<Long, CurrentNodes> nodes = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentNodesTbl).map(currentNodesTbl.id, currentNodesTbl);

                Assert.assertEquals(4, nodes.size());

                CurrentNodes nodeRecord = nodes.get(nodeIdsArr[0]);

                Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
                Assert.assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
                Assert.assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
                Assert.assertEquals(nodeIdsArr[0], nodeRecord.getId());
                Assert.assertEquals(
                        new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                        nodeRecord.getTile());
                Assert.assertTrue(nodeRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), nodeRecord.getVersion());
                Assert.assertTrue(nodeRecord.getVisible());

                Map<String, String> tags = PostgresUtils.postgresObjToHStore((PGobject) nodeRecord.getTags());

                Assert.assertNotNull(tags);
                Assert.assertEquals(2, tags.size());
                Assert.assertEquals("val 1", tags.get("key 1"));
                Assert.assertEquals("val 2", tags.get("key 2"));

                nodeRecord = nodes.get(nodeIdsArr[1]);

                Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
                Assert.assertEquals(new Double((originalBounds.getMaxLat())), nodeRecord.getLatitude());
                Assert.assertEquals(new Double((originalBounds.getMaxLon())), nodeRecord.getLongitude());
                Assert.assertEquals(nodeIdsArr[1], nodeRecord.getId());
                Assert.assertEquals(
                        new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                        nodeRecord.getTile());
                Assert.assertTrue(nodeRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), nodeRecord.getVersion());
                Assert.assertTrue(nodeRecord.getVisible());
                Assert.assertTrue((nodeRecord.getTags() == null)
                        || StringUtils.isEmpty(((PGobject) nodeRecord.getTags()).getValue()));

                nodeRecord = nodes.get(nodeIdsArr[2]);

                Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
                Assert.assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
                Assert.assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
                Assert.assertEquals(nodeIdsArr[2], nodeRecord.getId());
                Assert.assertEquals(
                        new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                        nodeRecord.getTile());
                Assert.assertTrue(nodeRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), nodeRecord.getVersion());
                Assert.assertEquals(true, nodeRecord.getVisible());
                Assert.assertTrue((nodeRecord.getTags() == null)
                        || StringUtils.isEmpty(((PGobject) nodeRecord.getTags()).getValue()));

                nodeRecord = nodes.get(nodeIdsArr[2]);

                Assert.assertTrue((nodeRecord.getTags() == null)
                        || StringUtils.isEmpty(((PGobject) nodeRecord.getTags()).getValue()));

                nodeRecord = nodes.get(nodeIdsArr[3]);

                Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
                Assert.assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
                Assert.assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
                Assert.assertEquals(nodeIdsArr[3], nodeRecord.getId());
                Assert.assertEquals(
                        new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                        nodeRecord.getTile());
                Assert.assertTrue(nodeRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), nodeRecord.getVersion());
                Assert.assertTrue(nodeRecord.getVisible());

                tags = PostgresUtils.postgresObjToHStore((PGobject) nodeRecord.getTags());

                Assert.assertNotNull(tags);
                Assert.assertEquals(1, tags.size());
                Assert.assertEquals("val 3", tags.get("key 3"));

                // verify the deleted node
                nodeRecord = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentNodesTbl)
                        .where(currentNodesTbl.id.eq(nodeIdsArr[4]))
                        .singleResult(currentNodesTbl);

                Assert.assertNull(nodeRecord);
            }
            catch (Exception e) {
                Assert.fail("Error checking nodes: " + e.getMessage());
            }

            try {
                Map<Long, CurrentWays> ways = new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentWaysTbl)
                        .map(currentWaysTbl.id, currentWaysTbl);

                Assert.assertEquals(2, ways.size());

                CurrentWays wayRecord = ways.get(wayIdsArr[1]);

                Assert.assertEquals(new Long(changesetId), wayRecord.getChangesetId());
                Assert.assertEquals(wayIdsArr[1], wayRecord.getId());
                Assert.assertTrue(wayRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), wayRecord.getVersion());
                Assert.assertTrue(wayRecord.getVisible());

                List<CurrentWayNodes> wayNodes = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentWayNodesTbl)
                        .where(currentWayNodesTbl.wayId.eq(wayIdsArr[1]))
                        .orderBy(currentWayNodesTbl.sequenceId.asc())
                        .list(currentWayNodesTbl);

                Assert.assertEquals(2, wayNodes.size());

                CurrentWayNodes wayNode = wayNodes.get(0);

                Assert.assertEquals(nodeIdsArr[2], wayNode.getNodeId());
                Assert.assertEquals(new Long(1), wayNode.getSequenceId());
                Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());

                wayNode = wayNodes.get(1);

                Assert.assertEquals(nodeIdsArr[1], wayNode.getNodeId());
                Assert.assertEquals(new Long(2), wayNode.getSequenceId());
                Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());

                // verify the way with no tags
                Assert.assertTrue((wayRecord.getTags() == null)
                        || StringUtils.isEmpty(((PGobject) wayRecord.getTags()).getValue()));

                wayRecord = ways.get(wayIdsArr[2]);

                Assert.assertEquals(new Long(changesetId), wayRecord.getChangesetId());
                Assert.assertEquals(wayIdsArr[2], wayRecord.getId());
                Assert.assertTrue(wayRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), wayRecord.getVersion());
                Assert.assertTrue(wayRecord.getVisible());

                wayNodes = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentWayNodesTbl)
                        .where(currentWayNodesTbl.wayId.eq(wayIdsArr[2]))
                        .orderBy(currentWayNodesTbl.sequenceId.asc())
                        .list(currentWayNodesTbl);

                Assert.assertEquals(2, wayNodes.size());

                wayNode = wayNodes.get(0);

                Assert.assertEquals(nodeIdsArr[0], wayNode.getNodeId());
                Assert.assertEquals(new Long(1), wayNode.getSequenceId());
                Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());

                wayNode = wayNodes.get(1);

                Assert.assertEquals(nodeIdsArr[1], wayNode.getNodeId());
                Assert.assertEquals(new Long(2), wayNode.getSequenceId());
                Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());

                // verify the previously existing tags
                Map<String, String> tags = PostgresUtils.postgresObjToHStore((PGobject) wayRecord.getTags());

                Assert.assertNotNull(tags);
                Assert.assertEquals(1, tags.size());
                Assert.assertEquals("val 3", tags.get("key 3"));

                // verify the deleted way
                Assert.assertNull(new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentWaysTbl)
                        .where(currentWaysTbl.id.eq(wayIdsArr[0]))
                        .singleResult(currentWaysTbl));
            }
            catch (Exception e) {
                Assert.fail("Error checking ways: " + e.getMessage());
            }

            try {
                changeset = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(changesets)
                        .where(changesets.id.eq(changesetId))
                        .singleResult(changesets);

                Assert.assertNotNull(changeset);
                Assert.assertTrue(changeset.getCreatedAt().before(now));
                Assert.assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));
                Assert.assertEquals(new Integer(10), changeset.getNumChanges());
                Assert.assertEquals(new Long(userId), changeset.getUserId());

                BoundingBox expandedBounds = new BoundingBox(originalBounds);
                expandedBounds.expand(originalBounds,
                        Double.parseDouble(HootProperties.getDefault("changesetBoundsExpansionFactorDeegrees")));
                Changeset hootChangeset = new Changeset(mapId, changesetId, conn);
                BoundingBox changesetBounds = hootChangeset.getBounds();
                Assert.assertEquals(changesetBounds, expandedBounds);
            }
            catch (Exception e) {
                Assert.fail("Error checking updated changeset: " + e.getMessage());
            }
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadDeleteNoNodeCoords() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        try {
            // Now, delete one of the nodes, one of the ways, and one of the
            // relations. An error should be returned and no data in the system deleted, since we require
            // passing in nodes with their coords...even for a delete.
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
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
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
            // Assert.assertTrue(
            // r.getEntity(String.class).contains("Element(s) being referenced
            // don't exist."));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testUploadDeleteNoWayNodes() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
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
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
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
        try {
            BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
            long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
            Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
            Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
            Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
            Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
            Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
            Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

            // now delete one of the nodes, one of the ways, and one of the relations
            Document responseData = null;
            try {
                responseData = resource()
                        .path("api/0.6/changeset/" + changesetId + "/upload")
                        .queryParam("mapId", String.valueOf(mapId))
                        .type(MediaType.TEXT_XML)
                        .accept(MediaType.TEXT_XML)
                        .post(Document.class,
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
                            "</osmChange>");
            }
            catch (UniformInterfaceException e) {
                ClientResponse r = e.getResponse();
                Assert.fail("Unexpected response " + r.getStatus() + " " + r.getEntity(String.class));
            }
            Assert.assertNotNull(responseData);

            XPath xpath = XmlUtils.createXPath();
            try {
                NodeList returnedNodes = XPathAPI.selectNodeList(responseData, "//osm/diffResult/node");
                Assert.assertEquals(1, returnedNodes.getLength());

                // check that the returned IDs are correct and the correct attributes were returned
                Assert.assertEquals((long) nodeIdsArr[3],
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@old_id", responseData)));
                org.w3c.dom.Node xml = XPathAPI.selectSingleNode(responseData, "//osm/diffResult/node[1]");
                Assert.assertNull(xml.getAttributes().getNamedItem("new_id"));
                Assert.assertNull(xml.getAttributes().getNamedItem("new_version"));

                NodeList returnedWays = XPathAPI.selectNodeList(responseData, "//osm/diffResult/way");
                Assert.assertEquals(1, returnedWays.getLength());

                Assert.assertEquals((long) wayIdsArr[2],
                        Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@old_id", responseData)));
                xml = XPathAPI.selectSingleNode(responseData, "//osm/diffResult/way[1]");
                Assert.assertNull(xml.getAttributes().getNamedItem("new_id"));
                Assert.assertNull(xml.getAttributes().getNamedItem("new_version"));

                NodeList returnedRelations = XPathAPI.selectNodeList(responseData, "//osm/diffResult/relation");
                Assert.assertEquals(1, returnedRelations.getLength());

                Assert.assertEquals((long) relationIdsArr[1],
                        Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@old_id", responseData)));
                xml = XPathAPI.selectSingleNode(responseData, "//osm/diffResult/relation[1]");
                Assert.assertNull(xml.getAttributes().getNamedItem("new_id"));
                Assert.assertNull(xml.getAttributes().getNamedItem("new_version"));
            }
            catch (XPathExpressionException e) {
                Assert.fail("Error parsing response document: " + e.getMessage());
            }

            Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
            QChangesets changesets = QChangesets.changesets;
            Changesets changeset = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                    .from(changesets)
                    .where(changesets.id.eq(changesetId))
                    .singleResult(changesets);

            try {
                Map<Long, CurrentNodes> nodes = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentNodesTbl)
                        .map(currentNodesTbl.id, currentNodesTbl);
                Assert.assertEquals(4, nodes.size());

                CurrentNodes nodeRecord = nodes.get(nodeIdsArr[0]);
                Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
                Assert.assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
                Assert.assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
                Assert.assertEquals(nodeIdsArr[0], nodeRecord.getId());
                Assert.assertEquals(
                        new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                        nodeRecord.getTile());
                Assert.assertTrue(nodeRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), nodeRecord.getVersion());
                Assert.assertTrue(nodeRecord.getVisible());
                Map<String, String> tags = PostgresUtils.postgresObjToHStore((PGobject) nodeRecord.getTags());
                Assert.assertNotNull(tags);
                Assert.assertEquals(2, tags.size());
                Assert.assertEquals("val 1", tags.get("key 1"));
                Assert.assertEquals("val 2", tags.get("key 2"));

                nodeRecord = nodes.get(nodeIdsArr[1]);
                Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
                Assert.assertEquals(new Double(originalBounds.getMaxLat()), nodeRecord.getLatitude());
                Assert.assertEquals(new Double(originalBounds.getMaxLon()), nodeRecord.getLongitude());
                Assert.assertEquals(nodeIdsArr[1], nodeRecord.getId());
                Assert.assertEquals(
                        new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                        nodeRecord.getTile());
                Assert.assertTrue(nodeRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), nodeRecord.getVersion());
                Assert.assertTrue(nodeRecord.getVisible());
                Assert.assertTrue((nodeRecord.getTags() == null)
                        || StringUtils.isEmpty(((PGobject) nodeRecord.getTags()).getValue()));

                nodeRecord = nodes.get(nodeIdsArr[2]);
                Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
                Assert.assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
                Assert.assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
                Assert.assertEquals(nodeIdsArr[2], nodeRecord.getId());
                Assert.assertEquals(
                        new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                        nodeRecord.getTile());
                Assert.assertTrue(nodeRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), nodeRecord.getVersion());
                Assert.assertTrue(nodeRecord.getVisible());
                Assert.assertTrue((nodeRecord.getTags() == null)
                        || StringUtils.isEmpty(((PGobject) nodeRecord.getTags()).getValue()));

                nodeRecord = nodes.get(nodeIdsArr[4]);
                Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
                Assert.assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
                Assert.assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
                Assert.assertEquals(nodeIdsArr[4], nodeRecord.getId());
                Assert.assertEquals(
                        new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                        nodeRecord.getTile());
                Assert.assertTrue(nodeRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), nodeRecord.getVersion());
                Assert.assertTrue(nodeRecord.getVisible());
                tags = PostgresUtils.postgresObjToHStore((PGobject) nodeRecord.getTags());
                Assert.assertNotNull(tags);
                Assert.assertEquals(1, tags.size());
                Assert.assertEquals("val 4", tags.get("key 4"));

                // verify the deleted node
                nodeRecord = new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentNodesTbl)
                        .where(currentNodesTbl.id.eq(nodeIdsArr[3])).singleResult(currentNodesTbl);
                Assert.assertNull(nodeRecord);
            }
            catch (Exception e) {
                Assert.fail("Error checking nodes: " + e.getMessage());
            }

            try {
                Map<Long, CurrentWays> ways = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentWaysTbl)
                        .map(currentWaysTbl.id, currentWaysTbl);

                Assert.assertEquals(2, ways.size());

                CurrentWays wayRecord = ways.get(wayIdsArr[0]);
                Assert.assertEquals(new Long(changesetId), wayRecord.getChangesetId());
                Assert.assertEquals(wayIdsArr[0], wayRecord.getId());
                Assert.assertTrue(wayRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), wayRecord.getVersion());
                Assert.assertTrue(wayRecord.getVisible());

                List<CurrentWayNodes> wayNodes = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentWayNodesTbl)
                        .where(currentWayNodesTbl.wayId.eq(wayIdsArr[0]))
                        .orderBy(currentWayNodesTbl.sequenceId.asc())
                        .list(currentWayNodesTbl);

                Assert.assertEquals(3, wayNodes.size());
                CurrentWayNodes wayNode = wayNodes.get(0);
                Assert.assertEquals(nodeIdsArr[0], wayNode.getNodeId());
                Assert.assertEquals(new Long(1), wayNode.getSequenceId());
                Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());
                wayNode = wayNodes.get(1);
                Assert.assertEquals(nodeIdsArr[1], wayNode.getNodeId());
                Assert.assertEquals(new Long(2), wayNode.getSequenceId());
                Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());
                wayNode = wayNodes.get(2);
                Assert.assertEquals(nodeIdsArr[4], wayNode.getNodeId());
                Assert.assertEquals(new Long(3), wayNode.getSequenceId());
                Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());
                // verify the previously existing tags
                Map<String, String> tags = PostgresUtils.postgresObjToHStore((PGobject) wayRecord.getTags());
                Assert.assertNotNull(tags);
                Assert.assertEquals(2, tags.size());
                Assert.assertEquals("val 1", tags.get("key 1"));
                Assert.assertEquals("val 2", tags.get("key 2"));

                wayRecord = ways.get(wayIdsArr[1]);
                Assert.assertEquals(new Long(changesetId), wayRecord.getChangesetId());
                Assert.assertEquals(wayIdsArr[1], wayRecord.getId());
                Assert.assertTrue(wayRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), wayRecord.getVersion());
                Assert.assertTrue(wayRecord.getVisible());

                wayNodes = new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentWayNodesTbl)
                        .where(currentWayNodesTbl.wayId.eq(wayIdsArr[1]))
                        .orderBy(currentWayNodesTbl.sequenceId.asc())
                        .list(currentWayNodesTbl);

                Assert.assertEquals(2, wayNodes.size());
                wayNode = wayNodes.get(0);
                Assert.assertEquals(nodeIdsArr[2], wayNode.getNodeId());
                Assert.assertEquals(new Long(1), wayNode.getSequenceId());
                Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());
                wayNode = wayNodes.get(1);
                Assert.assertEquals(nodeIdsArr[1], wayNode.getNodeId());
                Assert.assertEquals(new Long(2), wayNode.getSequenceId());
                Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());
                // verify the way with no tags
                Assert.assertTrue((wayRecord.getTags() == null)
                        || StringUtils.isEmpty(((PGobject) wayRecord.getTags()).getValue()));

                // verify the deleted way
                Assert.assertNull(new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentWaysTbl)
                        .where(currentWaysTbl.id.eq(wayIdsArr[2]))
                        .singleResult(currentWaysTbl));
            }
            catch (Exception e) {
                Assert.fail("Error checking ways: " + e.getMessage());
            }

            try {
                Map<Long, CurrentRelations> relations = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentRelationsTbl)
                        .map(currentRelationsTbl.id, currentRelationsTbl);

                Assert.assertEquals(3, relations.size());

                // verify the previously existing relations
                CurrentRelations relationRecord = relations.get(relationIdsArr[0]);
                Assert.assertEquals(new Long(changesetId), relationRecord.getChangesetId());
                Assert.assertEquals(relationIdsArr[0], relationRecord.getId());
                Assert.assertTrue(relationRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), relationRecord.getVersion());
                Assert.assertTrue(relationRecord.getVisible());

                List<CurrentRelationMembers> members = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentRelationMembersTbl)
                        .where(currentRelationMembersTbl.relationId.eq(relationIdsArr[0]))
                        .orderBy(currentRelationMembersTbl.sequenceId.asc()).list(currentRelationMembersTbl);

                Assert.assertEquals(4, members.size());
                CurrentRelationMembers member = members.get(0);
                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
                Assert.assertEquals("role1", member.getMemberRole());
                Assert.assertEquals(new Integer(1), member.getSequenceId());

                Assert.assertEquals(nodeIdsArr[0], member.getMemberId());
                member = members.get(1);
                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.way, member.getMemberType());
                Assert.assertEquals("role3", member.getMemberRole());
                Assert.assertEquals(new Integer(2), member.getSequenceId());

                Assert.assertEquals(wayIdsArr[1], member.getMemberId());
                member = members.get(2);
                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.way, member.getMemberType());
                Assert.assertEquals("role2", member.getMemberRole());
                Assert.assertEquals(new Integer(3), member.getSequenceId());

                Assert.assertEquals(wayIdsArr[0], member.getMemberId());
                member = members.get(3);
                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
                Assert.assertEquals("", member.getMemberRole());
                Assert.assertEquals(new Integer(4), member.getSequenceId());

                Assert.assertEquals(nodeIdsArr[2], member.getMemberId());
                // verify the previously existing tags
                Map<String, String> tags = PostgresUtils.postgresObjToHStore((PGobject) relationRecord.getTags());
                Assert.assertNotNull(tags);
                Assert.assertEquals(1, tags.size());
                Assert.assertEquals("val 1", tags.get("key 1"));

                relationRecord = relations.get(relationIdsArr[2]);
                Assert.assertEquals(new Long(changesetId), relationRecord.getChangesetId());
                Assert.assertEquals(relationIdsArr[2], relationRecord.getId());

                Assert.assertTrue(relationRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), relationRecord.getVersion());
                Assert.assertTrue(relationRecord.getVisible());

                members = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentRelationMembersTbl)
                        .where(currentRelationMembersTbl.relationId.eq(relationIdsArr[2]))
                        .orderBy(currentRelationMembersTbl.sequenceId.asc())
                        .list(currentRelationMembersTbl);

                Assert.assertEquals(1, members.size());
                member = members.get(0);
                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.way, member.getMemberType());
                Assert.assertEquals("", member.getMemberRole());
                Assert.assertEquals(new Integer(1), member.getSequenceId());

                Assert.assertEquals(wayIdsArr[1], member.getMemberId());
                tags = PostgresUtils.postgresObjToHStore((PGobject) relationRecord.getTags());
                Assert.assertNotNull(tags);
                Assert.assertEquals(1, tags.size());
                Assert.assertEquals("val 4", tags.get("key 4"));

                relationRecord = relations.get(relationIdsArr[3]);
                Assert.assertEquals(new Long(changesetId), relationRecord.getChangesetId());
                Assert.assertEquals(relationIdsArr[3], relationRecord.getId());

                Assert.assertTrue(relationRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), relationRecord.getVersion());
                Assert.assertTrue(relationRecord.getVisible());

                members = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentRelationMembersTbl)
                        .where(currentRelationMembersTbl.relationId.eq(relationIdsArr[3]))
                        .orderBy(currentRelationMembersTbl.sequenceId.asc())
                        .list(currentRelationMembersTbl);

                Assert.assertEquals(1, members.size());
                member = members.get(0);
                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
                Assert.assertEquals("role1", member.getMemberRole());
                Assert.assertEquals(new Integer(1), member.getSequenceId());

                Assert.assertEquals(nodeIdsArr[2], member.getMemberId());
                Assert.assertTrue((relationRecord.getTags() == null)
                        || StringUtils.isEmpty(((PGobject) relationRecord.getTags()).getValue()));

                // verify the deleted relation
                Assert.assertNull(new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentRelationsTbl)
                        .where(currentRelationsTbl.id.eq(relationIdsArr[1]))
                        .singleResult(currentRelationsTbl)

                );
            }
            catch (Exception e) {
                Assert.fail("Error checking relations: " + e.getMessage());
            }

            try {
                // changeset = changesetDao.findById(changesetId);
                changeset = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(changesets)
                        .where(changesets.id.eq(changesetId))
                        .singleResult(changesets);

                Assert.assertNotNull(changeset);
                Assert.assertTrue(changeset.getCreatedAt().before(now));
                Assert.assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));

                Assert.assertEquals(new Integer(15), changeset.getNumChanges());
                Assert.assertEquals(new Long(userId), changeset.getUserId());

                BoundingBox expandedBounds = new BoundingBox(originalBounds);
                expandedBounds.expand(originalBounds,
                        Double.parseDouble(HootProperties.getDefault("changesetBoundsExpansionFactorDeegrees")));
                Changeset hootChangeset = new Changeset(mapId, changesetId, conn);
                BoundingBox changesetBounds = hootChangeset.getBounds();
                Assert.assertEquals(changesetBounds, expandedBounds);
            }
            catch (Exception e) {
                Assert.fail("Error checking updated changeset: " + e.getMessage());
            }
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    /**
     * //TODO: this method is nearly identical to testUploadDelete
     */
    @Test
    @Category(UnitTest.class)
    public void testUploadDeleteNoTags() throws Exception {
        try {
            BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
            long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
            Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
            Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
            Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
            Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
            Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
            Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

            // now delete some elements
            Document responseData = null;
            try {
                responseData = resource()
                        .path("api/0.6/changeset/" + changesetId + "/upload")
                        .queryParam("mapId", String.valueOf(mapId))
                        .type(MediaType.TEXT_XML).accept(MediaType.TEXT_XML)
                        .post(Document.class,
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
                            "</osmChange>");
            }
            catch (UniformInterfaceException e) {
                ClientResponse r = e.getResponse();
                Assert.fail("Unexpected response " + r.getStatus() + " " + r.getEntity(String.class));
            }
            Assert.assertNotNull(responseData);

            XPath xpath = XmlUtils.createXPath();
            try {
                NodeList returnedNodes = XPathAPI.selectNodeList(responseData, "//osm/diffResult/node");
                Assert.assertEquals(1, returnedNodes.getLength());

                // check that the returned IDs are correct and the correct attributes were returned
                Assert.assertEquals((long) nodeIdsArr[3],
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@old_id", responseData)));
                org.w3c.dom.Node xml = XPathAPI.selectSingleNode(responseData, "//osm/diffResult/node[1]");
                Assert.assertNull(xml.getAttributes().getNamedItem("new_id"));
                Assert.assertNull(xml.getAttributes().getNamedItem("new_version"));

                NodeList returnedWays = XPathAPI.selectNodeList(responseData, "//osm/diffResult/way");
                Assert.assertEquals(1, returnedWays.getLength());

                Assert.assertEquals((long) wayIdsArr[2],
                        Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@old_id", responseData)));
                xml = XPathAPI.selectSingleNode(responseData, "//osm/diffResult/way[1]");
                Assert.assertNull(xml.getAttributes().getNamedItem("new_id"));
                Assert.assertNull(xml.getAttributes().getNamedItem("new_version"));

                NodeList returnedRelations = XPathAPI.selectNodeList(responseData, "//osm/diffResult/relation");
                Assert.assertEquals(1, returnedRelations.getLength());

                Assert.assertEquals((long) relationIdsArr[2],
                        Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@old_id", responseData)));
                xml = XPathAPI.selectSingleNode(responseData, "//osm/diffResult/relation[1]");
                Assert.assertNull(xml.getAttributes().getNamedItem("new_id"));
                Assert.assertNull(xml.getAttributes().getNamedItem("new_version"));
            }
            catch (XPathExpressionException e) {
                Assert.fail("Error parsing response document: " + e.getMessage());
            }

            Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
            QChangesets changesets = QChangesets.changesets;
            Changesets changeset = new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(changesets)
                    .where(changesets.id.eq(changesetId)).singleResult(changesets);

            try {
                Map<Long, CurrentNodes> nodes = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentNodesTbl)
                        .map(currentNodesTbl.id, currentNodesTbl);
                Assert.assertEquals(4, nodes.size());

                CurrentNodes nodeRecord = nodes.get(nodeIdsArr[0]);
                Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
                Assert.assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
                Assert.assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
                Assert.assertEquals(nodeIdsArr[0], nodeRecord.getId());
                Assert.assertEquals(
                        new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                        nodeRecord.getTile());
                Assert.assertTrue(nodeRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), nodeRecord.getVersion());
                Assert.assertTrue(nodeRecord.getVisible());
                Map<String, String> tags = PostgresUtils.postgresObjToHStore((PGobject) nodeRecord.getTags());
                Assert.assertNotNull(tags);
                Assert.assertEquals(2, tags.size());
                Assert.assertEquals("val 1", tags.get("key 1"));
                Assert.assertEquals("val 2", tags.get("key 2"));

                nodeRecord = nodes.get(nodeIdsArr[1]);
                Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
                Assert.assertEquals(new Double(originalBounds.getMaxLat()), nodeRecord.getLatitude());
                Assert.assertEquals(new Double(originalBounds.getMaxLon()), nodeRecord.getLongitude());
                Assert.assertEquals(nodeIdsArr[1], nodeRecord.getId());
                Assert.assertEquals(
                        new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                        nodeRecord.getTile());
                Assert.assertTrue(nodeRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), nodeRecord.getVersion());
                Assert.assertTrue(nodeRecord.getVisible());
                Assert.assertTrue((nodeRecord.getTags() == null)
                        || StringUtils.isEmpty(((PGobject) nodeRecord.getTags()).getValue()));

                nodeRecord = nodes.get(nodeIdsArr[2]);
                Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
                Assert.assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
                Assert.assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
                Assert.assertEquals(nodeIdsArr[2], nodeRecord.getId());
                Assert.assertEquals(
                        new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                        nodeRecord.getTile());
                Assert.assertTrue(nodeRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), nodeRecord.getVersion());
                Assert.assertTrue(nodeRecord.getVisible());
                Assert.assertTrue((nodeRecord.getTags() == null)
                        || StringUtils.isEmpty(((PGobject) nodeRecord.getTags()).getValue()));

                nodeRecord = nodes.get(nodeIdsArr[4]);
                Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
                Assert.assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
                Assert.assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
                Assert.assertEquals(nodeIdsArr[4], nodeRecord.getId());
                Assert.assertEquals(
                        new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                        nodeRecord.getTile());
                Assert.assertTrue(nodeRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), nodeRecord.getVersion());
                Assert.assertTrue(nodeRecord.getVisible());
                tags = PostgresUtils.postgresObjToHStore((PGobject) nodeRecord.getTags());
                Assert.assertNotNull(tags);
                Assert.assertEquals(1, tags.size());
                Assert.assertEquals("val 4", tags.get("key 4"));

                // verify the deleted node
                nodeRecord = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentNodesTbl)
                        .where(currentNodesTbl.id.eq(nodeIdsArr[3]))
                        .singleResult(currentNodesTbl);
                Assert.assertNull(nodeRecord);
            }
            catch (Exception e) {
                Assert.fail("Error checking nodes: " + e.getMessage());
            }

            try {
                Map<Long, CurrentWays> ways = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentWaysTbl).map(currentWaysTbl.id, currentWaysTbl);
                Assert.assertEquals(2, ways.size());

                CurrentWays wayRecord = ways.get(wayIdsArr[0]);
                Assert.assertEquals(new Long(changesetId), wayRecord.getChangesetId());
                Assert.assertEquals(wayIdsArr[0], wayRecord.getId());
                Assert.assertTrue(wayRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), wayRecord.getVersion());
                Assert.assertTrue(wayRecord.getVisible());
                List<CurrentWayNodes> wayNodes = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentWayNodesTbl)
                        .where(currentWayNodesTbl.wayId.eq(wayIdsArr[0]))
                        .orderBy(currentWayNodesTbl.sequenceId.asc())
                        .list(currentWayNodesTbl);
                Assert.assertEquals(3, wayNodes.size());
                CurrentWayNodes wayNode = wayNodes.get(0);
                Assert.assertEquals(nodeIdsArr[0], wayNode.getNodeId());
                Assert.assertEquals(new Long(1), wayNode.getSequenceId());
                Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());
                wayNode = wayNodes.get(1);
                Assert.assertEquals(nodeIdsArr[1], wayNode.getNodeId());
                Assert.assertEquals(new Long(2), wayNode.getSequenceId());
                Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());
                wayNode = wayNodes.get(2);
                Assert.assertEquals(nodeIdsArr[4], wayNode.getNodeId());
                Assert.assertEquals(new Long(3), wayNode.getSequenceId());
                Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());
                // verify the previously existing tags
                Map<String, String> tags = PostgresUtils.postgresObjToHStore((PGobject) wayRecord.getTags());
                Assert.assertNotNull(tags);
                Assert.assertEquals(2, tags.size());
                Assert.assertEquals("val 1", tags.get("key 1"));
                Assert.assertEquals("val 2", tags.get("key 2"));

                wayRecord = ways.get(wayIdsArr[1]);
                Assert.assertEquals(new Long(changesetId), wayRecord.getChangesetId());
                Assert.assertEquals(wayIdsArr[1], wayRecord.getId());
                Assert.assertTrue(wayRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), wayRecord.getVersion());
                Assert.assertTrue(wayRecord.getVisible());
                wayNodes = new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentWayNodesTbl)
                        .where(currentWayNodesTbl.wayId.eq(wayIdsArr[1]))
                        .orderBy(currentWayNodesTbl.sequenceId.asc())
                        .list(currentWayNodesTbl);
                Assert.assertEquals(2, wayNodes.size());
                wayNode = wayNodes.get(0);
                Assert.assertEquals(nodeIdsArr[2], wayNode.getNodeId());
                Assert.assertEquals(new Long(1), wayNode.getSequenceId());
                Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());
                wayNode = wayNodes.get(1);
                Assert.assertEquals(nodeIdsArr[1], wayNode.getNodeId());
                Assert.assertEquals(new Long(2), wayNode.getSequenceId());
                Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());
                // verify the way with no tags
                Assert.assertTrue((wayRecord.getTags() == null)
                        || StringUtils.isEmpty(((PGobject) wayRecord.getTags()).getValue()));

                // verify the deleted way
                Assert.assertNull(
                        new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                                .from(currentWaysTbl)
                                .where(currentWaysTbl.id.eq(wayIdsArr[2]))
                                .singleResult(currentWaysTbl));
            }
            catch (Exception e) {
                Assert.fail("Error checking ways: " + e.getMessage());
            }

            try {
                Map<Long, CurrentRelations> relations = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentRelationsTbl)
                        .map(currentRelationsTbl.id, currentRelationsTbl);

                Assert.assertEquals(3, relations.size());

                // verify the previously existing relations
                CurrentRelations relationRecord = relations.get(relationIdsArr[0]);
                Assert.assertEquals(new Long(changesetId), relationRecord.getChangesetId());
                Assert.assertEquals(relationIdsArr[0], relationRecord.getId());
                Assert.assertTrue(relationRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), relationRecord.getVersion());
                Assert.assertTrue(relationRecord.getVisible());
                List<CurrentRelationMembers> members = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentRelationMembersTbl)
                        .where(currentRelationMembersTbl.relationId.eq(relationIdsArr[0]))
                        .orderBy(currentRelationMembersTbl.sequenceId.asc())
                        .list(currentRelationMembersTbl);
                Assert.assertEquals(4, members.size());
                CurrentRelationMembers member = members.get(0);
                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
                Assert.assertEquals("role1", member.getMemberRole());
                Assert.assertEquals(new Integer(1), member.getSequenceId());

                Assert.assertEquals(nodeIdsArr[0], member.getMemberId());
                member = members.get(1);
                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.way, member.getMemberType());
                Assert.assertEquals("role3", member.getMemberRole());
                Assert.assertEquals(new Integer(2), member.getSequenceId());

                Assert.assertEquals(wayIdsArr[1], member.getMemberId());
                member = members.get(2);
                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.way, member.getMemberType());
                Assert.assertEquals("role2", member.getMemberRole());
                Assert.assertEquals(new Integer(3), member.getSequenceId());

                Assert.assertEquals(wayIdsArr[0], member.getMemberId());
                member = members.get(3);
                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
                Assert.assertEquals("", member.getMemberRole());
                Assert.assertEquals(new Integer(4), member.getSequenceId());

                Assert.assertEquals(nodeIdsArr[2], member.getMemberId());
                // verify the previously existing tags
                Map<String, String> tags = PostgresUtils.postgresObjToHStore((PGobject) relationRecord.getTags());
                Assert.assertNotNull(tags);
                Assert.assertEquals(1, tags.size());
                Assert.assertEquals("val 1", tags.get("key 1"));

                relationRecord = relations.get(relationIdsArr[1]);
                Assert.assertEquals(new Long(changesetId), relationRecord.getChangesetId());
                Assert.assertEquals(relationIdsArr[1], relationRecord.getId());

                Assert.assertTrue(relationRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), relationRecord.getVersion());
                Assert.assertTrue(relationRecord.getVisible());
                members = new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentRelationMembersTbl)
                        .where(currentRelationMembersTbl.relationId.eq(relationIdsArr[1]))
                        .orderBy(currentRelationMembersTbl.sequenceId.asc())
                        .list(currentRelationMembersTbl);
                Assert.assertEquals(2, members.size());
                member = members.get(0);
                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
                Assert.assertEquals("role1", member.getMemberRole());
                Assert.assertEquals(new Integer(1), member.getSequenceId());

                Assert.assertEquals(nodeIdsArr[4], member.getMemberId());
                member = members.get(1);
                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.relation, member.getMemberType());
                Assert.assertEquals("role1", member.getMemberRole());
                Assert.assertEquals(new Integer(2), member.getSequenceId());

                Assert.assertEquals(relationIdsArr[0], member.getMemberId());
                tags = PostgresUtils.postgresObjToHStore((PGobject) relationRecord.getTags());
                Assert.assertNotNull(tags);
                Assert.assertEquals(2, tags.size());
                Assert.assertEquals("val 2", tags.get("key 2"));
                Assert.assertEquals("val 3", tags.get("key 3"));

                relationRecord = relations.get(relationIdsArr[3]);
                Assert.assertEquals(new Long(changesetId), relationRecord.getChangesetId());
                Assert.assertEquals(relationIdsArr[3], relationRecord.getId());

                Assert.assertTrue(relationRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), relationRecord.getVersion());
                Assert.assertTrue(relationRecord.getVisible());
                members = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentRelationMembersTbl)
                        .where(currentRelationMembersTbl.relationId.eq(relationIdsArr[3]))
                        .orderBy(currentRelationMembersTbl.sequenceId.asc())
                        .list(currentRelationMembersTbl);
                Assert.assertEquals(1, members.size());
                member = members.get(0);
                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
                Assert.assertEquals("role1", member.getMemberRole());
                Assert.assertEquals(new Integer(1), member.getSequenceId());

                Assert.assertEquals(nodeIdsArr[2], member.getMemberId());
                Assert.assertTrue((relationRecord.getTags() == null)
                        || StringUtils.isEmpty(((PGobject) relationRecord.getTags()).getValue()));

                // verify the deleted relation
                Assert.assertNull(new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentRelationsTbl)
                        .where(currentRelationsTbl.id.eq(relationIdsArr[2]))
                        .singleResult(currentRelationsTbl));
            }
            catch (Exception e) {
                Assert.fail("Error checking relations: " + e.getMessage());
            }

            try {
                changeset = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(changesets)
                        .where(changesets.id.eq(changesetId))
                        .singleResult(changesets);

                Assert.assertNotNull(changeset);
                Assert.assertTrue(changeset.getCreatedAt().before(now));
                Assert.assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));
                Assert.assertEquals(new Integer(15), changeset.getNumChanges());
                Assert.assertEquals(new Long(userId), changeset.getUserId());

                BoundingBox expandedBounds = new BoundingBox(originalBounds);
                expandedBounds.expand(originalBounds,
                        Double.parseDouble(HootProperties.getDefault("changesetBoundsExpansionFactorDeegrees")));
                Changeset hootChangeset = new Changeset(mapId, changesetId, conn);
                BoundingBox changesetBounds = hootChangeset.getBounds();
                Assert.assertEquals(changesetBounds, expandedBounds);
            }
            catch (Exception e) {
                Assert.fail("Error checking updated changeset: " + e.getMessage());
            }
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadDeleteNonExistingNode() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        BoundingBox updateBounds = OsmTestUtils.createAfterModifiedTestChangesetBounds();
        // Try to delete a node in the changeset that doesn't exist. A failure
        // should occur and no data in the system should be modified.
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
                            "<delete if-unused=\"true\">" +
                                "<node id=\"" + nodeIdsArr[3] + "\" lon=\"" + originalBounds.getMinLon() +
                                       "\" " + "lat=\"" + originalBounds.getMinLat() + "\" version=\"1\" changeset=\""
                                       + changesetId + "\">" + "</node>" + "<node id=\"" + (nodeIdsArr[4] + 1) +
                                       "\" lon=\"" + updateBounds.getMinLon() + "\" " + "lat=\"" +
                                      updateBounds.getMinLat() + "\" version=\"1\" changeset=\"" + changesetId + "\">" +
                                    "<tag k=\"key 3\" v=\"val 3\"></tag>" +
                                "</node>" +
                            "</delete>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.NOT_FOUND, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Element(s) being referenced don't exist."));

            // make sure that any of the existing nodes weren't deleted
            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadDeleteNonExistingWay() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Try to delete a way in the changeset that doesn't exist. A failure
        // should occur and no data in the system should be modified.
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
                             "<delete if-unused=\"true\">" +
                                 "<way id=\"" + wayIdsArr[2] + "\" version=\"1\" changeset=\"" + changesetId + "\">" +
                                     "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                     "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                                 "</way>" +
                                 "<way id=\"" + (wayIdsArr[2] + 1) + "\" version=\"1\" changeset=\"" + changesetId + "\">" + "</way>" +
                             "</delete>" +
                         "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.NOT_FOUND, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Element(s) being referenced don't exist."));

            // make sure that any of the existing nodes weren't deleted
            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadDeleteNonExistingRelation() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // Try to delete a relation in the changeset that doesn't exist. A
        // failure should occur and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML).post(Document.class,
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
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.NOT_FOUND, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Element(s) being referenced don't exist."));

            // make sure that any of the existing nodes weren't deleted
            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadDeleteDuplicateNodeIds() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Try to delete two nodes with the same ID. A failure should occur and
        // no data in the system should be modified.
        try {
            resource().path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
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
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Duplicate OSM element ID"));

            // make sure that no nodes were deleted
            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadDeleteDuplicateWayIds() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Try to delete two ways with the same ID. A failure should occur and
        // no data in the system should be modified.
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
                            "<delete if-unused=\"true\">" +
                                "<way id=\"" + wayIdsArr[2] + "\" version=\"1\" changeset=\"" + changesetId + "\">" +
                                    "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                    "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                                    "<tag k=\"key 3\" v=\"val 3\"></tag>" +
                                "</way>" +
                                "<way id=\"" + wayIdsArr[2] + "\" version=\"1\" changeset=\"" + changesetId + "\">" + "</way>" +
                            "</delete>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Duplicate OSM element ID"));

            // make sure that no nodes were deleted
            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadDeleteDuplicateRelationIds() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // Try to delete two relations with the same ID. A failure should occur
        // and no data in the system should be modified.
        try {
            resource().path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
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
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Duplicate OSM element ID"));

            // make sure that no nodes were deleted
            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadDeleteNodeInvalidVersion() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Update the changeset where one of the nodes has version that doesn't
        // match the version on he server. A failure should occur and no data in the system should be modified.
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
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.CONFLICT, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Invalid version"));

            // make sure that no nodes were deleted
            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testUploadDeleteNegativeElementId() throws Exception {
        try {
            // We allow elements with a negative ID to exist, which allows for
            // using the hoot --convert command as a source of test data for the services.

            BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
            BoundingBox updateBounds = OsmTestUtils.createAfterModifiedTestChangesetBounds();
            long changesetId = OsmTestUtils.createTestChangeset(originalBounds);

            // link some nodes to the changeset
            Set<Long> nodeIds = new LinkedHashSet<>();

            // explicitly create a node with a negative ID
            Map<String, String> tags = new HashMap<>();
            tags.put("key 3", "val 3");
            long negativeNodeId = -1;
            Node.insertNew(negativeNodeId, changesetId, mapId, originalBounds.getMaxLat(), originalBounds.getMaxLon(), tags, conn);
            tags.clear();

            CurrentNodes insertedNodeRecord = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                    .from(currentNodesTbl)
                    .where(currentNodesTbl.id.eq(negativeNodeId))
                    .singleResult(currentNodesTbl);

            Assert.assertNotNull(insertedNodeRecord);
            Assert.assertEquals(new Long(negativeNodeId), insertedNodeRecord.getId());
            nodeIds.add(negativeNodeId);

            tags.put("key 1", "val 1");
            tags.put("key 2", "val 2");
            nodeIds.add(Node.insertNew(changesetId, mapId, originalBounds.getMinLat(), originalBounds.getMinLon(), tags, conn));
            tags.clear();

            Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);

            // link some ways to the changeset
            Set<Long> wayIds = new LinkedHashSet<>();
            List<Long> wayNodeIds = new ArrayList<>();

            // explicitly create a way with a negative ID
            wayNodeIds.add(nodeIdsArr[0]);
            wayNodeIds.add(nodeIdsArr[1]);
            tags.put("key 1", "val 1");
            tags.put("key 2", "val 2");
            long negativeWayId = -1;
            OsmTestUtils.insertNewWay(negativeWayId, changesetId, mapId, wayNodeIds, tags, conn);
            tags.clear();

            CurrentWays insertedWayRecord = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                    .from(currentWaysTbl)
                    .where(currentWaysTbl.id.eq(negativeWayId))
                    .singleResult(currentWaysTbl);

            Assert.assertNotNull(insertedWayRecord);
            Assert.assertEquals(new Long(negativeWayId), insertedWayRecord.getId());
            wayIds.add(negativeWayId);

            wayNodeIds.clear();
            wayNodeIds.add(nodeIdsArr[1]);
            wayNodeIds.add(nodeIdsArr[2]);
            wayIds.add(OsmTestUtils.insertNewWay(changesetId, mapId, wayNodeIds, null, conn));

            Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);

            // link some relations to the changeset
            Set<Long> relationIds = new LinkedHashSet<>();

            // explicitly create a relation with a negative ID
            List<RelationMember> members = new ArrayList<>();
            members.add(new RelationMember(nodeIdsArr[0], ElementType.Node, "role1"));
            members.add(new RelationMember(wayIdsArr[0], ElementType.Way, "role1"));
            tags.put("key 1", "val 1");
            tags.put("key 2", "val 2");
            long negativeRelationId = -1;
            OsmTestUtils.insertNewRelation(negativeRelationId, changesetId, mapId, members, tags, conn);
            tags.clear();

            CurrentRelations insertedRelationRecord = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                    .from(currentRelationsTbl)
                    .where(currentRelationsTbl.id.eq(negativeRelationId))
                    .singleResult(currentRelationsTbl);

            Assert.assertNotNull(insertedRelationRecord);
            Assert.assertEquals(new Long(negativeRelationId), insertedRelationRecord.getId());
            relationIds.add(negativeRelationId);

            members.clear();
            members.add(new RelationMember(nodeIdsArr[1], ElementType.Node, "role1"));
            members.add(new RelationMember(wayIdsArr[1], ElementType.Way, "role1"));
            relationIds.add(OsmTestUtils.insertNewRelation(changesetId, mapId, members, null, conn));

            Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

            // Update the changeset via the service. This update is valid,
            // because the property was changed above.
            Document responseData = null;
            try {
                responseData = resource()
                        .path("api/0.6/changeset/" + changesetId + "/upload")
                        .queryParam("mapId", String.valueOf(mapId))
                        .type(MediaType.TEXT_XML)
                        .accept(MediaType.TEXT_XML)
                        .post(Document.class,
                            "<osmChange version=\"0.3\" generator=\"iD\">" +
                                "<create/>" +
                                "<modify/>" +
                                "<delete if-unused=\"true\"/>" +
                                    "<node id=\"" + nodeIdsArr[1] + "\" lon=\"" + updateBounds.getMinLon() + "\" " +
                                        "lat=\"" + updateBounds.getMinLat() + "\" version=\"1\" changeset=\"" +
                                         changesetId + "\">" +
                                    "</node>" +
                                    "<node id=\"" + negativeNodeId + "\" lon=\"" + originalBounds.getMaxLon() + "\" " +
                                        "lat=\"" + updateBounds.getMinLat() + "\" version=\"1\" changeset=\"" + changesetId + "\">" +
                                    "</node>" +
                                    "<way id=\"" + wayIdsArr[1] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                        "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                                        "<nd ref=\"" + nodeIdsArr[2] + "\"></nd>" +
                                    "</way>" +
                                    "<way id=\"" + negativeWayId + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                        "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                        "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                                    "</way>" +
                                    "<relation id=\"" + relationIdsArr[1] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                        "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                                        "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[1] + "\"></member>" +
                                    "</relation>" +
                                    "<relation id=\"" + negativeRelationId + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                        "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                        "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[0] + "\"></member>" +
                                    "</relation>" +
                                "</delete>" +
                            "</osmChange>");
            }
            catch (UniformInterfaceException e) {
                ClientResponse r = e.getResponse();
                Assert.fail("Unexpected response " + r.getStatus() + " " + r.getEntity(String.class));
            }
            Assert.assertNotNull(responseData);

            XPath xpath = XmlUtils.createXPath();
            try {
                NodeList returnedNodes = XPathAPI.selectNodeList(responseData, "//osm/diffResult/node");
                Assert.assertEquals(2, returnedNodes.getLength());

                // ID output ordering matches input ordering

                Assert.assertEquals((long) nodeIdsArr[1],
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@old_id", responseData)));
                org.w3c.dom.Node xml = XPathAPI.selectSingleNode(responseData, "//osm/diffResult/node[1]");
                Assert.assertNull(xml.getAttributes().getNamedItem("new_id"));
                Assert.assertNull(xml.getAttributes().getNamedItem("new_version"));

                Assert.assertEquals(negativeNodeId,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@old_id", responseData)));
                xml = XPathAPI.selectSingleNode(responseData, "//osm/diffResult/node[2]");
                Assert.assertNull(xml.getAttributes().getNamedItem("new_id"));
                Assert.assertNull(xml.getAttributes().getNamedItem("new_version"));

                NodeList returnedWays = XPathAPI.selectNodeList(responseData, "//osm/diffResult/way");
                Assert.assertEquals(2, returnedWays.getLength());

                Assert.assertEquals((long) wayIdsArr[1],
                        Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@old_id", responseData)));
                xml = XPathAPI.selectSingleNode(responseData, "//osm/diffResult/way[1]");
                Assert.assertNull(xml.getAttributes().getNamedItem("new_id"));
                Assert.assertNull(xml.getAttributes().getNamedItem("new_version"));

                Assert.assertEquals(negativeWayId,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/way[2]/@old_id", responseData)));
                xml = XPathAPI.selectSingleNode(responseData, "//osm/diffResult/way[2]");
                Assert.assertNull(xml.getAttributes().getNamedItem("new_id"));
                Assert.assertNull(xml.getAttributes().getNamedItem("new_version"));

                NodeList returnedRelations = XPathAPI.selectNodeList(responseData, "//osm/diffResult/relation");
                Assert.assertEquals(2, returnedRelations.getLength());

                Assert.assertEquals((long) relationIdsArr[1],
                        Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@old_id", responseData)));
                xml = XPathAPI.selectSingleNode(responseData, "//osm/diffResult/relation[1]");
                Assert.assertNull(xml.getAttributes().getNamedItem("new_id"));
                Assert.assertNull(xml.getAttributes().getNamedItem("new_version"));

                Assert.assertEquals(negativeRelationId,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/relation[2]/@old_id", responseData)));
                xml = XPathAPI.selectSingleNode(responseData, "//osm/diffResult/relation[2]");
                Assert.assertNull(xml.getAttributes().getNamedItem("new_id"));
                Assert.assertNull(xml.getAttributes().getNamedItem("new_version"));
            }
            catch (XPathExpressionException e) {
                Assert.fail("Error parsing response document: " + e.getMessage());
            }

            QChangesets changesets = QChangesets.changesets;
            Changesets changeset = new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(changesets)
                    .where(changesets.id.eq(changesetId)).singleResult(changesets);
            try {
                Assert.assertEquals(0, new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentNodesTbl).count());
            }
            catch (Exception e) {
                Assert.fail("Error checking nodes: " + e.getMessage());
            }

            try {
                Assert.assertEquals(0,
                        new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentWaysTbl).count());
            }
            catch (Exception e) {
                Assert.fail("Error checking ways: " + e.getMessage());
            }

            try {
                Assert.assertEquals(0,
                        new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentRelationsTbl).count());
            }
            catch (Exception e) {
                Assert.fail("Error checking relations: " + e.getMessage());
            }

            try {
                changeset = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(changesets)
                        .where(changesets.id.eq(changesetId))
                        .singleResult(changesets);

                Assert.assertNotNull(changeset);
                Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
                Assert.assertTrue(changeset.getCreatedAt().before(now));
                Assert.assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));
                Assert.assertEquals(new Integer(12), changeset.getNumChanges());
                Assert.assertEquals(new Long(userId), changeset.getUserId());

                BoundingBox expandedBounds = new BoundingBox(originalBounds);
                expandedBounds.expand(updateBounds,
                        Double.parseDouble(HootProperties.getDefault("changesetBoundsExpansionFactorDeegrees")));
                Changeset hootChangeset = new Changeset(mapId, changesetId, conn);
                BoundingBox changesetBounds = hootChangeset.getBounds();
                Assert.assertEquals(changesetBounds, expandedBounds);
            }
            catch (Exception e) {
                Assert.fail("Error checking changeset: " + e.getMessage());
            }
        }
        catch (Exception e) {
            log.error(e.getMessage());
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testUploadDeleteMissingNodeTagValue() throws Exception {
        try {
            BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
            long changesetId = OsmTestUtils.createTestChangeset(originalBounds, 5);
            Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
            Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);

            // Now delete some nodes with an update to the changeset. One of the
            // nodes being deleted does not have its tag value specified. The test should pass b/c
            // node tags are optional for a delete and not parsed.
            Document responseData = null;
            try {
                responseData = resource()
                        .path("api/0.6/changeset/" + changesetId + "/upload")
                        .queryParam("mapId", String.valueOf(mapId))
                        .type(MediaType.TEXT_XML)
                        .accept(MediaType.TEXT_XML)
                        .post(Document.class,
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
                            "</osmChange>");
            }
            catch (UniformInterfaceException e) {
                ClientResponse r = e.getResponse();
                Assert.fail("Unexpected response " + r.getStatus() + " " + r.getEntity(String.class));
            }
            Assert.assertNotNull(responseData);

            Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
            XPath xpath = XmlUtils.createXPath();
            try {
                NodeList returnedNodes = XPathAPI.selectNodeList(responseData, "//osm/diffResult/node");
                Assert.assertEquals(2, returnedNodes.getLength());

                Assert.assertEquals((long) nodeIdsArr[3],
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@old_id", responseData)));

                Assert.assertEquals((long) nodeIdsArr[4],
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@old_id", responseData)));
            }
            catch (XPathExpressionException e) {
                Assert.fail("Error parsing response document: " + e.getMessage());
            }

            QChangesets changesets = QChangesets.changesets;
            Changesets changeset = new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(changesets)
                    .where(changesets.id.eq(changesetId)).singleResult(changesets);
            try {
                Map<Long, CurrentNodes> nodes = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentNodesTbl)
                        .map(currentNodesTbl.id, currentNodesTbl);

                Assert.assertEquals(3, nodes.size());

                CurrentNodes nodeRecord = nodes.get(nodeIdsArr[0]);
                Map<String, String> tags = PostgresUtils.postgresObjToHStore((PGobject) nodeRecord.getTags());
                Assert.assertNotNull(tags);
                Assert.assertEquals(2, tags.size());
                Assert.assertEquals("val 1", tags.get("key 1"));
                Assert.assertEquals("val 2", tags.get("key 2"));

                nodeRecord = nodes.get(nodeIdsArr[1]);
                Assert.assertTrue((nodeRecord.getTags() == null)
                        || StringUtils.isEmpty(((PGobject) nodeRecord.getTags()).getValue()));

                nodeRecord = nodes.get(nodeIdsArr[2]);
                Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
                Assert.assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
                Assert.assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
                Assert.assertEquals(nodeIdsArr[2], nodeRecord.getId());
                Assert.assertEquals(
                        new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                        nodeRecord.getTile());
                Assert.assertTrue(nodeRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), nodeRecord.getVersion());
                Assert.assertTrue(nodeRecord.getVisible());
                Assert.assertTrue((nodeRecord.getTags() == null)
                        || StringUtils.isEmpty(((PGobject) nodeRecord.getTags()).getValue()));

                Assert.assertNull(nodes.get(nodeIdsArr[3]));
                Assert.assertNull(nodes.get(nodeIdsArr[4]));
            }
            catch (Exception e) {
                Assert.fail("Error checking nodes: " + e.getMessage());
            }

            try {
                changeset = new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(changesets)
                        .where(changesets.id.eq(changesetId)).singleResult(changesets);

                Assert.assertNotNull(changeset);
                Assert.assertTrue(changeset.getCreatedAt().before(now));
                Assert.assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));
                Assert.assertEquals(new Integer(7), changeset.getNumChanges());
                Assert.assertEquals(new Long(userId), changeset.getUserId());

                BoundingBox expandedBounds = new BoundingBox(originalBounds);
                expandedBounds.expand(originalBounds,
                        Double.parseDouble(HootProperties.getDefault("changesetBoundsExpansionFactorDeegrees")));
                Changeset hootChangeset = new Changeset(mapId, changesetId, conn);
                BoundingBox changesetBounds = hootChangeset.getBounds();
                Assert.assertEquals(changesetBounds, expandedBounds);
            }
            catch (Exception e) {
                Assert.fail("Error checking updated changeset: " + e.getMessage());
            }
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testUploadDeleteMissingWayTagValue() throws Exception {
        try {
            BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
            long changesetId = OsmTestUtils.createTestChangeset(originalBounds, 8);
            Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
            Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
            Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
            Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);

            // Now delete some ways with an update to the changeset. One of the
            // ways being deleted does not have its tag value specified. The test should pass b/c
            // way tags are optional for a delete and not parsed.
            Document responseData = null;
            try {
                responseData = resource()
                        .path("api/0.6/changeset/" + changesetId + "/upload")
                        .queryParam("mapId", String.valueOf(mapId))
                        .type(MediaType.TEXT_XML)
                        .accept(MediaType.TEXT_XML)
                        .post(Document.class,
                            "<osmChange version=\"0.3\" generator=\"iD\">" +
                                "<create/>" +
                                "<modify/>" +
                                "<delete if-unused=\"true\">" +
                                    "<way id=\"" + wayIdsArr[0] + "\" version=\"1\" changeset=\"" + changesetId + "\">" + "</way>" +
                                    "<way id=\"" + wayIdsArr[2] + "\" version=\"1\" changeset=\"" + changesetId + "\">" + "</way>" +
                                "</delete>" +
                            "</osmChange>");
            }
            catch (UniformInterfaceException e) {
                ClientResponse r = e.getResponse();
                Assert.fail("Unexpected response " + r.getStatus() + " " + r.getEntity(String.class));
            }
            Assert.assertNotNull(responseData);

            XPath xpath = XmlUtils.createXPath();
            try {
                NodeList returnedWays = XPathAPI.selectNodeList(responseData, "//osm/diffResult/way");
                Assert.assertEquals(2, returnedWays.getLength());

                Assert.assertEquals((long) wayIdsArr[0],
                        Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@old_id", responseData)));

                Assert.assertEquals((long) wayIdsArr[2],
                        Long.parseLong(xpath.evaluate("//osm/diffResult/way[2]/@old_id", responseData)));
            }
            catch (XPathExpressionException e) {
                Assert.fail("Error parsing response document: " + e.getMessage());
            }

            Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
            QChangesets changesets = QChangesets.changesets;
            Changesets changeset = new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(changesets)
                    .where(changesets.id.eq(changesetId)).singleResult(changesets);
            try {
                Map<Long, CurrentWays> ways = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentWaysTbl)
                        .map(currentWaysTbl.id, currentWaysTbl);

                Assert.assertEquals(1, ways.size());
                Assert.assertNull(ways.get(wayIdsArr[0]));

                CurrentWays wayRecord = ways.get(wayIdsArr[1]);
                Assert.assertEquals(new Long(changesetId), wayRecord.getChangesetId());
                Assert.assertEquals(wayIdsArr[1], wayRecord.getId());
                Assert.assertTrue(wayRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), wayRecord.getVersion());
                Assert.assertTrue(wayRecord.getVisible());

                List<CurrentWayNodes> wayNodes = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentWayNodesTbl).where(currentWayNodesTbl.wayId.eq(wayIdsArr[1]))
                        .orderBy(currentWayNodesTbl.sequenceId.asc())
                        .list(currentWayNodesTbl);

                Assert.assertEquals(2, wayNodes.size());
                CurrentWayNodes wayNode = wayNodes.get(0);
                Assert.assertEquals(nodeIdsArr[2], wayNode.getNodeId());
                Assert.assertEquals(new Long(1), wayNode.getSequenceId());
                Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());
                wayNode = wayNodes.get(1);
                Assert.assertEquals(nodeIdsArr[1], wayNode.getNodeId());
                Assert.assertEquals(new Long(2), wayNode.getSequenceId());
                Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());
                Assert.assertTrue((wayRecord.getTags() == null)
                        || StringUtils.isEmpty(((PGobject) wayRecord.getTags()).getValue()));

                Assert.assertNull(ways.get(wayIdsArr[2]));
            }
            catch (Exception e) {
                Assert.fail("Error checking ways: " + e.getMessage());
            }

            try {
                changeset = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(changesets)
                        .where(changesets.id.eq(changesetId))
                        .singleResult(changesets);

                Assert.assertNotNull(changeset);
                Assert.assertTrue(changeset.getCreatedAt().before(now));
                Assert.assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));
                Assert.assertEquals(new Integer(10), changeset.getNumChanges());
                Assert.assertEquals(new Long(userId), changeset.getUserId());

                BoundingBox expandedBounds = new BoundingBox(originalBounds);
                expandedBounds.expand(originalBounds,
                        Double.parseDouble(HootProperties.getDefault("changesetBoundsExpansionFactorDeegrees")));
                Changeset hootChangeset = new Changeset(mapId, changesetId, conn);
                BoundingBox changesetBounds = hootChangeset.getBounds();
                Assert.assertEquals(changesetBounds, expandedBounds);
            }
            catch (Exception e) {
                Assert.fail("Error checking updated changeset: " + e.getMessage());
            }
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testUploadDeleteMissingRelationTagValue() throws Exception {
        try {
            BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
            long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
            Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
            Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
            Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
            Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
            Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
            Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

            // Now delete some relations with an update to the changeset. One of
            // the relations being deleted does not have its tag value specified. The test should
            // pass b/c tags are optional for a delete and not parsed.
            Document responseData = null;
            try {
                responseData = resource()
                        .path("api/0.6/changeset/" + changesetId + "/upload")
                        .queryParam("mapId", String.valueOf(mapId))
                        .type(MediaType.TEXT_XML)
                        .accept(MediaType.TEXT_XML)
                        .post(Document.class,
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
                            "</osmChange>");
            }
            catch (UniformInterfaceException e) {
                ClientResponse r = e.getResponse();
                Assert.fail("Unexpected response " + r.getStatus() + " " + r.getEntity(String.class));
            }
            Assert.assertNotNull(responseData);

            XPath xpath = XmlUtils.createXPath();
            try {
                NodeList returnedNodes = XPathAPI.selectNodeList(responseData, "//osm/diffResult/node");
                Assert.assertEquals(0, returnedNodes.getLength());

                NodeList returnedWays = XPathAPI.selectNodeList(responseData, "//osm/diffResult/way");
                Assert.assertEquals(0, returnedWays.getLength());

                NodeList returnedRelations = XPathAPI.selectNodeList(responseData, "//osm/diffResult/relation");
                Assert.assertEquals(2, returnedRelations.getLength());

                Assert.assertEquals((long) relationIdsArr[2],
                        Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@old_id", responseData)));
                org.w3c.dom.Node xml = XPathAPI.selectSingleNode(responseData, "//osm/diffResult/relation[1]");
                Assert.assertNull(xml.getAttributes().getNamedItem("new_id"));
                Assert.assertNull(xml.getAttributes().getNamedItem("new_version"));
                Assert.assertEquals((long) relationIdsArr[3],
                        Long.parseLong(xpath.evaluate("//osm/diffResult/relation[2]/@old_id", responseData)));
                xml = XPathAPI.selectSingleNode(responseData, "//osm/diffResult/relation[2]");
                Assert.assertNull(xml.getAttributes().getNamedItem("new_id"));
                Assert.assertNull(xml.getAttributes().getNamedItem("new_version"));
            }
            catch (XPathExpressionException e) {
                Assert.fail("Error parsing response document: " + e.getMessage());
            }

            QChangesets changesets = QChangesets.changesets;
            Changesets changeset = new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(changesets)
                    .where(changesets.id.eq(changesetId)).singleResult(changesets);

            OsmTestUtils.verifyTestNodesUnmodified(nodeIds, changesetId, originalBounds);
            OsmTestUtils.verifyTestWaysUnmodified(wayIds, nodeIds, changesetId);

            Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
            try {
                Map<Long, CurrentRelations> relations = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentRelationsTbl)
                        .map(currentRelationsTbl.id, currentRelationsTbl);

                Assert.assertEquals(2, relations.size());

                CurrentRelations relationRecord = relations.get(relationIdsArr[0]);
                Assert.assertEquals(new Long(changesetId), relationRecord.getChangesetId());
                Assert.assertEquals(relationIdsArr[0], relationRecord.getId());
                Assert.assertEquals(new Long(1), relationRecord.getVersion());

                List<CurrentRelationMembers> relationMembers = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentRelationMembersTbl)
                        .where(currentRelationMembersTbl.relationId.eq(relationIdsArr[0]))
                        .orderBy(currentRelationMembersTbl.sequenceId.asc()).list(currentRelationMembersTbl);

                Assert.assertEquals(4, relationMembers.size());
                CurrentRelationMembers member = relationMembers.get(0);
                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
                Assert.assertEquals("role1", member.getMemberRole());
                Assert.assertEquals(new Integer(1), member.getSequenceId());

                Assert.assertEquals(nodeIdsArr[0], member.getMemberId());
                member = relationMembers.get(1);
                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.way, member.getMemberType());
                Assert.assertEquals("role3", member.getMemberRole());
                Assert.assertEquals(new Integer(2), member.getSequenceId());

                Assert.assertEquals(wayIdsArr[1], member.getMemberId());
                member = relationMembers.get(2);
                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.way, member.getMemberType());
                Assert.assertEquals("role2", member.getMemberRole());
                Assert.assertEquals(new Integer(3), member.getSequenceId());

                Assert.assertEquals(wayIdsArr[0], member.getMemberId());
                member = relationMembers.get(3);
                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
                Assert.assertEquals("", member.getMemberRole());
                Assert.assertEquals(new Integer(4), member.getSequenceId());

                Assert.assertEquals(nodeIdsArr[2], member.getMemberId());
                Map<String, String> tags = PostgresUtils.postgresObjToHStore((PGobject) relationRecord.getTags());
                Assert.assertNotNull(tags);
                Assert.assertEquals(1, tags.size());
                Assert.assertEquals("val 1", tags.get("key 1"));

                relationRecord = relations.get(relationIdsArr[1]);
                Assert.assertEquals(new Long(changesetId), relationRecord.getChangesetId());
                Assert.assertEquals(relationIdsArr[1], relationRecord.getId());
                Assert.assertEquals(new Long(1), relationRecord.getVersion());

                relationMembers = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentRelationMembersTbl)
                        .where(currentRelationMembersTbl.relationId.eq(relationIdsArr[1]))
                        .orderBy(currentRelationMembersTbl.sequenceId.asc()).list(currentRelationMembersTbl);

                Assert.assertEquals(2, relationMembers.size());
                member = relationMembers.get(0);
                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
                Assert.assertEquals("role1", member.getMemberRole());
                Assert.assertEquals(new Integer(1), member.getSequenceId());

                Assert.assertEquals(nodeIdsArr[4], member.getMemberId());
                member = relationMembers.get(1);
                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.relation, member.getMemberType());
                Assert.assertEquals("role1", member.getMemberRole());
                Assert.assertEquals(new Integer(2), member.getSequenceId());

                Assert.assertEquals(relationIdsArr[0], member.getMemberId());
                tags = PostgresUtils.postgresObjToHStore((PGobject) relationRecord.getTags());
                Assert.assertNotNull(tags);
                Assert.assertEquals(2, tags.size());
                Assert.assertEquals("val 2", tags.get("key 2"));
                Assert.assertEquals("val 3", tags.get("key 3"));
                Assert.assertNull(relations.get(relationIdsArr[2]));
                Assert.assertNull(relations.get(relationIdsArr[3]));
            }
            catch (Exception e) {
                Assert.fail("Error checking relations: " + e.getMessage());
            }

            try {
                changeset = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(changesets)
                        .where(changesets.id.eq(changesetId))
                        .singleResult(changesets);

                Assert.assertNotNull(changeset);
                Assert.assertTrue(changeset.getCreatedAt().before(now));
                Assert.assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));
                Assert.assertEquals(new Integer(14), changeset.getNumChanges());
                Assert.assertEquals(new Long(userId), changeset.getUserId());

                BoundingBox expandedBounds = new BoundingBox(originalBounds);
                expandedBounds.expand(originalBounds,
                        Double.parseDouble(HootProperties.getDefault("changesetBoundsExpansionFactorDeegrees")));
                Changeset hootChangeset = new Changeset(mapId, changesetId, conn);
                BoundingBox changesetBounds = hootChangeset.getBounds();
                Assert.assertEquals(changesetBounds, expandedBounds);
            }
            catch (Exception e) {
                Assert.fail("Error checking updated changeset: " + e.getMessage());
            }
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadDeleteWayInvalidVersion() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Send a changeset delete request where one of the ways has a version
        // not equal to what's on the server. A failure should occur and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
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
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.CONFLICT, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Invalid version"));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadDeleteRelationInvalidVersion() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // Send a changeset delete request where one of the relations has a
        // version not equal to what's on the server. A failure should occur and no data in the system should be modified.
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
                            "<delete if-unused=\"true\">" +
                                "<relation id=\"" + relationIdsArr[0] + "\" version=\"0\" changeset=\"" +
                                    changesetId + "\" >" +
                                "</relation>" +
                                "<relation id=\"" + relationIdsArr[1] + "\" version=\"2\" changeset=\"" +
                                    changesetId + "\" >" +
                                "</relation>" +
                            "</delete>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.CONFLICT, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Invalid version"));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testUploadDeleteNodeStillBelongsToWayWithIfUnused() throws Exception {
        try {
            BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
            long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
            Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
            Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
            Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
            Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
            Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

            // Try to delete some elements with the "if-unused" tag is passed
            // where one of them is a node that still belongs to a way. No failure should occur, the node
            // should not be deleted, and the other element should be deleted.
            Document responseData = null;
            try {
                responseData = resource()
                        .path("api/0.6/changeset/" + changesetId + "/upload")
                        .queryParam("mapId", String.valueOf(mapId))
                        .type(MediaType.TEXT_XML)
                        .accept(MediaType.TEXT_XML)
                        .post(Document.class,
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
                            "</osmChange>");
            }
            catch (UniformInterfaceException e) {
                ClientResponse r = e.getResponse();
                Assert.fail("Unexpected response " + r.getStatus() + " " + r.getEntity(String.class));
            }

            Assert.assertNotNull(responseData);

            XPath xpath = XmlUtils.createXPath();
            try {
                NodeList returnedNodes = XPathAPI.selectNodeList(responseData, "//osm/diffResult/node");

                Assert.assertEquals(0, returnedNodes.getLength());

                NodeList returnedWays = XPathAPI.selectNodeList(responseData, "//osm/diffResult/way");

                Assert.assertEquals(1, returnedWays.getLength());

                Assert.assertEquals((long) wayIdsArr[2],
                        Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@old_id", responseData)));

                org.w3c.dom.Node xml = XPathAPI.selectSingleNode(responseData, "//osm/diffResult/way[1]");

                Assert.assertNull(xml.getAttributes().getNamedItem("new_id"));
                Assert.assertNull(xml.getAttributes().getNamedItem("new_version"));
            }
            catch (XPathExpressionException e) {
                Assert.fail("Error parsing response document: " + e.getMessage());
            }

            Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
            QChangesets changesets = QChangesets.changesets;
            Changesets changeset = new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(changesets)
                    .where(changesets.id.eq(changesetId)).singleResult(changesets);
            try {
                Map<Long, CurrentNodes> nodes = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentNodesTbl).map(currentNodesTbl.id, currentNodesTbl);

                Assert.assertEquals(5, nodes.size());

                CurrentNodes nodeRecord = nodes.get(nodeIdsArr[0]);

                Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
                Assert.assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
                Assert.assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
                Assert.assertEquals(nodeIdsArr[0], nodeRecord.getId());
                Assert.assertEquals(
                        new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                        nodeRecord.getTile());
                Assert.assertTrue(nodeRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), nodeRecord.getVersion());
                Assert.assertTrue(nodeRecord.getVisible());

                Map<String, String> tags = PostgresUtils.postgresObjToHStore((PGobject) nodeRecord.getTags());

                Assert.assertNotNull(tags);
                Assert.assertEquals(2, tags.size());
                Assert.assertEquals("val 1", tags.get("key 1"));
                Assert.assertEquals("val 2", tags.get("key 2"));

                nodeRecord = nodes.get(nodeIdsArr[1]);

                Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
                Assert.assertEquals(new Double(originalBounds.getMaxLat()), nodeRecord.getLatitude());
                Assert.assertEquals(new Double(originalBounds.getMaxLon()), nodeRecord.getLongitude());
                Assert.assertEquals(nodeIdsArr[1], nodeRecord.getId());
                Assert.assertEquals(
                        new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                        nodeRecord.getTile());
                Assert.assertTrue(nodeRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), nodeRecord.getVersion());
                Assert.assertTrue(nodeRecord.getVisible());
                Assert.assertTrue((nodeRecord.getTags() == null)
                        || StringUtils.isEmpty(((PGobject) nodeRecord.getTags()).getValue()));

                nodeRecord = nodes.get(nodeIdsArr[2]);

                Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
                Assert.assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
                Assert.assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
                Assert.assertEquals(nodeIdsArr[2], nodeRecord.getId());
                Assert.assertEquals(
                        new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                        nodeRecord.getTile());
                Assert.assertTrue(nodeRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), nodeRecord.getVersion());
                Assert.assertTrue(nodeRecord.getVisible());
                Assert.assertTrue((nodeRecord.getTags() == null)
                        || StringUtils.isEmpty(((PGobject) nodeRecord.getTags()).getValue()));

                nodeRecord = nodes.get(nodeIdsArr[3]);

                Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
                Assert.assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
                Assert.assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
                Assert.assertEquals(nodeIdsArr[3], nodeRecord.getId());
                Assert.assertEquals(
                        new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                        nodeRecord.getTile());
                Assert.assertTrue(nodeRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), nodeRecord.getVersion());
                Assert.assertTrue(nodeRecord.getVisible());

                tags = PostgresUtils.postgresObjToHStore((PGobject) nodeRecord.getTags());

                Assert.assertNotNull(tags);
                Assert.assertEquals(1, tags.size());
                Assert.assertEquals("val 3", tags.get("key 3"));

                nodeRecord = nodes.get(nodeIdsArr[4]);
                Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
                Assert.assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
                Assert.assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
                Assert.assertEquals(nodeIdsArr[4], nodeRecord.getId());
                Assert.assertEquals(
                        new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                        nodeRecord.getTile());
                Assert.assertTrue(nodeRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), nodeRecord.getVersion());
                Assert.assertTrue(nodeRecord.getVisible());

                tags = PostgresUtils.postgresObjToHStore((PGobject) nodeRecord.getTags());

                Assert.assertNotNull(tags);
                Assert.assertEquals(1, tags.size());
                Assert.assertEquals("val 4", tags.get("key 4"));
            }
            catch (Exception e) {
                Assert.fail("Error checking nodes: " + e.getMessage());
            }

            try {
                Map<Long, CurrentWays> ways = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentWaysTbl)
                        .map(currentWaysTbl.id, currentWaysTbl);
                Assert.assertEquals(2, ways.size());

                CurrentWays wayRecord = ways.get(wayIdsArr[0]);

                Assert.assertEquals(new Long(changesetId), wayRecord.getChangesetId());
                Assert.assertEquals(wayIdsArr[0], wayRecord.getId());
                Assert.assertTrue(wayRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), wayRecord.getVersion());
                Assert.assertTrue(wayRecord.getVisible());

                List<CurrentWayNodes> wayNodes = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentWayNodesTbl)
                        .where(currentWayNodesTbl.wayId.eq(wayIdsArr[0]))
                        .orderBy(currentWayNodesTbl.sequenceId.asc())
                        .list(currentWayNodesTbl);

                Assert.assertEquals(3, wayNodes.size());

                CurrentWayNodes wayNode = wayNodes.get(0);

                Assert.assertEquals(nodeIdsArr[0], wayNode.getNodeId());
                Assert.assertEquals(new Long(1), wayNode.getSequenceId());
                Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());

                wayNode = wayNodes.get(1);

                Assert.assertEquals(nodeIdsArr[1], wayNode.getNodeId());
                Assert.assertEquals(new Long(2), wayNode.getSequenceId());
                Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());

                wayNode = wayNodes.get(2);

                Assert.assertEquals(nodeIdsArr[4], wayNode.getNodeId());
                Assert.assertEquals(new Long(3), wayNode.getSequenceId());
                Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());

                // verify the previously existing tags
                Map<String, String> tags = PostgresUtils.postgresObjToHStore((PGobject) wayRecord.getTags());

                Assert.assertNotNull(tags);
                Assert.assertEquals(2, tags.size());
                Assert.assertEquals("val 1", tags.get("key 1"));
                Assert.assertEquals("val 2", tags.get("key 2"));

                wayRecord = ways.get(wayIdsArr[1]);

                Assert.assertEquals(new Long(changesetId), wayRecord.getChangesetId());
                Assert.assertEquals(wayIdsArr[1], wayRecord.getId());
                Assert.assertTrue(wayRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), wayRecord.getVersion());
                Assert.assertTrue(wayRecord.getVisible());

                wayNodes = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentWayNodesTbl)
                        .where(currentWayNodesTbl.wayId.eq(wayIdsArr[1]))
                        .orderBy(currentWayNodesTbl.sequenceId.asc())
                        .list(currentWayNodesTbl);

                Assert.assertEquals(2, wayNodes.size());

                wayNode = wayNodes.get(0);

                Assert.assertEquals(nodeIdsArr[2], wayNode.getNodeId());
                Assert.assertEquals(new Long(1), wayNode.getSequenceId());
                Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());

                wayNode = wayNodes.get(1);

                Assert.assertEquals(nodeIdsArr[1], wayNode.getNodeId());
                Assert.assertEquals(new Long(2), wayNode.getSequenceId());
                Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());

                // verify the way with no tags
                Assert.assertTrue((wayRecord.getTags() == null)
                        || StringUtils.isEmpty(((PGobject) wayRecord.getTags()).getValue()));

                // verify the deleted way
                Assert.assertNull(new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentWaysTbl)
                        .where(currentWaysTbl.id.eq(wayIdsArr[2])).singleResult(currentWaysTbl));
            }
            catch (Exception e) {
                Assert.fail("Error checking ways: " + e.getMessage());
            }

            OsmTestUtils.verifyTestRelationsUnmodified(relationIds, wayIds, nodeIds, changesetId);

            try {
                changeset = new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(changesets)
                        .where(changesets.id.eq(changesetId)).singleResult(changesets);

                Assert.assertNotNull(changeset);
                Assert.assertTrue(changeset.getCreatedAt().before(now));
                Assert.assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));
                Assert.assertEquals(new Integer(13), changeset.getNumChanges());
                Assert.assertEquals(new Long(userId), changeset.getUserId());

                BoundingBox expandedBounds = new BoundingBox(originalBounds);
                expandedBounds.expand(originalBounds,
                        Double.parseDouble(HootProperties.getDefault("changesetBoundsExpansionFactorDeegrees")));
                Changeset hootChangeset = new Changeset(mapId, changesetId, conn);
                BoundingBox changesetBounds = hootChangeset.getBounds();

                Assert.assertEquals(changesetBounds, expandedBounds);
            }
            catch (Exception e) {
                Assert.fail("Error checking updated changeset: " + e.getMessage());
            }
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testUploadDeleteNodeStillBelongsToRelationWithIfUnused() throws Exception {
        try {
            BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
            long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
            Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
            Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
            Set<Long> relationIds = OsmTestUtils.createTestRelationsNoWays(changesetId, nodeIds);
            Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

            // Try to delete some elements with the "if-unused" tag is passed
            // where one of them is a node that still belongs to a relation. No failure should occur, the
            // node should not be deleted, and the other element should be deleted.
            Document responseData = null;
            try {
                responseData = resource()
                        .path("api/0.6/changeset/" + changesetId + "/upload")
                        .queryParam("mapId", String.valueOf(mapId))
                        .type(MediaType.TEXT_XML)
                        .accept(MediaType.TEXT_XML)
                        .post(Document.class,
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
                            "</osmChange>");
            }
            catch (UniformInterfaceException e) {
                ClientResponse r = e.getResponse();
                Assert.fail("Unexpected response " + r.getStatus() + " " + r.getEntity(String.class));
            }
            Assert.assertNotNull(responseData);

            XPath xpath = XmlUtils.createXPath();
            try {
                NodeList returnedNodes = XPathAPI.selectNodeList(responseData, "//osm/diffResult/node");

                Assert.assertEquals(1, returnedNodes.getLength());
                Assert.assertEquals((long) nodeIdsArr[3],
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@old_id", responseData)));
                org.w3c.dom.Node xml = XPathAPI.selectSingleNode(responseData, "//osm/diffResult/node[1]");

                Assert.assertNull(xml.getAttributes().getNamedItem("new_id"));
                Assert.assertNull(xml.getAttributes().getNamedItem("new_version"));

                NodeList returnedWays = XPathAPI.selectNodeList(responseData, "//osm/diffResult/way");
                Assert.assertEquals(0, returnedWays.getLength());

                NodeList returnedRelations = XPathAPI.selectNodeList(responseData, "//osm/diffResult/relation");
                Assert.assertEquals(0, returnedRelations.getLength());
            }
            catch (XPathExpressionException e) {
                Assert.fail("Error parsing response document: " + e.getMessage());
            }

            Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
            QChangesets changesets = QChangesets.changesets;
            Changesets changeset = new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(changesets)
                    .where(changesets.id.eq(changesetId)).singleResult(changesets);
            try {
                Map<Long, CurrentNodes> nodes = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentNodesTbl)
                        .map(currentNodesTbl.id, currentNodesTbl);
                Assert.assertEquals(4, nodes.size());

                CurrentNodes nodeRecord = nodes.get(nodeIdsArr[0]);
                Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
                Assert.assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
                Assert.assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
                Assert.assertEquals(nodeIdsArr[0], nodeRecord.getId());
                Assert.assertEquals(
                        new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                        nodeRecord.getTile());
                Assert.assertTrue(nodeRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), nodeRecord.getVersion());
                Assert.assertTrue(nodeRecord.getVisible());

                Map<String, String> tags = PostgresUtils.postgresObjToHStore((PGobject) nodeRecord.getTags());

                Assert.assertNotNull(tags);
                Assert.assertEquals(2, tags.size());
                Assert.assertEquals("val 1", tags.get("key 1"));
                Assert.assertEquals("val 2", tags.get("key 2"));

                nodeRecord = nodes.get(nodeIdsArr[1]);

                Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
                Assert.assertEquals(new Double(originalBounds.getMaxLat()), nodeRecord.getLatitude());
                Assert.assertEquals(new Double(originalBounds.getMaxLon()), nodeRecord.getLongitude());
                Assert.assertEquals(nodeIdsArr[1], nodeRecord.getId());
                Assert.assertEquals(
                        new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                        nodeRecord.getTile());
                Assert.assertTrue(nodeRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), nodeRecord.getVersion());
                Assert.assertTrue(nodeRecord.getVisible());
                Assert.assertTrue((nodeRecord.getTags() == null)
                        || StringUtils.isEmpty(((PGobject) nodeRecord.getTags()).getValue()));

                nodeRecord = nodes.get(nodeIdsArr[2]);

                Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
                Assert.assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
                Assert.assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
                Assert.assertEquals(nodeIdsArr[2], nodeRecord.getId());
                Assert.assertEquals(
                        new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                        nodeRecord.getTile());
                Assert.assertTrue(nodeRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), nodeRecord.getVersion());
                Assert.assertTrue(nodeRecord.getVisible());
                Assert.assertTrue((nodeRecord.getTags() == null)
                        || StringUtils.isEmpty(((PGobject) nodeRecord.getTags()).getValue()));

                Assert.assertNull(nodes.get(nodeIdsArr[3]));

                nodeRecord = nodes.get(nodeIdsArr[4]);

                Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
                Assert.assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
                Assert.assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
                Assert.assertEquals(nodeIdsArr[4], nodeRecord.getId());
                Assert.assertEquals(
                        new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                        nodeRecord.getTile());
                Assert.assertTrue(nodeRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), nodeRecord.getVersion());
                Assert.assertTrue(nodeRecord.getVisible());

                tags = PostgresUtils.postgresObjToHStore((PGobject) nodeRecord.getTags());

                Assert.assertNotNull(tags);
                Assert.assertEquals(1, tags.size());
                Assert.assertEquals("val 4", tags.get("key 4"));
            }
            catch (Exception e) {
                Assert.fail("Error checking nodes: " + e.getMessage());
            }

            try {
                List<CurrentRelations> relations = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentRelationsTbl).list(currentRelationsTbl);

                Assert.assertEquals(3, relations.size());

                CurrentRelations relationRecord = relations.get(0);

                Assert.assertEquals(new Long(changesetId), relationRecord.getChangesetId());
                Assert.assertEquals(relationIdsArr[0], relationRecord.getId());
                Assert.assertTrue(relationRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), relationRecord.getVersion());
                Assert.assertTrue(relationRecord.getVisible());

                List<CurrentRelationMembers> members = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentRelationMembersTbl)
                        .where(currentRelationMembersTbl.relationId.eq(relationIdsArr[0]))
                        .orderBy(currentRelationMembersTbl.sequenceId.asc()).list(currentRelationMembersTbl);

                Assert.assertEquals(2, members.size());

                CurrentRelationMembers member = members.get(0);

                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
                Assert.assertEquals("role1", member.getMemberRole());
                Assert.assertEquals(new Integer(1), member.getSequenceId());
                Assert.assertEquals(nodeIdsArr[0], member.getMemberId());

                member = members.get(1);

                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
                Assert.assertEquals("", member.getMemberRole());
                Assert.assertEquals(new Integer(2), member.getSequenceId());

                Assert.assertEquals(nodeIdsArr[2], member.getMemberId());

                relationRecord = relations.get(1);

                Assert.assertEquals(new Long(changesetId), relationRecord.getChangesetId());
                Assert.assertEquals(relationIdsArr[1], relationRecord.getId());
                Assert.assertTrue(relationRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), relationRecord.getVersion());
                Assert.assertTrue(relationRecord.getVisible());

                members = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentRelationMembersTbl)
                        .where(currentRelationMembersTbl.relationId.eq(relationIdsArr[1]))
                        .orderBy(currentRelationMembersTbl.sequenceId.asc())
                        .list(currentRelationMembersTbl);

                Assert.assertEquals(2, members.size());

                member = members.get(0);

                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
                Assert.assertEquals("role1", member.getMemberRole());
                Assert.assertEquals(new Integer(1), member.getSequenceId());

                Assert.assertEquals(nodeIdsArr[4], member.getMemberId());

                member = members.get(1);

                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.relation, member.getMemberType());
                Assert.assertEquals("role1", member.getMemberRole());
                Assert.assertEquals(new Integer(2), member.getSequenceId());
                Assert.assertEquals(relationIdsArr[0], member.getMemberId());

                relationRecord = relations.get(2);

                Assert.assertEquals(new Long(changesetId), relationRecord.getChangesetId());
                Assert.assertEquals(relationIdsArr[2], relationRecord.getId());
                Assert.assertTrue(relationRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), relationRecord.getVersion());
                Assert.assertTrue(relationRecord.getVisible());

                members = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentRelationMembersTbl)
                        .where(currentRelationMembersTbl.relationId.eq(relationIdsArr[2]))
                        .orderBy(currentRelationMembersTbl.sequenceId.asc())
                        .list(currentRelationMembersTbl);

                Assert.assertEquals(1, members.size());

                member = members.get(0);

                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
                Assert.assertEquals("role1", member.getMemberRole());
                Assert.assertEquals(new Integer(1), member.getSequenceId());
                Assert.assertEquals(nodeIdsArr[2], member.getMemberId());
            }
            catch (Exception e) {
                Assert.fail("Error checking relations: " + e.getMessage());
            }

            OsmTestUtils.verifyTestRelationsNoWaysUnmodified(relationIds, nodeIds, changesetId, true);

            try {
                changeset = new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(changesets)
                        .where(changesets.id.eq(changesetId)).singleResult(changesets);

                Assert.assertNotNull(changeset);
                Assert.assertTrue(changeset.getCreatedAt().before(now));
                Assert.assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));
                Assert.assertEquals(new Integer(13), changeset.getNumChanges());
                Assert.assertEquals(new Long(userId), changeset.getUserId());

                BoundingBox expandedBounds = new BoundingBox(originalBounds);
                expandedBounds.expand(originalBounds,
                        Double.parseDouble(HootProperties.getDefault("changesetBoundsExpansionFactorDeegrees")));
                Changeset hootChangeset = new Changeset(mapId, changesetId, conn);
                BoundingBox changesetBounds = hootChangeset.getBounds();

                Assert.assertEquals(changesetBounds, expandedBounds);
            }
            catch (Exception e) {
                Assert.fail("Error checking updated changeset: " + e.getMessage());
            }
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testUploadDeleteWayStillBelongsToRelationWithIfUnused() throws Exception {
        try {
            BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
            long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
            Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
            Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
            Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
            Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
            Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

            // Try to delete some elements with the "if-unused" tag is passed
            // where one of them is a way that still belongs to a relation. No failure should occur, the
            // way should not be deleted, and the other element should be deleted.
            Document responseData = null;
            try {
                responseData = resource().path("api/0.6/changeset/" + changesetId + "/upload")
                        .queryParam("mapId", String.valueOf(mapId))
                        .type(MediaType.TEXT_XML)
                        .accept(MediaType.TEXT_XML)
                        .post(Document.class,
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
                            "</osmChange>");
            }
            catch (UniformInterfaceException e) {
                ClientResponse r = e.getResponse();
                Assert.fail("Unexpected response " + r.getStatus() + " " + r.getEntity(String.class));
            }
            Assert.assertNotNull(responseData);

            XPath xpath = XmlUtils.createXPath();
            try {
                NodeList returnedNodes = XPathAPI.selectNodeList(responseData, "//osm/diffResult/node");
                Assert.assertEquals(1, returnedNodes.getLength());

                Assert.assertEquals((long) nodeIdsArr[3],
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@old_id", responseData)));

                org.w3c.dom.Node xml = XPathAPI.selectSingleNode(responseData, "//osm/diffResult/node[1]");

                Assert.assertNull(xml.getAttributes().getNamedItem("new_id"));
                Assert.assertNull(xml.getAttributes().getNamedItem("new_version"));

                NodeList returnedWays = XPathAPI.selectNodeList(responseData, "//osm/diffResult/way");

                Assert.assertEquals(0, returnedWays.getLength());
            }
            catch (XPathExpressionException e) {
                Assert.fail("Error parsing response document: " + e.getMessage());
            }

            Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
            QChangesets changesets = QChangesets.changesets;
            Changesets changeset = new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(changesets)
                    .where(changesets.id.eq(changesetId)).singleResult(changesets);
            try {
                Map<Long, CurrentNodes> nodes = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentNodesTbl)
                        .map(currentNodesTbl.id, currentNodesTbl);

                Assert.assertEquals(4, nodes.size());

                CurrentNodes nodeRecord = nodes.get(nodeIdsArr[0]);

                Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
                Assert.assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
                Assert.assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
                Assert.assertEquals(nodeIdsArr[0], nodeRecord.getId());
                Assert.assertEquals(
                        new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                        nodeRecord.getTile());
                Assert.assertTrue(nodeRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), nodeRecord.getVersion());
                Assert.assertTrue(nodeRecord.getVisible());

                Map<String, String> tags = PostgresUtils.postgresObjToHStore((PGobject) nodeRecord.getTags());

                Assert.assertNotNull(tags);
                Assert.assertEquals(2, tags.size());
                Assert.assertEquals("val 1", tags.get("key 1"));
                Assert.assertEquals("val 2", tags.get("key 2"));

                nodeRecord = nodes.get(nodeIdsArr[1]);
                Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
                Assert.assertEquals(new Double(originalBounds.getMaxLat()), nodeRecord.getLatitude());
                Assert.assertEquals(new Double(originalBounds.getMaxLon()), nodeRecord.getLongitude());
                Assert.assertEquals(nodeIdsArr[1], nodeRecord.getId());
                Assert.assertEquals(
                        new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                        nodeRecord.getTile());
                Assert.assertTrue(nodeRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), nodeRecord.getVersion());
                Assert.assertTrue(nodeRecord.getVisible());
                Assert.assertTrue((nodeRecord.getTags() == null)
                        || StringUtils.isEmpty(((PGobject) nodeRecord.getTags()).getValue()));

                nodeRecord = nodes.get(nodeIdsArr[2]);
                Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
                Assert.assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
                Assert.assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
                Assert.assertEquals(nodeIdsArr[2], nodeRecord.getId());
                Assert.assertEquals(
                        new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                        nodeRecord.getTile());
                Assert.assertTrue(nodeRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), nodeRecord.getVersion());
                Assert.assertTrue(nodeRecord.getVisible());
                Assert.assertTrue((nodeRecord.getTags() == null)
                        || StringUtils.isEmpty(((PGobject) nodeRecord.getTags()).getValue()));

                Assert.assertNull(nodes.get(nodeIdsArr[3]));

                nodeRecord = nodes.get(nodeIdsArr[4]);

                Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
                Assert.assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
                Assert.assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
                Assert.assertEquals(nodeIdsArr[4], nodeRecord.getId());
                Assert.assertEquals(
                        new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                        nodeRecord.getTile());
                Assert.assertTrue(nodeRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), nodeRecord.getVersion());
                Assert.assertTrue(nodeRecord.getVisible());

                tags = PostgresUtils.postgresObjToHStore((PGobject) nodeRecord.getTags());

                Assert.assertNotNull(tags);
                Assert.assertEquals(1, tags.size());
                Assert.assertEquals("val 4", tags.get("key 4"));
            }
            catch (Exception e) {
                Assert.fail("Error checking nodes: " + e.getMessage());
            }

            OsmTestUtils.verifyTestWaysUnmodified(wayIds, nodeIds, changesetId);
            OsmTestUtils.verifyTestRelationsUnmodified(relationIds, wayIds, nodeIds, changesetId);

            try {
                changeset = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(changesets)
                        .where(changesets.id.eq(changesetId))
                        .singleResult(changesets);

                Assert.assertNotNull(changeset);
                Assert.assertTrue(changeset.getCreatedAt().before(now));
                Assert.assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));
                Assert.assertEquals(new Integer(13), changeset.getNumChanges());
                Assert.assertEquals(new Long(userId), changeset.getUserId());

                BoundingBox expandedBounds = new BoundingBox(originalBounds);
                expandedBounds.expand(originalBounds,
                        Double.parseDouble(HootProperties.getDefault("changesetBoundsExpansionFactorDeegrees")));
                Changeset hootChangeset = new Changeset(mapId, changesetId, conn);
                BoundingBox changesetBounds = hootChangeset.getBounds();

                Assert.assertEquals(changesetBounds, expandedBounds);
            }
            catch (Exception e) {
                Assert.fail("Error checking updated changeset: " + e.getMessage());
            }
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testUploadDeleteRelationStillBelongsToRelationWithIfUnused() throws Exception {
        try {
            BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
            long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
            Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
            Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
            Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
            Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
            Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
            Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

            // Try to delete some elements with the "if-unused" tag is passed
            // where one of them is a relation that still belongs to a relation. No failure should
            // occur, the relation should not be deleted, and the other element should be deleted.
            Document responseData = null;
            try {
                responseData = resource()
                        .path("api/0.6/changeset/" + changesetId + "/upload")
                        .queryParam("mapId", String.valueOf(mapId))
                        .type(MediaType.TEXT_XML)
                        .accept(MediaType.TEXT_XML)
                        .post(Document.class,
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
                            "</osmChange>");
            }
            catch (UniformInterfaceException e) {
                ClientResponse r = e.getResponse();
                Assert.fail("Unexpected response " + r.getStatus() + " " + r.getEntity(String.class));
            }
            Assert.assertNotNull(responseData);

            XPath xpath = XmlUtils.createXPath();
            try {
                NodeList returnedNodes = XPathAPI.selectNodeList(responseData, "//osm/diffResult/node");
                Assert.assertEquals(0, returnedNodes.getLength());

                NodeList returnedWays = XPathAPI.selectNodeList(responseData, "//osm/diffResult/way");
                Assert.assertEquals(0, returnedWays.getLength());

                NodeList returnedRelations = XPathAPI.selectNodeList(responseData, "//osm/diffResult/relation");

                Assert.assertEquals(1, returnedRelations.getLength());
                Assert.assertEquals((long) relationIdsArr[2],
                        Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@old_id", responseData)));
                org.w3c.dom.Node xml = XPathAPI.selectSingleNode(responseData, "//osm/diffResult/relation[1]");

                Assert.assertNull(xml.getAttributes().getNamedItem("new_id"));
                Assert.assertNull(xml.getAttributes().getNamedItem("new_version"));
            }
            catch (XPathExpressionException e) {
                Assert.fail("Error parsing response document: " + e.getMessage());
            }

            QChangesets changesets = QChangesets.changesets;
            Changesets changeset = new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(changesets)
                    .where(changesets.id.eq(changesetId)).singleResult(changesets);

            OsmTestUtils.verifyTestNodesUnmodified(nodeIds, changesetId, originalBounds);
            OsmTestUtils.verifyTestWaysUnmodified(wayIds, nodeIds, changesetId);

            Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
            try {
                Map<Long, CurrentRelations> relations = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentRelationsTbl)
                        .map(currentRelationsTbl.id, currentRelationsTbl);
                Assert.assertEquals(3, relations.size());

                // verify the previously existing relations
                CurrentRelations relationRecord = relations.get(relationIdsArr[0]);

                Assert.assertEquals(new Long(changesetId), relationRecord.getChangesetId());
                Assert.assertEquals(relationIdsArr[0], relationRecord.getId());
                Assert.assertTrue(relationRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), relationRecord.getVersion());
                Assert.assertTrue(relationRecord.getVisible());

                List<CurrentRelationMembers> members = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentRelationMembersTbl)
                        .where(currentRelationMembersTbl.relationId.eq(relationIdsArr[0]))
                        .orderBy(currentRelationMembersTbl.sequenceId.asc())
                        .list(currentRelationMembersTbl);

                Assert.assertEquals(4, members.size());

                CurrentRelationMembers member = members.get(0);

                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
                Assert.assertEquals("role1", member.getMemberRole());
                Assert.assertEquals(new Integer(1), member.getSequenceId());
                Assert.assertEquals(nodeIdsArr[0], member.getMemberId());

                member = members.get(1);

                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.way, member.getMemberType());
                Assert.assertEquals("role3", member.getMemberRole());
                Assert.assertEquals(new Integer(2), member.getSequenceId());
                Assert.assertEquals(wayIdsArr[1], member.getMemberId());

                member = members.get(2);

                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.way, member.getMemberType());
                Assert.assertEquals("role2", member.getMemberRole());
                Assert.assertEquals(new Integer(3), member.getSequenceId());
                Assert.assertEquals(wayIdsArr[0], member.getMemberId());

                member = members.get(3);

                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
                Assert.assertEquals("", member.getMemberRole());
                Assert.assertEquals(new Integer(4), member.getSequenceId());
                Assert.assertEquals(nodeIdsArr[2], member.getMemberId());

                Map<String, String> tags = PostgresUtils.postgresObjToHStore((PGobject) relationRecord.getTags());

                Assert.assertNotNull(tags);
                Assert.assertEquals(1, tags.size());
                Assert.assertEquals("val 1", tags.get("key 1"));

                relationRecord = relations.get(relationIdsArr[1]);

                Assert.assertEquals(new Long(changesetId), relationRecord.getChangesetId());
                Assert.assertEquals(relationIdsArr[1], relationRecord.getId());
                Assert.assertTrue(relationRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), relationRecord.getVersion());
                Assert.assertTrue(relationRecord.getVisible());

                members = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentRelationMembersTbl)
                        .where(currentRelationMembersTbl.relationId.eq(relationIdsArr[1]))
                        .orderBy(currentRelationMembersTbl.sequenceId.asc())
                        .list(currentRelationMembersTbl);

                Assert.assertEquals(2, members.size());

                member = members.get(0);

                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
                Assert.assertEquals("role1", member.getMemberRole());
                Assert.assertEquals(new Integer(1), member.getSequenceId());
                Assert.assertEquals(nodeIdsArr[4], member.getMemberId());

                member = members.get(1);

                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.relation, member.getMemberType());
                Assert.assertEquals("role1", member.getMemberRole());
                Assert.assertEquals(new Integer(2), member.getSequenceId());
                Assert.assertEquals(relationIdsArr[0], member.getMemberId());

                tags = PostgresUtils.postgresObjToHStore((PGobject) relationRecord.getTags());

                Assert.assertNotNull(tags);
                Assert.assertEquals(2, tags.size());
                Assert.assertEquals("val 2", tags.get("key 2"));
                Assert.assertEquals("val 3", tags.get("key 3"));
                Assert.assertNull(relations.get(relationIdsArr[2]));

                relationRecord = relations.get(relationIdsArr[3]);

                Assert.assertEquals(new Long(changesetId), relationRecord.getChangesetId());
                Assert.assertEquals(relationIdsArr[3], relationRecord.getId());
                Assert.assertTrue(relationRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), relationRecord.getVersion());
                Assert.assertTrue(relationRecord.getVisible());

                members = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentRelationMembersTbl)
                        .where(currentRelationMembersTbl.relationId.eq(relationIdsArr[3]))
                        .orderBy(currentRelationMembersTbl.sequenceId.asc())
                        .list(currentRelationMembersTbl);

                Assert.assertEquals(1, members.size());

                member = members.get(0);

                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
                Assert.assertEquals("role1", member.getMemberRole());
                Assert.assertEquals(new Integer(1), member.getSequenceId());
                Assert.assertEquals(nodeIdsArr[2], member.getMemberId());
                Assert.assertTrue((relationRecord.getTags() == null)
                        || StringUtils.isEmpty(((PGobject) relationRecord.getTags()).getValue()));
            }
            catch (Exception e) {
                Assert.fail("Error checking relations: " + e.getMessage());
            }

            try {
                changeset = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(changesets)
                        .where(changesets.id.eq(changesetId))
                        .singleResult(changesets);

                Assert.assertNotNull(changeset);
                Assert.assertTrue(changeset.getCreatedAt().before(now));
                Assert.assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));

                Assert.assertEquals(new Integer(13), changeset.getNumChanges());
                Assert.assertEquals(new Long(userId), changeset.getUserId());

                BoundingBox expandedBounds = new BoundingBox(originalBounds);
                expandedBounds.expand(originalBounds,
                        Double.parseDouble(HootProperties.getDefault("changesetBoundsExpansionFactorDeegrees")));
                Changeset hootChangeset = new Changeset(mapId, changesetId, conn);
                BoundingBox changesetBounds = hootChangeset.getBounds();
                Assert.assertEquals(changesetBounds, expandedBounds);
            }
            catch (Exception e) {
                Assert.fail("Error checking updated changeset: " + e.getMessage());
            }
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadDeleteNodeStillBelongsToWayWithoutIfUnused() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Try to delete a node that still belongs to a way when the "if-unused"
        // tag is not passed. A failure should occur and no data in the system should be modified.
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
                            "<delete>" +
                                "<node id=\"" + nodeIdsArr[1] + "\" lon=\"" + originalBounds.getMinLon() + "\" " +
                                     "lat=\"" + originalBounds.getMinLat() + "\" version=\"1\" changeset=\"" + changesetId + "\">" +
                                "</node>" +
                            "</delete>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.PRECONDITION_FAILED, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("still used by other way(s)"));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadDeleteNodeStillBelongsToRelationWithoutIfUnused() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> relationIds = OsmTestUtils.createTestRelationsNoWays(changesetId, nodeIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // Try to delete a node that still belongs to a relation when the
        // "if-unused" tag is not passed.  A failure should occur and no data in the system should be modified.
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
                            "<delete>" +
                                "<node id=\"" + nodeIdsArr[0] + "\" lon=\"" + originalBounds.getMinLon() + "\" " +
                                    "lat=\"" + originalBounds.getMinLat() + "\" version=\"1\" changeset=\"" + changesetId + "\">" +
                                "</node>" +
                            "</delete>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();

            Assert.assertEquals(Status.PRECONDITION_FAILED, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("still used by other relation(s)"));

            QChangesets changesets = QChangesets.changesets;

            new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                    .from(changesets)
                    .where(changesets.id.eq(changesetId))
                    .singleResult(changesets);

            OsmTestUtils.verifyTestChangesetUnmodified(changesetId, originalBounds);
            OsmTestUtils.verifyTestNodesUnmodified(nodeIds, changesetId, originalBounds);

            try {
                List<CurrentRelations> relations = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentRelationsTbl)
                        .orderBy(currentRelationsTbl.id.asc())
                        .list(currentRelationsTbl);

                Assert.assertEquals(3, relations.size());

                CurrentRelations relationRecord = relations.get(0);
                Assert.assertEquals(new Long(changesetId), relationRecord.getChangesetId());
                Assert.assertEquals(relationIdsArr[0], relationRecord.getId());

                Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());

                Assert.assertTrue(relationRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), relationRecord.getVersion());
                Assert.assertTrue(relationRecord.getVisible());

                List<CurrentRelationMembers> members = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentRelationMembersTbl)
                        .where(currentRelationMembersTbl.relationId.eq(relationIdsArr[0]))
                        .orderBy(currentRelationMembersTbl.sequenceId.asc())
                        .list(currentRelationMembersTbl);

                Assert.assertEquals(2, members.size());

                CurrentRelationMembers member = members.get(0);

                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
                Assert.assertEquals("role1", member.getMemberRole());
                Assert.assertEquals(new Integer(1), member.getSequenceId());

                Assert.assertEquals(nodeIdsArr[0], member.getMemberId());

                member = members.get(1);

                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
                Assert.assertEquals("", member.getMemberRole());
                Assert.assertEquals(new Integer(2), member.getSequenceId());
                Assert.assertEquals(nodeIdsArr[2], member.getMemberId());

                relationRecord = relations.get(1);

                Assert.assertEquals(new Long(changesetId), relationRecord.getChangesetId());
                Assert.assertEquals(relationIdsArr[1], relationRecord.getId());

                Assert.assertTrue(relationRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), relationRecord.getVersion());
                Assert.assertTrue(relationRecord.getVisible());

                members = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentRelationMembersTbl)
                        .where(currentRelationMembersTbl.relationId.eq(relationIdsArr[1]))
                        .orderBy(currentRelationMembersTbl.sequenceId.asc())
                        .list(currentRelationMembersTbl);

                Assert.assertEquals(2, members.size());

                member = members.get(0);

                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
                Assert.assertEquals("role1", member.getMemberRole());
                Assert.assertEquals(new Integer(1), member.getSequenceId());
                Assert.assertEquals(nodeIdsArr[4], member.getMemberId());

                member = members.get(1);

                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.relation, member.getMemberType());
                Assert.assertEquals("role1", member.getMemberRole());
                Assert.assertEquals(new Integer(2), member.getSequenceId());
                Assert.assertEquals(relationIdsArr[0], member.getMemberId());

                relationRecord = relations.get(2);

                Assert.assertEquals(new Long(changesetId), relationRecord.getChangesetId());
                Assert.assertEquals(relationIdsArr[2], relationRecord.getId());

                Assert.assertTrue(relationRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), relationRecord.getVersion());
                Assert.assertTrue(relationRecord.getVisible());

                members = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentRelationMembersTbl)
                        .where(currentRelationMembersTbl.relationId.eq(relationIdsArr[2]))
                        .orderBy(currentRelationMembersTbl.sequenceId.asc())
                        .list(currentRelationMembersTbl);

                Assert.assertEquals(1, members.size());

                member = members.get(0);

                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
                Assert.assertEquals("role1", member.getMemberRole());
                Assert.assertEquals(new Integer(1), member.getSequenceId());
                Assert.assertEquals(nodeIdsArr[2], member.getMemberId());
            }
            catch (Exception e2) {
                Assert.fail("Error checking relations: " + e2.getMessage());
            }

            OsmTestUtils.verifyTestRelationsNoWaysUnmodified(relationIds, nodeIds, changesetId, true);

            throw e;
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadDeleteWayStillBelongsToRelationWithoutIfUnused() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Try to delete a way that still belongs to a relation when the
        // "if-unused" tag is not passed. A failure should occur and no data in the system should be modified.
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
                            "<delete>" +
                                "<way id=\"" + wayIdsArr[1] + "\" version=\"1\" changeset=\"" + changesetId + "\">" + "</way>" +
                            "</delete>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.PRECONDITION_FAILED, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("still used by other relation(s)"));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadDeleteRelationStillBelongsToRelationWithoutIfUnused() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // Try to delete a relation that still belongs to a relation when the
        // "if-unused" tag is not passed. A failure should occur and no data in the system should be modified.
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
                            "<delete>" +
                                "<relation id=\"" + relationIdsArr[0] + "\" version=\"1\" changeset=\"" +
                                    changesetId + "\">" +
                                "</relation>" +
                            "</delete>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.PRECONDITION_FAILED, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("still used by other relation(s)"));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadDeleteRelationEmptyMemberId() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Update the changeset where one of the relations has a member with an
        // empty string for an ID.  A failure should occur and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML).accept(MediaType.TEXT_XML)
                    .post(Document.class,
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
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadDeleteRelationMissingMemberId() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Update the changeset with a relation that has a member with a missing
        // ID attribute. A failure should occur and no data in the system should be modified.
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
                                "<relation id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                    "<member type=\"node\" role=\"role1\"\"></member>" +
                                    "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                                "</relation>" +
                                "<relation id=\"-2\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                    "<member type=\"node\" role=\"role1\"></member>" + // mising member ID
                                    "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                                "</relation>" +
                            "</delete>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }
}
