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

import hoot.services.HootProperties;
import hoot.services.models.review.ReviewQueryMapper;
import hoot.services.models.review.ReviewableItem;


/**
 *
 */
public class RandomReviewableQuery extends ReviewableQueryBase implements IReviewableQuery {
    private static final Logger logger = LoggerFactory.getLogger(RandomReviewableQuery.class);

    public RandomReviewableQuery(Connection c, long mapid) {
        super(c, mapid);

        try {
            // TODO: Since this code will affect all subsequent calls to
            // random(), it is better moved to
            // a more centralized location. Given that this is the only class
            // using random() in a SQL
            // query so far, no harm is done for the time being.
            if (Boolean.parseBoolean(HootProperties.getPropertyOrDefault("seedRandomQueries"))) {
                double seed = Double.parseDouble(HootProperties.getPropertyOrDefault("randomQuerySeed"));

                if ((seed >= -1.0) && (seed <= 1.0)) {
                    try (Statement stmt = getConnection().createStatement()) {
                        // After executing this, all subsequent calls to
                        // random() will be seeded.
                        stmt.executeQuery("select setseed(" + seed + ");");
                    }
                }
            }
        }
        catch (Exception e) {
            logger.error("Unable to seed random review query.");
        }
    }

    @Override
    public ReviewQueryMapper execQuery() throws SQLException, Exception {
        ReviewableItem ret = new ReviewableItem(-1, getMapId(), -1);

        long relId = -1;
        String seqId = "-1";

        try (Statement stmt = getConnection().createStatement(); ResultSet rs = stmt.executeQuery(_getQueryString())) {
            long nResCnt = 0;

            if (rs == null) {
                throw new SQLException("Error executing ReviewQueryMapper::execQuery");
            }

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

        return ret;
    }

    protected String _getQueryString() {
        return "select id as relid, tags->'hoot:review:sort_order' as seq from current_relations_" + getMapId()
                + " where tags->'hoot:review:needs' = 'yes' order by random() limit 1";
    }
}
