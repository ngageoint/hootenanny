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
package hoot.services.writers.review;

import java.sql.Connection;
import java.sql.Statement;
import java.util.HashMap;
import java.util.Map;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.models.osm.Changeset;


/**
 * Resolves reviews in the database
 */
public class ReviewResolver {
    private static final Logger logger = LoggerFactory.getLogger(ReviewResolver.class);

    private final Connection conn;

    public ReviewResolver(Connection conn) throws Exception {
        this.conn = conn;
    }

    /**
     * Resolves all reviews for a given map
     *
     * @param mapId
     *            ID of the map owning the review data
     * @param userId
     *            user ID associated with the review data
     * @return the ID of the changeset used to resolve the reviews
     * @throws Exception
     */
    public long setAllReviewsResolved(long mapId, long userId) throws Exception {
        // create a changeset
        Map<String, String> changesetTags = new HashMap<>();
        changesetTags.put("bot", "yes");
        changesetTags.put("created_by", "hootenanny");
        long changesetId = Changeset.createChangeset(mapId, userId, changesetTags, conn);
        Changeset.closeChangeset(mapId, changesetId, conn);

        /*
         * - mark all review relations belonging to the map as resolved - update
         * the changeset id for each review relation - increment the version for
         * each review relation
         */
        String sql = "";
        sql += "update current_relations_" + mapId;
        sql += " set tags = tags || hstore('hoot:review:needs', 'no'),";
        sql += " changeset_id = " + changesetId + ",";
        sql += " version = version + 1";
        sql += " where tags->'type' = 'review'";

        try (Statement stmt = conn.createStatement()) {
            int numRecordsUpdated = stmt.executeUpdate(sql);
            logger.debug("{} records updated.", numRecordsUpdated);
        }

        return changesetId;
    }
}
