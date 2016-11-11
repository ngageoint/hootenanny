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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.osm;

import static com.querydsl.core.group.GroupBy.groupBy;
import static hoot.services.HootProperties.CHANGESET_BOUNDS_EXPANSION_FACTOR_DEEGREES;
import static hoot.services.HootProperties.MAX_RECORD_BATCH_SIZE;
import static hoot.services.models.db.QChangesets.changesets;
import static hoot.services.models.db.QCurrentNodes.currentNodes;
import static hoot.services.models.db.QCurrentRelationMembers.currentRelationMembers;
import static hoot.services.models.db.QCurrentRelations.currentRelations;
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

import javax.xml.xpath.XPath;

import org.apache.commons.lang3.reflect.MethodUtils;
import org.apache.xpath.XPathAPI;
import org.joda.time.format.DateTimeFormat;
import org.joda.time.format.DateTimeFormatter;
import org.w3c.dom.Document;

import com.querydsl.core.types.Predicate;
import com.querydsl.core.types.dsl.BooleanExpression;
import com.querydsl.core.types.dsl.Expressions;
import com.querydsl.sql.RelationalPathBase;
import com.querydsl.sql.SQLExpressions;
import com.querydsl.sql.dml.SQLDeleteClause;
import com.querydsl.sql.dml.SQLInsertClause;
import com.querydsl.sql.dml.SQLUpdateClause;

import hoot.services.geo.BoundingBox;
import hoot.services.models.db.Changesets;
import hoot.services.models.db.CurrentNodes;
import hoot.services.models.db.CurrentRelationMembers;
import hoot.services.models.db.CurrentRelations;
import hoot.services.models.db.CurrentWayNodes;
import hoot.services.models.db.CurrentWays;
import hoot.services.models.db.QCurrentWayNodes;
import hoot.services.models.osm.Changeset;
import hoot.services.models.osm.Element;
import hoot.services.models.osm.Element.ElementType;
import hoot.services.models.osm.ElementFactory;
import hoot.services.models.osm.Relation;
import hoot.services.models.osm.RelationMember;
import hoot.services.models.osm.Way;
import hoot.services.utils.DbUtils;
import hoot.services.utils.DbUtils.RecordBatchType;
import hoot.services.utils.GeoUtils;
import hoot.services.utils.PostgresUtils;
import hoot.services.utils.QuadTileCalculator;
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
public class OSMTestUtils {
    private static long mapId = -1;
    private static long userId = -1;
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
    static BoundingBox createAfterModifiedTestChangesetBounds() throws Exception {
        return new BoundingBox(-79.02265434416296, 37.90089748801109, -77.0224564416296, 39.90085678801109);
    }

    /*
     * If a null bounds is specified, then the changeset gets no starting
     * bounds, as if it has newly been created with no data uploaded to it.
     */
    static long createTestChangeset(BoundingBox bounds) throws Exception {
        // default changeset we use for testing has 12 edits
        return createTestChangeset(bounds, 12);
    }

    // allow num changes to be passed in as a var for testing purposes
    static long createTestChangeset(BoundingBox bounds, int numChanges) throws Exception {
        long changesetId = Changeset.insertNew(getMapId(), getUserId(), new HashMap<String, String>());
        Changeset changeset = new Changeset(getMapId(), changesetId);

        if (bounds != null) {
            changeset.setBounds(bounds);
        }

        changeset.updateNumChanges(numChanges);
        changeset.updateExpiration();

        Changesets changesetPojo = createQuery(getMapId())
                .select(changesets)
                .from(changesets)
                .where(changesets.id.eq(changesetId))
                .fetchOne();

        assertNotNull(changesetPojo);
        assertEquals(numChanges, (long) changesetPojo.getNumChanges());
        assertEquals(getUserId(), (long) changesetPojo.getUserId());

        return changesetId;
    }

    /*
     * Technically, this should return a list to support closed polys, but since
     * the test data used in this method doesn't have closed polys and other
     * tests test for that, leaving it returned as a Set for now.
     */
    static Set<Long> createTestNodes(long changesetId, BoundingBox bounds) throws Exception {
        Map<String, String> tags = new HashMap<>();

        tags.put("key 1", "val 1");
        tags.put("key 2", "val 2");
        Set<Long> nodeIds = new LinkedHashSet<>();

        nodeIds.add(insertNew(changesetId, getMapId(), bounds.getMinLat(), bounds.getMinLon(), tags));
        tags.clear();

        nodeIds.add(insertNew(changesetId, getMapId(), bounds.getMaxLat(), bounds.getMaxLon(), tags));
        nodeIds.add(insertNew(changesetId, getMapId(), bounds.getMinLat(), bounds.getMinLon(), tags));

        tags.put("key 3", "val 3");
        nodeIds.add(insertNew(changesetId, getMapId(), bounds.getMinLat(), bounds.getMinLon(), tags));
        tags.clear();

        tags.put("key 4", "val 4");
        nodeIds.add(insertNew(changesetId, getMapId(), bounds.getMinLat(), bounds.getMinLon(), tags));
        tags.clear();

        return nodeIds;
    }

    static Set<Long> createTestWays(long changesetId, Set<Long> nodeIds) throws Exception {
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Map<String, String> tags = new HashMap<>();

        tags.put("key 1", "val 1");
        tags.put("key 2", "val 2");
        List<Long> wayNodeIds = new ArrayList<>();
        wayNodeIds.add(nodeIdsArr[0]);
        wayNodeIds.add(nodeIdsArr[1]);
        wayNodeIds.add(nodeIdsArr[4]);
        Set<Long> wayIds = new LinkedHashSet<>();
        wayIds.add(insertNewWay(changesetId, getMapId(), wayNodeIds, tags));
        tags.clear();
        wayNodeIds.clear();

        wayNodeIds.add(nodeIdsArr[2]);
        wayNodeIds.add(nodeIdsArr[1]);
        wayIds.add(insertNewWay(changesetId, getMapId(), wayNodeIds, null));
        wayNodeIds.clear();

        tags.put("key 3", "val 3");
        wayNodeIds.add(nodeIdsArr[0]);
        wayNodeIds.add(nodeIdsArr[1]);
        wayIds.add(insertNewWay(changesetId, getMapId(), wayNodeIds, tags));
        tags.clear();
        wayNodeIds.clear();

        return wayIds;
    }

    static Set<Long> createTestRelations(long changesetId, Set<Long> nodeIds, Set<Long> wayIds) throws Exception {
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        List<RelationMember> members = new ArrayList<>();

        members.add(new RelationMember(nodeIdsArr[0], ElementType.Node, "role1"));
        members.add(new RelationMember(wayIdsArr[1], ElementType.Way, "role3"));
        members.add(new RelationMember(wayIdsArr[0], ElementType.Way, "role2"));
        members.add(new RelationMember(nodeIdsArr[2], ElementType.Node));
        Map<String, String> tags = new HashMap<>();
        tags.put("key 1", "val 1");
        long firstRelationId = insertNewRelation(changesetId, getMapId(), members, tags);
        Set<Long> relationIds = new LinkedHashSet<>();
        relationIds.add(firstRelationId);
        tags.clear();
        members.clear();

        tags.put("key 2", "val 2");
        tags.put("key 3", "val 3");
        members.add(new RelationMember(nodeIdsArr[4], ElementType.Node, "role1"));
        members.add(new RelationMember(firstRelationId, ElementType.Relation, "role1"));
        relationIds.add(insertNewRelation(changesetId, getMapId(), members, tags));
        tags.clear();
        members.clear();

        tags.put("key 4", "val 4");
        members.add(new RelationMember(wayIdsArr[1], ElementType.Way));
        relationIds.add(insertNewRelation(changesetId, getMapId(), members, tags));
        tags.clear();
        members.clear();

        members.add(new RelationMember(nodeIdsArr[2], ElementType.Node, "role1"));
        relationIds.add(insertNewRelation(changesetId, getMapId(), members, null));
        members.clear();

        return relationIds;
    }

    // This is similar to createTestRelations but without any ways involved to
    // support certain tests.
    static Set<Long> createTestRelationsNoWays(long changesetId, Set<Long> nodeIds) throws Exception {
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        List<RelationMember> members = new ArrayList<>();

        members.add(new RelationMember(nodeIdsArr[0], ElementType.Node, "role1"));
        members.add(new RelationMember(nodeIdsArr[2], ElementType.Node));
        Map<String, String> tags = new HashMap<>();
        tags.put("key 1", "val 1");
        long firstRelationId = insertNewRelation(changesetId, getMapId(), members, tags);
        Set<Long> relationIds = new LinkedHashSet<>();
        relationIds.add(firstRelationId);
        tags.clear();
        members.clear();

        tags.put("key 2", "val 2");
        tags.put("key 3", "val 3");
        members.add(new RelationMember(nodeIdsArr[4], ElementType.Node, "role1"));
        members.add(new RelationMember(firstRelationId, ElementType.Relation, "role1"));
        relationIds.add(insertNewRelation(changesetId, getMapId(), members, tags));
        tags.clear();
        members.clear();

        tags.put("key 4", "val 4");
        members.add(new RelationMember(nodeIdsArr[2], ElementType.Node, "role1"));
        relationIds.add(insertNewRelation(changesetId, getMapId(), members, tags));
        tags.clear();
        members.clear();

        return relationIds;
    }

    static void verifyTestChangesetCreatedByRequest(Long changesetId) {
        Changesets changeset = createQuery(getMapId())
                .select(changesets)
                .from(changesets)
                .where(changesets.id.eq(changesetId))
                .fetchOne();

        assertNotNull(changeset);
        Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
        assertTrue(changeset.getCreatedAt().before(now));
        assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));
        assertEquals(new Double(GeoUtils.DEFAULT_COORD_VALUE), changeset.getMaxLat());
        assertEquals(new Double(GeoUtils.DEFAULT_COORD_VALUE), changeset.getMaxLon());
        assertEquals(new Double(GeoUtils.DEFAULT_COORD_VALUE), changeset.getMinLon());
        assertEquals(new Double(GeoUtils.DEFAULT_COORD_VALUE), changeset.getMinLat());
        assertEquals(new Integer(0), changeset.getNumChanges());
        assertEquals(new Long(getUserId()), changeset.getUserId());
    }

    static void verifyTestDataUnmodified(BoundingBox originalBounds, long changesetId,
            Set<Long> nodeIds, Set<Long> wayIds, Set<Long> relationIds) {
        verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds, true);
    }

    static void verifyTestDataUnmodified(BoundingBox originalBounds, long changesetId,
            Set<Long> nodeIds, Set<Long> wayIds, Set<Long> relationIds, boolean verifyTags) {
        verifyTestChangesetUnmodified(changesetId, originalBounds);
        verifyTestNodesUnmodified(nodeIds, changesetId, originalBounds, verifyTags);
        verifyTestWaysUnmodified(wayIds, nodeIds, changesetId, verifyTags);
        verifyTestRelationsUnmodified(relationIds, wayIds, nodeIds, changesetId, verifyTags);
    }

    static void verifyTestChangesetUnmodified(long changesetId) {
        Changesets changeset = createQuery(getMapId())
                .select(changesets)
                .from(changesets)
                .where(changesets.id.eq(changesetId))
                .fetchOne();

        assertNotNull(changeset);
        Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
        assertTrue(changeset.getCreatedAt().before(now));
        assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));
        assertEquals(new Integer(12), changeset.getNumChanges());
        assertEquals(new Long(getUserId()), changeset.getUserId());

        // make sure the changeset bounds has never been modified
        Changeset hootChangeset = new Changeset(getMapId(), changesetId);
        BoundingBox changesetBounds = hootChangeset.getBounds();
        BoundingBox defaultBounds = new BoundingBox();
        assertEquals(defaultBounds, changesetBounds);
    }

    // for testing purposes allow specifying numChanges as a variable
    static void verifyTestChangesetClosed(long changesetId, int numChanges) {
        Changesets changeset = createQuery(getMapId())
                .select(changesets)
                .from(changesets)
                .where(changesets.id.eq(changesetId))
                .fetchOne();

        assertNotNull(changeset);
        Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
        assertTrue(changeset.getCreatedAt().before(now));
        assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));
        assertTrue(changeset.getClosedAt().before(now));
        assertEquals(new Integer(numChanges), changeset.getNumChanges());
        assertEquals(new Long(getUserId()), changeset.getUserId());
    }

    static void verifyTestChangesetClosed(long changesetId) {
        // default changeset we use for testing has 12 edits
        verifyTestChangesetClosed(changesetId, 12);
    }

    static void verifyTestChangesetUnmodified(long changesetId, BoundingBox originalBounds) {
        Changesets changeset = createQuery(getMapId())
                .select(changesets)
                .from(changesets)
                .where(changesets.id.eq(changesetId))
                .fetchOne();

        assertNotNull(changeset);
        Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
        assertTrue(changeset.getCreatedAt().before(now));
        assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));
        assertEquals(new Integer(12), changeset.getNumChanges());
        assertEquals(new Long(getUserId()), changeset.getUserId());

        // make sure the changeset bounds wasn't updated
        Changeset hootChangeset = new Changeset(getMapId(), changesetId);
        BoundingBox changesetBounds = hootChangeset.getBounds();
        BoundingBox defaultBounds = new BoundingBox();

        // a change the size of the expansion factor is made automatically,
        // so the changeset's
        // bounds should be no larger than that
        defaultBounds.expand(originalBounds, Double.parseDouble(CHANGESET_BOUNDS_EXPANSION_FACTOR_DEEGREES));
        assertEquals(defaultBounds, changesetBounds);
    }

    static void verifyTestNodesUnmodified(Set<Long> nodeIds, long changesetId, BoundingBox originalBounds) {
        verifyTestNodesUnmodified(nodeIds, changesetId, originalBounds, true);
    }

    private static void verifyTestNodesUnmodified(Set<Long> nodeIds, long changesetId,
            BoundingBox originalBounds, boolean verifyTags) {

        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Map<Long, CurrentNodes> nodes = createQuery(getMapId())
                .from(currentNodes)
                .where((currentNodes.changesetId.eq(changesetId)))
                .transform(groupBy(currentNodes.id).as(currentNodes));

        assertEquals(5, nodes.size());

        CurrentNodes nodeRecord = nodes.get(nodeIdsArr[0]);
        assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
        assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
        assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
        assertEquals(nodeIdsArr[0], nodeRecord.getId());
        assertEquals(new Long(1), nodeRecord.getVersion());

        if (verifyTags) {
            Map<String, String> tags = PostgresUtils.postgresObjToHStore(nodeRecord.getTags());
            assertEquals(2, tags.size());
            // System.out.println(tags.get("key 1"));
            assertEquals("val 1", tags.get("key 1"));
            // System.out.println(tags.get("key 2"));
            assertEquals("val 2", tags.get("key 2"));
        }

        nodeRecord = nodes.get(nodeIdsArr[1]);
        assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
        assertEquals(new Double(originalBounds.getMaxLat()), nodeRecord.getLatitude());
        assertEquals(new Double(originalBounds.getMaxLon()), nodeRecord.getLongitude());
        assertEquals(nodeIdsArr[1], nodeRecord.getId());
        assertEquals(new Long(1), nodeRecord.getVersion());
        if (verifyTags) {
            assertTrue((nodeRecord.getTags() == null)
                    || PostgresUtils.postgresObjToHStore(nodeRecord.getTags()).isEmpty());
        }

        nodeRecord = nodes.get(nodeIdsArr[2]);
        assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
        assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
        assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
        assertEquals(nodeIdsArr[2], nodeRecord.getId());
        assertEquals(new Long(1), nodeRecord.getVersion());
        if (verifyTags) {
            assertTrue((nodeRecord.getTags() == null)
                    || PostgresUtils.postgresObjToHStore(nodeRecord.getTags()).isEmpty());
        }

        nodeRecord = nodes.get(nodeIdsArr[3]);
        assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
        assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
        assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
        assertEquals(nodeIdsArr[3], nodeRecord.getId());
        assertEquals(new Long(1), nodeRecord.getVersion());
        if (verifyTags) {
            Map<String, String> tags = PostgresUtils.postgresObjToHStore(nodeRecord.getTags());
            assertEquals(1, tags.size());
            assertEquals("val 3", tags.get("key 3"));
        }

        nodeRecord = nodes.get(nodeIdsArr[4]);
        assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
        assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
        assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
        assertEquals(nodeIdsArr[4], nodeRecord.getId());
        assertEquals(new Long(1), nodeRecord.getVersion());
        if (verifyTags) {
            Map<String, String> tags = PostgresUtils.postgresObjToHStore(nodeRecord.getTags());
            assertEquals(1, tags.size());
            assertEquals("val 4", tags.get("key 4"));
        }
    }

    static void verifyTestWaysUnmodified(Set<Long> wayIds, Set<Long> nodeIds, long changesetId) {
        verifyTestWaysUnmodified(wayIds, nodeIds, changesetId, true);
    }

    private static void verifyTestWaysUnmodified(Set<Long> wayIds, Set<Long> nodeIds, long changesetId,
            boolean verifyTags) {
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Map<Long, CurrentWays> ways = createQuery(getMapId())
                .from(currentWays)
                .where((currentWays.changesetId.eq(changesetId)))
                .transform(groupBy(currentWays.id).as(currentWays));

        assertEquals(3, ways.size());

        CurrentWays wayRecord = ways.get(wayIdsArr[0]);
        assertEquals(new Long(changesetId), wayRecord.getChangesetId());
        assertEquals(wayIdsArr[0], wayRecord.getId());
        assertEquals(new Long(1), wayRecord.getVersion());
        QCurrentWayNodes currentWayNodes = QCurrentWayNodes.currentWayNodes;

        List<CurrentWayNodes> wayNodes = createQuery(getMapId())
                .select(QCurrentWayNodes.currentWayNodes)
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
        if (verifyTags) {
            Map<String, String> tags = PostgresUtils.postgresObjToHStore(wayRecord.getTags());
            assertEquals(2, tags.size());
            assertEquals("val 1", tags.get("key 1"));
            assertEquals("val 2", tags.get("key 2"));
        }

        wayRecord = ways.get(wayIdsArr[1]);
        assertEquals(new Long(changesetId), wayRecord.getChangesetId());
        assertEquals(wayIdsArr[1], wayRecord.getId());
        assertEquals(new Long(1), wayRecord.getVersion());

        wayNodes = createQuery(getMapId())
                .select(QCurrentWayNodes.currentWayNodes)
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

        if (verifyTags) {
            assertTrue((wayRecord.getTags() == null)
                    || PostgresUtils.postgresObjToHStore(wayRecord.getTags()).isEmpty());
        }

        wayRecord = ways.get(wayIdsArr[2]);
        assertEquals(new Long(changesetId), wayRecord.getChangesetId());
        assertEquals(wayIdsArr[2], wayRecord.getId());
        assertEquals(new Long(1), wayRecord.getVersion());

        wayNodes = createQuery(getMapId())
                .select(QCurrentWayNodes.currentWayNodes)
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
        if (verifyTags) {
            Map<String, String> tags = PostgresUtils.postgresObjToHStore(wayRecord.getTags());
            assertEquals(1, tags.size());
            assertEquals("val 3", tags.get("key 3"));
        }
    }

    static void verifyTestRelationsUnmodified(Set<Long> relationIds, Set<Long> wayIds,
            Set<Long> nodeIds, long changesetId) {
        verifyTestRelationsUnmodified(relationIds, wayIds, nodeIds, changesetId, true);
    }

    private static void verifyTestRelationsUnmodified(Set<Long> relationIds, Set<Long> wayIds,
            Set<Long> nodeIds, long changesetId, boolean verifyTags) {
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Map<Long, CurrentRelations> relations = createQuery(getMapId())
                .from(currentRelations)
                .where((currentRelations.changesetId.eq(changesetId)))
                .transform(groupBy(currentRelations.id).as(currentRelations));

        assertEquals(4, relations.size());

        CurrentRelations relationRecord = relations.get(relationIdsArr[0]);
        assertEquals(new Long(changesetId), relationRecord.getChangesetId());
        assertEquals(relationIdsArr[0], relationRecord.getId());
        assertEquals(new Long(1), relationRecord.getVersion());

        List<CurrentRelationMembers> relationMembers = createQuery(getMapId())
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
        if (verifyTags) {
            Map<String, String> tags = PostgresUtils.postgresObjToHStore(relationRecord.getTags());
            assertEquals(1, tags.size());
            assertEquals("val 1", tags.get("key 1"));
        }

        relationRecord = relations.get(relationIdsArr[1]);
        assertEquals(new Long(changesetId), relationRecord.getChangesetId());
        assertEquals(relationIdsArr[1], relationRecord.getId());
        assertEquals(new Long(1), relationRecord.getVersion());

        relationMembers = createQuery(getMapId())
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
        if (verifyTags) {
            Map<String, String> tags = PostgresUtils.postgresObjToHStore(relationRecord.getTags());
            assertEquals(2, tags.size());
            assertEquals("val 2", tags.get("key 2"));
            assertEquals("val 3", tags.get("key 3"));
        }

        relationRecord = relations.get(relationIdsArr[2]);
        assertEquals(new Long(changesetId), relationRecord.getChangesetId());
        assertEquals(relationIdsArr[2], relationRecord.getId());
        assertEquals(new Long(1), relationRecord.getVersion());

        relationMembers = createQuery(getMapId())
                .select(currentRelationMembers)
                .from(currentRelationMembers)
                .where(currentRelationMembers.relationId.eq(relationIdsArr[2]))
                .orderBy(currentRelationMembers.sequenceId.asc())
                .fetch();

        assertEquals(1, relationMembers.size());
        member = relationMembers.get(0);
        assertEquals(relationRecord.getId(), member.getRelationId());
        assertEquals(DbUtils.nwr_enum.way, member.getMemberType());
        assertEquals("", member.getMemberRole());
        assertEquals(new Integer(1), member.getSequenceId());

        assertEquals(wayIdsArr[1], member.getMemberId());
        if (verifyTags) {
            Map<String, String> tags = PostgresUtils.postgresObjToHStore(relationRecord.getTags());
            assertEquals(1, tags.size());
            assertEquals("val 4", tags.get("key 4"));
        }

        relationRecord = relations.get(relationIdsArr[3]);
        assertEquals(new Long(changesetId), relationRecord.getChangesetId());
        assertEquals(relationIdsArr[3], relationRecord.getId());
        assertEquals(new Long(1), relationRecord.getVersion());

        relationMembers = createQuery(getMapId())
                .select(currentRelationMembers)
                .from(currentRelationMembers)
                .where(currentRelationMembers.relationId.eq(relationIdsArr[3]))
                .orderBy(currentRelationMembers.sequenceId.asc())
                .fetch();

        assertEquals(1, relationMembers.size());
        member = relationMembers.get(0);
        assertEquals(relationRecord.getId(), member.getRelationId());
        assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
        assertEquals("role1", member.getMemberRole());
        assertEquals(new Integer(1), member.getSequenceId());

        assertEquals(nodeIdsArr[2], member.getMemberId());
        if (verifyTags) {
            assertTrue((relationRecord.getTags() == null)
                    || PostgresUtils.postgresObjToHStore(relationRecord.getTags()).isEmpty());
        }
    }

    static void verifyTestRelationsNoWaysUnmodified(Set<Long> relationIds, Set<Long> nodeIds,
            long changesetId, boolean verifyTags) {
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Map<Long, CurrentRelations> relations =
                createQuery(getMapId())
                        .from(currentRelations)
                        .where((currentRelations.changesetId.eq(changesetId)))
                        .transform(groupBy(currentRelations.id).as(currentRelations));

        assertEquals(3, relations.size());

        CurrentRelations relationRecord = relations.get(relationIdsArr[0]);
        assertEquals(new Long(changesetId), relationRecord.getChangesetId());
        assertEquals(relationIdsArr[0], relationRecord.getId());
        assertEquals(new Long(1), relationRecord.getVersion());

        List<CurrentRelationMembers> relationMembers =
                createQuery(getMapId())
                        .select(currentRelationMembers)
                        .from(currentRelationMembers)
                        .where(currentRelationMembers.relationId.eq(relationIdsArr[0]))
                        .orderBy(currentRelationMembers.sequenceId.asc())
                        .fetch();

        assertEquals(2, relationMembers.size());
        CurrentRelationMembers member = relationMembers.get(0);
        assertEquals(relationRecord.getId(), member.getRelationId());
        assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
        assertEquals("role1", member.getMemberRole());
        assertEquals(new Integer(1), member.getSequenceId());

        assertEquals(nodeIdsArr[0], member.getMemberId());
        member = relationMembers.get(1);
        assertEquals(relationRecord.getId(), member.getRelationId());
        assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
        assertEquals("", member.getMemberRole());
        assertEquals(new Integer(2), member.getSequenceId());

        assertEquals(nodeIdsArr[2], member.getMemberId());
        if (verifyTags) {
            Map<String, String> tags = PostgresUtils.postgresObjToHStore(relationRecord.getTags());
            assertEquals(1, tags.size());
            assertEquals("val 1", tags.get("key 1"));
        }

        relationRecord = relations.get(relationIdsArr[1]);
        assertEquals(new Long(changesetId), relationRecord.getChangesetId());
        assertEquals(relationIdsArr[1], relationRecord.getId());
        assertEquals(new Long(1), relationRecord.getVersion());

        relationMembers = createQuery(getMapId())
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
        if (verifyTags) {
            Map<String, String> tags = PostgresUtils.postgresObjToHStore(relationRecord.getTags());
            assertEquals(2, tags.size());
            assertEquals("val 2", tags.get("key 2"));
            assertEquals("val 3", tags.get("key 3"));
        }

        relationRecord = relations.get(relationIdsArr[2]);
        assertEquals(new Long(changesetId), relationRecord.getChangesetId());
        assertEquals(relationIdsArr[2], relationRecord.getId());
        assertEquals(new Long(1), relationRecord.getVersion());

        relationMembers = createQuery(getMapId())
                .select(currentRelationMembers)
                .from(currentRelationMembers)
                .where(currentRelationMembers.relationId.eq(relationIdsArr[2]))
                .orderBy(currentRelationMembers.sequenceId.asc())
                .fetch();

        assertEquals(1, relationMembers.size());
        member = relationMembers.get(0);
        assertEquals(relationIdsArr[2], member.getRelationId());
        assertEquals(DbUtils.nwr_enum.node, member.getMemberType());
        assertEquals("role1", member.getMemberRole());
        assertEquals(new Integer(1), member.getSequenceId());

        assertEquals(nodeIdsArr[2], member.getMemberId());
        if (verifyTags) {
            Map<String, String> tags = PostgresUtils.postgresObjToHStore(relationRecord.getTags());
            assertEquals(1, tags.size());
            assertEquals("val 4", tags.get("key 4"));
        }
    }

    static void verifyOsmHeader(Document responseData) throws Exception {
        XPath xpath = XmlUtils.createXPath();
        assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm").getLength());
        assertEquals("0.6", xpath.evaluate("//osm[1]/@version", responseData));
        assertNotNull(xpath.evaluate("//osm[1]/@generator", responseData));
        assertNotNull(xpath.evaluate("//osm[1]/@copyright", responseData));
        assertNotNull(xpath.evaluate("//osm[1]/@attribution", responseData));
        assertNotNull(xpath.evaluate("//osm[1]/@license", responseData));
    }

    static void verifyBounds(Document responseData, BoundingBox expectedBounds) throws Exception {
        XPath xpath = XmlUtils.createXPath();
        assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm/bounds").getLength());
        assertEquals(expectedBounds.getMinLat(),
                Double.parseDouble(xpath.evaluate("//osm/bounds[1]/@minlat", responseData)), 0.0);
        assertEquals(expectedBounds.getMinLon(),
                Double.parseDouble(xpath.evaluate("//osm/bounds[1]/@minlon", responseData)), 0.0);
        assertEquals(expectedBounds.getMaxLat(),
                Double.parseDouble(xpath.evaluate("//osm/bounds[1]/@maxlat", responseData)), 0.0);
        assertEquals(expectedBounds.getMaxLon(),
                Double.parseDouble(xpath.evaluate("//osm/bounds[1]/@maxlon", responseData)), 0.0);
    }

    static void verifyNode(Document responseData, long index, String expectedId,
            long changesetId, double lat, double lon, boolean multiLayerUniqueElementIds) throws Exception {
        XPath xpath = XmlUtils.createXPath();
        if (!multiLayerUniqueElementIds) {
            assertEquals(Long.parseLong(expectedId),
                    Long.parseLong(xpath.evaluate("//osm/node[" + index + "]/@id", responseData)));
        }
        else {
            assertEquals(expectedId, xpath.evaluate("//osm/node[" + index + "]/@id", responseData));
        }

        assertTrue(Boolean.parseBoolean(xpath.evaluate("//osm/node[" + index + "]/@visible", responseData)));
        assertEquals(1, Long.parseLong(xpath.evaluate("//osm/node[" + index + "]/@version", responseData)));
        assertEquals(changesetId, Long.parseLong(xpath.evaluate("//osm/node[" + index + "]/@changeset", responseData)));

        String timeStampStr = xpath.evaluate("//osm/node[" + index + "]/@timestamp", responseData);
        // remove the millseconds portion of the string, since it can be of
        // variable length and cause
        // seemingly random problems when parsed
        if (timeStampStr.contains(".")) {
            timeStampStr = timeStampStr.split("\\.")[0];
        }

//        Timestamp nodeTimestamp = new Timestamp(timeFormatter.parseDateTime(timeStampStr).getMillis());
//        Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
//        assertTrue(nodeTimestamp.before(now));
        assertEquals("user-with-id-" + getUserId(), xpath.evaluate("//osm/node[" + index + "]/@user", responseData));
        assertEquals(getUserId(), Long.parseLong(xpath.evaluate("//osm/node[" + index + "]/@uid", responseData)));
        assertEquals(lat, Double.parseDouble(xpath.evaluate("//osm/node[" + index + "]/@lat", responseData)), 0.0);
        assertEquals(lon, Double.parseDouble(xpath.evaluate("//osm/node[" + index + "]/@lon", responseData)), 0.0);
    }

    static void verifyWay(Document responseData, long index, String expectedId,
            long changesetId, Set<Long> expectedWayNodeIds, boolean multiLayerUniqueElementIds) throws Exception {
        XPath xpath = XmlUtils.createXPath();

        if (!multiLayerUniqueElementIds) {
            assertEquals(Long.parseLong(expectedId),
                    Long.parseLong(xpath.evaluate("//osm/way[" + index + "]/@id", responseData)));
        }
        else {
            assertEquals(expectedId, xpath.evaluate("//osm/way[" + index + "]/@id", responseData));
        }

        assertTrue(Boolean.parseBoolean(xpath.evaluate("//osm/way[" + index + "]/@visible", responseData)));
        assertEquals(1, Long.parseLong(xpath.evaluate("//osm/way[" + index + "]/@version", responseData)));
        assertEquals(changesetId, Long.parseLong(xpath.evaluate("//osm/way[" + index + "]/@changeset", responseData)));

        String timeStampStr = xpath.evaluate("//osm/way[" + index + "]/@timestamp", responseData);
        // remove the millseconds portion of the string, since it can be of
        // variable length and cause
        // seemingly random problems when parsed
        if (timeStampStr.contains(".")) {
            timeStampStr = timeStampStr.split("\\.")[0];
        }

//        Timestamp wayTimestamp = new Timestamp(timeFormatter.parseDateTime(timeStampStr).getMillis());
//        Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
//        assertTrue(wayTimestamp.before(now));
        assertEquals("user-with-id-" + getUserId(), xpath.evaluate("//osm/way[" + index + "]/@user", responseData));
        assertEquals(getUserId(), Long.parseLong(xpath.evaluate("//osm/way[" + index + "]/@uid", responseData)));

        if (expectedWayNodeIds != null) {
            Long[] expectedWayNodeIdsArr = expectedWayNodeIds.toArray(new Long[expectedWayNodeIds.size()]);
            assertEquals(expectedWayNodeIdsArr.length,
                    XPathAPI.selectNodeList(responseData, "//osm/way[" + index + "]/nd").getLength());
            for (int i = 0; i < expectedWayNodeIdsArr.length; i++) {
                assertEquals((long) expectedWayNodeIdsArr[i], Long.parseLong(
                        xpath.evaluate("//osm/way[" + index + "]/nd[" + (i + 1) + "]/@ref", responseData)));
            }
        }
    }

    static void verifyRelation(Document responseData, long index, String expectedId,
            long changesetId, List<RelationMember> expectedMembers, boolean multiLayerUniqueElementIds)
        throws Exception {
        XPath xpath = XmlUtils.createXPath();

        if (!multiLayerUniqueElementIds) {
            assertEquals(Long.parseLong(expectedId),
                    Long.parseLong(xpath.evaluate("//osm/relation[" + index + "]/@id", responseData)));
        }
        else {
            assertEquals(expectedId, xpath.evaluate("//osm/relation[" + index + "]/@id", responseData));
        }

        assertTrue(Boolean.parseBoolean(xpath.evaluate("//osm/relation[" + index + "]/@visible", responseData)));
        assertEquals(1, Long.parseLong(xpath.evaluate("//osm/relation[" + index + "]/@version", responseData)));
        assertEquals(changesetId,
                Long.parseLong(xpath.evaluate("//osm/relation[" + index + "]/@changeset", responseData)));

        String timeStampStr = xpath.evaluate("//osm/relation[" + index + "]/@timestamp", responseData);

        // remove the millseconds portion of the string, since it can be of
        // variable length and cause
        // seemingly random problems when parsed
        if (timeStampStr.contains(".")) {
            timeStampStr = timeStampStr.split("\\.")[0];
        }

//        Timestamp relationTimestamp = new Timestamp(timeFormatter.parseDateTime(timeStampStr).getMillis());
//        Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
//        assertTrue(relationTimestamp.before(now));
        assertEquals("user-with-id-" + getUserId(), xpath.evaluate("//osm/relation[" + index + "]/@user", responseData));
        assertEquals(getUserId(), Long.parseLong(xpath.evaluate("//osm/relation[" + index + "]/@uid", responseData)));

        if (expectedMembers != null) {
            assertEquals(expectedMembers.size(),
                    XPathAPI.selectNodeList(responseData, "//osm/relation[" + index + "]/member").getLength());
            int memberCtr = 1;
            for (RelationMember member : expectedMembers) {
                assertEquals(member.getId(), Long.parseLong(xpath
                        .evaluate("//osm/relation[" + index + "]/member[" + memberCtr + "]/@ref", responseData)));
                assertEquals(member.getRole(), xpath
                        .evaluate("//osm/relation[" + index + "]/member[" + memberCtr + "]/@role", responseData));
                assertEquals(member.getType().toString().toLowerCase(), xpath
                        .evaluate("//osm/relation[" + index + "]/member[" + memberCtr + "]/@type", responseData));

                memberCtr++;
            }
        }
    }

    static void closeChangeset(long mapId, long changesetId) {
        Changesets changeset = createQuery(mapId)
                .select(changesets)
                .from(changesets)
                .where(changesets.id.eq(changesetId))
                .fetchOne();

        Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
        changeset.setClosedAt(now);

        createQuery(mapId).update(changesets)
                .where(changesets.id.eq(changeset.getId()))
                .populate(changeset).execute();
    }

    static BoundingBox createTestQueryBounds() throws Exception {
        BoundingBox bounds = new BoundingBox(-78.02265434416296, 38.90089748801109, -77.9224564416296,
                39.00085678801109);
        BoundingBox expandedBounds = new BoundingBox();
        expandedBounds.expand(bounds, Double.parseDouble(CHANGESET_BOUNDS_EXPANSION_FACTOR_DEEGREES));
        return expandedBounds;
    }

    // This method adds nodes that are completely outside of the query bounds,
    // so that we can make sure it that ways and relations created with them don't come back in
    // the response.  Eventually, this out of bounds data should be made part of the test
    // dataset created in ChangesetResourceUtils. Since that involves updating *a lot* of tests, so
    // not doing it right now.
    static Set<Long> createNodesOutsideOfQueryBounds(long changesetId, BoundingBox queryBounds)
            throws Exception {
        Set<Long> nodeIds = new LinkedHashSet<>();
        nodeIds.add(insertNew(changesetId, getMapId(), queryBounds.getMinLat() - 5, queryBounds.getMinLon() - 5, new HashMap<String, String>()));
        nodeIds.add(insertNew(changesetId, getMapId(), queryBounds.getMinLat() - 10, queryBounds.getMinLon() - 10, new HashMap<String, String>()));
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
     * @return ID of the newly created way
     * @throws Exception
     */
    static long insertNewWay(long changesetId, long mapId, List<Long> nodeIds, Map<String, String> tags)
            throws Exception {

        long nextWayId = createQuery(mapId)
                .select(SQLExpressions.nextval(Long.class, "current_ways_id_seq"))
                .from()
                .fetchOne();

        insertNewWay(nextWayId, changesetId, mapId, nodeIds, tags);

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
     * @throws Exception
     *             see addNodeRefs
     */
    static void insertNewWay(long wayId, long changesetId, long mapId,
            List<Long> nodeIds, Map<String, String> tags) throws Exception {
        CurrentWays wayRecord = new CurrentWays();
        wayRecord.setChangesetId(changesetId);
        wayRecord.setId(wayId);

        Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
        wayRecord.setTimestamp(now);
        wayRecord.setVersion(1L);
        wayRecord.setVisible(true);
        if ((tags != null) && (!tags.isEmpty())) {
            wayRecord.setTags(tags);
        }

        createQuery(mapId).insert(currentWays)
                .columns(currentWays.id, currentWays.changesetId, currentWays.timestamp,
                         currentWays.visible, currentWays.version, currentWays.tags)
                .values(wayId, changesetId, Expressions.stringTemplate("CURRENT_TIMESTAMP"), true, 1, tags)
                .execute();

        addWayNodes(mapId, new Way(mapId, wayRecord), nodeIds);
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
    private static void addWayNodes(long mapId, Way way, List<Long> nodeIds) throws Exception {
        CurrentWays wayRecord = (CurrentWays) way.getRecord();
        List<CurrentWayNodes> wayNodeRecords = new ArrayList<>();
        long sequenceCtr = 1;
        for (long nodeId : nodeIds) {
            CurrentWayNodes wayNodeRecord = new CurrentWayNodes();
            wayNodeRecord.setNodeId(nodeId);
            wayNodeRecord.setSequenceId(sequenceCtr);
            wayNodeRecord.setWayId(wayRecord.getId());
            wayNodeRecords.add(wayNodeRecord);
            sequenceCtr++;
        }

        int maxRecordBatchSize = Integer.parseInt(MAX_RECORD_BATCH_SIZE);
        batchRecords(mapId, wayNodeRecords, QCurrentWayNodes.currentWayNodes, null, RecordBatchType.INSERT, maxRecordBatchSize);
    }

    /*
     * Adds this relation's members to the services database
     * 
     * relations of size = 0 are allowed; see
     * http://wiki.openstreetmap.org/wiki/Empty_relations
     */
    private static void addRelationMembers(long mapId, Relation relation, List<RelationMember> members)
            throws Exception {
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

        List<CurrentRelationMembers> memberRecords = new ArrayList<>();
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

        int maxRecordBatchSize = Integer.parseInt(MAX_RECORD_BATCH_SIZE);
        batchRecords(mapId, memberRecords, currentRelationMembers, null, RecordBatchType.INSERT, maxRecordBatchSize);
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
     * @return ID of the newly created element
     * @throws Exception
     */
    static long insertNewRelation(long changesetId, long mapId, List<RelationMember> members,
            Map<String, String> tags) throws Exception {

        long nextRelationId = createQuery(mapId)
                .select(SQLExpressions.nextval(Long.class, "current_relations_id_seq"))
                .from()
                .fetchOne();

        insertNewRelation(nextRelationId, changesetId, mapId, members, tags);

        return nextRelationId;
    }

    /**
     * Inserts a new relation into the services database with the specified ID;
     * useful for testing
     *
     * @param relId
     *            ID to assign to the new way
     * @param changesetId
     *            corresponding changeset ID for the element to be inserted
     * @param mapId
     *            corresponding map ID for the element to be inserted
     * @param members
     *            the relation's members
     * @param tags
     *            element tags
     * @throws Exception
     */
    static void insertNewRelation(long relId, long changesetId, long mapId,
            List<RelationMember> members, Map<String, String> tags) throws Exception {

        CurrentRelations relationRecord = new CurrentRelations();
        relationRecord.setChangesetId(changesetId);
        relationRecord.setId(relId);

        Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
        relationRecord.setTimestamp(now);
        relationRecord.setVersion(1L);
        relationRecord.setVisible(true);
        if ((tags != null) && (!tags.isEmpty())) {
            relationRecord.setTags(tags);
        }

        createQuery(mapId).insert(currentRelations)
                .columns(currentRelations.id, currentRelations.changesetId, currentRelations.timestamp,
                        currentRelations.visible, currentRelations.version, currentRelations.tags)
                .values(relId, changesetId, Expressions.stringTemplate("CURRENT_TIMESTAMP"), true, 1, tags)
                .execute();

        addRelationMembers(mapId, new Relation(mapId, relationRecord), members);
    }

    /**
     * Gets a total tag count for a specified element type belonging to a
     * specific map
     *
     * @param mapId
     *            ID of the map for which to retrieve the tag count
     * @param elementType
     *            element type for which to retrieve the tag count
     * @return a tag count
     * @throws Exception
     */
    static long getTagCountForElementType(long mapId, ElementType elementType) throws Exception {
        Element prototype = ElementFactory.create(mapId, elementType);

        List<?> records = createQuery(mapId)
                .select(prototype.getElementTable())
                .from(prototype.getElementTable())
                .fetch();

        long tagCount = 0;
        for (Object record : records) {
            Object tags = MethodUtils.invokeMethod(record, "getTags");
            if (tags != null) {
                tagCount += PostgresUtils.postgresObjToHStore(tags).size();
            }
        }
        return tagCount;
    }

    public static long getMapId() {
        return mapId;
    }

    public static void setMapId(long mapId) {
        OSMTestUtils.mapId = mapId;
    }

    public static long getUserId() {
        return userId;
    }

    public static void setUserId(long userId) {
        OSMTestUtils.userId = userId;
    }

    /**
     * Inserts a new node into the services database
     *
     * @param changesetId
     *            corresponding changeset ID for the node to be inserted
     * @param mapId
     *            corresponding map ID for the node to be inserted
     * @param latitude
     *            latitude coordinate for the node to be inserted
     * @param longitude
     *            longitude coordinate for the node to be inserted
     * @param tags
     *            element tags
     * @return ID of the newly created node
     */
    static long insertNew(long changesetId, long mapId, double latitude, double longitude, Map<String, String> tags) {
        long nextNodeId = createQuery(mapId)
                .select(SQLExpressions.nextval(Long.class, "current_nodes_id_seq"))
                .fetchOne();

        insertNew(nextNodeId, changesetId, mapId, latitude, longitude, tags);

        return nextNodeId;
    }

    /**
     * Inserts a new node into the services database with the specified ID;
     * useful for testing
     *
     * @param nodeId
     *            ID to assign to the new node
     * @param changesetId
     *            corresponding changeset ID for the node to be inserted
     * @param mapId
     *            corresponding map ID for the node to be inserted
     * @param latitude
     *            latitude coordinate for the node to be inserted
     * @param longitude
     *            longitude coordinate for the node to be inserted
     * @param tags
     *            element tags
     */
    static void insertNew(long nodeId, long changesetId, long mapId, double latitude, double longitude,
            Map<String, String> tags) {

        createQuery(mapId).insert(currentNodes)
                .columns(currentNodes.id, currentNodes.latitude, currentNodes.longitude, currentNodes.changesetId,
                        currentNodes.visible, currentNodes.tile, currentNodes.version, currentNodes.tags)
                .values(nodeId, latitude, longitude, changesetId,
                        Boolean.TRUE, QuadTileCalculator.tileForPoint(latitude, longitude), 1L, tags)
                .execute();
    }

    private static long batchRecords(long mapId, List<?> records, RelationalPathBase<?> t,
            List<List<BooleanExpression>> predicateslist, RecordBatchType recordBatchType, int maxRecordBatchSize) {
        if (recordBatchType == RecordBatchType.INSERT) {
            SQLInsertClause insert = createQuery(mapId).insert(t);
            long nBatch = 0;
            for (int i = 0; i < records.size(); i++) {
                Object oRec = records.get(i);
                insert.populate(oRec).addBatch();
                nBatch++;

                if ((maxRecordBatchSize > -1) && (i > 0)) {
                    if ((i % maxRecordBatchSize) == 0) {
                        insert.execute();

                        insert = createQuery(mapId).insert(t);
                        nBatch = 0;
                    }
                }
            }

            if (nBatch > 0) {
                return insert.execute();
            }

            return 0;
        }
        else if (recordBatchType == RecordBatchType.UPDATE) {
            SQLUpdateClause update = createQuery(mapId).update(t);
            long nBatchUpdate = 0;
            for (int i = 0; i < records.size(); i++) {
                Object oRec = records.get(i);

                List<BooleanExpression> predicates = predicateslist.get(i);

                BooleanExpression[] params = new BooleanExpression[predicates.size()];

                for (int j = 0; j < predicates.size(); j++) {
                    params[j] = predicates.get(j);
                }

                update.populate(oRec).where((Predicate[]) params).addBatch();
                nBatchUpdate++;

                if ((maxRecordBatchSize > -1) && (i > 0)) {
                    if ((i % maxRecordBatchSize) == 0) {
                        update.execute();

                        update = createQuery(mapId).update(t);
                        nBatchUpdate = 0;
                    }
                }
            }

            if (nBatchUpdate > 0) {
                return update.execute();
            }

            return 0;
        }
        else { //(recordBatchType == RecordBatchType.DELETE)
            SQLDeleteClause delete = createQuery(mapId).delete(t);
            long nBatchDel = 0;
            for (int i = 0; i < records.size(); i++) {
                List<BooleanExpression> predicates = predicateslist.get(i);

                BooleanExpression[] params = new BooleanExpression[predicates.size()];

                for (int j = 0; j < predicates.size(); j++) {
                    params[j] = predicates.get(j);
                }

                delete.where((Predicate[]) params).addBatch();
                nBatchDel++;
                if ((maxRecordBatchSize > -1) && (i > 0)) {
                    if ((i % maxRecordBatchSize) == 0) {
                        delete.execute();

                        delete = createQuery(mapId).delete(t);
                        nBatchDel = 0;
                    }
                }
            }

            if (nBatchDel > 0) {
                return delete.execute();
            }

            return 0;
        }
    }
}
