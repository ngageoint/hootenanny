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
package hoot.services.writers.review;

import java.sql.Connection;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.mysema.query.sql.Configuration;
import com.mysema.query.sql.dml.SQLDeleteClause;

import hoot.services.db.DbUtils;
import hoot.services.db2.QReviewBookmarks;
import hoot.services.models.review.ReviewBookmarkDelRequest;


public class ReviewBookmarksRemover {
    @SuppressWarnings("unused")
    private static final Logger log = LoggerFactory.getLogger(ReviewBookmarksRemover.class);

    private Connection _conn;

    public ReviewBookmarksRemover(final Connection cn) {
        _conn = cn;
    }

    /**
     * Removes review tag.
     * 
     * @param request
     *            - Request containing mapid and relationid
     * @return - total numbers of removed
     * @throws Exception
     */
    public long remove(final ReviewBookmarkDelRequest request) throws Exception {
        return _createDelClause(request).execute();
    }

    /**
     * Delete clause
     * 
     * @param request
     *            - Request containing bookmarkid
     * @return - toal numbers of removed
     * @throws Exception
     */
    protected SQLDeleteClause _createDelClause(final ReviewBookmarkDelRequest request) throws Exception {
        QReviewBookmarks reviewBookmarks = QReviewBookmarks.reviewBookmarks;
        Configuration configuration = DbUtils.getConfiguration();
        return new SQLDeleteClause(_conn, configuration, reviewBookmarks)
                .where(reviewBookmarks.id.eq(request.getBookmarkId()));
    }
}
