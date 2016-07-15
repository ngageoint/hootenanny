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

import hoot.services.utils.DbUtils;
import hoot.services.db2.QReviewBookmarks;
import hoot.services.db2.ReviewBookmarks;


public class ReviewBookmarkRetriever {
    private static final Logger logger = LoggerFactory.getLogger(ReviewBookmarkRetriever.class);

    private final Connection conn;
    private final QReviewBookmarks reviewBookmarks = QReviewBookmarks.reviewBookmarks;

    public ReviewBookmarkRetriever(Connection cn) {
        this.conn = cn;
    }

    /**
     * Retrieves all bookmarks for mapId+relationId using map id and relation id.
     */
    public List<ReviewBookmarks> retrieve(long mapId, long relationId) {
        SQLQuery query = getQuery(mapId, relationId);
        List<ReviewBookmarks> res = query.list(reviewBookmarks);
        return res;
    }

    /**
     * Retrieves all bookmarks for mapId+relationId using map id and relation id.
     */
    public List<ReviewBookmarks> retrieve(long boookMarkId) {
        SQLQuery query = getQuery(boookMarkId);
        List<ReviewBookmarks> res = query.list(reviewBookmarks);
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
     */
    public List<ReviewBookmarks> retrieveAll(String orderByCol, boolean isAsc, long limit,
            long offset, Long[] creatorArray, Long[] layerArray) {
        SQLQuery query = getAllQuery(orderByCol, isAsc, limit, offset, creatorArray, layerArray);
        List<ReviewBookmarks> res = query.list(reviewBookmarks);
        return res;
    }
    /**
     * Get the total counts of all review tags
     * 
     * @return - numbers of toal count
     */
    public long getbookmarksCount() {
        SQLQuery query = new SQLQuery(this.conn, DbUtils.getConfiguration());
        long res = query.from(reviewBookmarks).count();
        return res;
    }

    /**
     * SQL Query for retrieving review tag
     * 
     * @param mapId
     * @param relationId
     * @return - SQLQuery
     */
    protected SQLQuery getQuery(long mapId, long relationId) {
        SQLQuery query = new SQLQuery(this.conn, DbUtils.getConfiguration());
        query.from(reviewBookmarks)
             .where(reviewBookmarks.mapId.eq(mapId).and(reviewBookmarks.relationId.eq(relationId)));
        return query;
    }

    protected SQLQuery getQuery(long bookmarkId) {
        SQLQuery query = new SQLQuery(this.conn, DbUtils.getConfiguration());
        query.from(reviewBookmarks).where(reviewBookmarks.id.eq(bookmarkId));
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
     */
    protected SQLQuery getAllQuery(String orderByCol, boolean isAsc, long limit, long offset,
            Long[] creatorArray, Long[] layerArray) {
        QReviewBookmarks b = QReviewBookmarks.reviewBookmarks;

        SQLQuery query = new SQLQuery(this.conn, DbUtils.getConfiguration());
        if ((creatorArray != null) && (layerArray != null)) {
            query.from(reviewBookmarks).where(b.createdBy.in(creatorArray))
                    .where(b.mapId.in(layerArray))
                    .orderBy(getSpecifier(orderByCol, isAsc));
        }
        else if ((creatorArray != null) && (layerArray == null)) {
            query.from(reviewBookmarks).where(b.createdBy.in(creatorArray))
                    .orderBy(getSpecifier(orderByCol, isAsc));
        }
        else if ((creatorArray == null) && (layerArray != null)) {
            query.from(reviewBookmarks).where(b.mapId.in(layerArray))
                    .orderBy(getSpecifier(orderByCol, isAsc));
        }
        else {
            query.from(reviewBookmarks).orderBy(getSpecifier(orderByCol, isAsc));
        }

        if (limit > -1) {
            query.limit(limit);
        }

        if (offset > -1) {
            query.offset(offset);
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
    private OrderSpecifier getSpecifier(String orderByCol, boolean isAsc) {
        OrderSpecifier res = reviewBookmarks.id.asc();
        if (orderByCol != null) {
            switch (orderByCol) {
                case "id":
                    res = (isAsc) ? reviewBookmarks.id.asc() : reviewBookmarks.id.desc();
                break;
                case "createdAt":
                    res = (isAsc) ? reviewBookmarks.createdAt.asc() : reviewBookmarks.createdAt.desc();
                break;
                case "createdBy":
                    res = (isAsc) ? reviewBookmarks.createdBy.asc() : reviewBookmarks.createdBy.desc();
                break;
                case "lastModifiedAt":
                    res = (isAsc) ? reviewBookmarks.lastModifiedAt.asc() : reviewBookmarks.lastModifiedAt.desc();
                break;
                case "lastModifiedBy":
                    res = (isAsc) ? reviewBookmarks.lastModifiedBy.asc() : reviewBookmarks.lastModifiedBy.desc();
                break;
                case "mapId":
                    res = (isAsc) ? reviewBookmarks.mapId.asc() : reviewBookmarks.mapId.desc();
                break;
                case "relationId":
                    res = (isAsc) ? reviewBookmarks.relationId.asc() : reviewBookmarks.relationId.desc();
                break;
                default:
                    res = reviewBookmarks.id.asc();
                break;
            }
        }

        return res;
    }
}
