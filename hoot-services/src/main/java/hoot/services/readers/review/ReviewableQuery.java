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
import java.sql.Statement;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.models.review.ReviewQueryMapper;
import hoot.services.models.review.ReviewableItem;


public class ReviewableQuery extends ReviewableQueryBase implements IReviewableQuery {
    private static final Logger logger = LoggerFactory.getLogger(ReviewableQuery.class);
    private long seqId = -1;

    public ReviewableQuery(Connection connection, long mapid, long seqid) {
        super(connection, mapid);
        seqId = seqid;
    }

    protected String getQueryString() {
        return "select id from current_relations_" + getMapId()
                + " where tags->'hoot:review:needs' = 'yes' and tags->'hoot:review:sort_order'='" + seqId + "'";
    }

    @Override
    public ReviewQueryMapper execQuery() throws Exception {
        ReviewableItem ret = null;
        try (Connection connection = getConnection()) {
            try (Statement stmt = connection.createStatement()) {
                try (ResultSet rs = stmt.executeQuery(getQueryString())) {
                    long nResCnt = 0;
                    long relid = -1;

                    while (rs.next()) {
                        // Retrieve by column name
                        relid = rs.getLong("id");
                        nResCnt++;
                    }

                    ret = new ReviewableItem(seqId, getMapId(), relid);
                    ret.setResultCount(nResCnt);
                }
            }
        }

        return ret;
    }
}
