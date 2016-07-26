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

import static hoot.services.HootProperties.RANDOM_QUERY_SEED;
import static hoot.services.HootProperties.SEED_RANDOM_QUERIES;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.models.review.ReviewQueryMapper;
import hoot.services.models.review.ReviewableItem;


class RandomReviewableQuery extends ReviewableQueryBase implements IReviewableQuery {
    private static final Logger logger = LoggerFactory.getLogger(RandomReviewableQuery.class);

    RandomReviewableQuery(Connection connection, long mapid) {
        super(connection, mapid);

        // TODO: Since this code will affect all subsequent calls to
        // random(), it is better moved to
        // a more centralized location. Given that this is the only class
        // using random() in a SQL query so far, no harm is done for the time being.
        if (Boolean.parseBoolean(SEED_RANDOM_QUERIES)) {
            double seed = Double.parseDouble(RANDOM_QUERY_SEED);

            if ((seed >= -1.0) && (seed <= 1.0)) {
                try (Statement stmt = super.getConnection().createStatement()) {
                    // After executing this, all subsequent calls to random() will be seeded.
                    try (ResultSet rs = stmt.executeQuery("select setseed(" + seed + ");")) {
                    }
                }
                catch (SQLException e) {
                    throw new RuntimeException("Error setting seeed!", e);
                }
            }
        }
    }

    @Override
    public ReviewQueryMapper execQuery() {
        ReviewableItem ret = new ReviewableItem(-1, getMapId(), -1);

        try (Connection connection = getConnection()){
            try (Statement stmt = connection.createStatement()) {
                try (ResultSet rs = stmt.executeQuery(getQueryString())) {
                    long nResCnt = 0;
                    long relId = -1;
                    String seqId = "-1";

                    while (rs.next()) {
                        relId = rs.getLong("relid");
                        seqId = rs.getString("seq");
                        nResCnt++;
                    }

                    ret.setRelationId(relId);
                    long nSeq = -1;
                    if (seqId != null) {
                        nSeq = Long.parseLong(seqId);
                    }

                    ret.setSortOrder(nSeq);
                    ret.setResultCount(nResCnt);
                }
            }
        }
        catch (SQLException e) {
            throw new RuntimeException("Error executing query!", e);
        }

        return ret;
    }

    String getQueryString() {
        return "select id as relid, tags->'hoot:review:sort_order' as seq from current_relations_" + getMapId()
                + " where tags->'hoot:review:needs' = 'yes' order by random() limit 1";
    }
}
