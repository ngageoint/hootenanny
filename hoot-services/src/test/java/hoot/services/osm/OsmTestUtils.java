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
package hoot.services.osm;

import java.sql.Connection;
import java.sql.Statement;
import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import javax.xml.xpath.XPath;

import org.apache.commons.lang3.StringUtils;
import org.apache.commons.lang3.reflect.MethodUtils;
import org.apache.xpath.XPathAPI;
import org.joda.time.format.DateTimeFormat;
import org.joda.time.format.DateTimeFormatter;
import org.junit.Assert;
import org.postgresql.util.PGobject;
import org.w3c.dom.Document;

import com.mysema.query.sql.SQLExpressions;
import com.mysema.query.sql.SQLQuery;
import com.mysema.query.sql.dml.SQLUpdateClause;

import hoot.services.HootProperties;
import hoot.services.db.DbUtils;
import hoot.services.db.DbUtils.RecordBatchType;
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
import hoot.services.geo.GeoUtils;
import hoot.services.models.osm.Changeset;
import hoot.services.models.osm.Element;
import hoot.services.models.osm.Element.ElementType;
import hoot.services.models.osm.ElementFactory;
import hoot.services.models.osm.Node;
import hoot.services.models.osm.Relation;
import hoot.services.models.osm.RelationMember;
import hoot.services.models.osm.Way;
import hoot.services.utils.XmlUtils;


/*
 * Utilities for creating test data that's common to many OSM Resource related tests.  Tests
 * that need to test with custom data configurations should not use these methods and construct
 * their test data from scratch.
 *
 * Great effort has occurred to try to consolidate as much redundant code from the
 * ChangesetResourceTests in this class as possible into this class, but there is probably still
 * more code that could be refactored into here.
 *
 * //TODO: The verify individual element methods in this class could be used by the
 * ChangesetUploadResource tests and reduce their amount of code dramatically.
 *
 * //TODO: This class should add a method that writes a second test map with some data to verify
 * we're not working with any data from the wrong map.
 */
public class OsmTestUtils {
    public static Connection conn;
    public static long mapId = -1;
    public static long userId = -1;

    private static DateTimeFormatter timeFormatter = DateTimeFormat.forPattern(DbUtils.TIMESTAMP_DATE_FORMAT);

    /*
     * Creates a starting geo bounds for all the tests; simulates a bounds that
     * was calculated before these test run
     */
    public static BoundingBox createStartingTestBounds() throws Exception {
        return new BoundingBox(-78.02265434416296, 38.90089748801109, -77.0224564416296, 39.90085678801109);
    }

    /*
     * Creates a modified geo bounds (bounds that changeset should have after an
     * update) for all the tests
     */
    public static BoundingBox createAfterModifiedTestChangesetBounds() throws Exception {
        return new BoundingBox(-79.02265434416296, 37.90089748801109, -77.0224564416296, 39.90085678801109);
    }

    /*
     * If a null bounds is specified, then the changeset gets no starting
     * bounds, as if it has newly been created with no data uploaded to it.
     */
    public static long createTestChangeset(final BoundingBox bounds) throws Exception {
        // default changeset we use for testing has 12 edits
        return createTestChangeset(bounds, 12);
    }

    // allow num changes to be passed in as a var for testing purposes
    public static long createTestChangeset(final BoundingBox bounds, final int numChanges) throws Exception {
        final long changesetId = Changeset.insertNew(mapId, userId, conn);
        Changeset changeset = new Changeset(mapId, changesetId, conn);
        if (bounds != null) {
            changeset.setBounds(bounds);
        }
        changeset.updateNumChanges(numChanges);
        changeset.updateExpiration();

        QChangesets changesets = QChangesets.changesets;

        hoot.services.db2.Changesets changesetPojo = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                .from(changesets).where(changesets.id.eq(changesetId)).singleResult(changesets);

        Assert.assertNotNull(changesetPojo);
        Assert.assertEquals(numChanges, (long) changesetPojo.getNumChanges());
        Assert.assertEquals(userId, (long) changesetPojo.getUserId());
        return changesetId;
    }

    /*
     * Technically, this should return a list to support closed polys, but since
     * the test data used in this method doesn't have closed polys and other
     * tests test for that, leaving it returned as a Set for now.
     */
    public static Set<Long> createTestNodes(final long changesetId, final BoundingBox bounds) throws Exception {
        Set<Long> nodeIds = new LinkedHashSet<Long>();
        Map<String, String> tags = new HashMap<String, String>();

        tags.put("key 1", "val 1");
        tags.put("key 2", "val 2");
        nodeIds.add(Node.insertNew(changesetId, mapId, bounds.getMinLat(), bounds.getMinLon(), tags, conn));
        tags.clear();

        nodeIds.add(Node.insertNew(changesetId, mapId, bounds.getMaxLat(), bounds.getMaxLon(), null, conn));

        nodeIds.add(Node.insertNew(changesetId, mapId, bounds.getMinLat(), bounds.getMinLon(), null, conn));

        tags.put("key 3", "val 3");
        nodeIds.add(Node.insertNew(changesetId, mapId, bounds.getMinLat(), bounds.getMinLon(), tags, conn));
        tags.clear();

        tags.put("key 4", "val 4");
        nodeIds.add(Node.insertNew(changesetId, mapId, bounds.getMinLat(), bounds.getMinLon(), tags, conn));
        tags.clear();

        return nodeIds;
    }

    public static Set<Long> createTestWays(final long changesetId, final Set<Long> nodeIds) throws Exception {
        Set<Long> wayIds = new LinkedHashSet<Long>();
        List<Long> wayNodeIds = new ArrayList<Long>();
        final Long[] nodeIdsArr = nodeIds.toArray(new Long[] {});
        Map<String, String> tags = new HashMap<String, String>();

        tags.put("key 1", "val 1");
        tags.put("key 2", "val 2");
        wayNodeIds.add(nodeIdsArr[0]);
        wayNodeIds.add(nodeIdsArr[1]);
        wayNodeIds.add(nodeIdsArr[4]);
        wayIds.add(insertNewWay(changesetId, mapId, wayNodeIds, tags, conn));
        tags.clear();
        wayNodeIds.clear();

        wayNodeIds.add(nodeIdsArr[2]);
        wayNodeIds.add(nodeIdsArr[1]);
        wayIds.add(insertNewWay(changesetId, mapId, wayNodeIds, null, conn));
        wayNodeIds.clear();

        tags.put("key 3", "val 3");
        wayNodeIds.add(nodeIdsArr[0]);
        wayNodeIds.add(nodeIdsArr[1]);
        wayIds.add(insertNewWay(changesetId, mapId, wayNodeIds, tags, conn));
        tags.clear();
        wayNodeIds.clear();

        return wayIds;
    }

    public static Set<Long> createTestRelations(final long changesetId, final Set<Long> nodeIds, final Set<Long> wayIds)
            throws Exception {
        Set<Long> relationIds = new LinkedHashSet<Long>();
        final Long[] nodeIdsArr = nodeIds.toArray(new Long[] {});
        final Long[] wayIdsArr = wayIds.toArray(new Long[] {});
        Map<String, String> tags = new HashMap<String, String>();
        List<RelationMember> members = new ArrayList<RelationMember>();

        members.add(new RelationMember(nodeIdsArr[0], ElementType.Node, "role1"));
        members.add(new RelationMember(wayIdsArr[1], ElementType.Way, "role3"));
        members.add(new RelationMember(wayIdsArr[0], ElementType.Way, "role2"));
        members.add(new RelationMember(nodeIdsArr[2], ElementType.Node));
        tags.put("key 1", "val 1");
        final long firstRelationId = insertNewRelation(changesetId, mapId, members, tags, conn);
        relationIds.add(firstRelationId);
        tags.clear();
        members.clear();

        tags.put("key 2", "val 2");
        tags.put("key 3", "val 3");
        members.add(new RelationMember(nodeIdsArr[4], ElementType.Node, "role1"));
        members.add(new RelationMember(firstRelationId, ElementType.Relation, "role1"));
        relationIds.add(insertNewRelation(changesetId, mapId, members, tags, conn));
        tags.clear();
        members.clear();

        tags.put("key 4", "val 4");
        members.add(new RelationMember(wayIdsArr[1], ElementType.Way));
        relationIds.add(insertNewRelation(changesetId, mapId, members, tags, conn));
        tags.clear();
        members.clear();

        members.add(new RelationMember(nodeIdsArr[2], ElementType.Node, "role1"));
        relationIds.add(insertNewRelation(changesetId, mapId, members, null, conn));
        members.clear();

        return relationIds;
    }

    // This is similar to createTestRelations but without any ways involved to
    // support certain tests.
    public static Set<Long> createTestRelationsNoWays(final long changesetId, final Set<Long> nodeIds)
            throws Exception {
        Set<Long> relationIds = new LinkedHashSet<Long>();
        final Long[] nodeIdsArr = nodeIds.toArray(new Long[] {});
        Map<String, String> tags = new HashMap<String, String>();
        List<RelationMember> members = new ArrayList<RelationMember>();

        members.add(new RelationMember(nodeIdsArr[0], ElementType.Node, "role1"));
        members.add(new RelationMember(nodeIdsArr[2], ElementType.Node));
        tags.put("key 1", "val 1");
        final long firstRelationId = insertNewRelation(changesetId, mapId, members, tags, conn);
        relationIds.add(firstRelationId);
        tags.clear();
        members.clear();

        tags.put("key 2", "val 2");
        tags.put("key 3", "val 3");
        members.add(new RelationMember(nodeIdsArr[4], ElementType.Node, "role1"));
        members.add(new RelationMember(firstRelationId, ElementType.Relation, "role1"));
        relationIds.add(insertNewRelation(changesetId, mapId, members, tags, conn));
        tags.clear();
        members.clear();

        tags.put("key 4", "val 4");
        members.add(new RelationMember(nodeIdsArr[2], ElementType.Node, "role1"));
        relationIds.add(insertNewRelation(changesetId, mapId, members, tags, conn));
        tags.clear();
        members.clear();

        return relationIds;
    }

    public static void verifyTestChangesetCreatedByRequest(final Long changesetId) {
        QChangesets changesets = QChangesets.changesets;
        hoot.services.db2.Changesets changeset = new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(changesets)
                .where(changesets.id.eq(changesetId)).singleResult(changesets);

        Assert.assertNotNull(changeset);
        final Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
        Assert.assertTrue(changeset.getCreatedAt().before(now));
        Assert.assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));
        Assert.assertEquals(new Double(GeoUtils.DEFAULT_COORD_VALUE), changeset.getMaxLat());
        Assert.assertEquals(new Double(GeoUtils.DEFAULT_COORD_VALUE), changeset.getMaxLon());
        Assert.assertEquals(new Double(GeoUtils.DEFAULT_COORD_VALUE), changeset.getMinLon());
        Assert.assertEquals(new Double(GeoUtils.DEFAULT_COORD_VALUE), changeset.getMinLat());
        Assert.assertEquals(new Integer(0), changeset.getNumChanges());
        Assert.assertEquals(new Long(userId), changeset.getUserId());
    }

    public static void verifyTestDataUnmodified(final BoundingBox originalBounds, final long changesetId,
            final Set<Long> nodeIds, final Set<Long> wayIds, final Set<Long> relationIds) {
        verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds, true);
    }

    public static void verifyTestDataUnmodified(final BoundingBox originalBounds, final long changesetId,
            final Set<Long> nodeIds, final Set<Long> wayIds, final Set<Long> relationIds, final boolean verifyTags) {
        verifyTestChangesetUnmodified(changesetId, originalBounds);
        verifyTestNodesUnmodified(nodeIds, changesetId, originalBounds, verifyTags);
        verifyTestWaysUnmodified(wayIds, nodeIds, changesetId, verifyTags);
        verifyTestRelationsUnmodified(relationIds, wayIds, nodeIds, changesetId, verifyTags);
    }

    public static void verifyTestChangesetUnmodified(final long changesetId) {
        try {
            QChangesets changesets = QChangesets.changesets;
            hoot.services.db2.Changesets changeset = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                    .from(changesets).where(changesets.id.eq(changesetId)).singleResult(changesets);

            Assert.assertNotNull(changeset);
            final Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
            Assert.assertTrue(changeset.getCreatedAt().before(now));
            Assert.assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));
            Assert.assertEquals(new Integer(12), changeset.getNumChanges());
            Assert.assertEquals(new Long(userId), changeset.getUserId());

            // make sure the changeset bounds has never been modified
            hoot.services.models.osm.Changeset hootChangeset = new hoot.services.models.osm.Changeset(mapId,
                    changesetId, conn);
            BoundingBox changesetBounds = hootChangeset.getBounds();
            BoundingBox defaultBounds = new BoundingBox();
            Assert.assertTrue(changesetBounds.equals(defaultBounds));
        }
        catch (Exception e) {
            Assert.fail("Error checking changeset: " + e.getMessage());
        }
    }

    // for testing purposes allow specifying numChanges as a variable
    public static void verifyTestChangesetClosed(final long changesetId, final int numChanges) {
        QChangesets changesets = QChangesets.changesets;
        hoot.services.db2.Changesets changeset = new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(changesets)
                .where(changesets.id.eq(changesetId)).singleResult(changesets);

        Assert.assertNotNull(changeset);
        final Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
        Assert.assertTrue(changeset.getCreatedAt().before(now));
        Assert.assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));
        Assert.assertTrue(changeset.getClosedAt().before(now));
        Assert.assertEquals(new Integer(numChanges), changeset.getNumChanges());
        Assert.assertEquals(new Long(userId), changeset.getUserId());
    }

    public static void verifyTestChangesetClosed(final long changesetId) {
        // default changeset we use for testing has 12 edits
        verifyTestChangesetClosed(changesetId, 12);
    }

    public static void verifyTestChangesetUnmodified(final long changesetId, final BoundingBox originalBounds) {
        try {
            QChangesets changesets = QChangesets.changesets;
            hoot.services.db2.Changesets changeset = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                    .from(changesets).where(changesets.id.eq(changesetId)).singleResult(changesets);

            Assert.assertNotNull(changeset);
            final Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
            Assert.assertTrue(changeset.getCreatedAt().before(now));
            Assert.assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));
            Assert.assertEquals(new Integer(12), changeset.getNumChanges());
            Assert.assertEquals(new Long(userId), changeset.getUserId());

            // make sure the changeset bounds wasn't updated
            hoot.services.models.osm.Changeset hootChangeset = new hoot.services.models.osm.Changeset(mapId,
                    changesetId, conn);
            BoundingBox changesetBounds = hootChangeset.getBounds();
            BoundingBox defaultBounds = new BoundingBox();
            // a change the size of the expansion factor is made automatically,
            // so the changeset's
            // bounds should be no larger than that
            defaultBounds.expand(originalBounds,
                    Double.parseDouble(HootProperties.getDefault("changesetBoundsExpansionFactorDeegrees")));
            Assert.assertTrue(changesetBounds.equals(defaultBounds));
        }
        catch (Exception e) {
            Assert.fail("Error checking changeset: " + e.getMessage());
        }
    }

    public static void verifyTestNodesUnmodified(final Set<Long> nodeIds, final long changesetId,
            final BoundingBox originalBounds) {
        verifyTestNodesUnmodified(nodeIds, changesetId, originalBounds, true);
    }

    public static void verifyTestNodesUnmodified(final Set<Long> nodeIds, final long changesetId,
            final BoundingBox originalBounds, final boolean verifyTags) {
        try {
            QCurrentNodes currentNodes = QCurrentNodes.currentNodes;

            final Long[] nodeIdsArr = nodeIds.toArray(new Long[] {});
            final Map<Long, CurrentNodes> nodes = new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentNodes)
                    .where((currentNodes.changesetId.eq(changesetId))).map(currentNodes.id, currentNodes)

            ;
            Assert.assertEquals(5, nodes.size());

            CurrentNodes nodeRecord = nodes.get(nodeIdsArr[0]);
            Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
            Assert.assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
            Assert.assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
            Assert.assertEquals(nodeIdsArr[0], nodeRecord.getId());
            Assert.assertEquals(new Long(1), nodeRecord.getVersion());
            if (verifyTags) {
                try {
                    Map<String, String> tags = PostgresUtils.postgresObjToHStore((PGobject) nodeRecord.getTags());
                    Assert.assertEquals(2, tags.size());
                    // System.out.println(tags.get("key 1"));
                    Assert.assertEquals("val 1", tags.get("key 1"));
                    // System.out.println(tags.get("key 2"));
                    Assert.assertEquals("val 2", tags.get("key 2"));
                }
                catch (Exception e) {
                    Assert.fail("Error checking node tags: " + e.getMessage());
                }
            }

            nodeRecord = nodes.get(nodeIdsArr[1]);
            Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
            Assert.assertEquals(new Double(originalBounds.getMaxLat()), nodeRecord.getLatitude());
            Assert.assertEquals(new Double(originalBounds.getMaxLon()), nodeRecord.getLongitude());
            Assert.assertEquals(nodeIdsArr[1], nodeRecord.getId());
            Assert.assertEquals(new Long(1), nodeRecord.getVersion());
            if (verifyTags) {
                try {
                    Assert.assertTrue(nodeRecord.getTags() == null
                            || StringUtils.isEmpty(((PGobject) nodeRecord.getTags()).getValue()));
                }
                catch (Exception e) {
                    Assert.fail("Error checking node tags: " + e.getMessage());
                }
            }

            nodeRecord = nodes.get(nodeIdsArr[2]);
            Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
            Assert.assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
            Assert.assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
            Assert.assertEquals(nodeIdsArr[2], nodeRecord.getId());
            Assert.assertEquals(new Long(1), nodeRecord.getVersion());
            if (verifyTags) {
                try {
                    Assert.assertTrue(nodeRecord.getTags() == null
                            || StringUtils.isEmpty(((PGobject) nodeRecord.getTags()).getValue()));
                }
                catch (Exception e) {
                    Assert.fail("Error checking node tags: " + e.getMessage());
                }
            }

            nodeRecord = nodes.get(nodeIdsArr[3]);
            Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
            Assert.assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
            Assert.assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
            Assert.assertEquals(nodeIdsArr[3], nodeRecord.getId());
            Assert.assertEquals(new Long(1), nodeRecord.getVersion());
            if (verifyTags) {
                try {
                    Map<String, String> tags = PostgresUtils.postgresObjToHStore((PGobject) nodeRecord.getTags());
                    Assert.assertEquals(1, tags.size());
                    Assert.assertEquals("val 3", tags.get("key 3"));
                }
                catch (Exception e) {
                    Assert.fail("Error checking node tags: " + e.getMessage());
                }
            }

            nodeRecord = nodes.get(nodeIdsArr[4]);
            Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
            Assert.assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
            Assert.assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
            Assert.assertEquals(nodeIdsArr[4], nodeRecord.getId());
            Assert.assertEquals(new Long(1), nodeRecord.getVersion());
            if (verifyTags) {
                try {
                    Map<String, String> tags = PostgresUtils.postgresObjToHStore((PGobject) nodeRecord.getTags());
                    Assert.assertEquals(1, tags.size());
                    Assert.assertEquals("val 4", tags.get("key 4"));
                }
                catch (Exception e) {
                    Assert.fail("Error checking node tags: " + e.getMessage());
                }
            }
        }
        catch (Exception e) {
            Assert.fail("Error checking nodes: " + e.getMessage());
        }
    }

    public static void verifyTestWaysUnmodified(final Set<Long> wayIds, final Set<Long> nodeIds,
            final long changesetId) {
        verifyTestWaysUnmodified(wayIds, nodeIds, changesetId, true);
    }

    public static void verifyTestWaysUnmodified(final Set<Long> wayIds, final Set<Long> nodeIds, final long changesetId,
            final boolean verifyTags) {
        QCurrentWays currentWays = QCurrentWays.currentWays;
        final Long[] nodeIdsArr = nodeIds.toArray(new Long[] {});
        final Long[] wayIdsArr = wayIds.toArray(new Long[] {});
        try {
            final Map<Long, CurrentWays> ways = new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentWays)
                    .where((currentWays.changesetId.eq(changesetId))).map(currentWays.id, currentWays)

            ;
            Assert.assertEquals(3, ways.size());

            QCurrentWayNodes currentWayNodes = QCurrentWayNodes.currentWayNodes;
            CurrentWays wayRecord = ways.get(wayIdsArr[0]);
            Assert.assertEquals(new Long(changesetId), wayRecord.getChangesetId());
            Assert.assertEquals(wayIdsArr[0], wayRecord.getId());
            Assert.assertEquals(new Long(1), wayRecord.getVersion());
            List<CurrentWayNodes> wayNodes = new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentWayNodes)
                    .where(currentWayNodes.wayId.eq(wayIdsArr[0])).orderBy(currentWayNodes.sequenceId.asc())
                    .list(currentWayNodes);
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
            if (verifyTags) {
                try {
                    Map<String, String> tags = PostgresUtils.postgresObjToHStore((PGobject) wayRecord.getTags());
                    Assert.assertEquals(2, tags.size());
                    Assert.assertEquals("val 1", tags.get("key 1"));
                    Assert.assertEquals("val 2", tags.get("key 2"));
                }
                catch (Exception e) {
                    Assert.fail("Error checking way tags: " + e.getMessage());
                }
            }

            wayRecord = ways.get(wayIdsArr[1]);
            Assert.assertEquals(new Long(changesetId), wayRecord.getChangesetId());
            Assert.assertEquals(wayIdsArr[1], wayRecord.getId());
            Assert.assertEquals(new Long(1), wayRecord.getVersion());
            wayNodes = new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentWayNodes)
                    .where(currentWayNodes.wayId.eq(wayIdsArr[1])).orderBy(currentWayNodes.sequenceId.asc())
                    .list(currentWayNodes)

            ;
            Assert.assertEquals(2, wayNodes.size());
            wayNode = wayNodes.get(0);
            Assert.assertEquals(nodeIdsArr[2], wayNode.getNodeId());
            Assert.assertEquals(new Long(1), wayNode.getSequenceId());
            Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());
            wayNode = wayNodes.get(1);
            Assert.assertEquals(nodeIdsArr[1], wayNode.getNodeId());
            Assert.assertEquals(new Long(2), wayNode.getSequenceId());
            Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());
            if (verifyTags) {
                try {
                    Assert.assertTrue(wayRecord.getTags() == null
                            || StringUtils.isEmpty(((PGobject) wayRecord.getTags()).getValue()));
                }
                catch (Exception e) {
                    Assert.fail("Error checking way tags: " + e.getMessage());
                }
            }

            wayRecord = ways.get(wayIdsArr[2]);
            Assert.assertEquals(new Long(changesetId), wayRecord.getChangesetId());
            Assert.assertEquals(wayIdsArr[2], wayRecord.getId());
            Assert.assertEquals(new Long(1), wayRecord.getVersion());
            wayNodes = new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentWayNodes)
                    .where(currentWayNodes.wayId.eq(wayIdsArr[2])).orderBy(currentWayNodes.sequenceId.asc())
                    .list(currentWayNodes);
            Assert.assertEquals(2, wayNodes.size());
            wayNode = wayNodes.get(0);
            Assert.assertEquals(nodeIdsArr[0], wayNode.getNodeId());
            Assert.assertEquals(new Long(1), wayNode.getSequenceId());
            Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());
            wayNode = wayNodes.get(1);
            Assert.assertEquals(nodeIdsArr[1], wayNode.getNodeId());
            Assert.assertEquals(new Long(2), wayNode.getSequenceId());
            Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());
            if (verifyTags) {
                try {
                    Map<String, String> tags = PostgresUtils.postgresObjToHStore((PGobject) wayRecord.getTags());
                    Assert.assertEquals(1, tags.size());
                    Assert.assertEquals("val 3", tags.get("key 3"));
                }
                catch (Exception e) {
                    Assert.fail("Error checking way tags: " + e.getMessage());
                }
            }
        }
        catch (Exception e) {
            Assert.fail("Error checking ways: " + e.getMessage());
        }
    }

    public static void verifyTestRelationsUnmodified(final Set<Long> relationIds, final Set<Long> wayIds,
            final Set<Long> nodeIds, final long changesetId) {
        verifyTestRelationsUnmodified(relationIds, wayIds, nodeIds, changesetId, true);
    }

    public static void verifyTestRelationsUnmodified(final Set<Long> relationIds, final Set<Long> wayIds,
            final Set<Long> nodeIds, final long changesetId, final boolean verifyTags) {
        QCurrentRelations currentRelations = QCurrentRelations.currentRelations;
        final Long[] relationIdsArr = relationIds.toArray(new Long[] {});
        final Long[] nodeIdsArr = nodeIds.toArray(new Long[] {});
        final Long[] wayIdsArr = wayIds.toArray(new Long[] {});
        try {
            final Map<Long, CurrentRelations> relations = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                    .from(currentRelations).where((currentRelations.changesetId.eq(changesetId)))
                    .map(currentRelations.id, currentRelations);
            Assert.assertEquals(4, relations.size());

            QCurrentRelationMembers currentRelationMembers = QCurrentRelationMembers.currentRelationMembers;
            CurrentRelations relationRecord = relations.get(relationIdsArr[0]);
            Assert.assertEquals(new Long(changesetId), relationRecord.getChangesetId());
            Assert.assertEquals(relationIdsArr[0], relationRecord.getId());
            Assert.assertEquals(new Long(1), relationRecord.getVersion());
            List<CurrentRelationMembers> relationMembers = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                    .from(currentRelationMembers).where(currentRelationMembers.relationId.eq(relationIdsArr[0]))
                    .orderBy(currentRelationMembers.sequenceId.asc()).list(currentRelationMembers);
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
            if (verifyTags) {
                try {
                    Map<String, String> tags = PostgresUtils.postgresObjToHStore((PGobject) relationRecord.getTags());
                    Assert.assertEquals(1, tags.size());
                    Assert.assertEquals("val 1", tags.get("key 1"));
                }
                catch (Exception e) {
                    Assert.fail("Error checking relation tags: " + e.getMessage());
                }
            }

            relationRecord = relations.get(relationIdsArr[1]);
            Assert.assertEquals(new Long(changesetId), relationRecord.getChangesetId());
            Assert.assertEquals(relationIdsArr[1], relationRecord.getId());
            Assert.assertEquals(new Long(1), relationRecord.getVersion());
            relationMembers = new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentRelationMembers)
                    .where(currentRelationMembers.relationId.eq(relationIdsArr[1]))
                    .orderBy(currentRelationMembers.sequenceId.asc()).list(currentRelationMembers);
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
            if (verifyTags) {
                try {
                    Map<String, String> tags = PostgresUtils.postgresObjToHStore((PGobject) relationRecord.getTags());
                    Assert.assertEquals(2, tags.size());
                    Assert.assertEquals("val 2", tags.get("key 2"));
                    Assert.assertEquals("val 3", tags.get("key 3"));
                }
                catch (Exception e) {
                    Assert.fail("Error checking relation tags: " + e.getMessage());
                }
            }

            relationRecord = relations.get(relationIdsArr[2]);
            Assert.assertEquals(new Long(changesetId), relationRecord.getChangesetId());
            Assert.assertEquals(relationIdsArr[2], relationRecord.getId());
            Assert.assertEquals(new Long(1), relationRecord.getVersion());
            relationMembers = new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentRelationMembers)
                    .where(currentRelationMembers.relationId.eq(relationIdsArr[2]))
                    .orderBy(currentRelationMembers.sequenceId.asc()).list(currentRelationMembers);
            Assert.assertEquals(1, relationMembers.size());
            member = relationMembers.get(0);
            Assert.assertEquals(relationRecord.getId(), member.getRelationId());
            Assert.assertEquals(DbUtils.nwr_enum.way, member.getMemberType());
            Assert.assertEquals("", member.getMemberRole());
            Assert.assertEquals(new Integer(1), member.getSequenceId());

            Assert.assertEquals(wayIdsArr[1], member.getMemberId());
            if (verifyTags) {
                try {
                    Map<String, String> tags = PostgresUtils.postgresObjToHStore((PGobject) relationRecord.getTags());
                    Assert.assertEquals(1, tags.size());
                    Assert.assertEquals("val 4", tags.get("key 4"));
                }
                catch (Exception e) {
                    Assert.fail("Error checking relation tags: " + e.getMessage());
                }
            }

            relationRecord = relations.get(relationIdsArr[3]);
            Assert.assertEquals(new Long(changesetId), relationRecord.getChangesetId());
            Assert.assertEquals(relationIdsArr[3], relationRecord.getId());
            Assert.assertEquals(new Long(1), relationRecord.getVersion());
            relationMembers =

                    new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentRelationMembers)
                            .where(currentRelationMembers.relationId.eq(relationIdsArr[3]))
                            .orderBy(currentRelationMembers.sequenceId.asc()).list(currentRelationMembers);
            Assert.assertEquals(1, relationMembers.size());
            member = relationMembers.get(0);
            Assert.assertEquals(relationRecord.getId(), member.getRelationId());
            Assert.assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
            Assert.assertEquals("role1", member.getMemberRole());
            Assert.assertEquals(new Integer(1), member.getSequenceId());

            Assert.assertEquals(nodeIdsArr[2], member.getMemberId());
            if (verifyTags) {
                try {
                    Assert.assertTrue(relationRecord.getTags() == null
                            || StringUtils.isEmpty(((PGobject) relationRecord.getTags()).getValue()));
                }
                catch (Exception e) {
                    Assert.fail("Error checking relation tags: " + e.getMessage());
                }
            }
        }
        catch (Exception e) {
            Assert.fail("Error checking relations: " + e.getMessage());
        }
    }

    public static void verifyTestRelationsNoWaysUnmodified(final Set<Long> relationIds, final Set<Long> nodeIds,
            final long changesetId, final boolean verifyTags) {
        QCurrentRelations currentRelations = QCurrentRelations.currentRelations;
        final Long[] relationIdsArr = relationIds.toArray(new Long[] {});
        final Long[] nodeIdsArr = nodeIds.toArray(new Long[] {});
        try {
            final Map<Long, CurrentRelations> relations = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                    .from(currentRelations).where((currentRelations.changesetId.eq(changesetId)))
                    .map(currentRelations.id, currentRelations)

            ;
            Assert.assertEquals(3, relations.size());

            QCurrentRelationMembers currentRelationMembers = QCurrentRelationMembers.currentRelationMembers;
            CurrentRelations relationRecord = relations.get(relationIdsArr[0]);
            Assert.assertEquals(new Long(changesetId), relationRecord.getChangesetId());
            Assert.assertEquals(relationIdsArr[0], relationRecord.getId());
            Assert.assertEquals(new Long(1), relationRecord.getVersion());
            List<CurrentRelationMembers> relationMembers = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                    .from(currentRelationMembers).where(currentRelationMembers.relationId.eq(relationIdsArr[0]))
                    .orderBy(currentRelationMembers.sequenceId.asc()).list(currentRelationMembers)

            ;
            Assert.assertEquals(2, relationMembers.size());
            CurrentRelationMembers member = relationMembers.get(0);
            Assert.assertEquals(relationRecord.getId(), member.getRelationId());
            Assert.assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
            Assert.assertEquals("role1", member.getMemberRole());
            Assert.assertEquals(new Integer(1), member.getSequenceId());

            Assert.assertEquals(nodeIdsArr[0], member.getMemberId());
            member = relationMembers.get(1);
            Assert.assertEquals(relationRecord.getId(), member.getRelationId());
            Assert.assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
            Assert.assertEquals("", member.getMemberRole());
            Assert.assertEquals(new Integer(2), member.getSequenceId());

            Assert.assertEquals(nodeIdsArr[2], member.getMemberId());
            if (verifyTags) {
                try {
                    Map<String, String> tags = PostgresUtils.postgresObjToHStore((PGobject) relationRecord.getTags());
                    Assert.assertEquals(1, tags.size());
                    Assert.assertEquals("val 1", tags.get("key 1"));
                }
                catch (Exception e) {
                    Assert.fail("Error checking relation tags: " + e.getMessage());
                }
            }

            relationRecord = relations.get(relationIdsArr[1]);
            Assert.assertEquals(new Long(changesetId), relationRecord.getChangesetId());
            Assert.assertEquals(relationIdsArr[1], relationRecord.getId());
            Assert.assertEquals(new Long(1), relationRecord.getVersion());
            relationMembers = new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentRelationMembers)
                    .where(currentRelationMembers.relationId.eq(relationIdsArr[1]))
                    .orderBy(currentRelationMembers.sequenceId.asc()).list(currentRelationMembers)

            ;
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
            if (verifyTags) {
                try {
                    Map<String, String> tags = PostgresUtils.postgresObjToHStore((PGobject) relationRecord.getTags());
                    Assert.assertEquals(2, tags.size());
                    Assert.assertEquals("val 2", tags.get("key 2"));
                    Assert.assertEquals("val 3", tags.get("key 3"));
                }
                catch (Exception e) {
                    Assert.fail("Error checking relation tags: " + e.getMessage());
                }
            }

            relationRecord = relations.get(relationIdsArr[2]);
            Assert.assertEquals(new Long(changesetId), relationRecord.getChangesetId());
            Assert.assertEquals(relationIdsArr[2], relationRecord.getId());
            Assert.assertEquals(new Long(1), relationRecord.getVersion());
            relationMembers = new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentRelationMembers)
                    .where(currentRelationMembers.relationId.eq(relationIdsArr[2]))
                    .orderBy(currentRelationMembers.sequenceId.asc()).list(currentRelationMembers)

            ;
            Assert.assertEquals(1, relationMembers.size());
            member = relationMembers.get(0);
            Assert.assertEquals(relationIdsArr[2], member.getRelationId());
            Assert.assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
            Assert.assertEquals("role1", member.getMemberRole());
            Assert.assertEquals(new Integer(1), member.getSequenceId());

            Assert.assertEquals(nodeIdsArr[2], member.getMemberId());
            if (verifyTags) {
                try {
                    Map<String, String> tags = PostgresUtils.postgresObjToHStore((PGobject) relationRecord.getTags());
                    Assert.assertEquals(1, tags.size());
                    Assert.assertEquals("val 4", tags.get("key 4"));
                }
                catch (Exception e) {
                    Assert.fail("Error checking relation tags: " + e.getMessage());
                }
            }
        }
        catch (Exception e) {
            Assert.fail("Error checking relations with no ways: " + e.getMessage());
        }
    }

    public static void verifyOsmHeader(final Document responseData) {
        try {
            XPath xpath = XmlUtils.createXPath();
            Assert.assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm").getLength());
            Assert.assertEquals("0.6", xpath.evaluate("//osm[1]/@version", responseData));
            Assert.assertNotNull(xpath.evaluate("//osm[1]/@generator", responseData));
            Assert.assertNotNull(xpath.evaluate("//osm[1]/@copyright", responseData));
            Assert.assertNotNull(xpath.evaluate("//osm[1]/@attribution", responseData));
            Assert.assertNotNull(xpath.evaluate("//osm[1]/@license", responseData));
        }
        catch (Exception e) {
            Assert.fail("Error parsing header from response document: " + e.getMessage());
        }
    }

    public static void verifyBounds(final Document responseData, final BoundingBox expectedBounds) {
        try {
            XPath xpath = XmlUtils.createXPath();
            Assert.assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm/bounds").getLength());
            Assert.assertEquals(expectedBounds.getMinLat(),
                    Double.parseDouble(xpath.evaluate("//osm/bounds[1]/@minlat", responseData)), 0.0);
            Assert.assertEquals(expectedBounds.getMinLon(),
                    Double.parseDouble(xpath.evaluate("//osm/bounds[1]/@minlon", responseData)), 0.0);
            Assert.assertEquals(expectedBounds.getMaxLat(),
                    Double.parseDouble(xpath.evaluate("//osm/bounds[1]/@maxlat", responseData)), 0.0);
            Assert.assertEquals(expectedBounds.getMaxLon(),
                    Double.parseDouble(xpath.evaluate("//osm/bounds[1]/@maxlon", responseData)), 0.0);
        }
        catch (Exception e) {
            Assert.fail("Error parsing bounds from response document: " + e.getMessage());
        }
    }

    public static void verifyNode(final Document responseData, final long index, final String expectedId,
            final long changesetId, final double lat, final double lon, final boolean multiLayerUniqueElementIds) {
        try {
            XPath xpath = XmlUtils.createXPath();
            if (!multiLayerUniqueElementIds) {
                Assert.assertEquals(Long.parseLong(expectedId),
                        Long.parseLong(xpath.evaluate("//osm/node[" + index + "]/@id", responseData)));
            }
            else {
                Assert.assertEquals(expectedId, xpath.evaluate("//osm/node[" + index + "]/@id", responseData));
            }
            Assert.assertEquals(true,
                    Boolean.parseBoolean(xpath.evaluate("//osm/node[" + index + "]/@visible", responseData)));
            Assert.assertEquals(1, Long.parseLong(xpath.evaluate("//osm/node[" + index + "]/@version", responseData)));
            Assert.assertEquals(changesetId,
                    Long.parseLong(xpath.evaluate("//osm/node[" + index + "]/@changeset", responseData)));

            String timeStampStr = xpath.evaluate("//osm/node[" + index + "]/@timestamp", responseData);
            // remove the millseconds portion of the string, since it can be of
            // variable length and cause
            // seemingly random problems when parsed
            if (timeStampStr.contains(".")) {
                timeStampStr = timeStampStr.split("\\.")[0];
            }

            Timestamp nodeTimestamp = new Timestamp(timeFormatter.parseDateTime(timeStampStr).getMillis());
            final Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
            Assert.assertTrue(nodeTimestamp.before(now));
            Assert.assertEquals("user-with-id-" + userId,
                    xpath.evaluate("//osm/node[" + index + "]/@user", responseData));
            Assert.assertEquals(userId, Long.parseLong(xpath.evaluate("//osm/node[" + index + "]/@uid", responseData)));
            Assert.assertEquals(lat, Double.parseDouble(xpath.evaluate("//osm/node[" + index + "]/@lat", responseData)),
                    0.0);
            Assert.assertEquals(lon, Double.parseDouble(xpath.evaluate("//osm/node[" + index + "]/@lon", responseData)),
                    0.0);
        }
        catch (Exception e) {
            Assert.fail("Error parsing node from response document: " + e.getMessage());
        }
    }

    public static void verifyWay(final Document responseData, final long index, final String expectedId,
            final long changesetId, final Set<Long> expectedWayNodeIds, final boolean multiLayerUniqueElementIds) {
        try {
            XPath xpath = XmlUtils.createXPath();

            if (!multiLayerUniqueElementIds) {
                Assert.assertEquals(Long.parseLong(expectedId),
                        Long.parseLong(xpath.evaluate("//osm/way[" + index + "]/@id", responseData)));
            }
            else {
                Assert.assertEquals(expectedId, xpath.evaluate("//osm/way[" + index + "]/@id", responseData));
            }
            Assert.assertEquals(true,
                    Boolean.parseBoolean(xpath.evaluate("//osm/way[" + index + "]/@visible", responseData)));
            Assert.assertEquals(1, Long.parseLong(xpath.evaluate("//osm/way[" + index + "]/@version", responseData)));
            Assert.assertEquals(changesetId,
                    Long.parseLong(xpath.evaluate("//osm/way[" + index + "]/@changeset", responseData)));

            String timeStampStr = xpath.evaluate("//osm/way[" + index + "]/@timestamp", responseData);
            // remove the millseconds portion of the string, since it can be of
            // variable length and cause
            // seemingly random problems when parsed
            if (timeStampStr.contains(".")) {
                timeStampStr = timeStampStr.split("\\.")[0];
            }

            Timestamp wayTimestamp = new Timestamp(timeFormatter.parseDateTime(timeStampStr).getMillis());
            final Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
            Assert.assertTrue(wayTimestamp.before(now));
            Assert.assertEquals("user-with-id-" + userId,
                    xpath.evaluate("//osm/way[" + index + "]/@user", responseData));
            Assert.assertEquals(userId, Long.parseLong(xpath.evaluate("//osm/way[" + index + "]/@uid", responseData)));

            if (expectedWayNodeIds != null) {
                final Long[] expectedWayNodeIdsArr = expectedWayNodeIds.toArray(new Long[] {});
                Assert.assertEquals(expectedWayNodeIdsArr.length,
                        XPathAPI.selectNodeList(responseData, "//osm/way[" + index + "]/nd").getLength());
                for (int i = 0; i < expectedWayNodeIdsArr.length; i++) {
                    Assert.assertEquals((long) expectedWayNodeIdsArr[i], Long.parseLong(
                            xpath.evaluate("//osm/way[" + index + "]/nd[" + (i + 1) + "]/@ref", responseData)));
                }
            }
        }
        catch (Exception e) {
            Assert.fail("Error parsing way from response document: " + e.getMessage());
        }
    }

    public static void verifyRelation(final Document responseData, final long index, final String expectedId,
            final long changesetId, final List<RelationMember> expectedMembers,
            final boolean multiLayerUniqueElementIds) {
        try {
            XPath xpath = XmlUtils.createXPath();

            if (!multiLayerUniqueElementIds) {
                Assert.assertEquals(Long.parseLong(expectedId),
                        Long.parseLong(xpath.evaluate("//osm/relation[" + index + "]/@id", responseData)));
            }
            else {
                Assert.assertEquals(expectedId, xpath.evaluate("//osm/relation[" + index + "]/@id", responseData));
            }
            Assert.assertEquals(true,
                    Boolean.parseBoolean(xpath.evaluate("//osm/relation[" + index + "]/@visible", responseData)));
            Assert.assertEquals(1,
                    Long.parseLong(xpath.evaluate("//osm/relation[" + index + "]/@version", responseData)));
            Assert.assertEquals(changesetId,
                    Long.parseLong(xpath.evaluate("//osm/relation[" + index + "]/@changeset", responseData)));

            String timeStampStr = xpath.evaluate("//osm/relation[" + index + "]/@timestamp", responseData);
            // remove the millseconds portion of the string, since it can be of
            // variable length and cause
            // seemingly random problems when parsed
            if (timeStampStr.contains(".")) {
                timeStampStr = timeStampStr.split("\\.")[0];
            }

            Timestamp relationTimestamp = new Timestamp(timeFormatter.parseDateTime(timeStampStr).getMillis());
            final Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
            Assert.assertTrue(relationTimestamp.before(now));
            Assert.assertEquals("user-with-id-" + userId,
                    xpath.evaluate("//osm/relation[" + index + "]/@user", responseData));
            Assert.assertEquals(userId,
                    Long.parseLong(xpath.evaluate("//osm/relation[" + index + "]/@uid", responseData)));

            if (expectedMembers != null) {
                Assert.assertEquals(expectedMembers.size(),
                        XPathAPI.selectNodeList(responseData, "//osm/relation[" + index + "]/member").getLength());
                int memberCtr = 1;
                for (RelationMember member : expectedMembers) {
                    Assert.assertEquals(member.getId(), Long.parseLong(xpath
                            .evaluate("//osm/relation[" + index + "]/member[" + memberCtr + "]/@ref", responseData)));
                    Assert.assertEquals(member.getRole(), xpath
                            .evaluate("//osm/relation[" + index + "]/member[" + memberCtr + "]/@role", responseData));
                    Assert.assertEquals(member.getType().toString().toLowerCase(), xpath
                            .evaluate("//osm/relation[" + index + "]/member[" + memberCtr + "]/@type", responseData));

                    memberCtr++;
                }
            }
        }
        catch (Exception e) {
            Assert.fail("Error parsing relation from response document: " + e.getMessage());
        }
    }

    public static void closeChangeset(final long mapId, final long changesetId) {
        QChangesets changesets = QChangesets.changesets;
        hoot.services.db2.Changesets changeset = new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(changesets)
                .where(changesets.id.eq(changesetId)).singleResult(changesets);

        final Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
        changeset.setClosedAt(now);
        // changesetDao.update(changeset);

        new SQLUpdateClause(conn, DbUtils.getConfiguration(mapId), changesets)
                .where(changesets.id.eq(changeset.getId())).populate(changeset).execute();
    }

    public static BoundingBox createTestQueryBounds() throws Exception {
        BoundingBox bounds = new BoundingBox(-78.02265434416296, 38.90089748801109, -77.9224564416296,
                39.00085678801109);
        BoundingBox expandedBounds = new BoundingBox();
        expandedBounds.expand(bounds,
                Double.parseDouble(HootProperties.getDefault("changesetBoundsExpansionFactorDeegrees")));
        return expandedBounds;
    }

    // This method adds nodes that are completely outside of the query bounds,
    // so that we can
    // make sure it that ways and relations created with them don't come back in
    // the response.
    // Eventually, this out of bounds data should be made part of the test
    // dataset created in
    // ChangesetResourceUtils. Since that involves updating *a lot* of tests, so
    // not doing it right
    // now.
    public static Set<Long> createNodesOutsideOfQueryBounds(final long changesetId, final BoundingBox queryBounds)
            throws Exception {
        Set<Long> nodeIds = new LinkedHashSet<Long>();
        nodeIds.add(Node.insertNew(changesetId, mapId, queryBounds.getMinLat() - 5, queryBounds.getMinLon() - 5, null,
                conn));
        nodeIds.add(Node.insertNew(changesetId, mapId, queryBounds.getMinLat() - 10, queryBounds.getMinLon() - 10, null,
                conn));
        return nodeIds;
    }

    /**
     * <<<<<<< HEAD Inserts a new way into the services database
     *
     * @param changesetId
     *            corresponding changeset ID for the way to be inserted
     * @param mapId
     *            corresponding map ID for the way to be inserted
     * @param nodeIds
     *            IDs for the collection of nodes to be associated with this way
     * @param tags
     *            element tags
     * @param dbConn
     *            JDBC Connection
     * @return ID of the newly created way
     * @throws Exception
     */
    public static long insertNewWay(final long changesetId, final long mapId, final List<Long> nodeIds,
            final Map<String, String> tags, Connection dbConn) throws Exception {
        long nextWayId = new SQLQuery(dbConn, DbUtils.getConfiguration(mapId))
                .uniqueResult(SQLExpressions.nextval(Long.class, "current_ways_id_seq"));
        insertNewWay(nextWayId, changesetId, mapId, nodeIds, tags, dbConn);

        return nextWayId;
    }

    /**
     * Inserts a new way into the services database with the specified ID;
     * useful for testing
     *
     * @param wayId
     *            ID to assign to the new way
     * @param changesetId
     *            corresponding changeset ID for the way to be inserted
     * @param mapId
     *            corresponding map ID for the way to be inserted
     * @param nodeIds
     *            collection of nodes to be associated with this way
     * @param tags
     *            element tags
     * @param dbConn
     *            JDBC Connection
     * @throws Exception
     *             see addNodeRefs
     */
    public static void insertNewWay(final long wayId, final long changesetId, final long mapId,
            final List<Long> nodeIds, final Map<String, String> tags, Connection dbConn) throws Exception {
        CurrentWays wayRecord = new CurrentWays();
        wayRecord.setChangesetId(changesetId);
        wayRecord.setId(wayId);

        final Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
        wayRecord.setTimestamp(now);
        wayRecord.setVersion(new Long(1));
        wayRecord.setVisible(true);
        if (tags != null && tags.size() > 0) {
            wayRecord.setTags(tags);
        }

        String strKv = "";

        if (tags != null) {
            Iterator it = tags.entrySet().iterator();
            while (it.hasNext()) {
                Map.Entry pairs = (Map.Entry) it.next();
                String key = "\"" + pairs.getKey() + "\"";
                String val = "\"" + pairs.getValue() + "\"";
                if (strKv.length() > 0) {
                    strKv += ",";
                }

                strKv += key + "=>" + val;
            }
        }
        String strTags = "'";
        strTags += strKv;
        strTags += "'";

        String POSTGRESQL_DRIVER = "org.postgresql.Driver";
        Statement stmt = null;
        try {
            Class.forName(POSTGRESQL_DRIVER);

            stmt = dbConn.createStatement();

            String sql = "INSERT INTO current_ways_" + mapId + "("
                    + "            id, changeset_id, \"timestamp\", visible, version, tags)" + " VALUES(" + wayId + ","
                    + changesetId + "," + "CURRENT_TIMESTAMP" + "," + "true" + "," + "1" + "," + strTags +

                    ")";
            stmt.executeUpdate(sql);
            addWayNodes(mapId, new Way(mapId, dbConn, wayRecord), nodeIds);

        }
        catch (Exception e) {
            throw new Exception("Error inserting node.");
        }
        finally {
            if (stmt != null)
                stmt.close();
        }
    }

    /*
     * Adds node refs to the way nodes services database table
     *
     * @param mapId
     * 
     * @param way
     * 
     * @param nodeIds a list of node ref IDs; This is a List, rather than a Set,
     * since the same node ID can be used for the first and last node ID in the
     * way nodes sequence for closed polygons.
     *
     * @throws Exception if the number of node refs is larger than the max
     * allowed number of way nodes, if any of the referenced nodes do not exist
     * in the services db, or if any of the referenced nodes are not set to be
     * visible in the services db
     */
    private static void addWayNodes(final long mapId, final Way way, final List<Long> nodeIds) throws Exception {
        CurrentWays wayRecord = (CurrentWays) way.getRecord();
        List<CurrentWayNodes> wayNodeRecords = new ArrayList<CurrentWayNodes>();
        long sequenceCtr = 1;
        for (long nodeId : nodeIds) {
            CurrentWayNodes wayNodeRecord = new CurrentWayNodes();
            wayNodeRecord.setNodeId(nodeId);
            wayNodeRecord.setSequenceId(sequenceCtr);
            wayNodeRecord.setWayId(wayRecord.getId());
            wayNodeRecords.add(wayNodeRecord);
            sequenceCtr++;
        }

        int maxRecordBatchSize = Integer.parseInt(HootProperties.getPropertyOrDefault("maxRecordBatchSize"));
        DbUtils.batchRecords(mapId, wayNodeRecords, QCurrentWayNodes.currentWayNodes, null, RecordBatchType.INSERT,
                conn, maxRecordBatchSize);
    }

    /*
     * Adds this relation's members to the services database
     * 
     * relations of size = 0 are allowed; see
     * http://wiki.openstreetmap.org/wiki/Empty_relations
     */
    private static void addRelationMembers(final long mapId, final Relation relation,
            final List<RelationMember> members) throws Exception {
        CurrentRelations relationRecord = (CurrentRelations) relation.getRecord();
        /*
         * final Set<Long> nodeIds = getMemberIdsByType(members,
         * ElementType.Node); if (!Element.allElementsExist(getMapId(),
         * ElementType.Node, nodeIds, conn)) { throw new Exception(
         * "Not all nodes exist specified for relation with ID: " +
         * relationRecord.getId()); } if
         * (!Element.allElementsVisible(getMapId(), ElementType.Node, nodeIds,
         * conn)) { throw new Exception(
         * "Not all nodes are visible for relation with ID: " +
         * relationRecord.getId()); } final Set<Long> wayIds =
         * getMemberIdsByType(members, ElementType.Way); if
         * (!Element.allElementsExist(getMapId(), ElementType.Way, wayIds,
         * conn)) { throw new Exception(
         * "Not all ways exist specified for relation with ID: " +
         * relationRecord.getId()); } if
         * (!Element.allElementsVisible(getMapId(), ElementType.Way, wayIds,
         * conn)) { throw new Exception(
         * "Not all ways are visible for relation with ID: " +
         * relationRecord.getId()); } final Set<Long> relationIds =
         * getMemberIdsByType(members, ElementType.Relation); if
         * (!Element.allElementsExist(getMapId(), ElementType.Relation,
         * relationIds, conn)) { throw new Exception(
         * "Not all relations exist specified for relation with ID: " +
         * relationRecord.getId()); } if
         * (!Element.allElementsVisible(getMapId(), ElementType.Relation,
         * relationIds, conn)) { throw new Exception(
         * "Not all relations are visible for relation with ID: " +
         * relationRecord.getId()); }
         */

        List<CurrentRelationMembers> memberRecords = new ArrayList<CurrentRelationMembers>();
        int sequenceCtr = 1;
        for (RelationMember member : members) {
            CurrentRelationMembers memberRecord = new CurrentRelationMembers();
            memberRecord.setMemberId(member.getId());
            memberRecord.setMemberRole(member.getRole());
            memberRecord.setMemberType(Element.elementEnumForElementType(member.getType()));
            memberRecord.setRelationId(relationRecord.getId());
            memberRecord.setSequenceId(sequenceCtr);
            memberRecords.add(memberRecord);
            sequenceCtr++;
        }

        int maxRecordBatchSize = Integer.parseInt(HootProperties.getPropertyOrDefault("maxRecordBatchSize"));
        DbUtils.batchRecords(mapId, memberRecords, QCurrentRelationMembers.currentRelationMembers, null,
                RecordBatchType.INSERT, conn, maxRecordBatchSize);
    }

    /**
     * Inserts a new relation into the services database
     *
     * @param changesetId
     *            corresponding changeset ID for the way to be inserted
     * @param mapId
     *            corresponding map ID for the element to be inserted
     * @param members
     *            the relation's members
     * @param tags
     *            element tags
     * @param dbConn
     *            JDBC Connection
     * @return ID of the newly created element
     * @throws Exception
     */
    public static long insertNewRelation(final long changesetId, final long mapId, final List<RelationMember> members,
            final Map<String, String> tags, Connection dbConn) throws Exception {
        final long nextRelationId = new SQLQuery(dbConn, DbUtils.getConfiguration(mapId))
                .uniqueResult(SQLExpressions.nextval(Long.class, "current_relations_id_seq"));
        insertNewRelation(nextRelationId, changesetId, mapId, members, tags, dbConn);
        return nextRelationId;
    }

    /**
     * Inserts a new relation into the services database with the specified ID;
     * useful for testing
     *
     * @param wayId
     *            ID to assign to the new way
     * @param changesetId
     *            corresponding changeset ID for the element to be inserted
     * @param mapId
     *            corresponding map ID for the element to be inserted
     * @param members
     *            the relation's members
     * @param tags
     *            element tags
     * @param dbConn
     *            JDBC Connection
     * @throws Exception
     */
    public static void insertNewRelation(final long relId, final long changesetId, final long mapId,
            final List<RelationMember> members, final Map<String, String> tags, Connection dbConn) throws Exception {
        CurrentRelations relationRecord = new CurrentRelations();
        relationRecord.setChangesetId(changesetId);
        relationRecord.setId(relId);
        final Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
        relationRecord.setTimestamp(now);
        relationRecord.setVersion(new Long(1));
        relationRecord.setVisible(true);
        if (tags != null && tags.size() > 0) {
            relationRecord.setTags(tags);
        }

        String strKv = "";
        if (tags != null) {
            Iterator it = tags.entrySet().iterator();
            while (it.hasNext()) {
                Map.Entry pairs = (Map.Entry) it.next();
                String key = "\"" + pairs.getKey() + "\"";
                String val = "\"" + pairs.getValue() + "\"";
                if (strKv.length() > 0) {
                    strKv += ",";
                }

                strKv += key + "=>" + val;
            }
        }
        String strTags = "'";
        strTags += strKv;
        strTags += "'";

        String POSTGRESQL_DRIVER = "org.postgresql.Driver";
        Statement stmt = null;
        try {
            Class.forName(POSTGRESQL_DRIVER);

            stmt = dbConn.createStatement();

            String sql = "INSERT INTO current_relations_" + mapId + "(\n"
                    + "            id, changeset_id, \"timestamp\", visible, version, tags)\n" + " VALUES(" + relId
                    + "," + changesetId + "," + "CURRENT_TIMESTAMP" + "," + "true" + "," + "1" + "," + strTags +

                    ")";
            stmt.executeUpdate(sql);
            addRelationMembers(mapId, new Relation(mapId, dbConn, relationRecord), members);

        }
        catch (Exception e) {
            throw new Exception("Error inserting node.");
        }

        finally {
            if (stmt != null)
                stmt.close();
        }
    }

    /**
     * Gets a total tag count for a specified element type belonging to a
     * specific map
     *
     * @param mapId
     *            ID of the map for which to retrieve the tag count
     * @param elementType
     *            element type for which to retrieve the tag count
     * @param dbConn
     *            JDBC Connection
     * @return a tag count
     * @throws Exception
     */
    public static long getTagCountForElementType(final long mapId, final ElementType elementType, Connection dbConn)
            throws Exception {
        final Element prototype = ElementFactory.getInstance().create(mapId, elementType, dbConn);
        List<?> records = new SQLQuery(dbConn, DbUtils.getConfiguration(mapId)).from(prototype.getElementTable())
                .list(prototype.getElementTable());
        long tagCount = 0;
        for (Object record : records) {
            PGobject tags = (PGobject) MethodUtils.invokeMethod(record, "getTags", new Object[] {});
            if (tags != null) {
                tagCount += PostgresUtils.postgresObjToHStore(tags).size();
            }
        }
        return tagCount;
    }
}
