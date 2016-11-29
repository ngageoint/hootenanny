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
package hoot.services.controllers.review;

import static com.querydsl.sql.SQLExpressions.select;
import static hoot.services.HootProperties.MAX_QUERY_NODES;
import static hoot.services.models.db.QCurrentNodes.currentNodes;
import static hoot.services.models.db.QCurrentRelationMembers.currentRelationMembers;
import static hoot.services.models.db.QCurrentRelations.currentRelations;
import static hoot.services.models.db.QCurrentWayNodes.currentWayNodes;
import static hoot.services.utils.DbUtils.createQuery;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.querydsl.core.Tuple;
import com.querydsl.core.types.Path;
import com.querydsl.core.types.dsl.Expressions;
import com.querydsl.sql.SQLQuery;

import hoot.services.geo.BoundingBox;
import hoot.services.models.db.CurrentRelationMembers;
import hoot.services.utils.DbUtils;


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
class AllReviewableItemsQuery extends ReviewableQueryAbstract {
    private static final Logger logger = LoggerFactory.getLogger(AllReviewableItemsQuery.class);
    private static final Path<Void> currentRelMembersSubQPath = Expressions.path(Void.class, "currentRelMembersSubQ");
    private static final Path<Void> reviewableCurrentRelSubQPath = Expressions.path(Void.class, "reviewableCurrentRelSubQ");

    private final BoundingBox bbox;

    static final long MAX_RESULT_SIZE = Long.parseLong(MAX_QUERY_NODES);

    AllReviewableItemsQuery(long mapid, BoundingBox bbox) {
        super(mapid);
        this.bbox = bbox;
    }

    @Override
    public ReviewQueryMapper execQuery() {
        // get counts for each element types of all review relation members
        Map<Long, ReviewableItemBboxInfo> allReviewables = new HashMap<>();
        long nodeCount = getReviewableRelationMembersCount(DbUtils.nwr_enum.node);
        long wayCount = getReviewableRelationMembersCount(DbUtils.nwr_enum.way);
        long relationCount = getReviewableRelationMembersCount(DbUtils.nwr_enum.relation);

        boolean isPastLimit = false;

        // filter down to element type. Don't bother to run query on non
        // existent element type
        if ((nodeCount > 0) && !isPastLimit) {
            // get the node members bbox grouped by review relation_id
            Map<Long, ReviewableItemBboxInfo> reviewRelationWithNodeMembers =
                    getReviewableRelatioWithNodeMembersCentroidInBbox();

            // merge bbox
            isPastLimit = combineBbox(allReviewables, reviewRelationWithNodeMembers);
        }

        if ((wayCount > 0) && !isPastLimit) {
            // get the way members bbox grouped by review relation_id
            Map<Long, ReviewableItemBboxInfo> reviewRelationWithWayMembers =
                    getReviewableRelatioWithWayMembersCentroidInBbox();

            isPastLimit = combineBbox(allReviewables, reviewRelationWithWayMembers);
        }

        if ((relationCount > 0) && !isPastLimit) {
            // for relation, we need do recursive calculation for bbox since
            // relation can contain another relation
            Map<Long, BoundingBox> relsBbox = new HashMap<>();
            List<Tuple> result = getReviewableRelationMembers();

            // since multiple relation can contain same member
            // we want have unique member relations bbox
            for (Tuple row : result) {
                Long memberId = row.get(Expressions.path(Long.class, currentRelMembersSubQPath, "member_id"));
                if (!relsBbox.containsKey(memberId)) {
                    ReviewableItemBbox bbxInfo = getRelationMemberBbox(memberId);
                    if (bbxInfo != null) {
                        relsBbox.put(memberId, bbxInfo.getBbox());
                    }
                }
            }

            // now organize the reviews with relation members
            Map<Long, ReviewableItemBboxInfo> reviewRelationWithRelationMembers = new HashMap<>();

            for (Tuple row : result) {
                Long relationId = row.get(Expressions.path(Long.class, currentRelMembersSubQPath, "relation_id"));
                Long memberId = row.get(Expressions.path(Long.class, currentRelMembersSubQPath, "member_id"));
                String needReview = row.get(Expressions.path(String.class, reviewableCurrentRelSubQPath, "needreview"));

                BoundingBox memberBbox = relsBbox.get(memberId);

                // we have relation member bbox and it is within the
                // requested bbox so if the relation member bbox exists in map (meaning
                // relation has multiple relation members) then expand the bbox else just add to the map
                if ((memberBbox != null) && bbox.intersects(memberBbox)) {
                    ReviewableItemBboxInfo relBboxInfo = reviewRelationWithRelationMembers.get(relationId);

                    if (relBboxInfo != null) {
                        BoundingBox relBbox = relBboxInfo.getBbox();
                        relBbox.add(memberBbox);
                    }
                    else {
                        ReviewableItemBboxInfo newInfo =
                                new ReviewableItemBboxInfo(memberBbox, getMapId(), relationId, needReview);
                        reviewRelationWithRelationMembers.put(relationId, newInfo);
                    }
                }
            }

            isPastLimit = combineBbox(allReviewables, reviewRelationWithRelationMembers);
        }

        AllReviewableItems allReviewableItems = new AllReviewableItems(getMapId(), allReviewables);
        allReviewableItems.setOverflow(isPastLimit);

        return allReviewableItems;
    }

    /**
     * Using ReviewableBboxQuery do recursive calculation of relation bbox
     * 
     * @param relId
     *            - target relation id
     * @return ReviewableItemBbox
     */
    private ReviewableItemBbox getRelationMemberBbox(long relId) {
        ReviewableBboxQuery reviewableBboxQuery = new ReviewableBboxQuery(getMapId(), relId);
        return (ReviewableItemBbox) reviewableBboxQuery.execQuery();
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
        for (Map.Entry<Long, ReviewableItemBboxInfo> pair : reviewRelationWithMembers.entrySet()) {
            ReviewableItemBboxInfo currInfo = pair.getValue();
            Long currRelId = pair.getKey();
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
    private SQLQuery<CurrentRelationMembers> getReviewableRelationMembersCountByTypeQuery(DbUtils.nwr_enum memberType) {
        return createQuery(getMapId())
                .select(currentRelationMembers)
                .from(currentRelationMembers)
                .where(currentRelationMembers.relationId
                        .in(
                                select(currentRelations.id)
                                .from(currentRelations)
                                .where(Expressions.booleanTemplate("exist(tags,'hoot:review:needs')")))
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
        return getReviewableRelationMembersCountByTypeQuery(type).fetchCount();
    }

    /**
     * Helper function to translation java.sql.ResultSet to BoundingBox
     * 
     * @return - BoundingBox
     */
    private static BoundingBox resultSetToBbox(Tuple tup, Path<Void> bboxPath) {
        Double minLat = tup.get(Expressions.numberPath(Double.class, bboxPath, "minlat"));
        Double maxLat = tup.get(Expressions.numberPath(Double.class, bboxPath, "maxlat"));
        Double minLon = tup.get(Expressions.numberPath(Double.class, bboxPath, "minlon"));
        Double maxLon = tup.get(Expressions.numberPath(Double.class, bboxPath, "maxlon"));

        return new BoundingBox(minLon, minLat, maxLon, maxLat);
    }

    /**
     * Retrieves relation_id, needreview, minlat, maxlat, minlon, maxlon of
     * reviewable item relation way members group by relation_id. The bbox of
     * all members of each relation is calculated and then the centroid value is
     * compare with the user specified bounding box and returns only ones that
     * intersets.
     * 
     * @return - Map of ReviewableItemBboxInfo which contains relation and its bbox information
     */
    private Map<Long, ReviewableItemBboxInfo> getReviewableRelatioWithWayMembersCentroidInBbox() {
        Map<Long, ReviewableItemBboxInfo> relationBboxMap = new HashMap<>();

        Path<Void> reviewRelWayMembersCentroidSubQPath = Expressions.path(Void.class, "reviewRelWayMembersCentroidSubQ");
        List<Tuple> result = getReviewableRelatioWithWayMembersCentroidInBboxQuery().fetch();

        for (Tuple row : result) {
            Long relationId = row.get(Expressions.path(Long.class, reviewRelWayMembersCentroidSubQPath, "relation_id"));
            String needReview = row.get(Expressions.path(String.class, reviewRelWayMembersCentroidSubQPath, "needreview"));
            BoundingBox bbox = resultSetToBbox(row, reviewRelWayMembersCentroidSubQPath);
            ReviewableItemBboxInfo info = new ReviewableItemBboxInfo(bbox, getMapId(), relationId, needReview);
            relationBboxMap.put(relationId, info);
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
     */
    private SQLQuery<Tuple> getReviewableRelatioWithWayMembersCentroidInBboxQuery() {
        if (bbox == null) {
            throw new IllegalArgumentException("Invalid Bounding box.");
        }

        SQLQuery<Tuple> reviewableCurrentRelSubQ =
                select(currentRelations.id,
                        Expressions.stringTemplate("tags->'hoot:review:needs'").as("needreview"))
                .from(currentRelations)
                .where(Expressions.booleanTemplate("exist(tags,'hoot:review:needs')"));

        SQLQuery<Tuple> currentRelMembersSubQ =
                select(currentRelationMembers.memberId,
                        currentRelationMembers.relationId,
                        currentRelationMembers.memberType)
                .from(currentRelationMembers)
                .where(currentRelationMembers.memberType.eq(DbUtils.nwr_enum.way));

        Path<Void> reviewRelJoinRelMemberSubQPath = Expressions.path(Void.class, "reviewRelJoinRelMemberSubQ");

        SQLQuery<Tuple> reviewRelJoinRelMemberSubQ =
                select(Expressions.path(Long.class, currentRelMembersSubQPath, "member_id"),
                       Expressions.path(Long.class, currentRelMembersSubQPath, "relation_id"),
                       Expressions.path(String.class, reviewableCurrentRelSubQPath, "needreview"))
                .join(currentRelMembersSubQ, currentRelMembersSubQPath)
                .join(reviewableCurrentRelSubQ, reviewableCurrentRelSubQPath)
                .on(Expressions.path(Long.class, currentRelMembersSubQPath, "relation_id")
                        .eq(Expressions.path(Long.class, reviewableCurrentRelSubQPath, "id")));

        Path<Void> currentWayNodesSubQPath = Expressions.path(Void.class, "currentWayNodesSubQ");

        SQLQuery<Tuple> currentWayNodesSubQ =
                select(currentWayNodes.nodeId, currentWayNodes.wayId)
                .from(currentWayNodes);

        Path<Void> reviewRelJoinRelMemberJoinCurrentWayNodesSubQPath =
                Expressions.path(Void.class, "reviewRelJoinRelMemberJoinCurrentWayNodesSubQ");

        SQLQuery<Tuple> reviewRelJoinRelMemberJoinCurrentWayNodesSubQ =
                select(Expressions.path(Long.class, currentWayNodesSubQPath, "node_id"),
                        Expressions.path(Long.class, reviewRelJoinRelMemberSubQPath, "relation_id"),
                        Expressions.path(String.class, reviewRelJoinRelMemberSubQPath, "needreview"))
                .join(currentWayNodesSubQ, currentWayNodesSubQPath)
                .join(reviewRelJoinRelMemberSubQ, reviewRelJoinRelMemberSubQPath)
                .on(Expressions.path(Long.class, currentWayNodesSubQPath, "way_id")
                        .eq(Expressions.path(Long.class, reviewRelJoinRelMemberSubQPath, "member_id")));

        Path<Void> currentNodeSubQPath = Expressions.path(Void.class, "currentNodeSubQ");

        SQLQuery<Tuple> currentNodesSubQ =
                select(currentNodes.id, currentNodes.latitude, currentNodes.longitude)
                .from(currentNodes);

        Path<Void> reviewRelWayMembersCentroidSubQPath = Expressions.path(Void.class, "reviewRelWayMembersCentroidSubQ");

        SQLQuery<Tuple> reviewRelWayMembersCentroidSubQ =
                select(Expressions.path(Long.class, reviewRelJoinRelMemberJoinCurrentWayNodesSubQPath, "relation_id"),
                        Expressions.path(String.class, reviewRelJoinRelMemberJoinCurrentWayNodesSubQPath, "needreview"),
                        Expressions.numberPath(Double.class, currentNodeSubQPath, "latitude").max().as("maxlat"),
                        Expressions.numberPath(Double.class, currentNodeSubQPath, "latitude").min().as("minlat"),
                        Expressions.numberPath(Double.class, currentNodeSubQPath, "longitude").max().as("maxlon"),
                        Expressions.numberPath(Double.class, currentNodeSubQPath, "longitude").min().as("minlon"),
                        Expressions.numberTemplate(Double.class,
                                "(((max(\"currentNodeSubQ\".\"latitude\") - min(\"currentNodeSubQ\".\"latitude\"))/2)+min(\"currentNodeSubQ\".\"latitude\"))")
                                .as("centlat"),
                        Expressions.numberTemplate(Double.class,
                                "(((max(\"currentNodeSubQ\".\"longitude\") - min(\"currentNodeSubQ\".\"longitude\"))/2)+min(\"currentNodeSubQ\".\"longitude\"))")
                                .as("centlon"))
                .join(currentNodesSubQ, currentNodeSubQPath)
                .join(reviewRelJoinRelMemberJoinCurrentWayNodesSubQ, reviewRelJoinRelMemberJoinCurrentWayNodesSubQPath)
                .on(Expressions.path(Long.class, currentNodeSubQPath, "id")
                        .eq(Expressions.path(Long.class, reviewRelJoinRelMemberJoinCurrentWayNodesSubQPath,
                                "node_id")))
                .groupBy(Expressions.path(Long.class, reviewRelJoinRelMemberJoinCurrentWayNodesSubQPath, "relation_id"),
                        Expressions.path(String.class, reviewRelJoinRelMemberJoinCurrentWayNodesSubQPath,
                                "needreview"));

        SQLQuery<Tuple> query = createQuery(getMapId())
                .select(Expressions.path(Long.class, reviewRelWayMembersCentroidSubQPath, "relation_id"),
                        Expressions.path(String.class, reviewRelWayMembersCentroidSubQPath, "needreview"),
                        Expressions.numberPath(Double.class, reviewRelWayMembersCentroidSubQPath, "maxlat"),
                        Expressions.numberPath(Double.class, reviewRelWayMembersCentroidSubQPath, "minlat"),
                        Expressions.numberPath(Double.class, reviewRelWayMembersCentroidSubQPath, "maxlon"),
                        Expressions.numberPath(Double.class, reviewRelWayMembersCentroidSubQPath, "minlon"))
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

        return query;
    }

    /**
     * Retrieves relation_id, needreview, minlat, maxlat, minlon, maxlon of
     * reviewable item relation node members group by relation_id. The bbox of
     * all members of each relation is calculated and then the centroid value is
     * compare with the user specified bounding box and returns only ones that
     * intersets.
     * 
     * @return - Map of ReviewableItemBboxInfo which contains relation and its bbox information
     */
    private Map<Long, ReviewableItemBboxInfo> getReviewableRelatioWithNodeMembersCentroidInBbox()  {
        Map<Long, ReviewableItemBboxInfo> relationBboxMap = new HashMap<>();

        Path<Void> reviewRelNodeMembersCentroidSubQPath =
                Expressions.path(Void.class, "reviewRelNodeMembersCentroidSubQ");

        List<Tuple> result = getReviewableRelatioWithNodeMembersCentroidInBboxQuery().fetch();

        for (Tuple row : result) {
            Long relId = row.get(Expressions.path(Long.class, reviewRelNodeMembersCentroidSubQPath, "relation_id"));
            String needReview = row.get(Expressions.path(String.class, reviewRelNodeMembersCentroidSubQPath, "needreview"));
            BoundingBox bbox = resultSetToBbox(row, reviewRelNodeMembersCentroidSubQPath);
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
    private SQLQuery<Tuple> getReviewableRelatioWithNodeMembersCentroidInBboxQuery() {
        if (bbox == null) {
            throw new IllegalArgumentException("Invalid Bounding box.");
        }

        SQLQuery<Tuple> reviewableCurrentRelSubQ =
                select(currentRelations.id,
                        Expressions.stringTemplate("tags->'hoot:review:needs'").as("needreview"))
                .from(currentRelations)
                .where(Expressions.booleanTemplate("exist(tags,'hoot:review:needs')"));

        SQLQuery<Tuple> currentRelMembersSubQ =
                select(currentRelationMembers.memberId,
                        currentRelationMembers.relationId,
                        currentRelationMembers.memberType)
                .from(currentRelationMembers)
                .where(currentRelationMembers.memberType.eq(DbUtils.nwr_enum.node));

        Path<Void> reviewRelJoinRelMemberSubQPath = Expressions.path(Void.class, "reviewRelJoinRelMemberSubQ");

        SQLQuery<Tuple> reviewRelJoinRelMemberSubQ =
                select(Expressions.path(Long.class, currentRelMembersSubQPath, "member_id"),
                        Expressions.path(Long.class, currentRelMembersSubQPath, "relation_id"),
                        Expressions.path(String.class, reviewableCurrentRelSubQPath, "needreview"))
                .join(currentRelMembersSubQ, currentRelMembersSubQPath)
                .join(reviewableCurrentRelSubQ, reviewableCurrentRelSubQPath)
                .on(Expressions.path(Long.class, currentRelMembersSubQPath, "relation_id")
                        .eq(Expressions.path(Long.class, reviewableCurrentRelSubQPath, "id")));

        Path<Void> currentNodeSubQPath = Expressions.path(Void.class, "currentNodeSubQ");

        SQLQuery<Tuple> currentNodesSubQ =
                select(currentNodes.id,
                        currentNodes.latitude,
                        currentNodes.longitude)
                .from(currentNodes);

        Path<Void> reviewRelNodeMembersCentroidSubQPath =
                Expressions.path(Void.class, "reviewRelNodeMembersCentroidSubQ");

        SQLQuery<Tuple> reviewRelWayMembersCentroidSubQ =
                select(Expressions.path(Long.class, reviewRelJoinRelMemberSubQPath, "relation_id"),
                        Expressions.path(String.class, reviewRelJoinRelMemberSubQPath, "needreview"),
                        Expressions.numberPath(Double.class, currentNodeSubQPath, "latitude").max().as("maxlat"),
                        Expressions.numberPath(Double.class, currentNodeSubQPath, "latitude").min().as("minlat"),
                        Expressions.numberPath(Double.class, currentNodeSubQPath, "longitude").max().as("maxlon"),
                        Expressions.numberPath(Double.class, currentNodeSubQPath, "longitude").min().as("minlon"),
                        Expressions.numberTemplate(Double.class,
                                "(((max(\"currentNodeSubQ\".\"latitude\") - min(\"currentNodeSubQ\".\"latitude\"))/2)+min(\"currentNodeSubQ\".\"latitude\"))")
                                .as("centlat"),
                        Expressions.numberTemplate(Double.class,
                                "(((max(\"currentNodeSubQ\".\"longitude\") - min(\"currentNodeSubQ\".\"longitude\"))/2)+min(\"currentNodeSubQ\".\"longitude\"))")
                                .as("centlon"))
                .join(currentNodesSubQ, currentNodeSubQPath)
                .join(reviewRelJoinRelMemberSubQ, reviewRelJoinRelMemberSubQPath)
                .on(Expressions.path(Long.class, currentNodeSubQPath, "id")
                        .eq(Expressions.path(Long.class, reviewRelJoinRelMemberSubQPath, "member_id")))
                .groupBy(Expressions.path(Long.class, reviewRelJoinRelMemberSubQPath, "relation_id"),
                        Expressions.path(String.class, reviewRelJoinRelMemberSubQPath, "needreview"));

        SQLQuery<Tuple> query = createQuery(getMapId())
                .select(Expressions.path(Long.class, reviewRelNodeMembersCentroidSubQPath, "relation_id"),
                        Expressions.path(String.class, reviewRelNodeMembersCentroidSubQPath, "needreview"),
                        Expressions.numberPath(Double.class, reviewRelNodeMembersCentroidSubQPath, "maxlat"),
                        Expressions.numberPath(Double.class, reviewRelNodeMembersCentroidSubQPath, "minlat"),
                        Expressions.numberPath(Double.class, reviewRelNodeMembersCentroidSubQPath, "maxlon"),
                        Expressions.numberPath(Double.class, reviewRelNodeMembersCentroidSubQPath, "minlon"))
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

        return query;
    }

    /**
     * Retrieve relation_id, relation_id, needreview of all review relation relation members
     * 
     * @return - List of tuples containing relation_id, member_id, needreview
     */
    private List<Tuple> getReviewableRelationMembers() {
        SQLQuery<Tuple> reviewableCurrentRelSubQ =
                select(currentRelations.id,
                        Expressions.stringTemplate("tags->'hoot:review:needs'").as("needreview"))
                .from(currentRelations)
                .where(Expressions.booleanTemplate("exist(tags,'hoot:review:needs')"));

        SQLQuery<Tuple> currentRelMembersSubQ =
                select(currentRelationMembers.memberId,
                        currentRelationMembers.relationId,
                        currentRelationMembers.memberType)
                .from(currentRelationMembers)
                .where(currentRelationMembers.memberType.eq(DbUtils.nwr_enum.relation));

        SQLQuery<Tuple> reviewableRelationMembersQuery = createQuery(this.getMapId())
                .select(Expressions.path(Long.class, currentRelMembersSubQPath, "member_id"),
                        Expressions.path(Long.class, currentRelMembersSubQPath, "relation_id"),
                        Expressions.path(String.class, reviewableCurrentRelSubQPath, "needreview"))
                .join(currentRelMembersSubQ, currentRelMembersSubQPath)
                .join(reviewableCurrentRelSubQ, reviewableCurrentRelSubQPath)
                        .on(Expressions.path(Long.class, currentRelMembersSubQPath, "relation_id")
                        .eq(Expressions.path(Long.class, reviewableCurrentRelSubQPath, "id")))
                .groupBy(Expressions.path(Long.class, currentRelMembersSubQPath, "member_id"),
                         Expressions.path(Long.class, currentRelMembersSubQPath, "relation_id"),
                         Expressions.path(String.class, reviewableCurrentRelSubQPath, "needreview"));

        return reviewableRelationMembersQuery.fetch();
    }
}
