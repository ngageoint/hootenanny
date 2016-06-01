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

import java.lang.reflect.InvocationTargetException;
import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import org.apache.commons.lang3.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.mysema.query.sql.SQLQuery;

import hoot.services.controllers.osm.MapResource;
import hoot.services.db.DbUtils;
import hoot.services.db2.CurrentRelationMembers;
import hoot.services.db2.QCurrentRelationMembers;
import hoot.services.models.osm.Element;
import hoot.services.models.osm.ElementInfo;
import hoot.services.models.review.ReviewRef;
import hoot.services.review.ReviewUtils;


/**
 * Retrieves element references to reviews for a query element
 */
public class ReviewReferencesRetriever {
    private static final Logger log = LoggerFactory.getLogger(ReviewReferencesRetriever.class);

    private static final QCurrentRelationMembers currentRelationMembers = QCurrentRelationMembers.currentRelationMembers;

    private Connection conn;

    public ReviewReferencesRetriever(final Connection conn) throws Exception {
        this.conn = conn;
    }

    // would like to do this with QueryDSL, rather than straight JDBC, but don't
    // know how to
    // do the tags part of the query with QueryDSL
    private List<Long> getUnresolvedReviewRelations(final ElementInfo queryElementInfo, final long mapId)
            throws SQLException {
        final String currentRelationMembersTableName = "current_relation_members_" + mapId;
        final String currentRelationsTableName = "current_relations_" + mapId;
        String sql = "";
        sql += "select relation_id from " + currentRelationMembersTableName;
        sql += " join " + currentRelationsTableName + " on " + currentRelationMembersTableName + ".relation_id = "
                + currentRelationsTableName + ".id";
        sql += " where " + currentRelationMembersTableName + ".member_id = " + queryElementInfo.getId() + " and "
                + currentRelationMembersTableName + ".member_type = '"
                + Element.elementTypeFromString(queryElementInfo.getType()).toString().toLowerCase() + "' and "
                + currentRelationsTableName + ".tags->'hoot:review:needs' = 'yes'";

        Statement stmt = null;
        ResultSet rs = null;
        List<Long> relationIds = new ArrayList<Long>();
        try {
            stmt = conn.createStatement();
            rs = stmt.executeQuery(sql);

            if (rs == null) {
                throw new SQLException("Error executing getUnresolvedReviewRelations");
            }

            while (rs.next()) {
                relationIds.add(rs.getLong(1));
            }
        }
        finally {
            if (stmt != null) {
                stmt.close();
            }
            if (rs != null) {
                rs.close();
            }
        }

        return relationIds;
    }

    private List<Long> getAllReviewRelations(final ElementInfo queryElementInfo, final long mapId) throws SQLException {
        final String currentRelationMembersTableName = "current_relation_members_" + mapId;
        final String currentRelationsTableName = "current_relations_" + mapId;
        String sql = "";
        sql += "select relation_id from " + currentRelationMembersTableName;
        sql += " join " + currentRelationsTableName + " on " + currentRelationMembersTableName + ".relation_id = "
                + currentRelationsTableName + ".id";
        sql += " where " + currentRelationMembersTableName + ".member_id = " + queryElementInfo.getId() + " and "
                + currentRelationMembersTableName + ".member_type = '"
                + Element.elementTypeFromString(queryElementInfo.getType()).toString().toLowerCase() + "'";

        Statement stmt = null;
        ResultSet rs = null;
        List<Long> relationIds = new ArrayList<Long>();
        try {
            stmt = conn.createStatement();
            rs = stmt.executeQuery(sql);
            if (rs == null) {
                throw new SQLException("Error executing getAllReviewRelations");
            }
            while (rs.next()) {
                relationIds.add(rs.getLong(1));
            }
        }
        finally {
            if (rs != null) {
                rs.close();
            }
            if (stmt != null) {
                stmt.close();
            }
        }

        return relationIds;
    }

    /**
     * Retrieves all other unresolved element references to reviews for a given
     * element
     * 
     * @param queryElementInfo
     *            element whose review references are to be retrieved
     * @return a list containing all features the input feature needs to be
     *         reviewed with
     * @throws Exception
     * @throws InvocationTargetException
     * @throws NoSuchMethodException
     * @throws ClassNotFoundException
     * @throws IllegalAccessException
     * @throws InstantiationException
     */
    public List<ReviewRef> getUnresolvedReferences(final ElementInfo queryElementInfo)
            throws InstantiationException, IllegalAccessException, ClassNotFoundException, NoSuchMethodException,
            InvocationTargetException, Exception {
        log.debug("requestingElementInfo: " + queryElementInfo.toString());
        List<ReviewRef> references = new ArrayList<ReviewRef>();

        final long mapIdNum = MapResource.validateMap(queryElementInfo.getMapId(), conn);
        assert (mapIdNum != -1);

        // check for query element existence
        Set<Long> elementIds = new HashSet<Long>();
        elementIds.add(queryElementInfo.getId());
        if (StringUtils.trimToNull(queryElementInfo.getType()) == null || !Element.allElementsExist(mapIdNum,
                Element.elementTypeFromString(queryElementInfo.getType()), elementIds, conn)) {
            ReviewUtils.handleError(new Exception("Element with ID: " + queryElementInfo + " and type: "
                    + queryElementInfo.getType() + " does not exist."), "", false);
        }

        // select all unresolved review relation id's from current relation
        // members where member id =
        // requesting element's member id and the element type = the requesting
        // element type
        final List<Long> unresolvedReviewRelationIds = getUnresolvedReviewRelations(queryElementInfo, mapIdNum);

        if (unresolvedReviewRelationIds.size() > 0) {
            // select all relation members in the group of remaining unresolved
            // relation id's where the
            // member's id is not equal to the requesting element's id and the
            // member's type is not = to
            // the requesting element's type
            final List<CurrentRelationMembers> referencedMembers = new SQLQuery(conn,
                    DbUtils.getConfiguration(mapIdNum))
                            .from(currentRelationMembers)
                            .where(currentRelationMembers.relationId.in(unresolvedReviewRelationIds)
                                    .and(currentRelationMembers.memberId.ne(queryElementInfo.getId())
                                            .or(currentRelationMembers.memberType
                                                    .ne(Element.elementEnumFromString(queryElementInfo.getType())))))
                            .orderBy(currentRelationMembers.relationId.asc(), currentRelationMembers.memberId.asc(),
                                    currentRelationMembers.sequenceId.asc())
                            .list(currentRelationMembers);

            // return all elements corresponding to the filtered down set of
            // relation members
            for (CurrentRelationMembers member : referencedMembers) {
                references.add(new ReviewRef(queryElementInfo.getMapId(), member.getMemberId(),
                        Element.elementTypeForElementEnum(member.getMemberType()).toString().toLowerCase(),
                        member.getRelationId()));
            }
        }

        return references;
    }

    /**
     * Retrieves all other unresolved element references to reviews for a given
     * element
     * 
     * @param queryElementInfo
     *            element whose review references are to be retrieved
     * @return a list containing all features the input feature needs to be
     *         reviewed with
     * @throws Exception
     * @throws InvocationTargetException
     * @throws NoSuchMethodException
     * @throws ClassNotFoundException
     * @throws IllegalAccessException
     * @throws InstantiationException
     */
    public List<ReviewRef> getAllReferences(final ElementInfo queryElementInfo)
            throws InstantiationException, IllegalAccessException, ClassNotFoundException, NoSuchMethodException,
            InvocationTargetException, Exception {
        log.debug("requestingElementInfo: " + queryElementInfo.toString());
        List<ReviewRef> references = new ArrayList<ReviewRef>();

        final long mapIdNum = MapResource.validateMap(queryElementInfo.getMapId(), conn);
        assert (mapIdNum != -1);

        // check for query element existence
        Set<Long> elementIds = new HashSet<Long>();
        elementIds.add(queryElementInfo.getId());
        if (StringUtils.trimToNull(queryElementInfo.getType()) == null || !Element.allElementsExist(mapIdNum,
                Element.elementTypeFromString(queryElementInfo.getType()), elementIds, conn)) {
            ReviewUtils.handleError(new Exception("Element with ID: " + queryElementInfo + " and type: "
                    + queryElementInfo.getType() + " does not exist."), "", false);
        }

        // select all review relation id's from current relation members where
        // member id =
        // requesting element's member id and the element type = the requesting
        // element type
        final List<Long> allReviewRelationIds = getAllReviewRelations(queryElementInfo, mapIdNum);

        if (allReviewRelationIds.size() > 0) {
            // select all relation members where the
            // member's id is not equal to the requesting element's id and the
            // member's type is not = to
            // the requesting element's type
            final List<CurrentRelationMembers> referencedMembers = new SQLQuery(conn,
                    DbUtils.getConfiguration(mapIdNum)).from(currentRelationMembers)
                            .where(currentRelationMembers.relationId.in(allReviewRelationIds))
                            .orderBy(currentRelationMembers.relationId.asc(), currentRelationMembers.memberId.asc(),
                                    currentRelationMembers.sequenceId.asc())
                            .list(currentRelationMembers);

            // return all elements corresponding to the filtered down set of
            // relation members
            for (CurrentRelationMembers member : referencedMembers) {
                references.add(new ReviewRef(queryElementInfo.getMapId(), member.getMemberId(),
                        Element.elementTypeForElementEnum(member.getMemberType()).toString().toLowerCase(),
                        member.getRelationId()));

            }
        }

        return references;
    }
}