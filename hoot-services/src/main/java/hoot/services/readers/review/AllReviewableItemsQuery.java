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
package hoot.services.readers.review;

import static hoot.services.HootProperties.MAX_QUERY_NODES;

import java.sql.Connection;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.mysema.query.Tuple;
import com.mysema.query.sql.SQLQuery;
import com.mysema.query.sql.SQLSubQuery;
import com.mysema.query.support.Expressions;
import com.mysema.query.types.Path;
import com.mysema.query.types.query.ListSubQuery;

import hoot.services.db.DbUtils;
import hoot.services.db2.QCurrentNodes;
import hoot.services.db2.QCurrentRelationMembers;
import hoot.services.db2.QCurrentRelations;
import hoot.services.db2.QCurrentWayNodes;
import hoot.services.geo.BoundingBox;
import hoot.services.models.review.AllReviewableItems;
import hoot.services.models.review.ReviewQueryMapper;
import hoot.services.models.review.ReviewableItemBbox;
import hoot.services.models.review.ReviewableItemBboxInfo;


/**
 * The purpose of this class is to retrieve all reviewable items within the
 * bounding box. Following is the flow 1. Get counts of node, way and relation
 * members for all reviewable items. This is fast way to determine which query
 * to run since query can be time consuming. 2. For each element types involved
 * in review relation run query to retrieve relation_id and bbox grouped by
 * relation_id 3. Since review relation usually contains multiple members,
 * combine the bounding box of each members which results in bbox expansion.
 *
 */
class AllReviewableItemsQuery extends ReviewableQueryBase implements IReviewableQuery {
    private static final Logger logger = LoggerFactory.getLogger(AllReviewableItemsQuery.class);
    private static final long MAX_RESULT_SIZE;

    private final Path currentRelMembersSubQPath = Expressions.path(Void.class, "currentRelMembersSubQ");
    private final Path reviewableCurrentRelSubQPath = Expressions.path(Void.class, "reviewableCurrentRelSubQ");
    private final BoundingBox bbox;

    static {
        long value;
        try {
            value = Long.parseLong(MAX_QUERY_NODES);
        }
        catch (NumberFormatException ignored) {
            value = 60000;
            logger.info("Defaulting MAX_RESULT_SIZE to {}", value);
        }

        MAX_RESULT_SIZE = value;
    }

    public AllReviewableItemsQuery(Connection connection, long mapid, BoundingBox bbox) {
        super(connection, mapid);
        // maxQueryNodes
        this.bbox = bbox;
    }

    @Override
    public ReviewQueryMapper execQuery() throws Exception {
        // get counts for each element types of all review relation members
        Map<Long, ReviewableItemBboxInfo> allReviewables = new HashMap<>();
        long nNodeCnt = getReviewableRelationMembersCount(DbUtils.nwr_enum.node);
        long nWayCnt = getReviewableRelationMembersCount(DbUtils.nwr_enum.way);
        long nRelationCnt = getReviewableRelationMembersCount(DbUtils.nwr_enum.relation);

        boolean isPastLimit = false;

        // filter down to element type. Don't bother to run query on non
        // existent element type
        if ((nNodeCnt > 0) && !isPastLimit) {
            // get the node members bbox grouped by review relation_id
            Map<Long, ReviewableItemBboxInfo> reviewRelationWithNodeMembers = getReviewableRelatioWithNodeMembersCentroidInBbox();
            // merge bbox
            isPastLimit = combineBbox(allReviewables, reviewRelationWithNodeMembers);
        }

        if ((nWayCnt > 0) && !isPastLimit) {
            // get the way members bbox grouped by review relation_id
            Map<Long, ReviewableItemBboxInfo> reviewRelationWithWayMembers = getReviewableRelatioWithWayMembersCentroidInBbox();
            isPastLimit = combineBbox(allReviewables, reviewRelationWithWayMembers);
        }

        if ((nRelationCnt > 0) && !isPastLimit) {
            // for relation, we need do recursive calculation for bbox since
            // relation can contain another relation
            Map<Long, BoundingBox> relsBbox = new HashMap<>();
            List<Tuple> rels = getReviewableRelationMembers();

            // since multiple relation can contain same member
            // we want have unique member relations bbox
            for (Tuple rel : rels) {
                long relId = rel.get(Expressions.path(Long.class, currentRelMembersSubQPath, "member_id"));
                if (!relsBbox.containsKey(relId)) {
                    ReviewableItemBbox bbxInfo = getRelationMemberBbox(relId);
                    if (bbxInfo != null) {
                        relsBbox.put(relId, bbxInfo.getBbox());
                    }
                }
            }

            // now organize the reviews with relation members
            Map<Long, ReviewableItemBboxInfo> reviewRelationWithRelationMembers = new HashMap<>();
            for (Tuple rel : rels) {
                long relId = rel.get(Expressions.path(Long.class, currentRelMembersSubQPath, "relation_id"));
                long memId = rel.get(Expressions.path(Long.class, currentRelMembersSubQPath, "member_id"));
                String needReview = rel.get(Expressions.path(String.class, reviewableCurrentRelSubQPath, "needreview"));

                BoundingBox memBbox = relsBbox.get(memId);

                // we have relation member bbox and it is within the
                // requested bbox so if the relation member bbox exists in map (meaning
                // relation has multiple relation members) then expand the bbox else just add to the map
                if ((memBbox != null) && bbox.intersects(memBbox)) {
                    ReviewableItemBboxInfo relBboxInfo = reviewRelationWithRelationMembers.get(relId);

                    if (relBboxInfo != null) {
                        BoundingBox relBbox = relBboxInfo.getBbox();
                        relBbox.add(memBbox);
                    }
                    else {
                        ReviewableItemBboxInfo newInfo = new ReviewableItemBboxInfo(memBbox, getMapId(), relId, needReview);
                        reviewRelationWithRelationMembers.put(relId, newInfo);
                    }
                }
            }

            isPastLimit = combineBbox(allReviewables, reviewRelationWithRelationMembers);
        }

        AllReviewableItems ret = new AllReviewableItems(getMapId(), allReviewables);
        ret.setOverFlow(isPastLimit);

        return ret;
    }

    /**
     * Using ReviewableBboxQuery do recursive calculation of relation bbox
     * 
     * @param relId
     *            - target relation id
     * @return ReviewableItemBbox
     */
    private ReviewableItemBbox getRelationMemberBbox(long relId) {
        ReviewableBboxQuery bbq = new ReviewableBboxQuery(super.getConnection(), getMapId(), relId);
        ReviewableItemBbox ret = (ReviewableItemBbox) bbq.execQuery();
        return ret;
    }

    /**
     * Combines review relation members bbox relative to relation id
     * 
     * @param allReviewables
     *            - target map
     * @param reviewRelationWithMembers
     *            - list of member bbox
     */
    private static boolean combineBbox(Map<Long, ReviewableItemBboxInfo> allReviewables,
            Map<Long, ReviewableItemBboxInfo> reviewRelationWithMembers) {
        boolean isOverFlow = false;
        for (Object o : reviewRelationWithMembers.entrySet()) {
            Map.Entry pair = (Map.Entry) o;
            ReviewableItemBboxInfo currInfo = (ReviewableItemBboxInfo) pair.getValue();
            Long currRelId = (Long) pair.getKey();
            ReviewableItemBboxInfo info = allReviewables.get(currRelId);
            if (info != null) {
                // expand bbox
                info.getBbox().add(currInfo.getBbox());
            }
            else {
                if (allReviewables.size() > MAX_RESULT_SIZE) {
                    isOverFlow = true;
                    break;
                }

                // just add
                allReviewables.put(currRelId, currInfo);
            }
        }

        return isOverFlow;
    }

    /**
     * Gets sql query string which counts of all reviewable relation members of
     * the element type.
     * 
     * @param memberType
     *            - [node|way|relation]
     * @return - SQL String
     */
    SQLQuery getReviewableRelationMembersCountByTypeQuery(DbUtils.nwr_enum memberType) {

        QCurrentRelationMembers currentRelationMembers = QCurrentRelationMembers.currentRelationMembers;
        QCurrentRelations currentRelations = QCurrentRelations.currentRelations;

        return new SQLQuery(super.getConnection(),
                DbUtils.getConfiguration(
                        getMapId()))
                                .from(currentRelationMembers).where(
                                        currentRelationMembers.relationId
                                                .in(new SQLSubQuery().from(currentRelations)
                                                        .where(Expressions
                                                                .booleanTemplate("exist(tags,'hoot:review:needs')"))
                                                        .list(currentRelations.id))
                                                .and(currentRelationMembers.memberType.eq(memberType)));

    }

    /**
     * counts of all reviewable relation members of the element type by
     * executing getReviewableRelationMembersCountByTypeQuery
     * 
     * @param type
     *            - [node|way|relation]
     * @return - result count
     */
    private long getReviewableRelationMembersCount(DbUtils.nwr_enum type) {
        long recordCount = getReviewableRelationMembersCountByTypeQuery(type).count();
        return recordCount;
    }

    /**
     * Helper function to translation java.sql.ResultSet to BoundingBox
     * 
     * @return - BoundingBox
     */
    private static BoundingBox resultSetToBbox(Tuple tup, Path bboxPath) throws Exception {
        double minLat = tup.get(Expressions.numberPath(Double.class, bboxPath, "minlat"));
        double maxLat = tup.get(Expressions.numberPath(Double.class, bboxPath, "maxlat"));
        double minLon = tup.get(Expressions.numberPath(Double.class, bboxPath, "minlon"));
        double maxLon = tup.get(Expressions.numberPath(Double.class, bboxPath, "maxlon"));

        BoundingBox bbox = new BoundingBox(minLon, minLat, maxLon, maxLat);
        return bbox;
    }

    /**
     * Helper wraper function for create sql statement
     * 
     * @return - java,sql.Statement object
     */
    protected Statement _createStatement() throws SQLException {
        return super.getConnection().createStatement();
    }

    /**
     * Retrieves relation_id, needreview, minlat, maxlat, minlon, maxlon of
     * reviewable item relation way members group by relation_id. The bbox of
     * all members of each relation is calculated and then the centroid value is
     * compare with the user specified bounding box and returns only ones that
     * intersets.
     * 
     * @return - Map of ReviewableItemBboxInfo which contains relation and its
     *         bbox information
     * @throws Exception
     */
    private Map<Long, ReviewableItemBboxInfo> getReviewableRelatioWithWayMembersCentroidInBbox() throws Exception {
        Map<Long, ReviewableItemBboxInfo> relationBboxMap = new HashMap<>();

        Path reviewRelWayMembersCentroidSubQPath = Expressions.path(Void.class, "reviewRelWayMembersCentroidSubQ");
        List<Tuple> tups = getReviewableRelatioWithWayMembersCentroidInBboxQuery().list(
                Expressions.path(Long.class, reviewRelWayMembersCentroidSubQPath, "relation_id"),
                Expressions.path(String.class, reviewRelWayMembersCentroidSubQPath, "needreview"),
                Expressions.numberPath(Double.class, reviewRelWayMembersCentroidSubQPath, "maxlat"),
                Expressions.numberPath(Double.class, reviewRelWayMembersCentroidSubQPath, "minlat"),
                Expressions.numberPath(Double.class, reviewRelWayMembersCentroidSubQPath, "maxlon"),
                Expressions.numberPath(Double.class, reviewRelWayMembersCentroidSubQPath, "minlon"));

        for (Tuple tup : tups) {
            long relId = tup.get(Expressions.path(Long.class, reviewRelWayMembersCentroidSubQPath, "relation_id"));
            String needReview = tup.get(Expressions.path(String.class, reviewRelWayMembersCentroidSubQPath, "needreview"));
            BoundingBox bbox = resultSetToBbox(tup, reviewRelWayMembersCentroidSubQPath);
            ReviewableItemBboxInfo info = new ReviewableItemBboxInfo(bbox, getMapId(), relId, needReview);
            relationBboxMap.put(relId, info);
        }

        return relationBboxMap;
    }

    /**
     * SQL Query that retrieves relation_id, needreview, minlat, maxlat, minlon,
     * maxlon of reviewable item relation way members group by relation_id. The
     * bbox of all members of each relation is calculated and then the centroid
     * value is compare with the user specified bounding box and returns only
     * ones that intersets.
     * 
     * @return - SQL query string
     * 
     * @throws Exception
     */
    SQLQuery getReviewableRelatioWithWayMembersCentroidInBboxQuery() throws Exception {
        if (bbox == null) {
            throw new Exception("Invalid Bounding box.");
        }

        QCurrentRelationMembers currentRelationMembers = QCurrentRelationMembers.currentRelationMembers;
        QCurrentRelations currentRelations = QCurrentRelations.currentRelations;

        ListSubQuery<Tuple> reviewableCurrentRelSubQ = new SQLSubQuery().from(currentRelations)
                .where(Expressions.booleanTemplate("exist(tags,'hoot:review:needs')")).list(currentRelations.id,
                        Expressions.stringTemplate("tags->'hoot:review:needs'").as("needreview"));

        ListSubQuery<Tuple> currentRelMembersSubQ = new SQLSubQuery().from(currentRelationMembers)
                .where(currentRelationMembers.memberType.eq(DbUtils.nwr_enum.way))
                .list(currentRelationMembers.memberId, currentRelationMembers.relationId,
                        currentRelationMembers.memberType);

        Path reviewRelJoinRelMemberSubQPath = Expressions.path(Void.class, "reviewRelJoinRelMemberSubQ");
        ListSubQuery<Tuple> reviewRelJoinRelMemberSubQ = new SQLSubQuery()
                .join(currentRelMembersSubQ, currentRelMembersSubQPath)
                .join(reviewableCurrentRelSubQ, reviewableCurrentRelSubQPath)
                .on(Expressions.path(Long.class, currentRelMembersSubQPath, "relation_id")
                        .eq(Expressions.path(Long.class, reviewableCurrentRelSubQPath, "id")))
                .list(Expressions.path(Long.class, currentRelMembersSubQPath, "member_id"),
                        Expressions.path(Long.class, currentRelMembersSubQPath, "relation_id"),
                        Expressions.path(String.class, reviewableCurrentRelSubQPath, "needreview"));

        QCurrentWayNodes currentWayNodes = QCurrentWayNodes.currentWayNodes;

        Path currentWayNodesSubQPath = Expressions.path(Void.class, "currentWayNodesSubQ");
        ListSubQuery<Tuple> currentWayNodesSubQ = new SQLSubQuery().from(currentWayNodes)
                .list(currentWayNodes.nodeId, currentWayNodes.wayId);

        Path reviewRelJoinRelMemberJoinCurrentWayNodesSubQPath = Expressions.path(Void.class,
                "reviewRelJoinRelMemberJoinCurrentWayNodesSubQ");
        ListSubQuery<Tuple> reviewRelJoinRelMemberJoinCurrentWayNodesSubQ = new SQLSubQuery()
                .join(currentWayNodesSubQ, currentWayNodesSubQPath)
                .join(reviewRelJoinRelMemberSubQ, reviewRelJoinRelMemberSubQPath)
                .on(Expressions.path(Long.class, currentWayNodesSubQPath, "way_id")
                        .eq(Expressions.path(Long.class, reviewRelJoinRelMemberSubQPath, "member_id")))
                .list(Expressions.path(Long.class, currentWayNodesSubQPath, "node_id"),
                        Expressions.path(Long.class, reviewRelJoinRelMemberSubQPath, "relation_id"),
                        Expressions.path(String.class, reviewRelJoinRelMemberSubQPath, "needreview"));

        QCurrentNodes currentNodes = QCurrentNodes.currentNodes;
        Path currentNodeSubQPath = Expressions.path(Void.class, "currentNodeSubQ");
        ListSubQuery<Tuple> currentNodesSubQ = new SQLSubQuery().from(currentNodes).list(currentNodes.id,
                currentNodes.latitude, currentNodes.longitude);

        Path reviewRelWayMembersCentroidSubQPath = Expressions.path(Void.class, "reviewRelWayMembersCentroidSubQ");
        ListSubQuery<Tuple> reviewRelWayMembersCentroidSubQ = new SQLSubQuery()
                .join(currentNodesSubQ, currentNodeSubQPath)
                .join(reviewRelJoinRelMemberJoinCurrentWayNodesSubQ,
                        reviewRelJoinRelMemberJoinCurrentWayNodesSubQPath)
                .on(Expressions.path(Long.class, currentNodeSubQPath, "id")
                        .eq(Expressions.path(Long.class, reviewRelJoinRelMemberJoinCurrentWayNodesSubQPath,
                                "node_id")))
                .groupBy(
                        Expressions.path(Long.class, reviewRelJoinRelMemberJoinCurrentWayNodesSubQPath,
                                "relation_id"),
                        Expressions.path(String.class, reviewRelJoinRelMemberJoinCurrentWayNodesSubQPath,
                                "needreview"))
                .list(Expressions.path(Long.class, reviewRelJoinRelMemberJoinCurrentWayNodesSubQPath,
                        "relation_id"),
                        Expressions.path(String.class, reviewRelJoinRelMemberJoinCurrentWayNodesSubQPath,
                                "needreview"),
                        Expressions.numberPath(Double.class, currentNodeSubQPath, "latitude").max().as("maxlat"),
                        Expressions.numberPath(Double.class, currentNodeSubQPath, "latitude").min().as("minlat"),
                        Expressions.numberPath(Double.class, currentNodeSubQPath, "longitude").max().as("maxlon"),
                        Expressions.numberPath(Double.class, currentNodeSubQPath, "longitude").min().as("minlon"),
                        Expressions
                                .numberTemplate(Double.class,
                                        "(((max(\"currentNodeSubQ\".\"latitude\") - min(\"currentNodeSubQ\".\"latitude\"))/2)+min(\"currentNodeSubQ\".\"latitude\"))")
                                .as("centlat"),
                        Expressions
                                .numberTemplate(Double.class,
                                        "(((max(\"currentNodeSubQ\".\"longitude\") - min(\"currentNodeSubQ\".\"longitude\"))/2)+min(\"currentNodeSubQ\".\"longitude\"))")
                                .as("centlon"));

        SQLQuery sql = new SQLQuery(super.getConnection(), DbUtils.getConfiguration(getMapId()))
                .from(reviewRelWayMembersCentroidSubQ.as(reviewRelWayMembersCentroidSubQPath))
                .where(Expressions.numberPath(Double.class, reviewRelWayMembersCentroidSubQPath, "centlat")
                        .goe(bbox.getMinLat())
                        .and(Expressions.numberPath(Double.class, reviewRelWayMembersCentroidSubQPath, "centlat")
                                .loe(bbox.getMaxLat()))
                        .and(Expressions.numberPath(Double.class, reviewRelWayMembersCentroidSubQPath, "centlon")
                                .goe(bbox.getMinLon()))
                        .and(Expressions.numberPath(Double.class, reviewRelWayMembersCentroidSubQPath, "centlon")
                                .loe(bbox.getMaxLon())))
                .limit(MAX_RESULT_SIZE + 1);

        return sql;
    }

    /**
     * Retrieves relation_id, needreview, minlat, maxlat, minlon, maxlon of
     * reviewable item relation node members group by relation_id. The bbox of
     * all members of each relation is calculated and then the centroid value is
     * compare with the user specified bounding box and returns only ones that
     * intersets.
     * 
     * @return - Map of ReviewableItemBboxInfo which contains relation and its
     *         bbox information
     * @throws Exception
     */
    private Map<Long, ReviewableItemBboxInfo> getReviewableRelatioWithNodeMembersCentroidInBbox() throws Exception {
        Map<Long, ReviewableItemBboxInfo> relationBboxMap = new HashMap<>();

        Path reviewRelNodeMembersCentroidSubQPath = Expressions.path(Void.class,
                "reviewRelNodeMembersCentroidSubQ");
        List<Tuple> tups = getReviewableRelatioWithNodeMembersCentroidInBboxQuery().list(
                Expressions.path(Long.class, reviewRelNodeMembersCentroidSubQPath, "relation_id"),
                Expressions.path(String.class, reviewRelNodeMembersCentroidSubQPath, "needreview"),
                Expressions.numberPath(Double.class, reviewRelNodeMembersCentroidSubQPath, "maxlat"),
                Expressions.numberPath(Double.class, reviewRelNodeMembersCentroidSubQPath, "minlat"),
                Expressions.numberPath(Double.class, reviewRelNodeMembersCentroidSubQPath, "maxlon"),
                Expressions.numberPath(Double.class, reviewRelNodeMembersCentroidSubQPath, "minlon"));

        for (Tuple tup : tups) {
            long relId = tup.get(Expressions.path(Long.class, reviewRelNodeMembersCentroidSubQPath, "relation_id"));
            String needReview = tup.get(Expressions.path(String.class, reviewRelNodeMembersCentroidSubQPath, "needreview"));
            BoundingBox bbox = resultSetToBbox(tup, reviewRelNodeMembersCentroidSubQPath);
            ReviewableItemBboxInfo info = new ReviewableItemBboxInfo(bbox, getMapId(), relId, needReview);
            relationBboxMap.put(relId, info);
        }

        return relationBboxMap;
    }

    /**
     * SQL Query that retrieves relation_id, needreview, minlat, maxlat, minlon,
     * maxlon of reviewable item relation node members group by relation_id. The
     * bbox of all members of each relation is calculated and then the centroid
     * value is compare with the user specified bounding box and returns only
     * ones that intersets.
     * 
     * @return - SQL query string
     */
    SQLQuery getReviewableRelatioWithNodeMembersCentroidInBboxQuery() throws Exception {
        if (bbox == null) {
            throw new Exception("Invalid Bounding box.");
        }

        QCurrentRelationMembers currentRelationMembers = QCurrentRelationMembers.currentRelationMembers;
        QCurrentRelations currentRelations = QCurrentRelations.currentRelations;

        ListSubQuery<Tuple> reviewableCurrentRelSubQ = new SQLSubQuery().from(currentRelations)
                .where(Expressions.booleanTemplate("exist(tags,'hoot:review:needs')")).list(currentRelations.id,
                        Expressions.stringTemplate("tags->'hoot:review:needs'").as("needreview"));

        ListSubQuery<Tuple> currentRelMembersSubQ = new SQLSubQuery().from(currentRelationMembers)
                .where(currentRelationMembers.memberType.eq(DbUtils.nwr_enum.node))
                .list(currentRelationMembers.memberId, currentRelationMembers.relationId,
                        currentRelationMembers.memberType);

        Path reviewRelJoinRelMemberSubQPath = Expressions.path(Void.class, "reviewRelJoinRelMemberSubQ");
        ListSubQuery<Tuple> reviewRelJoinRelMemberSubQ = new SQLSubQuery()
                .join(currentRelMembersSubQ, currentRelMembersSubQPath)
                .join(reviewableCurrentRelSubQ, reviewableCurrentRelSubQPath)
                .on(Expressions.path(Long.class, currentRelMembersSubQPath, "relation_id")
                        .eq(Expressions.path(Long.class, reviewableCurrentRelSubQPath, "id")))
                .list(Expressions.path(Long.class, currentRelMembersSubQPath, "member_id"),
                        Expressions.path(Long.class, currentRelMembersSubQPath, "relation_id"),
                        Expressions.path(String.class, reviewableCurrentRelSubQPath, "needreview"));

        QCurrentNodes currentNodes = QCurrentNodes.currentNodes;
        Path currentNodeSubQPath = Expressions.path(Void.class, "currentNodeSubQ");
        ListSubQuery<Tuple> currentNodesSubQ = new SQLSubQuery().from(currentNodes).list(currentNodes.id,
                currentNodes.latitude, currentNodes.longitude);

        Path reviewRelNodeMembersCentroidSubQPath = Expressions.path(Void.class,
                "reviewRelNodeMembersCentroidSubQ");
        ListSubQuery<Tuple> reviewRelWayMembersCentroidSubQ = new SQLSubQuery()
                .join(currentNodesSubQ, currentNodeSubQPath)
                .join(reviewRelJoinRelMemberSubQ, reviewRelJoinRelMemberSubQPath)
                .on(Expressions.path(Long.class, currentNodeSubQPath, "id")
                        .eq(Expressions.path(Long.class, reviewRelJoinRelMemberSubQPath, "member_id")))
                .groupBy(Expressions.path(Long.class, reviewRelJoinRelMemberSubQPath, "relation_id"),
                        Expressions.path(String.class, reviewRelJoinRelMemberSubQPath, "needreview"))
                .list(Expressions.path(Long.class, reviewRelJoinRelMemberSubQPath, "relation_id"),
                        Expressions.path(String.class, reviewRelJoinRelMemberSubQPath, "needreview"),
                        Expressions.numberPath(Double.class, currentNodeSubQPath, "latitude").max().as("maxlat"),
                        Expressions.numberPath(Double.class, currentNodeSubQPath, "latitude").min().as("minlat"),
                        Expressions.numberPath(Double.class, currentNodeSubQPath, "longitude").max().as("maxlon"),
                        Expressions.numberPath(Double.class, currentNodeSubQPath, "longitude").min().as("minlon"),
                        Expressions
                                .numberTemplate(Double.class,
                                        "(((max(\"currentNodeSubQ\".\"latitude\") - min(\"currentNodeSubQ\".\"latitude\"))/2)+min(\"currentNodeSubQ\".\"latitude\"))")
                                .as("centlat"),
                        Expressions
                                .numberTemplate(Double.class,
                                        "(((max(\"currentNodeSubQ\".\"longitude\") - min(\"currentNodeSubQ\".\"longitude\"))/2)+min(\"currentNodeSubQ\".\"longitude\"))")
                                .as("centlon"));

        SQLQuery sql = new SQLQuery(super.getConnection(), DbUtils.getConfiguration(getMapId()))
                .from(reviewRelWayMembersCentroidSubQ.as(reviewRelNodeMembersCentroidSubQPath))
                .where(Expressions.numberPath(Double.class, reviewRelNodeMembersCentroidSubQPath, "centlat")
                        .goe(bbox.getMinLat())
                        .and(Expressions.numberPath(Double.class, reviewRelNodeMembersCentroidSubQPath, "centlat")
                                .loe(bbox.getMaxLat()))
                        .and(Expressions.numberPath(Double.class, reviewRelNodeMembersCentroidSubQPath, "centlon")
                                .goe(bbox.getMinLon()))
                        .and(Expressions.numberPath(Double.class, reviewRelNodeMembersCentroidSubQPath, "centlon")
                                .loe(bbox.getMaxLon())))
                .limit(MAX_RESULT_SIZE + 1);

        return sql;
    }

    /**
     * Retrieve relation_id, relation_id, needreview of all review relation
     * relation members
     * 
     * @return - List of JSONObject containing relation_id, member_id,
     *         needreview
     */
    private List<Tuple> getReviewableRelationMembers() {
        return getReviewableRelationMembersQuery().list(
                Expressions.path(Long.class, currentRelMembersSubQPath, "member_id"),
                Expressions.path(Long.class, currentRelMembersSubQPath, "relation_id"),
                Expressions.path(String.class, reviewableCurrentRelSubQPath, "needreview"));
    }

    /**
     * Generates SQL query string for retrieving relation_id, relation_id,
     * needreview of all review relation relation members
     * 
     * @return - SQL String
     */
    private SQLQuery getReviewableRelationMembersQuery() {

        QCurrentRelationMembers currentRelationMembers = QCurrentRelationMembers.currentRelationMembers;
        QCurrentRelations currentRelations = QCurrentRelations.currentRelations;

        ListSubQuery<Tuple> reviewableCurrentRelSubQ = new SQLSubQuery().from(currentRelations)
                .where(Expressions.booleanTemplate("exist(tags,'hoot:review:needs')"))
                .list(currentRelations.id, Expressions.stringTemplate("tags->'hoot:review:needs'").as("needreview"));

        ListSubQuery<Tuple> currentRelMembersSubQ = new SQLSubQuery().from(currentRelationMembers)
                .where(currentRelationMembers.memberType.eq(DbUtils.nwr_enum.relation))
                .list(currentRelationMembers.memberId, currentRelationMembers.relationId,
                        currentRelationMembers.memberType);

        return new SQLQuery(super.getConnection(), DbUtils.getConfiguration(this.getMapId()))
                .join(currentRelMembersSubQ, currentRelMembersSubQPath)
                .join(reviewableCurrentRelSubQ, reviewableCurrentRelSubQPath)
                .on(Expressions.path(Long.class, currentRelMembersSubQPath, "relation_id")
                        .eq(Expressions.path(Long.class, reviewableCurrentRelSubQPath, "id")))
                .groupBy(Expressions.path(Long.class, currentRelMembersSubQPath, "member_id"),
                        Expressions.path(Long.class, currentRelMembersSubQPath, "relation_id"),
                        Expressions.path(String.class, reviewableCurrentRelSubQPath, "needreview"));
    }
}
