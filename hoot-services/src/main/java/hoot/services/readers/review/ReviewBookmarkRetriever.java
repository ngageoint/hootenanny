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
package hoot.services.readers.review;

import java.sql.Connection;
import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.mysema.query.sql.SQLQuery;
import com.mysema.query.types.OrderSpecifier;

import hoot.services.db.DbUtils;
import hoot.services.db2.QReviewBookmarks;
import hoot.services.db2.ReviewBookmarks;


public class ReviewBookmarkRetriever {

    private static final Logger log = LoggerFactory.getLogger(ReviewBookmarkRetriever.class);

    private Connection _conn;
    private QReviewBookmarks _reviewBookmarks = QReviewBookmarks.reviewBookmarks;

    public ReviewBookmarkRetriever(final Connection cn) {
        this._conn = cn;
    }

    /**
     * Retrieves all bookmarks for mapId+relationId using map id and relation
     * id.
     */
    public List<ReviewBookmarks> retrieve(final long mapId, final long relationId) throws Exception {
        List<ReviewBookmarks> res = null;
        try {
            SQLQuery query = _getQuery(mapId, relationId);
            res = query.list(_reviewBookmarks);
        }
        catch (Exception ex) {
            log.error(ex.getMessage());
            throw ex;
        }
        return res;
    }

    /**
     * Retrieves all bookmarks for mapId+relationId using map id and relation
     * id.
     */
    public List<ReviewBookmarks> retrieve(final long boookMarkId) throws Exception {
        List<ReviewBookmarks> res = null;
        try {
            SQLQuery query = _getQuery(boookMarkId);
            res = query.list(_reviewBookmarks);
        }
        catch (Exception ex) {
            log.error(ex.getMessage());
            throw ex;
        }
        return res;
    }

    /**
     * Retrieves all review tags
     * 
     * @param orderByCol
     *            - order by column to sort
     * @param isAsc
     *            - is order by asc | desc
     * @param limit
     *            - limit for numbers of returned results
     * @param offset
     *            - offset row for paging
     * @return - list of Review tags
     * @throws Exception
     */
    public List<ReviewBookmarks> retrieveAll(final String orderByCol, final boolean isAsc, final long limit,
            final long offset, final String filterCol, final Object filterVal) throws Exception {

        List<ReviewBookmarks> res = null;
        try {
            SQLQuery query = _getAllQuery(orderByCol, isAsc, limit, offset, filterCol, filterVal);
            res = query.list(_reviewBookmarks);
        }
        catch (Exception ex) {
            log.error(ex.getMessage());
            throw ex;
        }
        return res;
    }

    /**
     * Get the total counts of all review tags
     * 
     * @return - numbers of toal count
     * @throws Exception
     */
    public long getbookmarksCount() throws Exception {
        long res = -1;

        try {
            SQLQuery query = new SQLQuery(this._conn, DbUtils.getConfiguration());
            res = query.from(_reviewBookmarks).count();
        }
        catch (Exception ex) {
            log.error(ex.getMessage());
            throw ex;
        }

        return res;
    }

    /**
     * SQL Query for retrieving review tag
     * 
     * @param mapId
     * @param relationId
     * @return - SQLQuery
     * @throws Exception
     */
    protected SQLQuery _getQuery(final long mapId, final long relationId) throws Exception {
        SQLQuery query = new SQLQuery(this._conn, DbUtils.getConfiguration());
        try {
            query.from(_reviewBookmarks)
                    .where(_reviewBookmarks.mapId.eq(mapId).and(_reviewBookmarks.relationId.eq(relationId)));
        }
        catch (Exception ex) {
            log.error(ex.getMessage());
            throw ex;
        }

        return query;
    }

    protected SQLQuery _getQuery(final long bookmarkId) throws Exception {
        SQLQuery query = new SQLQuery(this._conn, DbUtils.getConfiguration());
        try {
            query.from(_reviewBookmarks).where(_reviewBookmarks.id.eq(bookmarkId));
        }
        catch (Exception ex) {
            log.error(ex.getMessage());
            throw ex;
        }

        return query;
    }

    /**
     * SQL Query for retrieving all tags
     * 
     * @param orderByCol
     * @param isAsc
     * @param limit
     * @param offset
     * @return - SQLQuery
     * @throws Exception
     */
    protected SQLQuery _getAllQuery(final String orderByCol, final boolean isAsc, final long limit, final long offset,
            final String filterCol, final Object filterVal) throws Exception {
        QReviewBookmarks b = QReviewBookmarks.reviewBookmarks;

        SQLQuery query = new SQLQuery(this._conn, DbUtils.getConfiguration());
        try {
            if (filterCol != null && filterVal != null && filterCol.equalsIgnoreCase("createdBy")) {
                query.from(_reviewBookmarks).where(b.createdBy.eq((Long) filterVal))
                        .orderBy(_getSpecifier(orderByCol, isAsc));
            }
            else if (filterCol != null && filterVal != null && filterCol.equalsIgnoreCase("mapId")) {
                query.from(_reviewBookmarks).where(b.mapId.eq((Long) filterVal))
                        .orderBy(_getSpecifier(orderByCol, isAsc));
            }
            else {
                query.from(_reviewBookmarks).orderBy(_getSpecifier(orderByCol, isAsc));
            }

            if (limit > -1) {
                query.limit(limit);
            }

            if (offset > -1) {
                query.offset(offset);
            }

        }
        catch (Exception ex) {
            log.error(ex.getMessage());
            throw ex;
        }

        return query;
    }

    /**
     * Filter for allowed columns for order by
     * 
     * @param orderByCol
     *            - String representation of order by column
     * @param isAsc
     *            - asc | dsc
     * @return - OrderSpecifier
     */
    protected OrderSpecifier _getSpecifier(final String orderByCol, final boolean isAsc) {
        OrderSpecifier res = _reviewBookmarks.id.asc();
        try {
            if (orderByCol != null) {
                switch (orderByCol) {
                    case "id":
                        res = (isAsc) ? _reviewBookmarks.id.asc() : _reviewBookmarks.id.desc();
                    break;
                    case "createdAt":
                        res = (isAsc) ? _reviewBookmarks.createdAt.asc() : _reviewBookmarks.createdAt.desc();
                    break;
                    case "createdBy":
                        res = (isAsc) ? _reviewBookmarks.createdBy.asc() : _reviewBookmarks.createdBy.desc();
                    break;
                    case "lastModifiedAt":
                        res = (isAsc) ? _reviewBookmarks.lastModifiedAt.asc() : _reviewBookmarks.lastModifiedAt.desc();
                    break;
                    case "lastModifiedBy":
                        res = (isAsc) ? _reviewBookmarks.lastModifiedBy.asc() : _reviewBookmarks.lastModifiedBy.desc();
                    break;
                    case "mapId":
                        res = (isAsc) ? _reviewBookmarks.mapId.asc() : _reviewBookmarks.mapId.desc();
                    break;
                    case "relationId":
                        res = (isAsc) ? _reviewBookmarks.relationId.asc() : _reviewBookmarks.relationId.desc();
                    break;
                    default:
                        res = _reviewBookmarks.id.asc();
                    break;
                }
            }
        }
        catch (Exception ex) {
            log.warn("Defualting to reviewBookmarks.id asc order: " + ex.getMessage());
        }

        return res;
    }

}
