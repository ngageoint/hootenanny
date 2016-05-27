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
import com.mysema.query.sql.dml.SQLUpdateClause;
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
import hoot.services.utils.HootCustomPropertiesSetter;
import hoot.services.utils.RandomNumberGenerator;
import hoot.services.utils.XmlUtils;


public class ChangesetResourceUploadModifyTest extends OsmResourceTestAbstract {
    private static final Logger log = LoggerFactory.getLogger(ChangesetResourceUploadModifyTest.class);

    private final QCurrentNodes currentNodesTbl = QCurrentNodes.currentNodes;
    private final QCurrentWays currentWaysTbl = QCurrentWays.currentWays;
    private final QCurrentWayNodes currentWayNodesTbl = QCurrentWayNodes.currentWayNodes;
    private final QCurrentRelations currentRelationsTbl = QCurrentRelations.currentRelations;
    private final QCurrentRelationMembers currentRelationMembersTbl = QCurrentRelationMembers.currentRelationMembers;

    public ChangesetResourceUploadModifyTest() {
        super("hoot.services.controllers.osm");
    }

    @Test
    @Category(UnitTest.class)
    public void testUploadModify() throws Exception {

        try {
            BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
            long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
            Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
            Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
            Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
            Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
            Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
            Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

            // Now, update some of the elements and their tags.
            BoundingBox updatedBounds = OsmTestUtils.createAfterModifiedTestChangesetBounds();
            Document responseData = null;
            try {
                responseData = resource()
                        .path("api/0.6/changeset/" + changesetId + "/upload")
                        .queryParam("mapId", String.valueOf(mapId))
                        .type(MediaType.TEXT_XML).accept(MediaType.TEXT_XML)
                        .post(Document.class,
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

                Assert.assertEquals((long) nodeIdsArr[0],
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@old_id", responseData)));
                Assert.assertEquals(Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@old_id", responseData)),
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@new_id", responseData)));
                Assert.assertEquals(2,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@new_version", responseData)));
                Assert.assertEquals((long) nodeIdsArr[1],
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@old_id", responseData)));
                Assert.assertEquals(Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@old_id", responseData)),
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@new_id", responseData)));
                Assert.assertEquals(2,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@new_version", responseData)));

                NodeList returnedWays = XPathAPI.selectNodeList(responseData, "//osm/diffResult/way");

                Assert.assertEquals(2, returnedWays.getLength());
                Assert.assertEquals((long) wayIdsArr[0],
                        Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@old_id", responseData)));
                Assert.assertEquals(Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@old_id", responseData)),
                        Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@new_id", responseData)));
                Assert.assertEquals(2,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@new_version", responseData)));
                Assert.assertEquals((long) wayIdsArr[1],
                        Long.parseLong(xpath.evaluate("//osm/diffResult/way[2]/@old_id", responseData)));
                Assert.assertEquals(Long.parseLong(xpath.evaluate("//osm/diffResult/way[2]/@old_id", responseData)),
                        Long.parseLong(xpath.evaluate("//osm/diffResult/way[2]/@new_id", responseData)));
                Assert.assertEquals(2,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/way[2]/@new_version", responseData)));

                NodeList returnedRelations = XPathAPI.selectNodeList(responseData, "//osm/diffResult/relation");
                Assert.assertEquals(2, returnedRelations.getLength());

                // check the modified relations
                Assert.assertEquals((long) relationIdsArr[0],
                        Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@old_id", responseData)));
                Assert.assertEquals(
                        Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@old_id", responseData)),
                        Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@new_id", responseData)));
                Assert.assertEquals(2,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@new_version", responseData)));

                Assert.assertEquals((long) relationIdsArr[1],
                        Long.parseLong(xpath.evaluate("//osm/diffResult/relation[2]/@old_id", responseData)));
                Assert.assertEquals(
                        Long.parseLong(xpath.evaluate("//osm/diffResult/relation[2]/@old_id", responseData)),
                        Long.parseLong(xpath.evaluate("//osm/diffResult/relation[2]/@new_id", responseData)));
                Assert.assertEquals(2,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/relation[2]/@new_version", responseData)));
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
                Assert.assertEquals(5, nodes.size());

                CurrentNodes nodeRecord = nodes.get(nodeIdsArr[0]);
                Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
                Assert.assertEquals(new Double(updatedBounds.getMinLat()), nodeRecord.getLatitude());
                Assert.assertEquals(new Double(updatedBounds.getMinLon()), nodeRecord.getLongitude());
                Assert.assertEquals(nodeIdsArr[0], nodeRecord.getId());
                Assert.assertEquals(
                        new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                        nodeRecord.getTile());
                Assert.assertTrue(nodeRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(2), nodeRecord.getVersion());
                Assert.assertTrue(nodeRecord.getVisible());
                Map<String, String> tags = PostgresUtils.postgresObjToHStore((PGobject) nodeRecord.getTags());
                Assert.assertNotNull(tags);
                Assert.assertEquals(2, tags.size());
                Assert.assertEquals("val 1b", tags.get("key 1b"));
                Assert.assertEquals("val 2b", tags.get("key 2b"));

                nodeRecord = nodes.get(nodeIdsArr[1]);
                Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
                Assert.assertEquals(new Double(updatedBounds.getMinLat()), nodeRecord.getLatitude());
                Assert.assertEquals(new Double(originalBounds.getMaxLon()), nodeRecord.getLongitude());
                Assert.assertEquals(nodeIdsArr[1], nodeRecord.getId());
                Assert.assertEquals(
                        new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                        nodeRecord.getTile());
                Assert.assertTrue(nodeRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(2), nodeRecord.getVersion());
                Assert.assertTrue(nodeRecord.getVisible());
                tags = PostgresUtils.postgresObjToHStore((PGobject) nodeRecord.getTags());
                Assert.assertNotNull(tags);
                Assert.assertEquals(1, tags.size());
                Assert.assertEquals("val 3b", tags.get("key 3b"));

                nodeRecord = nodes.get(nodeIdsArr[2]);
                Assert.assertTrue((nodeRecord.getTags() == null)
                        || StringUtils.isEmpty(((PGobject) nodeRecord.getTags()).getValue()));

                nodeRecord = nodes.get(nodeIdsArr[3]);
                tags = PostgresUtils.postgresObjToHStore((PGobject) nodeRecord.getTags());
                Assert.assertNotNull(tags);
                Assert.assertEquals(1, tags.size());
                Assert.assertEquals("val 3", tags.get("key 3"));

                nodeRecord = nodes.get(nodeIdsArr[4]);
                tags = PostgresUtils.postgresObjToHStore((PGobject) nodeRecord.getTags());
                Assert.assertNotNull(tags);
                Assert.assertEquals(1, tags.size());
                Assert.assertEquals("val 4", tags.get("key 4"));
            }
            catch (Exception e) {
                Assert.fail("Error checking nodes: " + e.getMessage());
            }

            try {
                Map<Long, CurrentWays> ways = new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentWaysTbl)
                        .map(currentWaysTbl.id, currentWaysTbl);

                Assert.assertEquals(3, ways.size());

                CurrentWays wayRecord = ways.get(wayIdsArr[0]);
                Assert.assertEquals(new Long(changesetId), wayRecord.getChangesetId());
                Assert.assertEquals(wayIdsArr[0], wayRecord.getId());
                Assert.assertTrue(wayRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(2), wayRecord.getVersion());
                Assert.assertTrue(wayRecord.getVisible());
                List<CurrentWayNodes> wayNodes =
                        new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                                .from(currentWayNodesTbl)
                                .where(currentWayNodesTbl.wayId.eq(wayIdsArr[0]))
                                .orderBy(currentWayNodesTbl.sequenceId.asc()).list(currentWayNodesTbl);

                Assert.assertEquals(2, wayNodes.size());
                CurrentWayNodes wayNode = wayNodes.get(0);
                Assert.assertEquals(nodeIdsArr[0], wayNode.getNodeId());
                Assert.assertEquals(new Long(1), wayNode.getSequenceId());
                Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());
                wayNode = wayNodes.get(1);
                Assert.assertEquals(nodeIdsArr[4], wayNode.getNodeId());
                Assert.assertEquals(new Long(2), wayNode.getSequenceId());
                Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());
                // verify the previously existing tags
                Map<String, String> tags = PostgresUtils.postgresObjToHStore((PGobject) wayRecord.getTags());
                Assert.assertNotNull(tags);
                Assert.assertEquals(1, tags.size());
                Assert.assertEquals("val 2", tags.get("key 2"));

                wayRecord = ways.get(wayIdsArr[1]);
                Assert.assertEquals(new Long(changesetId), wayRecord.getChangesetId());
                Assert.assertEquals(wayIdsArr[1], wayRecord.getId());
                Assert.assertTrue(wayRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(2), wayRecord.getVersion());
                Assert.assertTrue(wayRecord.getVisible());

                wayNodes = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                                .from(currentWayNodesTbl)
                                .where(currentWayNodesTbl.wayId.eq(wayIdsArr[1]))
                                .orderBy(currentWayNodesTbl.sequenceId.asc())
                                .list(currentWayNodesTbl);

                Assert.assertEquals(2, wayNodes.size());

                wayNode = wayNodes.get(0);

                Assert.assertEquals(nodeIdsArr[4], wayNode.getNodeId());
                Assert.assertEquals(new Long(1), wayNode.getSequenceId());
                Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());

                wayNode = wayNodes.get(1);

                Assert.assertEquals(nodeIdsArr[2], wayNode.getNodeId());
                Assert.assertEquals(new Long(2), wayNode.getSequenceId());
                Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());
                // verify the way with no tags
                Assert.assertTrue((wayRecord.getTags() == null)
                        || StringUtils.isEmpty(((PGobject) wayRecord.getTags()).getValue()));

                // verify the created ways
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

                // verify the created tags
                tags = PostgresUtils.postgresObjToHStore((PGobject) wayRecord.getTags());

                Assert.assertNotNull(tags);
                Assert.assertEquals(1, tags.size());
                Assert.assertEquals("val 3", tags.get("key 3"));
            }
            catch (Exception e) {
                Assert.fail("Error checking ways: " + e.getMessage());
            }

            try {
                Map<Long, CurrentRelations> relations = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentRelationsTbl)
                        .map(currentRelationsTbl.id, currentRelationsTbl);

                CurrentRelations relationRecord = relations.get(relationIdsArr[0]);
                Assert.assertEquals(new Long(changesetId), relationRecord.getChangesetId());
                Assert.assertEquals(relationIdsArr[0], relationRecord.getId());
                Assert.assertTrue(relationRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(2), relationRecord.getVersion());
                Assert.assertTrue(relationRecord.getVisible());

                List<CurrentRelationMembers> members = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentRelationMembersTbl)
                        .where(currentRelationMembersTbl.relationId.eq(relationIdsArr[0]))
                        .orderBy(currentRelationMembersTbl.sequenceId.asc()).list(currentRelationMembersTbl);

                Assert.assertEquals(3, members.size());

                CurrentRelationMembers member = members.get(0);

                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.way, member.getMemberType());
                Assert.assertEquals("role4", member.getMemberRole());
                Assert.assertEquals(new Integer(1), member.getSequenceId());
                Assert.assertEquals(wayIdsArr[1], member.getMemberId());

                member = members.get(1);

                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.way, member.getMemberType());
                Assert.assertEquals("role2", member.getMemberRole());
                Assert.assertEquals(new Integer(2), member.getSequenceId());
                Assert.assertEquals(wayIdsArr[0], member.getMemberId());

                member = members.get(2);

                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
                Assert.assertEquals("", member.getMemberRole());
                Assert.assertEquals(new Integer(3), member.getSequenceId());
                Assert.assertEquals(nodeIdsArr[2], member.getMemberId());

                // removed tag
                Assert.assertTrue((relationRecord.getTags() == null)
                        || StringUtils.isEmpty(((PGobject) relationRecord.getTags()).getValue()));

                relationRecord = relations.get(relationIdsArr[1]);

                Assert.assertEquals(new Long(changesetId), relationRecord.getChangesetId());
                Assert.assertEquals(relationIdsArr[1], relationRecord.getId());

                Assert.assertTrue(relationRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(2), relationRecord.getVersion());
                Assert.assertTrue(relationRecord.getVisible());

                members = new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentRelationMembersTbl)
                        .where(currentRelationMembersTbl.relationId.eq(relationIdsArr[1]))
                        .orderBy(currentRelationMembersTbl.sequenceId.asc()).list(currentRelationMembersTbl);

                Assert.assertEquals(2, members.size());

                member = members.get(0);

                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.relation, member.getMemberType());
                Assert.assertEquals("role1", member.getMemberRole());
                Assert.assertEquals(new Integer(1), member.getSequenceId());
                Assert.assertEquals(relationIdsArr[0], member.getMemberId());

                member = members.get(1);

                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
                Assert.assertEquals("", member.getMemberRole());
                Assert.assertEquals(new Integer(2), member.getSequenceId());
                Assert.assertEquals(nodeIdsArr[4], member.getMemberId());

                Map<String, String> tags = PostgresUtils.postgresObjToHStore((PGobject) relationRecord.getTags());

                Assert.assertNotNull(tags);
                Assert.assertEquals(2, tags.size());
                Assert.assertEquals("val 2", tags.get("key 2"));
                Assert.assertEquals("val 3", tags.get("key 3"));

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
            }
            catch (Exception e) {
                Assert.fail("Error checking relations: " + e.getMessage());
            }

            try {
                Assert.assertNotNull(changeset);
                Assert.assertTrue(changeset.getCreatedAt().before(now));
                Assert.assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));
                Assert.assertEquals(new Integer(18), changeset.getNumChanges());
                Assert.assertEquals(new Long(userId), changeset.getUserId());

                BoundingBox expandedBounds = new BoundingBox(originalBounds);
                expandedBounds.expand(updatedBounds,
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
            throw e;
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testUploadModifyClosedPolygon() throws Exception {
        try {
            BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
            long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
            Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
            Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
            Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
            Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);

            // Now, update some of the elements and their tags.
            // TODO: why was this disabled?
            // final BoundingBox updatedBounds =
            // OsmTestUtils.createAfterModifiedTestChangesetBounds();
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
                                "<modify>" +
                                    "<way id=\"" + wayIdsArr[0] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                        "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                        "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                                        "<nd ref=\"" + nodeIdsArr[2] + "\"></nd>" +
                                        "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                    "</way>" +
                                "</modify>" +
                                "<delete if-unused=\"true\"/>" +
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
                Assert.assertEquals(1, returnedWays.getLength());
                Assert.assertEquals((long) wayIdsArr[0],
                        Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@old_id", responseData)));
                Assert.assertEquals(Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@old_id", responseData)),
                        Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@new_id", responseData)));
                Assert.assertEquals(2,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@new_version", responseData)));
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
                Map<Long, CurrentWays> ways = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentWaysTbl)
                        .map(currentWaysTbl.id, currentWaysTbl);

                Assert.assertEquals(3, ways.size());

                CurrentWays wayRecord = ways.get(wayIdsArr[0]);

                Assert.assertEquals(new Long(changesetId), wayRecord.getChangesetId());
                Assert.assertEquals(wayIdsArr[0], wayRecord.getId());
                Assert.assertTrue(wayRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(2), wayRecord.getVersion());
                Assert.assertTrue(wayRecord.getVisible());

                List<CurrentWayNodes> wayNodes = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentWayNodesTbl).where(currentWayNodesTbl.wayId.eq(wayIdsArr[0]))
                        .orderBy(currentWayNodesTbl.sequenceId.asc())
                        .list(currentWayNodesTbl);

                Assert.assertEquals(4, wayNodes.size());

                CurrentWayNodes wayNode = wayNodes.get(0);

                Assert.assertEquals(nodeIdsArr[0], wayNode.getNodeId());
                Assert.assertEquals(new Long(1), wayNode.getSequenceId());
                Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());

                wayNode = wayNodes.get(1);

                Assert.assertEquals(nodeIdsArr[1], wayNode.getNodeId());
                Assert.assertEquals(new Long(2), wayNode.getSequenceId());
                Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());

                wayNode = wayNodes.get(2);

                Assert.assertEquals(nodeIdsArr[2], wayNode.getNodeId());
                Assert.assertEquals(new Long(3), wayNode.getSequenceId());
                Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());
                wayNode = wayNodes.get(3);

                Assert.assertEquals(nodeIdsArr[0], wayNode.getNodeId());
                Assert.assertEquals(new Long(4), wayNode.getSequenceId());
                Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());
            }
            catch (Exception e) {
                Assert.fail("Error checking ways: " + e.getMessage());
            }

            try {
                Assert.assertNotNull(changeset);
                Assert.assertTrue(changeset.getCreatedAt().before(now));
                Assert.assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));

                Assert.assertEquals(new Integer(13), changeset.getNumChanges());
                Assert.assertEquals(new Long(userId), changeset.getUserId());

                BoundingBox expandedBounds = new BoundingBox(originalBounds);
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
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadModifyDuplicateNodeIds() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Try to create two nodes with the same ID. A failure should occur and
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
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();

            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Duplicate OSM element ID"));

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
    public void testUploadModifyInvalidChangesetSpecifiedInUrl() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(null);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);

        // Upload a changeset where the changeset specified in the URL doesn't
        // exist. A failure should occur and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + (changesetId + 1) + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
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
                         "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();

            Assert.assertEquals(404, r.getStatus());
            Assert.assertTrue(r.getEntity(String.class).contains("Changeset to be updated does not exist"));

            OsmTestUtils.verifyTestChangesetUnmodified(changesetId);

            throw e;
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadModifyEmptyNodeCoords() throws Exception {
        // Try to update a changeset with nodes that have empty coordinate
        // strings. A failure should
        // occur and no data in system should be modified.
        uploadModifyNodeCoords("", "");
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadModifyNodeCoordsOutOfBounds() throws Exception {
        // Try to update a changeset with nodes that have out of world bounds
        // coordinate values.
        // A failure should occur and no data in system should be modified.
        uploadModifyNodeCoords("91", "-181");
    }

    private void uploadModifyNodeCoords(String lat, String lon) throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Now update the changeset via the service where one of the nodes has a
        // empty coords. A ailure should occur and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
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
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();

            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Error uploading changeset with ID"));

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
    public void testUploadModifyNoTags() throws Exception {
        try {
            BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
            long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
            Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
            Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
            Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
            Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
            Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
            Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

            // Now update the changeset with elements that have no tags in them,
            // which is allowed. The existing tags for the specified elements will be deleted, since
            // every request must specify all the tags it wants retained for each element. The elements not
            // referenced in this modify request should retain their tags.
            BoundingBox updatedBounds = OsmTestUtils.createAfterModifiedTestChangesetBounds();
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

                Assert.assertEquals((long) nodeIdsArr[0],
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@old_id", responseData)));
                Assert.assertEquals(Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@old_id", responseData)),
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@new_id", responseData)));
                Assert.assertEquals(2,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@new_version", responseData)));
                Assert.assertEquals((long) nodeIdsArr[1],
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@old_id", responseData)));
                Assert.assertEquals(Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@old_id", responseData)),
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@new_id", responseData)));
                Assert.assertEquals(2,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@new_version", responseData)));

                NodeList returnedWays = XPathAPI.selectNodeList(responseData, "//osm/diffResult/way");

                Assert.assertEquals(2, returnedWays.getLength());
                Assert.assertEquals((long) wayIdsArr[0],
                        Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@old_id", responseData)));
                Assert.assertEquals(Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@old_id", responseData)),
                        Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@new_id", responseData)));
                Assert.assertEquals(2,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@new_version", responseData)));

                Assert.assertEquals((long) wayIdsArr[1],
                        Long.parseLong(xpath.evaluate("//osm/diffResult/way[2]/@old_id", responseData)));
                Assert.assertEquals(Long.parseLong(xpath.evaluate("//osm/diffResult/way[2]/@old_id", responseData)),
                        Long.parseLong(xpath.evaluate("//osm/diffResult/way[2]/@new_id", responseData)));
                Assert.assertEquals(2,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@new_version", responseData)));

                NodeList returnedRelations = XPathAPI.selectNodeList(responseData, "//osm/diffResult/relation");
                Assert.assertEquals(2, returnedRelations.getLength());

                Assert.assertEquals((long) relationIdsArr[0],
                        Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@old_id", responseData)));
                Assert.assertEquals(
                        Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@old_id", responseData)),
                        Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@new_id", responseData)));
                Assert.assertEquals(2,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@new_version", responseData)));

                Assert.assertEquals((long) relationIdsArr[1],
                        Long.parseLong(xpath.evaluate("//osm/diffResult/relation[2]/@old_id", responseData)));
                Assert.assertEquals(
                        Long.parseLong(xpath.evaluate("//osm/diffResult/relation[2]/@old_id", responseData)),
                        Long.parseLong(xpath.evaluate("//osm/diffResult/relation[2]/@new_id", responseData)));
                Assert.assertEquals(2,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/relation[2]/@new_version", responseData)));
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
                Assert.assertEquals(5, nodes.size());

                CurrentNodes nodeRecord = nodes.get(nodeIdsArr[0]);
                Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
                Assert.assertEquals(new Double(updatedBounds.getMinLat()), nodeRecord.getLatitude());
                Assert.assertEquals(new Double(updatedBounds.getMinLon()), nodeRecord.getLongitude());
                Assert.assertEquals(nodeIdsArr[0], nodeRecord.getId());
                Assert.assertEquals(
                        new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                        nodeRecord.getTile());
                Assert.assertTrue(nodeRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(2), nodeRecord.getVersion());
                Assert.assertTrue(nodeRecord.getVisible());
                Assert.assertTrue((nodeRecord.getTags() == null)
                        || StringUtils.isEmpty(((PGobject) nodeRecord.getTags()).getValue()));

                nodeRecord = nodes.get(nodeIdsArr[1]);
                Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
                Assert.assertEquals(new Double(updatedBounds.getMinLat()), nodeRecord.getLatitude());
                Assert.assertEquals(new Double(originalBounds.getMaxLon()), nodeRecord.getLongitude());
                Assert.assertEquals(nodeIdsArr[1], nodeRecord.getId());
                Assert.assertEquals(
                        new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                        nodeRecord.getTile());
                Assert.assertTrue(nodeRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(2), nodeRecord.getVersion());
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
                Map<String, String> tags = PostgresUtils.postgresObjToHStore((PGobject) nodeRecord.getTags());
                Assert.assertNotNull(tags);
                Assert.assertEquals(1, tags.size());
                Assert.assertEquals("val 3", tags.get("key 3"));

                nodeRecord = nodes.get(nodeIdsArr[4]);
                tags = PostgresUtils.postgresObjToHStore((PGobject) nodeRecord.getTags());
                Assert.assertNotNull(tags);
                Assert.assertEquals(1, tags.size());
                Assert.assertEquals("val 4", tags.get("key 4"));
            }
            catch (Exception e) {
                Assert.fail("Error checking nodes: " + e.getMessage());
            }

            try {
                Map<Long, CurrentWays> ways = new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentWaysTbl)
                        .map(currentWaysTbl.id, currentWaysTbl);
                Assert.assertEquals(3, ways.size());

                CurrentWays wayRecord = ways.get(wayIdsArr[0]);

                Assert.assertEquals(new Long(changesetId), wayRecord.getChangesetId());
                Assert.assertEquals(wayIdsArr[0], wayRecord.getId());
                Assert.assertTrue(wayRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(2), wayRecord.getVersion());
                Assert.assertTrue(wayRecord.getVisible());

                List<CurrentWayNodes> wayNodes = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentWayNodesTbl).where(currentWayNodesTbl.wayId.eq(wayIdsArr[0]))
                        .orderBy(currentWayNodesTbl.sequenceId.asc()).list(currentWayNodesTbl);

                Assert.assertEquals(2, wayNodes.size());

                CurrentWayNodes wayNode = wayNodes.get(0);

                Assert.assertEquals(nodeIdsArr[0], wayNode.getNodeId());
                Assert.assertEquals(new Long(1), wayNode.getSequenceId());
                Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());

                wayNode = wayNodes.get(1);

                Assert.assertEquals(nodeIdsArr[1], wayNode.getNodeId());
                Assert.assertEquals(new Long(2), wayNode.getSequenceId());
                Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());
                Assert.assertTrue((wayRecord.getTags() == null)
                        || StringUtils.isEmpty(((PGobject) wayRecord.getTags()).getValue()));

                wayRecord = ways.get(wayIdsArr[1]);

                Assert.assertEquals(new Long(changesetId), wayRecord.getChangesetId());
                Assert.assertEquals(wayIdsArr[1], wayRecord.getId());
                Assert.assertTrue(wayRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(2), wayRecord.getVersion());
                Assert.assertTrue(wayRecord.getVisible());

                wayNodes = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentWayNodesTbl)
                        .where(currentWayNodesTbl.wayId.eq(wayIdsArr[1]))
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

                Map<String, String> tags = PostgresUtils.postgresObjToHStore((PGobject) wayRecord.getTags());

                Assert.assertNotNull(tags);
                Assert.assertEquals(1, tags.size());
                Assert.assertEquals("val 3", tags.get("key 3"));
            }
            catch (Exception e) {
                Assert.fail("Error checking ways: " + e.getMessage());
            }

            try {
                Map<Long, CurrentRelations> relations = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentRelationsTbl).map(currentRelationsTbl.id, currentRelationsTbl);

                Assert.assertEquals(4, relations.size());

                CurrentRelations relationRecord = relations.get(relationIdsArr[0]);
                Assert.assertEquals(new Long(changesetId), relationRecord.getChangesetId());
                Assert.assertEquals(relationIdsArr[0], relationRecord.getId());
                Assert.assertTrue(relationRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(2), relationRecord.getVersion());
                Assert.assertTrue(relationRecord.getVisible());

                List<CurrentRelationMembers> members = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentRelationMembersTbl)
                        .where(currentRelationMembersTbl.relationId.eq(relationIdsArr[0]))
                        .orderBy(currentRelationMembersTbl.sequenceId.asc()).list(currentRelationMembersTbl);

                Assert.assertEquals(3, members.size());

                CurrentRelationMembers member = members.get(0);

                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.way, member.getMemberType());
                Assert.assertEquals("role4", member.getMemberRole());
                Assert.assertEquals(new Integer(1), member.getSequenceId());
                Assert.assertEquals(wayIdsArr[1], member.getMemberId());

                member = members.get(1);

                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.way, member.getMemberType());
                Assert.assertEquals("role2", member.getMemberRole());
                Assert.assertEquals(new Integer(2), member.getSequenceId());
                Assert.assertEquals(wayIdsArr[0], member.getMemberId());

                member = members.get(2);

                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
                Assert.assertEquals("", member.getMemberRole());
                Assert.assertEquals(new Integer(3), member.getSequenceId());

                Assert.assertEquals(nodeIdsArr[2], member.getMemberId());
                Assert.assertTrue((relationRecord.getTags() == null)
                        || StringUtils.isEmpty(((PGobject) relationRecord.getTags()).getValue()));

                relationRecord = relations.get(relationIdsArr[1]);

                Assert.assertEquals(new Long(changesetId), relationRecord.getChangesetId());
                Assert.assertEquals(relationIdsArr[1], relationRecord.getId());

                Assert.assertTrue(relationRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(2), relationRecord.getVersion());
                Assert.assertTrue(relationRecord.getVisible());

                members = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentRelationMembersTbl)
                        .where(currentRelationMembersTbl.relationId.eq(relationIdsArr[1]))
                        .orderBy(currentRelationMembersTbl.sequenceId.asc())
                        .list(currentRelationMembersTbl);

                Assert.assertEquals(2, members.size());

                member = members.get(0);

                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.relation, member.getMemberType());
                Assert.assertEquals("role1", member.getMemberRole());
                Assert.assertEquals(new Integer(1), member.getSequenceId());
                Assert.assertEquals(relationIdsArr[0], member.getMemberId());

                member = members.get(1);

                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
                Assert.assertEquals("", member.getMemberRole());
                Assert.assertEquals(new Integer(2), member.getSequenceId());
                Assert.assertEquals(nodeIdsArr[4], member.getMemberId());
                Assert.assertTrue((relationRecord.getTags() == null)
                        || StringUtils.isEmpty(((PGobject) relationRecord.getTags()).getValue()));

                relationRecord = relations.get(relationIdsArr[2]);
                Assert.assertEquals(new Long(changesetId), relationRecord.getChangesetId());
                Assert.assertEquals(relationIdsArr[2], relationRecord.getId());

                Assert.assertTrue(relationRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), relationRecord.getVersion());
                Assert.assertEquals(true, relationRecord.getVisible());
                members = new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentRelationMembersTbl)
                        .where(currentRelationMembersTbl.relationId.eq(relationIdsArr[2]))
                        .orderBy(currentRelationMembersTbl.sequenceId.asc()).list(currentRelationMembersTbl);

                Assert.assertEquals(1, members.size());

                member = members.get(0);

                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.way, member.getMemberType());
                Assert.assertEquals("", member.getMemberRole());
                Assert.assertEquals(new Integer(1), member.getSequenceId());
                Assert.assertEquals(wayIdsArr[1], member.getMemberId());

                Map<String, String> tags = PostgresUtils.postgresObjToHStore((PGobject) relationRecord.getTags());

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
            }
            catch (Exception e) {
                Assert.fail("Error checking relations: " + e.getMessage());
            }

            try {
                changeset = new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(changesets)
                        .where(changesets.id.eq(changesetId)).singleResult(changesets);

                Assert.assertNotNull(changeset);
                Assert.assertTrue(changeset.getCreatedAt().before(now));
                Assert.assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));
                Assert.assertEquals(new Integer(18), changeset.getNumChanges());
                Assert.assertEquals(new Long(userId), changeset.getUserId());

                BoundingBox expandedBounds = new BoundingBox(originalBounds);
                expandedBounds.expand(updatedBounds,
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
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadModifyInvalidChangesetIdInNode() throws Exception {
        BoundingBox changeset1Bounds = OsmTestUtils.createStartingTestBounds();
        long changesetId1 = OsmTestUtils.createTestChangeset(changeset1Bounds);
        BoundingBox changeset2Bounds = OsmTestUtils.createStartingTestBounds();
        long changesetId2 = OsmTestUtils.createTestChangeset(changeset2Bounds);

        // link some nodes to the changesets
        Set<Long> nodeIds1 = OsmTestUtils.createTestNodes(changesetId1, changeset1Bounds);
        Long[] nodeIds1Arr = nodeIds1.toArray(new Long[nodeIds1.size()]);
        Set<Long> nodeIds2 = OsmTestUtils.createTestNodes(changesetId2, changeset2Bounds);
        Long[] nodeIds2Arr = nodeIds2.toArray(new Long[nodeIds2.size()]);

        // Upload a changeset where one of the elements in the changeset lists a
        // changeset ID that doesn't match the changeset ID specified in the URL. A failure should
        // occur and no data in the system should be modified.
        BoundingBox updatedBounds = OsmTestUtils.createAfterModifiedTestChangesetBounds();
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId1 + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
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
                        "<delete if-unused=\"true\"/>" + "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();

            Assert.assertEquals(Status.CONFLICT, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Invalid changeset ID"));

            OsmTestUtils.verifyTestNodesUnmodified(nodeIds1, changesetId1, changeset1Bounds);
            OsmTestUtils.verifyTestNodesUnmodified(nodeIds2, changesetId2, changeset2Bounds);

            try {
                QChangesets changesets = QChangesets.changesets;
                Changesets changeset = new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(changesets)
                        .where(changesets.id.eq(changesetId1)).singleResult(changesets);

                Assert.assertNotNull(changeset);
                final Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
                Assert.assertTrue(changeset.getCreatedAt().before(now));
                Assert.assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));
                Assert.assertEquals(new Integer(12), changeset.getNumChanges());
                Assert.assertEquals(new Long(userId), changeset.getUserId());

                // changeset = changesetDao.findById(changesetId2);
                changeset = new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(changesets)
                        .where(changesets.id.eq(changesetId2)).singleResult(changesets);

                Assert.assertNotNull(changeset);
                Assert.assertTrue(changeset.getCreatedAt().before(now));
                Assert.assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));
                Assert.assertEquals(new Integer(12), changeset.getNumChanges());
                Assert.assertEquals(new Long(userId), changeset.getUserId());

                // make sure the changeset bounds weren't updated
                Changeset hootChangeset = new Changeset(mapId, changesetId1, conn);
                BoundingBox changesetBounds = hootChangeset.getBounds();
                BoundingBox expandedBounds = new BoundingBox();
                double boundsExpansionFactor = Double
                        .parseDouble(HootProperties.getDefault("changesetBoundsExpansionFactorDeegrees"));
                expandedBounds.expand(changeset1Bounds, boundsExpansionFactor);
                Assert.assertEquals(expandedBounds, changesetBounds);

                hootChangeset = new Changeset(mapId, changesetId2, conn);
                changesetBounds = hootChangeset.getBounds();
                expandedBounds = new BoundingBox();
                expandedBounds.expand(changeset2Bounds, boundsExpansionFactor);
                Assert.assertEquals(expandedBounds, changesetBounds);
            }
            catch (Exception e2) {
                Assert.fail("Error checking changeset: " + e.getMessage());
            }

            throw e;
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadModifyNonExistingNode() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        BoundingBox updateBounds = OsmTestUtils.createAfterModifiedTestChangesetBounds();

        // Update the changeset with a node that has an ID that doesn't exist. A
        // failure should occur and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
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
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();

            Assert.assertEquals(Status.NOT_FOUND, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Element(s) being referenced don't exist."));

            // make sure the bad node wasn't created and the other node wasn't
            // updated
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
    public void testUploadModifyNegativeElementId() throws Exception {
        try {
            // We allow elements with a negative ID to exist, which allows for
            // using the hootProps.hoot --convert command as a source of test data for the services.

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

            CurrentNodes insertedWayRecord = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                    .from(currentNodesTbl)
                    .where(currentNodesTbl.id.eq(negativeWayId))
                    .singleResult(currentNodesTbl);

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
            tags.put("key 2", "val 2");
            relationIds.add(OsmTestUtils.insertNewRelation(changesetId, mapId, members, tags, conn));
            tags.clear();

            Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

            // Update the changeset via the service. This update is valid, because the property was changed above.
            Document responseData = null;
            try {
                responseData = resource().path("api/0.6/changeset/" + changesetId + "/upload")
                        .queryParam("mapId", String.valueOf(mapId))
                        .type(MediaType.TEXT_XML)
                        .accept(MediaType.TEXT_XML)
                        .post(Document.class,
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
                                        "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                                        "<nd ref=\"" + nodeIdsArr[2] + "\"></nd>" +
                                        "<tag k=\"key 1b\" v=\"val 1b\"></tag>" +
                                        "<tag k=\"key 2b\" v=\"val 2b\"></tag>" +
                                    "</way>" +
                                    "<way id=\"" + negativeWayId + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                        "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                        "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                                        "<tag k=\"key 1\" v=\"val 1\"></tag>" +
                                    "</way>" +
                                    "<relation id=\"" + relationIdsArr[1] + "\" version=\"1\" changeset=\"" +
                                           changesetId + "\" >" +
                                        "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                        "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[0] + "\"></member>" +
                                        "<tag k=\"key 2b\" v=\"val 2b\"></tag>" +
                                    "</relation>" +
                                    "<relation id=\"" + negativeRelationId + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                        "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                                        "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[1] + "\"></member>" +
                                    "</relation>" +
                                "</modify>" +
                                "<delete if-unused=\"true\"/>"
                            + "</osmChange>");
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
                Assert.assertEquals((long) nodeIdsArr[1],
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@old_id", responseData)));
                Assert.assertEquals(Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@old_id", responseData)),
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@new_id", responseData)));
                Assert.assertEquals(2,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@new_version", responseData)));

                Assert.assertEquals(negativeNodeId,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@old_id", responseData)));
                Assert.assertEquals(Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@old_id", responseData)),
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@new_id", responseData)));
                Assert.assertEquals(2,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@new_version", responseData)));

                NodeList returnedWays = XPathAPI.selectNodeList(responseData, "//osm/diffResult/way");

                Assert.assertEquals(2, returnedWays.getLength());

                Assert.assertEquals((long) wayIdsArr[1],
                        Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@old_id", responseData)));

                Assert.assertEquals(Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@old_id", responseData)),
                        Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@new_id", responseData)));

                Assert.assertEquals(2,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@new_version", responseData)));

                Assert.assertEquals(negativeWayId,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/way[2]/@old_id", responseData)));

                Assert.assertEquals(Long.parseLong(xpath.evaluate("//osm/diffResult/way[2]/@old_id", responseData)),
                        Long.parseLong(xpath.evaluate("//osm/diffResult/way[2]/@new_id", responseData)));

                Assert.assertEquals(2,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/way[2]/@new_version", responseData)));

                NodeList returnedRelations = XPathAPI.selectNodeList(responseData, "//osm/diffResult/relation");

                Assert.assertEquals(2, returnedRelations.getLength());
                Assert.assertEquals((long) relationIdsArr[1],
                        Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@old_id", responseData)));
                Assert.assertEquals(
                        Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@old_id", responseData)),
                        Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@new_id", responseData)));
                Assert.assertEquals(2,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@new_version", responseData)));

                Assert.assertEquals(negativeRelationId,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/relation[2]/@old_id", responseData)));
                Assert.assertEquals(
                        Long.parseLong(xpath.evaluate("//osm/diffResult/relation[2]/@old_id", responseData)),
                        Long.parseLong(xpath.evaluate("//osm/diffResult/relation[2]/@new_id", responseData)));
                Assert.assertEquals(2,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/relation[2]/@new_version", responseData)));
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

                Assert.assertEquals(2, nodes.size());
                Assert.assertEquals(negativeNodeId, (long) nodeIdsArr[0]);

                CurrentNodes nodeRecord = nodes.get(nodeIdsArr[0]);

                Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
                Assert.assertEquals(new Double(updateBounds.getMinLat()), nodeRecord.getLatitude());
                Assert.assertEquals(new Double(originalBounds.getMaxLon()), nodeRecord.getLongitude());
                Assert.assertEquals(new Long(negativeNodeId), nodeRecord.getId());

                Assert.assertEquals(
                        new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                        nodeRecord.getTile());

                Assert.assertTrue(nodeRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(2), nodeRecord.getVersion());
                Assert.assertTrue(nodeRecord.getVisible());

                Map<String, String> parsedTags = PostgresUtils.postgresObjToHStore((PGobject) nodeRecord.getTags());

                Assert.assertNotNull(parsedTags);
                Assert.assertEquals(1, parsedTags.size());
                Assert.assertEquals("val 3b", parsedTags.get("key 3b"));

                nodeRecord = nodes.get(nodeIdsArr[1]);

                Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
                Assert.assertEquals(new Double(updateBounds.getMinLat()), nodeRecord.getLatitude());
                Assert.assertEquals(new Double(updateBounds.getMinLon()), nodeRecord.getLongitude());
                Assert.assertEquals(nodeIdsArr[1], nodeRecord.getId());
                Assert.assertEquals(
                        new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                        nodeRecord.getTile());
                Assert.assertTrue(nodeRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(2), nodeRecord.getVersion());
                Assert.assertTrue(nodeRecord.getVisible());
                parsedTags = PostgresUtils.postgresObjToHStore((PGobject) nodeRecord.getTags());
                Assert.assertNotNull(parsedTags);
                Assert.assertEquals(2, parsedTags.size());
                Assert.assertEquals("val 1b", parsedTags.get("key 1b"));
                Assert.assertEquals("val 2b", parsedTags.get("key 2b"));
            }
            catch (Exception e) {
                Assert.fail("Error checking nodes: " + e.getMessage());
            }

            try {
                Map<Long, CurrentWays> ways = new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentWaysTbl)
                        .map(currentWaysTbl.id, currentWaysTbl);
                Assert.assertEquals(2, ways.size());

                // verify the updated ways
                Assert.assertEquals(negativeWayId, (long) wayIdsArr[0]);

                CurrentWays wayRecord = ways.get(wayIdsArr[0]);

                Assert.assertEquals(new Long(changesetId), wayRecord.getChangesetId());
                Assert.assertEquals(new Long(negativeWayId), wayRecord.getId());
                Assert.assertTrue(wayRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(2), wayRecord.getVersion());
                Assert.assertTrue(wayRecord.getVisible());

                List<CurrentWayNodes> wayNodes =

                        new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentWayNodesTbl)
                                .where(currentWayNodesTbl.wayId.eq(wayIdsArr[1]))
                                .orderBy(currentWayNodesTbl.sequenceId.asc()).list(currentWayNodesTbl);

                Assert.assertEquals(2, wayNodes.size());

                CurrentWayNodes wayNode = wayNodes.get(0);

                Assert.assertEquals(nodeIdsArr[0], wayNode.getNodeId());
                Assert.assertEquals(new Long(1), wayNode.getSequenceId());
                Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());

                wayNode = wayNodes.get(1);

                Assert.assertEquals(nodeIdsArr[1], wayNode.getNodeId());
                Assert.assertEquals(new Long(2), wayNode.getSequenceId());
                Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());

                Map<String, String> parsedTags = PostgresUtils.postgresObjToHStore((PGobject) wayRecord.getTags());

                Assert.assertNotNull(parsedTags);
                Assert.assertEquals(1, parsedTags.size());
                Assert.assertEquals("val 1", parsedTags.get("key 1"));

                wayRecord = ways.get(wayIdsArr[1]);

                Assert.assertEquals(new Long(changesetId), wayRecord.getChangesetId());
                Assert.assertEquals(wayIdsArr[1], wayRecord.getId());
                Assert.assertTrue(wayRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(2), wayRecord.getVersion());
                Assert.assertTrue(wayRecord.getVisible());

                wayNodes = new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentWayNodesTbl)
                        .where(currentWayNodesTbl.wayId.eq(wayIdsArr[1])).orderBy(currentWayNodesTbl.sequenceId.asc())
                        .list(currentWayNodesTbl);

                Assert.assertEquals(2, wayNodes.size());

                wayNode = wayNodes.get(0);

                Assert.assertEquals(nodeIdsArr[1], wayNode.getNodeId());
                Assert.assertEquals(new Long(1), wayNode.getSequenceId());
                Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());

                wayNode = wayNodes.get(1);

                Assert.assertEquals(nodeIdsArr[2], wayNode.getNodeId());
                Assert.assertEquals(new Long(2), wayNode.getSequenceId());
                Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());

                parsedTags = PostgresUtils.postgresObjToHStore((PGobject) wayRecord.getTags());

                Assert.assertNotNull(parsedTags);
                Assert.assertEquals(2, parsedTags.size());
                Assert.assertEquals("val 1b", parsedTags.get("key 1b"));
                Assert.assertEquals("val 2b", parsedTags.get("key 2b"));
            }
            catch (Exception e) {
                Assert.fail("Error checking ways: " + e.getMessage());
            }

            try {
                Map<Long, CurrentRelations> relations = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentRelationsTbl).map(currentRelationsTbl.id, currentRelationsTbl);

                Assert.assertEquals(2, relations.size());
                Assert.assertEquals(negativeRelationId, (long) relationIdsArr[0]);

                CurrentRelations relationRecord = relations.get(relationIdsArr[0]);

                Assert.assertEquals(new Long(changesetId), relationRecord.getChangesetId());
                Assert.assertEquals(relationIdsArr[0], relationRecord.getId());
                Assert.assertTrue(relationRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(2), relationRecord.getVersion());
                Assert.assertTrue(relationRecord.getVisible());

                List<CurrentRelationMembers> memberRecords = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentRelationMembersTbl)
                        .where(currentRelationMembersTbl.relationId.eq(relationIdsArr[0]))
                        .orderBy(currentRelationMembersTbl.sequenceId.asc()).list(currentRelationMembersTbl);

                Assert.assertEquals(2, members.size());

                CurrentRelationMembers member = memberRecords.get(0);

                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
                Assert.assertEquals("role1", member.getMemberRole());
                Assert.assertEquals(new Integer(1), member.getSequenceId());
                Assert.assertEquals(nodeIdsArr[1], member.getMemberId());

                member = memberRecords.get(1);

                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.way, member.getMemberType());
                Assert.assertEquals("role1", member.getMemberRole());
                Assert.assertEquals(new Integer(2), member.getSequenceId());
                Assert.assertEquals(wayIdsArr[1], member.getMemberId());

                Map<String, String> parsedTags = PostgresUtils.postgresObjToHStore((PGobject) relationRecord.getTags());

                Assert.assertNotNull(parsedTags);
                Assert.assertEquals(1, parsedTags.size());
                Assert.assertEquals("val 1", parsedTags.get("key 1"));

                relationRecord = relations.get(relationIdsArr[1]);

                Assert.assertEquals(new Long(changesetId), relationRecord.getChangesetId());
                Assert.assertEquals(relationIdsArr[1], relationRecord.getId());
                Assert.assertTrue(relationRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(2), relationRecord.getVersion());
                Assert.assertTrue(relationRecord.getVisible());

                memberRecords = new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentRelationMembersTbl)
                        .where(currentRelationMembersTbl.relationId.eq(relationIdsArr[1]))
                        .orderBy(currentRelationMembersTbl.sequenceId.asc()).list(currentRelationMembersTbl);

                Assert.assertEquals(2, members.size());

                member = memberRecords.get(0);

                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
                Assert.assertEquals("role1", member.getMemberRole());
                Assert.assertEquals(new Integer(1), member.getSequenceId());
                Assert.assertEquals(nodeIdsArr[0], member.getMemberId());

                member = memberRecords.get(1);

                Assert.assertEquals(relationRecord.getId(), member.getRelationId());
                Assert.assertEquals(DbUtils.nwr_enum.way, member.getMemberType());
                Assert.assertEquals("role1", member.getMemberRole());
                Assert.assertEquals(new Integer(2), member.getSequenceId());
                Assert.assertEquals(wayIdsArr[0], member.getMemberId());

                parsedTags = PostgresUtils.postgresObjToHStore((PGobject) relationRecord.getTags());

                Assert.assertNotNull(parsedTags);
                Assert.assertEquals(1, parsedTags.size());
                Assert.assertEquals("val 2b", parsedTags.get("key 2b"));
            }
            catch (Exception e) {
                Assert.fail("Error checking relations: " + e.getMessage());
            }

            try {
                changeset = new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(changesets)
                        .where(changesets.id.eq(changesetId)).singleResult(changesets);

                Assert.assertNotNull(changeset);
                Assert.assertTrue(changeset.getCreatedAt().before(now));
                Assert.assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));
                Assert.assertEquals(new Integer(6), changeset.getNumChanges());
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

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadModifyNodeInvalidVersion() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        BoundingBox updateBounds = OsmTestUtils.createAfterModifiedTestChangesetBounds();

        // Update the changeset where one of the nodes has version that doesn't
        // match the version on the server. A failure should occur and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML).post(Document.class,
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
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();

            Assert.assertEquals(Status.CONFLICT, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Invalid version"));

            // make sure neither node was updated
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
    public void testUploadModifyNoMembersInRelation() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // relations with no members are allowed
        Document responseData = resource()
                .path("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .type(MediaType.TEXT_XML)
                .accept(MediaType.TEXT_XML)
                .post(Document.class,
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create/>" +
                        "<modify>" +
                           "<relation id=\"" + relationIdsArr[0] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" + "</relation>" +
                        "</modify>" +
                        "<delete if-unused=\"true\"/>" +
                    "</osmChange>");

        Assert.assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm/diffResult/relation").getLength());
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadModifyWayInvalidVersion() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Update the changeset where one of the nodes has a version greater
        // than zero, which is what the version should be for all new elements. A failure should occur
        // and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
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
    public void testUploadModifyRelationInvalidVersion() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // Update the changeset where one of the relations has an invalid
        // version. A failure should occur and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
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
    public void testUploadModifyChangesetClosed() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // close the changeset
        QChangesets changesets = QChangesets.changesets;
        Changesets changeset = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                .from(changesets)
                .where(changesets.id.eq(changesetId))
                .singleResult(changesets);

        Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
        changeset.setClosedAt(now);

        new SQLUpdateClause(conn, DbUtils.getConfiguration(mapId), changesets)
                .where(changesets.id.eq(changeset.getId()))
                .set(changesets.closedAt, now)
                .execute();

        changeset = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                .from(changesets)
                .where(changesets.id.eq(changesetId))
                .singleResult(changesets);
        Assert.assertTrue(changeset.getClosedAt().equals(now));

        BoundingBox updateBounds = OsmTestUtils.createAfterModifiedTestChangesetBounds();

        // Try to update the closed changeset. A failure should occur and no
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
                            "<modify>" +
                                "<node id=\"" + nodeIdsArr[0] + "\" lon=\"" + updateBounds.getMinLon() + "\" " +
                                     "lat=\"" + updateBounds.getMinLat() + "\" version=\"1\" changeset=\"" + changesetId + "\">" +
                                     "<tag k=\"key 1\" v=\"val 1\"></tag>" +
                                     "<tag k=\"key 2\" v=\"val 2\"></tag>" +
                                "</node>" +
                            "</modify>" +
                            "<delete if-unused=\"true\"/>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();

            Assert.assertEquals(Status.CONFLICT, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(
                    r.getEntity(String.class).contains("The changeset with ID: " + changesetId + " was closed at"));

            // make sure nothing was updated
            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            OsmTestUtils.verifyTestChangesetClosed(changesetId);

            throw e;
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadModifyMissingNodeTagValue() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Upload a changeset where one of the tags specified has no value
        // attribute. A failure should occur and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML).accept(MediaType.TEXT_XML)
                    .post(Document.class,
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
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();

            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Error parsing tag"));

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
    public void testUploadModifyTooFewNodesForWay() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // A way has to have two or more nodes. Try to upload a way with one
        // node. The request should fail and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
                        "<osmChange version=\"0.3\" generator=\"iD\">" +
                            "<create/>" +
                            "<modify>" +
                                "<way id=\"" + wayIdsArr[0] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                    "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                    "<tag k=\"key 1\" v=\"val 1\"/>" +
                                "</way>" +
                            "</modify>" +
                            "<delete if-unused=\"true\"/>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();

            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Too few nodes specified for way"));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadModifyTooManyNodesForWay() throws Exception {
        String originalMaximumWayNodes = HootProperties.getProperty("maximumWayNodes");

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
            originalBounds = OsmTestUtils.createStartingTestBounds();
            changesetId = OsmTestUtils.createTestChangeset(originalBounds);
            nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
            Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
            wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
            Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
            relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

            // remember the original value to restore it later
            originalMaximumWayNodes = HootProperties.getProperty("maximumWayNodes");

            // use a lower number of max way nodes then default for efficiency
            HootCustomPropertiesSetter.setProperty("maximumWayNodes", "2");

            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
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
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();

            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Too many nodes specified for way"));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
        finally {
            HootCustomPropertiesSetter.setProperty("maximumWayNodes", originalMaximumWayNodes);
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadModifyWayWithNonExistentNode() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Try to create a way referencing a node that doesn't exist. The
        // request should fail and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
                        "<osmChange version=\"0.3\" generator=\"iD\">" +
                            "<create/>" +
                            "<modify>" +
                                "<way id=\"" + wayIdsArr[0] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                    "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                    "<nd ref=\"" + (nodeIdsArr[4] + 2) + "\"></nd>" +
                                "</way>" +
                            "</modify>" +
                            "<delete if-unused=\"true\"/>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();

            Assert.assertEquals(Status.NOT_FOUND, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Element(s) being referenced don't exist."));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadModifyRelationWithNonExistentNode() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // Try to update a relation referencing a node that doesn't exist. The
        // request should fail and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
                        "<osmChange version=\"0.3\" generator=\"iD\">" +
                            "<create/>" +
                            "<modify>" +
                                "<relation id=\"" + relationIdsArr[0] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                    "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                    "<member type=\"node\" role=\"role1\" ref=\"" + (nodeIdsArr[4] + 2) + "\"></member>" +
                                "</relation>" +
                            "</modify>" +
                            "<delete if-unused=\"true\"/>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();

            Assert.assertEquals(Status.NOT_FOUND, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Element(s) being referenced don't exist."));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadModifyRelationWithNonExistentWay() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // Try to update a relation referencing a way that doesn't exist. The
        // request should fail and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
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
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();

            Assert.assertEquals(Status.NOT_FOUND, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Element(s) being referenced don't exist."));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadModifyRelationWithNonExistentRelation() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // Try to create a relation referencing a relation that doesn't exist.
        // The request should fail and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
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
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();

            Assert.assertEquals(Status.NOT_FOUND, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Element(s) being referenced don't exist."));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadModifyWayWithInvisibleNode() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // make one of way nodes invisible
        CurrentNodes invisibleNode = new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentNodesTbl)
                .where(currentNodesTbl.id.eq(nodeIdsArr[0])).singleResult(currentNodesTbl);

        Assert.assertNotNull(invisibleNode);

        invisibleNode.setVisible(false);

        int success = (int) new SQLUpdateClause(conn, DbUtils.getConfiguration(mapId), currentNodesTbl)
                .where(currentNodesTbl.id.eq(invisibleNode.getId()))
                .set(currentNodesTbl.visible, false)
                .execute();

        Assert.assertEquals(1, success);
        Assert.assertEquals(false, new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                .from(currentNodesTbl)
                .where(currentNodesTbl.id.eq(nodeIdsArr[0]))
                .singleResult(currentNodesTbl.visible));

        // Try to upload a way which references an invisible node. The request
        // should fail and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
                        "<osmChange version=\"0.3\" generator=\"iD\">" +
                            "<create/>" +
                            "<modify>" +
                                "<way id=\"" + wayIdsArr[0] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                    "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                    "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                                "</way>" +
                            "</modify>" +
                            "<delete if-unused=\"true\"/>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("aren't visible for way"));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadModifyRelationWithInvisibleNode() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // make one of relation node members invisible
        CurrentNodes invisibleNode = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                .from(currentNodesTbl)
                .where(currentNodesTbl.id.eq(nodeIdsArr[0]))
                .singleResult(currentNodesTbl);

        Assert.assertNotNull(invisibleNode);

        invisibleNode.setVisible(false);

        int success = (int) new SQLUpdateClause(conn, DbUtils.getConfiguration(mapId), currentNodesTbl)
                .where(currentNodesTbl.id.eq(invisibleNode.getId()))
                .set(currentNodesTbl.visible, false)
                .execute();

        Assert.assertEquals(1, success);
        Assert.assertEquals(false, new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                .from(currentNodesTbl)
                .where(currentNodesTbl.id.eq(nodeIdsArr[0]))
                .singleResult(currentNodesTbl.visible));

        // Try to upload a relation which references an invisible node. The
        // request should fail and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
                        "<osmChange version=\"0.3\" generator=\"iD\">" +
                            "<create/>" +
                            "<modify>" +
                                "<relation id=\"" + relationIdsArr[0] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                    "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                    "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                                "</relation>" +
                            "</modify>" +
                            "<delete if-unused=\"true\"/>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();

            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("visible for relation"));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadModifyRelationWithInvisibleWay() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // make one of relation way members invisible
        CurrentWays invisibleWay = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                .from(currentWaysTbl)
                .where(currentWaysTbl.id.eq(wayIdsArr[0]))
                .singleResult(currentWaysTbl);

        Assert.assertNotNull(invisibleWay);

        invisibleWay.setVisible(false);

        int success = (int) new SQLUpdateClause(conn, DbUtils.getConfiguration(mapId), currentWaysTbl)
                .where(currentWaysTbl.id.eq(invisibleWay.getId()))
                .set(currentWaysTbl.visible, false)
                .execute();

        Assert.assertEquals(1, success);
        Assert.assertEquals(false, new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                .from(currentWaysTbl)
                .where(currentWaysTbl.id.eq(wayIdsArr[0]))
                .singleResult(currentWaysTbl.visible));

        // Try to upload a relation which references an invisible way. The
        // request should fail and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
                        "<osmChange version=\"0.3\" generator=\"iD\">" +
                            "<create/>" +
                            "<modify>" +
                                "<relation id=\"" + relationIdsArr[0] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                    "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[0] + "\"></member>" +
                                    "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                                "</relation>" +
                            "</modify>" +
                            "<delete if-unused=\"true\"/>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();

            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("visible for relation"));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadModifyRelationWithInvisibleRelation() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // make one of relation's members invisible
        CurrentRelations invisibleRelation = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                .from(currentRelationsTbl)
                .where(currentRelationsTbl.id.eq(relationIdsArr[0]))
                .singleResult(currentRelationsTbl);

        Assert.assertNotNull(invisibleRelation);
        invisibleRelation.setVisible(false);

        int success = (int) new SQLUpdateClause(conn, DbUtils.getConfiguration(mapId), currentRelationsTbl)
                .where(currentRelationsTbl.id.eq(invisibleRelation.getId()))
                .set(currentRelationsTbl.visible, false)
                .execute();

        Assert.assertEquals(1, success);
        Assert.assertEquals(false, new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                .from(currentRelationsTbl)
                .where(currentRelationsTbl.id.eq(relationIdsArr[0]))
                .singleResult(currentRelationsTbl.visible));

        // Try to upload a relation which references an invisible relation. The
        // request should fail and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
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
                            "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();

            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("visible for relation"));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadModifyDuplicateWayIds() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Try to upload two ways with the same ID. The request should fail and
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
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();

            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Duplicate OSM element ID"));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadModifyInvalidChangesetIdInWay() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Upload a changeset where one of the elements in the changeset lists a
        // changeset ID that doesn't match the changeset ID specified in the URL. A failure should
        // occur and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
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
                            "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();

            Assert.assertEquals(Status.CONFLICT, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Invalid changeset ID"));

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
    public void testUploadModifyInvalidChangesetIdInRelation() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // Upload a changeset where one of the elements in the changeset lists a
        // changeset ID that doesn't match the changeset ID specified in the URL. A failure should
        // occur and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
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
                        "</osmChange>");
    }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();

            Assert.assertEquals(Status.CONFLICT, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Invalid changeset ID"));

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
    public void testUploadModifyMissingWayTagValue() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Upload a changeset where one of the tags specified has no value
        // attribute. A failure should occur and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
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
                            "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();

            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Error parsing tag"));

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
    public void testUploadModifyMissingRelationTagValue() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // Upload a changeset where one of the element tags specified has no
        // value attribute. A failure should occur and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
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
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();

            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Error parsing tag"));

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
    public void testUploadModifyDuplicateWayNodeIds() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);

        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Try to upload a way which lists the same node twice. The request
        // should fail and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
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
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();

            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Duplicate OSM element ID"));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadModifyWayEmptyNodeId() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Update the changeset where one of the ways has a node with an empty
        // string for its ID. A failure should occur and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
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
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
            // TODO: needed?
            // Assert.assertTrue(r.getEntity(String.class).contains("Invalid
            // version"));

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
    public void testUploadModifyWayMissingNodeId() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);

        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Update the changeset where one of the ways has a node with no ID
        // attribute. A failure should occur and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
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
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
            // TODO: needed?
            // Assert.assertTrue(r.getEntity(String.class).contains("Invalid
            // version"));

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
    public void testUploadModifyDuplicateRelationIds() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // Try to upload two relations with the same ID. The request should fail
        // and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
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
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();

            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Duplicate OSM element ID"));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadModifyRelationReferencingItself() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // Update the changeset with a relation that references itself as a
        // member. A failure should occur and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
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
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();

            Assert.assertEquals(Status.CONFLICT, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("contains a relation member that references itself"));

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
    public void testUploadModifyRelationEmptyMemberId() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // Update the changeset where one of the relations has a member with an
        // empty string for an ID. A failure should occur and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
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
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();

            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Element in changeset has empty ID."));

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
    public void testUploadModifyRelationMissingMemberId() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // Update the changeset where one of the relations has a member with an
        // empty string for an ID.  A failure should occur and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
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
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Element in changeset has empty ID."));

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
    public void testUploadModifyRelationInvalidMemberType() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // Update the changeset where one of the relations has a member with an
        // empty string for one of its member's type. A failure should occur and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
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
    public void testUploadModifyRelationMissingMemberType() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // Update the changeset where one of the relations has a member with no
        // type. A failure should occur and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
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
}
