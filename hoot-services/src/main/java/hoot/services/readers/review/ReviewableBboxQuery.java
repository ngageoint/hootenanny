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
import java.util.ArrayList;
import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.mysema.query.Tuple;
import com.mysema.query.sql.SQLQuery;
import com.mysema.query.sql.SQLSubQuery;
import com.mysema.query.types.query.ListSubQuery;

import hoot.services.db.DbUtils;
import hoot.services.db2.QCurrentNodes;
import hoot.services.db2.QCurrentRelationMembers;
import hoot.services.db2.QCurrentWayNodes;
import hoot.services.geo.BoundingBox;
import hoot.services.models.review.ReviewQueryMapper;
import hoot.services.models.review.ReviewableItemBbox;


/**
 * The purpose of this class is to retrieve the bounding box of all members of
 * specified relation combined recursively.
 *
 */
public class ReviewableBboxQuery extends ReviewableQueryBase implements IReviewableQuery {
    private static final Logger logger = LoggerFactory.getLogger(ReviewableBboxQuery.class);

    private final Long relationId;

    public ReviewableBboxQuery(Connection connection, long mapid, long relationid) {
        super(connection, mapid);
        relationId = relationid;
    }

    @Override
    public ReviewQueryMapper execQuery() {
        BoundingBox currBbox = new BoundingBox();
        ReviewableItemBbox ret = new ReviewableItemBbox(currBbox, getMapId(), relationId);
        List<BoundingBox> membersBboxList = new ArrayList<>();

        // do recursive bbox retrieval since relation may contain other relation
        List<Long> relIds = new ArrayList<>();
        relIds.add(relationId);
        getRelationMembersBboxRecursive(membersBboxList, relIds);

        for (BoundingBox bbx : membersBboxList) {
            currBbox.add(bbx);
        }

        return ret;
    }

    /**
     * This function gets bbox of node and way members and add to members list.
     * If the relation contain relation member then it recurse.
     * 
     * @param membersBboxList
     *            - storage containing bbox of all member elements
     * @param relIds
     *            - relation ids of relation members
     */
    private void getRelationMembersBboxRecursive(List<BoundingBox> membersBboxList, List<Long> relIds) {
        for (Long relId : relIds) {
            BoundingBox nodeMemBbox = getRelationNodeMembersBbox(relId);
            if (nodeMemBbox != null) {
                membersBboxList.add(nodeMemBbox);
            }

            BoundingBox wayMemBbox = getRelationWayMembersBbox(relId);
            if (wayMemBbox != null) {
                membersBboxList.add(wayMemBbox);
            }

            List<Long> memRelationIds = getRelationMembers(relId);
            if (!memRelationIds.isEmpty()) {
                getRelationMembersBboxRecursive(membersBboxList, memRelationIds);
            }
        }
    }

    private static boolean validateTuple(Tuple tup) {
        boolean ret = true;

        int nCols = tup.size();
        for (int i = 0; i < nCols; i++) {
            Object o = tup.get(i, Object.class);
            if (o == null) {
                ret = false;
                break;
            }
        }

        return ret;
    }

    /**
     * Helper function to translation Tuple to Bonding box
     * 
     * @param tup
     *            - source Tuple
     * @return - BoundingBox
     */
    private static BoundingBox resultSetToBbox(Tuple tup) {
        BoundingBox bbox = null;
        try {
            if (validateTuple(tup)) {
                double minLat = tup.get(0, Double.class);
                double maxLat = tup.get(1, Double.class);
                double minLon = tup.get(2, Double.class);
                double maxLon = tup.get(3, Double.class);

                bbox = new BoundingBox(minLon, minLat, maxLon, maxLat);
            }
        }
        catch (Exception ignored) {
            // we will not throw error since ret will be null and null ret
            // will be handled gracefully by caller.
            logger.error("Error during tuple to bounding box conversion!", ignored);
        }

        return bbox;
    }

    /**
     * Retrieves relation way member's bbox.
     * 
     * @param relationId
     *            - container relation id
     * @return - BoundingBox
     */
    private BoundingBox getRelationWayMembersBbox(long relationId) {
        BoundingBox bbox = null;

        QCurrentNodes currentNodes = QCurrentNodes.currentNodes;
        SQLQuery sql = getRelationWayMembersBboxQuery(relationId);
        List<Tuple> tups = sql.list(currentNodes.latitude.min(), currentNodes.latitude.max(),
                currentNodes.longitude.min(), currentNodes.longitude.min());

        for (Tuple tup : tups) {
            bbox = resultSetToBbox(tup);
        }

        return bbox;
    }

    /**
     * Query for retrieving relation way member's bbox.
     * 
     * @param relationId
     *            - container relation id
     * @return - com.mysema.query.sql.SQLQuery object
     */
    protected SQLQuery getRelationWayMembersBboxQuery(long relationId) {
        QCurrentRelationMembers currentRelationMembers = QCurrentRelationMembers.currentRelationMembers;
        ListSubQuery<Long> sub = new SQLSubQuery().from(currentRelationMembers)
                .where(currentRelationMembers.relationId.eq(relationId)
                        .and(currentRelationMembers.memberType.eq(DbUtils.nwr_enum.way)))
                .list(currentRelationMembers.memberId);

        QCurrentWayNodes currentWayNodes = QCurrentWayNodes.currentWayNodes;
        ListSubQuery<Long> wayNodesSub = new SQLSubQuery().from(currentWayNodes).where(currentWayNodes.wayId.in(sub))
                .list(currentWayNodes.nodeId);

        QCurrentNodes currentNodes = QCurrentNodes.currentNodes;
        return new SQLQuery(this.getConnection(), DbUtils.getConfiguration(this.getMapId())).from(currentNodes)
                .where(currentNodes.id.in(wayNodesSub));
    }

    /**
     * Retrieves relation node member's bbox.
     * 
     * @param relationId
     *            - container relation id
     * @return - BoundingBox
     */
    private BoundingBox getRelationNodeMembersBbox(long relationId) {
        BoundingBox bbox = null;
        QCurrentNodes currentNodes = QCurrentNodes.currentNodes;
        SQLQuery sql = getRelationNodeMembersBboxQuery(relationId);
        List<Tuple> tups = sql.list(currentNodes.latitude.min(), currentNodes.latitude.max(),
                currentNodes.longitude.min(), currentNodes.longitude.min());

        for (Tuple tup : tups) {
            bbox = resultSetToBbox(tup);
        }

        return bbox;
    }

    /**
     * Query for retrieving relation node member's bbox.
     * 
     * @param relationId
     *            - container relation id
     * @return - com.mysema.query.sql.SQLQuery object
     */
    protected SQLQuery getRelationNodeMembersBboxQuery(long relationId) {
        QCurrentRelationMembers currentRelationMembers = QCurrentRelationMembers.currentRelationMembers;
        ListSubQuery<Long> sub = new SQLSubQuery().from(currentRelationMembers)
                .where(currentRelationMembers.relationId.eq(relationId)
                        .and(currentRelationMembers.memberType.eq(DbUtils.nwr_enum.node)))
                .list(currentRelationMembers.memberId);

        QCurrentNodes currentNodes = QCurrentNodes.currentNodes;
        return new SQLQuery(this.getConnection(), DbUtils.getConfiguration(this.getMapId())).from(currentNodes)
                .where(currentNodes.id.in(sub));
    }

    /**
     * Retrieves all relation members of a relation.
     * 
     * @param relationId
     *            - container relation id
     * @return - List of relation member ids
     */
    private List<Long> getRelationMembers(long relationId) {
        QCurrentRelationMembers currentRelationMembers = QCurrentRelationMembers.currentRelationMembers;
        List<Long> relMemberIds = getRelationMembersQuery(relationId).list(currentRelationMembers.memberId);
        return relMemberIds;
    }

    /**
     * Query for retrieving all relation members of a relation.
     * 
     * @param relationId
     *            - container relation id
     * @return - com.mysema.query.sql.SQLQuery object
     */
    protected SQLQuery getRelationMembersQuery(long relationId) {
        QCurrentRelationMembers currentRelationMembers = QCurrentRelationMembers.currentRelationMembers;
        return new SQLQuery(this.getConnection(), DbUtils.getConfiguration(this.getMapId()))
                .from(currentRelationMembers).where(currentRelationMembers.relationId.eq(relationId)
                        .and(currentRelationMembers.memberType.eq(DbUtils.nwr_enum.relation)));
    }
}
