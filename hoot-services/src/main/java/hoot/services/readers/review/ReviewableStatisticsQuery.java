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
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.models.review.ReviewQueryMapper;
import hoot.services.models.review.ReviewableStatistics;


/**
 * 
 */
public class ReviewableStatisticsQuery extends ReviewableQueryBase implements IReviewableQuery {
    @SuppressWarnings("unused")
    private static final Logger log = LoggerFactory.getLogger(ReviewableStatisticsQuery.class);

    public ReviewableStatisticsQuery(final Connection c, final long mapid) {
        super(c, mapid);
    }

    protected long getTotalReviewablesCount() throws SQLException, Exception {
        long recordCount = 0;

        try (Statement stmt = getConnection().createStatement();
                ResultSet rs = stmt.executeQuery(_getTotalReviewableCountQueryString())) {
            if (rs == null) {
                throw new SQLException("Error executing getTotalReviewablesCount");
            }

            while (rs.next()) {
                recordCount = rs.getLong("totalcnt");
            }
        }

        return recordCount;
    }

    protected long getRemainingReviewablesCount() throws SQLException, Exception {
        long recordCount = 0;

        try (Statement stmt = getConnection().createStatement();
                ResultSet rs = stmt.executeQuery(_getUnreviewedCountQueryString())) {
            if (rs == null) {
                throw new SQLException("Error executing getRemainingReviewablesCount");
            }

            while (rs.next()) {
                recordCount = rs.getLong("remaining");
            }
        }

        return recordCount;
    }

    @Override
    public ReviewQueryMapper execQuery() throws SQLException, Exception {
        long nTotal = getTotalReviewablesCount();
        long nUnReviewed = getRemainingReviewablesCount();

        ReviewableStatistics ret = new ReviewableStatistics(nTotal, nUnReviewed);
        return ret;
    }

    protected String _getTotalReviewableCountQueryString() {
        return "select count(*) as totalcnt from current_relations_" + getMapId() + " where tags->'type' = 'review'";
    }

    protected String _getUnreviewedCountQueryString() {
        return "select count(*) as remaining from current_relations_" + getMapId()
                + " where tags->'hoot:review:needs' = 'yes'";
    }
}
