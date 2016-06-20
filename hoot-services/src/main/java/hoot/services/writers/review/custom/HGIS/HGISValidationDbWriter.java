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
package hoot.services.writers.review.custom.HGIS;

import java.sql.Connection;
import java.sql.SQLException;
import java.util.HashMap;
import java.util.Map;

import hoot.services.db.DbUtils;
import hoot.services.exceptions.review.custom.HGIS.ReviewMapTagUpdateException;

//import org.slf4j.Logger;
//import org.slf4j.LoggerFactory;


public class HGISValidationDbWriter {
    // private static final Logger log =
    // LoggerFactory.getLogger(HGISValidationDbWriter.class);

    private Connection conn;
    private long mapId;

    public HGISValidationDbWriter(final Connection cn, final long mapid) {
        this.conn = cn;
        this.mapId = mapid;
    }

    public void updateMapTagWithReviewType() throws SQLException, ReviewMapTagUpdateException {
        Map<String, String> tags = new HashMap<>();
        tags.put("reviewtype", "hgisvalidation");
        final long resCnt = DbUtils.updateMapsTableTags(tags, this.mapId, conn);
        if (resCnt < 1) {
            throw new ReviewMapTagUpdateException("Failed to update maps table for mapid:" + this.mapId);
        }

    }
}
