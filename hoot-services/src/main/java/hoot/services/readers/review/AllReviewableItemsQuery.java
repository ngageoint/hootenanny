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

import java.sql.Connection;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.HashMap;
import java.util.Iterator;
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

import hoot.services.HootProperties;
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
public class AllReviewableItemsQuery extends ReviewableQueryBase implements IReviewableQuery {

    private BoundingBox _bbox;
    private static final Logger log = LoggerFactory.getLogger(AllReviewableItemsQuery.class);

    private Path _currentRelMembersSubQPath = Expressions.path(Void.class, "currentRelMembersSubQ");
    private Path _reviewableCurrentRelSubQPath = Expressions.path(Void.class, "reviewableCurrentRelSubQ");

    private static long MAX_RESULT_SIZE = 60000;
    static {
        try {
            String maxQuerySize = HootProperties.getProperty("maxQueryNodes");
            MAX_RESULT_SIZE = Long.parseLong(maxQuerySize);
        }
        catch (Exception ex) {
            log.error(ex.getMessage());
        }
    }

    public AllReviewableItemsQuery(final Connection c, final long mapid, final BoundingBox bbox) {
        super(c, mapid);
        // maxQueryNodes
        _bbox = bbox;
    }

    /*
     * (non-Javadoc)
     * 
     * @see hoot.services.readers.review.IReviewableQuery#execQuery() This is
     * the main entry point function invoked
     */
    /*
     * (non-Javadoc)
     * 
     * @see hoot.services.readers.review.IReviewableQuery#execQuery()
     */
    @Override
    public ReviewQueryMapper execQuery() throws SQLException, Exception {

        AllReviewableItems ret = new AllReviewableItems(getMapId(), new HashMap<Long, ReviewableItemBboxInfo>());
        try {
            // get counts for each element types of all review relation members
            Map<Long, ReviewableItemBboxInfo> allReviewables = new HashMap<>();
            long nNodeCnt = _getReviewableRelationMembersCount(DbUtils.nwr_enum.node);
            long nWayCnt = _getReviewableRelationMembersCount(DbUtils.nwr_enum.way);
            long nRelationCnt = _getReviewableRelationMembersCount(DbUtils.nwr_enum.relation);

            boolean isPastLimit = false;

            // filter down to element type. Don't bother to run query on non
            // existent element type
            if (nNodeCnt > 0 && !isPastLimit) {
                // get the node members bbox grouped by review relation_id
                Map<Long, ReviewableItemBboxInfo> reviewRelationWithNodeMembers = _getReviewableRelatioWithNodeMembersCentroidInBbox();
                // merge bbox
                isPastLimit = _combineBbox(allReviewables, reviewRelationWithNodeMembers);
            }

            if (nWayCnt > 0 && !isPastLimit) {
                // get the way members bbox grouped by review relation_id
                Map<Long, ReviewableItemBboxInfo> reviewRelationWithWayMembers = _getReviewableRelatioWithWayMembersCentroidInBbox();
                isPastLimit = _combineBbox(allReviewables, reviewRelationWithWayMembers);
            }

            if (nRelationCnt > 0 && !isPastLimit) {
                // for relation, we need do recursive calculation for bbox since
                // relation can contain another relation
                Map<Long, BoundingBox> relsBbox = new HashMap<>();
                List<Tuple> rels = _getReviewableRelationMembers();

                // since multiple relation can contain same member
                // we want have unique member relations bbox
                for (Tuple rel : rels) {
                    long relId = rel.get(Expressions.path(Long.class, _currentRelMembersSubQPath, "member_id"));
                    if (!relsBbox.containsKey(relId)) {
                        ReviewableItemBbox bbxInfo = _getRelationMemberBbox(relId);
                        if (bbxInfo != null) {
                            relsBbox.put(relId, bbxInfo.getBbox());
                        }
                    }
                }
                // now organize the reviews with relation members
                Map<Long, ReviewableItemBboxInfo> reviewRelationWithRelationMembers = new HashMap<>();
                for (Tuple rel : rels) {
                    long relId = rel.get(Expressions.path(Long.class, _currentRelMembersSubQPath, "relation_id"));
                    long memId = rel.get(Expressions.path(Long.class, _currentRelMembersSubQPath, "member_id"));
                    String needReview = rel
                            .get(Expressions.path(String.class, _reviewableCurrentRelSubQPath, "needreview"));

                    BoundingBox memBbox = relsBbox.get(memId);
                    // we have relation member bbox and it is within the
                    // requested bbox
                    // so if the relation member bbox exists in map (meaning
                    // relation has multiple relation members)
                    // then expand the bbox else just add to the map
                    if (memBbox != null && _bbox.intersects(memBbox)) {
                        ReviewableItemBboxInfo relBboxInfo = reviewRelationWithRelationMembers.get(relId);

                        if (relBboxInfo != null) {
                            BoundingBox relBbox = relBboxInfo.getBbox();
                            relBbox.add(memBbox);
                        }
                        else {
                            ReviewableItemBboxInfo newInfo = new ReviewableItemBboxInfo(memBbox, getMapId(), relId,
                                    needReview);
                            reviewRelationWithRelationMembers.put(relId, newInfo);
                        }

                    }
                }

                isPastLimit = _combineBbox(allReviewables, reviewRelationWithRelationMembers);

            }

            ret = new AllReviewableItems(getMapId(), allReviewables);
            ret.setOverFlow(isPastLimit);

        }
        catch (Exception ex) {
            log.error(ex.getMessage());
            throw ex;
        }
        return ret;
    }

    /**
     * Using ReviewableBboxQuery do recursive calculation of relation bbox
     * 
     * @param relId
     *            - target relation id
     * @return ReviewableItemBbox
     * @throws Exception
     */
    protected ReviewableItemBbox _getRelationMemberBbox(final long relId) throws Exception {
        ReviewableItemBbox ret = null;
        try {
            ReviewableBboxQuery bbq = new ReviewableBboxQuery(getConnection(), getMapId(), relId);
            ret = (ReviewableItemBbox) bbq.execQuery();
        }
        catch (Exception ex) {
            log.error(ex.getMessage());
            throw ex;
        }
        return ret;
    }

    /**
     * Combines review relation members bbox relative to relation id
     * 
     * @param allReviewables
     *            - target map
     * @param reviewRelationWithMembers
     *            - list of member bbox
     * @throws Exception
     */
    private static boolean _combineBbox(final Map<Long, ReviewableItemBboxInfo> allReviewables,
            final Map<Long, ReviewableItemBboxInfo> reviewRelationWithMembers) throws Exception {
        boolean isOverFlow = false;
        try {
            Iterator it = reviewRelationWithMembers.entrySet().iterator();
            while (it.hasNext()) {
                Map.Entry pair = (Map.Entry) it.next();
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
        }
        catch (Exception ex) {
            log.error(ex.getMessage());
            throw ex;
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
    protected SQLQuery _getReviewableRelationMembersCountByTypeQuery(final DbUtils.nwr_enum memberType) {

        QCurrentRelationMembers currentRelationMembers = QCurrentRelationMembers.currentRelationMembers;
        QCurrentRelations currentRelations = QCurrentRelations.currentRelations;

        return new SQLQuery(getConnection(),
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
     * executing _getReviewableRelationMembersCountByTypeQuery
     * 
     * @param type
     *            - [node|way|relation]
     * @return - result count
     * @throws Exception
     */
    protected long _getReviewableRelationMembersCount(final DbUtils.nwr_enum type) throws Exception {
        long recordCount = 0;

        try {
            recordCount = _getReviewableRelationMembersCountByTypeQuery(type).count();

        }
        catch (Exception ex) {
            log.error(ex.getMessage());
            throw ex;
        }

        return recordCount;
    }

    /**
     * Helper function to translation java.sql.ResultSet to BoundingBox
     * 
     * @param rs
     *            - source result set
     * @return - BoundingBox
     * @throws Exception
     */
    private static BoundingBox _resultSetToBbox(final Tuple tup, final Path bboxPath) throws Exception {
        BoundingBox bbox = null;
        double minLon = -1, minLat = -1, maxLon = -1, maxLat = -1;
        try {

            minLat = tup.get(Expressions.numberPath(Double.class, bboxPath, "minlat"));
            maxLat = tup.get(Expressions.numberPath(Double.class, bboxPath, "maxlat"));
            minLon = tup.get(Expressions.numberPath(Double.class, bboxPath, "minlon"));
            maxLon = tup.get(Expressions.numberPath(Double.class, bboxPath, "maxlon"));

            bbox = new BoundingBox(minLon, minLat, maxLon, maxLat);
        }
        catch (Exception ex) {
            log.error(ex.getMessage());
            throw ex;
        }
        return bbox;
    }

    /**
     * Helper wraper function for create sql statement
     * 
     * @return - java,sql.Statement object
     * @throws Exception
     */
    protected Statement _createStatement() throws Exception {
        return getConnection().createStatement();
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
    protected Map<Long, ReviewableItemBboxInfo> _getReviewableRelatioWithWayMembersCentroidInBbox() throws Exception {
        Map<Long, ReviewableItemBboxInfo> relationBboxMap = new HashMap<>();

        try {
            Path reviewRelWayMembersCentroidSubQPath = Expressions.path(Void.class, "reviewRelWayMembersCentroidSubQ");
            List<Tuple> tups = _getReviewableRelatioWithWayMembersCentroidInBboxQuery().list(
                    Expressions.path(Long.class, reviewRelWayMembersCentroidSubQPath, "relation_id"),
                    Expressions.path(String.class, reviewRelWayMembersCentroidSubQPath, "needreview"),
                    Expressions.numberPath(Double.class, reviewRelWayMembersCentroidSubQPath, "maxlat"),
                    Expressions.numberPath(Double.class, reviewRelWayMembersCentroidSubQPath, "minlat"),
                    Expressions.numberPath(Double.class, reviewRelWayMembersCentroidSubQPath, "maxlon"),
                    Expressions.numberPath(Double.class, reviewRelWayMembersCentroidSubQPath, "minlon"));

            for (Tuple tup : tups) {
                long relId = tup.get(Expressions.path(Long.class, reviewRelWayMembersCentroidSubQPath, "relation_id"));
                String needReview = tup
                        .get(Expressions.path(String.class, reviewRelWayMembersCentroidSubQPath, "needreview"));
                BoundingBox bbox = _resultSetToBbox(tup, reviewRelWayMembersCentroidSubQPath);
                ReviewableItemBboxInfo info = new ReviewableItemBboxInfo(bbox, getMapId(), relId, needReview);
                relationBboxMap.put(relId, info);
            }

        }
        catch (Exception ex) {
            log.error(ex.getMessage());
            throw ex;
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
    protected SQLQuery _getReviewableRelatioWithWayMembersCentroidInBboxQuery() throws Exception {
        SQLQuery sql = null;
        try {
            if (_bbox == null) {
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
                    .join(currentRelMembersSubQ, _currentRelMembersSubQPath)
                    .join(reviewableCurrentRelSubQ, _reviewableCurrentRelSubQPath)
                    .on(Expressions.path(Long.class, _currentRelMembersSubQPath, "relation_id")
                            .eq(Expressions.path(Long.class, _reviewableCurrentRelSubQPath, "id")))
                    .list(Expressions.path(Long.class, _currentRelMembersSubQPath, "member_id"),
                            Expressions.path(Long.class, _currentRelMembersSubQPath, "relation_id"),
                            Expressions.path(String.class, _reviewableCurrentRelSubQPath, "needreview"));

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

            sql = new SQLQuery(getConnection(), DbUtils.getConfiguration(getMapId()))
                    .from(reviewRelWayMembersCentroidSubQ.as(reviewRelWayMembersCentroidSubQPath))
                    .where(Expressions.numberPath(Double.class, reviewRelWayMembersCentroidSubQPath, "centlat")
                            .goe(_bbox.getMinLat())
                            .and(Expressions.numberPath(Double.class, reviewRelWayMembersCentroidSubQPath, "centlat")
                                    .loe(_bbox.getMaxLat()))
                            .and(Expressions.numberPath(Double.class, reviewRelWayMembersCentroidSubQPath, "centlon")
                                    .goe(_bbox.getMinLon()))
                            .and(Expressions.numberPath(Double.class, reviewRelWayMembersCentroidSubQPath, "centlon")
                                    .loe(_bbox.getMaxLon())))
                    .limit(MAX_RESULT_SIZE + 1);

        }
        catch (Exception ex) {
            log.error(ex.getMessage());
            throw ex;
        }

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
    protected Map<Long, ReviewableItemBboxInfo> _getReviewableRelatioWithNodeMembersCentroidInBbox() throws Exception {
        Map<Long, ReviewableItemBboxInfo> relationBboxMap = new HashMap<>();

        try {
            Path reviewRelNodeMembersCentroidSubQPath = Expressions.path(Void.class,
                    "reviewRelNodeMembersCentroidSubQ");
            List<Tuple> tups = _getReviewableRelatioWithNodeMembersCentroidInBboxQuery().list(
                    Expressions.path(Long.class, reviewRelNodeMembersCentroidSubQPath, "relation_id"),
                    Expressions.path(String.class, reviewRelNodeMembersCentroidSubQPath, "needreview"),
                    Expressions.numberPath(Double.class, reviewRelNodeMembersCentroidSubQPath, "maxlat"),
                    Expressions.numberPath(Double.class, reviewRelNodeMembersCentroidSubQPath, "minlat"),
                    Expressions.numberPath(Double.class, reviewRelNodeMembersCentroidSubQPath, "maxlon"),
                    Expressions.numberPath(Double.class, reviewRelNodeMembersCentroidSubQPath, "minlon"));

            for (Tuple tup : tups) {
                long relId = tup.get(Expressions.path(Long.class, reviewRelNodeMembersCentroidSubQPath, "relation_id"));
                String needReview = tup
                        .get(Expressions.path(String.class, reviewRelNodeMembersCentroidSubQPath, "needreview"));
                BoundingBox bbox = _resultSetToBbox(tup, reviewRelNodeMembersCentroidSubQPath);
                ReviewableItemBboxInfo info = new ReviewableItemBboxInfo(bbox, getMapId(), relId, needReview);
                relationBboxMap.put(relId, info);
            }

        }
        catch (Exception ex) {
            log.error(ex.getMessage());
            throw ex;
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
     * 
     * @throws Exception
     */
    protected SQLQuery _getReviewableRelatioWithNodeMembersCentroidInBboxQuery() throws Exception {
        SQLQuery sql = null;
        try {
            if (_bbox == null) {
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
                    .join(currentRelMembersSubQ, _currentRelMembersSubQPath)
                    .join(reviewableCurrentRelSubQ, _reviewableCurrentRelSubQPath)
                    .on(Expressions.path(Long.class, _currentRelMembersSubQPath, "relation_id")
                            .eq(Expressions.path(Long.class, _reviewableCurrentRelSubQPath, "id")))
                    .list(Expressions.path(Long.class, _currentRelMembersSubQPath, "member_id"),
                            Expressions.path(Long.class, _currentRelMembersSubQPath, "relation_id"),
                            Expressions.path(String.class, _reviewableCurrentRelSubQPath, "needreview"));

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

            sql = new SQLQuery(getConnection(), DbUtils.getConfiguration(getMapId()))
                    .from(reviewRelWayMembersCentroidSubQ.as(reviewRelNodeMembersCentroidSubQPath))
                    .where(Expressions.numberPath(Double.class, reviewRelNodeMembersCentroidSubQPath, "centlat")
                            .goe(_bbox.getMinLat())
                            .and(Expressions.numberPath(Double.class, reviewRelNodeMembersCentroidSubQPath, "centlat")
                                    .loe(_bbox.getMaxLat()))
                            .and(Expressions.numberPath(Double.class, reviewRelNodeMembersCentroidSubQPath, "centlon")
                                    .goe(_bbox.getMinLon()))
                            .and(Expressions.numberPath(Double.class, reviewRelNodeMembersCentroidSubQPath, "centlon")
                                    .loe(_bbox.getMaxLon())))
                    .limit(MAX_RESULT_SIZE + 1);

        }
        catch (Exception ex) {
            log.error(ex.getMessage());
            throw ex;
        }

        return sql;
    }

    /**
     * Retrieve relation_id, relation_id, needreview of all review relation
     * relation members
     * 
     * @return - List of JSONObject containing relation_id, member_id,
     *         needreview
     * @throws Exception
     */
    protected List<Tuple> _getReviewableRelationMembers() throws Exception {
        return _getReviewableRelationMembersQuery().list(
                Expressions.path(Long.class, _currentRelMembersSubQPath, "member_id"),
                Expressions.path(Long.class, _currentRelMembersSubQPath, "relation_id"),
                Expressions.path(String.class, _reviewableCurrentRelSubQPath, "needreview"));
    }

    /**
     * Generates SQL query string for retrieving relation_id, relation_id,
     * needreview of all review relation relation members
     * 
     * @return - SQL String
     */
    protected SQLQuery _getReviewableRelationMembersQuery() {

        QCurrentRelationMembers currentRelationMembers = QCurrentRelationMembers.currentRelationMembers;
        QCurrentRelations currentRelations = QCurrentRelations.currentRelations;

        ListSubQuery<Tuple> reviewableCurrentRelSubQ = new SQLSubQuery().from(currentRelations)
                .where(Expressions.booleanTemplate("exist(tags,'hoot:review:needs')"))
                .list(currentRelations.id, Expressions.stringTemplate("tags->'hoot:review:needs'").as("needreview"));

        ListSubQuery<Tuple> currentRelMembersSubQ = new SQLSubQuery().from(currentRelationMembers)
                .where(currentRelationMembers.memberType.eq(DbUtils.nwr_enum.relation))
                .list(currentRelationMembers.memberId, currentRelationMembers.relationId,
                        currentRelationMembers.memberType);

        return new SQLQuery(getConnection(), DbUtils.getConfiguration(this.getMapId()))
                .join(currentRelMembersSubQ, _currentRelMembersSubQPath)
                .join(reviewableCurrentRelSubQ, _reviewableCurrentRelSubQPath)
                .on(Expressions.path(Long.class, _currentRelMembersSubQPath, "relation_id")
                        .eq(Expressions.path(Long.class, _reviewableCurrentRelSubQPath, "id")))
                .groupBy(Expressions.path(Long.class, _currentRelMembersSubQPath, "member_id"),
                        Expressions.path(Long.class, _currentRelMembersSubQPath, "relation_id"),
                        Expressions.path(String.class, _reviewableCurrentRelSubQPath, "needreview"));
    }

}
