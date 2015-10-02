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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.osm;

import java.io.IOException;
import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.HashMap;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Map;
import java.util.Properties;
import java.util.Set;

import javax.ws.rs.core.MediaType;
import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathExpressionException;

import org.junit.Assert;
import org.apache.commons.lang3.StringUtils;
import org.apache.xpath.XPathAPI;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.postgresql.util.PGobject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.w3c.dom.Document;
import org.w3c.dom.NodeList;

import hoot.services.HootProperties;
import hoot.services.UnitTest;
import hoot.services.db.DbUtils;
import hoot.services.db.postgres.PostgresUtils;
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
import hoot.services.models.osm.Element.ElementType;
import hoot.services.models.osm.Node;
import hoot.services.models.osm.Relation;
import hoot.services.models.osm.RelationMember;
import hoot.services.models.osm.Way;
import hoot.services.osm.OsmResourceTestAbstract;
import hoot.services.osm.OsmTestUtils;
import hoot.services.utils.XmlUtils;

import com.mysema.query.sql.SQLQuery;
import com.mysema.query.sql.dml.SQLUpdateClause;
import com.sun.jersey.api.client.ClientResponse;
import com.sun.jersey.api.client.UniformInterfaceException;
import com.sun.jersey.api.client.ClientResponse.Status;

/*
 * @todo Most of these tests could be converted to integration tests and after a refactoring,
 * could be replace with unit tests that test only the internal classes being used by this
 * Jersey resource.
 */
public class ChangesetResourceUploadModifyTest extends OsmResourceTestAbstract
{
  private static final Logger log = LoggerFactory.getLogger(ChangesetResourceUploadModifyTest.class);
  private QCurrentNodes currentNodesTbl = QCurrentNodes.currentNodes;
  private QCurrentWays currentWaysTbl = QCurrentWays.currentWays;
  private QCurrentWayNodes currentWayNodesTbl = QCurrentWayNodes.currentWayNodes;
  private QCurrentRelations  currentRelationsTbl = QCurrentRelations.currentRelations;
  private QCurrentRelationMembers currentRelationMembersTbl = QCurrentRelationMembers.currentRelationMembers;

  public ChangesetResourceUploadModifyTest() throws NumberFormatException, IOException
  {
    super("hoot.services.controllers.osm");
  }

  @Test
  @Category(UnitTest.class)
  public void testUploadModify() throws Exception
  {

    try
    {
      final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
      final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
      final Set<Long> nodeIds =
        OsmTestUtils.createTestNodes(changesetId, originalBounds);
      final Long[] nodeIdsArr = nodeIds.toArray(new Long[]{});

      final Set<Long> wayIds =
        OsmTestUtils.createTestWays(changesetId, nodeIds);
      final Long[] wayIdsArr = wayIds.toArray(new Long[]{});

      final Set<Long> relationIds =
        OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
      final Long[] relationIdsArr = relationIds.toArray(new Long[]{});

      //Now, update some of the elements and their tags.
      final BoundingBox updatedBounds =
        OsmTestUtils.createAfterModifiedTestChangesetBounds();
      Document responseData = null;
      try
      {
        responseData =
          resource()
            .path("api/0.6/changeset/" + changesetId + "/upload")
            .queryParam("mapId", "" + mapId)
            .type(MediaType.TEXT_XML)
            .accept(MediaType.TEXT_XML)
            .post(
              Document.class,
              "<osmChange version=\"0.3\" generator=\"iD\">" +
                "<create/>" +
                "<modify>" +
                  "<node id=\"" + nodeIdsArr[0] + "\" lon=\"" + updatedBounds.getMinLon() + "\" " +
                    "lat=\"" + updatedBounds.getMinLat() + "\" version=\"1\" changeset=\"" +
                    changesetId + "\">" +
                    "<tag k=\"key 1b\" v=\"val 1b\"></tag>" +
                    "<tag k=\"key 2b\" v=\"val 2b\"></tag>" +
                  "</node>" +
                  "<node id=\"" + nodeIdsArr[1] + "\" lon=\"" + originalBounds.getMaxLon() + "\" " +
                    "lat=\"" + updatedBounds.getMinLat() + "\" version=\"1\" changeset=\"" +
                    changesetId + "\">" +
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
              "</osmChange>");
      }
      catch (UniformInterfaceException e)
      {
        ClientResponse r = e.getResponse();
        Assert.fail("Unexpected response " + r.getStatus() + " " + r.getEntity(String.class));
      }
      Assert.assertNotNull(responseData);

      XPath xpath = XmlUtils.createXPath();
      try
      {
        NodeList returnedNodes = XPathAPI.selectNodeList(responseData, "//osm/diffResult/node");
        Assert.assertEquals(2, returnedNodes.getLength());

        Assert.assertEquals(
          (long)nodeIdsArr[0],
          Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@old_id",
          responseData)));
        Assert.assertEquals(
          Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@old_id", responseData)),
          Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@new_id", responseData)));
        Assert.assertEquals(
          2, Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@new_version", responseData)));

        Assert.assertEquals(
          (long)nodeIdsArr[1],
          Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@old_id",
          responseData)));
        Assert.assertEquals(
          Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@old_id", responseData)),
          Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@new_id", responseData)));
        Assert.assertEquals(
          2, Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@new_version", responseData)));

        NodeList returnedWays = XPathAPI.selectNodeList(responseData, "//osm/diffResult/way");
        Assert.assertEquals(2, returnedWays.getLength());

        Assert.assertEquals(
          (long)wayIdsArr[0],
          Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@old_id",
          responseData)));
        Assert.assertEquals(
          Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@old_id", responseData)),
          Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@new_id", responseData)));
        Assert.assertEquals(
          2, Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@new_version", responseData)));

        Assert.assertEquals(
          (long)wayIdsArr[1],
          Long.parseLong(xpath.evaluate("//osm/diffResult/way[2]/@old_id",
          responseData)));
        Assert.assertEquals(
          Long.parseLong(xpath.evaluate("//osm/diffResult/way[2]/@old_id", responseData)),
          Long.parseLong(xpath.evaluate("//osm/diffResult/way[2]/@new_id", responseData)));
        Assert.assertEquals(
          2, Long.parseLong(xpath.evaluate("//osm/diffResult/way[2]/@new_version", responseData)));

        NodeList returnedRelations =
          XPathAPI.selectNodeList(responseData, "//osm/diffResult/relation");
        Assert.assertEquals(2, returnedRelations.getLength());

        //check the modified relations
        Assert.assertEquals(
          (long)relationIdsArr[0],
          Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@old_id",
          responseData)));
        Assert.assertEquals(
          Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@old_id", responseData)),
          Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@new_id", responseData)));
        Assert.assertEquals(
          2, Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@new_version", responseData)));

        Assert.assertEquals(
          (long)relationIdsArr[1],
          Long.parseLong(xpath.evaluate("//osm/diffResult/relation[2]/@old_id",
          responseData)));
        Assert.assertEquals(
          Long.parseLong(xpath.evaluate("//osm/diffResult/relation[2]/@old_id", responseData)),
          Long.parseLong(xpath.evaluate("//osm/diffResult/relation[2]/@new_id", responseData)));
        Assert.assertEquals(
          2, Long.parseLong(xpath.evaluate("//osm/diffResult/relation[2]/@new_version", responseData)));
      }
      catch (XPathExpressionException e)
      {
        Assert.fail("Error parsing response document: " + e.getMessage());
      }

      final Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
      QChangesets changesets = QChangesets.changesets;
      hoot.services.db2.Changesets changeset =
      new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(changesets)
      .where(changesets.id.eq(changesetId)).singleResult(changesets);

      try
      {
        final Map<Long, CurrentNodes> nodes =
        		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentNodesTbl)
        		.map(currentNodesTbl.id, currentNodesTbl);
        		;
        Assert.assertEquals(5, nodes.size());

        CurrentNodes nodeRecord = nodes.get(nodeIdsArr[0]);
        Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
        Assert.assertEquals(
          new Double(updatedBounds.getMinLat()),
          nodeRecord.getLatitude());
        Assert.assertEquals(
          new Double(updatedBounds.getMinLon()),
          nodeRecord.getLongitude());
        Assert.assertEquals(nodeIdsArr[0], nodeRecord.getId());
        Assert.assertEquals(
          new Long(QuadTileCalculator.tileForPoint(
            nodeRecord.getLatitude(),
            nodeRecord.getLongitude())),
          nodeRecord.getTile());
        Assert.assertTrue(nodeRecord.getTimestamp().before(now));
        Assert.assertEquals(new Long(2), nodeRecord.getVersion());
        Assert.assertEquals(new Boolean(true), nodeRecord.getVisible());;
        Map<String, String> tags = PostgresUtils.postgresObjToHStore((PGobject)nodeRecord.getTags());
        Assert.assertNotNull(tags);
        Assert.assertEquals(2, tags.size());
        Assert.assertEquals("val 1b", tags.get("key 1b"));
        Assert.assertEquals("val 2b", tags.get("key 2b"));

        nodeRecord = nodes.get(nodeIdsArr[1]);
        Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
        Assert.assertEquals(
          new Double(updatedBounds.getMinLat()),
          nodeRecord.getLatitude());
        Assert.assertEquals(
          new Double(originalBounds.getMaxLon()),
          nodeRecord.getLongitude());
        Assert.assertEquals(nodeIdsArr[1], nodeRecord.getId());
        Assert.assertEquals(
          new Long(QuadTileCalculator.tileForPoint(
            nodeRecord.getLatitude(),
            nodeRecord.getLongitude())),
          nodeRecord.getTile());
        Assert.assertTrue(nodeRecord.getTimestamp().before(now));
        Assert.assertEquals(new Long(2), nodeRecord.getVersion());
        Assert.assertEquals(new Boolean(true), nodeRecord.getVisible());
        tags = PostgresUtils.postgresObjToHStore((PGobject)nodeRecord.getTags());
        Assert.assertNotNull(tags);
        Assert.assertEquals(1, tags.size());
        Assert.assertEquals("val 3b", tags.get("key 3b"));

        nodeRecord = nodes.get(nodeIdsArr[2]);
        Assert.assertTrue(
          nodeRecord.getTags() == null ||
          StringUtils.isEmpty(((PGobject)nodeRecord.getTags()).getValue()));

        nodeRecord = nodes.get(nodeIdsArr[3]);
        tags = PostgresUtils.postgresObjToHStore((PGobject)nodeRecord.getTags());
        Assert.assertNotNull(tags);
        Assert.assertEquals(1, tags.size());
        Assert.assertEquals("val 3", tags.get("key 3"));

        nodeRecord = nodes.get(nodeIdsArr[4]);
        tags = PostgresUtils.postgresObjToHStore((PGobject)nodeRecord.getTags());
        Assert.assertNotNull(tags);
        Assert.assertEquals(1, tags.size());
        Assert.assertEquals("val 4", tags.get("key 4"));
      }
      catch (Exception e)
      {
        Assert.fail("Error checking nodes: " + e.getMessage());
      }

      try
      {
        final Map<Long, CurrentWays> ways =

        		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentWaysTbl)
        		.map(currentWaysTbl.id, currentWaysTbl);
        		;
        Assert.assertEquals(3, ways.size());

        CurrentWays wayRecord = ways.get(wayIdsArr[0]);
        Assert.assertEquals(new Long(changesetId), wayRecord.getChangesetId());
        Assert.assertEquals(wayIdsArr[0], wayRecord.getId());
        Assert.assertTrue(wayRecord.getTimestamp().before(now));
        Assert.assertEquals(new Long(2), wayRecord.getVersion());
        Assert.assertEquals(new Boolean(true), wayRecord.getVisible());
        List<CurrentWayNodes> wayNodes =

        new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentWayNodesTbl)
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
        //verify the previously existing tags
        Map<String, String> tags = PostgresUtils.postgresObjToHStore((PGobject)wayRecord.getTags());
        Assert.assertNotNull(tags);
        Assert.assertEquals(1, tags.size());
        Assert.assertEquals("val 2", tags.get("key 2"));

        wayRecord = ways.get(wayIdsArr[1]);
        Assert.assertEquals(new Long(changesetId), wayRecord.getChangesetId());
        Assert.assertEquals(wayIdsArr[1], wayRecord.getId());
        Assert.assertTrue(wayRecord.getTimestamp().before(now));
        Assert.assertEquals(new Long(2), wayRecord.getVersion());
        Assert.assertEquals(new Boolean(true), wayRecord.getVisible());
        wayNodes =
        		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentWayNodesTbl)
            .where(currentWayNodesTbl.wayId.eq(wayIdsArr[1]))
            .orderBy(currentWayNodesTbl.sequenceId.asc()).list(currentWayNodesTbl);
        Assert.assertEquals(2, wayNodes.size());
        wayNode = wayNodes.get(0);
        Assert.assertEquals(nodeIdsArr[4], wayNode.getNodeId());
        Assert.assertEquals(new Long(1), wayNode.getSequenceId());
        Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());
        wayNode = wayNodes.get(1);
        Assert.assertEquals(nodeIdsArr[2], wayNode.getNodeId());
        Assert.assertEquals(new Long(2), wayNode.getSequenceId());
        Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());
        //verify the way with no tags
        Assert.assertTrue(
            wayRecord.getTags() == null ||
            StringUtils.isEmpty(((PGobject)wayRecord.getTags()).getValue()));

        //verify the created ways
        wayRecord = ways.get(wayIdsArr[2]);
        Assert.assertEquals(new Long(changesetId), wayRecord.getChangesetId());
        Assert.assertEquals(wayIdsArr[2], wayRecord.getId());
        Assert.assertTrue(wayRecord.getTimestamp().before(now));
        Assert.assertEquals(new Long(1), wayRecord.getVersion());
        Assert.assertEquals(new Boolean(true), wayRecord.getVisible());
        wayNodes =
        		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentWayNodesTbl)
            .where(currentWayNodesTbl.wayId.eq(wayIdsArr[2]))
            .orderBy(currentWayNodesTbl.sequenceId.asc()).list(currentWayNodesTbl);
        Assert.assertEquals(2, wayNodes.size());
        wayNode = wayNodes.get(0);
        Assert.assertEquals(nodeIdsArr[0], wayNode.getNodeId());
        Assert.assertEquals(new Long(1), wayNode.getSequenceId());
        Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());
        wayNode = wayNodes.get(1);
        Assert.assertEquals(nodeIdsArr[1], wayNode.getNodeId());
        Assert.assertEquals(new Long(2), wayNode.getSequenceId());
        Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());
        //verify the created tags
        tags = PostgresUtils.postgresObjToHStore((PGobject)wayRecord.getTags());
        Assert.assertNotNull(tags);
        Assert.assertEquals(1, tags.size());
        Assert.assertEquals("val 3", tags.get("key 3"));
      }
      catch (Exception e)
      {
        Assert.fail("Error checking ways: " + e.getMessage());
      }

      try
      {
        final Map<Long, CurrentRelations> relations =
        		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentRelationsTbl)
            .map(currentRelationsTbl.id, currentRelationsTbl);

        CurrentRelations relationRecord =
          relations.get(relationIdsArr[0]);
        Assert.assertEquals(new Long(changesetId), relationRecord.getChangesetId());
        Assert.assertEquals(relationIdsArr[0], relationRecord.getId());
        Assert.assertTrue(relationRecord.getTimestamp().before(now));
        Assert.assertEquals(new Long(2), relationRecord.getVersion());
        Assert.assertEquals(new Boolean(true), relationRecord.getVisible());
        List<CurrentRelationMembers> members =
        		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentRelationMembersTbl)
            .where(currentRelationMembersTbl.relationId.eq(relationIdsArr[0]))
            .orderBy(currentRelationMembersTbl.sequenceId.asc()).list(currentRelationMembersTbl);
        Assert.assertEquals(3, members.size());
        CurrentRelationMembers member = members.get(0);
        Assert.assertEquals(
          relationRecord.getId(), member.getRelationId());
        Assert.assertEquals(DbUtils.nwr_enum.way, member.getMemberType());
        Assert.assertEquals("role4", member.getMemberRole());
        Assert.assertEquals(
          new Integer(1), member.getSequenceId());

        Assert.assertEquals(wayIdsArr[1], member.getMemberId());
        member = members.get(1);
        Assert.assertEquals(
          relationRecord.getId(), member.getRelationId());
        Assert.assertEquals(DbUtils.nwr_enum.way, member.getMemberType());
        Assert.assertEquals("role2", member.getMemberRole());
        Assert.assertEquals(
          new Integer(2), member.getSequenceId());

        Assert.assertEquals(wayIdsArr[0], member.getMemberId());
        member = members.get(2);
        Assert.assertEquals(
          relationRecord.getId(), member.getRelationId());
        Assert.assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
        Assert.assertEquals("", member.getMemberRole());
        Assert.assertEquals(
          new Integer(3), member.getSequenceId());

        Assert.assertEquals(nodeIdsArr[2], member.getMemberId());
        //removed tag
        Assert.assertTrue(
            relationRecord.getTags() == null ||
            StringUtils.isEmpty(((PGobject)relationRecord.getTags()).getValue()));

        relationRecord = relations.get(relationIdsArr[1]);
        Assert.assertEquals(new Long(changesetId), relationRecord.getChangesetId());
        Assert.assertEquals(relationIdsArr[1], relationRecord.getId());

        Assert.assertTrue(relationRecord.getTimestamp().before(now));
        Assert.assertEquals(new Long(2), relationRecord.getVersion());
        Assert.assertEquals(new Boolean(true), relationRecord.getVisible());
        members =
        		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentRelationMembersTbl)
            .where(currentRelationMembersTbl.relationId.eq(relationIdsArr[1]))
            .orderBy(currentRelationMembersTbl.sequenceId.asc()).list(currentRelationMembersTbl);
        Assert.assertEquals(2, members.size());
        member = members.get(0);
        Assert.assertEquals(
          relationRecord.getId(), member.getRelationId());
        Assert.assertEquals(DbUtils.nwr_enum.relation, member.getMemberType());
        Assert.assertEquals("role1", member.getMemberRole());
        Assert.assertEquals(
          new Integer(1), member.getSequenceId());

        Assert.assertEquals(relationIdsArr[0], member.getMemberId());
        member = members.get(1);
        Assert.assertEquals(
          relationRecord.getId(), member.getRelationId());
        Assert.assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
        Assert.assertEquals("", member.getMemberRole());
        Assert.assertEquals(
          new Integer(2), member.getSequenceId());

        Assert.assertEquals(nodeIdsArr[4], member.getMemberId());
        Map<String, String> tags = PostgresUtils.postgresObjToHStore((PGobject)relationRecord.getTags());
        Assert.assertNotNull(tags);
        Assert.assertEquals(2, tags.size());
        Assert.assertEquals("val 2", tags.get("key 2"));
        Assert.assertEquals("val 3", tags.get("key 3"));

        relationRecord = relations.get(relationIdsArr[2]);
        Assert.assertEquals(new Long(changesetId), relationRecord.getChangesetId());
        Assert.assertEquals(relationIdsArr[2], relationRecord.getId());

        Assert.assertTrue(relationRecord.getTimestamp().before(now));
        Assert.assertEquals(new Long(1), relationRecord.getVersion());
        Assert.assertEquals(new Boolean(true), relationRecord.getVisible());
        members =
        		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentRelationMembersTbl)
            .where(currentRelationMembersTbl.relationId.eq(relationIdsArr[2]))
            .orderBy(currentRelationMembersTbl.sequenceId.asc()).list(currentRelationMembersTbl);
        Assert.assertEquals(1, members.size());
        member = members.get(0);
        Assert.assertEquals(
          relationRecord.getId(), member.getRelationId());
        Assert.assertEquals(DbUtils.nwr_enum.way, member.getMemberType());
        Assert.assertEquals("", member.getMemberRole());
        Assert.assertEquals(
          new Integer(1), member.getSequenceId());

        Assert.assertEquals(wayIdsArr[1], member.getMemberId());
        tags = PostgresUtils.postgresObjToHStore((PGobject)relationRecord.getTags());
        Assert.assertNotNull(tags);
        Assert.assertEquals(1, tags.size());
        Assert.assertEquals("val 4", tags.get("key 4"));

        relationRecord = relations.get(relationIdsArr[3]);
        Assert.assertEquals(new Long(changesetId), relationRecord.getChangesetId());
        Assert.assertEquals(relationIdsArr[3], relationRecord.getId());

        Assert.assertTrue(relationRecord.getTimestamp().before(now));
        Assert.assertEquals(new Long(1), relationRecord.getVersion());
        Assert.assertEquals(new Boolean(true), relationRecord.getVisible());
        members =
        		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentRelationMembersTbl)
            .where(currentRelationMembersTbl.relationId.eq(relationIdsArr[3]))
            .orderBy(currentRelationMembersTbl.sequenceId.asc()).list(currentRelationMembersTbl);
        Assert.assertEquals(1, members.size());
        member = members.get(0);
        Assert.assertEquals(
          relationRecord.getId(), member.getRelationId());
        Assert.assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
        Assert.assertEquals("role1", member.getMemberRole());
        Assert.assertEquals(
          new Integer(1), member.getSequenceId());

        Assert.assertEquals(nodeIdsArr[2], member.getMemberId());
        Assert.assertTrue(
            relationRecord.getTags() == null ||
            StringUtils.isEmpty(((PGobject)relationRecord.getTags()).getValue()));
      }
      catch (Exception e)
      {
        Assert.fail("Error checking relations: " + e.getMessage());
      }

      try
      {
        Assert.assertNotNull(changeset);
        Assert.assertTrue(changeset.getCreatedAt().before(now));
        Assert.assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));
        Assert.assertEquals(new Integer(18), changeset.getNumChanges());
        Assert.assertEquals(new Long(userId), changeset.getUserId());

        BoundingBox expandedBounds = new BoundingBox(originalBounds);
        expandedBounds.expand(
          updatedBounds,
          Double.parseDouble(HootProperties.getDefault("changesetBoundsExpansionFactorDeegrees")));
        hoot.services.models.osm.Changeset hootChangeset =
          new hoot.services.models.osm.Changeset(mapId, changesetId, conn);
        BoundingBox changesetBounds = hootChangeset.getBounds();
        Assert.assertTrue(changesetBounds.equals(expandedBounds));
      }
      catch (Exception e)
      {
        Assert.fail("Error checking changeset: " + e.getMessage());
      }
    }
    catch (Exception e)
    {
      log.error(e.getMessage());
      throw e;
    }
  }

  @Test
  @Category(UnitTest.class)
  public void testUploadModifyClosedPolygon() throws Exception
  {
    try
    {
      final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
      final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
      final Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
      final Long[] nodeIdsArr = nodeIds.toArray(new Long[]{});

      final Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
      final Long[] wayIdsArr = wayIds.toArray(new Long[]{});

      //Now, update some of the elements and their tags.
      //TODO: why was this disabled?
      //final BoundingBox updatedBounds =
        //OsmTestUtils.createAfterModifiedTestChangesetBounds();
      Document responseData = null;
      try
      {
        responseData =
          resource()
            .path("api/0.6/changeset/" + changesetId + "/upload")
            .queryParam("mapId", "" + mapId)
            .type(MediaType.TEXT_XML)
            .accept(MediaType.TEXT_XML)
            .post(
              Document.class,
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
      catch (UniformInterfaceException e)
      {
        ClientResponse r = e.getResponse();
        Assert.fail("Unexpected response " + r.getStatus() + " " + r.getEntity(String.class));
      }
      Assert.assertNotNull(responseData);

      XPath xpath = XmlUtils.createXPath();
      try
      {
        NodeList returnedWays = XPathAPI.selectNodeList(responseData, "//osm/diffResult/way");
        Assert.assertEquals(1, returnedWays.getLength());

        Assert.assertEquals(
          (long)wayIdsArr[0],
          Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@old_id",
          responseData)));
        Assert.assertEquals(
          Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@old_id", responseData)),
          Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@new_id", responseData)));
        Assert.assertEquals(
          2, Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@new_version", responseData)));
      }
      catch (XPathExpressionException e)
      {
        Assert.fail("Error parsing response document: " + e.getMessage());
      }

      final Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
      QChangesets changesets = QChangesets.changesets;
      hoot.services.db2.Changesets changeset =
      new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(changesets)
      .where(changesets.id.eq(changesetId)).singleResult(changesets);
      try
      {
        final Map<Long, CurrentWays> ways =
        		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentWaysTbl)
        		.map(currentWaysTbl.id, currentWaysTbl);
        Assert.assertEquals(3, ways.size());

        CurrentWays wayRecord = ways.get(wayIdsArr[0]);
        Assert.assertEquals(new Long(changesetId), wayRecord.getChangesetId());
        Assert.assertEquals(wayIdsArr[0], wayRecord.getId());
        Assert.assertTrue(wayRecord.getTimestamp().before(now));
        Assert.assertEquals(new Long(2), wayRecord.getVersion());
        Assert.assertEquals(new Boolean(true), wayRecord.getVisible());
        List<CurrentWayNodes> wayNodes =
        		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentWayNodesTbl)
            .where(currentWayNodesTbl.wayId.eq(wayIdsArr[0]))
            .orderBy(currentWayNodesTbl.sequenceId.asc()).list(currentWayNodesTbl);
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
      catch (Exception e)
      {
        Assert.fail("Error checking ways: " + e.getMessage());
      }

      try
      {
        Assert.assertNotNull(changeset);
        Assert.assertTrue(changeset.getCreatedAt().before(now));
        Assert.assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));

        Assert.assertEquals(new Integer(13), changeset.getNumChanges());
        Assert.assertEquals(new Long(userId), changeset.getUserId());

        BoundingBox expandedBounds = new BoundingBox(originalBounds);
        //System.out.println("expandedBounds: " + expandedBounds.toString());
//        expandedBounds.expand(
//          updatedBounds,
//          Double.parseDouble(HootProperties.getDefault("changesetBoundsExpansionFactorDeegrees")));
        hoot.services.models.osm.Changeset hootChangeset =
          new hoot.services.models.osm.Changeset(mapId, changesetId, conn);
        BoundingBox changesetBounds = hootChangeset.getBounds();
        //System.out.println("changesetBounds: " + changesetBounds.toString());
        Assert.assertTrue(changesetBounds.equals(expandedBounds));
      }
      catch (Exception e)
      {
        Assert.fail("Error checking changeset: " + e.getMessage());
      }
    }
    catch (Exception e)
    {
      log.error(e.getMessage());
      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testUploadModifyDuplicateNodeIds() throws Exception
  {
    final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
    final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
    final Set<Long> nodeIds =
      OsmTestUtils.createTestNodes(changesetId, originalBounds);
    final Long[] nodeIdsArr = nodeIds.toArray(new Long[]{});

    final Set<Long> wayIds =
      OsmTestUtils.createTestWays(changesetId, nodeIds);

    final Set<Long> relationIds =
      OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);


    //Try to create two nodes with the same ID.  A failure should occur and no data in the system
    //should be modified.
    try
    {
      resource()
        .path("api/0.6/changeset/" + changesetId + "/upload")
        .queryParam("mapId", "" + mapId)
        .type(MediaType.TEXT_XML)
        .accept(MediaType.TEXT_XML)
        .post(
          Document.class,
          "<osmChange version=\"0.3\" generator=\"iD\">" +
            "<create/>" +
            "<modify>" +
              "<node id=\"" + nodeIdsArr[0] + "\" lon=\"" + originalBounds.getMinLon() + "\" " +
                "lat=\"" + originalBounds.getMinLat() + "\" version=\"1\" changeset=\"" +
                changesetId + "\">" +
                "<tag k=\"key 1b\" v=\"val 1b\"></tag>" +
                "<tag k=\"key 2b\" v=\"val 2b\"></tag>" +
              "</node>" +
              "<node id=\"" + nodeIdsArr[0] + "\" lon=\"" + originalBounds.getMinLon() + "\" " +
                "lat=\"" + originalBounds.getMinLat() + "\" version=\"1\" changeset=\"" +
                changesetId + "\">" +
                "<tag k=\"key 3\" v=\"val 3\"></tag>" +
              "</node>" +
            "</modify>" +
            "<delete if-unused=\"true\"/>" +
          "</osmChange>");
    }
    catch (UniformInterfaceException e)
    {
      ClientResponse r = e.getResponse();
      Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
      Assert.assertTrue(r.getEntity(String.class).contains("Duplicate OSM element ID"));

      OsmTestUtils.verifyTestDataUnmodified(
        originalBounds, changesetId, nodeIds, wayIds, relationIds);

      throw e;
    }
    catch (Exception e)
    {
      log.error(e.getMessage());
      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testUploadModifyInvalidChangesetSpecifiedInUrl() throws Exception
  {
    final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
    final long changesetId = OsmTestUtils.createTestChangeset(null);
    final Set<Long> nodeIds =
      OsmTestUtils.createTestNodes(changesetId, originalBounds);
    final Long[] nodeIdsArr = nodeIds.toArray(new Long[]{});


    //Upload a changeset where the changeset specified in the URL doesn't exist.  A failure
    //should occur and no data in the system should be modified.
    try
    {
      resource()
        .path("api/0.6/changeset/" + (changesetId + 1) + "/upload")
        .queryParam("mapId", "" + mapId)
        .type(MediaType.TEXT_XML)
        .accept(MediaType.TEXT_XML)
        .post(
          Document.class,
          "<osmChange version=\"0.3\" generator=\"iD\">" +
            "<modify>" +
              "<node id=\"" + nodeIdsArr[0] + "\" lon=\"" + originalBounds.getMinLon() +
                "\" lat=\"" + originalBounds.getMinLat() + "\" version=\"0\" changeset=\"" +
                changesetId + "\">" +
                "<tag k=\"name 1\" v=\"val 1\"/>" +
                "<tag k=\"name 2\" v=\"val 2\"/>" +
              "</node>" +
              "<node id=\"" + nodeIdsArr[1] + "\" lon=\"" + originalBounds.getMinLon() +
                "\" lat=\"" + originalBounds.getMinLat() + "\" version=\"0\" changeset=\"" +
                changesetId + "\">" +
                "<tag k=\"name 1\" v=\"val 1\"/>" +
              "</node>" +
            "</modify>" +
            "<create/>" +
            "<delete if-unused=\"true\"/>" +
          "</osmChange>");
    }
    catch (UniformInterfaceException e)
    {
      ClientResponse r = e.getResponse();
      Assert.assertEquals(404, r.getStatus());
      Assert.assertTrue(r.getEntity(String.class).contains("Changeset to be updated does not exist"));

      OsmTestUtils.verifyTestChangesetUnmodified(changesetId);

      throw e;
    }
    catch (Exception e)
    {
      log.error(e.getMessage());
      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testUploadModifyEmptyNodeCoords() throws Exception
  {
    //Try to update a changeset with nodes that have empty coordinate strings.  A failure should
    //occur and no data in system should be modified.
    uploadModifyNodeCoords("", "");
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testUploadModifyNodeCoordsOutOfBounds() throws Exception
  {
    //Try to update a changeset with nodes that have out of world bounds coordinate values.
    //A failure should occur and no data in system should be modified.
    uploadModifyNodeCoords("91", "-181");
  }

  private void uploadModifyNodeCoords(final String lat, final String lon) throws Exception
  {
    final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
    final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
    final Set<Long> nodeIds =
      OsmTestUtils.createTestNodes(changesetId, originalBounds);

    final Long[] nodeIdsArr = nodeIds.toArray(new Long[]{});
    final Set<Long> wayIds =
      OsmTestUtils.createTestWays(changesetId, nodeIds);

    final Set<Long> relationIds =
      OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);


    //Now update the changeset via the service where one of the nodes has a empty coords.  A
    //failure should occur and no data in the system should be modified.
    try
    {
      resource()
        .path("api/0.6/changeset/" + changesetId + "/upload")
        .queryParam("mapId", "" + mapId)
        .type(MediaType.TEXT_XML)
        .accept(MediaType.TEXT_XML)
        .post(
          Document.class,
          "<osmChange version=\"0.3\" generator=\"iD\">" +
            "<create/>" +
            "<modify>" +
              "<node id=\"" + nodeIdsArr[0] + "\" lon=\"" + originalBounds.getMinLon() + "\" " +
                "lat=\"" + originalBounds.getMinLat() + "\" version=\"1\" changeset=\"" +
                changesetId + "\">" +
                "<tag k=\"key 1\" v=\"val 1\"></tag>" +
                "<tag k=\"key 2\" v=\"val 2\"></tag>" +
              "</node>" +
              "<node id=\"" + nodeIdsArr[1] + "\" lon=\"" + lon + "\" " +
                "lat=\"" + lat + "\" version=\"1\" changeset=\"" + changesetId + "\">" +
                "<tag k=\"key 3\" v=\"val 3\"></tag>" +
              "</node>" +
            "</modify>" +
            "<delete if-unused=\"true\"/>" +
          "</osmChange>");
    }
    catch (UniformInterfaceException e)
    {
      ClientResponse r = e.getResponse();
      Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
      Assert.assertTrue(r.getEntity(String.class).contains("Error uploading changeset with ID"));

      OsmTestUtils.verifyTestDataUnmodified(
        originalBounds, changesetId, nodeIds, wayIds, relationIds);

      throw e;
    }
    catch (Exception e)
    {
      log.error(e.getMessage());
      throw e;
    }
  }

  @Test
  @Category(UnitTest.class)
  public void testUploadModifyNoTags() throws Exception
  {
    try
    {
      final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
      final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
      final Set<Long> nodeIds =
        OsmTestUtils.createTestNodes(changesetId, originalBounds);
      final Long[] nodeIdsArr = nodeIds.toArray(new Long[]{});

      final Set<Long> wayIds =
        OsmTestUtils.createTestWays(changesetId, nodeIds);
      final Long[] wayIdsArr = wayIds.toArray(new Long[]{});

      final Set<Long> relationIds =
        OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
      final Long[] relationIdsArr = relationIds.toArray(new Long[]{});


      //Now update the changeset with elements that have no tags in them, which is allowed.  The
      //existing tags for the specified elements will be deleted, since every request must specify
      //all the tags it wants retained for each element.  The elements not referenced in this modify
      //request should retain their tags.
      final BoundingBox updatedBounds =
        OsmTestUtils.createAfterModifiedTestChangesetBounds();
      Document responseData = null;
      try
      {
        responseData =
          resource()
            .path("api/0.6/changeset/" + changesetId + "/upload")
            .queryParam("mapId", "" + mapId)
            .type(MediaType.TEXT_XML)
            .accept(MediaType.TEXT_XML)
            .post(
              Document.class,
              "<osmChange version=\"0.3\" generator=\"iD\">" +
                "<create/>" +
                "<modify>" +
                  "<node id=\"" + nodeIdsArr[0] + "\" lon=\"" + updatedBounds.getMinLon() +
                    "\" lat=\"" + updatedBounds.getMinLat() + "\" version=\"1\" changeset=\"" +
                    changesetId + "\">" +
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
              "</osmChange>");
      }
      catch (UniformInterfaceException e)
      {
        ClientResponse r = e.getResponse();
        Assert.fail("Unexpected response " + r.getStatus() + " " + r.getEntity(String.class));
      }
      Assert.assertNotNull(responseData);

      XPath xpath = XmlUtils.createXPath();
      try
      {
        NodeList returnedNodes = XPathAPI.selectNodeList(responseData, "//osm/diffResult/node");
        Assert.assertEquals(2, returnedNodes.getLength());

        Assert.assertEquals(
          (long)nodeIdsArr[0],
          Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@old_id",
          responseData)));
        Assert.assertEquals(
          Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@old_id", responseData)),
          Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@new_id", responseData)));
        Assert.assertEquals(
          2, Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@new_version", responseData)));

        Assert.assertEquals(
          (long)nodeIdsArr[1],
          Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@old_id",
          responseData)));
        Assert.assertEquals(
          Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@old_id", responseData)),
          Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@new_id", responseData)));
        Assert.assertEquals(
          2, Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@new_version", responseData)));

        NodeList returnedWays = XPathAPI.selectNodeList(responseData, "//osm/diffResult/way");
        Assert.assertEquals(2, returnedWays.getLength());

        Assert.assertEquals(
          (long)wayIdsArr[0],
          Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@old_id",
          responseData)));
        Assert.assertEquals(
          Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@old_id", responseData)),
          Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@new_id", responseData)));
        Assert.assertEquals(
          2, Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@new_version", responseData)));

        Assert.assertEquals(
          (long)wayIdsArr[1],
          Long.parseLong(xpath.evaluate("//osm/diffResult/way[2]/@old_id",
          responseData)));
        Assert.assertEquals(
          Long.parseLong(xpath.evaluate("//osm/diffResult/way[2]/@old_id", responseData)),
          Long.parseLong(xpath.evaluate("//osm/diffResult/way[2]/@new_id", responseData)));
        Assert.assertEquals(
          2, Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@new_version", responseData)));

        NodeList returnedRelations =
          XPathAPI.selectNodeList(responseData, "//osm/diffResult/relation");
        Assert.assertEquals(2, returnedRelations.getLength());

        Assert.assertEquals(
          (long)relationIdsArr[0],
          Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@old_id",
          responseData)));
        Assert.assertEquals(
          Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@old_id", responseData)),
          Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@new_id", responseData)));
        Assert.assertEquals(
          2, Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@new_version", responseData)));

        Assert.assertEquals(
          (long)relationIdsArr[1],
          Long.parseLong(xpath.evaluate("//osm/diffResult/relation[2]/@old_id",
          responseData)));
        Assert.assertEquals(
          Long.parseLong(xpath.evaluate("//osm/diffResult/relation[2]/@old_id", responseData)),
          Long.parseLong(xpath.evaluate("//osm/diffResult/relation[2]/@new_id", responseData)));
        Assert.assertEquals(
          2, Long.parseLong(xpath.evaluate("//osm/diffResult/relation[2]/@new_version", responseData)));
      }
      catch (XPathExpressionException e)
      {
        Assert.fail("Error parsing response document: " + e.getMessage());
      }

      final Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
      QChangesets changesets = QChangesets.changesets;
      hoot.services.db2.Changesets changeset =
      new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(changesets)
      .where(changesets.id.eq(changesetId)).singleResult(changesets);


      try
      {
        final Map<Long, CurrentNodes> nodes =
        		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentNodesTbl)
        		.map(currentNodesTbl.id, currentNodesTbl);
        Assert.assertEquals(5, nodes.size());

        CurrentNodes nodeRecord = nodes.get(nodeIdsArr[0]);
        Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
        Assert.assertEquals(
          new Double(updatedBounds.getMinLat()),
          nodeRecord.getLatitude());
        Assert.assertEquals(
          new Double(updatedBounds.getMinLon()),
          nodeRecord.getLongitude());
        Assert.assertEquals(nodeIdsArr[0], nodeRecord.getId());

        Assert.assertEquals(
          new Long(QuadTileCalculator.tileForPoint(
            nodeRecord.getLatitude(),
            nodeRecord.getLongitude())),
          nodeRecord.getTile());
        Assert.assertTrue(nodeRecord.getTimestamp().before(now));
        Assert.assertEquals(new Long(2), nodeRecord.getVersion());
        Assert.assertEquals(new Boolean(true), nodeRecord.getVisible());
        Assert.assertTrue(
            nodeRecord.getTags() == null ||
            StringUtils.isEmpty(((PGobject)nodeRecord.getTags()).getValue()));

        nodeRecord = nodes.get(nodeIdsArr[1]);
        Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
        Assert.assertEquals(
          new Double(updatedBounds.getMinLat()),
          nodeRecord.getLatitude());
        Assert.assertEquals(
          new Double(originalBounds.getMaxLon()),
          nodeRecord.getLongitude());
        Assert.assertEquals(nodeIdsArr[1], nodeRecord.getId());
        Assert.assertEquals(
          new Long(QuadTileCalculator.tileForPoint(
            nodeRecord.getLatitude(),
            nodeRecord.getLongitude())),
          nodeRecord.getTile());
        Assert.assertTrue(nodeRecord.getTimestamp().before(now));
        Assert.assertEquals(new Long(2), nodeRecord.getVersion());
        Assert.assertEquals(new Boolean(true), nodeRecord.getVisible());
        Assert.assertTrue(
            nodeRecord.getTags() == null ||
            StringUtils.isEmpty(((PGobject)nodeRecord.getTags()).getValue()));

        nodeRecord = nodes.get(nodeIdsArr[2]);
        Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
        Assert.assertEquals(
          new Double(originalBounds.getMinLat()),
          nodeRecord.getLatitude());
        Assert.assertEquals(
          new Double(originalBounds.getMinLon()),
          nodeRecord.getLongitude());
        Assert.assertEquals(nodeIdsArr[2], nodeRecord.getId());
        Assert.assertEquals(
          new Long(QuadTileCalculator.tileForPoint(
            nodeRecord.getLatitude(),
            nodeRecord.getLongitude())),
          nodeRecord.getTile());
        Assert.assertTrue(nodeRecord.getTimestamp().before(now));
        Assert.assertEquals(new Long(1), nodeRecord.getVersion());
        Assert.assertEquals(new Boolean(true), nodeRecord.getVisible());
        Assert.assertTrue(
            nodeRecord.getTags() == null ||
            StringUtils.isEmpty(((PGobject)nodeRecord.getTags()).getValue()));

        nodeRecord = nodes.get(nodeIdsArr[3]);
        Map<String, String> tags = PostgresUtils.postgresObjToHStore((PGobject)nodeRecord.getTags());
        Assert.assertNotNull(tags);
        Assert.assertEquals(1, tags.size());
        Assert.assertEquals("val 3", tags.get("key 3"));

        nodeRecord = nodes.get(nodeIdsArr[4]);
        tags = PostgresUtils.postgresObjToHStore((PGobject)nodeRecord.getTags());
        Assert.assertNotNull(tags);
        Assert.assertEquals(1, tags.size());
        Assert.assertEquals("val 4", tags.get("key 4"));
      }
      catch (Exception e)
      {
        Assert.fail("Error checking nodes: " + e.getMessage());
      }

      try
      {
        final Map<Long, CurrentWays> ways =
        		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentWaysTbl)
        		.map(currentWaysTbl.id, currentWaysTbl);
        Assert.assertEquals(3, ways.size());

        CurrentWays wayRecord = ways.get(wayIdsArr[0]);
        Assert.assertEquals(new Long(changesetId), wayRecord.getChangesetId());
        Assert.assertEquals(wayIdsArr[0], wayRecord.getId());
        Assert.assertTrue(wayRecord.getTimestamp().before(now));
        Assert.assertEquals(new Long(2), wayRecord.getVersion());
        Assert.assertEquals(new Boolean(true), wayRecord.getVisible());
        List<CurrentWayNodes> wayNodes =
        		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentWayNodesTbl)
            .where(currentWayNodesTbl.wayId.eq(wayIdsArr[0]))
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
        Assert.assertTrue(
            wayRecord.getTags() == null ||
            StringUtils.isEmpty(((PGobject)wayRecord.getTags()).getValue()));

        wayRecord = ways.get(wayIdsArr[1]);
        Assert.assertEquals(new Long(changesetId), wayRecord.getChangesetId());
        Assert.assertEquals(wayIdsArr[1], wayRecord.getId());
        Assert.assertTrue(wayRecord.getTimestamp().before(now));
        Assert.assertEquals(new Long(2), wayRecord.getVersion());
        Assert.assertEquals(new Boolean(true), wayRecord.getVisible());
        wayNodes =
        		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentWayNodesTbl)
            .where(currentWayNodesTbl.wayId.eq(wayIdsArr[1]))
            .orderBy(currentWayNodesTbl.sequenceId.asc()).list(currentWayNodesTbl);
        Assert.assertEquals(2, wayNodes.size());
        wayNode = wayNodes.get(0);
        Assert.assertEquals(nodeIdsArr[0], wayNode.getNodeId());
        Assert.assertEquals(new Long(1), wayNode.getSequenceId());
        Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());
        wayNode = wayNodes.get(1);
        Assert.assertEquals(nodeIdsArr[1], wayNode.getNodeId());
        Assert.assertEquals(new Long(2), wayNode.getSequenceId());
        Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());
        Assert.assertTrue(
            wayRecord.getTags() == null ||
            StringUtils.isEmpty(((PGobject)wayRecord.getTags()).getValue()));

        wayRecord = ways.get(wayIdsArr[2]);
        Assert.assertEquals(new Long(changesetId), wayRecord.getChangesetId());
        Assert.assertEquals(wayIdsArr[2], wayRecord.getId());
        Assert.assertTrue(wayRecord.getTimestamp().before(now));
        Assert.assertEquals(new Long(1), wayRecord.getVersion());
        Assert.assertEquals(new Boolean(true), wayRecord.getVisible());
        wayNodes =
        		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentWayNodesTbl)
            .where(currentWayNodesTbl.wayId.eq(wayIdsArr[2]))
            .orderBy(currentWayNodesTbl.sequenceId.asc()).list(currentWayNodesTbl);
        Assert.assertEquals(2, wayNodes.size());
        wayNode = wayNodes.get(0);
        Assert.assertEquals(nodeIdsArr[0], wayNode.getNodeId());
        Assert.assertEquals(new Long(1), wayNode.getSequenceId());
        Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());
        wayNode = wayNodes.get(1);
        Assert.assertEquals(nodeIdsArr[1], wayNode.getNodeId());
        Assert.assertEquals(new Long(2), wayNode.getSequenceId());
        Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());
        Map<String, String> tags = PostgresUtils.postgresObjToHStore((PGobject)wayRecord.getTags());
        Assert.assertNotNull(tags);
        Assert.assertEquals(1, tags.size());
        Assert.assertEquals("val 3", tags.get("key 3"));
      }
      catch (Exception e)
      {
        Assert.fail("Error checking ways: " + e.getMessage());
      }

      try
      {
        final Map<Long, CurrentRelations> relations =
        		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentRelationsTbl)
        		.map(currentRelationsTbl.id, currentRelationsTbl);
        Assert.assertEquals(4, relations.size());

        CurrentRelations relationRecord =
          relations.get(relationIdsArr[0]);
        Assert.assertEquals(new Long(changesetId), relationRecord.getChangesetId());
        Assert.assertEquals(relationIdsArr[0], relationRecord.getId());
        Assert.assertTrue(relationRecord.getTimestamp().before(now));
        Assert.assertEquals(new Long(2), relationRecord.getVersion());
        Assert.assertEquals(new Boolean(true), relationRecord.getVisible());
        List<CurrentRelationMembers> members =
        		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentRelationMembersTbl)
            .where(currentRelationMembersTbl.relationId.eq(relationIdsArr[0])
            		)
            .orderBy(currentRelationMembersTbl.sequenceId.asc())
            .list(currentRelationMembersTbl);

        Assert.assertEquals(3, members.size());
        CurrentRelationMembers member = members.get(0);
        Assert.assertEquals(
          relationRecord.getId(), member.getRelationId());
        Assert.assertEquals(DbUtils.nwr_enum.way, member.getMemberType());
        Assert.assertEquals("role4", member.getMemberRole());
        Assert.assertEquals(
          new Integer(1), member.getSequenceId());

        Assert.assertEquals(wayIdsArr[1], member.getMemberId());
        member = members.get(1);
        Assert.assertEquals(
          relationRecord.getId(), member.getRelationId());
        Assert.assertEquals(DbUtils.nwr_enum.way, member.getMemberType());
        Assert.assertEquals("role2", member.getMemberRole());
        Assert.assertEquals(
          new Integer(2), member.getSequenceId());

        Assert.assertEquals(wayIdsArr[0], member.getMemberId());
        member = members.get(2);
        Assert.assertEquals(
          relationRecord.getId(), member.getRelationId());
        Assert.assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
        Assert.assertEquals("", member.getMemberRole());
        Assert.assertEquals(
          new Integer(3), member.getSequenceId());

        Assert.assertEquals(nodeIdsArr[2], member.getMemberId());
        Assert.assertTrue(
            relationRecord.getTags() == null ||
            StringUtils.isEmpty(((PGobject)relationRecord.getTags()).getValue()));

        relationRecord = relations.get(relationIdsArr[1]);
        Assert.assertEquals(new Long(changesetId), relationRecord.getChangesetId());
        Assert.assertEquals(relationIdsArr[1], relationRecord.getId());

        Assert.assertTrue(relationRecord.getTimestamp().before(now));
        Assert.assertEquals(new Long(2), relationRecord.getVersion());
        Assert.assertEquals(new Boolean(true), relationRecord.getVisible());
        members =
        		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentRelationMembersTbl)
            .where(currentRelationMembersTbl.relationId.eq(relationIdsArr[1])
            		)
            .orderBy(currentRelationMembersTbl.sequenceId.asc())
            .list(currentRelationMembersTbl);
        Assert.assertEquals(2, members.size());
        member = members.get(0);
        Assert.assertEquals(
          relationRecord.getId(), member.getRelationId());
        Assert.assertEquals(DbUtils.nwr_enum.relation, member.getMemberType());
        Assert.assertEquals("role1", member.getMemberRole());
        Assert.assertEquals(
          new Integer(1), member.getSequenceId());

        Assert.assertEquals(relationIdsArr[0], member.getMemberId());
        member = members.get(1);
        Assert.assertEquals(
          relationRecord.getId(), member.getRelationId());
        Assert.assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
        Assert.assertEquals("", member.getMemberRole());
        Assert.assertEquals(
          new Integer(2), member.getSequenceId());

        Assert.assertEquals(nodeIdsArr[4], member.getMemberId());
        Assert.assertTrue(
            relationRecord.getTags() == null ||
            StringUtils.isEmpty(((PGobject)relationRecord.getTags()).getValue()));

        relationRecord = relations.get(relationIdsArr[2]);
        Assert.assertEquals(new Long(changesetId), relationRecord.getChangesetId());
        Assert.assertEquals(relationIdsArr[2], relationRecord.getId());

        Assert.assertTrue(relationRecord.getTimestamp().before(now));
        Assert.assertEquals(new Long(1), relationRecord.getVersion());
        Assert.assertEquals(new Boolean(true), relationRecord.getVisible());
        members =
        		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentRelationMembersTbl)
            .where(currentRelationMembersTbl.relationId.eq(relationIdsArr[2])
            		)
            .orderBy(currentRelationMembersTbl.sequenceId.asc())
            .list(currentRelationMembersTbl);
        Assert.assertEquals(1, members.size());
        member = members.get(0);
        Assert.assertEquals(
          relationRecord.getId(), member.getRelationId());
        Assert.assertEquals(DbUtils.nwr_enum.way, member.getMemberType());
        Assert.assertEquals("", member.getMemberRole());
        Assert.assertEquals(
          new Integer(1), member.getSequenceId());

        Assert.assertEquals(wayIdsArr[1], member.getMemberId());
        Map<String, String> tags = PostgresUtils.postgresObjToHStore((PGobject)relationRecord.getTags());
        Assert.assertNotNull(tags);
        Assert.assertEquals(1, tags.size());
        Assert.assertEquals("val 4", tags.get("key 4"));

        relationRecord = relations.get(relationIdsArr[3]);
        Assert.assertEquals(new Long(changesetId), relationRecord.getChangesetId());
        Assert.assertEquals(relationIdsArr[3], relationRecord.getId());
        Assert.assertTrue(relationRecord.getTimestamp().before(now));
        Assert.assertEquals(new Long(1), relationRecord.getVersion());
        Assert.assertEquals(new Boolean(true), relationRecord.getVisible());
        members =
        		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentRelationMembersTbl)
            .where(currentRelationMembersTbl.relationId.eq(relationIdsArr[3])
            		)
            .orderBy(currentRelationMembersTbl.sequenceId.asc())
            .list(currentRelationMembersTbl);
        Assert.assertEquals(1, members.size());
        member = members.get(0);
        Assert.assertEquals(
          relationRecord.getId(), member.getRelationId());
        Assert.assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
        Assert.assertEquals("role1", member.getMemberRole());
        Assert.assertEquals(
          new Integer(1), member.getSequenceId());

        Assert.assertEquals(nodeIdsArr[2], member.getMemberId());
        Assert.assertTrue(
            relationRecord.getTags() == null ||
            StringUtils.isEmpty(((PGobject)relationRecord.getTags()).getValue()));
      }
      catch (Exception e)
      {
        Assert.fail("Error checking relations: " + e.getMessage());
      }

      try
      {


        changeset =
        new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(changesets)
        .where(changesets.id.eq(changesetId)).singleResult(changesets);

        Assert.assertNotNull(changeset);
        Assert.assertTrue(changeset.getCreatedAt().before(now));
        Assert.assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));
        Assert.assertEquals(new Integer(18), changeset.getNumChanges());
        Assert.assertEquals(new Long(userId), changeset.getUserId());

        BoundingBox expandedBounds = new BoundingBox(originalBounds);
        expandedBounds.expand(
          updatedBounds,
          Double.parseDouble(HootProperties.getDefault("changesetBoundsExpansionFactorDeegrees")));
        hoot.services.models.osm.Changeset hootChangeset =
          new hoot.services.models.osm.Changeset(mapId, changesetId, conn);
        BoundingBox changesetBounds = hootChangeset.getBounds();
        Assert.assertTrue(changesetBounds.equals(expandedBounds));
      }
      catch (Exception e)
      {
        Assert.fail("Error checking changeset: " + e.getMessage());
      }
    }
    catch (Exception e)
    {
      log.error(e.getMessage());
      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testUploadModifyInvalidChangesetIdInNode() throws Exception
  {
    final BoundingBox changeset1Bounds = OsmTestUtils.createStartingTestBounds();
    final long changesetId1 = OsmTestUtils.createTestChangeset(changeset1Bounds);
    final BoundingBox changeset2Bounds = OsmTestUtils.createStartingTestBounds();
    final long changesetId2 = OsmTestUtils.createTestChangeset(changeset2Bounds);

    //link some nodes to the changesets
    final Set<Long> nodeIds1 =
      OsmTestUtils.createTestNodes(changesetId1, changeset1Bounds);
    final Long[] nodeIds1Arr = nodeIds1.toArray(new Long[]{});
    final Set<Long> nodeIds2 =
      OsmTestUtils.createTestNodes(changesetId2, changeset2Bounds);
    final Long[] nodeIds2Arr = nodeIds2.toArray(new Long[]{});

    //Upload a changeset where one of the elements in the changeset lists a changeset ID that
    //doesn't match the changeset ID specified in the URL.  A failure should occur and no data
    //in the system should be modified.
    final BoundingBox updatedBounds =
      OsmTestUtils.createAfterModifiedTestChangesetBounds();
    try
    {
      resource()
        .path("api/0.6/changeset/" + changesetId1 + "/upload")
        .queryParam("mapId", "" + mapId)
        .type(MediaType.TEXT_XML)
        .accept(MediaType.TEXT_XML)
        .post(
          Document.class,
          "<osmChange version=\"0.3\" generator=\"iD\">" +
            "<create/>" +
            "<modify>" +
              "<node id=\"" + nodeIds1Arr[0] + "\" lon=\"" + updatedBounds.getMinLon() + "\" " +
                "lat=\"" + updatedBounds.getMinLat() + "\" version=\"1\" changeset=\"" +
                changesetId1 + "\">" +
                "<tag k=\"key 1b\" v=\"val 1b\"></tag>" +
                "<tag k=\"key 2b\" v=\"val 2b\"></tag>" +
              "</node>" +
              "<node id=\"" + nodeIds2Arr[2] + "\" lon=\"" + updatedBounds.getMinLon() + "\" " +
                "lat=\"" + updatedBounds.getMinLat() + "\" version=\"1\" changeset=\"" +
                changesetId2 + "\">" +
                "<tag k=\"key 3b\" v=\"val 3b\"></tag>" +
              "</node>" +
            "</modify>" +
            "<delete if-unused=\"true\"/>" +
          "</osmChange>");
    }
    catch (UniformInterfaceException e)
    {
      ClientResponse r = e.getResponse();
      Assert.assertEquals(Status.CONFLICT, Status.fromStatusCode(r.getStatus()));
      Assert.assertTrue(r.getEntity(String.class).contains("Invalid changeset ID"));

      OsmTestUtils.verifyTestNodesUnmodified(nodeIds1, changesetId1, changeset1Bounds);
      OsmTestUtils.verifyTestNodesUnmodified(nodeIds2, changesetId2, changeset2Bounds);

      try
      {

      	QChangesets changesets = QChangesets.changesets;
        hoot.services.db2.Changesets changeset =
        new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(changesets)
        .where(changesets.id.eq(changesetId1)).singleResult(changesets);

        Assert.assertNotNull(changeset);
        final Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
        Assert.assertTrue(changeset.getCreatedAt().before(now));
        Assert.assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));
        Assert.assertEquals(new Integer(12), changeset.getNumChanges());
        Assert.assertEquals(new Long(userId), changeset.getUserId());

        //changeset = changesetDao.findById(changesetId2);
        changeset =
            new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(changesets)
            .where(changesets.id.eq(changesetId2)).singleResult(changesets);
        Assert.assertNotNull(changeset);
        Assert.assertTrue(changeset.getCreatedAt().before(now));
        Assert.assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));
        Assert.assertEquals(new Integer(12), changeset.getNumChanges());
        Assert.assertEquals(new Long(userId), changeset.getUserId());

        //make sure the changeset bounds weren't updated
        hoot.services.models.osm.Changeset hootChangeset =
          new hoot.services.models.osm.Changeset(mapId, changesetId1, conn);
        BoundingBox changesetBounds = hootChangeset.getBounds();
        BoundingBox expandedBounds = new BoundingBox();
        final double boundsExpansionFactor =
          Double.parseDouble(HootProperties.getDefault("changesetBoundsExpansionFactorDeegrees"));
        expandedBounds.expand(changeset1Bounds, boundsExpansionFactor);
        Assert.assertTrue(changesetBounds.equals(expandedBounds));

        hootChangeset = new hoot.services.models.osm.Changeset(mapId, changesetId2, conn);
        changesetBounds = hootChangeset.getBounds();
        expandedBounds = new BoundingBox();
        expandedBounds.expand(changeset2Bounds, boundsExpansionFactor);
        Assert.assertTrue(changesetBounds.equals(expandedBounds));
      }
      catch (Exception e2)
      {
        Assert.fail("Error checking changeset: " + e.getMessage());
      }

      throw e;
    }
    catch (Exception e)
    {
      log.error(e.getMessage());
      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testUploadModifyNonExistingNode() throws Exception
  {
    final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
    final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
    final Set<Long> nodeIds =
      OsmTestUtils.createTestNodes(changesetId, originalBounds);
    final Long[] nodeIdsArr = nodeIds.toArray(new Long[]{});

    final Set<Long> wayIds =
      OsmTestUtils.createTestWays(changesetId, nodeIds);

    final Set<Long> relationIds =
      OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);


    final BoundingBox updateBounds =
      OsmTestUtils.createAfterModifiedTestChangesetBounds();
    //Update the changeset with a node that has an ID that doesn't exist.  A failure should occur
    //and no data in the system should be modified.
    try
    {
      resource()
        .path("api/0.6/changeset/" + changesetId + "/upload")
        .queryParam("mapId", "" + mapId)
        .type(MediaType.TEXT_XML)
        .accept(MediaType.TEXT_XML)
        .post(
          Document.class,
          "<osmChange version=\"0.3\" generator=\"iD\">" +
            "<create/>" +
            "<modify>" +
              "<node id=\"" + nodeIdsArr[0] + "\" lon=\"" + originalBounds.getMinLon() + "\" " +
                "lat=\"" + originalBounds.getMinLat() + "\" version=\"1\" changeset=\"" +
                changesetId + "\">" +
                "<tag k=\"key 1\" v=\"val 1\"></tag>" +
                "<tag k=\"key 2\" v=\"val 2\"></tag>" +
              "</node>" +
              "<node id=\"" + (nodeIdsArr[0] + 6) + "\" lon=\"" + updateBounds.getMinLon() + "\" " +
                "lat=\"" + updateBounds.getMinLat() + "\" version=\"1\" changeset=\"" +
                changesetId + "\">" +
                "<tag k=\"key 3\" v=\"val 3\"></tag>" +
              "</node>" +
            "</modify>" +
            "<delete if-unused=\"true\"/>" +
          "</osmChange>");
    }
    catch (UniformInterfaceException e)
    {
      ClientResponse r = e.getResponse();
      Assert.assertEquals(404, r.getStatus());
      Assert.assertTrue(r.getEntity(String.class).contains("to be updated does not exist"));

      //make sure the bad node wasn't created and the other node wasn't updated
      OsmTestUtils.verifyTestDataUnmodified(
        originalBounds, changesetId, nodeIds, wayIds, relationIds);

      throw e;
    }
    catch (Exception e)
    {
      log.error(e.getMessage());
      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testUploadModifyNegativeNodeId() throws Exception
  {
    //hootCoreServicesDatabaseWriterCompatibility must = false for this test to pass
    Properties hootProps = HootProperties.getInstance();
    hootProps.setProperty("hootCoreServicesDatabaseWriterCompatibility", "false");
    HootProperties.setProperties(hootProps);
    Assert.assertEquals(
      false,
      Boolean.parseBoolean(hootProps.getProperty("hootCoreServicesDatabaseWriterCompatibility")));

    final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
    final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
    final Set<Long> nodeIds =
      OsmTestUtils.createTestNodes(changesetId, originalBounds);

    final Long[] nodeIdsArr = nodeIds.toArray(new Long[]{});
    final Set<Long> wayIds =
      OsmTestUtils.createTestWays(changesetId, nodeIds);

    final Set<Long> relationIds =
      OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);


    final BoundingBox updateBounds =
      OsmTestUtils.createAfterModifiedTestChangesetBounds();
    //Update the changeset with a node that has a negative ID, not allowed here since the property
    //was changed above.  A failure should occur and no data in the system should be modified.
    //Techically, the node with the negative ID doesn't exist, so even with the compatibility mode
    //set to false here, this request will still fail.  I'm just looking for a specific error
    //message to be thrown here that's guaranteed to throw before the error caused by trying to
    //update an element that doesn't exist, so that's ok.
    try
    {
      resource()
        .path("api/0.6/changeset/" + changesetId + "/upload")
        .queryParam("mapId", "" + mapId)
        .type(MediaType.TEXT_XML)
        .accept(MediaType.TEXT_XML)
        .post(
          Document.class,
          "<osmChange version=\"0.3\" generator=\"iD\">" +
            "<create/>" +
            "<modify>" +
              "<node id=\"" + nodeIdsArr[0] + "\" lon=\"" + originalBounds.getMinLon() + "\" " +
                "lat=\"" + originalBounds.getMinLat() + "\" version=\"1\" changeset=\"" +
                changesetId + "\">" +
                "<tag k=\"key 1\" v=\"val 1\"></tag>" +
                "<tag k=\"key 2\" v=\"val 2\"></tag>" +
              "</node>" +
              "<node id=\"-1\" lon=\"" + updateBounds.getMinLon() + "\" " +
                "lat=\"" + updateBounds.getMinLat() + "\" version=\"1\" changeset=\"" +
                changesetId + "\">" +
                "<tag k=\"key 3\" v=\"val 3\"></tag>" +
              "</node>" +
            "</modify>" +
            "<delete if-unused=\"true\"/>" +
          "</osmChange>");
    }
    catch (UniformInterfaceException e)
    {
      ClientResponse r = e.getResponse();
      Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
      Assert.assertTrue(r.getEntity(String.class).contains("Invalid OSM element ID"));

      OsmTestUtils.verifyTestDataUnmodified(
        originalBounds, changesetId, nodeIds, wayIds, relationIds);

      throw e;
    }
    catch (Exception e)
    {
      log.error(e.getMessage());
      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testUploadModifyNegativeWayId() throws Exception
  {
    //hootCoreServicesDatabaseWriterCompatibility must = false for this test to pass
    Properties hootProps = HootProperties.getInstance();
    hootProps.setProperty("hootCoreServicesDatabaseWriterCompatibility", "false");
    HootProperties.setProperties(hootProps);
    Assert.assertEquals(
      false,
      Boolean.parseBoolean(hootProps.getProperty("hootCoreServicesDatabaseWriterCompatibility")));

    final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
    final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
    final Set<Long> nodeIds =
      OsmTestUtils.createTestNodes(changesetId, originalBounds);
    final Long[] nodeIdsArr = nodeIds.toArray(new Long[]{});

    final Set<Long> wayIds =
      OsmTestUtils.createTestWays(changesetId, nodeIds);
    final Long[] wayIdsArr = wayIds.toArray(new Long[]{});

    final Set<Long> relationIds =
      OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);


    //Update the changeset with a way that has a negative ID, not allowed here since the property
    //was changed above.  A failure should occur and no data in the system should be modified.
    try
    {
      resource()
        .path("api/0.6/changeset/" + changesetId + "/upload")
        .queryParam("mapId", "" + mapId)
        .type(MediaType.TEXT_XML)
        .accept(MediaType.TEXT_XML)
        .post(
          Document.class,
          "<osmChange version=\"0.3\" generator=\"iD\">" +
            "<create/>" +
            "<modify>" +
              "<way id=\"" + wayIdsArr[0] + "\" version=\"1\" changeset=\"" +
                changesetId + "\">" +
                "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                "<nd ref=\"" + nodeIdsArr[4] + "\"></nd>" +
                "<tag k=\"key 1\" v=\"val 1\"></tag>" +
                "<tag k=\"key 2\" v=\"val 2\"></tag>" +
              "</way>" +
              "<way id=\"-1\" version=\"1\" changeset=\"" + changesetId + "\">" +
                "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                "<nd ref=\"" + nodeIdsArr[4] + "\"></nd>" +
                "<tag k=\"key 3\" v=\"val 3\"></tag>" +
              "</way>" +
            "</modify>" +
            "<delete if-unused=\"true\"/>" +
          "</osmChange>");
    }
    catch (UniformInterfaceException e)
    {
      ClientResponse r = e.getResponse();
      Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
      Assert.assertTrue(r.getEntity(String.class).contains("Invalid OSM element ID"));

      OsmTestUtils.verifyTestDataUnmodified(
        originalBounds, changesetId, nodeIds, wayIds, relationIds);

      throw e;
    }
    catch (Exception e)
    {
      log.error(e.getMessage());
      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testUploadModifyNegativeRelationId() throws Exception
  {
    //hootCoreServicesDatabaseWriterCompatibility must = false for this test to pass
    Properties hootProps = HootProperties.getInstance();
    hootProps.setProperty("hootCoreServicesDatabaseWriterCompatibility", "false");
    HootProperties.setProperties(hootProps);
    Assert.assertEquals(
      false,
      Boolean.parseBoolean(hootProps.getProperty("hootCoreServicesDatabaseWriterCompatibility")));

    final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
    final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
    final Set<Long> nodeIds =
      OsmTestUtils.createTestNodes(changesetId, originalBounds);
    final Long[] nodeIdsArr = nodeIds.toArray(new Long[]{});

    final Set<Long> wayIds =
      OsmTestUtils.createTestWays(changesetId, nodeIds);
    final Long[] wayIdsArr = wayIds.toArray(new Long[]{});

    final Set<Long> relationIds =
      OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
    final Long[] relationIdsArr = relationIds.toArray(new Long[]{});


    //Update the changeset with a relation that has a negative ID, not allowed here since the
    //property was changed above.  A failure should occur and no data in the system should be
    //modified.
    try
    {
      resource()
        .path("api/0.6/changeset/" + changesetId + "/upload")
        .queryParam("mapId", "" + mapId)
        .type(MediaType.TEXT_XML)
        .accept(MediaType.TEXT_XML)
        .post(
          Document.class,
          "<osmChange version=\"0.3\" generator=\"iD\">" +
            "<create/>" +
            "<modify>" +
              "<relation id=\"" + relationIdsArr[0] + "\" version=\"1\" changeset=\"" +
                changesetId + "\">" +
                "<member type=\"way\" role=\"role4\" ref=\"" + wayIdsArr[1] + "\"></member>" +
                "<member type=\"way\" role=\"role2\" ref=\"" + wayIdsArr[0] + "\"></member>" +
                "<member type=\"node\" ref=\"" + nodeIdsArr[2] + "\"></member>" +
                "<tag k=\"key 1\" v=\"val 1\"></tag>" +
                "<tag k=\"key 2\" v=\"val 2\"></tag>" +
              "</relation>" +
              "<relation id=\"-1\" version=\"1\" changeset=\"" + changesetId + "\">" +
                "<member type=\"way\" role=\"role4\" ref=\"" + wayIdsArr[1] + "\"></member>" +
                "<member type=\"way\" role=\"role2\" ref=\"" + wayIdsArr[0] + "\"></member>" +
                "<member type=\"node\" ref=\"" + nodeIdsArr[2] + "\"></member>" +
                "<tag k=\"key 3\" v=\"val 3\"></tag>" +
              "</relation>" +
            "</modify>" +
            "<delete if-unused=\"true\"/>" +
          "</osmChange>");
    }
    catch (UniformInterfaceException e)
    {
      ClientResponse r = e.getResponse();
      Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
      Assert.assertTrue(r.getEntity(String.class).contains("Invalid OSM element ID"));

      OsmTestUtils.verifyTestDataUnmodified(
        originalBounds, changesetId, nodeIds, wayIds, relationIds);

      throw e;
    }
    catch (Exception e)
    {
      log.error(e.getMessage());
      throw e;
    }
  }

  @Test
  @Category(UnitTest.class)
  public void testUploadModifyNegativeElementIdHootCoreCompatibilityMode() throws Exception
  {
    Properties hootProps = null;
    try
    {
      //Temporarily, (potentially) we allow elements with a negative ID to exist if a particular
      //compatibility mode is enabled, which allows for using the hoot --convert command as a
      //source of test data for the services.  This mode may go away in the future.
      hootProps = HootProperties.getInstance();
      hootProps.setProperty("hootCoreServicesDatabaseWriterCompatibility", "true");
      HootProperties.setProperties(hootProps);
      Assert.assertEquals(
        true,
        Boolean.parseBoolean(hootProps.getProperty("hootCoreServicesDatabaseWriterCompatibility")));

      final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
      final BoundingBox updateBounds =
        OsmTestUtils.createAfterModifiedTestChangesetBounds();
      final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);

      //link some nodes to the changeset
      Set<Long> nodeIds = new LinkedHashSet<Long>();
      //explicitly create a node with a negative ID to test the compatibility mode
      final long negativeNodeId = -1;
      Map<String, String> tags = new HashMap<String, String>();
      tags.put("key 3", "val 3");
      Node.insertNew(
        negativeNodeId, changesetId, mapId, originalBounds.getMaxLat(), originalBounds.getMaxLon(),
        tags, conn);
      tags.clear();
      CurrentNodes insertedNodeRecord =
      		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentNodesTbl)
      		.where(currentNodesTbl.id.eq(new Long(negativeNodeId)))
      		.singleResult(currentNodesTbl);
      Assert.assertNotNull(insertedNodeRecord);
      Assert.assertEquals(new Long(negativeNodeId), insertedNodeRecord.getId());
      nodeIds.add(negativeNodeId);

      tags.put("key 1", "val 1");
      tags.put("key 2", "val 2");
      nodeIds.add(
        Node.insertNew(
          changesetId, mapId, originalBounds.getMinLat(), originalBounds.getMinLon(), tags, conn));
      tags.clear();

      final Long[] nodeIdsArr = nodeIds.toArray(new Long[]{});

      //link some ways to the changeset
      Set<Long> wayIds = new LinkedHashSet<Long>();
      List<Long> wayNodeIds = new ArrayList<Long>();
      //explicitly create a way with a negative ID to test the compatibility mode
      final long negativeWayId = -1;
      wayNodeIds.add(nodeIdsArr[0]);
      wayNodeIds.add(nodeIdsArr[1]);
      tags.put("key 1", "val 1");
      tags.put("key 2", "val 2");
      Way.insertNew(negativeWayId, changesetId, mapId, wayNodeIds, tags, conn);
      tags.clear();
      CurrentNodes insertedWayRecord =
      		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentNodesTbl)
      		.where(currentNodesTbl.id.eq(new Long(negativeWayId)))
      		.singleResult(currentNodesTbl);
      Assert.assertNotNull(insertedWayRecord);
      Assert.assertEquals(new Long(negativeWayId), insertedWayRecord.getId());
      wayIds.add(negativeWayId);

      wayNodeIds.clear();
      wayNodeIds.add(nodeIdsArr[1]);
      wayNodeIds.add(nodeIdsArr[2]);
      wayIds.add(Way.insertNew(changesetId, mapId, wayNodeIds, null, conn));

      final Long[] wayIdsArr = wayIds.toArray(new Long[]{});

      //link some relations to the changeset
      Set<Long> relationIds = new LinkedHashSet<Long>();
      //explicitly create a relation with a negative ID to test the compatibility mode
      final long negativeRelationId = -1;
      List<RelationMember> members = new ArrayList<RelationMember>();
      members.add(new RelationMember(nodeIdsArr[0], ElementType.Node, "role1"));
      members.add(new RelationMember(wayIdsArr[0], ElementType.Way, "role1"));
      tags.put("key 1", "val 1");
      Relation.insertNew(negativeRelationId, changesetId, mapId, members, tags, conn);
      tags.clear();
      CurrentRelations insertedRelationRecord =
      		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentRelationsTbl)
      		.where(currentRelationsTbl.id.eq(new Long(negativeRelationId)))
      		.singleResult(currentRelationsTbl);
      Assert.assertNotNull(insertedRelationRecord);
      Assert.assertEquals(new Long(negativeRelationId), insertedRelationRecord.getId());
      relationIds.add(negativeRelationId);

      members.clear();
      members.add(new RelationMember(nodeIdsArr[1], ElementType.Node, "role1"));
      members.add(new RelationMember(wayIdsArr[1], ElementType.Way, "role1"));
      tags.put("key 2", "val 2");
      relationIds.add(Relation.insertNew(changesetId, mapId, members, tags, conn));
      tags.clear();

      final Long[] relationIdsArr = relationIds.toArray(new Long[]{});

      //Update the changeset via the service.  This update is valid, because the property was
      //changed above.
      Document responseData = null;
      try
      {
        responseData =
          resource()
            .path("api/0.6/changeset/" + changesetId + "/upload")
            .queryParam("mapId", "" + mapId)
            .type(MediaType.TEXT_XML)
            .accept(MediaType.TEXT_XML)
            .post(
              Document.class,
              "<osmChange version=\"0.3\" generator=\"iD\">" +
                "<create/>" +
                "<modify>" +
                  "<node id=\"" + nodeIdsArr[1] + "\" lon=\"" + updateBounds.getMinLon() + "\" " +
                    "lat=\"" + updateBounds.getMinLat() + "\" version=\"1\" changeset=\"" +
                    changesetId + "\">" +
                    "<tag k=\"key 1b\" v=\"val 1b\"></tag>" +
                    "<tag k=\"key 2b\" v=\"val 2b\"></tag>" +
                  "</node>" +
                  "<node id=\"" + negativeNodeId + "\" lon=\"" + originalBounds.getMaxLon() + "\" " +
                    "lat=\"" + updateBounds.getMinLat() + "\" version=\"1\" changeset=\"" +
                    changesetId + "\">" +
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
              "</osmChange>");
      }
      catch (UniformInterfaceException e)
      {
        ClientResponse r = e.getResponse();
        Assert.fail("Unexpected response " + r.getStatus() + " " + r.getEntity(String.class));
      }
      Assert.assertNotNull(responseData);

      XPath xpath = XmlUtils.createXPath();
      try
      {
        NodeList returnedNodes = XPathAPI.selectNodeList(responseData, "//osm/diffResult/node");
        Assert.assertEquals(2, returnedNodes.getLength());

        Assert.assertEquals(
          (long)nodeIdsArr[1],
          Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@old_id",
          responseData)));
        Assert.assertEquals(
          Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@old_id", responseData)),
          Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@new_id", responseData)));
        Assert.assertEquals(
          2, Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@new_version", responseData)));

        Assert.assertEquals(
          negativeNodeId,
          Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@old_id",
          responseData)));
        Assert.assertEquals(
          Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@old_id", responseData)),
          Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@new_id", responseData)));
        Assert.assertEquals(
          2, Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@new_version", responseData)));

        NodeList returnedWays = XPathAPI.selectNodeList(responseData, "//osm/diffResult/way");
        Assert.assertEquals(2, returnedWays.getLength());

        Assert.assertEquals(
          (long)wayIdsArr[1],
          Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@old_id",
          responseData)));
        Assert.assertEquals(
          Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@old_id", responseData)),
          Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@new_id", responseData)));
        Assert.assertEquals(
          2, Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@new_version", responseData)));

        Assert.assertEquals(
          negativeWayId,
          Long.parseLong(xpath.evaluate("//osm/diffResult/way[2]/@old_id",
          responseData)));
        Assert.assertEquals(
          Long.parseLong(xpath.evaluate("//osm/diffResult/way[2]/@old_id", responseData)),
          Long.parseLong(xpath.evaluate("//osm/diffResult/way[2]/@new_id", responseData)));
        Assert.assertEquals(
          2, Long.parseLong(xpath.evaluate("//osm/diffResult/way[2]/@new_version", responseData)));

        NodeList returnedRelations =
          XPathAPI.selectNodeList(responseData, "//osm/diffResult/relation");
        Assert.assertEquals(2, returnedRelations.getLength());

        Assert.assertEquals(
          (long)relationIdsArr[1],
          Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@old_id",
          responseData)));
        Assert.assertEquals(
          Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@old_id", responseData)),
          Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@new_id", responseData)));
        Assert.assertEquals(
          2, Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@new_version", responseData)));

        Assert.assertEquals(
          negativeRelationId,
          Long.parseLong(xpath.evaluate("//osm/diffResult/relation[2]/@old_id",
          responseData)));
        Assert.assertEquals(
          Long.parseLong(xpath.evaluate("//osm/diffResult/relation[2]/@old_id", responseData)),
          Long.parseLong(xpath.evaluate("//osm/diffResult/relation[2]/@new_id", responseData)));
        Assert.assertEquals(
          2, Long.parseLong(xpath.evaluate("//osm/diffResult/relation[2]/@new_version", responseData)));
      }
      catch (XPathExpressionException e)
      {
        Assert.fail("Error parsing response document: " + e.getMessage());
      }

      final Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
      QChangesets changesets = QChangesets.changesets;
      hoot.services.db2.Changesets changeset =
      new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(changesets)
      .where(changesets.id.eq(changesetId)).singleResult(changesets);
      try
      {
        final Map<Long, CurrentNodes> nodes =
        		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentNodesTbl)
        		.map(currentNodesTbl.id, currentNodesTbl);
        		;
        Assert.assertEquals(2, nodes.size());

        Assert.assertEquals(negativeNodeId, (long)nodeIdsArr[0]);
        CurrentNodes nodeRecord = nodes.get(nodeIdsArr[0]);
        Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
        Assert.assertEquals(
          new Double(updateBounds.getMinLat()),
          nodeRecord.getLatitude());
        Assert.assertEquals(
          new Double(originalBounds.getMaxLon()),
          nodeRecord.getLongitude());
        Assert.assertEquals(new Long(negativeNodeId), nodeRecord.getId());

        Assert.assertEquals(
          new Long(QuadTileCalculator.tileForPoint(
            nodeRecord.getLatitude(),
            nodeRecord.getLongitude())),
          nodeRecord.getTile());
        Assert.assertTrue(nodeRecord.getTimestamp().before(now));
        Assert.assertEquals(new Long(2), nodeRecord.getVersion());
        Assert.assertEquals(new Boolean(true), nodeRecord.getVisible());
        Map<String, String> parsedTags =
          PostgresUtils.postgresObjToHStore((PGobject)nodeRecord.getTags());
        Assert.assertNotNull(parsedTags);
        Assert.assertEquals(1, parsedTags.size());
        Assert.assertEquals("val 3b", parsedTags.get("key 3b"));

        nodeRecord = nodes.get(nodeIdsArr[1]);
        Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
        Assert.assertEquals(
          new Double(updateBounds.getMinLat()),
          nodeRecord.getLatitude());
        Assert.assertEquals(
          new Double(updateBounds.getMinLon()),
          nodeRecord.getLongitude());
        Assert.assertEquals(nodeIdsArr[1], nodeRecord.getId());
        Assert.assertEquals(
          new Long(QuadTileCalculator.tileForPoint(
            nodeRecord.getLatitude(),
            nodeRecord.getLongitude())),
          nodeRecord.getTile());
        Assert.assertTrue(nodeRecord.getTimestamp().before(now));
        Assert.assertEquals(new Long(2), nodeRecord.getVersion());
        Assert.assertEquals(new Boolean(true), nodeRecord.getVisible());
        parsedTags = PostgresUtils.postgresObjToHStore((PGobject)nodeRecord.getTags());
        Assert.assertNotNull(parsedTags);
        Assert.assertEquals(2, parsedTags.size());
        Assert.assertEquals("val 1b", parsedTags.get("key 1b"));
        Assert.assertEquals("val 2b", parsedTags.get("key 2b"));
      }
      catch (Exception e)
      {
        Assert.fail("Error checking nodes: " + e.getMessage());
      }

      try
      {
        final Map<Long, CurrentWays> ways =
        		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentWaysTbl)
        		.map(currentWaysTbl.id, currentWaysTbl);
        		;
        Assert.assertEquals(2, ways.size());

        //verify the updated ways
        Assert.assertEquals(negativeWayId, (long)wayIdsArr[0]);
        CurrentWays wayRecord = ways.get(wayIdsArr[0]);
        Assert.assertEquals(new Long(changesetId), wayRecord.getChangesetId());
        Assert.assertEquals(new Long(negativeWayId), wayRecord.getId());
        Assert.assertTrue(wayRecord.getTimestamp().before(now));
        Assert.assertEquals(new Long(2), wayRecord.getVersion());
        Assert.assertEquals(new Boolean(true), wayRecord.getVisible());
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
        Map<String, String> parsedTags = PostgresUtils.postgresObjToHStore((PGobject)wayRecord.getTags());
        Assert.assertNotNull(parsedTags);
        Assert.assertEquals(1, parsedTags.size());
        Assert.assertEquals("val 1", parsedTags.get("key 1"));

        wayRecord = ways.get(wayIdsArr[1]);
        Assert.assertEquals(new Long(changesetId), wayRecord.getChangesetId());
        Assert.assertEquals(wayIdsArr[1], wayRecord.getId());
        Assert.assertTrue(wayRecord.getTimestamp().before(now));
        Assert.assertEquals(new Long(2), wayRecord.getVersion());
        Assert.assertEquals(new Boolean(true), wayRecord.getVisible());
        wayNodes =
        		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentWayNodesTbl)
            .where(currentWayNodesTbl.wayId.eq(wayIdsArr[1]))
            .orderBy(currentWayNodesTbl.sequenceId.asc()).list(currentWayNodesTbl);
        Assert.assertEquals(2, wayNodes.size());
        wayNode = wayNodes.get(0);
        Assert.assertEquals(nodeIdsArr[1], wayNode.getNodeId());
        Assert.assertEquals(new Long(1), wayNode.getSequenceId());
        Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());
        wayNode = wayNodes.get(1);
        Assert.assertEquals(nodeIdsArr[2], wayNode.getNodeId());
        Assert.assertEquals(new Long(2), wayNode.getSequenceId());
        Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());
        parsedTags = PostgresUtils.postgresObjToHStore((PGobject)wayRecord.getTags());
        Assert.assertNotNull(parsedTags);
        Assert.assertEquals(2, parsedTags.size());
        Assert.assertEquals("val 1b", parsedTags.get("key 1b"));
        Assert.assertEquals("val 2b", parsedTags.get("key 2b"));
      }
      catch (Exception e)
      {
        Assert.fail("Error checking ways: " + e.getMessage());
      }

      try
      {
        final Map<Long, CurrentRelations> relations =
        		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentRelationsTbl)
        		.map(currentRelationsTbl.id, currentRelationsTbl);
        		;
        Assert.assertEquals(2, relations.size());

        Assert.assertEquals(negativeRelationId, (long)relationIdsArr[0]);
        CurrentRelations relationRecord =
          relations.get(relationIdsArr[0]);
        Assert.assertEquals(new Long(changesetId), relationRecord.getChangesetId());
        Assert.assertEquals(relationIdsArr[0], relationRecord.getId());
        Assert.assertTrue(relationRecord.getTimestamp().before(now));
        Assert.assertEquals(new Long(2), relationRecord.getVersion());
        Assert.assertEquals(new Boolean(true), relationRecord.getVisible());
        List<CurrentRelationMembers> memberRecords =

        		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentRelationMembersTbl)
            .where(currentRelationMembersTbl.relationId.eq(relationIdsArr[0]))
            .orderBy(currentRelationMembersTbl.sequenceId.asc()).list(currentRelationMembersTbl);
        Assert.assertEquals(2, members.size());
        CurrentRelationMembers member = memberRecords.get(0);
        Assert.assertEquals(
          relationRecord.getId(), member.getRelationId());
        Assert.assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
        Assert.assertEquals("role1", member.getMemberRole());
        Assert.assertEquals(
          new Integer(1), member.getSequenceId());

        Assert.assertEquals(nodeIdsArr[1], member.getMemberId());
        member = memberRecords.get(1);
        Assert.assertEquals(
          relationRecord.getId(), member.getRelationId());
        Assert.assertEquals(DbUtils.nwr_enum.way, member.getMemberType());
        Assert.assertEquals("role1", member.getMemberRole());
        Assert.assertEquals(
          new Integer(2), member.getSequenceId());

        Assert.assertEquals(wayIdsArr[1], member.getMemberId());
        Map<String, String> parsedTags = PostgresUtils.postgresObjToHStore((PGobject)relationRecord.getTags());
        Assert.assertNotNull(parsedTags);
        Assert.assertEquals(1, parsedTags.size());
        Assert.assertEquals("val 1", parsedTags.get("key 1"));

        relationRecord = relations.get(relationIdsArr[1]);
        Assert.assertEquals(new Long(changesetId), relationRecord.getChangesetId());
        Assert.assertEquals(relationIdsArr[1], relationRecord.getId());

        Assert.assertTrue(relationRecord.getTimestamp().before(now));
        Assert.assertEquals(new Long(2), relationRecord.getVersion());
        Assert.assertEquals(new Boolean(true), relationRecord.getVisible());
        memberRecords =
        		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentRelationMembersTbl)
            .where(currentRelationMembersTbl.relationId.eq(relationIdsArr[1]))
            .orderBy(currentRelationMembersTbl.sequenceId.asc()).list(currentRelationMembersTbl);
        Assert.assertEquals(2, members.size());
        member = memberRecords.get(0);
        Assert.assertEquals(
          relationRecord.getId(), member.getRelationId());
        Assert.assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
        Assert.assertEquals("role1", member.getMemberRole());
        Assert.assertEquals(
          new Integer(1), member.getSequenceId());

        Assert.assertEquals(nodeIdsArr[0], member.getMemberId());
        member = memberRecords.get(1);
        Assert.assertEquals(
          relationRecord.getId(), member.getRelationId());
        Assert.assertEquals(DbUtils.nwr_enum.way, member.getMemberType());
        Assert.assertEquals("role1", member.getMemberRole());
        Assert.assertEquals(
          new Integer(2), member.getSequenceId());

        Assert.assertEquals(wayIdsArr[0], member.getMemberId());
        parsedTags = PostgresUtils.postgresObjToHStore((PGobject)relationRecord.getTags());
        Assert.assertNotNull(parsedTags);
        Assert.assertEquals(1, parsedTags.size());
        Assert.assertEquals("val 2b", parsedTags.get("key 2b"));
      }
      catch (Exception e)
      {
        Assert.fail("Error checking relations: " + e.getMessage());
      }

      try
      {
      	changeset =
            new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(changesets)
            .where(changesets.id.eq(changesetId)).singleResult(changesets);

        Assert.assertNotNull(changeset);
        Assert.assertTrue(changeset.getCreatedAt().before(now));
        Assert.assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));
        Assert.assertEquals(new Integer(6), changeset.getNumChanges());
        Assert.assertEquals(new Long(userId), changeset.getUserId());

        BoundingBox expandedBounds = new BoundingBox(originalBounds);
        expandedBounds.expand(
          updateBounds,
          Double.parseDouble(HootProperties.getDefault("changesetBoundsExpansionFactorDeegrees")));
        hoot.services.models.osm.Changeset hootChangeset =
          new hoot.services.models.osm.Changeset(mapId, changesetId, conn);
        BoundingBox changesetBounds = hootChangeset.getBounds();
        Assert.assertTrue(changesetBounds.equals(expandedBounds));
      }
      catch (Exception e)
      {
        Assert.fail("Error checking changeset: " + e.getMessage());
      }
    }
    catch (Exception e)
    {
      log.error(e.getMessage());
    }
    finally
    {
      //set this back to default now that this test is over
      hootProps.setProperty("hootCoreServicesDatabaseWriterCompatibility", "false");
      HootProperties.setProperties(hootProps);
      Assert.assertEquals(
        false,
        Boolean.parseBoolean(hootProps.getProperty("hootCoreServicesDatabaseWriterCompatibility")));
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testUploadModifyNodeInvalidVersion() throws Exception
  {
    final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
    final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
    final Set<Long> nodeIds =
      OsmTestUtils.createTestNodes(changesetId, originalBounds);
    final Long[] nodeIdsArr = nodeIds.toArray(new Long[]{});

    final Set<Long> wayIds =
      OsmTestUtils.createTestWays(changesetId, nodeIds);

    final Set<Long> relationIds =
      OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);


    final BoundingBox updateBounds =
      OsmTestUtils.createAfterModifiedTestChangesetBounds();
    //Update the changeset where one of the nodes has version that doesn't match the version on
    //the server.  A failure should occur and no data in the system should be modified.
    try
    {
      resource()
        .path("api/0.6/changeset/" + changesetId + "/upload")
        .queryParam("mapId", "" + mapId)
        .type(MediaType.TEXT_XML)
        .accept(MediaType.TEXT_XML)
        .post(
          Document.class,
          "<osmChange version=\"0.3\" generator=\"iD\">" +
            "<create/>" +
            "<modify>" +
              "<node id=\"" + nodeIdsArr[0] + "\" lon=\"" + originalBounds.getMinLon() + "\" " +
                "lat=\"" + originalBounds.getMinLat() + "\" version=\"1\" changeset=\"" +
                changesetId + "\">" +
                "<tag k=\"key 1\" v=\"val 1\"></tag>" +
                "<tag k=\"key 2\" v=\"val 2\"></tag>" +
              "</node>" +
              "<node id=\"" + nodeIdsArr[1] + "\" lon=\"" + updateBounds.getMinLon() + "\" " +
                "lat=\"" + updateBounds.getMinLat() + "\" version=\"2\" changeset=\"" +
                changesetId + "\">" +
                "<tag k=\"key 3\" v=\"val 3\"></tag>" +
              "</node>" +
            "</modify>" +
            "<delete if-unused=\"true\"/>" +
          "</osmChange>");
    }
    catch (UniformInterfaceException e)
    {
      ClientResponse r = e.getResponse();
      Assert.assertEquals(Status.CONFLICT, Status.fromStatusCode(r.getStatus()));
      Assert.assertTrue(r.getEntity(String.class).contains("Invalid version"));

      //make sure neither node was updated
      OsmTestUtils.verifyTestDataUnmodified(
        originalBounds, changesetId, nodeIds, wayIds, relationIds);

      throw e;
    }
    catch (Exception e)
    {
      log.error(e.getMessage());
      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testUploadModifyNoMembersInRelation() throws Exception
  {
    final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
    final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
    final Set<Long> nodeIds =
      OsmTestUtils.createTestNodes(changesetId, originalBounds);

    final Set<Long> wayIds =
      OsmTestUtils.createTestWays(changesetId, nodeIds);

    final Set<Long> relationIds =
      OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
    final Long[] relationIdsArr = relationIds.toArray(new Long[]{});


    //A relation must have at least one member.  Try to upload a relation with no members.  The
    //request should fail and no data in the system should be modified.
    try
    {
      resource()
        .path("api/0.6/changeset/" + changesetId + "/upload")
        .queryParam("mapId", "" + mapId)
        .type(MediaType.TEXT_XML)
        .accept(MediaType.TEXT_XML)
        .post(
          Document.class,
          "<osmChange version=\"0.3\" generator=\"iD\">" +
            "<create/>" +
            "<modify>" +
              "<relation id=\"" + relationIdsArr[0] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
              "</relation>" +
            "</modify>" +
            "<delete if-unused=\"true\"/>" +
          "</osmChange>");
    }
    catch (UniformInterfaceException e)
    {
      ClientResponse r = e.getResponse();
      Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
      Assert.assertTrue(
        r.getEntity(String.class).contains("Too few members specified for relation"));

      OsmTestUtils.verifyTestDataUnmodified(
        originalBounds, changesetId, nodeIds, wayIds, relationIds);

      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testUploadModifyWayInvalidVersion() throws Exception
  {
    final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
    final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
    final Set<Long> nodeIds =
      OsmTestUtils.createTestNodes(changesetId, originalBounds);
    final Long[] nodeIdsArr = nodeIds.toArray(new Long[]{});
    final Set<Long> wayIds =
      OsmTestUtils.createTestWays(changesetId, nodeIds);
    final Long[] wayIdsArr = wayIds.toArray(new Long[]{});
    final Set<Long> relationIds =
      OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

    //Update the changeset where one of the nodes has a version greater than zero, which is what
    //the version should be for all new elements.  A failure should occur and no data in the
    //system should be modified.
    try
    {
      resource()
        .path("api/0.6/changeset/" + changesetId + "/upload")
        .queryParam("mapId", "" + mapId)
        .type(MediaType.TEXT_XML)
        .accept(MediaType.TEXT_XML)
        .post(
          Document.class,
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
    catch (UniformInterfaceException e)
    {
      ClientResponse r = e.getResponse();
      Assert.assertEquals(Status.CONFLICT, Status.fromStatusCode(r.getStatus()));
      Assert.assertTrue(r.getEntity(String.class).contains("Invalid version"));

      OsmTestUtils.verifyTestDataUnmodified(
        originalBounds, changesetId, nodeIds, wayIds, relationIds);

      throw e;
    }
    catch (Exception e)
    {
      log.error(e.getMessage());
      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testUploadModifyRelationInvalidVersion() throws Exception
  {
    final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
    final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
    final Set<Long> nodeIds =
      OsmTestUtils.createTestNodes(changesetId, originalBounds);
    final Long[] nodeIdsArr = nodeIds.toArray(new Long[]{});

    final Set<Long> wayIds =
      OsmTestUtils.createTestWays(changesetId, nodeIds);

    final Set<Long> relationIds =
      OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
    final Long[] relationIdsArr = relationIds.toArray(new Long[]{});


    //Update the changeset where one of the relations has an invalid version.  A failure should
    //occur and no data in the system should be modified.
    try
    {
      resource()
        .path("api/0.6/changeset/" + changesetId + "/upload")
        .queryParam("mapId", "" + mapId)
        .type(MediaType.TEXT_XML)
        .accept(MediaType.TEXT_XML)
        .post(
          Document.class,
          "<osmChange version=\"0.3\" generator=\"iD\">" +
            "<create/>" +
            "<modify>" +
              "<relation id=\"" + relationIdsArr[0] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
              "</relation>" +
              "<relation id=\"" + relationIdsArr[1] + "\" version=\"2\" changeset=\"" + changesetId + "\" >" +
                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
              "</relation>" +
            "</modify>" +
            "<delete if-unused=\"true\"/>" +
          "</osmChange>");
    }
    catch (UniformInterfaceException e)
    {
      ClientResponse r = e.getResponse();
      Assert.assertEquals(Status.CONFLICT, Status.fromStatusCode(r.getStatus()));
      Assert.assertTrue(r.getEntity(String.class).contains("Invalid version"));

      OsmTestUtils.verifyTestDataUnmodified(
        originalBounds, changesetId, nodeIds, wayIds, relationIds);

      throw e;
    }
    catch (Exception e)
    {
      log.error(e.getMessage());
      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testUploadModifyChangesetClosed() throws Exception
  {
    final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
    final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
    final Set<Long> nodeIds =
      OsmTestUtils.createTestNodes(changesetId, originalBounds);
    final Long[] nodeIdsArr = nodeIds.toArray(new Long[]{});

    final Set<Long> wayIds =
      OsmTestUtils.createTestWays(changesetId, nodeIds);

    final Set<Long> relationIds =
      OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);


    //close the changeset
    QChangesets changesets = QChangesets.changesets;
    hoot.services.db2.Changesets changeset =
    new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(changesets)
    .where(changesets.id.eq(changesetId)).singleResult(changesets);

    final Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
    changeset.setClosedAt(now);
    new SQLUpdateClause(conn, DbUtils.getConfiguration(mapId), changesets)
    .where(changesets.id.eq(changeset.getId()))
    .set(changesets.closedAt, now)
    .execute();

    changeset =
        new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(changesets)
        .where(changesets.id.eq(changesetId)).singleResult(changesets);
    Assert.assertTrue(changeset.getClosedAt().equals(now));

    final BoundingBox updateBounds =
      OsmTestUtils.createAfterModifiedTestChangesetBounds();
    //Try to update the closed changeset.  A failure should occur and no data in the system should
    //be modified.
    try
    {
      resource()
        .path("api/0.6/changeset/" + changesetId + "/upload")
        .queryParam("mapId", "" + mapId)
        .type(MediaType.TEXT_XML)
        .accept(MediaType.TEXT_XML)
        .post(
          Document.class,
          "<osmChange version=\"0.3\" generator=\"iD\">" +
            "<create/>" +
            "<modify>" +
              "<node id=\"" + nodeIdsArr[0] + "\" lon=\"" + updateBounds.getMinLon() + "\" " +
                "lat=\"" + updateBounds.getMinLat() + "\" version=\"1\" changeset=\"" +
                changesetId + "\">" +
                "<tag k=\"key 1\" v=\"val 1\"></tag>" +
                "<tag k=\"key 2\" v=\"val 2\"></tag>" +
              "</node>" +
            "</modify>" +
            "<delete if-unused=\"true\"/>" +
          "</osmChange>");
    }
    catch (UniformInterfaceException e)
    {
      ClientResponse r = e.getResponse();
      Assert.assertEquals(Status.CONFLICT, Status.fromStatusCode(r.getStatus()));
      Assert.assertTrue(
        r.getEntity(String.class).contains(
          "The changeset with ID: " + changesetId + " was closed at"));

      //make sure nothing was updated
      OsmTestUtils.verifyTestDataUnmodified(
        originalBounds, changesetId, nodeIds, wayIds, relationIds);
      OsmTestUtils.verifyTestChangesetClosed(changesetId);

      throw e;
    }
    catch (Exception e)
    {
      log.error(e.getMessage());
      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testUploadModifyMissingNodeTagValue() throws Exception
  {
    final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
    final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
    final Set<Long> nodeIds =
      OsmTestUtils.createTestNodes(changesetId, originalBounds);
    final Long[] nodeIdsArr = nodeIds.toArray(new Long[]{});

    final Set<Long> wayIds =
      OsmTestUtils.createTestWays(changesetId, nodeIds);

    final Set<Long> relationIds =
      OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);


    //Upload a changeset where one of the tags specified has no value attribute.  A failure
    //should occur and no data in the system should be modified.
    try
    {
      resource()
        .path("api/0.6/changeset/" + changesetId + "/upload")
        .queryParam("mapId", "" + mapId)
        .type(MediaType.TEXT_XML)
        .accept(MediaType.TEXT_XML)
        .post(
          Document.class,
          "<osmChange version=\"0.3\" generator=\"iD\">" +
            "<modify>" +
              "<node id=\"" + nodeIdsArr[0] + "\" lon=\"" + originalBounds.getMinLon() +
                "\" lat=\"" + originalBounds.getMinLat() + "\" version=\"1\" changeset=\"" +
                changesetId + "\">" +
                "<tag k=\"name 1\"/>" +  //missing tag value
                "<tag k=\"name 2\" v=\"val 2\"/>" +
              "</node>" +
              "<node id=\"" + nodeIdsArr[1] + "\" lon=\"" + originalBounds.getMinLon() +
                "\" lat=\"" + originalBounds.getMinLat() + "\" version=\"1\" changeset=\"" +
                changesetId + "\">" +
                "<tag k=\"name 1\" v=\"val 1\"/>" +
              "</node>" +
            "</modify>" +
            "<create/>" +
            "<delete if-unused=\"true\"/>" +
          "</osmChange>");
    }
    catch (UniformInterfaceException e)
    {
      ClientResponse r = e.getResponse();
      Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
      Assert.assertTrue(r.getEntity(String.class).contains("Error parsing tag"));

      OsmTestUtils.verifyTestDataUnmodified(
        originalBounds, changesetId, nodeIds, wayIds, relationIds);

      throw e;
    }
    catch (Exception e)
    {
      log.error(e.getMessage());
      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testUploadModifyTooFewNodesForWay() throws Exception
  {
    final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
    final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
    final Set<Long> nodeIds =
      OsmTestUtils.createTestNodes(changesetId, originalBounds);
    final Long[] nodeIdsArr = nodeIds.toArray(new Long[]{});

    final Set<Long> wayIds =
      OsmTestUtils.createTestWays(changesetId, nodeIds);
    final Long[] wayIdsArr = wayIds.toArray(new Long[]{});

    final Set<Long> relationIds =
      OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);


    //A way has to have two or more nodes.  Try to upload a way with one node.  The request should
    //fail and no data in the system should be modified.
    try
    {
      resource()
        .path("api/0.6/changeset/" + changesetId + "/upload")
        .queryParam("mapId", "" + mapId)
        .type(MediaType.TEXT_XML)
        .accept(MediaType.TEXT_XML)
        .post(
          Document.class,
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
    catch (UniformInterfaceException e)
    {
      ClientResponse r = e.getResponse();
      Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
      Assert.assertTrue(r.getEntity(String.class).contains("Too few nodes specified for way"));

      OsmTestUtils.verifyTestDataUnmodified(
        originalBounds, changesetId, nodeIds, wayIds, relationIds);

      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testUploadModifyTooManyNodesForWay() throws Exception
  {
    final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
    final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
    final Set<Long> nodeIds =
      OsmTestUtils.createTestNodes(changesetId, originalBounds);
    final Long[] nodeIdsArr = nodeIds.toArray(new Long[]{});

    final Set<Long> wayIds =
      OsmTestUtils.createTestWays(changesetId, nodeIds);
    final Long[] wayIdsArr = wayIds.toArray(new Long[]{});

    final Set<Long> relationIds =
      OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);


    //use a lower number of max way nodes then default for efficiency
    Properties hootProps = HootProperties.getInstance();
    hootProps.setProperty("maximumWayNodes", "2");
    HootProperties.setProperties(hootProps);
    final long maxWayNodes = 2;
    Assert.assertEquals(
      maxWayNodes,
      Long.parseLong(HootProperties.getInstance().getProperty("maximumWayNodes")));

    //A way has to have two or more nodes.  Try to upload a way more nodes than is allowed by the
    //system.  The request should fail and no data in the system should be modified.
    try
    {
      resource()
        .path("api/0.6/changeset/" + changesetId + "/upload")
        .queryParam("mapId", "" + mapId)
        .type(MediaType.TEXT_XML)
        .accept(MediaType.TEXT_XML)
        .post(
          Document.class,
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
    catch (UniformInterfaceException e)
    {
      ClientResponse r = e.getResponse();
      Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
      Assert.assertTrue(r.getEntity(String.class).contains("Too many nodes specified for way"));

      OsmTestUtils.verifyTestDataUnmodified(
        originalBounds, changesetId, nodeIds, wayIds, relationIds);

      throw e;
    }
    finally
    {
      //set this back to default now that this test is over
      HootProperties.getInstance().setProperty(
        "maximumWayNodes", HootProperties.getDefault("maximumWayNodes"));
      Assert.assertEquals(
        Long.parseLong(HootProperties.getDefault("maximumWayNodes")),
        Long.parseLong(HootProperties.getInstance().getProperty("maximumWayNodes")));
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testUploadModifyWayWithNonExistentNode() throws Exception
  {
    final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
    final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
    final Set<Long> nodeIds =
      OsmTestUtils.createTestNodes(changesetId, originalBounds);
    final Long[] nodeIdsArr = nodeIds.toArray(new Long[]{});

    final Set<Long> wayIds =
      OsmTestUtils.createTestWays(changesetId, nodeIds);
    final Long[] wayIdsArr = wayIds.toArray(new Long[]{});

    final Set<Long> relationIds =
      OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);


    //Try to create a way referencing a node that doesn't exist.  The request should fail and no
    //data in the system should be modified.
    try
    {
      resource()
        .path("api/0.6/changeset/" + changesetId + "/upload")
        .queryParam("mapId", "" + mapId)
        .type(MediaType.TEXT_XML)
        .accept(MediaType.TEXT_XML)
        .post(
          Document.class,
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
    catch (UniformInterfaceException e)
    {
      ClientResponse r = e.getResponse();
      Assert.assertEquals(Status.PRECONDITION_FAILED, Status.fromStatusCode(r.getStatus()));
      Assert.assertTrue(r.getEntity(String.class).contains("does not exist for way"));

      OsmTestUtils.verifyTestDataUnmodified(
        originalBounds, changesetId, nodeIds, wayIds, relationIds);

      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testUploadModifyRelationWithNonExistentNode() throws Exception
  {
    final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
    final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
    final Set<Long> nodeIds =
      OsmTestUtils.createTestNodes(changesetId, originalBounds);
    final Long[] nodeIdsArr = nodeIds.toArray(new Long[]{});

    final Set<Long> wayIds =
      OsmTestUtils.createTestWays(changesetId, nodeIds);

    final Set<Long> relationIds =
      OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
    final Long[] relationIdsArr = relationIds.toArray(new Long[]{});


    //Try to update a relation referencing a node that doesn't exist.  The request should fail and
    //no data in the system should be modified.
    try
    {
      resource()
        .path("api/0.6/changeset/" + changesetId + "/upload")
        .queryParam("mapId", "" + mapId)
        .type(MediaType.TEXT_XML)
        .accept(MediaType.TEXT_XML)
        .post(
          Document.class,
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
    catch (UniformInterfaceException e)
    {
      ClientResponse r = e.getResponse();
      Assert.assertEquals(Status.PRECONDITION_FAILED, Status.fromStatusCode(r.getStatus()));
      Assert.assertTrue(r.getEntity(String.class).contains("does not exist for relation"));

      OsmTestUtils.verifyTestDataUnmodified(
        originalBounds, changesetId, nodeIds, wayIds, relationIds);

      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testUploadModifyRelationWithNonExistentWay() throws Exception
  {
    final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
    final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
    final Set<Long> nodeIds =
      OsmTestUtils.createTestNodes(changesetId, originalBounds);
    final Long[] nodeIdsArr = nodeIds.toArray(new Long[]{});

    final Set<Long> wayIds =
      OsmTestUtils.createTestWays(changesetId, nodeIds);
    final Long[] wayIdsArr = wayIds.toArray(new Long[]{});

    final Set<Long> relationIds =
      OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
    final Long[] relationIdsArr = relationIds.toArray(new Long[]{});


    //Try to update a relation referencing a way that doesn't exist.  The request should fail and
    //no data in the system should be modified.
    try
    {
      resource()
        .path("api/0.6/changeset/" + changesetId + "/upload")
        .queryParam("mapId", "" + mapId)
        .type(MediaType.TEXT_XML)
        .accept(MediaType.TEXT_XML)
        .post(
          Document.class,
          "<osmChange version=\"0.3\" generator=\"iD\">" +
            "<create/>" +
            "<modify>" +
              "<relation id=\"" + relationIdsArr[0] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                "<member type=\"way\" role=\"role1\" ref=\"" + (wayIdsArr[2] + 1) + "\"></member>" +
                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
              "</relation>" +
            "</modify>" +
            "<delete if-unused=\"true\"/>" +
          "</osmChange>");
    }
    catch (UniformInterfaceException e)
    {
      ClientResponse r = e.getResponse();
      Assert.assertEquals(Status.PRECONDITION_FAILED, Status.fromStatusCode(r.getStatus()));
      Assert.assertTrue(r.getEntity(String.class).contains("does not exist for relation"));

      OsmTestUtils.verifyTestDataUnmodified(
        originalBounds, changesetId, nodeIds, wayIds, relationIds);

      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testUploadModifyRelationWithNonExistentRelation() throws Exception
  {
    final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
    final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
    final Set<Long> nodeIds =
      OsmTestUtils.createTestNodes(changesetId, originalBounds);
    final Long[] nodeIdsArr = nodeIds.toArray(new Long[]{});

    final Set<Long> wayIds =
      OsmTestUtils.createTestWays(changesetId, nodeIds);

    final Set<Long> relationIds =
      OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
    final Long[] relationIdsArr = relationIds.toArray(new Long[]{});


    //Try to create a relation referencing a relation that doesn't exist.  The request should fail
    //and no data in the system should be modified.
    try
    {
      resource()
        .path("api/0.6/changeset/" + changesetId + "/upload")
        .queryParam("mapId", "" + mapId)
        .type(MediaType.TEXT_XML)
        .accept(MediaType.TEXT_XML)
        .post(
          Document.class,
          "<osmChange version=\"0.3\" generator=\"iD\">" +
            "<create/>" +
            "<modify>" +
              "<relation id=\"" + relationIdsArr[0] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                "<member type=\"relation\" role=\"role1\" ref=\"" + (relationIdsArr[3] + 1) + "\"></member>" +
                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
              "</relation>" +
            "</modify>" +
            "<delete if-unused=\"true\"/>" +
          "</osmChange>");
    }
    catch (UniformInterfaceException e)
    {
      ClientResponse r = e.getResponse();
      Assert.assertEquals(Status.PRECONDITION_FAILED, Status.fromStatusCode(r.getStatus()));
      Assert.assertTrue(r.getEntity(String.class).contains("does not exist for relation"));

      OsmTestUtils.verifyTestDataUnmodified(
        originalBounds, changesetId, nodeIds, wayIds, relationIds);

      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testUploadModifyWayWithInvisibleNode() throws Exception
  {
    final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
    final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
    final Set<Long> nodeIds =
      OsmTestUtils.createTestNodes(changesetId, originalBounds);
    final Long[] nodeIdsArr = nodeIds.toArray(new Long[]{});

    final Set<Long> wayIds =
      OsmTestUtils.createTestWays(changesetId, nodeIds);
    final Long[] wayIdsArr = wayIds.toArray(new Long[]{});

    final Set<Long> relationIds =
      OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);


    //make one of way nodes invisible
    CurrentNodes invisibleNode =
    		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentNodesTbl)
    		.where(currentNodesTbl.id.eq(nodeIdsArr[0]))
    		.singleResult(currentNodesTbl);
    invisibleNode.setVisible(false);
    int success = 
    		(int) new SQLUpdateClause(conn, DbUtils.getConfiguration(mapId), currentNodesTbl)
    .where(currentNodesTbl.id.eq(invisibleNode.getId()))
  .set(currentNodesTbl.visible, false)
  .execute();

    Assert.assertEquals(1, success);
    Assert.assertEquals(
      false,
      new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentNodesTbl)
  		.where(currentNodesTbl.id.eq(nodeIdsArr[0]))
  		.singleResult(currentNodesTbl.visible)
    		);

    //Try to upload a way which references an invisible node.  The request should fail and no
    //data in the system should be modified.
    try
    {
      resource()
        .path("api/0.6/changeset/" + changesetId + "/upload")
        .queryParam("mapId", "" + mapId)
        .type(MediaType.TEXT_XML)
        .accept(MediaType.TEXT_XML)
        .post(
          Document.class,
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
    catch (UniformInterfaceException e)
    {
      ClientResponse r = e.getResponse();
      Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
      Assert.assertTrue(r.getEntity(String.class).contains("is not visible for way"));

      OsmTestUtils.verifyTestDataUnmodified(
        originalBounds, changesetId, nodeIds, wayIds, relationIds);

      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testUploadModifyRelationWithInvisibleNode() throws Exception
  {
    final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
    final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
    final Set<Long> nodeIds =
      OsmTestUtils.createTestNodes(changesetId, originalBounds);
    final Long[] nodeIdsArr = nodeIds.toArray(new Long[]{});

    final Set<Long> wayIds =
      OsmTestUtils.createTestWays(changesetId, nodeIds);

    final Set<Long> relationIds =
      OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
    final Long[] relationIdsArr = relationIds.toArray(new Long[]{});


    //make one of relation node members invisible
    CurrentNodes invisibleNode =
    		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentNodesTbl)
    		.where(currentNodesTbl.id.eq(nodeIdsArr[0]))
    		.singleResult(currentNodesTbl);
    invisibleNode.setVisible(false);
    int success = 
    		(int) new SQLUpdateClause(conn, DbUtils.getConfiguration(mapId), currentNodesTbl)
    .where(currentNodesTbl.id.eq(invisibleNode.getId()))
  .set(currentNodesTbl.visible, false)
  .execute();
    Assert.assertEquals(1, success);
    Assert.assertEquals(
      false,
      new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentNodesTbl)
  		.where(currentNodesTbl.id.eq(nodeIdsArr[0]))
  		.singleResult(currentNodesTbl.visible)
    		);

    //Try to upload a relation which references an invisible node.  The request should fail and no
    //data in the system should be modified.
    try
    {
      resource()
        .path("api/0.6/changeset/" + changesetId + "/upload")
        .queryParam("mapId", "" + mapId)
        .type(MediaType.TEXT_XML)
        .accept(MediaType.TEXT_XML)
        .post(
          Document.class,
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
    catch (UniformInterfaceException e)
    {
      ClientResponse r = e.getResponse();
      Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
      Assert.assertTrue(r.getEntity(String.class).contains("is not visible for relation"));

      OsmTestUtils.verifyTestDataUnmodified(
        originalBounds, changesetId, nodeIds, wayIds, relationIds);

      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testUploadModifyRelationWithInvisibleWay() throws Exception
  {
    final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
    final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
    final Set<Long> nodeIds =
      OsmTestUtils.createTestNodes(changesetId, originalBounds);
    final Long[] nodeIdsArr = nodeIds.toArray(new Long[]{});

    final Set<Long> wayIds =
      OsmTestUtils.createTestWays(changesetId, nodeIds);
    final Long[] wayIdsArr = wayIds.toArray(new Long[]{});

    final Set<Long> relationIds =
      OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
    final Long[] relationIdsArr = relationIds.toArray(new Long[]{});


    //make one of relation way members invisible
    CurrentWays invisibleWay =

    		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentWaysTbl)
    		.where(currentWaysTbl.id.eq(wayIdsArr[0]))
    		.singleResult(currentWaysTbl);
    invisibleWay.setVisible(false);
    int success =
    		(int) new SQLUpdateClause(conn, DbUtils.getConfiguration(mapId), currentWaysTbl)
    .where(currentWaysTbl.id.eq(invisibleWay.getId()))
  .set(currentWaysTbl.visible, false)
  .execute();

    Assert.assertEquals(1, success);
    Assert.assertEquals(
      false,

      new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentWaysTbl)
  		.where(currentWaysTbl.id.eq(wayIdsArr[0]))
  		.singleResult(currentWaysTbl.visible)

    		);

    //Try to upload a relation which references an invisible way.  The request should fail and no
    //data in the system should be modified.
    try
    {
      resource()
        .path("api/0.6/changeset/" + changesetId + "/upload")
        .queryParam("mapId", "" + mapId)
        .type(MediaType.TEXT_XML)
        .accept(MediaType.TEXT_XML)
        .post(
          Document.class,
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
    catch (UniformInterfaceException e)
    {
      ClientResponse r = e.getResponse();
      Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
      Assert.assertTrue(r.getEntity(String.class).contains("is not visible for relation"));

      OsmTestUtils.verifyTestDataUnmodified(
        originalBounds, changesetId, nodeIds, wayIds, relationIds);

      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testUploadModifyRelationWithInvisibleRelation() throws Exception
  {
    final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
    final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
    final Set<Long> nodeIds =
      OsmTestUtils.createTestNodes(changesetId, originalBounds);
    final Long[] nodeIdsArr = nodeIds.toArray(new Long[]{});

    final Set<Long> wayIds =
      OsmTestUtils.createTestWays(changesetId, nodeIds);
    final Long[] wayIdsArr = wayIds.toArray(new Long[]{});

    final Set<Long> relationIds =
      OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
    final Long[] relationIdsArr = relationIds.toArray(new Long[]{});


    //make one of relation's members invisible
    CurrentRelations invisibleRelation =
    		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentRelationsTbl)
    		.where(currentRelationsTbl.id.eq(relationIdsArr[0]))
    		.singleResult(currentRelationsTbl);
    invisibleRelation.setVisible(false);
    int success = 
    		(int) new SQLUpdateClause(conn, DbUtils.getConfiguration(mapId), currentRelationsTbl)
    .where(currentRelationsTbl.id.eq(invisibleRelation.getId()))
  .set(currentRelationsTbl.visible, false)
  .execute();
    Assert.assertEquals(1, success);
    Assert.assertEquals(
      false,
      new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentRelationsTbl)
  		.where(currentRelationsTbl.id.eq(relationIdsArr[0]))
  		.singleResult(currentRelationsTbl.visible)
    		);

    //Try to upload a relation which references an invisible relation.  The request should fail
    //and no data in the system should be modified.
    try
    {
      resource()
        .path("api/0.6/changeset/" + changesetId + "/upload")
        .queryParam("mapId", "" + mapId)
        .type(MediaType.TEXT_XML)
        .accept(MediaType.TEXT_XML)
        .post(
          Document.class,
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
    catch (UniformInterfaceException e)
    {
      ClientResponse r = e.getResponse();
      Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
      Assert.assertTrue(r.getEntity(String.class).contains("is not visible for relation"));

      OsmTestUtils.verifyTestDataUnmodified(
        originalBounds, changesetId, nodeIds, wayIds, relationIds);

      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testUploadModifyDuplicateWayIds() throws Exception
  {
    final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
    final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
    final Set<Long> nodeIds =
      OsmTestUtils.createTestNodes(changesetId, originalBounds);
    final Long[] nodeIdsArr = nodeIds.toArray(new Long[]{});

    final Set<Long> wayIds =
      OsmTestUtils.createTestWays(changesetId, nodeIds);
    final Long[] wayIdsArr = wayIds.toArray(new Long[]{});

    final Set<Long> relationIds =
      OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);


    //Try to upload two ways with the same ID.  The request should fail and no
    //data in the system should be modified.
    try
    {
      resource()
        .path("api/0.6/changeset/" + changesetId + "/upload")
        .queryParam("mapId", "" + mapId)
        .type(MediaType.TEXT_XML)
        .accept(MediaType.TEXT_XML)
        .post(
          Document.class,
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
    catch (UniformInterfaceException e)
    {
      ClientResponse r = e.getResponse();
      Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
      Assert.assertTrue(r.getEntity(String.class).contains("Duplicate OSM element ID"));

      OsmTestUtils.verifyTestDataUnmodified(
        originalBounds, changesetId, nodeIds, wayIds, relationIds);

      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testUploadModifyInvalidChangesetIdInWay() throws Exception
  {
    final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
    final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
    final Set<Long> nodeIds =
      OsmTestUtils.createTestNodes(changesetId, originalBounds);
    final Long[] nodeIdsArr = nodeIds.toArray(new Long[]{});

    final Set<Long> wayIds =
      OsmTestUtils.createTestWays(changesetId, nodeIds);
    final Long[] wayIdsArr = wayIds.toArray(new Long[]{});

    final Set<Long> relationIds =
      OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);


    //Upload a changeset where one of the elements in the changeset lists a changeset ID that
    //doesn't match the changeset ID specified in the URL.  A failure should occur and no data
    //in the system should be modified.
    try
    {
      resource()
        .path("api/0.6/changeset/" + changesetId + "/upload")
        .queryParam("mapId", "" + mapId)
        .type(MediaType.TEXT_XML)
        .accept(MediaType.TEXT_XML)
        .post(
          Document.class,
          "<osmChange version=\"0.3\" generator=\"iD\">" +
            "<create/>" +
            "<modify>" +
              "<way id=\"" + wayIdsArr[0] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
              "</way>" +
              "<way id=\"" + wayIdsArr[1] + "\" version=\"1\" changeset=\"" + (changesetId + 1) + "\" >" +
                "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                "<nd ref=\"" + nodeIdsArr[2] + "\"></nd>" +
              "</way>" +
            "</modify>" +
            "<delete if-unused=\"true\"/>" +
          "</osmChange>");
    }
    catch (UniformInterfaceException e)
    {
      ClientResponse r = e.getResponse();
      Assert.assertEquals(Status.CONFLICT, Status.fromStatusCode(r.getStatus()));
      Assert.assertTrue(r.getEntity(String.class).contains("Invalid changeset ID"));

      OsmTestUtils.verifyTestDataUnmodified(
        originalBounds, changesetId, nodeIds, wayIds, relationIds);

      throw e;
    }
    catch (Exception e)
    {
      log.error(e.getMessage());
      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testUploadModifyInvalidChangesetIdInRelation() throws Exception
  {
    final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
    final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
    final Set<Long> nodeIds =
      OsmTestUtils.createTestNodes(changesetId, originalBounds);
    final Long[] nodeIdsArr = nodeIds.toArray(new Long[]{});

    final Set<Long> wayIds =
      OsmTestUtils.createTestWays(changesetId, nodeIds);
    final Long[] wayIdsArr = wayIds.toArray(new Long[]{});

    final Set<Long> relationIds =
      OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
    final Long[] relationIdsArr = relationIds.toArray(new Long[]{});


    //Upload a changeset where one of the elements in the changeset lists a changeset ID that
    //doesn't match the changeset ID specified in the URL.  A failure should occur and no data
    //in the system should be modified.
    try
    {
      resource()
        .path("api/0.6/changeset/" + changesetId + "/upload")
        .queryParam("mapId", "" + mapId)
        .type(MediaType.TEXT_XML)
        .accept(MediaType.TEXT_XML)
        .post(
          Document.class,
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
    catch (UniformInterfaceException e)
    {
      ClientResponse r = e.getResponse();
      Assert.assertEquals(Status.CONFLICT, Status.fromStatusCode(r.getStatus()));
      Assert.assertTrue(r.getEntity(String.class).contains("Invalid changeset ID"));

      OsmTestUtils.verifyTestDataUnmodified(
        originalBounds, changesetId, nodeIds, wayIds, relationIds);

      throw e;
    }
    catch (Exception e)
    {
      log.error(e.getMessage());
      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testUploadModifyMissingWayTagValue() throws Exception
  {
    final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
    final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
    final Set<Long> nodeIds =
      OsmTestUtils.createTestNodes(changesetId, originalBounds);
    final Long[] nodeIdsArr = nodeIds.toArray(new Long[]{});

    final Set<Long> wayIds =
      OsmTestUtils.createTestWays(changesetId, nodeIds);
    final Long[] wayIdsArr = wayIds.toArray(new Long[]{});

    final Set<Long> relationIds =
      OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);


    //Upload a changeset where one of the tags specified has no value attribute.  A failure
    //should occur and no data in the system should be modified.
    try
    {
      resource()
        .path("api/0.6/changeset/" + changesetId + "/upload")
        .queryParam("mapId", "" + mapId)
        .type(MediaType.TEXT_XML)
        .accept(MediaType.TEXT_XML)
        .post(
          Document.class,
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
                "<tag k=\"name 1\"/>" +  //missing tag value
              "</way>" +
            "</modify>" +
            "<delete if-unused=\"true\"/>" +
          "</osmChange>");
    }
    catch (UniformInterfaceException e)
    {
      ClientResponse r = e.getResponse();
      Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
      Assert.assertTrue(r.getEntity(String.class).contains("Error parsing tag"));

      OsmTestUtils.verifyTestDataUnmodified(
        originalBounds, changesetId, nodeIds, wayIds, relationIds);

      throw e;
    }
    catch (Exception e)
    {
      log.error(e.getMessage());
      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testUploadModifyMissingRelationTagValue() throws Exception
  {
    final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
    final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
    final Set<Long> nodeIds =
      OsmTestUtils.createTestNodes(changesetId, originalBounds);
    final Long[] nodeIdsArr = nodeIds.toArray(new Long[]{});

    final Set<Long> wayIds =
      OsmTestUtils.createTestWays(changesetId, nodeIds);

    final Set<Long> relationIds =
      OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
    final Long[] relationIdsArr = relationIds.toArray(new Long[]{});


    //Upload a changeset where one of the element tags specified has no value attribute.  A failure
    //should occur and no data in the system should be modified.
    try
    {
      resource()
        .path("api/0.6/changeset/" + changesetId + "/upload")
        .queryParam("mapId", "" + mapId)
        .type(MediaType.TEXT_XML)
        .accept(MediaType.TEXT_XML)
        .post(
          Document.class,
          "<osmChange version=\"0.3\" generator=\"iD\">" +
            "<create/>" +
            "<modify>" +
              "<relation id=\"" + relationIdsArr[0] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
              "</relation>" +
              "<relation id=\"" + relationIdsArr[1] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                "<tag k=\"name 1\"/>" +  //missing tag value
              "</relation>" +
            "</modify>" +
            "<delete if-unused=\"true\"/>" +
          "</osmChange>");
    }
    catch (UniformInterfaceException e)
    {
      ClientResponse r = e.getResponse();
      Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
      Assert.assertTrue(r.getEntity(String.class).contains("Error parsing tag"));

      OsmTestUtils.verifyTestDataUnmodified(
        originalBounds, changesetId, nodeIds, wayIds, relationIds);

      throw e;
    }
    catch (Exception e)
    {
      log.error(e.getMessage());
      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testUploadModifyDuplicateWayNodeIds() throws Exception
  {
    final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
    final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
    final Set<Long> nodeIds =
      OsmTestUtils.createTestNodes(changesetId, originalBounds);
    final Long[] nodeIdsArr = nodeIds.toArray(new Long[]{});

    final Set<Long> wayIds =
      OsmTestUtils.createTestWays(changesetId, nodeIds);
    final Long[] wayIdsArr = wayIds.toArray(new Long[]{});

    final Set<Long> relationIds =
      OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);


    //Try to upload a way which lists the same node twice.  The request should fail and no
    //data in the system should be modified.
    try
    {
      resource()
        .path("api/0.6/changeset/" + changesetId + "/upload")
        .queryParam("mapId", "" + mapId)
        .type(MediaType.TEXT_XML)
        .accept(MediaType.TEXT_XML)
        .post(
          Document.class,
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
    catch (UniformInterfaceException e)
    {
      ClientResponse r = e.getResponse();
      Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
      Assert.assertTrue(r.getEntity(String.class).contains("Duplicate OSM element ID"));

      OsmTestUtils.verifyTestDataUnmodified(
        originalBounds, changesetId, nodeIds, wayIds, relationIds);

      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testUploadModifyWayEmptyNodeId() throws Exception
  {
    final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
    final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
    final Set<Long> nodeIds =
      OsmTestUtils.createTestNodes(changesetId, originalBounds);
    final Long[] nodeIdsArr = nodeIds.toArray(new Long[]{});

    final Set<Long> wayIds =
      OsmTestUtils.createTestWays(changesetId, nodeIds);
    final Long[] wayIdsArr = wayIds.toArray(new Long[]{});

    final Set<Long> relationIds =
      OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);


    //Update the changeset where one of the ways has a node with an empty string for its ID.  A
    //failure should occur and no data in the system should be modified.
    try
    {
      resource()
        .path("api/0.6/changeset/" + changesetId + "/upload")
        .queryParam("mapId", "" + mapId)
        .type(MediaType.TEXT_XML)
        .accept(MediaType.TEXT_XML)
        .post(
          Document.class,
          "<osmChange version=\"0.3\" generator=\"iD\">" +
            "<create/>" +
            "<modify>" +
              "<way id=\"" + wayIdsArr[0] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
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
    catch (UniformInterfaceException e)
    {
      ClientResponse r = e.getResponse();
      Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
      //TODO: needed?
      //Assert.assertTrue(r.getEntity(String.class).contains("Invalid version"));

      OsmTestUtils.verifyTestDataUnmodified(
        originalBounds, changesetId, nodeIds, wayIds, relationIds);

      throw e;
    }
    catch (Exception e)
    {
      log.error(e.getMessage());
      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testUploadModifyWayMissingNodeId() throws Exception
  {
    final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
    final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
    final Set<Long> nodeIds =
      OsmTestUtils.createTestNodes(changesetId, originalBounds);
    final Long[] nodeIdsArr = nodeIds.toArray(new Long[]{});

    final Set<Long> wayIds =
      OsmTestUtils.createTestWays(changesetId, nodeIds);
    final Long[] wayIdsArr = wayIds.toArray(new Long[]{});

    final Set<Long> relationIds =
      OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);


    //Update the changeset where one of the ways has a node with no ID attribute.  A
    //failure should occur and no data in the system should be modified.
    try
    {
      resource()
        .path("api/0.6/changeset/" + changesetId + "/upload")
        .queryParam("mapId", "" + mapId)
        .type(MediaType.TEXT_XML)
        .accept(MediaType.TEXT_XML)
        .post(
          Document.class,
          "<osmChange version=\"0.3\" generator=\"iD\">" +
            "<create/>" +
            "<modify>" +
              "<way id=\"" + wayIdsArr[0] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                "<nd></nd>" + //missing node ID
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
    catch (UniformInterfaceException e)
    {
      ClientResponse r = e.getResponse();
      Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
      //TODO: needed?
      //Assert.assertTrue(r.getEntity(String.class).contains("Invalid version"));

      OsmTestUtils.verifyTestDataUnmodified(
        originalBounds, changesetId, nodeIds, wayIds, relationIds);

      throw e;
    }
    catch (Exception e)
    {
      log.error(e.getMessage());
      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testUploadModifyDuplicateRelationIds() throws Exception
  {
    final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
    final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
    final Set<Long> nodeIds =
      OsmTestUtils.createTestNodes(changesetId, originalBounds);
    final Long[] nodeIdsArr = nodeIds.toArray(new Long[]{});

    final Set<Long> wayIds =
      OsmTestUtils.createTestWays(changesetId, nodeIds);
    final Long[] wayIdsArr = wayIds.toArray(new Long[]{});

    final Set<Long> relationIds =
      OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
    final Long[] relationIdsArr = relationIds.toArray(new Long[]{});


    //Try to upload two relations with the same ID.  The request should fail and no
    //data in the system should be modified.
    try
    {
      resource()
        .path("api/0.6/changeset/" + changesetId + "/upload")
        .queryParam("mapId", "" + mapId)
        .type(MediaType.TEXT_XML)
        .accept(MediaType.TEXT_XML)
        .post(
          Document.class,
          "<osmChange version=\"0.3\" generator=\"iD\">" +
            "<create/>" +
            "<modify>" +
              "<relation id=\"" + relationIdsArr[0] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[0] + "\"></member>" +
              "</relation>" +
              "<relation id=\"" + relationIdsArr[0] + "\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[2] + "\"></member>" +
              "</relation>" +
            "</modify>" +
            "<delete if-unused=\"true\"/>" +
          "</osmChange>");
    }
    catch (UniformInterfaceException e)
    {
      ClientResponse r = e.getResponse();
      Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
      Assert.assertTrue(r.getEntity(String.class).contains("Duplicate OSM element ID"));

      OsmTestUtils.verifyTestDataUnmodified(
        originalBounds, changesetId, nodeIds, wayIds, relationIds);

      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testUploadModifyRelationReferencingItself() throws Exception
  {
    final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
    final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
    final Set<Long> nodeIds =
      OsmTestUtils.createTestNodes(changesetId, originalBounds);
    final Long[] nodeIdsArr = nodeIds.toArray(new Long[]{});

    final Set<Long> wayIds =
      OsmTestUtils.createTestWays(changesetId, nodeIds);
    final Long[] wayIdsArr = wayIds.toArray(new Long[]{});

    final Set<Long> relationIds =
      OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
    final Long[] relationIdsArr = relationIds.toArray(new Long[]{});


    //Update the changeset with a relation that references itself as a member.  A failure should
    //occur and no data in the system should be modified.
    try
    {
      resource()
        .path("api/0.6/changeset/" + changesetId + "/upload")
        .queryParam("mapId", "" + mapId)
        .type(MediaType.TEXT_XML)
        .accept(MediaType.TEXT_XML)
        .post(
          Document.class,
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
    catch (UniformInterfaceException e)
    {
      ClientResponse r = e.getResponse();
      Assert.assertEquals(Status.CONFLICT, Status.fromStatusCode(r.getStatus()));
      Assert.assertTrue(r.getEntity(String.class).contains(
        "contains a relation member that references itself"));

      OsmTestUtils.verifyTestDataUnmodified(
        originalBounds, changesetId, nodeIds, wayIds, relationIds);

      throw e;
    }
    catch (Exception e)
    {
      log.error(e.getMessage());
      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testUploadModifyRelationEmptyMemberId() throws Exception
  {
    final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
    final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
    final Set<Long> nodeIds =
      OsmTestUtils.createTestNodes(changesetId, originalBounds);
    final Long[] nodeIdsArr = nodeIds.toArray(new Long[]{});

    final Set<Long> wayIds =
      OsmTestUtils.createTestWays(changesetId, nodeIds);

    final Set<Long> relationIds =
      OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
    final Long[] relationIdsArr = relationIds.toArray(new Long[]{});


    //Update the changeset where one of the relations has a member with an empty string for an ID.
    //A failure should occur and no data in the system should be modified.
    try
    {
      resource()
        .path("api/0.6/changeset/" + changesetId + "/upload")
        .queryParam("mapId", "" + mapId)
        .type(MediaType.TEXT_XML)
        .accept(MediaType.TEXT_XML)
        .post(
          Document.class,
          "<osmChange version=\"0.3\" generator=\"iD\">" +
            "<create/>" +
            "<modify>" +
              "<relation id=\"" + relationIdsArr[0] + "\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
              "</relation>" +
              "<relation id=\"" + relationIdsArr[1] + "\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                "<member type=\"node\" role=\"role1\" ref=\"\"></member>" + //empty member ID
                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
              "</relation>" +
            "</modify>" +
            "<delete if-unused=\"true\"/>" +
          "</osmChange>");
    }
    catch (UniformInterfaceException e)
    {
      ClientResponse r = e.getResponse();
      Assert.assertEquals(Status.CONFLICT, Status.fromStatusCode(r.getStatus()));
      Assert.assertTrue(r.getEntity(String.class).contains("Invalid version"));

      OsmTestUtils.verifyTestDataUnmodified(
        originalBounds, changesetId, nodeIds, wayIds, relationIds);

      throw e;
    }
    catch (Exception e)
    {
      log.error(e.getMessage());
      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testUploadModifyRelationMissingMemberId() throws Exception
  {
    final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
    final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
    final Set<Long> nodeIds =
      OsmTestUtils.createTestNodes(changesetId, originalBounds);
    final Long[] nodeIdsArr = nodeIds.toArray(new Long[]{});

    final Set<Long> wayIds =
      OsmTestUtils.createTestWays(changesetId, nodeIds);

    final Set<Long> relationIds =
      OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
    final Long[] relationIdsArr = relationIds.toArray(new Long[]{});


    //Update the changeset where one of the relations has a member with an empty string for an ID.
    //A failure should occur and no data in the system should be modified.
    try
    {
      resource()
        .path("api/0.6/changeset/" + changesetId + "/upload")
        .queryParam("mapId", "" + mapId)
        .type(MediaType.TEXT_XML)
        .accept(MediaType.TEXT_XML)
        .post(
          Document.class,
          "<osmChange version=\"0.3\" generator=\"iD\">" +
            "<create/>" +
            "<modify>" +
              "<relation id=\"" + relationIdsArr[0] + "\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
              "</relation>" +
              "<relation id=\"" + relationIdsArr[1] + "\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                "<member type=\"node\" role=\"role1\"></member>" + //missing member ID
                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
              "</relation>" +
            "</modify>" +
            "<delete if-unused=\"true\"/>" +
          "</osmChange>");
    }
    catch (UniformInterfaceException e)
    {
      ClientResponse r = e.getResponse();
      Assert.assertEquals(Status.CONFLICT, Status.fromStatusCode(r.getStatus()));
      Assert.assertTrue(r.getEntity(String.class).contains("Invalid version"));

      OsmTestUtils.verifyTestDataUnmodified(
        originalBounds, changesetId, nodeIds, wayIds, relationIds);

      throw e;
    }
    catch (Exception e)
    {
      log.error(e.getMessage());
      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testUploadModifyRelationInvalidMemberType() throws Exception
  {
    final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
    final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
    final Set<Long> nodeIds =
      OsmTestUtils.createTestNodes(changesetId, originalBounds);
    final Long[] nodeIdsArr = nodeIds.toArray(new Long[]{});

    final Set<Long> wayIds =
      OsmTestUtils.createTestWays(changesetId, nodeIds);

    final Set<Long> relationIds =
      OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
    final Long[] relationIdsArr = relationIds.toArray(new Long[]{});


    //Update the changeset where one of the relations has a member with an empty string for one
    //of its member's type.  A failure should occur and no data in the system should be modified.
    try
    {
      resource()
        .path("api/0.6/changeset/" + changesetId + "/upload")
        .queryParam("mapId", "" + mapId)
        .type(MediaType.TEXT_XML)
        .accept(MediaType.TEXT_XML)
        .post(
          Document.class,
          "<osmChange version=\"0.3\" generator=\"iD\">" +
            "<create/>" +
            "<modify>" +
              "<relation id=\"" + relationIdsArr[0] + "\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                //invalid member type
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
    catch (UniformInterfaceException e)
    {
      ClientResponse r = e.getResponse();
      Assert.assertEquals(Status.CONFLICT, Status.fromStatusCode(r.getStatus()));
      Assert.assertTrue(r.getEntity(String.class).contains("Invalid version"));

      OsmTestUtils.verifyTestDataUnmodified(
        originalBounds, changesetId, nodeIds, wayIds, relationIds);

      throw e;
    }
    catch (Exception e)
    {
      log.error(e.getMessage());
      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testUploadModifyRelationMissingMemberType() throws Exception
  {
    final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
    final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
    final Set<Long> nodeIds =
      OsmTestUtils.createTestNodes(changesetId, originalBounds);
    final Long[] nodeIdsArr = nodeIds.toArray(new Long[]{});

    final Set<Long> wayIds =
      OsmTestUtils.createTestWays(changesetId, nodeIds);

    final Set<Long> relationIds =
      OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
    final Long[] relationIdsArr = relationIds.toArray(new Long[]{});


    //Update the changeset where one of the relations has a member with no type.  A failure should
    //occur and no data in the system should be modified.
    try
    {
      resource()
        .path("api/0.6/changeset/" + changesetId + "/upload")
        .queryParam("mapId", "" + mapId)
        .type(MediaType.TEXT_XML)
        .accept(MediaType.TEXT_XML)
        .post(
          Document.class,
          "<osmChange version=\"0.3\" generator=\"iD\">" +
            "<create/>" +
            "<modify>" +
              "<relation id=\"" + relationIdsArr[0] + "\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                //missing member type
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
    catch (UniformInterfaceException e)
    {
      ClientResponse r = e.getResponse();
      Assert.assertEquals(Status.CONFLICT, Status.fromStatusCode(r.getStatus()));
      Assert.assertTrue(r.getEntity(String.class).contains("Invalid version"));

      OsmTestUtils.verifyTestDataUnmodified(
        originalBounds, changesetId, nodeIds, wayIds, relationIds);

      throw e;
    }
    catch (Exception e)
    {
      log.error(e.getMessage());
      throw e;
    }
  }
}
